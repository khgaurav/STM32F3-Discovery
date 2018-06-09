/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f3xx.h"
#include "stm32f3_discovery.h"
			

int main(void)
{
	__HAL_RCC_GPIOE_CLK_ENABLE();
	volatile int i=0;
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_PIN_13;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Pull = GPIO_PULLUP;

	HAL_GPIO_Init(GPIOE,&GPIO_InitStruct);
	for(;;)
	{
		HAL_GPIO_TogglePin(GPIOE,GPIO_PIN_13);
		for ( i = 0; i < 1000000; ++i);//delay
		HAL_GPIO_TogglePin(GPIOE,GPIO_PIN_13); //set pin 13 low
		for ( i = 0; i < 1000000; ++i) ; //delay
	}
}
