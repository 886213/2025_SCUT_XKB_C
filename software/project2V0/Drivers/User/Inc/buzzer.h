#ifndef __BUZZER_H__
#define __BUZZER_H__

#include "main.h"

void Buzzer_Init(void);
void Buzzer_SetFrequency(uint32_t freq);
void Buzzer_SetVolume(uint8_t volume);
void Buzzer_On(void);
void Buzzer_Off(void);
void Buzzer_Swich(void);

#endif