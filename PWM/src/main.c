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
			
GPIO_InitTypeDef		GPIO_InitStructure;			//structure for GPIO setup
TIM_Base_InitTypeDef	TIM_TimeBaseStructure;		//structure for TIM Time Base
TIM_OC_InitTypeDef		TIM_OCInitStructure;		//structure for TIM Output Compare

void PWM_Init(void);
int main(void)
{
	PWM_Init();
	while(1)
	{
		for(int i=0;i<500;i++)
		{
			TIM_OCInitStructure.Pulse=i;
			for(int i=0;i <= 50000;i++);

		}

	}


}
void PWM_Init(void)
{


	__HAL_RCC_GPIOB_CLK_ENABLE();			//enable the AHB Peripheral Clock to use GPIOB
	__HAL_RCC_TIM4_CLK_ENABLE();			//enable the TIM4 clock

	/* Pin configuration */
	GPIO_InitStructure.Pin = GPIO_PIN_6;
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_MEDIUM;
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);


	TIM_TimeBaseStructure.Prescaler= 71;
	TIM_TimeBaseStructure.Period = 49;
	TIM_TimeBaseStructure.CounterMode = TIM_COUNTERMODE_UP;
	TIM_Base_SetConfig(TIM4, &TIM_TimeBaseStructure);

	TIM_HandleTypeDef TimHandle;
	TimHandle.Init=TIM_TimeBaseStructure;
	TimHandle.Instance = TIM4;

	HAL_TIM_PWM_Init(&TimHandle);

	TIM_OCInitStructure.OCMode = TIM_OCMODE_PWM1;
	TIM_OCInitStructure.OCPolarity = TIM_OCPOLARITY_HIGH;
	TIM_OCInitStructure.OCFastMode = TIM_OCFAST_DISABLE;
	TIM_OCInitStructure.Pulse=0;

	//HAL_TIM_PWM_ConfigChannel(&TimHandle, &TIM_OCInitStructure, TIM_CHANNEL_1);

	TIM_OC1_SetConfig(TIM4, &TIM_OCInitStructure);

	//HAL_TIM_PWM_Start(&TimHandle, TIM_CHANNEL_1);


}
