#include "sys.h"

#ifndef _SPI_H_
#define _SPI_H_

//�����Գ���ʹ�õ���Ӳ��SPI�ӿ�����
//����SPIʱ���ź��Լ�SPI����д�ź����Ų��ɸ��ģ��������Ŷ����Ը���
//SPI��ʱ�����Ŷ���̶�ΪPB13
//SPI�Ķ��������Ŷ���̶�ΪPB14
//SPI��д�������Ŷ���̶�ΪPB15
 
extern SPI_HandleTypeDef SPI4_Handler;  //SPI���
extern SPI_HandleTypeDef SPI2_Handler;  //SPI���

void SPI4_Init(void);
void SPI4_SetSpeed(u32 SPI_BaudRatePrescaler);
u8 SPI4_ReadWriteByte(u8 TxData);

void SPI2_Init(void);

#endif
