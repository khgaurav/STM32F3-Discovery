
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


#define I2Cx_RCC        RCC_APB1Periph_I2C1
#define I2Cx            I2C1
#define I2C_GPIO_RCC    RCC_AHBPeriph_GPIOB
#define I2C_GPIO        GPIOB
#define I2C_PIN_SDA     GPIO_Pin_7
#define I2C_PIN_SCL     GPIO_Pin_6

#define SLAVE_ADDRESS    0x11

void i2c_init(void);
void i2c_start(void);
void i2c_stop(void);
void i2c_address_direction(uint8_t address, uint8_t direction);
void i2c_transmit(uint8_t byte);
uint8_t i2c_receive_ack(void);
uint8_t i2c_receive_nack(void);
void i2c_write(uint8_t address, uint8_t data);
void i2c_read(uint8_t address, uint8_t* data);

uint8_t receivedByte;
void DelayMs(int s)
{
	for(int i=0;i<s*138888;i++);
}
int main(void)
{



    // Initialize I2C
    i2c_init();

    while (1)
    {
        // Write 0x01 to slave (turn on LED blinking)
        i2c_write(SLAVE_ADDRESS, 0x01);
        DelayMs(5);
        // Read LED blinking status (off/on)
        i2c_read(SLAVE_ADDRESS, &receivedByte);
        // Display LED blinking status

        DelayMs(2500);

        // Write 0x00 to slave (turn off LED blinking)
        i2c_write(SLAVE_ADDRESS, 0x00);
        DelayMs(5);
        // Read LED blinking status (off/on)
        i2c_read(SLAVE_ADDRESS, &receivedByte);
        // Display LED blinking status

        DelayMs(2500);
    }
}

void i2c_init()
{
    // Initialization struct
    I2C_InitTypeDef I2C_InitStruct;
    GPIO_InitTypeDef GPIO_InitStruct;

    // Step 1: Initialize I2C
    RCC_APB1PeriphClockCmd(I2Cx_RCC, ENABLE);
    I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStruct.I2C_OwnAddress1 = 0x10;
    I2C_InitStruct.I2C_Ack = I2C_Ack_Disable;
    I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_Init(I2Cx, &I2C_InitStruct);
    I2C_Cmd(I2Cx, ENABLE);

    // Step 2: Initialize GPIO as open drain alternate function
    RCC_APB2PeriphClockCmd(I2C_GPIO_RCC, ENABLE);
    GPIO_InitStruct.GPIO_Pin = I2C_PIN_SCL | I2C_PIN_SDA;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(I2C_GPIO, &GPIO_InitStruct);
}

void i2c_start()
{
    // Wait until I2Cx is not busy anymore
    while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));

    // Generate start condition
    I2C_GenerateSTART(I2Cx, ENABLE);

    // Wait for I2C EV5.
    // It means that the start condition has been correctly released
    // on the I2C bus (the bus is free, no other devices is communicating))
    while (!I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));
}

void i2c_stop()
{
    // Generate I2C stop condition
    I2C_GenerateSTOP(I2Cx, ENABLE);
    // Wait until I2C stop condition is finished
    while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_STOPF));
}

void i2c_address_direction(uint8_t address, uint8_t direction)
{
    // Send slave address
    i2c_write(address, address);

    // Wait for I2C EV6
    // It means that a slave acknowledges his address
    if (direction == I2C_Direction_Transmitter)
    {
        while (!I2C_GetFlagStatus(I2Cx,I2C_FLAG_TXE));
    }
    else if (direction == I2C_Direction_Receiver)
    {
        while (!I2C_GetFlagStatus(I2Cx, I2C_FLAG_RXNE));
    }
}

void i2c_transmit(uint8_t byte)
{
    // Send data byte
    I2C_SendData(I2Cx, byte);
    // Wait for I2C EV8_2.
    // It means that the data has been physically shifted out and
    // output on the bus)
    while (!I2C_GetFlagStatus(I2Cx, I2C_FLAG_TXE));
}

uint8_t i2c_receive_ack()
{
    // Enable ACK of received data
    I2C_AcknowledgeConfig(I2Cx, ENABLE);
    // Wait for I2C EV7
    // It means that the data has been received in I2C data register
    while (!I2C_GetFlagStatus(I2Cx, I2C_FLAG_RXNE));

    // Read and return data byte from I2C data register
    return I2C_ReceiveData(I2Cx);
}

uint8_t i2c_receive_nack()
{
    // Disable ACK of received data
    I2C_AcknowledgeConfig(I2Cx, DISABLE);
    // Wait for I2C EV7
    // It means that the data has been received in I2C data register
    while (!I2C_GetFlagStatus(I2Cx, I2C_FLAG_RXNE));

    // Read and return data byte from I2C data register
    return I2C_ReceiveData(I2Cx);
}

void i2c_write(uint8_t address, uint8_t data)
{
    i2c_start();
    i2c_address_direction(address << 1, I2C_Direction_Transmitter);
    i2c_transmit(data);
    i2c_stop();
}

void i2c_read(uint8_t address, uint8_t* data)
{
    i2c_start();
    i2c_address_direction(address << 1, I2C_Direction_Receiver);
    *data = i2c_receive_nack();
    i2c_stop();
}
