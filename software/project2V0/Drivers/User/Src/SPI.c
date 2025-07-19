#include "spi.h"

SPI_HandleTypeDef SPI4_Handler; // SPI4句柄
SPI_HandleTypeDef SPI2_Handler; // SPI4句柄

// 以下是SPI模块的初始化代码，配置成主机模式
// SPI口初始化
// 这里针是对SPI2的初始化
void SPI4_Init(void)
{
  SPI4_Handler.Instance = SPI4;                                          // SPI4
  SPI4_Handler.Init.Mode = SPI_MODE_MASTER;                              // 设置SPI工作模式，设置为主模式
  SPI4_Handler.Init.Direction = SPI_DIRECTION_2LINES;                    // 设置SPI单向或者双向的数据模式:SPI设置为双线模式
  SPI4_Handler.Init.DataSize = SPI_DATASIZE_8BIT;                        // 设置SPI的数据大小:SPI发送接收8位帧结构
  SPI4_Handler.Init.CLKPolarity = SPI_POLARITY_HIGH;                     // 串行同步时钟的空闲状态为低电平
  SPI4_Handler.Init.CLKPhase = SPI_PHASE_2EDGE;                          // 串行同步时钟的第一个跳变沿（上升或下降）数据被采样
  SPI4_Handler.Init.NSS = SPI_NSS_SOFT;                                  // NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
  SPI4_Handler.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;                    // NSS信号脉冲失能
  SPI4_Handler.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_ENABLE; // SPI主模式IO状态保持使能
  SPI4_Handler.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;         // 定义波特率预分频的值:波特率预分频值为4
  SPI4_Handler.Init.FirstBit = SPI_FIRSTBIT_MSB;                         // 指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
  SPI4_Handler.Init.TIMode = SPI_TIMODE_DISABLE;                         // 关闭TI模式
  SPI4_Handler.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;         // 关闭硬件CRC校验
  SPI4_Handler.Init.CRCPolynomial = 7;                                   // CRC值计算的多项式
  HAL_SPI_Init(&SPI4_Handler);

  __HAL_SPI_ENABLE(&SPI4_Handler); // 使能SPI4
  SPI4_ReadWriteByte(0Xff);        // 启动传输
}

void SPI2_Init(void)
{
  SPI2_Handler.Instance = SPI2;                                          // SPI2
  SPI2_Handler.Init.Mode = SPI_MODE_MASTER;                              // 设置SPI工作模式，设置为主模式
  SPI2_Handler.Init.Direction = SPI_DIRECTION_2LINES;                    // 设置SPI单向或者双向的数据模式:SPI设置为双线模式
  SPI2_Handler.Init.DataSize = SPI_DATASIZE_8BIT;                        // 设置SPI的数据大小:SPI发送接收8位帧结构
  SPI2_Handler.Init.CLKPolarity = SPI_POLARITY_HIGH;                     // 串行同步时钟的空闲状态为低电平
  SPI2_Handler.Init.CLKPhase = SPI_PHASE_2EDGE;                          // 串行同步时钟的第一个跳变沿（上升或下降）数据被采样
  SPI2_Handler.Init.NSS = SPI_NSS_SOFT;                                  // NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
  SPI2_Handler.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;                    // NSS信号脉冲失能
  SPI2_Handler.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_ENABLE; // SPI主模式IO状态保持使能
  SPI2_Handler.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;        // 定义波特率预分频的值:波特率预分频值为4
  SPI2_Handler.Init.FirstBit = SPI_FIRSTBIT_LSB;                         // 指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
  SPI2_Handler.Init.TIMode = SPI_TIMODE_DISABLE;                         // 关闭TI模式
  SPI2_Handler.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;         // 关闭硬件CRC校验
  SPI2_Handler.Init.CRCPolynomial = 7;                                   // CRC值计算的多项式
  HAL_SPI_Init(&SPI2_Handler);

  __HAL_SPI_ENABLE(&SPI2_Handler); // 使能SPI2
}

// SPI2底层驱动，时钟使能，引脚配置
// 此函数会被HAL_SPI_Init()调用
// hspi:SPI句柄
// void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
// {
//   if (hspi->Instance == SPI4)
//   {
//     GPIO_InitTypeDef GPIO_Initure;

//     __HAL_RCC_GPIOE_CLK_ENABLE(); // 使能GPIOF时钟
//     __HAL_RCC_SPI4_CLK_ENABLE();  // 使能SPI2时钟

//     GPIO_Initure.Pin = GPIO_PIN_12 | GPIO_PIN_14;   // SCK、MOSI引脚
//     GPIO_Initure.Mode = GPIO_MODE_AF_PP;            // 复用推挽输出
//     GPIO_Initure.Pull = GPIO_NOPULL;                // 无上下拉
//     GPIO_Initure.Speed = GPIO_SPEED_FREQ_VERY_HIGH; // 最高速度等级
//     GPIO_Initure.Alternate = GPIO_AF5_SPI4;         // 复用到SPI4，复用线1
//     HAL_GPIO_Init(GPIOE, &GPIO_Initure);

//     GPIO_Initure.Pin = GPIO_PIN_13;                 // MISO引脚
//     GPIO_Initure.Mode = GPIO_MODE_INPUT;            // 复用推挽输出
//     GPIO_Initure.Pull = GPIO_NOPULL;                // 无上下拉
//     GPIO_Initure.Speed = GPIO_SPEED_FREQ_VERY_HIGH; // 最高速度等级
//     GPIO_Initure.Alternate = GPIO_AF5_SPI4;         // 复用到SPI4，复用线1
//     HAL_GPIO_Init(GPIOE, &GPIO_Initure);
//   }
//   else if(hspi->Instance == SPI2)
//   {
//     GPIO_InitTypeDef GPIO_Initure;

//     __HAL_RCC_GPIOB_CLK_ENABLE(); // 使能GPIOF时钟
//     __HAL_RCC_SPI2_CLK_ENABLE();  // 使能SPI2时钟

//     GPIO_Initure.Pin = GPIO_PIN_13 | GPIO_PIN_15;   // SCK、MOSI引脚
//     GPIO_Initure.Mode = GPIO_MODE_AF_PP;            // 复用推挽输出
//     GPIO_Initure.Pull = GPIO_NOPULL;                // 无上下拉
//     GPIO_Initure.Speed = GPIO_SPEED_FREQ_VERY_HIGH; // 最高速度等级
//     GPIO_Initure.Alternate = GPIO_AF5_SPI2;         // 复用到SPI4，复用线1
//     HAL_GPIO_Init(GPIOB, &GPIO_Initure);

//     GPIO_Initure.Pin = GPIO_PIN_14;                 // MISO引脚
//     GPIO_Initure.Mode = GPIO_MODE_INPUT;            // 复用推挽输出
//     GPIO_Initure.Pull = GPIO_NOPULL;                // 无上下拉
//     GPIO_Initure.Speed = GPIO_SPEED_FREQ_VERY_HIGH; // 最高速度等级
//     GPIO_Initure.Alternate = GPIO_AF5_SPI2;         // 复用到SPI4，复用线1
//     HAL_GPIO_Init(GPIOB, &GPIO_Initure);
//   }
// }

// SPI速度设置函数
// SPI速度=PLL1Q/分频系数
//@ref SPI_BaudRate_Prescaler:SPI_BAUDRATEPRESCALER_2~SPI_BAUDRATEPRESCALER_256
// PLL1Q时钟一般为200Mhz：
void SPI4_SetSpeed(u32 SPI_BaudRatePrescaler)
{
  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler)); // 判断有效性
  __HAL_SPI_DISABLE(&SPI4_Handler);                               // 关闭SPI
  SPI4_Handler.Instance->CFG1 &= ~(0X7 << 28);                    // 位30-28清零，用来设置波特率
  SPI4_Handler.Instance->CFG1 |= SPI_BaudRatePrescaler;           // 设置SPI速度
  __HAL_SPI_ENABLE(&SPI4_Handler);                                // 使能SPI
}

// SPI2 读写一个字节
// TxData:要写入的字节
// 返回值:读取到的字节
u8 SPI4_ReadWriteByte(u8 TxData)
{
  u8 Rxdata;
  HAL_SPI_TransmitReceive(&SPI4_Handler, &TxData, &Rxdata, 1, 1000);
  return Rxdata; // 返回收到的数据
}
