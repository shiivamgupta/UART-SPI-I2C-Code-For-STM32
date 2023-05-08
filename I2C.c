/*
=======================================================
Code for I2C communication on STM32 Board
Author: Shivam Gupta
=======================================================
*/
#include "stm32f4xx.h"

void I2C1_Init(void);
void I2C1_Write(uint8_t addr, uint8_t reg, uint8_t data);
uint8_t I2C1_Read(uint8_t addr, uint8_t reg);

int main(void)
{
    I2C1_Init();  // Initialize I2C1 peripheral

    while (1)
    {
        uint8_t data = 0xAA;
        I2C1_Write(0x50, 0x00, data);  // Write data to I2C slave device
        data = I2C1_Read(0x50, 0x00);  // Read data from I2C slave device

        // Process received data or perform other operations
    }
}

void I2C1_Init(void)
{
    // Enable I2C1 peripheral clock
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

    // Configure I2C1 pins
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_9;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    // Connect I2C1 pins to I2C peripheral
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_I2C1);  // SCL
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_I2C1);  // SDA

    // Configure I2C1 peripheral
    I2C_InitTypeDef I2C_InitStruct;
    I2C_InitStruct.I2C_ClockSpeed = 100000;
    I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStruct.I2C_OwnAddress1 = 0x00;
    I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_Init(I2C1, &I2C_InitStruct);

    // Enable I2C1 peripheral
    I2C_Cmd(I2C1, ENABLE);
}

void I2C1_Write(uint8_t addr, uint8_t reg, uint8_t data)
{
    // Generate start condition
    I2C_GenerateSTART(I2C1, ENABLE);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

    // Send device address with write bit
    I2C_Send7bitAddress(I2C1, addr << 1, I2C_Direction_Transmitter);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

    // Send register address
    I2C_SendData(I2C1, reg);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    // Send data
    I2C_SendData(I2C1, data);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    // Generate stop condition
    I2C_GenerateSTOP(I2C1, ENABLE);
}

uint8_t I2C1_Read(uint8_t addr, uint8_t reg)
{
    uint8_t data = 0;

    // Generate start condition
    I2C_GenerateSTART(I2C1, ENABLE);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

    // Send device address with write bit
    I2C_Send7bitAddress(I2C1, addr << 1, I2C_Direction_Transmitter);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

    // Send register address
    I2C_SendData(I2C1, reg);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    // Generate repeated start condition
    I2C_GenerateSTART(I2C1, ENABLE);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

    // Send device address with read bit
    I2C_Send7bitAddress(I2C1, addr << 1, I2C_Direction_Receiver);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

    // Disable ACK
    I2C_AcknowledgeConfig(I2C1, DISABLE);

    // Generate stop condition before receiving the last byte
    I2C_GenerateSTOP(I2C1, ENABLE);

    // Wait for data reception
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));

    // Read the data
    data = I2C_ReceiveData(I2C1);

    return data;
}
