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
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_PIN_0;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA,&GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_13 | GPIO_PIN_12 | GPIO_PIN_11 |GPIO_PIN_10 |GPIO_PIN_9 |GPIO_PIN_8;;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Pull = GPIO_PULLUP;

	HAL_GPIO_Init(GPIOE,&GPIO_InitStruct);
	for(;;)
	{
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_13,0);
			while(1)
			{
				//Change LED state with button press
				int cnt=0;
				if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)==1)
					for(int i=0;i<500;i++)
						if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)==1)
							cnt++;
				if(cnt>=250)
					HAL_GPIO_TogglePin(GPIOE,GPIO_PIN_13);
			}

	}
}
