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
long double map(long double x, long double in_min, long double in_max, long double out_min, long double out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
void pwminit()
{
	//structure for GPIO setup
	GPIO_InitTypeDef            GPIO_InitStructure;
	//structure for TIM Time Base
	TIM_TimeBaseInitTypeDef     TIM_TimeBaseStructure;
	//structure for TIM Output Compare
	TIM_OCInitTypeDef			TIM_OCInitStructure;

	//enable the AHB Peripheral Clock to use GPIOE
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	//enable the TIM1 and 3 clock

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	// Pin configuration of PWM
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;


	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1 | GPIO_Pin_4;

	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource1, GPIO_AF_2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_2);

	TIM_TimeBaseStructure.TIM_Period = 20000;
	TIM_TimeBaseStructure.TIM_Prescaler = 72;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);//For GPIOB 1,4

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 1500;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;


	TIM_OC1Init(TIM3, &TIM_OCInitStructure);
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);
	//enable the PWM output

	TIM_CtrlPWMOutputs(TIM3, ENABLE);
	TIM_Cmd(TIM3, ENABLE);

	TIM_SetCompare4(TIM3, 2500);
	TIM_SetCompare1(TIM3, 2500);

}

int main(void)
{
	int min=300,max=3000;
	pwminit();
	while(1)
	{
		for(int i=min;i<max;i++)
		{
			TIM_SetCompare1(TIM3, i);
			for(int i=0;i<200;i++)
				for(int i=0;i<100;i++);
		}
		for(int i=min;i<max;i++)
		{
			TIM_SetCompare4(TIM3, i);
			for(int i=0;i<200;i++)
				for(int i=0;i<100;i++);
		}
		for(int i=max;i>min;i--)
		{
			TIM_SetCompare4(TIM3, i);
			for(int i=0;i<200;i++)
				for(int i=0;i<100;i++);
		}
		for(int i=max;i>min;i--)
		{
			TIM_SetCompare1(TIM3, i);
			for(int i=0;i<200;i++)
				for(int i=0;i<100;i++);
		}

	}

}
