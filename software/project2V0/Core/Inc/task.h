#ifndef __TAST_H__
#define __TAST_H__

#include "main.h"

#define Scan_Num 5

#define Button_UP       6
#define Button_DOWN     2
#define Button_LEFT     4
#define Button_RIGHT    1
#define Button_Click1   5
#define Button_Click2   3
#define Button_Click3   7
#define Button_NONE     0

#define Button1_PIN   GPIO_PIN_13
#define Button1_PORT  GPIOE
#define Button1_CLK_ENABLE __HAL_RCC_GPIOE_CLK_ENABLE()
#define Button2_PIN   GPIO_PIN_10
#define Button2_PORT  GPIOB
#define Button2_CLK_ENABLE __HAL_RCC_GPIOB_CLK_ENABLE()
#define Button3_PIN   GPIO_PIN_12
#define Button3_PORT  GPIOB
#define Button3_CLK_ENABLE __HAL_RCC_GPIOB_CLK_ENABLE()

#define Key1_PIN GPIO_PIN_11
#define Key1_PORT GPIOB
#define Key1_CLK_ENABLE __HAL_RCC_GPIOB_CLK_ENABLE()
#define Key2_PIN GPIO_PIN_7
#define Key2_PORT GPIOE
#define Key2_CLK_ENABLE __HAL_RCC_GPIOE_CLK_ENABLE()
#define Key3_PIN
#define Key3_PORT
#define Key3_CLK_ENABLE

typedef enum
{
  Nonetask,
  Move_Origin,
  Scan_Screen,
  Scan_Seting_A4Target,
  Scan_Freedom_A4Target
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

void Key_Button_Init(void);
uint8_t Get_Button_State(void);
uint8_t Get_Key1_State(void);
uint8_t Get_Key2_State(void);

#endif