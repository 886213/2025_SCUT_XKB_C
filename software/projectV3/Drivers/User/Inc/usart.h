#ifndef __USART_H
#define __USART_H

#include "stdio.h"
#include "sys.h"
#include "ANO_Protocol.h"

/*-------------------------------------------- USART���ú� ---------------------------------------*/

#define USART3_BaudRate 5000000
extern UART_HandleTypeDef huart3; // UART_HandleTypeDef �ṹ�����

#define USART3_TX_PIN GPIO_PIN_8                               // TX ����
#define USART3_TX_PORT GPIOD                                   // TX ���Ŷ˿�
#define GPIO_USART3_TX_CLK_ENABLE __HAL_RCC_GPIOD_CLK_ENABLE() // TX ����ʱ��

#define USART3_RX_PIN GPIO_PIN_9                               // RX ����
#define USART3_RX_PORT GPIOD                                   // RX ���Ŷ˿�
#define GPIO_USART3_RX_CLK_ENABLE __HAL_RCC_GPIOD_CLK_ENABLE() // RX ����ʱ��

/*---------------------------------------------- �������� ---------------------------------------*/

extern uint8_t rx_buffer;

void USART3_Init(void); // USART1��ʼ������

#endif //__USART_H
