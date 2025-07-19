#include "main.h"

void FlexiDatas_Show_Conctroller(void);
void FlexiDatas_Show_Enable(void);

uint8_t Buzzer_Volum = 10;        // ����������
uint16_t Buzzer_Frequency = 1000; // ������Ƶ��
uint8_t Buzzer_Flag = 0;          // ��������־
uint16_t Buzzer_Time = 500;       // ������ʱ�䣬��λms

uint16_t UpdateUITime = 1000; // UI���¼��ʱ��
uint8_t UpdateUI_Flag = 0;    // UI���±�־

uint8_t FlexiDatasShowFlag = 0; // �Ƿ�ʹ�ܷ�������
uint8_t FlexiDatasFlag[8];      // ������ݱ�־

uint8_t StableFlag = 0; // �ȶ���־

uint8_t key1state = 1;

#define DOTOFFERSET 2

// ʹ��ʾ��
int main(void)
{
  MPU_Config();       // MPU����
  SCB_EnableICache(); // ʹ��ICache
  SCB_EnableDCache(); // ʹ��DCache
  HAL_Init();
  SystemClock_Config();

  delay_init(480); // ��ʱ��ʼ��
  USART3_Init();   // USART1��ʼ��
  UART4_Init();
  LED_Init();         // ��ʼ��LED����
  MX_TIM6_Init();     // 1ms��ʱ�жϳ�ʼ��
  QSPI_W25Qxx_Init(); // ��ʼ��W25Q64

  StepMotor_PID_load(); // ���ز������PID����
  ANO_Init();           // ANOЭ���ʼ��
  StepMotor_Init();     // ���������ʼ��
  Buzzer_Init();        // ��������ʼ��
  SPI_LCD_Init();
  Key_Button_Init();

  CMD_Set(&StepMotorSwich_CMD, "�����������", "�л������������״̬", StepMotor_controler_Swich);
  CMD_Set(&MoveToZero_CMD, "ԭ�㸴λ", "����ʱ����ԭ��", Task_Move_to_Zero_Flag);
  CMD_Set(&ScanScreen_CMD, "ɨ����Ļ", "����ʱ��λ��ɨ����Ļ", Task_Scan_Screen_Flag);
  CMD_Set(&SetingA4Target_CMD, "ɨ���趨��A4��ֽ", "����ʱɨ���趨��A4��ֽ", Task_Seting_A4Target_Flag);
  CMD_Set(&FreedomA4Target_CMD, "ɨ�������A4��ֽ", "����ʱɨ�������A4��ֽ", Task_Freedom_A4Target_Flag);

  CMD_Set(&SetStepMotorAngle_CMD, "��������", "�������������Ŀ��Ƕ�", Set_StepMotor_Angle);
  CMD_RxValue_Set(&SetStepMotorAngle_CMD, CMD_VAL_FLOAT, CMD_VAL_ID_NO1); // ��������
  CMD_RxValue_Set(&SetStepMotorAngle_CMD, CMD_VAL_FLOAT, CMD_VAL_ID_NO2); // ��������

  CMD_Set(&DatasSave_CMD, "�������", "���������浽Ƭ��flash��", DatasSave);

  CMD_Set(&FlexiDatasShowEnable_CMD, "ʹ�ܷ�������", "ʵ��Ŀ��Ƕ�,Ŀ���ٶ�,PID,PID", FlexiDatas_Show_Enable);
  CMD_RxValue_Set(&FlexiDatasShowEnable_CMD, CMD_VAL_UINT8, CMD_VAL_ID_NO1);
  CMD_RxValue_Set(&FlexiDatasShowEnable_CMD, CMD_VAL_UINT8, CMD_VAL_ID_NO2);
  CMD_RxValue_Set(&FlexiDatasShowEnable_CMD, CMD_VAL_UINT8, CMD_VAL_ID_NO3);
  CMD_RxValue_Set(&FlexiDatasShowEnable_CMD, CMD_VAL_UINT8, CMD_VAL_ID_NO4);
  CMD_RxValue_Set(&FlexiDatasShowEnable_CMD, CMD_VAL_UINT8, CMD_VAL_ID_NO5);
  CMD_RxValue_Set(&FlexiDatasShowEnable_CMD, CMD_VAL_UINT8, CMD_VAL_ID_NO6);
  CMD_RxValue_Set(&FlexiDatasShowEnable_CMD, CMD_VAL_UINT8, CMD_VAL_ID_NO7);
  CMD_RxValue_Set(&FlexiDatasShowEnable_CMD, CMD_VAL_UINT8, CMD_VAL_ID_NO8);

  CMD_Set(&BuzzerSwich_CMD, "����������", "�л�����������,����,Ƶ��", Buzzer_Swich);
  CMD_RxValue_Set(&BuzzerSwich_CMD, CMD_VAL_UINT8, CMD_VAL_ID_NO1);  // ����������
  CMD_RxValue_Set(&BuzzerSwich_CMD, CMD_VAL_UINT16, CMD_VAL_ID_NO2); // ������Ƶ��

  CMD_Set(&SetLCDPoint_CMD, "����LCD����", "���ú�㵽LCD�����ص���", Set_LCD_Point);
  CMD_RxValue_Set(&SetLCDPoint_CMD, CMD_VAL_UINT16, CMD_VAL_ID_NO1); // X����
  CMD_RxValue_Set(&SetLCDPoint_CMD, CMD_VAL_UINT16, CMD_VAL_ID_NO2); // Y����

  CMD_Set(&UpdateUI_CMD, "ˢ��UI", "ˢ��UI����", LCD_Show_MyUI);

  PAR_Set(&PAR[0], FLOAT, &StepMotor1.kp_angle, "�������1", "Kpֵ");
  PAR_Set(&PAR[1], FLOAT, &StepMotor1.ki_angle, "�������1", "Kiֵ");
  PAR_Set(&PAR[2], FLOAT, &StepMotor1.kd_angle, "�������1", "Kdֵ");
  PAR_Set(&PAR[3], FLOAT, &StepMotor2.kp_angle, "�������2", "Kpֵ");
  PAR_Set(&PAR[4], FLOAT, &StepMotor2.ki_angle, "�������1", "Kiֵ");
  PAR_Set(&PAR[5], FLOAT, &StepMotor2.kd_angle, "�������2", "Kdֵ");

  StepMotor_PID_save(); // ���沽�����PID����

  LED1_OFF;

  HAL_TIM_Base_Start_IT(&htim6); // ������ʱ�ж�
  LED1_ON;

  float data1 = 0.0f;
  float data2 = 0.0f;
  OPENMV_Dot Last_RxRedDot, Last_RxGreenDot;
  LCD_DrawRect(0, 10, 320, 230);
  while (1)
  {
    if (OPENMV_RxFlag == 1)
    {
      OPENMV_RxFlag = 0;
      // printf("Rx:%d,Ry:%d,Gx:%d,Gy:%d\n", Rx_RedDot.x, Rx_RedDot.y, Rx_GreenDot.x, Rx_GreenDot.y);
      /*����ϴεĵ�*/
      LCD_FillCircle2(Last_RxRedDot.x, Last_RxRedDot.y, 4, LCD_WHITE);
      LCD_FillCircle2(Last_RxGreenDot.x, Last_RxGreenDot.y, 4, LCD_WHITE);
      /*��ʾ���εĵ�*/
      LCD_FillCircle2(Rx_RedDot.x, Rx_RedDot.y, 4, LCD_RED);
      LCD_FillCircle2(Rx_GreenDot.x, Rx_GreenDot.y, 4, LCD_GREEN);
      if (key1state == 1)
      { /*PID����*/
        if(Rx_RedDot.x == 0 && Rx_RedDot.y == 0 && Rx_GreenDot.x == 0 && Rx_GreenDot.y == 0)
        {
          StepMotor_controler_main = StepMotor_controler_DISABLE;
          StepMotor1.target_speed = 0;
                    StepMotor2.target_speed = 0;

        }
        else
        {
          StepMotor_controler_main = StepMotor_controler_ENABLE;
        }
        if(Rx_RedDot.x > Rx_GreenDot.x) 
          data1 = (float)Rx_RedDot.x - DOTOFFERSET;
        else
          data1 = (float)Rx_RedDot.x + DOTOFFERSET;
        data2 = (float)Rx_GreenDot.x;
        StepMotor_PID(&StepMotor1, data1, data2);
        if(Rx_RedDot.y > Rx_GreenDot.y) 
          data1 = (float)Rx_RedDot.y - DOTOFFERSET;
        else
          data1 = (float)Rx_RedDot.y + DOTOFFERSET;
        data2 = (float)Rx_GreenDot.y;
        StepMotor_PID(&StepMotor2, data1, data2);
      }
      Last_RxRedDot = Rx_RedDot;
      Last_RxGreenDot = Rx_GreenDot;
    }
    if (NearFlag == 1)
    {
      Buzzer_On();
    }
    else
    {
      Buzzer_Off();
    }

    if (FlexiDatasShowFlag == 1)
    {
      FlexiDatasShowFlag = 0;        // �����־λ
      FlexiDatas_Show_Conctroller(); // �������ݵ���λ��
    }
  }
}

void FlexiDatas_Show_Enable(void)
{
  uint8_t flag;
  for (uint8_t i = 0; i < 8; i++)
  {
    CMD_RxValue_Read(&FlexiDatasShowEnable_CMD, i, &flag);
    if (flag == 1)
    {
      FlexiDatasFlag[i] = 1;
    }
    else
    {
      FlexiDatasFlag[i] = 0;
    }
  }
}

void FlexiDatas_Show_Conctroller(void)
{
  if (FlexiDatasFlag[0] == 1)
  {
    ANO_Protocol_Tx_floatfloatfloatfloat(StepMotor1.target_angle, StepMotor1.current_angle, StepMotor2.target_angle, StepMotor2.current_angle, CH1); // ���Ͳ������Ŀ��ǶȺ�ʵ�ʽǶ�
    // ANO_Protocol_Tx_floatfloatfloatfloat(angles[0], angles[1], StepMotor2.target_angle, StepMotor2.current_angle, CH1); // ���Ͳ������Ŀ��ǶȺ�ʵ�ʽǶ�
  }
  if (FlexiDatasFlag[1] == 1)
  {
    // ANO_Protocol_Tx_floatfloat(StepMotor1.target_speed, StepMotor2.target_speed, CH2); // ���Ͳ������Ŀ���ٶ�
    // ANO_Protocol_Tx_floatfloat(StepMotor1.dir, StepMotor2.dir, CH2); // ���Ͳ������Ŀ���ٶ�
    ANO_Protocol_Tx_floatfloat(StepMotor1.current_error_angle, StepMotor2.current_error_angle, CH2); // ���Ͳ������Ŀ���ٶ�
    // ANO_Protocol_Tx_floatfloat(StepMotor1.pid_value, StepMotor2.pid_value, CH2); // ���Ͳ������Ŀ���ٶ�
  }
  if (FlexiDatasFlag[2] == 1)
  {
    ANO_Protocol_Tx_floatfloatfloatfloat(StepMotor1.P, StepMotor1.D, StepMotor2.P, StepMotor2.D, CH3); // ���Ͳ������PID������P��Dֵ
  }
  if (FlexiDatasFlag[3] == 1)
  {
    // ANO_Protocol_Tx_floatfloatfloatfloat(StepMotor1.P, StepMotor1.D, StepMotor2.P, StepMotor2.D, CH4); // ���Ͳ������PID������P��Dֵ
  }
}

uint16_t n = 0;
uint16_t Buzzer_Count = 0;
uint16_t UpdateUI_Count = 0;

/*�жϴ�����*/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM6)
  {
    n++;
    UpdateUI_Count++;

    // �������1ms�жϴ������
    if (StepMotor_controler_main == StepMotor_controler_ENABLE)
    {
      StepMotor1_control();
      StepMotor2_control();
    }

    CMD_Func();
    if (n > 20)
    {
      n = 0;
      // AngleReadFlag = 1;      // 20msˢ��һ��
      FlexiDatasShowFlag = 1; // 20msˢ��һ��
      uint8_t buttonstate = Get_Button_State();
      key1state = Get_Key1_State();
      if (key1state == 0)
      {
        StepMotor_controler_main = StepMotor_controler_ENABLE;
        switch (buttonstate)
        {
        case Button_UP:
          StepMotor2.target_speed = StepMotor_speedMIN;
          StepMotor2_DIR_right;
          break;
        case Button_DOWN:
          StepMotor2.target_speed = StepMotor_speedMIN;
          StepMotor2_DIR_left;
          break;
        case Button_LEFT:
          StepMotor1.target_speed = StepMotor_speedMIN;
          StepMotor1_DIR_right;
          break;
        case Button_RIGHT:
          StepMotor1.target_speed = StepMotor_speedMIN;
          StepMotor1_DIR_left;
          break;
        default:
          StepMotor1.target_speed = 0;
          StepMotor2.target_speed = 0;
          break;
        }
      }
      if (buttonstate != 0)
      {
        switch (buttonstate)
        {
        case Button_Click1:
          StepMotor_controler_Swich();
          break;
        case Button_Click2:
          break;
        case Button_Click3:
          break;
        default:
          break;
        }
      }
    }

    if (UpdateUI_Count > UpdateUITime)
    {
      UpdateUI_Count = 0;
      UpdateUI_Flag = 1; // 100msˢ��һ��
    }

    if (Buzzer_Flag == 1)
    {
      Buzzer_On();
      Buzzer_Count++;
      if (Buzzer_Count > Buzzer_Time)
      {
        Buzzer_Count = 0;
        Buzzer_Flag = 0;
        Buzzer_Off();
      }
    }
    else
    {
      Buzzer_Count = 0;
    }
  }
}
