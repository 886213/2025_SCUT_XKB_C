#ifndef __TAST_H__
#define __TAST_H__

#include "main.h"

#define Scan_Num 5

typedef enum
{
  Nonetask,
  Move_Origin,
  Scan_Screen,
  Scan_Seting_A4Target,
  Scan_Freedom_A4Target,
  FreeCTRL
} Task;
extern Task main_task;

typedef struct
{
  uint16_t x;
  uint16_t y;
  uint16_t w;
  uint16_t h;
  uint16_t color;
  uint8_t click_flag;
} Button;

typedef struct RedDot_LCD
{
  uint16_t x;
  uint16_t y;
} RedDot_LCD;

extern RedDot_LCD Now_RedDot;
extern RedDot_LCD Last_RedDot;
extern RedDot_LCD Target_RedDot;

extern RedDot_LCD Screen_LeftTop;
extern RedDot_LCD Screen_RightTop;
extern RedDot_LCD Screen_LeftBottom;
extern RedDot_LCD Screen_RightBottom;
extern RedDot_LCD Screen_Center;

extern RedDot_LCD A4Target_LeftTop;
extern RedDot_LCD A4Target_RightTop;
extern RedDot_LCD A4Target_LeftBottom;
extern RedDot_LCD A4Target_RightBottom;

extern uint8_t Scan_A4Target_Flag;
extern uint8_t Scan_A4Target_Count;

void MyStateMachine(void);

void Task_Move_to_Zero_Flag(void);
void Task_Scan_Screen_Flag(void);
void Task_Seting_A4Target_Flag(void);
void Task_Freedom_A4Target_Flag(void);

void Task_Move_to_Zero(void);
void Task_Scan_Screen(void);
void Task_Seting_A4Target(void);
void Task_Freedom_A4Target(void);

void LCD_Show_Screen(void);
void LCD_Show_MyUI(void);

unsigned int mode_int(unsigned int arr[10]);
float mode_float(float arr[10]);

#endif