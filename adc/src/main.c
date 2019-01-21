#include "stm32f30x.h"
#include "stm32f30x_rcc.h"
#include "stm32f30x_adc.h"
#include <stm32f30x_gpio.h>

#define LED (GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15)

uint16_t convertedValue= 0;

void delay_ms (int time)
{
    int i;
    for (i = 0; i < time * 4000; i++) {}
}
int uartreceive()
{
	int cnt=0;
	//Waiting for register to update
	while(!USART_GetFlagStatus(UART4, USART_FLAG_RXNE))
	{
		cnt++;
		if(cnt>20000)
			return ' ';
	}
	return USART_ReceiveData(UART4);
}
void uarttransmit(uint16_t data)
{
	//Waiting for register to update
	while(!USART_GetFlagStatus(UART4, USART_FLAG_TXE));
	USART_SendData(UART4,data);
}




void UART_Init()
{

	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	// Enable GPIO clock
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);

	// Enable UART clock
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);

	/* UART configuration */
	USART_InitStructure.USART_BaudRate = 38400;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	/* UART4 configured as follow:
					- BaudRate = speed parameter above
					- Word Length = 8 Bits
					- One Stop Bit
					- No parity
					- Hardware flow control disabled (RTS and CTS signals)
					- Receive and transmit enabled
	*/
	USART_Init(UART4, &USART_InitStructure);

	// Configure UART Tx as alternate function push-pull
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	// Configure UART Rx as alternate function push-pull
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	// Connect PC10 to UART4_Tx
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_5);

	// Connect PC11 to UART4_Rx
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_5);

	// Enable UART
	USART_Cmd(UART4, ENABLE);
}

void Configure_ADC(void)
{
    RCC_ADCCLKConfig(RCC_ADC12PLLCLK_Div2);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_ADC12,ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA,&GPIO_InitStructure);

    ADC_DeInit(ADC1);
    ADC_InitTypeDef ADC_InitStructure;
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_10b;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_ExternalTrigConvEvent = ADC_ExternalTrigConvEvent_0;
    ADC_InitStructure.ADC_NbrOfRegChannel = 1;
    ADC_InitStructure.ADC_ExternalTrigEventEdge = ADC_ExternalTrigEventEdge_None;
    ADC_InitStructure.ADC_ContinuousConvMode = ADC_ContinuousConvMode_Enable;
    ADC_Init(ADC1,&ADC_InitStructure);
    ADC_Cmd(ADC1, ENABLE);
}

uint16_t Read_ADC()
{
    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_RDY));
    ADC_RegularChannelConfig(ADC1,ADC_Channel_2, 1,    ADC_SampleTime_1Cycles5);
    ADC_StartConversion(ADC1);
    while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC)==RESET);

    return ADC_GetConversionValue(ADC1);
}

int main()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, ENABLE);
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Pin  = LED;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
    Configure_ADC();
    uint32_t led = 0;


    while(1)
      {

    	convertedValue = Read_ADC();//Read the ADC converted value
    	          //GPIO_SetBits(GPIOE, 1 << led); //turn on a diode
    	          delay_ms(150); // wait
    	          //GPIO_ResetBits(GPIOE, 1 << led); //turn off a diode
    	          uarttransmit(convertedValue);
    	          if (convertedValue<30)
    	              {
    	        	  GPIO_SetBits(GPIOE, GPIO_Pin_8);
    	              }
    	      }
}
