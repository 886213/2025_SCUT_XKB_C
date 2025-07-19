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

uint8_t NextStateFlag; // ������һ��״̬�ı�־

void Task_Move_to_Zero_Flag(void) { main_task = Move_Origin; }
void Task_Scan_Screen_Flag(void) { main_task = Scan_Screen; }
void Task_Seting_A4Target_Flag(void) { main_task = Scan_Seting_A4Target; }
void Task_Freedom_A4Target_Flag(void) { main_task = Scan_Freedom_A4Target; }

/*
 * @brief  �˶���ԭ�㣬��һ�ν���ʱ����Ŀ��㵽��Ļ���ģ��ڶ��ν���ʱ���ص�������״̬
 * @param  ��
 * @retval ��
 */
void Task_Move_to_Zero(void)
{
  static uint8_t state = 0; // ��̬����������״̬
  if (state == 0)
  {
    StepMotor_controler_main = StepMotor_controler_ENABLE; // ʹ�ܲ������
    Set_LCD_Point2(Screen_Center.x, Screen_Center.y);      // ���õ���Ļ����
    state = 1;
  }
  else if (state == 1)
  {
    LCD_Show_Screen();
    main_task = Nonetask; // ������ɣ����ص�������״̬
    NextStateFlag = 1;    // ����״̬��־Ϊ1����ʾ������ɣ�������һ���������
    state = 0;
  }
}

/*
 * @brief  ɨ����Ļ����һ�ν���ʱ����Ŀ��㵽���Ͻǣ��ڶ��ν���ʱ����Ŀ��㵽���Ͻǣ������ν���ʱ����Ŀ��㵽���½ǣ����Ĵν���ʱ����Ŀ��㵽���½ǣ�����ν���ʱ���ص�������״̬
 * @param  ��
 * @retval ��
 */
void Task_Scan_Screen(void)
{
  static uint8_t state = 0;
  if (state == 0)
  {
    StepMotor_controler_main = StepMotor_controler_ENABLE; // ʹ�ܲ������
    Set_LCD_Point2(Screen_LeftTop.x, Screen_LeftTop.y);    // ���õ����Ͻ�
    state = 1;
  }
  else if (state == 1)
  {
    StepMotor_controler_main = StepMotor_controler_ENABLE; // ʹ�ܲ������
    Set_LCD_Point2(Screen_RightTop.x, Screen_RightTop.y);  // ���õ����Ͻ�
    state = 2;
  }
  else if (state == 2)
  {
    StepMotor_controler_main = StepMotor_controler_ENABLE;      // ʹ�ܲ������
    Set_LCD_Point2(Screen_RightBottom.x, Screen_RightBottom.y); // ���õ����½�
    state = 3;
  }
  else if (state == 3)
  {
    StepMotor_controler_main = StepMotor_controler_ENABLE;    // ʹ�ܲ������
    Set_LCD_Point2(Screen_LeftBottom.x, Screen_LeftBottom.y); // ���õ����½�
    state = 4;
  }
  else if (state == 4)
  {
    StepMotor_controler_main = StepMotor_controler_ENABLE; // ʹ�ܲ������
    Set_LCD_Point2(Screen_LeftTop.x, Screen_LeftTop.y);    // ���õ����Ͻ�
    state = 5;
  }
  else if (state == 5)
  {
    LCD_Show_Screen();
    main_task = Nonetask; // ������ɣ����ص�������״̬
    NextStateFlag = 1;    // ����״̬��־Ϊ1����ʾ������ɣ�������һ���������
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
    StepMotor_controler_main = StepMotor_controler_ENABLE; // ʹ�ܲ������
    Set_LCD_Point2(OutSide.x, OutSide.y);                  // ���õ�����
    RxTargetFlag = 3;
    state = 1;
  }
  else if (state == 1)
  {
    // OV2640_DCMI_Resume(); // �ָ�����ͷ
    Scan_A4Target_Flag = 1;
    RxTargetFlag = 3;
    state = 2;
  }
  else if (state == 2)
  {
    StepMotor_controler_main = StepMotor_controler_ENABLE;  // ʹ�ܲ������
    Set_LCD_Point2(A4Target_LeftTop.x, A4Target_LeftTop.y); // ���õ���ֽ���϶���
    RxTargetFlag = 3;
    state = 30;
  }
  else if (state == 30)
  {
    StepMotor_controler_main = StepMotor_controler_ENABLE;                                                          // ʹ�ܲ������
    Set_LCD_Point2((A4Target_RightTop.x + A4Target_LeftTop.x) / 2, (A4Target_RightTop.y + A4Target_LeftTop.y) / 2); // ���õ���ֽ���϶���
    RxTargetFlag = 3;
    state = 31;
  }
  else if (state == 31)
  {
    StepMotor_controler_main = StepMotor_controler_ENABLE;    // ʹ�ܲ������
    Set_LCD_Point2(A4Target_RightTop.x, A4Target_RightTop.y); // ���õ���ֽ���϶���
    RxTargetFlag = 3;
    state = 40;
  }
  else if (state == 40)
  {
    StepMotor_controler_main = StepMotor_controler_ENABLE;                                                                  // ʹ�ܲ������
    Set_LCD_Point2((A4Target_RightTop.x + A4Target_RightBottom.x) / 2, (A4Target_RightTop.y + A4Target_RightBottom.y) / 2); // ���õ���ֽ���¶���
    RxTargetFlag = 3;
    state = 41;
  }
  else if (state == 41)
  {
    StepMotor_controler_main = StepMotor_controler_ENABLE;          // ʹ�ܲ������
    Set_LCD_Point2(A4Target_RightBottom.x, A4Target_RightBottom.y); // ���õ���ֽ���¶���
    RxTargetFlag = 3;
    state = 50;
  }
  else if (state == 50)
  {
    StepMotor_controler_main = StepMotor_controler_ENABLE;                                                                      // ʹ�ܲ������
    Set_LCD_Point2((A4Target_RightBottom.x + A4Target_LeftBottom.x) / 2, (A4Target_RightBottom.y + A4Target_LeftBottom.y) / 2); // ���õ���ֽ���¶���
    RxTargetFlag = 3;
    state = 51;
  }
  else if (state == 51)
  {
    StepMotor_controler_main = StepMotor_controler_ENABLE;        // ʹ�ܲ������
    Set_LCD_Point2(A4Target_LeftBottom.x, A4Target_LeftBottom.y); // ���õ���ֽ���¶���
    RxTargetFlag = 3;
    state = 60;
  }
  else if (state == 60)
  {
    StepMotor_controler_main = StepMotor_controler_ENABLE;                                                              // ʹ�ܲ������
    Set_LCD_Point2((A4Target_LeftTop.x + A4Target_LeftBottom.x) / 2, (A4Target_LeftTop.y + A4Target_LeftBottom.y) / 2); // ���õ���ֽ���϶���
    RxTargetFlag = 3;
    state = 61;
  }
  else if (state == 61)
  {
    StepMotor_controler_main = StepMotor_controler_ENABLE;  // ʹ�ܲ������
    Set_LCD_Point2(A4Target_LeftTop.x, A4Target_LeftTop.y); // ���õ���ֽ���϶���
    RxTargetFlag = 3;
    main_task = Nonetask;
    NextStateFlag = 1; // ����״̬��־Ϊ1����ʾ������ɣ�������һ���������
    state = 0;
  }
}

void LCD_Show_Screen(void)
{
  // LCD_DrawLine2(110 - 6, 32 - 6, 385 + 6, 35 - 6, 2, RED);   // ������
  // LCD_DrawLine2(110 - 6, 32 - 6, 108 - 6, 272 + 6, 2, RED);  // ������
  // LCD_DrawLine2(385 + 6, 35 - 6, 385 + 6, 272 + 6, 2, RED);  // ������
  // LCD_DrawLine2(385 + 6, 272 + 6, 108 - 6, 272 + 6, 2, RED); // ������
}

void LCD_Show_MyUI(void)
{
  LCD_Show_Screen();
}

void MyStateMachine(void)
{
  if (NextStateFlag == 0) // ���״̬��־Ϊ�㣬��������δ���
  {
    return; // ֱ�ӷ���
  }
  switch (main_task)
  {
  case Nonetask:
    break;
  case Move_Origin:
    NextStateFlag = 0; // �����־λ���ȴ������������
    Task_Move_to_Zero();
    break;
  case Scan_Screen:
    NextStateFlag = 0; // ������ɣ����ص�������״̬
    Task_Scan_Screen();
    break;
  case Scan_Seting_A4Target:
    NextStateFlag = 0; // ������ɣ����ص�������״̬
    Task_Seting_A4Target();
    break;
  case Scan_Freedom_A4Target:
    NextStateFlag = 0; // ������ɣ����ص�������״̬
    Task_Freedom_A4Target();
    break;
  default:
    break;
  }
}

unsigned int mode_int(unsigned int arr[10])
{
  int max_count = 0;
  unsigned int mode = arr[0]; // ��ʼ��Ϊ��һ��Ԫ��

  // ��������Ԫ��ͳ�Ƴ��ִ���
  for (int i = 0; i < 10; i++)
  {
    unsigned int current = arr[i];
    int count = 0;

    // �ڲ�ѭ��ͳ�Ƶ�ǰԪ�صĳ��ִ���
    for (int j = 0; j < 10; j++)
    {
      if (arr[j] == current)
        count++;
    }

    // ���������߼���
    // 1. �����ǰԪ�س��ִ������ֱ࣬�Ӹ���
    // 2. ���������ͬ����ֵ����Ҳ���£���֤������ֵ����������
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
  float mode = arr[0]; // ��ʼ��Ϊ��һ��Ԫ��

  // ��������Ԫ��ͳ�Ƴ��ִ���
  for (int i = 0; i < 10; i++)
  {
    float current = arr[i];
    int count = 0;

    // �ڲ�ѭ��ͳ�Ƶ�ǰԪ�صĳ��ִ���
    for (int j = 0; j < 10; j++)
    {
      if (arr[j] == current)
        count++;
    }

    // ���������߼���
    // 1. �����ǰԪ�س��ִ������ֱ࣬�Ӹ���
    // 2. ���������ͬ����ֵ����Ҳ���£���֤������ֵ����������
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
