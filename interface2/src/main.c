/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f30x.h"
#include "stm32f3_discovery.h"
			

#include <stm32f30x.h>
#include <stm32f30x_rcc.h>
#include <stm32f30x_gpio.h>

GPIO_InitTypeDef GPIO_InitStruct;

int main(void)
{
	unsigned int count=0;
	unsigned int display_count=0;
	unsigned int i=0;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE | RCC_AHBPeriph_GPIOA, ENABLE);

	//configuration for 8 leds connected to PORTE
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;	//which pins to setup, seperated by |
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOE, &GPIO_InitStruct);

	//configuration for user button connected to PA0
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_Write(GPIOE, 0x00);

    while(1)
    {
    	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)==1)				//is the pushbutton pressed (input = 0)
    	    {

    	       count = count + 1;         //increment count
    	       display_count = count << 7;
    	       for ( i = 0; i < 500000; ++i) ;

    	    }
    	GPIO_Write(GPIOE, display_count);  //display count on LEDs
    }
}
