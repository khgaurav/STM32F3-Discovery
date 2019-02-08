/**
  **************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  **************************
*/


#include "stm32f30x.h"
#include "stm32f3_discovery.h"

long double map(long double x, long double in_min, long double in_max, long double out_min, long double out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int uartreceive()
{
	int cnt=0;
	//Waiting for register to update
	while(!USART_GetFlagStatus(UART4, USART_FLAG_RXNE))
	{
		cnt++;
			if(cnt>20)
				return ' ';
	}
	return USART_ReceiveData(UART4);
}
void uarttransmit(char data)
{
	USART_SendData(UART4,data);
	        while(USART_GetFlagStatus(UART4,USART_FLAG_TXE)==RESET)
	        {}
}
void seperate(int num)
{
	int i, rem, len = 0, n;

	    n = num;
	    while (n != 0)
	    {
	        len++;
	        n /= 10;
	    }

	    for (i = 0; i < 3; i++)
	    {
	        rem = num % 10;
	        num = num / 10;
	        uarttransmit(rem + '0');
	    }
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
	USART_InitStructure.USART_StopBits = USART_StopBits_2;
	USART_InitStructure.USART_Parity = USART_Parity_Even;
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


	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1 | GPIO_Pin_0;

	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource1, GPIO_AF_2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource0, GPIO_AF_2);

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


	TIM_OC3Init(TIM3, &TIM_OCInitStructure);
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);
	//enable the PWM output

	TIM_CtrlPWMOutputs(TIM3, ENABLE);
	TIM_Cmd(TIM3, ENABLE);

	TIM_SetCompare4(TIM3, 400);
	TIM_SetCompare1(TIM3, 400);

}

int main(void)
{
	int min=410,max=2490,angle=0;
	pwminit();
	UART_Init();
	while(1)
	{
	start:
		TIM_SetCompare4(TIM3, min);
		TIM_SetCompare3(TIM3, min);
		if(uartreceive()=='s')
		{
			for(int i=min;i<max;i++)
			{
				TIM_SetCompare3(TIM3, i);
				angle=map(i,min,max,0,64);
				for(int i=0;i<200;i++)
					//for(int i=0;i<50;i++)
						if(uartreceive()=='f')
						{
							uarttransmit(angle);
							goto start;
						}
			}
			for(int i=min;i<max;i++)
			{
				angle=63+map(i,min,max,0,64);
				TIM_SetCompare4(TIM3, i);
				for(int i=0;i<200;i++)
					//for(int i=0;i<50;i++)
						if(uartreceive()=='f')
						{
							uarttransmit(angle);
							goto start;
						}
			}
			for(int i=max;i>min;i--)
			{
				angle=63+map(i,min,max,0,64);
				TIM_SetCompare4(TIM3, i);
				for(int i=0;i<200;i++)
					//for(int i=0;i<50;i++)
						if(uartreceive()=='f')
						{
							uarttransmit(angle);
							goto start;
						}
			}
			for(int i=max;i>min;i--)
			{
				angle=map(i,min,max,0,64);
				TIM_SetCompare3(TIM3, i);
				for(int i=0;i<200;i++)
					//for(int i=0;i<50;i++)
						if(uartreceive()=='f')
						{
							uarttransmit(angle);
							goto start;
						}
			}
		}

	}

}
