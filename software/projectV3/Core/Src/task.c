#include "task.h"

Task main_task;

struct RedDot_LCD Now_RedDot;
struct RedDot_LCD Last_RedDot;
struct RedDot_LCD Target_RedDot;

struct RedDot_LCD Screen_LeftTop = {380, 277};
struct RedDot_LCD Screen_RightTop = {102, 274};
struct RedDot_LCD Screen_MidTop = {180, 277};
struct RedDot_LCD Screen_LeftBottom = {380, 38};
struct RedDot_LCD Screen_RightBottom = {108, 35};
struct RedDot_LCD Screen_Center = {242, 155};

// struct RedDot_LCD OutSide = {242, 300};
struct RedDot_LCD OutSide = {242, 155};

struct RedDot_LCD A4Target_LeftTop;
struct RedDot_LCD A4Target_RightTop;
struct RedDot_LCD A4Target_LeftBottom;
struct RedDot_LCD A4Target_RightBottom;

uint8_t Scan_A4Target_Flag = 0;
uint8_t Scan_A4Target_Count = 0;
uint8_t NextStateFlag = 1; // 设置状态标志为1，表示任务完成，方便下一次任务调用

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
    Set_LCD_Point2(Screen_MidTop.x, Screen_MidTop.y);      // 设置到上线中点
    state = 2;
  }
  else if (state == 2)
  {
    StepMotor_controler_main = StepMotor_controler_ENABLE; // 使能步进电机
    Set_LCD_Point2(Screen_RightTop.x, Screen_RightTop.y);  // 设置到右上角
    state = 3;
  }
  else if (state == 3)
  {
    StepMotor_controler_main = StepMotor_controler_ENABLE;                                                          // 使能步进电机
    Set_LCD_Point2((Screen_RightBottom.x + Screen_RightTop.x) / 2, (Screen_RightTop.y + Screen_RightBottom.y) / 2); // 设置到右线中点
    state = 4;
  }
  else if (state == 4)
  {
    StepMotor_controler_main = StepMotor_controler_ENABLE;      // 使能步进电机
    Set_LCD_Point2(Screen_RightBottom.x, Screen_RightBottom.y); // 设置到右下角
    state = 5;
  }
  else if (state == 5)
  {
    StepMotor_controler_main = StepMotor_controler_ENABLE;                                                              // 使能步进电机
    Set_LCD_Point2((Screen_LeftBottom.x + Screen_RightBottom.x) / 2, (Screen_LeftBottom.y + Screen_RightBottom.y) / 2); // 设置到下线中点
    state = 6;
  }
  else if (state == 6)
  {
    StepMotor_controler_main = StepMotor_controler_ENABLE;    // 使能步进电机
    Set_LCD_Point2(Screen_LeftBottom.x, Screen_LeftBottom.y); // 设置到左下角
    state = 7;
  }
  else if (state == 7)
  {
    StepMotor_controler_main = StepMotor_controler_ENABLE;                                                      // 使能步进电机
    Set_LCD_Point2((Screen_LeftBottom.x + Screen_LeftTop.x) / 2, (Screen_LeftTop.y + Screen_LeftBottom.y) / 2); // 设置到左线中点
    state = 8;
  }
  else if (state == 8)
  {
    StepMotor_controler_main = StepMotor_controler_ENABLE; // 使能步进电机
    Set_LCD_Point2(Screen_LeftTop.x, Screen_LeftTop.y);    // 设置到左上角
    state = 9;
  }
  else if (state == 9)
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
    OV2640_DCMI_Resume(); // 恢复摄像头
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

void Task_FreeCTRL(void)
{
  
}

void LCD_Show_Screen(void)
{
  LCD_DrawLine2(LCD_W - (Screen_RightBottom.x - 6), LCD_H - (Screen_RightBottom.y - 6), LCD_W - (Screen_LeftBottom.x + 6), LCD_H - (Screen_LeftBottom.y - 6), 2, RED); // 画底线
  LCD_DrawLine2(LCD_W - (Screen_RightBottom.x - 6), LCD_H - (Screen_RightBottom.y - 6), LCD_W - (Screen_RightTop.x - 6), LCD_H - (Screen_RightTop.y + 6), 2, RED);     // 画右线
  LCD_DrawLine2(LCD_W - (Screen_LeftBottom.x + 6), LCD_H - (Screen_LeftBottom.y - 6), LCD_W - (Screen_LeftTop.x + 6), LCD_H - (Screen_LeftTop.y + 6), 2, RED);         // 画左线
  LCD_DrawLine2(LCD_W - (Screen_LeftTop.x + 6), LCD_H - (Screen_LeftTop.y + 6), LCD_W - (Screen_RightTop.x - 6), LCD_H - (Screen_RightTop.y + 6), 2, RED);             // 画上线
}

void LCD_Show_MyUI(void)
{
  LCD_DrawFillRectangle2(0, 0, 480 - 1, 320 - 1, WHITE); // 清屏

  LCD_Show_Screen();
  LCD_DrawFillRectangle2(10, 10, 90, 50, BLUE);
  LCD_ShowString(30, 22, 16, "Reset", 1);

  LCD_DrawFillRectangle2(10, 60, 90, 100, BLUE);
  LCD_ShowString(26, 72, 16, "Screen", 1);

  LCD_DrawFillRectangle2(10, 110, 90, 150, BLUE);
  LCD_ShowString(18, 122, 16, "A4Target", 1);

  LCD_DrawFillRectangle2(10, 160, 90, 200, BLUE);
  LCD_ShowString(18, 172, 16, "FreeCTRL", 1);

  LCD_DrawFillRectangle2(10, 210, 90, 250, GREEN);
  LCD_ShowString(30, 222, 16, "Clear", 1);

  LCD_DrawFillRectangle2(10, 260, 90, 300, RED);
  LCD_ShowString(34, 272, 16, "Stop", 1);
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
    NextStateFlag = 0; // 清零标志位，等待本次任务完成
    Task_Scan_Screen();
    break;
  case Scan_Seting_A4Target:
    NextStateFlag = 0; // 清零标志位，等待本次任务完成
    Task_Seting_A4Target();
    break;
  case Scan_Freedom_A4Target:
    NextStateFlag = 0; // 清零标志位，等待本次任务完成
    Task_Freedom_A4Target();
    break;
  case FreeCTRL:
    NextStateFlag = 0; // 清零标志位，等待本次任务完成
    Task_FreeCTRL();
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