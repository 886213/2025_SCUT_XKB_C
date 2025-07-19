#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32h7xx_hal.h"

#include "led.h"
#include "usart.h"
#include "lcd_spi_200.h"
#include "dcmi_ov2640.h"
#include "lcd.h"
#include "delay.h"
#include "sys.h"
#include "touch.h"
#include "gui.h"
#include "test.h"
#include "AS5600.h"
#include "Step_motor_controller.h"
#include "Brushless_motor_controller.h"
#include "pid.h"
#include "ANO_Protocol.h"
#include "qspi_w25q64.h"
#include "task.h"
#include "buzzer.h"
#include "swj.h"
#include "color_tracer.h"
#include "image.h"

#include <stdio.h>
#include <string.h>

extern uint8_t Buzzer_Volum; 
extern uint16_t Buzzer_Frequency; 
extern uint8_t Buzzer_Flag;       
extern uint8_t tximagetimeflag;  
extern uint8_t NextStateFlag; // 进入下一个状态的标志 

void Error_Handler(void);

#ifdef __cplusplus
}
#endif

#endif
