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

uint8_t AngleReadFlag = 0; // ��ȡ�Ƕȱ�־
uint8_t StableFlag = 0;    // �ȶ���־
uint8_t TouchFlag = 0;
uint8_t UIClearFlag = 0;

uint8_t TxImageFlag = 0;        // ͼ���ͱ�־��1���ȴ�ͼ���ȡ��ɣ�2��ʹ�ܷ���
uint16_t TxImageWaitTime = 100; // ����ͼ��ȴ�ʱ��,��λms

uint8_t tximagetimeflag = 0;

void TXImage(void)
{
  OV2640_DCMI_Resume(); // �ָ�����ͷ
  TxImageFlag = 1;      // ���÷���ͼ���־
}

// ʹ��ʾ��
int main(void)
{
  MPU_Config();       // MPU����
  SCB_EnableICache(); // ʹ��ICache
  SCB_EnableDCache(); // ʹ��DCache
  HAL_Init();
  SystemClock_Config();

  delay_init(480);    // ��ʱ��ʼ��
  USART3_Init();      // USART1��ʼ��
  LED_Init();         // ��ʼ��LED����
  AS5600_Init();      // AS5600��ʼ��
  MX_TIM6_Init();     // 1ms��ʱ�жϳ�ʼ��
  QSPI_W25Qxx_Init(); // ��ʼ��W25Q64

  StepMotor_PID_load(); // ���ز������PID����
  ANO_Init();           // ANOЭ���ʼ��
  StepMotor_Init();     // ���������ʼ��
  Buzzer_Init();        // ��������ʼ��
  LCD_Init();           // Һ������ʼ��

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

  CMD_Set(&TXJPEGIMAGE_CMD, "����", "���ղ�����ͼ����λ��", TXImage);

  PAR_Set(&PAR[0], FLOAT, &StepMotor1.kp_angle, "�������1", "Kpֵ");
  PAR_Set(&PAR[1], FLOAT, &StepMotor1.kd_angle, "�������1", "Kdֵ");
  PAR_Set(&PAR[2], FLOAT, &StepMotor2.kp_angle, "�������2", "Kpֵ");
  PAR_Set(&PAR[3], FLOAT, &StepMotor2.kd_angle, "�������2", "Kdֵ");
  PAR_Set(&PAR[4], UINT8, &img_thread, "��ֵ����ֵ", "���Ҷ�ͼ��ֵ������ֵ");

  StepMotor_PID_save(); // ���沽�����PID����

  LED1_OFF;

  while (DCMI_OV2640_Init())
  {
    delay_ms(100);
  }
  while (TP_Init())
  {
    delay_ms(100);
  }

  OV2640_DMA_Transmit_Continuous(Camera_Buffer, OV2640_BufferSize); // ����DMA��������
  HAL_TIM_Base_Start_IT(&htim6);                                    // ������ʱ�ж�
  LED1_ON;

  uint8_t N_read = 10;
  uint8_t N_count = 0;

  uint16_t circle_x = 0;
  uint16_t circle_y = 0;

  uint8_t N_touch = 0;

  // Scan_A4Target_Flag = 1;

  LCD_Show_MyUI();
  while (1)
  {
    if (DCMI_FrameState == 1) // �ɼ�����һ֡ͼ��
    {
      DCMI_FrameState = 0; // �����־λ
      if (Scan_A4Target_Flag == 1)
      {
        N_count++;
      }

      // CPU���ֽڽ������Ż�Ϊ32λ������
      for (uint32_t i = 0; i < Display_Width * Display_Height; i++)
      {
        uint16_t *pixel = (uint16_t *)(Camera_Buffer + 2 * i);
        *pixel = (*pixel << 8) | (*pixel >> 8); // ������/���ֽ�
      }
      if (circle_x < 380 && circle_x > 108 && circle_y < 274 && circle_y > 38)
        gui_circle(circle_x, circle_y, WHITE, 6, 1);              // ����ϴε�ԲȦ
      if (color_trace(&target_color_condi, &target_pos_out) == 1) // ��ɫ����
      {
        circle_x = LCD_W - target_pos_out.x;
        circle_y = LCD_H - target_pos_out.y;
        if (circle_x < 380 && circle_x > 108 && circle_y < 274 && circle_y > 38)
          gui_circle(circle_x, circle_y, RED, 6, 1); // ����ԲȦ
        // printf("x=%d,y=%d", target_pos_out.x, target_pos_out.y);
      }
      if (Scan_A4Target_Flag == 1 && N_count > N_read - 1) // ��ȡ10֡ͼ��
      {
        N_count = 0;
        OV2640_DCMI_Suspend(); // ��ͣDMA����
        delay_ms(500);
        Image_process(); // ͼ����
        NextStateFlag = 1;
      }
    }
    MyStateMachine(); // ״̬��
    if (AngleReadFlag == 1)
    {
      AngleReadFlag = 0; // �����־λ
      angles[0] = Read_AS5600_Angle_Single(0);
      angles[1] = Read_AS5600_Angle_Single(1);
      if (StepMotor_controler_main == StepMotor_controler_ENABLE)
      {
        if (RxTargetFlag == 1)
        {
          StepMotor_PID(&StepMotor1, RxTarget.x, angles[0]);
          StepMotor_PID(&StepMotor2, RxTarget.y, angles[1]);
          StableFlag = StepMotor_Stable_Judge();
        }
        else if (RxTargetFlag == 2)
        {
          StepMotor_PID(&StepMotor1, RxTarget.x, angles[0]);
          StepMotor_PID(&StepMotor2, RxTarget.y, angles[1]);
          StableFlag = StepMotor_Stable_Judge();
        }
        else if (RxTargetFlag == 3)
        {
          StepMotor_PID(&StepMotor1, RxTarget.x, angles[0]);
          StepMotor_PID(&StepMotor2, RxTarget.y, angles[1]);
          StableFlag = StepMotor_Stable_Judge();
        }
        if (StableFlag == 1)
        {
          if (RxTargetFlag == 1)
          {
            RxTargetFlag = 0; // �����־λ
            StableFlag = 0;   // �����־λ
            Buzzer_Flag = 1;  // ��������
          }
          else if (RxTargetFlag == 2)
          {
            RxTargetFlag = 0;  // �����־λ
            StableFlag = 0;    // �����־λ
            NextStateFlag = 1; // ������һ��״̬
            Buzzer_Flag = 1;   // ��������
          }
          else if (RxTargetFlag == 3)
          {
            StableFlag = 0;    // �����־λ
            NextStateFlag = 1; // ������һ��״̬
            Buzzer_Flag = 1;   // ��������
          }
        }
      }
    }
    if (TouchFlag == 1)
    {
      TouchFlag = 0;
      if (main_task == Nonetask)
      {
        tp_dev.scan();
        if ((tp_dev.sta) & (1 << 0)) // �ж��Ƿ��е㴥����
        {
          if (N_touch < 1)
          {
            N_touch++;
          }
          else
          {
            N_touch = 0;
            uint16_t touchx = LCD_W - tp_dev.y[0];
            uint16_t touchy = tp_dev.x[0];
            // printf("x=%d,y=%d\n",touchx, touchy);
            if ((touchx >= (10)) && (touchx <= (90)) && (touchy > 10) && (touchy < 50))
            {
              main_task = Move_Origin;
            }
            else if ((touchx >= (10)) && (touchx <= (90)) && (touchy > 60) && (touchy < 100))
            {
              main_task = Scan_Screen;
            }
            else if ((touchx >= (10)) && (touchx <= (90)) && (touchy > 110) && (touchy < 150))
            {
              main_task = Scan_Freedom_A4Target;
            }
            else if ((touchx >= (10)) && (touchx <= (90)) && (touchy > 160) && (touchy < 200))
            {
              main_task = FreeCTRL;
              LCD_ShowString(100, 10, 16, "Click on the screen to move the RedDot!", 1);
            }
            else if ((touchx >= (10)) && (touchx <= (90)) && (touchy > 210) && (touchy < 250))
            {
              UIClearFlag = 1;
            }
          }
        }
      }
      else if (main_task == FreeCTRL)
      {
        tp_dev.scan();
        if ((tp_dev.sta) & (1 << 0)) // �ж��Ƿ��е㴥����
        {
          if (N_touch < 2)
          {
            N_touch++;
          }
          else
          {
            N_touch = 0;
            uint16_t touchx = LCD_W - tp_dev.y[0];
            uint16_t touchy = tp_dev.x[0];
            if ((touchx >= (10)) && (touchx <= (90)) && (touchy > 160) && (touchy < 200))
            {
              main_task = Nonetask;
              LCD_Show_MyUI();
            }
            else if ((touchx <= (LCD_W - (Screen_RightBottom.x - 6))) && (touchx >= (LCD_W - (Screen_LeftBottom.x + 6))) && (touchy < (LCD_H - (Screen_RightBottom.y - 6))) && (touchy > (LCD_H - (Screen_RightTop.y + 6))))
            {
              gui_circle(touchx, touchy, GREEN, 4, 1);
              StepMotor_controler_main = StepMotor_controler_ENABLE;
              Set_LCD_Point2(LCD_W - touchx, LCD_H - touchy);
            }
            else if ((touchx >= (10)) && (touchx <= (90)) && (touchy > 210) && (touchy < 250))
            {
              UIClearFlag = 1;
            }
          }
        }
      }
      else
      {
        tp_dev.scan();
        if ((tp_dev.sta) & (1 << 0)) // �ж��Ƿ��е㴥����
        {
          uint16_t touchx = LCD_W - tp_dev.y[0];
          uint16_t touchy = tp_dev.x[0];
          if ((touchx >= (10)) && (touchx <= (90)) && (touchy > 260) && (touchy < 300))
          {
            StepMotor_controler_main = StepMotor_controler_DISABLE;
          }
          else
          {
            StepMotor_controler_main = StepMotor_controler_ENABLE;
          }
        }
      }
    }
    if (UIClearFlag == 1)
    {
      UIClearFlag = 0;
      LCD_Show_MyUI();
      if (main_task == FreeCTRL)
      {
        LCD_ShowString(100, 10, 16, "Click on the screen to move the RedDot!", 1);
      }
    }
    if (FlexiDatasShowFlag == 1)
    {
      FlexiDatasShowFlag = 0;        // �����־λ
      FlexiDatas_Show_Conctroller(); // �������ݵ���λ��
    }
    if (TxImageFlag == 1) // ����ͼ��
    {
      TxImageFlag = 0; // �����־λ
      // for(uint16_t i = 0; i < Display_Height; i++)
      // {
      //   uint8_t *pixel = (uint8_t*)(Camera_Buffer);
      //   HAL_UART_Transmit(&huart3, pixel + i * Display_Width, Display_Width, 1000); // ����ͼ������
      // }
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
    // ANO_Protocol_Tx_floatfloatfloatfloat(angles[0],angles[1], StepMotor2.target_angle, StepMotor2.current_angle, CH1); // ���Ͳ������Ŀ��ǶȺ�ʵ�ʽǶ�
  }
  if (FlexiDatasFlag[1] == 1)
  {
    ANO_Protocol_Tx_floatfloat(StepMotor1.target_speed, StepMotor2.target_speed, CH2); // ���Ͳ������Ŀ���ٶ�
    // ANO_Protocol_Tx_floatfloat(StepMotor1.dir, StepMotor2.dir, CH2); // ���Ͳ������Ŀ���ٶ�
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
uint16_t TxImage_Count = 0;
uint16_t TxImageflag_Count = 0;
uint16_t touchscancount = 0;

/*�жϴ�����*/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM6)
  {
    n++;
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
      AngleReadFlag = 1;      // 20msˢ��һ��
      FlexiDatasShowFlag = 1; // 20msˢ��һ��
      TouchFlag = 1;
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

    if (TxImageFlag == 1)
    {
      TxImage_Count++;
      if (TxImage_Count > TxImageWaitTime)
      {
        TxImage_Count = 0;
        TxImageFlag = 2;
      }
    }

    if (TouchFlag == 0)
    {
      touchscancount++;
      if (touchscancount > 40)
      {
        TouchFlag = 1;
      }
    }

    if (tximagetimeflag == 1)
    {
      TxImageflag_Count++;
      if (TxImageflag_Count > 1000)
      {
        TxImageflag_Count = 0;
        tximagetimeflag = 0;
        TXImage();
      }
    }
  }
}
