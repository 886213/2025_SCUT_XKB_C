#ifndef __Step_motor_controller_H__
#define __Step_motor_controller_H__
#include "sys.h"
#include "usart.h"
#include "AS5600.h"
#include "qspi_w25q64.h"
#include "stdint.h"
#include "Ano_Protocol.h"

#define StepMotor_speedMAX 995 // 最大速度，对应100Hz，0.1125度/步，即500*0.1125=56.25度/s
#define StepMotor_speedMIN 985 // 最小速度，对应50Hz，0.1125度/步，即100*0.1125=11.25度/s
#define STEPMOTOR_ANGLE_PER_STEP 0.1125f // 步进电机单步步进角度，单位为度
#define StepMotor_PIDperiod 10 // PID更新周期,单位ms
#define StepMotor_Stable_threshold 1.0f // 稳定阈值，单位为单步步进角度
#define StepMotor_Stable_count 10 // 稳定时需达到的计数，单位为两次计算PID的间隔时间
#define StepMotor1_Angle_MAX 60.0f // 步进电机1最大角度
#define StepMotor1_Angle_MIN 10.0f // 步进电机1最小角度
#define StepMotor2_Angle_MAX 100.0f // 步进电机2最大角度
#define StepMotor2_Angle_MIN 20.0f // 步进电机2最小角度

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
  StepMotor_DIR dir;         // 方向
  StepMotor_DIR last_dir;    // 上次运动的方向
  StepMotor_VAL current_val; // 当前使能状态
  StepMotor_ID ID;           // 电机ID

  float target_angle;  // 目标角度
  float current_angle; // 当前角度
  float angle_max;    // 最大角度
  float angle_min;    // 最小角度

  float current_error_angle; // 当前误差
  float last_error_angle;    // 上次误差

  float kp_angle; // 比例系数
  float ki_angle; // 积分系数
  float kd_angle; // 微分系数

  float P; // 比例值
  float I; // 积分值
  float D; // 微分值

  float pid_value; // PID值

  u16 target_speed; // 目标速度

  u16 count; // 步进脉冲计数
} StepMotor;

// 红色激光点相关坐标，与步进电机坐标对应
typedef struct RedDot_coordinate
{
  float x; // x坐标，对应与步进电机1
  float y; // y坐标，对应与步进电机2
} RedDot_coordinate;

extern uint8_t RxTargetFlag; // 接收目标坐标标志位
extern StepMotor StepMotor1, StepMotor2;  // 步进电机1和2的结构体实例
extern StepMotor_controler StepMotor_controler_main; // 步进电机闭环控制器使能状态
extern RedDot_coordinate origin, RxTarget; // 红色激光点坐标，原点坐标和从上位机接受到的目标坐标
extern RedDot_coordinate MovePoint, LastMovePoint; // 移动点坐标和上次移动点坐标

int8_t QSPI_W25Qxx_Test(void);		//Flash读写测试

void StepMotor_controler_Swich(void);
void Set_StepMotor_Angle(void); // 设置步进电机目标角度
void Set_LCD_Point(void); // 设置LCD显示点坐标
void Set_LCD_Point2(uint16_t x, uint16_t y); // 设置LCD显示点坐标
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
