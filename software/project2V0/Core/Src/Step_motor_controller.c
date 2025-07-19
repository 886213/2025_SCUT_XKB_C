#include "Step_motor_controller.h"

StepMotor StepMotor1, StepMotor2;

StepMotor_controler StepMotor_controler_main = StepMotor_controler_DISABLE;

// ÿ4KBΪһ�����ݿ飬����������ݴ洢��W25Qxx��0x000000~0x000FFF��ַ��������������ݴ洢��W25Qxx��0x001000~0x001FFF��ַ��

/*����������������ʽ���������1��kp��ki��kd���������2��kp��ki��kdȫ��float���ݣ���24�ֽ�*/
#define StepMotor_W25Qxx_DataSize 4 * 1024                  // 4KB����������Ʋ������ݴ洢��С
uint8_t StepMotor_W25Qxx_W_Data[StepMotor_W25Qxx_DataSize]; // 4KB����������Ʋ���д��������
uint8_t StepMotor_W25Qxx_R_Data[StepMotor_W25Qxx_DataSize]; // 4KB����������Ʋ�������������
uint32_t StepMotor_W25Qxx_DataAddr = 0x000000;              // ����������ݴ洢��ַ
int32_t StepMotor_W25Qxx_status;                            // ����������ݴ洢״̬

/*����������������ʽ��ÿһ������x��y��float���ͣ���8�ֽڣ����洢4KB/8=512�������������*/
#define RedDots_W25Qxx_DataSize 4 * 1024                // 4KB����������ݴ洢��С
uint8_t RedDots_W25Qxx_W_Data[RedDots_W25Qxx_DataSize]; // 4KB����������������
uint8_t RedDots_W25Qxx_R_Data[RedDots_W25Qxx_DataSize]; // 4KB����������������
uint32_t RedDots_W25Qxx_DataAddr = 0x001000;            // ����������ݴ洢��ַ
int32_t RedDots_W25Qxx_status;                          // ����������ݴ洢״̬

// ��LCD���ص㵽������������ת��ϵ��������Ϊ[1,x,y,x^2,xy,y^2]
float StepMotor1_coef[6]={3.631357, 0.105766, 0.005880, -0.000004, -0.000011, -0.000007};
float StepMotor2_coef[6]={54.302408, 0.004356, 0.126166, -0.000016, 0.000003, -0.000047};

uint8_t RxTargetFlag = 0;
RedDot_coordinate origin, RxTarget;
RedDot_coordinate MovePoint, LastMovePoint; // �ƶ���������ϴ��ƶ�������

#define W25Qxx_NumByteToTest 32 * 1024 // �������ݵĳ��ȣ�64K

int32_t QSPI_Status; // ����־λ

uint32_t W25Qxx_TestAddr = 0;                     // ���Ե�ַ
uint8_t W25Qxx_WriteBuffer[W25Qxx_NumByteToTest]; //	д��������
uint8_t W25Qxx_ReadBuffer[W25Qxx_NumByteToTest];  //	����������
int8_t QSPI_W25Qxx_Test(void)                     // Flash��д����
{
  uint32_t i = 0;               // ��������
  uint32_t ExecutionTime_Begin; // ��ʼʱ��
  uint32_t ExecutionTime_End;   // ����ʱ��
  uint32_t ExecutionTime;       // ִ��ʱ��
  float ExecutionSpeed;         // ִ���ٶ�

  // ���� >>>>>>>

  ExecutionTime_Begin = HAL_GetTick();                       // ��ȡ systick ��ǰʱ�䣬��λms
  QSPI_Status = QSPI_W25Qxx_BlockErase_32K(W25Qxx_TestAddr); // ����32K�ֽ�
  ExecutionTime_End = HAL_GetTick();                         // ��ȡ systick ��ǰʱ�䣬��λms

  ExecutionTime = ExecutionTime_End - ExecutionTime_Begin; // �������ʱ�䣬��λms

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

  // д�� >>>>>>>

  for (i = 0; i < W25Qxx_NumByteToTest; i++) // �Ƚ�����д������
  {
    W25Qxx_WriteBuffer[i] = i;
  }
  ExecutionTime_Begin = HAL_GetTick();                                                              // ��ȡ systick ��ǰʱ�䣬��λms
  QSPI_Status = QSPI_W25Qxx_WriteBuffer(W25Qxx_WriteBuffer, W25Qxx_TestAddr, W25Qxx_NumByteToTest); // д������
  ExecutionTime_End = HAL_GetTick();                                                                // ��ȡ systick ��ǰʱ�䣬��λms

  ExecutionTime = ExecutionTime_End - ExecutionTime_Begin;      // �������ʱ�䣬��λms
  ExecutionSpeed = (float)W25Qxx_NumByteToTest / ExecutionTime; // ����д���ٶȣ���λ KB/S
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

  // ��ȡ >>>>>>>

  ExecutionTime_Begin = HAL_GetTick();                                                            // ��ȡ systick ��ǰʱ�䣬��λms
  QSPI_Status = QSPI_W25Qxx_ReadBuffer(W25Qxx_ReadBuffer, W25Qxx_TestAddr, W25Qxx_NumByteToTest); // ��ȡ����
  ExecutionTime_End = HAL_GetTick();                                                              // ��ȡ systick ��ǰʱ�䣬��λms

  ExecutionTime = ExecutionTime_End - ExecutionTime_Begin;             // �������ʱ�䣬��λms
  ExecutionSpeed = (float)W25Qxx_NumByteToTest / ExecutionTime / 1024; // �����ȡ�ٶȣ���λ MB/S

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

  // ����У�� >>>>>>>

  for (i = 0; i < W25Qxx_NumByteToTest; i++) // ��֤�����������Ƿ����д�������
  {
    if (W25Qxx_WriteBuffer[i] != W25Qxx_ReadBuffer[i]) // ������ݲ���ȣ��򷵻�0
    {
      printf("\r\n-4\r\n");
      while (1)
        ;
    }
  }
  printf("\r\n4\r\n");

  return QSPI_W25Qxx_OK; // ����ͨ��
}

/**
 * @brief  ����������������غ������л����������������ʹ��״̬
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
  CMD_RxValue_Read(&SetStepMotorAngle_CMD, CMD_VAL_ID_NO1, &Motor1Angle); // �������1�ĽǶ�
  CMD_RxValue_Read(&SetStepMotorAngle_CMD, CMD_VAL_ID_NO2, &Motor2Angle); // �������2�ĽǶ�
  // ���ò��������Ŀ��Ƕȣ�ȷ���Ƕ��ں���Χ��
  int16_t Motor1Angle_int = (int16_t)(Motor1Angle / STEPMOTOR_ANGLE_PER_STEP); // �������1�ĽǶ�
  int16_t Motor2Angle_int = (int16_t)(Motor2Angle / STEPMOTOR_ANGLE_PER_STEP); // �������2�ĽǶ�
  float Motor1Angle_float = (float)Motor1Angle_int * STEPMOTOR_ANGLE_PER_STEP; // �������1�ĽǶ�
  float Motor2Angle_float = (float)Motor2Angle_int * STEPMOTOR_ANGLE_PER_STEP; // �������2�ĽǶ�
  // ���ƽǶȷ�Χ��0-360��֮��
  if (Motor1Angle_float > 0 && Motor2Angle_float > 0 && Motor1Angle_float < 360 && Motor2Angle_float < 360)
  {
    RxTarget.x = Motor1Angle_float; // �������1��Ŀ��Ƕ�
    RxTarget.y = Motor2Angle_float; // �������2��Ŀ��Ƕ�
    RxTargetFlag = 1;               // ���ý��ձ�־λ
  }
}

void Set_LCD_Point(void)
{
  uint16_t x, y;
  CMD_RxValue_Read(&SetLCDPoint_CMD, CMD_VAL_ID_NO1, &x); // LCD��X����
  CMD_RxValue_Read(&SetLCDPoint_CMD, CMD_VAL_ID_NO2, &y); // LCD��Y����
  float X = (float)x; // LCD��X����
  float Y = (float)y; // LCD��Y����
  // ����LCD������ӳ�䵽��������ĽǶ�
  float Motor1Angle_float = StepMotor1_coef[0] + StepMotor1_coef[1] * X + StepMotor1_coef[2] * Y + 
                            StepMotor1_coef[3] * X * X + StepMotor1_coef[4] * X * Y + StepMotor1_coef[5] * Y * Y;
  float Motor2Angle_float = StepMotor2_coef[0] + StepMotor2_coef[1] * X + StepMotor2_coef[2] * Y +
                            StepMotor2_coef[3] * X * X + StepMotor2_coef[4] * X * Y + StepMotor2_coef[5] * Y * Y;
  // ���ƽǶȷ�Χ��0-360��֮��
  if (Motor1Angle_float > 0 && Motor2Angle_float > 0 && Motor1Angle_float < 360 && Motor2Angle_float < 360)
  {
    RxTarget.x = Motor1Angle_float; // �������1��Ŀ��Ƕ�
    RxTarget.y = Motor2Angle_float; // �������2��Ŀ��Ƕ�
    RxTargetFlag = 1;               // ���ý��ձ�־λ
  }
}

void Set_LCD_Point2(uint16_t x, uint16_t y)
{
  float X = (float)x; // LCD��X����
  float Y = (float)y; // LCD��Y����
  // ����LCD������ӳ�䵽��������ĽǶ�
  float Motor1Angle_float = StepMotor1_coef[0] + StepMotor1_coef[1] * X + StepMotor1_coef[2] * Y +
                            StepMotor1_coef[3] * X * X + StepMotor1_coef[4] * X * Y + StepMotor1_coef[5] * Y * Y;
  float Motor2Angle_float = StepMotor2_coef[0] + StepMotor2_coef[1] * X + StepMotor2_coef[2] * Y +
                            StepMotor2_coef[3] * X * X + StepMotor2_coef[4] * X * Y + StepMotor2_coef[5] * Y * Y;
  uint16_t Motor1Angle_int = (uint16_t)(Motor1Angle_float / STEPMOTOR_ANGLE_PER_STEP); // �������1�ĽǶ�
  uint16_t Motor2Angle_int = (uint16_t)(Motor2Angle_float / STEPMOTOR_ANGLE_PER_STEP); // �������2�ĽǶ�
  Motor1Angle_float = (float)Motor1Angle_int * STEPMOTOR_ANGLE_PER_STEP; // �������1�ĽǶ�
  Motor2Angle_float = (float)Motor2Angle_int * STEPMOTOR_ANGLE_PER_STEP; // �������2�ĽǶ�                          
  // ���ƽǶȷ�Χ��0-360��֮��
  if (Motor1Angle_float > 0 && Motor2Angle_float > 0 && Motor1Angle_float < 360 && Motor2Angle_float < 360)
  {
    RxTarget.x = Motor1Angle_float; // �������1��Ŀ��Ƕ�
    RxTarget.y = Motor2Angle_float; // �������2��Ŀ��Ƕ�
    RxTargetFlag = 2;               // ���ý��ձ�־λ
  }
}

/**
 * @brief  ��ά���������̨���ƺ������Ŷ�ʱ�ж��У����ݲ�������ĵ�ǰ״̬��Ŀ���ٶȣ�������Ϊ1��λ��ʱ�ж����ڵĶ����壬���Ʋ��������ת�����ٶ�Խ�ߣ�����Ƶ��Խ�ߣ����Ƶ��Ϊ��ʱ�ж�Ƶ��/2
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

  motor->last_dir = motor->dir; // ��¼�ϴεķ���

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

  motor->target_speed = (uint16_t)(motor->pid_value / 1) > (StepMotor_speedMAX - StepMotor_speedMIN) ? StepMotor_speedMAX : ((uint16_t)(motor->pid_value / 1) + StepMotor_speedMIN); // ���������Ŀ���ٶ�
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

  motor->last_dir = motor->dir; // ��¼�ϴεķ���

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

  motor->target_speed = (int16_t)(motor->pid_value / STEPMOTOR_ANGLE_PER_STEP) > 100 ? StepMotor_speedMAX : ((int16_t)(motor->pid_value / STEPMOTOR_ANGLE_PER_STEP) + StepMotor_speedMIN); // ���������Ŀ���ٶ�
}

/**
 * @brief  ��������ȶ�����жϺ����������������ǰ�ǶȻ����ȶ���Ŀ��Ƕ���ά��һ��ʱ��ʱ��ֻ����StepMotor_controler_mainΪ0������������Ȩ��
 * @retval None
 **/
uint8_t StepMotor_Stable_Judge(void)
{
  static uint16_t count1 = 0; // ��������ȶ��ж���ֵ
  static uint16_t count2 = 0; // ��������ȶ��ж���ֵ
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
    // ��������ȶ�, ����ֹͣ
    StepMotor_controler_main = StepMotor_controler_DISABLE;
    StepMotor1.target_angle = StepMotor1.current_angle;
    StepMotor2.target_angle = StepMotor2.current_angle;
    StepMotor1.target_speed = 0;
    StepMotor2.target_speed = 0;
    count1 = 0;
    count2 = 0;
    return 1; // ��������ȶ�
  }
  return 0; // ����������ȶ�
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

  // �������е�����ת��Ϊ������
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

  // ��ȡÿ���ֽڴ洢��������
  RedDots_W25Qxx_W_Data[0] = (temp >> 0) & 0xFF;  // ��ȡ����ֽ�
  RedDots_W25Qxx_W_Data[1] = (temp >> 8) & 0xFF;  // ��ȡ�ε��ֽ�
  RedDots_W25Qxx_W_Data[2] = (temp >> 16) & 0xFF; // ��ȡ�θ��ֽ�
  RedDots_W25Qxx_W_Data[3] = (temp >> 24) & 0xFF; // ��ȡ����ֽ�

  converter.float_data = origin.y;
  temp = converter.u32_data;

  // ��ȡÿ���ֽڴ洢��������
  RedDots_W25Qxx_W_Data[4] = (temp >> 0) & 0xFF;  // ��ȡ����ֽ�
  RedDots_W25Qxx_W_Data[5] = (temp >> 8) & 0xFF;  // ��ȡ�ε��ֽ�
  RedDots_W25Qxx_W_Data[6] = (temp >> 16) & 0xFF; // ��ȡ�θ��ֽ�
  RedDots_W25Qxx_W_Data[7] = (temp >> 24) & 0xFF; // ��ȡ����ֽ�

  QSPI_W25Qxx_SectorErase(RedDots_W25Qxx_DataAddr); // ����������ݴ洢����

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

  // �� float ת��Ϊ uint32_t
  converter.float_data = StepMotor1.kp_angle;
  StepMotor_W25Qxx_W_Data[0] = (converter.u32_data >> 0) & 0xFF;  // ��ȡ����ֽ�
  StepMotor_W25Qxx_W_Data[1] = (converter.u32_data >> 8) & 0xFF;  // ��ȡ�ε��ֽ�
  StepMotor_W25Qxx_W_Data[2] = (converter.u32_data >> 16) & 0xFF; // ��ȡ�θ��ֽ�
  StepMotor_W25Qxx_W_Data[3] = (converter.u32_data >> 24) & 0xFF; // ��ȡ����ֽ�

  converter.float_data = StepMotor1.ki_angle;
  StepMotor_W25Qxx_W_Data[4] = (converter.u32_data >> 0) & 0xFF;  // ��ȡ����ֽ�
  StepMotor_W25Qxx_W_Data[5] = (converter.u32_data >> 8) & 0xFF;  // ��ȡ�ε��ֽ�
  StepMotor_W25Qxx_W_Data[6] = (converter.u32_data >> 16) & 0xFF; // ��ȡ�θ��ֽ�
  StepMotor_W25Qxx_W_Data[7] = (converter.u32_data >> 24) & 0xFF; // ��ȡ����ֽ�

  converter.float_data = StepMotor1.kd_angle;
  StepMotor_W25Qxx_W_Data[8] = (converter.u32_data >> 0) & 0xFF;   // ��ȡ����ֽ�
  StepMotor_W25Qxx_W_Data[9] = (converter.u32_data >> 8) & 0xFF;   // ��ȡ�ε��ֽ�
  StepMotor_W25Qxx_W_Data[10] = (converter.u32_data >> 16) & 0xFF; // ��ȡ�θ��ֽ�
  StepMotor_W25Qxx_W_Data[11] = (converter.u32_data >> 24) & 0xFF; // ��ȡ����ֽ�

  converter.float_data = StepMotor2.kp_angle;
  StepMotor_W25Qxx_W_Data[12] = (converter.u32_data >> 0) & 0xFF;  // ��ȡ����ֽ�
  StepMotor_W25Qxx_W_Data[13] = (converter.u32_data >> 8) & 0xFF;  // ��ȡ�ε��ֽ�
  StepMotor_W25Qxx_W_Data[14] = (converter.u32_data >> 16) & 0xFF; // ��ȡ�θ��ֽ�
  StepMotor_W25Qxx_W_Data[15] = (converter.u32_data >> 24) & 0xFF; // ��ȡ����ֽ�

  converter.float_data = StepMotor2.ki_angle;
  StepMotor_W25Qxx_W_Data[16] = (converter.u32_data >> 0) & 0xFF;  // ��ȡ����ֽ�
  StepMotor_W25Qxx_W_Data[17] = (converter.u32_data >> 8) & 0xFF;  // ��ȡ�ε��ֽ�
  StepMotor_W25Qxx_W_Data[18] = (converter.u32_data >> 16) & 0xFF; // ��ȡ�θ��ֽ�
  StepMotor_W25Qxx_W_Data[19] = (converter.u32_data >> 24) & 0xFF; // ��ȡ����ֽ�

  converter.float_data = StepMotor2.kd_angle;
  StepMotor_W25Qxx_W_Data[20] = (converter.u32_data >> 0) & 0xFF;  // ��ȡ����ֽ�
  StepMotor_W25Qxx_W_Data[21] = (converter.u32_data >> 8) & 0xFF;  // ��ȡ�ε��ֽ�
  StepMotor_W25Qxx_W_Data[22] = (converter.u32_data >> 16) & 0xFF; // ��ȡ�θ��ֽ�
  StepMotor_W25Qxx_W_Data[23] = (converter.u32_data >> 24) & 0xFF; // ��ȡ����ֽ�

  // StepMotor_W25Qxx_W_Data[24] = img_thread;

  QSPI_W25Qxx_SectorErase(StepMotor_W25Qxx_DataAddr); // �����������������������

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

  // ��ʼ���������ʹ�ܣ��ҷ���Ϊ˳ʱ��
  StepMotor1_ENABLE;
  StepMotor1_DIR_right;
  StepMotor2_ENABLE;
  StepMotor2_DIR_right;

  // ��ʼ����������Ĳ���
  StepMotor1.ID = NO1;
  StepMotor1.current_val = StepMotor_VAL_DISABLE;
  StepMotor1.dir = StepMotor_DIR_RIGHT;
  StepMotor2.ID = NO2;
  StepMotor2.current_val = StepMotor_VAL_DISABLE;
  StepMotor2.dir = StepMotor_DIR_RIGHT;

  // ��ʼ�����������PID�����ò���Ϊ0��������Ϊ0
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

  // ��ʼ����������ĽǶȣ���ǰ�ǶȺ�Ŀ��Ƕ���ȣ����ڵ�ǰʵ�ʽǶ�
  StepMotor_GetAngle(&StepMotor1);
  StepMotor1.target_angle = StepMotor1.current_angle;
  StepMotor_GetAngle(&StepMotor2);
  StepMotor2.target_angle = StepMotor2.current_angle;

  // ��Flash�ж�ȡ���������PID����
  StepMotor_PID_load();

  // ��Flash�ж�ȡ��������������
  RedDots_W25Qxx_Data_load();
}
