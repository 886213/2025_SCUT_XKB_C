#include "sys.h"

#ifndef _SPI_H_
#define _SPI_H_

//本测试程序使用的是硬件SPI接口驱动
//除了SPI时钟信号以及SPI读、写信号引脚不可更改，其他引脚都可以更改
//SPI的时钟引脚定义固定为PB13
//SPI的读数据引脚定义固定为PB14
//SPI的写数据引脚定义固定为PB15
 
extern SPI_HandleTypeDef SPI4_Handler;  //SPI句柄
extern SPI_HandleTypeDef SPI2_Handler;  //SPI句柄

void SPI4_Init(void);
void SPI4_SetSpeed(u32 SPI_BaudRatePrescaler);
u8 SPI4_ReadWriteByte(u8 TxData);

void SPI2_Init(void);

#endif
