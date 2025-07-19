/***
  ************************************************************************************************
  *	@file  	usart.c
  *	@version V1.0
  *  @date    2020-5-18
  *	@author  反客科技
  *	@brief   usart相关函数
   ************************************************************************************************
   *  @description
  *
  *	实验平台：反客STM32H750VBT6核心板 （型号：FK750M1-VBT6）
  *	淘宝地址：https://shop212360197.taobao.com
  *	QQ交流群：536665479
  *
>>>>> 文件说明：
  *
  *  初始化usart引脚，配置波特率等参数
  *
  ************************************************************************************************
***/

#include "usart.h"
#include "stm32h7xx_hal.h"

UART_HandleTypeDef huart3; // UART_HandleTypeDef 结构体变量
uint8_t rx_buffer;         // 接收缓冲区

/*************************************************************************************************
 *	函 数 名:	HAL_UART_MspInit
 *	入口参数:	huart - UART_HandleTypeDef定义的变量，即表示定义的串口
 *	返 回 值:	无
 *	函数功能:	初始化串口引脚
 *	说    明:	无
 *************************************************************************************************/

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  if (huart->Instance == USART3)
  {
    __HAL_RCC_USART3_CLK_ENABLE(); // 开启 USART1 时钟

    GPIO_USART3_TX_CLK_ENABLE; // 开启 USART1 TX 引脚的 GPIO 时钟
    GPIO_USART3_RX_CLK_ENABLE; // 开启 USART1 RX 引脚的 GPIO 时钟

    GPIO_InitStruct.Pin = USART3_TX_PIN;               // TX引脚
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;            // 复用推挽输出
    GPIO_InitStruct.Pull = GPIO_PULLUP;                // 上拉
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH; // 速度等级
    GPIO_InitStruct.Alternate = GPIO_AF7_USART3;       // 复用为USART1
    HAL_GPIO_Init(USART3_TX_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = USART3_RX_PIN; // RX引脚
    HAL_GPIO_Init(USART3_RX_PORT, &GPIO_InitStruct);
  }
}

/*************************************************************************************************
 *	函 数 名:	USART1_Init
 *	入口参数:	无
 *	返 回 值:	无
 *	函数功能:	初始化串口配置
 *	说    明:	无
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
  // 配置USART1中断
  HAL_NVIC_SetPriority(USART3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(USART3_IRQn);
  HAL_UART_Receive_IT(&huart3, &rx_buffer, 1);
}

/*************************************************************************************************
 *	函 数 名:	fputc
 *	入口参数:	ch - 要输出的字符 ，  f - 文件指针（这里用不到）
 *	返 回 值:	正常时返回字符，出错时返回 EOF（-1）
 *	函数功能:	重定向 fputc 函数，目的是使用 printf 函数
 *	说    明:	无
 *************************************************************************************************/

int fputc(int ch, FILE *f)
{
  HAL_UART_Transmit(&huart3, (uint8_t *)&ch, 1, 100); // 发送单字节数据
  return (ch);
}

/*************************************************************************************************
 *	函 数 名:	HAL_UART_RxCpltCallback
 *	入口参数:	huart - UART_HandleTypeDef定义的变量，即表示定义的串口
 *	返 回 值:	无
 *	函数功能:	串口接收完成中断回调函数
 *	说    明:	无
 *************************************************************************************************/
// UART 接收完成中断回调
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == USART3)
  {
// 将接收到的字节回传（回显）
#ifdef ANO_Protocol_ENABLE
    ANO_RX_Process(rx_buffer); // 处理接收到的数据
#endif

    // 重新启动接收中断，持续监听
    HAL_UART_Receive_IT(&huart3, &rx_buffer, 1);
  }
}
