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
	__HAL_RCC_USART2_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin			= GPIO_PIN_2 | GPIO_PIN_3 ;
	GPIO_InitStruct.Mode		= GPIO_MODE_AF_PP;
	GPIO_InitStruct.Speed		= GPIO_SPEED_FREQ_MEDIUM;
	GPIO_InitStruct.Pull		= GPIO_NOPULL;
	GPIO_InitStruct.Alternate	= GPIO_AF7_USART2;

	HAL_GPIO_Init(GPIOA,&GPIO_InitStruct);

	UART_HandleTypeDef UartHandle;
	UartHandle.Instance			 = USART2;
	UartHandle.Init.BaudRate     = 9600;
	UartHandle.Init.WordLength   = UART_WORDLENGTH_8B;
	UartHandle.Init.StopBits     = UART_STOPBITS_1;
	UartHandle.Init.Parity       = UART_PARITY_NONE;
	UartHandle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
	UartHandle.Init.Mode         = UART_MODE_TX_RX;
	UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;
	UartHandle.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

	HAL_UART_Init(&UartHandle);
	NVIC_EnableIRQ(USART2_IRQn);
	uint8_t data=30;

	HAL_UART_Transmit(&UartHandle,&data,0x100,0x1FFFFFF);
	BSP_LED_Init(LED9);
	BSP_LED_On(LED9);
	//HAL_UART_Receive(&UartHandle,&data,0x100,0x1FFFFFF);

	for(;;);
}
