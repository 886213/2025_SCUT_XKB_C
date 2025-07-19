/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

#include "led.h"
#include "usart.h"
#include "lcd_spi_200.h"
// #include "dcmi_ov2640.h"
// #include "lcd.h"
// #include "lcd_spi_200.h"
#include "delay.h"
#include "sys.h"
// #include "touch.h"
// #include "gui.h"
// #include "test.h"
// #include "AS5600.h"
#include "Step_motor_controller.h"
// #include "Brushless_motor_controller.h"
#include "pid.h"
#include "ANO_Protocol.h"
#include "qspi_w25q64.h"
#include "task.h"
#include "buzzer.h"
// #include "swj.h"
// #include "color_tracer.h"
// #include "image.h"
#include "Filter.h"

extern uint8_t Buzzer_Volum; // 蜂鸣器音量
extern uint16_t Buzzer_Frequency; // 蜂鸣器频率
extern uint8_t Buzzer_Flag;         // 蜂鸣器标志
extern uint8_t tximagetimeflag;           // 蜂鸣器模式
extern uint8_t NextStateFlag; // 进入下一个状态的标志

void Error_Handler(void);

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
