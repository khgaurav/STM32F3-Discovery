#include "stm32f30x.h"
#include "stm32f3_discovery.h"
#include "math.h"
int min=410,max=2490;
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
void uarttransmit(char data)
{
	USART_SendData(UART4,data);
	        while(USART_GetFlagStatus(UART4,USART_FLAG_TXE)==RESET);


}


//Map function
long double map(long double x, long double in_min, long double in_max, long double out_min, long double out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

//Converts Square Coordinates to Circular
void ellipticalSquareToDisc(long double x, long double y, long double *u, long double *v)
{
	*u = x * sqrt(1.0 - y*y/2.0);
	*v = y * sqrt(1.0 - x*x/2.0);
}

//Converts Circular Coordinates to Square
void ellipticalDiscToSquare(long double u, long double v, long double *x, long double *y)
{
	long double u2 = u * u;
	long double v2 = v * v;
	long double twosqrt2 = 2.0 * sqrt(2.0);
	long double subtermx = 2.0 + u2 - v2;
	long double subtermy = 2.0 - u2 + v2;
	long double termx1 = subtermx + u * twosqrt2;
	long double termx2 = subtermx - u * twosqrt2;
	long double termy1 = subtermy + v * twosqrt2;
	long double termy2 = subtermy - v * twosqrt2;
	*x = 0.5 * sqrt(termx1) - 0.5 * sqrt(termx2);
	*y = 0.5 * sqrt(termy1) - 0.5 * sqrt(termy2);
}

void gpioinit()
{
	//Enable clock for GPIOE
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE,ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD, ENABLE);
	//SET GPIO PIN 10, 12, 13 as output pins
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_15|GPIO_Pin_14|GPIO_Pin_13| GPIO_Pin_12| GPIO_Pin_10 |GPIO_Pin_11 | GPIO_Pin_8;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;

	// Initialization of GPIO PORT E Pin 10, 13 and Pin 12
	GPIO_Init(GPIOE,&GPIO_InitStruct);

		GPIO_ResetBits(GPIOE,GPIO_Pin_15|GPIO_Pin_14|GPIO_Pin_13 | GPIO_Pin_12 | GPIO_Pin_10 |GPIO_Pin_11 | GPIO_Pin_8);


	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_2;
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;

		// Initialization of GPIO PORT E Pin 10, 13 and Pin 12
		GPIO_Init(GPIOD,&GPIO_InitStruct);
		GPIO_ResetBits(GPIOD,GPIO_Pin_5|GPIO_Pin_2);


}
void gpioinit1()
{
	//Enable clock for GPIOA
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);

	//SET GPIO PIN 0-11 as output pins
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_10 | GPIO_Pin_9 | GPIO_Pin_8 | GPIO_Pin_7 | GPIO_Pin_6 | GPIO_Pin_5 | GPIO_Pin_4 | GPIO_Pin_3 | GPIO_Pin_2 | GPIO_Pin_1 | GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;

	// Initialization of GPIO PORT A Pin 0-11
	GPIO_Init(GPIOA,&GPIO_InitStruct);



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
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
	//enable the TIM1 and 3 clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	// Pin configuration of PWM
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_9 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	GPIO_Init(GPIOE, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;

	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOE, GPIO_PinSource9, GPIO_AF_2);//Right front
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource11, GPIO_AF_2);//Left front
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_2);//Right back
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_2);//Left back

	TIM_TimeBaseStructure.TIM_Period = 4800-1;
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);//For GPIOE 9,11
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);//For GPIOE 6,7

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 4799;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	TIM_OC1Init(TIM1, &TIM_OCInitStructure);
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);
	//enable the PWM output
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
	TIM_Cmd(TIM1, ENABLE);

	TIM_SetCompare1(TIM1, 0);
	TIM_SetCompare2(TIM1, 0);

	TIM_CtrlPWMOutputs(TIM3, ENABLE);
	TIM_Cmd(TIM3, ENABLE);

	TIM_SetCompare1(TIM3, 0);
	TIM_SetCompare2(TIM3, 0);

}
void pwminitservo()
{
	//structure for GPIO setup
	GPIO_InitTypeDef            GPIO_InitStructure;
	//structure for TIM Time Base
	TIM_TimeBaseInitTypeDef     TIM_TimeBaseStructure;
	//structure for TIM Output Compare
	TIM_OCInitTypeDef			TIM_OCInitStructure;

	//enable the AHB Peripheral Clock to use GPIOE
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD, ENABLE);
	//enable the TIM1 and 3 clock

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	// Pin configuration of PWM
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;


	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4 | GPIO_Pin_6 | GPIO_Pin_7;

	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOD, GPIO_PinSource4, GPIO_AF_2);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource7, GPIO_AF_2);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_2);

	TIM_TimeBaseStructure.TIM_Period = 20000;
	TIM_TimeBaseStructure.TIM_Prescaler = 72;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 1500;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	TIM_OC2Init(TIM2, &TIM_OCInitStructure);
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);
	TIM_OC4Init(TIM2, &TIM_OCInitStructure);
	//enable the PWM output

	TIM_CtrlPWMOutputs(TIM2, ENABLE);
	TIM_Cmd(TIM2, ENABLE);

	TIM_SetCompare4(TIM2, 400);
	TIM_SetCompare3(TIM2, 400);
	TIM_SetCompare2(TIM2, 400);

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
void shut()
{


	GPIO_ResetBits(GPIOE,GPIO_Pin_15|GPIO_Pin_14|GPIO_Pin_13 | GPIO_Pin_12 | GPIO_Pin_10|GPIO_Pin_11 | GPIO_Pin_8);
	GPIO_ResetBits(GPIOA,GPIO_Pin_11 | GPIO_Pin_10 | GPIO_Pin_9 | GPIO_Pin_8 | GPIO_Pin_7 | GPIO_Pin_6 | GPIO_Pin_5 | GPIO_Pin_4 | GPIO_Pin_3 | GPIO_Pin_2 | GPIO_Pin_1 | GPIO_Pin_0);	GPIO_ResetBits(GPIOB,GPIO_Pin_1 | GPIO_Pin_3 | GPIO_Pin_5 | GPIO_Pin_8);
	GPIO_ResetBits(GPIOD,GPIO_Pin_5);
	TIM_SetCompare1(TIM1, 0);
	TIM_SetCompare2(TIM1, 0);
	TIM_SetCompare1(TIM3, 0);
	TIM_SetCompare2(TIM3, 0);

}

void Delay(int time)
{
	volatile int i,j;

	time = time*10;
	for (i=0;i<time;i++)
		j++;
}

void motorcode(long double x, long double y,long double gear,char n)
{
	//Converting to Normal Coordinates
	x = x - 4999.5;
	y = 4999.5 - y;

	//Buffer for X-axis and Y-axis
	if(y/x>=12.8&&y/x<=-12.8)
	x=0;
	if(y/x<=0.390625&&y/x>=-0.390625)
	y=0;

	x=map(x,-4999.5,4999.5,-1.0,1.0);
	y=map(y,-4999.5,4999.5,-1.0,1.0);

	long double xans,yans;

	ellipticalSquareToDisc(x, y, &xans, &yans);

	x = (xans * 0.707) + (yans * 0.707);
	y = (-xans * 0.707) + (yans * 0.707);

	ellipticalDiscToSquare(x, y, &xans, &yans);


	x=(int)map(xans,-0.991273,0.991273,-4799,4799);
	y=(int)map(yans,-0.991273,0.991273,-4799,4799);

	if(x>4500)
			x=4799;
	if(y>4500)
			y=4799;

	//Buffer for (0,0)
	if( x < 300	&&	x > -300 )
		x=0;

	if( y < 300	&&	y > -300 )
		y=0;
	x*=gear/9;
	y*=gear/9;
	if(x>0)
	{
		GPIO_SetBits(GPIOE,GPIO_Pin_13);
		TIM_SetCompare1(TIM1, x);
		TIM_SetCompare1(TIM3, x);
	}
	else
	{
		GPIO_ResetBits(GPIOE,GPIO_Pin_13);
		TIM_SetCompare1(TIM1, -x);
		TIM_SetCompare1(TIM3, -x);
	}

	if(y>0)
	{
		GPIO_SetBits(GPIOE,GPIO_Pin_12);
		TIM_SetCompare2(TIM1, y);
		TIM_SetCompare2(TIM3, y);
	}
	else
	{
		GPIO_ResetBits(GPIOE,GPIO_Pin_12);
		TIM_SetCompare2(TIM1, -y);
		TIM_SetCompare2(TIM3, -y);
	}

}
void armcode(char link)
{
	if(link=='A')
				{

				GPIO_SetBits(GPIOD,GPIO_Pin_2); // swivel close
				GPIO_SetBits(GPIOD,GPIO_Pin_5);
				}
			else if(link=='B')
				{
					GPIO_SetBits(GPIOD,GPIO_Pin_2); // swivel open
					GPIO_ResetBits(GPIOD,GPIO_Pin_5);
				}
			else if(link=='C')
				{
					GPIO_SetBits(GPIOA,GPIO_Pin_8); //linear act. f
					GPIO_SetBits(GPIOA,GPIO_Pin_9);
				}
			else if(link=='D')
				{
					GPIO_SetBits(GPIOA,GPIO_Pin_8); /// linear act. b
					GPIO_ResetBits(GPIOA,GPIO_Pin_9);
				}
			else if(link=='E')
				{

				GPIO_SetBits(GPIOE,GPIO_Pin_15); //roll b
					GPIO_SetBits(GPIOE,GPIO_Pin_14);

			         GPIO_SetBits(GPIOA,GPIO_Pin_4);
					 GPIO_ResetBits(GPIOA,GPIO_Pin_5);

				}
			else if(link=='F')
				{

					GPIO_SetBits(GPIOE,GPIO_Pin_15); //roll f
						GPIO_ResetBits(GPIOE,GPIO_Pin_14);

				         GPIO_SetBits(GPIOA,GPIO_Pin_4);
						 GPIO_SetBits(GPIOA,GPIO_Pin_5);
				}


			else if(link=='G')
				{ //gripper
				GPIO_SetBits(GPIOA,GPIO_Pin_6);
			 GPIO_SetBits(GPIOA,GPIO_Pin_7);


				}
			else if(link=='H')
				{
				GPIO_SetBits(GPIOA,GPIO_Pin_6);
				 GPIO_ResetBits(GPIOA,GPIO_Pin_7);
				}
			else if(link=='I')
				{
				GPIO_SetBits(GPIOE,GPIO_Pin_15); //PITCH b
					GPIO_SetBits(GPIOE,GPIO_Pin_14);

			         GPIO_SetBits(GPIOA,GPIO_Pin_4);
					 GPIO_SetBits(GPIOA,GPIO_Pin_5);

				}
			else if(link=='J')
				{

				GPIO_SetBits(GPIOE,GPIO_Pin_15); //PITCH b
					GPIO_ResetBits(GPIOE,GPIO_Pin_14);

			         GPIO_SetBits(GPIOA,GPIO_Pin_4);
					 GPIO_ResetBits(GPIOA,GPIO_Pin_5);

				}


			else if(link=='K')
					{//link 2 linear act.
						GPIO_SetBits(GPIOA,GPIO_Pin_0);
						GPIO_SetBits(GPIOA,GPIO_Pin_1);
					}
				else if(link=='L')
					{
						GPIO_SetBits(GPIOA,GPIO_Pin_0);
						GPIO_ResetBits(GPIOA,GPIO_Pin_1);
					}
			/*else if(link=='K')
				{
					GPIO_SetBits(GPIOA,GPIO_Pin_10);
					GPIO_SetBits(GPIOA,GPIO_Pin_11);
				}
			else if(link=='L')
				{
					GPIO_SetBits(GPIOA,GPIO_Pin_10);
					GPIO_ResetBits(GPIOA,GPIO_Pin_11);
				}*/
			else if(link=='M')

			shut();

}

void servo(void)
{

		int angle=0;
		TIM_SetCompare4(TIM2, min);
		TIM_SetCompare3(TIM2, min);
			for(int i=min;i<max;i+=2)
			{
				TIM_SetCompare3(TIM2, i);
				angle=map(i,min,max,0,63);
				//for(int i=0;i<2;i++)
					//for(int i=0;i<50;i++)
					{
						char ch=uartreceive();
						if(ch=='f')

							{
								uarttransmit(angle);
								GPIO_SetBits(GPIOD,GPIO_Pin_2);
								goto start;
							}
						else if(ch=='m')
							goto start;
					}
			}
			for(int i=min;i<max;i+=2)
			{
				angle=63+map(i,min,max,0,63);
				TIM_SetCompare4(TIM2, i);
				//for(int i=0;i<2;i++)
					//for(int i=0;i<50;i++)
				{
					char ch=uartreceive();
					if(ch=='f')

						{
							uarttransmit(angle);
							GPIO_SetBits(GPIOD,GPIO_Pin_2);
							goto start;
						}
					else if(ch=='m')
						goto start;
				}
			}
			for(int i=max;i>min;i-=2)
			{
				angle=63+map(i,min,max,0,63);
				TIM_SetCompare4(TIM2, i);
				//for(int i=0;i<2;i++)
					//for(int i=0;i<50;i++)
				{
					char ch=uartreceive();
					if(ch=='f')

						{
							uarttransmit(angle);
							GPIO_SetBits(GPIOD,GPIO_Pin_2);
							goto start;
						}
					else if(ch=='m')
						goto start;
				}
			}
			for(int i=max;i>min;i-=2)
			{
				angle=map(i,min,max,0,63);
				TIM_SetCompare3(TIM2, i);
				//for(int i=0;i<2;i++)
					//for(int i=0;i<50;i++)
				{
					char ch=uartreceive();
					if(ch=='f')

						{
							uarttransmit(angle);
							GPIO_SetBits(GPIOD,GPIO_Pin_2);

							goto start;
						}
					else if(ch=='m')
							goto start;
				}
			}
			uarttransmit(127);
			start:
			for(int i=0;i<6000;i++)
				for(int j=0;j<5000;j++);
			GPIO_ResetBits(GPIOD,GPIO_Pin_2);
			TIM_SetCompare4(TIM2, min);
			TIM_SetCompare3(TIM2, min);

		}

int main(void)
{
	uint32_t x = 0, y = 0, gear = 0;
	char c = 'k';
	long cnt=0;

	gpioinit();
	gpioinit1();
	pwminit();
	pwminitservo();
	UART_Init();

	shut();
	int angle=min,a=52;
	TIM_SetCompare2(TIM2, angle);
	GPIO_ResetBits(GPIOD,GPIO_Pin_2);

	while(1)
	{
		if(cnt>200)
			shut();
		TIM_SetCompare2(TIM2, (int)angle);
		//uarttransmit(map(angle,410,2490,0,127));
		char d=uartreceive();
		if(d=='m')

		{
			cnt=0;
			GPIO_SetBits(GPIOE, GPIO_Pin_10);
			motorcode(x,y,gear,c);
			//uarttransmit('.');
			gear=uartreceive()-'0';
			if(uartreceive()=='x')
					{
						x=(uartreceive()-'0')*1000+(uartreceive()-'0')*100+(uartreceive()-'0')*10+(uartreceive()-'0');
					}
			else

					{
						//shut();
						//continue;
					}
			if(uartreceive()=='y')
					{
						y=(uartreceive()-'0')*1000+(uartreceive()-'0')*100+(uartreceive()-'0')*10+(uartreceive()-'0');
						c=uartreceive();
					}
					{
						//shut();
						//continue;
					}
			//camera(c);
			//motorcode(x,y,gear,c);
		//	camera(c);

			}
		else if(d=='f')
				{
					GPIO_SetBits(GPIOD,GPIO_Pin_2);
					for(int i=0;i<6000;i++)
						for(int j=0;j<5000;j++);
					GPIO_ResetBits(GPIOD,GPIO_Pin_2);
				}

		else if(d==' ')
		{
			cnt++;

		}

		angle+=a;
		char send='a';

		int points=5;
		int diff=(max-min)/points;
		if(angle>=max)
		{
			uarttransmit(send+points-1);
			a=-a;
		}
		else if(angle<=min)
		{
			uarttransmit(send);
			a=-a;
		}
		else if(angle==min+diff)
			uarttransmit(send+1);
		else if(angle==min+2*diff)
			uarttransmit(send+2);
		else if(angle==min+3*diff)
			uarttransmit(send+3);
		}

	}

