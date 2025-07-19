#ifndef __AS5600_H__
#define __AS5600_H__
#include "sys.h"
#include "usart.h"
#include "math.h"
#include "Step_motor_controller.h"

/* ?????? --------------------------------------------------------*/
#define HC4052_A0_PORT GPIOD
#define HC4052_A0_PIN GPIO_PIN_0
#define HC4052_A1_PORT GPIOD
#define HC4052_A1_PIN GPIO_PIN_1

// I²C3 ?????PA8: SCL, PC9: SDA?
#define I2C3_SCL_PIN      GPIO_PIN_8
#define I2C3_SCL_PORT     GPIOA
#define I2C3_SDA_PIN      GPIO_PIN_9
#define I2C3_SDA_PORT     GPIOC

#define AS5600_ANGLE_REG_H     0x0C    // ????????
#define AS5600_ANGLE_REG_L     0x0D    // ????????
#define AS5600_ADDR 0x36 // AS5600 7???

extern float angles[4];


void AS5600_Init(void);
void MX_I2C3_Init(void);
void Select_AS5600(uint8_t dev_num);
float Read_AS5600_Angle_Single(uint8_t dev_num);
void Read_AS5600_Angle_All(void);
void Deselect_AS5600(void);


#endif
