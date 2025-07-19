#ifndef __USART_H
#define __USART_H

#include "stdio.h"
#include "sys.h"
#include "ANO_Protocol.h"

/*-------------------------------------------- USART配置宏 ---------------------------------------*/

#define USART3_BaudRate 5000000
extern UART_HandleTypeDef huart3; // UART_HandleTypeDef 结构体变量

#define USART3_TX_PIN GPIO_PIN_8                               // TX 引脚
#define USART3_TX_PORT GPIOD                                   // TX 引脚端口
#define GPIO_USART3_TX_CLK_ENABLE __HAL_RCC_GPIOD_CLK_ENABLE() // TX 引脚时钟

#define USART3_RX_PIN GPIO_PIN_9                               // RX 引脚
#define USART3_RX_PORT GPIOD                                   // RX 引脚端口
#define GPIO_USART3_RX_CLK_ENABLE __HAL_RCC_GPIOD_CLK_ENABLE() // RX 引脚时钟

/*---------------------------------------------- 函数声明 ---------------------------------------*/

extern uint8_t rx_buffer;

void USART3_Init(void); // USART1初始化函数

#endif //__USART_H
