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

UART_HandleTypeDef huart3; // UART_HandleTypeDef �ṹ�����
uint8_t rx_buffer;         // ���ջ�����

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
  HAL_NVIC_SetPriority(USART3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(USART3_IRQn);
  HAL_UART_Receive_IT(&huart3, &rx_buffer, 1);
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
}
