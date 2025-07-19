#include "task.h"

Task main_task;

struct RedDot_LCD Now_RedDot;
struct RedDot_LCD Last_RedDot;
struct RedDot_LCD Target_RedDot;

struct RedDot_LCD Screen_LeftTop = {385, 272};
struct RedDot_LCD Screen_RightTop = {108, 272};
struct RedDot_LCD Screen_LeftBottom = {385, 35};
struct RedDot_LCD Screen_RightBottom = {110, 32};
struct RedDot_LCD Screen_Center = {245, 152};

struct RedDot_LCD OutSide = {245, 152};

struct RedDot_LCD A4Target_LeftTop;
struct RedDot_LCD A4Target_RightTop;
struct RedDot_LCD A4Target_LeftBottom;
struct RedDot_LCD A4Target_RightBottom;

uint8_t Scan_A4Target_Flag = 0;
uint8_t Scan_A4Target_Count = 0;

uint8_t NextStateFlag; // 进入下一个状态的标志

void Task_Move_to_Zero_Flag(void) { main_task = Move_Origin; }
void Task_Scan_Screen_Flag(void) { main_task = Scan_Screen; }
void Task_Seting_A4Target_Flag(void) { main_task = Scan_Seting_A4Target; }
void Task_Freedom_A4Target_Flag(void) { main_task = Scan_Freedom_A4Target; }

/*
 * @brief  运动到原点，第一次进入时设置目标点到屏幕中心，第二次进入时返回到非任务状态
 * @param  无
 * @retval 无
 */
void Task_Move_to_Zero(void)
{
  static uint8_t state = 0; // 静态变量，保持状态
  if (state == 0)
  {
    StepMotor_controler_main = StepMotor_controler_ENABLE; // 使能步进电机
    Set_LCD_Point2(Screen_Center.x, Screen_Center.y);      // 设置到屏幕中心
    state = 1;
  }
  else if (state == 1)
  {
    LCD_Show_Screen();
    main_task = Nonetask; // 任务完成，返回到非任务状态
    NextStateFlag = 1;    // 设置状态标志为1，表示任务完成，方便下一次任务调用
    state = 0;
  }
}

/*
 * @brief  扫描屏幕，第一次进入时设置目标点到左上角，第二次进入时设置目标点到右上角，第三次进入时设置目标点到右下角，第四次进入时设置目标点到左下角，第五次进入时返回到非任务状态
 * @param  无
 * @retval 无
 */
void Task_Scan_Screen(void)
{
  static uint8_t state = 0;
  if (state == 0)
  {
    StepMotor_controler_main = StepMotor_controler_ENABLE; // 使能步进电机
    Set_LCD_Point2(Screen_LeftTop.x, Screen_LeftTop.y);    // 设置到左上角
    state = 1;
  }
  else if (state == 1)
  {
    StepMotor_controler_main = StepMotor_controler_ENABLE; // 使能步进电机
    Set_LCD_Point2(Screen_RightTop.x, Screen_RightTop.y);  // 设置到右上角
    state = 2;
  }
  else if (state == 2)
  {
    StepMotor_controler_main = StepMotor_controler_ENABLE;      // 使能步进电机
    Set_LCD_Point2(Screen_RightBottom.x, Screen_RightBottom.y); // 设置到右下角
    state = 3;
  }
  else if (state == 3)
  {
    StepMotor_controler_main = StepMotor_controler_ENABLE;    // 使能步进电机
    Set_LCD_Point2(Screen_LeftBottom.x, Screen_LeftBottom.y); // 设置到左下角
    state = 4;
  }
  else if (state == 4)
  {
    StepMotor_controler_main = StepMotor_controler_ENABLE; // 使能步进电机
    Set_LCD_Point2(Screen_LeftTop.x, Screen_LeftTop.y);    // 设置到左上角
    state = 5;
  }
  else if (state == 5)
  {
    LCD_Show_Screen();
    main_task = Nonetask; // 任务完成，返回到非任务状态
    NextStateFlag = 1;    // 设置状态标志为1，表示任务完成，方便下一次任务调用
    state = 0;
  }
}

void Task_Seting_A4Target(void)
{
}

void Task_Freedom_A4Target(void)
{
  static uint8_t state = 0;
  if (state == 0)
  {
    StepMotor_controler_main = StepMotor_controler_ENABLE; // 使能步进电机
    Set_LCD_Point2(OutSide.x, OutSide.y);                  // 设置到场外
    RxTargetFlag = 3;
    state = 1;
  }
  else if (state == 1)
  {
    // OV2640_DCMI_Resume(); // 恢复摄像头
    Scan_A4Target_Flag = 1;
    RxTargetFlag = 3;
    state = 2;
  }
  else if (state == 2)
  {
    StepMotor_controler_main = StepMotor_controler_ENABLE;  // 使能步进电机
    Set_LCD_Point2(A4Target_LeftTop.x, A4Target_LeftTop.y); // 设置到靶纸左上顶点
    RxTargetFlag = 3;
    state = 30;
  }
  else if (state == 30)
  {
    StepMotor_controler_main = StepMotor_controler_ENABLE;                                                          // 使能步进电机
    Set_LCD_Point2((A4Target_RightTop.x + A4Target_LeftTop.x) / 2, (A4Target_RightTop.y + A4Target_LeftTop.y) / 2); // 设置到靶纸右上顶点
    RxTargetFlag = 3;
    state = 31;
  }
  else if (state == 31)
  {
    StepMotor_controler_main = StepMotor_controler_ENABLE;    // 使能步进电机
    Set_LCD_Point2(A4Target_RightTop.x, A4Target_RightTop.y); // 设置到靶纸右上顶点
    RxTargetFlag = 3;
    state = 40;
  }
  else if (state == 40)
  {
    StepMotor_controler_main = StepMotor_controler_ENABLE;                                                                  // 使能步进电机
    Set_LCD_Point2((A4Target_RightTop.x + A4Target_RightBottom.x) / 2, (A4Target_RightTop.y + A4Target_RightBottom.y) / 2); // 设置到靶纸右下顶点
    RxTargetFlag = 3;
    state = 41;
  }
  else if (state == 41)
  {
    StepMotor_controler_main = StepMotor_controler_ENABLE;          // 使能步进电机
    Set_LCD_Point2(A4Target_RightBottom.x, A4Target_RightBottom.y); // 设置到靶纸右下顶点
    RxTargetFlag = 3;
    state = 50;
  }
  else if (state == 50)
  {
    StepMotor_controler_main = StepMotor_controler_ENABLE;                                                                      // 使能步进电机
    Set_LCD_Point2((A4Target_RightBottom.x + A4Target_LeftBottom.x) / 2, (A4Target_RightBottom.y + A4Target_LeftBottom.y) / 2); // 设置到靶纸左下顶点
    RxTargetFlag = 3;
    state = 51;
  }
  else if (state == 51)
  {
    StepMotor_controler_main = StepMotor_controler_ENABLE;        // 使能步进电机
    Set_LCD_Point2(A4Target_LeftBottom.x, A4Target_LeftBottom.y); // 设置到靶纸左下顶点
    RxTargetFlag = 3;
    state = 60;
  }
  else if (state == 60)
  {
    StepMotor_controler_main = StepMotor_controler_ENABLE;                                                              // 使能步进电机
    Set_LCD_Point2((A4Target_LeftTop.x + A4Target_LeftBottom.x) / 2, (A4Target_LeftTop.y + A4Target_LeftBottom.y) / 2); // 设置到靶纸左上顶点
    RxTargetFlag = 3;
    state = 61;
  }
  else if (state == 61)
  {
    StepMotor_controler_main = StepMotor_controler_ENABLE;  // 使能步进电机
    Set_LCD_Point2(A4Target_LeftTop.x, A4Target_LeftTop.y); // 设置到靶纸左上顶点
    RxTargetFlag = 3;
    main_task = Nonetask;
    NextStateFlag = 1; // 设置状态标志为1，表示任务完成，方便下一次任务调用
    state = 0;
  }
}

void LCD_Show_Screen(void)
{
  // LCD_DrawLine2(110 - 6, 32 - 6, 385 + 6, 35 - 6, 2, RED);   // 画底线
  // LCD_DrawLine2(110 - 6, 32 - 6, 108 - 6, 272 + 6, 2, RED);  // 画右线
  // LCD_DrawLine2(385 + 6, 35 - 6, 385 + 6, 272 + 6, 2, RED);  // 画左线
  // LCD_DrawLine2(385 + 6, 272 + 6, 108 - 6, 272 + 6, 2, RED); // 画上线
}

void LCD_Show_MyUI(void)
{
  LCD_Show_Screen();
}

void MyStateMachine(void)
{
  if (NextStateFlag == 0) // 如果状态标志为零，本次任务未完成
  {
    return; // 直接返回
  }
  switch (main_task)
  {
  case Nonetask:
    break;
  case Move_Origin:
    NextStateFlag = 0; // 清零标志位，等待本次任务完成
    Task_Move_to_Zero();
    break;
  case Scan_Screen:
    NextStateFlag = 0; // 任务完成，返回到非任务状态
    Task_Scan_Screen();
    break;
  case Scan_Seting_A4Target:
    NextStateFlag = 0; // 任务完成，返回到非任务状态
    Task_Seting_A4Target();
    break;
  case Scan_Freedom_A4Target:
    NextStateFlag = 0; // 任务完成，返回到非任务状态
    Task_Freedom_A4Target();
    break;
  default:
    break;
  }
}

unsigned int mode_int(unsigned int arr[10])
{
  int max_count = 0;
  unsigned int mode = arr[0]; // 初始化为第一个元素

  // 遍历所有元素统计出现次数
  for (int i = 0; i < 10; i++)
  {
    unsigned int current = arr[i];
    int count = 0;

    // 内层循环统计当前元素的出现次数
    for (int j = 0; j < 10; j++)
    {
      if (arr[j] == current)
        count++;
    }

    // 更新众数逻辑：
    // 1. 如果当前元素出现次数更多，直接更新
    // 2. 如果次数相同但数值更大，也更新（保证返回数值最大的众数）
    if (count > max_count || (count == max_count && current > mode))
    {
      max_count = count;
      mode = current;
    }
  }
  return mode;
}

float mode_float(float arr[10])
{
  int max_count = 0;
  float mode = arr[0]; // 初始化为第一个元素

  // 遍历所有元素统计出现次数
  for (int i = 0; i < 10; i++)
  {
    float current = arr[i];
    int count = 0;

    // 内层循环统计当前元素的出现次数
    for (int j = 0; j < 10; j++)
    {
      if (arr[j] == current)
        count++;
    }

    // 更新众数逻辑：
    // 1. 如果当前元素出现次数更多，直接更新
    // 2. 如果次数相同但数值更大，也更新（保证返回数值最大的众数）
    if (count > max_count || (count == max_count && current > mode))
    {
      max_count = count;
      mode = current;
    }
  }
  return mode;
}

void Key_Button_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  Button1_CLK_ENABLE;
  Button2_CLK_ENABLE;
  Button3_CLK_ENABLE;

  Key1_CLK_ENABLE;
  Key2_CLK_ENABLE;

  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

  GPIO_InitStruct.Pin = Button1_PIN;
  HAL_GPIO_Init(Button1_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = Button2_PIN;
  HAL_GPIO_Init(Button2_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = Button3_PIN;
  HAL_GPIO_Init(Button3_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = Key1_PIN;
  HAL_GPIO_Init(Key1_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = Key2_PIN;
  HAL_GPIO_Init(Key2_PORT, &GPIO_InitStruct);
}

uint8_t Last_Button_State = 7;
uint8_t Current_Button_State = 0;
uint8_t Get_Button_State(void)
{
  Current_Button_State = HAL_GPIO_ReadPin(Button1_PORT, Button1_PIN) << 2;
  Current_Button_State += HAL_GPIO_ReadPin(Button2_PORT, Button2_PIN) << 1;
  Current_Button_State += HAL_GPIO_ReadPin(Button3_PORT, Button3_PIN);

  uint8_t Button_State = 0;

  if (Last_Button_State == Current_Button_State)
  {
    switch (7 - Last_Button_State)
    {
    case Button_UP:
      return Button_UP;
      break;
    case Button_DOWN:
      return Button_DOWN;
      break;
    case Button_LEFT:
      return Button_LEFT;
      break;
    case Button_RIGHT:
      return Button_RIGHT;
      break;
    default:
      break;
    }
  }

  if (Last_Button_State != Current_Button_State && Last_Button_State == 7)
  {
    Button_State = 7 - Current_Button_State;
    Last_Button_State = Current_Button_State;
    return Button_State;
  }

  Last_Button_State = Current_Button_State;
  return 0;
}

uint8_t Get_Key1_State(void)
{
  return HAL_GPIO_ReadPin(Key1_PORT, Key1_PIN);
}

uint8_t Get_Key2_State(void)
{
  return HAL_GPIO_ReadPin(Key2_PORT, Key2_PIN);
}
