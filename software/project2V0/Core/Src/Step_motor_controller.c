#include "Step_motor_controller.h"

StepMotor StepMotor1, StepMotor2;

StepMotor_controler StepMotor_controler_main = StepMotor_controler_DISABLE;

// 每4KB为一个数据块，步进电机数据存储在W25Qxx的0x000000~0x000FFF地址处，红点坐标数据存储在W25Qxx的0x001000~0x001FFF地址处

/*步进电机数据数组格式：步进电机1的kp、ki、kd；步进电机2的kp、ki、kd全是float数据，共24字节*/
#define StepMotor_W25Qxx_DataSize 4 * 1024                  // 4KB步进电机控制参数数据存储大小
uint8_t StepMotor_W25Qxx_W_Data[StepMotor_W25Qxx_DataSize]; // 4KB步进电机控制参数写数据数组
uint8_t StepMotor_W25Qxx_R_Data[StepMotor_W25Qxx_DataSize]; // 4KB步进电机控制参数读数据数组
uint32_t StepMotor_W25Qxx_DataAddr = 0x000000;              // 步进电机数据存储地址
int32_t StepMotor_W25Qxx_status;                            // 步进电机数据存储状态

/*红点坐标数据数组格式：每一个点有x，y（float类型），8字节，最多存储4KB/8=512个红点坐标数据*/
#define RedDots_W25Qxx_DataSize 4 * 1024                // 4KB红点坐标数据存储大小
uint8_t RedDots_W25Qxx_W_Data[RedDots_W25Qxx_DataSize]; // 4KB红点坐标读数据数组
uint8_t RedDots_W25Qxx_R_Data[RedDots_W25Qxx_DataSize]; // 4KB红点坐标读数据数组
uint32_t RedDots_W25Qxx_DataAddr = 0x001000;            // 红点坐标数据存储地址
int32_t RedDots_W25Qxx_status;                          // 红点坐标数据存储状态

// 从LCD像素点到步进电机坐标的转换系数，依次为[1,x,y,x^2,xy,y^2]
float StepMotor1_coef[6]={3.631357, 0.105766, 0.005880, -0.000004, -0.000011, -0.000007};
float StepMotor2_coef[6]={54.302408, 0.004356, 0.126166, -0.000016, 0.000003, -0.000047};

uint8_t RxTargetFlag = 0;
RedDot_coordinate origin, RxTarget;
RedDot_coordinate MovePoint, LastMovePoint; // 移动点坐标和上次移动点坐标

#define W25Qxx_NumByteToTest 32 * 1024 // 测试数据的长度，64K

int32_t QSPI_Status; // 检测标志位

uint32_t W25Qxx_TestAddr = 0;                     // 测试地址
uint8_t W25Qxx_WriteBuffer[W25Qxx_NumByteToTest]; //	写数据数组
uint8_t W25Qxx_ReadBuffer[W25Qxx_NumByteToTest];  //	读数据数组
int8_t QSPI_W25Qxx_Test(void)                     // Flash读写测试
{
  uint32_t i = 0;               // 计数变量
  uint32_t ExecutionTime_Begin; // 开始时间
  uint32_t ExecutionTime_End;   // 结束时间
  uint32_t ExecutionTime;       // 执行时间
  float ExecutionSpeed;         // 执行速度

  // 擦除 >>>>>>>

  ExecutionTime_Begin = HAL_GetTick();                       // 获取 systick 当前时间，单位ms
  QSPI_Status = QSPI_W25Qxx_BlockErase_32K(W25Qxx_TestAddr); // 擦除32K字节
  ExecutionTime_End = HAL_GetTick();                         // 获取 systick 当前时间，单位ms

  ExecutionTime = ExecutionTime_End - ExecutionTime_Begin; // 计算擦除时间，单位ms

  if (QSPI_Status == QSPI_W25Qxx_OK)
  {
    printf("\r\n1");
  }
  else
  {
    printf("\r\n-1");
    while (1)
      ;
  }

  // 写入 >>>>>>>

  for (i = 0; i < W25Qxx_NumByteToTest; i++) // 先将数据写入数组
  {
    W25Qxx_WriteBuffer[i] = i;
  }
  ExecutionTime_Begin = HAL_GetTick();                                                              // 获取 systick 当前时间，单位ms
  QSPI_Status = QSPI_W25Qxx_WriteBuffer(W25Qxx_WriteBuffer, W25Qxx_TestAddr, W25Qxx_NumByteToTest); // 写入数据
  ExecutionTime_End = HAL_GetTick();                                                                // 获取 systick 当前时间，单位ms

  ExecutionTime = ExecutionTime_End - ExecutionTime_Begin;      // 计算擦除时间，单位ms
  ExecutionSpeed = (float)W25Qxx_NumByteToTest / ExecutionTime; // 计算写入速度，单位 KB/S
  if (QSPI_Status == QSPI_W25Qxx_OK)
  {
    printf("\r\n2");
  }
  else
  {
    printf("\r\n-2");
    while (1)
      ;
  }

  // 读取 >>>>>>>

  ExecutionTime_Begin = HAL_GetTick();                                                            // 获取 systick 当前时间，单位ms
  QSPI_Status = QSPI_W25Qxx_ReadBuffer(W25Qxx_ReadBuffer, W25Qxx_TestAddr, W25Qxx_NumByteToTest); // 读取数据
  ExecutionTime_End = HAL_GetTick();                                                              // 获取 systick 当前时间，单位ms

  ExecutionTime = ExecutionTime_End - ExecutionTime_Begin;             // 计算擦除时间，单位ms
  ExecutionSpeed = (float)W25Qxx_NumByteToTest / ExecutionTime / 1024; // 计算读取速度，单位 MB/S

  if (QSPI_Status == QSPI_W25Qxx_OK)
  {
    printf("\r\n3");
  }
  else
  {
    printf("\r\n-3");
    while (1)
      ;
  }

  // 数据校验 >>>>>>>

  for (i = 0; i < W25Qxx_NumByteToTest; i++) // 验证读出的数据是否等于写入的数据
  {
    if (W25Qxx_WriteBuffer[i] != W25Qxx_ReadBuffer[i]) // 如果数据不相等，则返回0
    {
      printf("\r\n-4\r\n");
      while (1)
        ;
    }
  }
  printf("\r\n4\r\n");

  return QSPI_W25Qxx_OK; // 测试通过
}

/**
 * @brief  步进电机控制器开关函数，切换步进电机控制器的使能状态
 * @retval None
 */
void StepMotor_controler_Swich(void)
{
  if (StepMotor_controler_main == StepMotor_controler_DISABLE)
  {
    StepMotor_controler_main = StepMotor_controler_ENABLE;
  }
  else
  {
    StepMotor_controler_main = StepMotor_controler_DISABLE;
  }
}

void Set_StepMotor_Angle(void)
{
  float Motor1Angle, Motor2Angle;
  CMD_RxValue_Read(&SetStepMotorAngle_CMD, CMD_VAL_ID_NO1, &Motor1Angle); // 步进电机1的角度
  CMD_RxValue_Read(&SetStepMotorAngle_CMD, CMD_VAL_ID_NO2, &Motor2Angle); // 步进电机2的角度
  // 设置步进电机的目标角度，确保角度在合理范围内
  int16_t Motor1Angle_int = (int16_t)(Motor1Angle / STEPMOTOR_ANGLE_PER_STEP); // 步进电机1的角度
  int16_t Motor2Angle_int = (int16_t)(Motor2Angle / STEPMOTOR_ANGLE_PER_STEP); // 步进电机2的角度
  float Motor1Angle_float = (float)Motor1Angle_int * STEPMOTOR_ANGLE_PER_STEP; // 步进电机1的角度
  float Motor2Angle_float = (float)Motor2Angle_int * STEPMOTOR_ANGLE_PER_STEP; // 步进电机2的角度
  // 限制角度范围在0-360度之间
  if (Motor1Angle_float > 0 && Motor2Angle_float > 0 && Motor1Angle_float < 360 && Motor2Angle_float < 360)
  {
    RxTarget.x = Motor1Angle_float; // 步进电机1的目标角度
    RxTarget.y = Motor2Angle_float; // 步进电机2的目标角度
    RxTargetFlag = 1;               // 设置接收标志位
  }
}

void Set_LCD_Point(void)
{
  uint16_t x, y;
  CMD_RxValue_Read(&SetLCDPoint_CMD, CMD_VAL_ID_NO1, &x); // LCD的X坐标
  CMD_RxValue_Read(&SetLCDPoint_CMD, CMD_VAL_ID_NO2, &y); // LCD的Y坐标
  float X = (float)x; // LCD的X坐标
  float Y = (float)y; // LCD的Y坐标
  // 根据LCD的坐标映射到步进电机的角度
  float Motor1Angle_float = StepMotor1_coef[0] + StepMotor1_coef[1] * X + StepMotor1_coef[2] * Y + 
                            StepMotor1_coef[3] * X * X + StepMotor1_coef[4] * X * Y + StepMotor1_coef[5] * Y * Y;
  float Motor2Angle_float = StepMotor2_coef[0] + StepMotor2_coef[1] * X + StepMotor2_coef[2] * Y +
                            StepMotor2_coef[3] * X * X + StepMotor2_coef[4] * X * Y + StepMotor2_coef[5] * Y * Y;
  // 限制角度范围在0-360度之间
  if (Motor1Angle_float > 0 && Motor2Angle_float > 0 && Motor1Angle_float < 360 && Motor2Angle_float < 360)
  {
    RxTarget.x = Motor1Angle_float; // 步进电机1的目标角度
    RxTarget.y = Motor2Angle_float; // 步进电机2的目标角度
    RxTargetFlag = 1;               // 设置接收标志位
  }
}

void Set_LCD_Point2(uint16_t x, uint16_t y)
{
  float X = (float)x; // LCD的X坐标
  float Y = (float)y; // LCD的Y坐标
  // 根据LCD的坐标映射到步进电机的角度
  float Motor1Angle_float = StepMotor1_coef[0] + StepMotor1_coef[1] * X + StepMotor1_coef[2] * Y +
                            StepMotor1_coef[3] * X * X + StepMotor1_coef[4] * X * Y + StepMotor1_coef[5] * Y * Y;
  float Motor2Angle_float = StepMotor2_coef[0] + StepMotor2_coef[1] * X + StepMotor2_coef[2] * Y +
                            StepMotor2_coef[3] * X * X + StepMotor2_coef[4] * X * Y + StepMotor2_coef[5] * Y * Y;
  uint16_t Motor1Angle_int = (uint16_t)(Motor1Angle_float / STEPMOTOR_ANGLE_PER_STEP); // 步进电机1的角度
  uint16_t Motor2Angle_int = (uint16_t)(Motor2Angle_float / STEPMOTOR_ANGLE_PER_STEP); // 步进电机2的角度
  Motor1Angle_float = (float)Motor1Angle_int * STEPMOTOR_ANGLE_PER_STEP; // 步进电机1的角度
  Motor2Angle_float = (float)Motor2Angle_int * STEPMOTOR_ANGLE_PER_STEP; // 步进电机2的角度                          
  // 限制角度范围在0-360度之间
  if (Motor1Angle_float > 0 && Motor2Angle_float > 0 && Motor1Angle_float < 360 && Motor2Angle_float < 360)
  {
    RxTarget.x = Motor1Angle_float; // 步进电机1的目标角度
    RxTarget.y = Motor2Angle_float; // 步进电机2的目标角度
    RxTargetFlag = 2;               // 设置接收标志位
  }
}

/**
 * @brief  二维步进电机云台控制函数，放定时中断中，根据步进电机的当前状态和目标速度，输出宽度为1单位定时中断周期的短脉冲，控制步进电机的转动，速度越高，脉冲频率越高，最高频率为定时中断频率/2
 * @retval None
 */
void StepMotor1_control(void)
{
  if (StepMotor1.current_val == StepMotor_VAL_ENABLE)
  {
    StepMotor1_STOP;
    StepMotor1.current_val = StepMotor_VAL_DISABLE;
    StepMotor1.count = 0;
    return;
  }
  if (StepMotor1.target_speed == 0)
  {
    return;
  }
  if (StepMotor1.count < 1001 - StepMotor1.target_speed)
  {
    StepMotor1.count++;
  }
  else
  {
    StepMotor1_STEP;
    StepMotor1.current_val = StepMotor_VAL_ENABLE;
  }
}

void StepMotor2_control(void)
{
  if (StepMotor2.current_val == StepMotor_VAL_ENABLE)
  {
    StepMotor2_STOP;
    StepMotor2.current_val = StepMotor_VAL_DISABLE;
    StepMotor2.count = 0;
    return;
  }
  if (StepMotor2.target_speed == 0)
  {
    return;
  }
  if (StepMotor2.count < 1001 - StepMotor2.target_speed)
  {
    StepMotor2.count++;
  }
  else
  {
    StepMotor2_STEP;
    StepMotor2.current_val = StepMotor_VAL_ENABLE;
  }
}

void StepMotor_PID(StepMotor *motor, float target_angle, float current_angle)
{
  motor->target_angle = target_angle;
  motor->current_angle = current_angle;

  motor->current_error_angle = motor->target_angle - motor->current_angle;

  motor->P = motor->kp_angle * motor->current_error_angle;
  motor->I += motor->ki_angle * motor->current_error_angle;
  motor->D = motor->kd_angle * (motor->current_error_angle - motor->last_error_angle);

  motor->pid_value = motor->P + motor->I + motor->D;

  motor->last_dir = motor->dir; // 记录上次的方向

  if (motor->pid_value > 0)
  {
    motor->dir = StepMotor_DIR_LEFT;
    switch (motor->ID)
    {
    case NO1:
      StepMotor1_DIR_left;
      break;
    case NO2:
      StepMotor2_DIR_left;
      break;
    default:
      break;
    }
  }
  else
  {
    motor->dir = StepMotor_DIR_RIGHT;
    switch (motor->ID)
    {
    case NO1:
      StepMotor1_DIR_right;
      break;
    case NO2:
      StepMotor2_DIR_right;
      break;
    default:
      break;
    }
    motor->pid_value = -motor->pid_value;
  }

  motor->last_error_angle = motor->current_error_angle;

  motor->target_speed = (uint16_t)(motor->pid_value / 1) > (StepMotor_speedMAX - StepMotor_speedMIN) ? StepMotor_speedMAX : ((uint16_t)(motor->pid_value / 1) + StepMotor_speedMIN); // 步进电机的目标速度
}

void StepMotor_PID2(StepMotor *motor, float target_angle, float current_angle)
{
  if (target_angle > motor->angle_max)
  {
    target_angle = motor->angle_max;
  }
  else if (target_angle < motor->angle_min)
  {
    target_angle = motor->angle_min;
  }
  motor->target_angle = target_angle;
  motor->current_angle = current_angle;

  motor->current_error_angle = motor->target_angle - motor->current_angle;

  motor->P = motor->kp_angle * motor->current_error_angle;
  motor->I += motor->ki_angle * motor->current_error_angle;
  motor->D = motor->kd_angle * (motor->current_error_angle - motor->last_error_angle);

  motor->pid_value = motor->P + motor->I + motor->D;

  motor->last_dir = motor->dir; // 记录上次的方向

  if (motor->pid_value > 0)
  {
    motor->dir = StepMotor_DIR_LEFT;
    switch (motor->ID)
    {
    case NO1:
      StepMotor1_DIR_left;
      break;
    case NO2:
      StepMotor2_DIR_left;
      break;
    default:
      break;
    }
  }
  else
  {
    motor->dir = StepMotor_DIR_RIGHT;
    switch (motor->ID)
    {
    case NO1:
      StepMotor1_DIR_right;
      break;
    case NO2:
      StepMotor2_DIR_right;
      break;
    default:
      break;
    }
    motor->pid_value = -motor->pid_value;
  }

  motor->last_error_angle = motor->current_error_angle;

  motor->target_speed = (int16_t)(motor->pid_value / STEPMOTOR_ANGLE_PER_STEP) > 100 ? StepMotor_speedMAX : ((int16_t)(motor->pid_value / STEPMOTOR_ANGLE_PER_STEP) + StepMotor_speedMIN); // 步进电机的目标速度
}

/**
 * @brief  步进电机稳定情况判断函数，当步进电机当前角度基本稳定到目标角度且维持一定时间时，只能置StepMotor_controler_main为0，不给予启动权限
 * @retval None
 **/
uint8_t StepMotor_Stable_Judge(void)
{
  static uint16_t count1 = 0; // 步进电机稳定判断阈值
  static uint16_t count2 = 0; // 步进电机稳定判断阈值
  // if ((StepMotor1.target_angle - StepMotor1.current_angle < StepMotor_Stable_threshold * STEPMOTOR_ANGLE_PER_STEP) || (StepMotor1.current_angle - StepMotor1.target_angle < StepMotor_Stable_threshold * STEPMOTOR_ANGLE_PER_STEP))
  // {
  //   if ((StepMotor2.target_angle - StepMotor2.current_angle < StepMotor_Stable_threshold * STEPMOTOR_ANGLE_PER_STEP) || (StepMotor2.current_angle - StepMotor2.target_angle < StepMotor_Stable_threshold * STEPMOTOR_ANGLE_PER_STEP))
  //   {
  //     count++;
  //   }
  //   else
  //   {
  //     count = 0;
  //   }
  // }
  if(StepMotor1.target_angle == StepMotor1.current_angle)
  {
    count1++;
  }
  if(StepMotor2.target_angle == StepMotor2.current_angle)
  {
    count2++;
  }
  // else
  // {
  //   count = 0;
  // }
  if (count1 > StepMotor_Stable_count && count2 > StepMotor_Stable_count)
  {
    // 步进电机稳定, 可以停止
    StepMotor_controler_main = StepMotor_controler_DISABLE;
    StepMotor1.target_angle = StepMotor1.current_angle;
    StepMotor2.target_angle = StepMotor2.current_angle;
    StepMotor1.target_speed = 0;
    StepMotor2.target_speed = 0;
    count1 = 0;
    count2 = 0;
    return 1; // 步进电机稳定
  }
  return 0; // 步进电机不稳定
}

void StepMotor_GetAngle(StepMotor *motor)
{
  motor->current_angle = Read_AS5600_Angle_Single(motor->ID);
}

void RedDots_W25Qxx_Data_load(void)
{
  RedDots_W25Qxx_status = QSPI_W25Qxx_ReadBuffer(RedDots_W25Qxx_R_Data, RedDots_W25Qxx_DataAddr, RedDots_W25Qxx_DataSize);
  if (RedDots_W25Qxx_status != QSPI_W25Qxx_OK)
  {
    printf("Read RedDots Data Error!\r\n");
    return;
  }
  printf("Load RedDots Data Success!\r\n");

  // unsigned char *byte_ptr = (unsigned char *)&origin;
  // int32_t i = 0;
  // for (i = 0; i < sizeof(origin); i++)
  // {
  //   *(byte_ptr + i) = RedDots_W25Qxx_Data[i];
  // }

  union
  {
    float float_data;
    uint32_t u32_data;
  } converter;

  // 将数组中的数据转换为浮点数
  converter.u32_data = *(uint32_t *)(RedDots_W25Qxx_R_Data);
  origin.x = converter.float_data;
  converter.u32_data = *(uint32_t *)(RedDots_W25Qxx_R_Data + 4);
  origin.y = converter.float_data;
}

void RedDots_W25Qxx_Data_save(void)
{
  union
  {
    float float_data;
    uint32_t u32_data;
  } converter;

  converter.float_data = origin.x;
  uint32_t temp = converter.u32_data;

  // 提取每个字节存储到数组中
  RedDots_W25Qxx_W_Data[0] = (temp >> 0) & 0xFF;  // 提取最低字节
  RedDots_W25Qxx_W_Data[1] = (temp >> 8) & 0xFF;  // 提取次低字节
  RedDots_W25Qxx_W_Data[2] = (temp >> 16) & 0xFF; // 提取次高字节
  RedDots_W25Qxx_W_Data[3] = (temp >> 24) & 0xFF; // 提取最高字节

  converter.float_data = origin.y;
  temp = converter.u32_data;

  // 提取每个字节存储到数组中
  RedDots_W25Qxx_W_Data[4] = (temp >> 0) & 0xFF;  // 提取最低字节
  RedDots_W25Qxx_W_Data[5] = (temp >> 8) & 0xFF;  // 提取次低字节
  RedDots_W25Qxx_W_Data[6] = (temp >> 16) & 0xFF; // 提取次高字节
  RedDots_W25Qxx_W_Data[7] = (temp >> 24) & 0xFF; // 提取最高字节

  QSPI_W25Qxx_SectorErase(RedDots_W25Qxx_DataAddr); // 擦除红点数据存储扇区

  RedDots_W25Qxx_status = QSPI_W25Qxx_WriteBuffer(RedDots_W25Qxx_W_Data, RedDots_W25Qxx_DataAddr, RedDots_W25Qxx_DataSize);
  if (RedDots_W25Qxx_status != QSPI_W25Qxx_OK)
  {
    printf("Write RedDots Data Error!\r\n");
    return;
  }
  printf("Save RedDots Data Success!\r\n");
}

void StepMotor_PID_load(void)
{
  StepMotor_W25Qxx_status = QSPI_W25Qxx_ReadBuffer(StepMotor_W25Qxx_R_Data, StepMotor_W25Qxx_DataAddr, StepMotor_W25Qxx_DataSize);
  if (StepMotor_W25Qxx_status != QSPI_W25Qxx_OK)
  {
    printf("Read StepMotor PID Data Error!\r\n");
    return;
  }
  printf("Load StepMotor PID Data Success!\r\n");

  union
  {
    float float_data;
    uint32_t u32_data;
  } converter;

  converter.u32_data = *(uint32_t *)(StepMotor_W25Qxx_R_Data);
  StepMotor1.kp_angle = converter.float_data;
  converter.u32_data = *(uint32_t *)(StepMotor_W25Qxx_R_Data + 4);
  StepMotor1.ki_angle = converter.float_data;
  converter.u32_data = *(uint32_t *)(StepMotor_W25Qxx_R_Data + 8);
  StepMotor1.kd_angle = converter.float_data;
  converter.u32_data = *(uint32_t *)(StepMotor_W25Qxx_R_Data + 12);
  StepMotor2.kp_angle = converter.float_data;
  converter.u32_data = *(uint32_t *)(StepMotor_W25Qxx_R_Data + 16);
  StepMotor2.ki_angle = converter.float_data;
  converter.u32_data = *(uint32_t *)(StepMotor_W25Qxx_R_Data + 20);
  StepMotor2.kd_angle = converter.float_data;

  // img_thread = *(uint8_t *)(StepMotor_W25Qxx_R_Data + 24);
}

void StepMotor_PID_save(void)
{
  union
  {
    float float_data;
    uint32_t u32_data;
  } converter;

  // 将 float 转换为 uint32_t
  converter.float_data = StepMotor1.kp_angle;
  StepMotor_W25Qxx_W_Data[0] = (converter.u32_data >> 0) & 0xFF;  // 提取最低字节
  StepMotor_W25Qxx_W_Data[1] = (converter.u32_data >> 8) & 0xFF;  // 提取次低字节
  StepMotor_W25Qxx_W_Data[2] = (converter.u32_data >> 16) & 0xFF; // 提取次高字节
  StepMotor_W25Qxx_W_Data[3] = (converter.u32_data >> 24) & 0xFF; // 提取最高字节

  converter.float_data = StepMotor1.ki_angle;
  StepMotor_W25Qxx_W_Data[4] = (converter.u32_data >> 0) & 0xFF;  // 提取最低字节
  StepMotor_W25Qxx_W_Data[5] = (converter.u32_data >> 8) & 0xFF;  // 提取次低字节
  StepMotor_W25Qxx_W_Data[6] = (converter.u32_data >> 16) & 0xFF; // 提取次高字节
  StepMotor_W25Qxx_W_Data[7] = (converter.u32_data >> 24) & 0xFF; // 提取最高字节

  converter.float_data = StepMotor1.kd_angle;
  StepMotor_W25Qxx_W_Data[8] = (converter.u32_data >> 0) & 0xFF;   // 提取最低字节
  StepMotor_W25Qxx_W_Data[9] = (converter.u32_data >> 8) & 0xFF;   // 提取次低字节
  StepMotor_W25Qxx_W_Data[10] = (converter.u32_data >> 16) & 0xFF; // 提取次高字节
  StepMotor_W25Qxx_W_Data[11] = (converter.u32_data >> 24) & 0xFF; // 提取最高字节

  converter.float_data = StepMotor2.kp_angle;
  StepMotor_W25Qxx_W_Data[12] = (converter.u32_data >> 0) & 0xFF;  // 提取最低字节
  StepMotor_W25Qxx_W_Data[13] = (converter.u32_data >> 8) & 0xFF;  // 提取次低字节
  StepMotor_W25Qxx_W_Data[14] = (converter.u32_data >> 16) & 0xFF; // 提取次高字节
  StepMotor_W25Qxx_W_Data[15] = (converter.u32_data >> 24) & 0xFF; // 提取最高字节

  converter.float_data = StepMotor2.ki_angle;
  StepMotor_W25Qxx_W_Data[16] = (converter.u32_data >> 0) & 0xFF;  // 提取最低字节
  StepMotor_W25Qxx_W_Data[17] = (converter.u32_data >> 8) & 0xFF;  // 提取次低字节
  StepMotor_W25Qxx_W_Data[18] = (converter.u32_data >> 16) & 0xFF; // 提取次高字节
  StepMotor_W25Qxx_W_Data[19] = (converter.u32_data >> 24) & 0xFF; // 提取最高字节

  converter.float_data = StepMotor2.kd_angle;
  StepMotor_W25Qxx_W_Data[20] = (converter.u32_data >> 0) & 0xFF;  // 提取最低字节
  StepMotor_W25Qxx_W_Data[21] = (converter.u32_data >> 8) & 0xFF;  // 提取次低字节
  StepMotor_W25Qxx_W_Data[22] = (converter.u32_data >> 16) & 0xFF; // 提取次高字节
  StepMotor_W25Qxx_W_Data[23] = (converter.u32_data >> 24) & 0xFF; // 提取最高字节

  // StepMotor_W25Qxx_W_Data[24] = img_thread;

  QSPI_W25Qxx_SectorErase(StepMotor_W25Qxx_DataAddr); // 擦除步进电机参数数据扇区

  StepMotor_W25Qxx_status = QSPI_W25Qxx_WriteBuffer(StepMotor_W25Qxx_W_Data, StepMotor_W25Qxx_DataAddr, StepMotor_W25Qxx_DataSize);
  if (StepMotor_W25Qxx_status == QSPI_W25Qxx_OK)
  {
    printf("StepMotor Data save success\r\n");
  }
  else
  {
    printf("StepMotor Data save failed\r\n");
  }
}

void DatasSave(void)
{
  StepMotor_PID_save();
  // RedDots_W25Qxx_Data_save();
}

void StepMotor_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  StepMotor1_EN_CLK_ENABLE;
  GPIO_InitStruct.Pin = StepMotor1_EN_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(StepMotor1_EN_PORT, &GPIO_InitStruct);

  StepMotor1_STP_CLK_ENABLE;
  GPIO_InitStruct.Pin = StepMotor1_STP_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(StepMotor1_STP_PORT, &GPIO_InitStruct);

  StepMotor1_DIR_CLK_ENABLE;
  GPIO_InitStruct.Pin = StepMotor1_DIR_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(StepMotor1_DIR_PORT, &GPIO_InitStruct);

  StepMotor2_EN_CLK_ENABLE;
  GPIO_InitStruct.Pin = StepMotor2_EN_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(StepMotor2_EN_PORT, &GPIO_InitStruct);

  StepMotor2_STP_CLK_ENABLE;
  GPIO_InitStruct.Pin = StepMotor2_STP_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(StepMotor2_STP_PORT, &GPIO_InitStruct);

  StepMotor2_DIR_CLK_ENABLE;
  GPIO_InitStruct.Pin = StepMotor2_DIR_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(StepMotor2_DIR_PORT, &GPIO_InitStruct);

  // 初始化步进电机使能，且反向为顺时针
  StepMotor1_ENABLE;
  StepMotor1_DIR_right;
  StepMotor2_ENABLE;
  StepMotor2_DIR_right;

  // 初始化步进电机的参数
  StepMotor1.ID = NO1;
  StepMotor1.current_val = StepMotor_VAL_DISABLE;
  StepMotor1.dir = StepMotor_DIR_RIGHT;
  StepMotor2.ID = NO2;
  StepMotor2.current_val = StepMotor_VAL_DISABLE;
  StepMotor2.dir = StepMotor_DIR_RIGHT;

  // 初始化步进电机的PID计算用参数为0，计数器为0
  StepMotor1.current_error_angle = 0;
  StepMotor1.last_error_angle = 0;
  StepMotor1.count = 0;
  // StepMotor1.angle_max = StepMotor1_Angle_MAX;
  // StepMotor1.angle_min = StepMotor1_Angle_MIN;
  StepMotor2.current_error_angle = 0;
  StepMotor2.last_error_angle = 0;
  StepMotor2.count = 0;
  // StepMotor2.angle_max = StepMotor2_Angle_MAX;
  // StepMotor2.angle_min = StepMotor2_Angle_MIN;

  // 初始化步进电机的角度，当前角度和目标角度相等，等于当前实际角度
  StepMotor_GetAngle(&StepMotor1);
  StepMotor1.target_angle = StepMotor1.current_angle;
  StepMotor_GetAngle(&StepMotor2);
  StepMotor2.target_angle = StepMotor2.current_angle;

  // 从Flash中读取步进电机的PID参数
  StepMotor_PID_load();

  // 从Flash中读取各个点的坐标参数
  RedDots_W25Qxx_Data_load();
}
