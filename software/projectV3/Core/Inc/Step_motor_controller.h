#ifndef __Step_motor_controller_H__
#define __Step_motor_controller_H__
#include "sys.h"
#include "usart.h"
#include "AS5600.h"
#include "qspi_w25q64.h"
#include "stdint.h"
#include "Ano_Protocol.h"

#define StepMotor_speedMAX 995 // ����ٶȣ���Ӧ100Hz��0.1125��/������500*0.1125=56.25��/s
#define StepMotor_speedMIN 985 // ��С�ٶȣ���Ӧ50Hz��0.1125��/������100*0.1125=11.25��/s
#define STEPMOTOR_ANGLE_PER_STEP 0.1125f // ����������������Ƕȣ���λΪ��
#define StepMotor_PIDperiod 10 // PID��������,��λms
#define StepMotor_Stable_threshold 1.0f // �ȶ���ֵ����λΪ���������Ƕ�
#define StepMotor_Stable_count 10 // �ȶ�ʱ��ﵽ�ļ�������λΪ���μ���PID�ļ��ʱ��
#define StepMotor1_Angle_MAX 60.0f // �������1���Ƕ�
#define StepMotor1_Angle_MIN 10.0f // �������1��С�Ƕ�
#define StepMotor2_Angle_MAX 100.0f // �������2���Ƕ�
#define StepMotor2_Angle_MIN 20.0f // �������2��С�Ƕ�

#define StepMotor1_EN_PIN GPIO_PIN_2
#define StepMotor1_EN_PORT GPIOD
#define StepMotor1_EN_CLK_ENABLE __HAL_RCC_GPIOD_CLK_ENABLE()

#define StepMotor1_STP_PIN GPIO_PIN_8
#define StepMotor1_STP_PORT GPIOC
#define StepMotor1_STP_CLK_ENABLE __HAL_RCC_GPIOC_CLK_ENABLE()

#define StepMotor1_DIR_PIN GPIO_PIN_3
#define StepMotor1_DIR_PORT GPIOE
#define StepMotor1_DIR_CLK_ENABLE __HAL_RCC_GPIOE_CLK_ENABLE()

#define StepMotor2_EN_PIN GPIO_PIN_3
#define StepMotor2_EN_PORT GPIOB
#define StepMotor2_EN_CLK_ENABLE __HAL_RCC_GPIOB_CLK_ENABLE()

#define StepMotor2_STP_PIN GPIO_PIN_6
#define StepMotor2_STP_PORT GPIOD
#define StepMotor2_STP_CLK_ENABLE __HAL_RCC_GPIOD_CLK_ENABLE()

#define StepMotor2_DIR_PIN GPIO_PIN_7
#define StepMotor2_DIR_PORT GPIOD
#define StepMotor2_DIR_CLK_ENABLE __HAL_RCC_GPIOD_CLK_ENABLE()

#define StepMotor1_ENABLE HAL_GPIO_WritePin(StepMotor1_EN_PORT, StepMotor1_EN_PIN, GPIO_PIN_RESET)
#define StepMotor1_DISABLE HAL_GPIO_WritePin(StepMotor1_EN_PORT, StepMotor1_EN_PIN, GPIO_PIN_SET)
#define StepMotor2_ENABLE HAL_GPIO_WritePin(StepMotor2_EN_PORT, StepMotor2_EN_PIN, GPIO_PIN_RESET)
#define StepMotor2_DISABLE HAL_GPIO_WritePin(StepMotor2_EN_PORT, StepMotor2_EN_PIN, GPIO_PIN_SET)

#define StepMotor1_DIR_right HAL_GPIO_WritePin(StepMotor1_DIR_PORT, StepMotor1_DIR_PIN, GPIO_PIN_SET)
#define StepMotor1_DIR_left HAL_GPIO_WritePin(StepMotor1_DIR_PORT, StepMotor1_DIR_PIN, GPIO_PIN_RESET)
#define StepMotor2_DIR_right HAL_GPIO_WritePin(StepMotor2_DIR_PORT, StepMotor2_DIR_PIN, GPIO_PIN_SET)
#define StepMotor2_DIR_left HAL_GPIO_WritePin(StepMotor2_DIR_PORT, StepMotor2_DIR_PIN, GPIO_PIN_RESET)

#define StepMotor1_STEP HAL_GPIO_WritePin(StepMotor1_STP_PORT, StepMotor1_STP_PIN, GPIO_PIN_SET)
#define StepMotor1_STOP HAL_GPIO_WritePin(StepMotor1_STP_PORT, StepMotor1_STP_PIN, GPIO_PIN_RESET)
#define StepMotor2_STEP HAL_GPIO_WritePin(StepMotor2_STP_PORT, StepMotor2_STP_PIN, GPIO_PIN_SET)
#define StepMotor2_STOP HAL_GPIO_WritePin(StepMotor2_STP_PORT, StepMotor2_STP_PIN, GPIO_PIN_RESET)

typedef enum
{
  StepMotor_controler_DISABLE,
  StepMotor_controler_ENABLE
} StepMotor_controler;

typedef enum
{
  StepMotor_DIR_RIGHT,
  StepMotor_DIR_LEFT
} StepMotor_DIR;

typedef enum
{
  StepMotor_VAL_ENABLE,
  StepMotor_VAL_DISABLE
} StepMotor_VAL;

typedef enum
{
  NO1,
  NO2
} StepMotor_ID;

typedef struct StepMotor
{
  StepMotor_DIR dir;         // ����
  StepMotor_DIR last_dir;    // �ϴ��˶��ķ���
  StepMotor_VAL current_val; // ��ǰʹ��״̬
  StepMotor_ID ID;           // ���ID

  float target_angle;  // Ŀ��Ƕ�
  float current_angle; // ��ǰ�Ƕ�
  float angle_max;    // ���Ƕ�
  float angle_min;    // ��С�Ƕ�

  float current_error_angle; // ��ǰ���
  float last_error_angle;    // �ϴ����

  float kp_angle; // ����ϵ��
  float ki_angle; // ����ϵ��
  float kd_angle; // ΢��ϵ��

  float P; // ����ֵ
  float I; // ����ֵ
  float D; // ΢��ֵ

  float pid_value; // PIDֵ

  u16 target_speed; // Ŀ���ٶ�

  u16 count; // �����������
} StepMotor;

// ��ɫ�����������꣬�벽����������Ӧ
typedef struct RedDot_coordinate
{
  float x; // x���꣬��Ӧ�벽�����1
  float y; // y���꣬��Ӧ�벽�����2
} RedDot_coordinate;

extern uint8_t RxTargetFlag; // ����Ŀ�������־λ
extern StepMotor StepMotor1, StepMotor2;  // �������1��2�Ľṹ��ʵ��
extern StepMotor_controler StepMotor_controler_main; // ��������ջ�������ʹ��״̬
extern RedDot_coordinate origin, RxTarget; // ��ɫ��������꣬ԭ������ʹ���λ�����ܵ���Ŀ������
extern RedDot_coordinate MovePoint, LastMovePoint; // �ƶ���������ϴ��ƶ�������

int8_t QSPI_W25Qxx_Test(void);		//Flash��д����

void StepMotor_controler_Swich(void);
void Set_StepMotor_Angle(void); // ���ò������Ŀ��Ƕ�
void Set_LCD_Point(void); // ����LCD��ʾ������
void Set_LCD_Point2(uint16_t x, uint16_t y); // ����LCD��ʾ������
void StepMotor1_control(void);
void StepMotor2_control(void);
void StepMotor_PID(StepMotor *motor, float target_angle, float current_angle);
void StepMotor_PID2(StepMotor *motor, float target_angle, float current_angle);
uint8_t StepMotor_Stable_Judge(void);
void StepMotor_Init(void);
void StepMotor_PID_load(void);
void StepMotor_PID_save(void);
void RedDots_W25Qxx_Data_load(void);
void RedDots_W25Qxx_Data_save(void);
void DatasSave(void);
void StepMotor_GetAngle(StepMotor *motor);

#endif
