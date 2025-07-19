#include "main.h"

void FlexiDatas_Show_Conctroller(void);
void FlexiDatas_Show_Enable(void);

uint8_t Buzzer_Volum = 10;        // 蜂鸣器音量
uint16_t Buzzer_Frequency = 1000; // 蜂鸣器频率
uint8_t Buzzer_Flag = 0;          // 蜂鸣器标志
uint16_t Buzzer_Time = 500;       // 蜂鸣器时间，单位ms

uint16_t UpdateUITime = 1000; // UI更新间隔时间
uint8_t UpdateUI_Flag = 0;    // UI更新标志

uint8_t FlexiDatasShowFlag = 0; // 是否使能发送数据
uint8_t FlexiDatasFlag[8];      // 灵活数据标志

uint8_t AngleReadFlag = 0; // 读取角度标志
uint8_t StableFlag = 0;    // 稳定标志
uint8_t TouchFlag = 0;
uint8_t UIClearFlag = 0;

uint8_t TxImageFlag = 0;        // 图像发送标志，1：等待图像获取完成，2：使能发送
uint16_t TxImageWaitTime = 100; // 发送图像等待时间,单位ms

uint8_t tximagetimeflag = 0;

void TXImage(void)
{
  OV2640_DCMI_Resume(); // 恢复摄像头
  TxImageFlag = 1;      // 设置发送图像标志
}

// 使用示例
int main(void)
{
  MPU_Config();       // MPU配置
  SCB_EnableICache(); // 使能ICache
  SCB_EnableDCache(); // 使能DCache
  HAL_Init();
  SystemClock_Config();

  delay_init(480);    // 延时初始化
  USART3_Init();      // USART1初始化
  LED_Init();         // 初始化LED引脚
  AS5600_Init();      // AS5600初始化
  MX_TIM6_Init();     // 1ms定时中断初始化
  QSPI_W25Qxx_Init(); // 初始化W25Q64

  StepMotor_PID_load(); // 加载步进电机PID参数
  ANO_Init();           // ANO协议初始化
  StepMotor_Init();     // 步进电机初始化
  Buzzer_Init();        // 蜂鸣器初始化
  LCD_Init();           // 液晶屏初始化

  CMD_Set(&StepMotorSwich_CMD, "步进电机开关", "切换步进电机开关状态", StepMotor_controler_Swich);
  CMD_Set(&MoveToZero_CMD, "原点复位", "触发时返回原点", Task_Move_to_Zero_Flag);
  CMD_Set(&ScanScreen_CMD, "扫描屏幕", "触发时复位并扫描屏幕", Task_Scan_Screen_Flag);
  CMD_Set(&SetingA4Target_CMD, "扫描设定的A4靶纸", "触发时扫描设定的A4靶纸", Task_Seting_A4Target_Flag);
  CMD_Set(&FreedomA4Target_CMD, "扫描任意的A4靶纸", "触发时扫描任意的A4靶纸", Task_Freedom_A4Target_Flag);

  CMD_Set(&SetStepMotorAngle_CMD, "设置坐标", "设置两步进电机目标角度", Set_StepMotor_Angle);
  CMD_RxValue_Set(&SetStepMotorAngle_CMD, CMD_VAL_FLOAT, CMD_VAL_ID_NO1); // 测试命令
  CMD_RxValue_Set(&SetStepMotorAngle_CMD, CMD_VAL_FLOAT, CMD_VAL_ID_NO2); // 测试命令

  CMD_Set(&DatasSave_CMD, "保存参数", "将参数保存到片外flash中", DatasSave);

  CMD_Set(&FlexiDatasShowEnable_CMD, "使能发送数据", "实际目标角度,目标速度,PID,PID", FlexiDatas_Show_Enable);
  CMD_RxValue_Set(&FlexiDatasShowEnable_CMD, CMD_VAL_UINT8, CMD_VAL_ID_NO1);
  CMD_RxValue_Set(&FlexiDatasShowEnable_CMD, CMD_VAL_UINT8, CMD_VAL_ID_NO2);
  CMD_RxValue_Set(&FlexiDatasShowEnable_CMD, CMD_VAL_UINT8, CMD_VAL_ID_NO3);
  CMD_RxValue_Set(&FlexiDatasShowEnable_CMD, CMD_VAL_UINT8, CMD_VAL_ID_NO4);
  CMD_RxValue_Set(&FlexiDatasShowEnable_CMD, CMD_VAL_UINT8, CMD_VAL_ID_NO5);
  CMD_RxValue_Set(&FlexiDatasShowEnable_CMD, CMD_VAL_UINT8, CMD_VAL_ID_NO6);
  CMD_RxValue_Set(&FlexiDatasShowEnable_CMD, CMD_VAL_UINT8, CMD_VAL_ID_NO7);
  CMD_RxValue_Set(&FlexiDatasShowEnable_CMD, CMD_VAL_UINT8, CMD_VAL_ID_NO8);

  CMD_Set(&BuzzerSwich_CMD, "蜂鸣器开关", "切换蜂鸣器开关,音量,频率", Buzzer_Swich);
  CMD_RxValue_Set(&BuzzerSwich_CMD, CMD_VAL_UINT8, CMD_VAL_ID_NO1);  // 蜂鸣器音量
  CMD_RxValue_Set(&BuzzerSwich_CMD, CMD_VAL_UINT16, CMD_VAL_ID_NO2); // 蜂鸣器频率

  CMD_Set(&SetLCDPoint_CMD, "设置LCD坐标", "设置红点到LCD的像素点上", Set_LCD_Point);
  CMD_RxValue_Set(&SetLCDPoint_CMD, CMD_VAL_UINT16, CMD_VAL_ID_NO1); // X坐标
  CMD_RxValue_Set(&SetLCDPoint_CMD, CMD_VAL_UINT16, CMD_VAL_ID_NO2); // Y坐标

  CMD_Set(&UpdateUI_CMD, "刷新UI", "刷新UI界面", LCD_Show_MyUI);

  CMD_Set(&TXJPEGIMAGE_CMD, "拍照", "拍照并发送图像到上位机", TXImage);

  PAR_Set(&PAR[0], FLOAT, &StepMotor1.kp_angle, "步进电机1", "Kp值");
  PAR_Set(&PAR[1], FLOAT, &StepMotor1.kd_angle, "步进电机1", "Kd值");
  PAR_Set(&PAR[2], FLOAT, &StepMotor2.kp_angle, "步进电机2", "Kp值");
  PAR_Set(&PAR[3], FLOAT, &StepMotor2.kd_angle, "步进电机2", "Kd值");
  PAR_Set(&PAR[4], UINT8, &img_thread, "二值化阈值", "将灰度图二值化的阈值");

  StepMotor_PID_save(); // 保存步进电机PID参数

  LED1_OFF;

  while (DCMI_OV2640_Init())
  {
    delay_ms(100);
  }
  while (TP_Init())
  {
    delay_ms(100);
  }

  OV2640_DMA_Transmit_Continuous(Camera_Buffer, OV2640_BufferSize); // 启动DMA连续传输
  HAL_TIM_Base_Start_IT(&htim6);                                    // 开启定时中断
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
    if (DCMI_FrameState == 1) // 采集到了一帧图像
    {
      DCMI_FrameState = 0; // 清零标志位
      if (Scan_A4Target_Flag == 1)
      {
        N_count++;
      }

      // CPU逐字节交换（优化为32位操作）
      for (uint32_t i = 0; i < Display_Width * Display_Height; i++)
      {
        uint16_t *pixel = (uint16_t *)(Camera_Buffer + 2 * i);
        *pixel = (*pixel << 8) | (*pixel >> 8); // 交换高/低字节
      }
      if (circle_x < 380 && circle_x > 108 && circle_y < 274 && circle_y > 38)
        gui_circle(circle_x, circle_y, WHITE, 6, 1);              // 清除上次的圆圈
      if (color_trace(&target_color_condi, &target_pos_out) == 1) // 颜色跟踪
      {
        circle_x = LCD_W - target_pos_out.x;
        circle_y = LCD_H - target_pos_out.y;
        if (circle_x < 380 && circle_x > 108 && circle_y < 274 && circle_y > 38)
          gui_circle(circle_x, circle_y, RED, 6, 1); // 绘制圆圈
        // printf("x=%d,y=%d", target_pos_out.x, target_pos_out.y);
      }
      if (Scan_A4Target_Flag == 1 && N_count > N_read - 1) // 读取10帧图像
      {
        N_count = 0;
        OV2640_DCMI_Suspend(); // 暂停DMA传输
        delay_ms(500);
        Image_process(); // 图像处理
        NextStateFlag = 1;
      }
    }
    MyStateMachine(); // 状态机
    if (AngleReadFlag == 1)
    {
      AngleReadFlag = 0; // 清零标志位
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
            RxTargetFlag = 0; // 清零标志位
            StableFlag = 0;   // 清零标志位
            Buzzer_Flag = 1;  // 蜂鸣器响
          }
          else if (RxTargetFlag == 2)
          {
            RxTargetFlag = 0;  // 清零标志位
            StableFlag = 0;    // 清零标志位
            NextStateFlag = 1; // 进入下一个状态
            Buzzer_Flag = 1;   // 蜂鸣器响
          }
          else if (RxTargetFlag == 3)
          {
            StableFlag = 0;    // 清零标志位
            NextStateFlag = 1; // 进入下一个状态
            Buzzer_Flag = 1;   // 蜂鸣器响
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
        if ((tp_dev.sta) & (1 << 0)) // 判断是否有点触摸？
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
        if ((tp_dev.sta) & (1 << 0)) // 判断是否有点触摸？
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
        if ((tp_dev.sta) & (1 << 0)) // 判断是否有点触摸？
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
      FlexiDatasShowFlag = 0;        // 清零标志位
      FlexiDatas_Show_Conctroller(); // 发送数据到上位机
    }
    if (TxImageFlag == 1) // 发送图像
    {
      TxImageFlag = 0; // 清零标志位
      // for(uint16_t i = 0; i < Display_Height; i++)
      // {
      //   uint8_t *pixel = (uint8_t*)(Camera_Buffer);
      //   HAL_UART_Transmit(&huart3, pixel + i * Display_Width, Display_Width, 1000); // 发送图像数据
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
    ANO_Protocol_Tx_floatfloatfloatfloat(StepMotor1.target_angle, StepMotor1.current_angle, StepMotor2.target_angle, StepMotor2.current_angle, CH1); // 发送步进电机目标角度和实际角度
    // ANO_Protocol_Tx_floatfloatfloatfloat(angles[0],angles[1], StepMotor2.target_angle, StepMotor2.current_angle, CH1); // 发送步进电机目标角度和实际角度
  }
  if (FlexiDatasFlag[1] == 1)
  {
    ANO_Protocol_Tx_floatfloat(StepMotor1.target_speed, StepMotor2.target_speed, CH2); // 发送步进电机目标速度
    // ANO_Protocol_Tx_floatfloat(StepMotor1.dir, StepMotor2.dir, CH2); // 发送步进电机目标速度
  }
  if (FlexiDatasFlag[2] == 1)
  {
    ANO_Protocol_Tx_floatfloatfloatfloat(StepMotor1.P, StepMotor1.D, StepMotor2.P, StepMotor2.D, CH3); // 发送步进电机PID计算后的P和D值
  }
  if (FlexiDatasFlag[3] == 1)
  {
    // ANO_Protocol_Tx_floatfloatfloatfloat(StepMotor1.P, StepMotor1.D, StepMotor2.P, StepMotor2.D, CH4); // 发送步进电机PID计算后的P和D值
  }
}

uint16_t n = 0;
uint16_t Buzzer_Count = 0;
uint16_t TxImage_Count = 0;
uint16_t TxImageflag_Count = 0;
uint16_t touchscancount = 0;

/*中断处理函数*/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM6)
  {
    n++;
    // 这里放置1ms中断处理代码
    if (StepMotor_controler_main == StepMotor_controler_ENABLE)
    {
      StepMotor1_control();
      StepMotor2_control();
    }

    CMD_Func();
    if (n > 20)
    {
      n = 0;
      AngleReadFlag = 1;      // 20ms刷新一次
      FlexiDatasShowFlag = 1; // 20ms刷新一次
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
