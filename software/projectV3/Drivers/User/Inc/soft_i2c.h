// �ļ���soft_i2c.h
#ifndef __SOFT_I2C_H
#define __SOFT_I2C_H

#include "stm32h7xx_hal.h"

// ���I2C���Ŷ���
#define SOFT_I2C_SCL_PIN    GPIO_PIN_1
#define SOFT_I2C_SDA_PIN    GPIO_PIN_3
#define SOFT_I2C_GPIO_PORT  GPIOC

// ��ʱ�꣨������Ƶ����������ϵͳ��ƵΪ400MHz��
#define I2C_DELAY()     do { for (volatile int i = 0; i < 120; i++); } while(0)

// ��������
void Soft_I2C_Init(void);
void Soft_I2C_Start(void);
void Soft_I2C_Stop(void);
uint8_t Soft_I2C_Write_Byte(uint8_t data);
uint8_t Soft_I2C_Read_Byte(uint8_t ack);
void Soft_I2C_Ack(void);
void Soft_I2C_NAck(void);
uint8_t FT6336_ReadReg(uint8_t reg, uint8_t *data, uint8_t len);
uint8_t FT6336_ReadTouch(uint16_t *x, uint16_t *y);


#endif