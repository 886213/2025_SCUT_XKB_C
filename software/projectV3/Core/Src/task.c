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
uint8_t NextStateFlag = 1; // ����״̬��־Ϊ1����ʾ������ɣ�������һ���������

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
    Set_LCD_Point2(Screen_MidTop.x, Screen_MidTop.y);      // ���õ������е�
    state = 2;
  }
  else if (state == 2)
  {
    StepMotor_controler_main = StepMotor_controler_ENABLE; // ʹ�ܲ������
    Set_LCD_Point2(Screen_RightTop.x, Screen_RightTop.y);  // ���õ����Ͻ�
    state = 3;
  }
  else if (state == 3)
  {
    StepMotor_controler_main = StepMotor_controler_ENABLE;                                                          // ʹ�ܲ������
    Set_LCD_Point2((Screen_RightBottom.x + Screen_RightTop.x) / 2, (Screen_RightTop.y + Screen_RightBottom.y) / 2); // ���õ������е�
    state = 4;
  }
  else if (state == 4)
  {
    StepMotor_controler_main = StepMotor_controler_ENABLE;      // ʹ�ܲ������
    Set_LCD_Point2(Screen_RightBottom.x, Screen_RightBottom.y); // ���õ����½�
    state = 5;
  }
  else if (state == 5)
  {
    StepMotor_controler_main = StepMotor_controler_ENABLE;                                                              // ʹ�ܲ������
    Set_LCD_Point2((Screen_LeftBottom.x + Screen_RightBottom.x) / 2, (Screen_LeftBottom.y + Screen_RightBottom.y) / 2); // ���õ������е�
    state = 6;
  }
  else if (state == 6)
  {
    StepMotor_controler_main = StepMotor_controler_ENABLE;    // ʹ�ܲ������
    Set_LCD_Point2(Screen_LeftBottom.x, Screen_LeftBottom.y); // ���õ����½�
    state = 7;
  }
  else if (state == 7)
  {
    StepMotor_controler_main = StepMotor_controler_ENABLE;                                                      // ʹ�ܲ������
    Set_LCD_Point2((Screen_LeftBottom.x + Screen_LeftTop.x) / 2, (Screen_LeftTop.y + Screen_LeftBottom.y) / 2); // ���õ������е�
    state = 8;
  }
  else if (state == 8)
  {
    StepMotor_controler_main = StepMotor_controler_ENABLE; // ʹ�ܲ������
    Set_LCD_Point2(Screen_LeftTop.x, Screen_LeftTop.y);    // ���õ����Ͻ�
    state = 9;
  }
  else if (state == 9)
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
    OV2640_DCMI_Resume(); // �ָ�����ͷ
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

void Task_FreeCTRL(void)
{
  
}

void LCD_Show_Screen(void)
{
  LCD_DrawLine2(LCD_W - (Screen_RightBottom.x - 6), LCD_H - (Screen_RightBottom.y - 6), LCD_W - (Screen_LeftBottom.x + 6), LCD_H - (Screen_LeftBottom.y - 6), 2, RED); // ������
  LCD_DrawLine2(LCD_W - (Screen_RightBottom.x - 6), LCD_H - (Screen_RightBottom.y - 6), LCD_W - (Screen_RightTop.x - 6), LCD_H - (Screen_RightTop.y + 6), 2, RED);     // ������
  LCD_DrawLine2(LCD_W - (Screen_LeftBottom.x + 6), LCD_H - (Screen_LeftBottom.y - 6), LCD_W - (Screen_LeftTop.x + 6), LCD_H - (Screen_LeftTop.y + 6), 2, RED);         // ������
  LCD_DrawLine2(LCD_W - (Screen_LeftTop.x + 6), LCD_H - (Screen_LeftTop.y + 6), LCD_W - (Screen_RightTop.x - 6), LCD_H - (Screen_RightTop.y + 6), 2, RED);             // ������
}

void LCD_Show_MyUI(void)
{
  LCD_DrawFillRectangle2(0, 0, 480 - 1, 320 - 1, WHITE); // ����

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
    NextStateFlag = 0; // �����־λ���ȴ������������
    Task_Scan_Screen();
    break;
  case Scan_Seting_A4Target:
    NextStateFlag = 0; // �����־λ���ȴ������������
    Task_Seting_A4Target();
    break;
  case Scan_Freedom_A4Target:
    NextStateFlag = 0; // �����־λ���ȴ������������
    Task_Freedom_A4Target();
    break;
  case FreeCTRL:
    NextStateFlag = 0; // �����־λ���ȴ������������
    Task_FreeCTRL();
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