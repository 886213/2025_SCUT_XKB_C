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

uint8_t StableFlag = 0; // 稳定标志

uint8_t key1state = 1;

#define DOTOFFERSET 2

// 使用示例
int main(void)
{
  MPU_Config();       // MPU配置
  SCB_EnableICache(); // 使能ICache
  SCB_EnableDCache(); // 使能DCache
  HAL_Init();
  SystemClock_Config();

  delay_init(480); // 延时初始化
  USART3_Init();   // USART1初始化
  UART4_Init();
  LED_Init();         // 初始化LED引脚
  MX_TIM6_Init();     // 1ms定时中断初始化
  QSPI_W25Qxx_Init(); // 初始化W25Q64

  StepMotor_PID_load(); // 加载步进电机PID参数
  ANO_Init();           // ANO协议初始化
  StepMotor_Init();     // 步进电机初始化
  Buzzer_Init();        // 蜂鸣器初始化
  SPI_LCD_Init();
  Key_Button_Init();

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

  PAR_Set(&PAR[0], FLOAT, &StepMotor1.kp_angle, "步进电机1", "Kp值");
  PAR_Set(&PAR[1], FLOAT, &StepMotor1.ki_angle, "步进电机1", "Ki值");
  PAR_Set(&PAR[2], FLOAT, &StepMotor1.kd_angle, "步进电机1", "Kd值");
  PAR_Set(&PAR[3], FLOAT, &StepMotor2.kp_angle, "步进电机2", "Kp值");
  PAR_Set(&PAR[4], FLOAT, &StepMotor2.ki_angle, "步进电机1", "Ki值");
  PAR_Set(&PAR[5], FLOAT, &StepMotor2.kd_angle, "步进电机2", "Kd值");

  StepMotor_PID_save(); // 保存步进电机PID参数

  LED1_OFF;

  HAL_TIM_Base_Start_IT(&htim6); // 开启定时中断
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
      /*清除上次的点*/
      LCD_FillCircle2(Last_RxRedDot.x, Last_RxRedDot.y, 4, LCD_WHITE);
      LCD_FillCircle2(Last_RxGreenDot.x, Last_RxGreenDot.y, 4, LCD_WHITE);
      /*显示本次的点*/
      LCD_FillCircle2(Rx_RedDot.x, Rx_RedDot.y, 4, LCD_RED);
      LCD_FillCircle2(Rx_GreenDot.x, Rx_GreenDot.y, 4, LCD_GREEN);
      if (key1state == 1)
      { /*PID控制*/
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
      FlexiDatasShowFlag = 0;        // 清零标志位
      FlexiDatas_Show_Conctroller(); // 发送数据到上位机
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
    // ANO_Protocol_Tx_floatfloatfloatfloat(angles[0], angles[1], StepMotor2.target_angle, StepMotor2.current_angle, CH1); // 发送步进电机目标角度和实际角度
  }
  if (FlexiDatasFlag[1] == 1)
  {
    // ANO_Protocol_Tx_floatfloat(StepMotor1.target_speed, StepMotor2.target_speed, CH2); // 发送步进电机目标速度
    // ANO_Protocol_Tx_floatfloat(StepMotor1.dir, StepMotor2.dir, CH2); // 发送步进电机目标速度
    ANO_Protocol_Tx_floatfloat(StepMotor1.current_error_angle, StepMotor2.current_error_angle, CH2); // 发送步进电机目标速度
    // ANO_Protocol_Tx_floatfloat(StepMotor1.pid_value, StepMotor2.pid_value, CH2); // 发送步进电机目标速度
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
uint16_t UpdateUI_Count = 0;

/*中断处理函数*/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM6)
  {
    n++;
    UpdateUI_Count++;

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
      // AngleReadFlag = 1;      // 20ms刷新一次
      FlexiDatasShowFlag = 1; // 20ms刷新一次
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
      UpdateUI_Flag = 1; // 100ms刷新一次
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
