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

#define UART4_BaudRate 115200
extern UART_HandleTypeDef huart4; // UART_HandleTypeDef �ṹ�����

#define UART4_TX_PIN GPIO_PIN_12                               // TX ����
#define UART4_TX_PORT GPIOA                                   // TX ���Ŷ˿�
#define GPIO_UART4_TX_CLK_ENABLE __HAL_RCC_GPIOA_CLK_ENABLE() // TX ����ʱ��

#define UART4_RX_PIN GPIO_PIN_11                               // RX ����
#define UART4_RX_PORT GPIOA                                   // RX ���Ŷ˿�
#define GPIO_UART4_RX_CLK_ENABLE __HAL_RCC_GPIOA_CLK_ENABLE() // RX ����ʱ��

#define OPENMV_Packet_Head '@'
#define OPENMV_Packet_End  '#'

/*---------------------------------------------- �������� ---------------------------------------*/

typedef struct OPENMV_Dot
{
  uint16_t x;
  uint16_t y;
} OPENMV_Dot;
extern OPENMV_Dot Rx_RedDot, Rx_GreenDot;
extern uint8_t NearFlag;

extern uint8_t OPENMV_RxFlag;
extern uint8_t rx_buffer;

void USART3_Init(void);
void UART4_Init(void);

#endif //__USART_H
