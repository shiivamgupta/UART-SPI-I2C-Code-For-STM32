/*
=======================================================
Code for UART communication on STM32 Board
Author: Shivam Gupta
=======================================================
*/

#include "stm32f4xx.h"

void USART1_Init(void);
void USART1_SendChar(char ch);
void USART1_SendString(char* str);

int main(void)
{
    USART1_Init();  // Initialize USART1 peripheral

    while (1)
    {
        USART1_SendString("Hello World!\r\n");  // Send "Hello World!" string
    }
}

void USART1_Init(void)
{
    // Enable USART1 peripheral clock
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    // Configure USART1 TX and RX pins
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    // Configure USART1 peripheral
    USART_InitTypeDef USART_InitStruct;
    USART_InitStruct.USART_BaudRate = 9600;
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;
    USART_InitStruct.USART_StopBits = USART_StopBits_1;
    USART_InitStruct.USART_Parity = USART_Parity_No;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStruct);

    // Enable USART1 peripheral
    USART_Cmd(USART1, ENABLE);
}

void USART1_SendChar(char ch)
{
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);  // Wait for USART1 TX buffer empty
    USART_SendData(USART1, (uint16_t)ch);  // Send character
}

void USART1_SendString(char* str)
{
    while (*str)
    {
        USART1_SendChar(*str++);
    }
}
