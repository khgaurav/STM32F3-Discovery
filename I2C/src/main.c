/**
  ******************************************************************************
  * @file    main.
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f30x.h"
#include "stm32f3_discovery.h"
void i2c1_init(void)
{
	//activate clock for I2C1 and I2C2
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2,ENABLE);
	//config clock of I2C1 and I2C2
	RCC_I2CCLKConfig(RCC_I2C1CLK_SYSCLK);
	RCC_I2CCLKConfig(RCC_I2C2CLK_SYSCLK);
	GPIO_InitTypeDef GPIO_InitStructure;
	I2C_InitTypeDef I2C_InitStructure;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init(GPIOB,&GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_4);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_4);
    I2C_DeInit(I2C1);

    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_AnalogFilter =  I2C_AnalogFilter_Enable;
    I2C_InitStructure.I2C_DigitalFilter = 0x00;
    I2C_InitStructure.I2C_OwnAddress1 = 0x00;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_Timing = 0xA061192F;

    I2C_Init(I2C1,&I2C_InitStructure);
    I2C_Cmd(I2C1,ENABLE);
    I2C_AcknowledgeConfig(I2C1,ENABLE);
    I2C_Init(I2C1,&I2C_InitStructure);
    I2C_Cmd(I2C1,ENABLE);

}

uint8_t I2Cx_Write(I2C_TypeDef* I2Cx, uint8_t* buff, uint32_t nbyte, uint8_t SlaveAddr){

    /*--------Test on BUSY Flag-------------*/
        __IO uint32_t timeout = 0xFFFF;

        while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY)!=RESET)
        {
            if (timeout-- == 0) return 0;
        }

        I2C_TransferHandling(I2Cx,SlaveAddr,nbyte,I2C_AutoEnd_Mode,I2C_Generate_Start_Write);

        while(I2C_GetFlagStatus(I2Cx,I2C_FLAG_TXIS)== RESET)
        {
            if((timeout--) == 0) return 0;
        }

        while(nbyte){
        I2C_SendData(I2Cx, *buff++);
        GPIOE->ODR |= 1L << 11;

        while(I2C_GetFlagStatus(I2Cx,I2C_FLAG_TXE) == RESET);
        nbyte-- ;
        }

        I2C_GenerateSTOP(I2Cx, ENABLE);

        while(I2C_GetFlagStatus(I2Cx,I2C_FLAG_STOPF) == RESET)
        {
                if((timeout--) == 0) return 0;
        }

        return 1;

}
int main()
{
	i2c1_init();
	unsigned char data=3;
	while(1)
	{
		I2Cx_Write(I2C1, &data, 255, 0x11);
	}

}
