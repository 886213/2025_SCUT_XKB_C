#ifndef __ANO_Protocol_H__
#define __ANO_Protocol_H__

#include "ANO_Protocol_Reg.h"
#include "sys.h"
#include "string.h"
#include "Led.h"
#include "usart.h"

#define ANO_Protocol_ENABLE

#define ANO_UART huart3      // 定义协议使用的串口
#define ANO_UART_TIMEOUT 100 // 定义协议使用的串口超时时间

// 固定 huart 和 Timeout，仅需传入 data 和 Size
#define ANO_TX(pData, Size) \
  HAL_UART_Transmit(&ANO_UART, pData, Size, ANO_UART_TIMEOUT)

#define StepMotorSwich_CMD CMD[0]
#define MoveToZero_CMD CMD[1]
#define ScanScreen_CMD CMD[2]
#define SetingA4Target_CMD CMD[3]
#define FreedomA4Target_CMD CMD[4]
#define SetStepMotorAngle_CMD CMD[5]
#define DatasSave_CMD CMD[6]
#define FlexiDatasShowEnable_CMD CMD[7]
#define BuzzerSwich_CMD CMD[8]
#define TXJPEGIMAGE_CMD CMD[9]
#define SetLCDPoint_CMD CMD[10]
#define UpdateUI_CMD CMD[11]

typedef enum
{
  CH1 = 0xF1,
  CH2 = 0xF2,
  CH3 = 0xF3,
  CH4 = 0xF4,
  CH5 = 0xF5,
  CH6 = 0xF6,
  CH7 = 0xF7,
  CH8 = 0xF8,
  CH9 = 0xF9,
  CH10 = 0xFA
} FlexiDatas_ShowChannel;

#define CMDNUMS 15 // 命令数量
#define PARNUMS 10 // 参数数量

// 回调函数类型定义
typedef void (*CMDCallbackFunc)(void);

typedef enum
{
  CMD_VAL_NONE, // 无符号8位整数
  CMD_VAL_UINT8,
  CMD_VAL_INT8,
  CMD_VAL_UINT16,
  CMD_VAL_INT16,
  CMD_VAL_UINT32,
  CMD_VAL_INT32,
  CMD_VAL_UINT64,
  CMD_VAL_INT64,
  CMD_VAL_FLOAT
} CMD_VAL_type;

typedef enum
{
  CMD_VAL_ID_NO1,
  CMD_VAL_ID_NO2,
  CMD_VAL_ID_NO3,
  CMD_VAL_ID_NO4,
  CMD_VAL_ID_NO5,
  CMD_VAL_ID_NO6,
  CMD_VAL_ID_NO7,
  CMD_VAL_ID_NO8,
} CMD_VAL_ID;

typedef struct
{
  uint16_t CMD_ID;
  uint8_t CID[3];
  CMD_VAL_type types[8]; // 8个参数类型
  union
  {                    // 联合体存储不同类型的值
    uint8_t uint8_val; // 8个参数值
    int8_t int8_val;
    uint16_t uint16_val;
    int16_t int16_val;
    uint32_t uint32_val;
    int32_t int32_val;
    float float_val;
  } values[8];
  char Name[20]; // 20byte
  char Info[50]; // 50byte
  uint8_t Flag;
  CMDCallbackFunc Callback; // 回调函数
} Command;
extern Command CMD[];

typedef void (*PARCallbackFunc)(void);

typedef enum
{
  UINT8,
  INT8,
  UINT16,
  INT16,
  UINT32,
  INT32,
  UINT64,
  INT64,
  FLOAT,
  DOUBLE,
  STRING
} PAR_type;

typedef struct
{
  uint16_t PAR_ID; // 参数ID
  PAR_type type;   // 参数类型
  union
  { // 联合体存储不同类型的值
    uint8_t uint8_val;
    uint16_t uint16_val;
    uint32_t uint32_val;
    int8_t int8_val;
    int16_t int16_val;
    int32_t int32_val;
    float float_val;
  } value;
  char Name[20];      // 20byte
  char Info[50];      // 50byte
  void *external_ptr; // 指向外部变量的指针
} Parameter;
extern Parameter PAR[]; // 参数结构体数组

// 协议初始化函数，并为CMD和PAR赋初值
void ANO_Init(void);

// 下位机接收状态机，接收数据实际处理函数，放在串口接收中断中调用
void ANO_RX_Process(uint8_t rxdata);

// CMDx的回调函数响应函数，放在主函数或定时中断中调用，内部自动检测各CMD的Flag，并调用对应的回调函数
void CMD_Func(void);

// 应用型函数
void CMD_Set(Command *cmd, const char *Name, const char *Info, void (*Callback)(void));
void CMD_RxValue_Set(Command *cmd, CMD_VAL_type VAL_type, CMD_VAL_ID VAL_ID);
void CMD_RxValue_Read(Command *cmd, CMD_VAL_ID VAL_ID, void *value);
void PAR_Set(Parameter *param, PAR_type type, void *external_ptr, const char *name, const char *info);

/*显示波形函数*/
// 单参数
void ANO_Protocol_Tx_uint8(uint8_t data, FlexiDatas_ShowChannel channel);
void ANO_Protocol_Tx_uint16(uint16_t data, FlexiDatas_ShowChannel channel);
void ANO_Protocol_Tx_uint32(uint32_t data, FlexiDatas_ShowChannel channel);
void ANO_Protocol_Tx_int8(int8_t data, FlexiDatas_ShowChannel channel);
void ANO_Protocol_Tx_int16(int16_t data, FlexiDatas_ShowChannel channel);
void ANO_Protocol_Tx_int32(int32_t data, FlexiDatas_ShowChannel channel);
void ANO_Protocol_Tx_float(float data, FlexiDatas_ShowChannel channel);
// 三参数函数
void ANO_Protocol_Tx_int8int8int8(int8_t data1, int8_t data2, int8_t data3, FlexiDatas_ShowChannel channel);
void ANO_Protocol_Tx_uint8uint8uint8(uint8_t data1, uint8_t data2, uint8_t data3, FlexiDatas_ShowChannel channel);
void ANO_Protocol_Tx_int16int16int16(int16_t data1, int16_t data2, int16_t data3, FlexiDatas_ShowChannel channel);
void ANO_Protocol_Tx_uint16uint16uint16(uint16_t data1, uint16_t data2, uint16_t data3, FlexiDatas_ShowChannel channel);
void ANO_Protocol_Tx_floatfloatfloat(float data1, float data2, float data3, FlexiDatas_ShowChannel channel);
// 特殊多参数函数，用户可参考以下函数进行扩展
void ANO_Protocol_Tx_floatfloat(float data1, float data2, FlexiDatas_ShowChannel channel);
void ANO_Protocol_Tx_floatfloatfloatfloat(float data1, float data2, float data3, float data4, FlexiDatas_ShowChannel channel);
void ANO_Protocol_Tx_uint8uint8floatfloatfloat(uint8_t data1, uint8_t data2, float data3, float data4, float data5, FlexiDatas_ShowChannel channel);

#endif