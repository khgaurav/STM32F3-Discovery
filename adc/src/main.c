#include "stm32f30x.h"
#include "stm32f3_discovery.h"
#include "math.h"
#define ARRAY_SIZE 32

/**
 * Classic delay
 */
void badprogDelay(u32 myTime) {
    u32 i;
    RCC_ClocksTypeDef RCC_Clocks;

    RCC_GetClocksFreq(&RCC_Clocks);
    // i = myTime * 68
    i = myTime * (RCC_Clocks.SYSCLK_Frequency >> 20);

    for (; i != 0; i--)
        ;
}

/**
 * Initialization of ADC and the poentiomenter's GPIO
 */
float badprogADC() {
    /// declaring GPIO stuff
    GPIO_InitTypeDef GPIO_InitStructure;
    // declaring ADC struct
    ADC_InitTypeDef ADC_InitStructure;

    // deinit ADC
    ADC_DeInit(ADC1);

    // enabling clock
    RCC_APB2PeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
    // enabling ADC clock
    RCC_APB2PeriphClockCmd(RCC_AHBPeriph_ADC12, ENABLE);

    // C - Init the GPIO with the structure - Testing ADC
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    // init ADC struct
    ADC_StructInit(&ADC_InitStructure);

    // setting the ADC struct
    ADC_InitStructure.ADC_AutoInjMode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 1;

    // init adc
    ADC_Init(ADC1, &ADC_InitStructure);

    // enable ADC
    ADC_Cmd(ADC1, ENABLE);

    // start ADC1 calibration and check the end
    ADC_StartCalibration(ADC1);

    // configure ADC_IN14
    ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 1, ADC_SampleTime_7Cycles5);

    // start ADC -> retrieve the ADC value from the potentiometer
    // and add it into ADC1->DR with:
    // ADCx->CR2 |= CR2_EXTTRIG_SWSTART_Set;
    // this without using ADC1->DR o_O
    // CR2 = Configuration Register2 -> it seems to be a config with
    // a binary number -> for example 1000010100101010 which sets all
    // registers with default values
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);

    // check the end of the ADC1 calibration
    // setting ADC1->DR with:
    // if ((ADCx->CR2 & CR2_CAL_Set) != (uint32_t)RESET)
    while (ADC_GetCalibrationStatus(ADC1) == SET)
        ;

    // convert valueADC to valueVolt -> valueADC * (MAX VOLT / 2^12)
    // and also :
    // ADC_SoftwareStartConvCmdsoftwareS,
    // ADC_GetCalibrationStatus
    // with
    // return (uint16_t) ADCx->DR;
    uint16_t valueADC = ADC_GetConversionValue(ADC1);

    // convert the "uint_16 valueADC" into a "float valueVolt"
    // Volt = 3.3
    // ADC = 12 bits, so 2^12 = 4096
    float valueVolt = valueADC * (3.3 / 4095);

    return valueVolt;
}

/**
 * Init the array with '\0';
 */
void badprogInitArray(float *myArray, int size) {
    int i = 0;
    while (i < size) {
        myArray[i] = '\0';
        ++i;
    }
}

/*******************************************************************************
 * Main, what else?
 *******************************************************************************/
int main(void) {
    float theArray[ARRAY_SIZE];
    int i = 0;

    // init with 0 the whole array
    badprogInitArray(theArray, ARRAY_SIZE);

    // our beautiful infinite loop while i < 32
    while (-2013) {
        if (i < ARRAY_SIZE) {
            theArray[i] = badprogADC();
            badprogDelay(100000);                          // 1 sec
            ++i;
        }
    }
    return 0;
}
