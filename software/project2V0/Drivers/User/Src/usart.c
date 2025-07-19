/***
  ************************************************************************************************
  *	@file  	usart.c
  *	@version V1.0
  *  @date    2020-5-18
  *	@author  ���ͿƼ�
  *	@brief   usart��غ���
   ************************************************************************************************
   *  @description
  *
  *	ʵ��ƽ̨������STM32H750VBT6���İ� ���ͺţ�FK750M1-VBT6��
  *	�Ա���ַ��https://shop212360197.taobao.com
  *	QQ����Ⱥ��536665479
  *
>>>>> �ļ�˵����
  *
  *  ��ʼ��usart���ţ����ò����ʵȲ���
  *
  ************************************************************************************************
***/

#include "usart.h"
#include "stm32h7xx_hal.h"
#include <string.h>
#include <stdlib.h>

UART_HandleTypeDef huart3, huart4; // UART_HandleTypeDef �ṹ�����
uint8_t rx_buffer;                 // ���ջ�����

uint8_t openmvRxData;
uint8_t OPENMV_RxFlag = 0;
char OPENMV_RxPacket[50] = {0};
OPENMV_Dot Rx_RedDot, Rx_GreenDot;
uint8_t NearFlag = 0;

/*************************************************************************************************
 *	�� �� ��:	HAL_UART_MspInit
 *	��ڲ���:	huart - UART_HandleTypeDef����ı���������ʾ����Ĵ���
 *	�� �� ֵ:	��
 *	��������:	��ʼ����������
 *	˵    ��:	��
 *************************************************************************************************/

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  if (huart->Instance == USART3)
  {
    __HAL_RCC_USART3_CLK_ENABLE(); // ���� USART1 ʱ��

    GPIO_USART3_TX_CLK_ENABLE; // ���� USART1 TX ���ŵ� GPIO ʱ��
    GPIO_USART3_RX_CLK_ENABLE; // ���� USART1 RX ���ŵ� GPIO ʱ��

    GPIO_InitStruct.Pin = USART3_TX_PIN;               // TX����
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;            // �����������
    GPIO_InitStruct.Pull = GPIO_PULLUP;                // ����
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH; // �ٶȵȼ�
    GPIO_InitStruct.Alternate = GPIO_AF7_USART3;       // ����ΪUSART1
    HAL_GPIO_Init(USART3_TX_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = USART3_RX_PIN; // RX����
    HAL_GPIO_Init(USART3_RX_PORT, &GPIO_InitStruct);
  }
  else if (huart->Instance == UART4)
  {
    __HAL_RCC_UART4_CLK_ENABLE(); // ���� USART1 ʱ��

    GPIO_UART4_TX_CLK_ENABLE; // ���� USART1 TX ���ŵ� GPIO ʱ��
    GPIO_UART4_RX_CLK_ENABLE; // ���� USART1 RX ���ŵ� GPIO ʱ��

    GPIO_InitStruct.Pin = UART4_TX_PIN;                // TX����
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;            // �����������
    GPIO_InitStruct.Pull = GPIO_PULLUP;                // ����
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH; // �ٶȵȼ�
    GPIO_InitStruct.Alternate = GPIO_AF6_UART4;        // ����ΪUSART1
    HAL_GPIO_Init(UART4_TX_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = UART4_RX_PIN; // RX����
    HAL_GPIO_Init(UART4_RX_PORT, &GPIO_InitStruct);
  }
}

/*************************************************************************************************
 *	�� �� ��:	USART1_Init
 *	��ڲ���:	��
 *	�� �� ֵ:	��
 *	��������:	��ʼ����������
 *	˵    ��:	��
 *************************************************************************************************/

void USART3_Init(void)
{
  huart3.Instance = USART3;
  huart3.Init.BaudRate = USART3_BaudRate;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart3, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart3, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
  }
  if (HAL_UARTEx_DisableFifoMode(&huart3) != HAL_OK)
  {
  }
  // ����USART1�ж�
  HAL_NVIC_SetPriority(USART3_IRQn, 1, 1);
  HAL_NVIC_EnableIRQ(USART3_IRQn);
  HAL_UART_Receive_IT(&huart3, &rx_buffer, 1);
}

void UART4_Init(void)
{
  huart4.Instance = UART4;
  huart4.Init.BaudRate = UART4_BaudRate;
  huart4.Init.WordLength = UART_WORDLENGTH_8B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_NONE;
  huart4.Init.Mode = UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
  huart4.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart4.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart4.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

  if (HAL_UART_Init(&huart4) != HAL_OK)
  {
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart4, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart4, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
  }
  if (HAL_UARTEx_DisableFifoMode(&huart4) != HAL_OK)
  {
  }
  // ����USART1�ж�
  HAL_NVIC_SetPriority(UART4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(UART4_IRQn);
  HAL_UART_Receive_IT(&huart4, &openmvRxData, 1);
}

/*************************************************************************************************
 *	�� �� ��:	fputc
 *	��ڲ���:	ch - Ҫ������ַ� ��  f - �ļ�ָ�루�����ò�����
 *	�� �� ֵ:	����ʱ�����ַ�������ʱ���� EOF��-1��
 *	��������:	�ض��� fputc ������Ŀ����ʹ�� printf ����
 *	˵    ��:	��
 *************************************************************************************************/

int fputc(int ch, FILE *f)
{
  HAL_UART_Transmit(&huart3, (uint8_t *)&ch, 1, 100); // ���͵��ֽ�����
  return (ch);
}

void parseData(void)
{
  char *token = strtok(OPENMV_RxPacket, ","); // �״ε��ã��ָ��һ������
  if (token)
    Rx_RedDot.x = (uint16_t)atoi(token);

  token = strtok(NULL, ","); // �������ã������ָ�
  if (token)
    Rx_RedDot.y = (uint16_t)atoi(token);

  token = strtok(NULL, ",");
  if (token)
    Rx_GreenDot.x = (uint16_t)atoi(token);

  token = strtok(NULL, ",");
  if (token)
    Rx_GreenDot.y = (uint16_t)atoi(token);

  token = strtok(NULL, ",");
  if (token)
    NearFlag = (uint8_t)atoi(token);

  for(uint8_t i = 0; i < 50; i++)
  {
    OPENMV_RxPacket[i] = 0;
  }
}

void OPENMV_RX_Process(uint8_t rxdata)
{
  static uint8_t OPENMV_RxState = 0;
  static uint8_t OPENMV_pRxPacket = 0;

  uint8_t RxData = rxdata;
  if (OPENMV_RxState == 0)
  {
    if (RxData == OPENMV_Packet_Head)
    {
      OPENMV_RxState = 1;
      OPENMV_pRxPacket = 0;
    }
  }
  else if (OPENMV_RxState == 1)
  {
    if (RxData == OPENMV_Packet_End)
    {
      OPENMV_RxState = 0;
      parseData();
      OPENMV_RxFlag = 1;
    }
    else
    {
      OPENMV_RxPacket[OPENMV_pRxPacket] = rxdata;
      OPENMV_pRxPacket++;
    }
  }
}

/*************************************************************************************************
 *	�� �� ��:	HAL_UART_RxCpltCallback
 *	��ڲ���:	huart - UART_HandleTypeDef����ı���������ʾ����Ĵ���
 *	�� �� ֵ:	��
 *	��������:	���ڽ�������жϻص�����
 *	˵    ��:	��
 *************************************************************************************************/
// UART ��������жϻص�
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == USART3)
  {
// �����յ����ֽڻش������ԣ�
#ifdef ANO_Protocol_ENABLE
    ANO_RX_Process(rx_buffer); // ������յ�������
#endif

    // �������������жϣ���������
    HAL_UART_Receive_IT(&huart3, &rx_buffer, 1);
  }
  else if (huart->Instance == UART4)
  {
    OPENMV_RX_Process(openmvRxData); // ������յ�������

    // �������������жϣ���������
    HAL_UART_Receive_IT(&huart4, &openmvRxData, 1);
  }
}
