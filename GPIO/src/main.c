#include <stm32f30x.h>
#include <stm32f30x_rcc.h>
#include <stm32f30x_gpio.h>

int main(void)
{
	int i=0;
	//Enable clock for GPIOE
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE,ENABLE);

	//SET GPIO PIN 13 as output
	GPIO_InitTypeDef GPIO_InitStruct;

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13| GPIO_Pin_15 |GPIO_Pin_12|GPIO_Pin_14 | GPIO_Pin_11 | GPIO_Pin_10 | GPIO_Pin_9 | GPIO_Pin_8 | GPIO_Pin_7 | GPIO_Pin_6 | GPIO_Pin_5 | GPIO_Pin_4 | GPIO_Pin_3 | GPIO_Pin_2 | GPIO_Pin_1 | GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;

	// Initialization of GPIO PORTE PIN 13
	GPIO_Init(GPIOE,&GPIO_InitStruct);

    while(1)
    {
    	GPIO_SetBits(GPIOE,GPIO_Pin_13|GPIO_Pin_13| GPIO_Pin_15 |GPIO_Pin_12|GPIO_Pin_14 | GPIO_Pin_11 | GPIO_Pin_10 | GPIO_Pin_9 | GPIO_Pin_8 | GPIO_Pin_7 | GPIO_Pin_6 | GPIO_Pin_5 | GPIO_Pin_4 | GPIO_Pin_3 | GPIO_Pin_2 | GPIO_Pin_1 | GPIO_Pin_0);


    }
}
