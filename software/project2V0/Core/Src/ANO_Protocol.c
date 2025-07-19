#include "ANO_Protocol.h"

char ANO_Protocol_Temp[100];
char ANO_Protocol_RxPacket[30];

uint8_t ANO_Protocol_RxFlag;
uint8_t ANO_Protocol_CMD;
uint16_t ANO_Protocol_VAR;
uint8_t ANO_Protocol_RxID;
uint8_t ANO_Protocol_RxSC;
uint8_t ANO_Protocol_RxAC;
uint8_t ANO_Protocol_StateSelect = 1; // 无线串口状态选择，1为命令控制，2为参数调节

uint8_t CMDNums = CMDNUMS; // 命令数量
Command CMD[CMDNUMS];      // 命令结构体数组
// Command CMD1, CMD2, CMD3, CMD4, CMD5;
uint8_t CID_V1;

uint8_t PARNums = PARNUMS; // 参数数量
Parameter PAR[PARNUMS];
uint16_t ANO_Protocol_RxPAR_ID;
uint8_t ANO_Protocol_RxPAR_Datasize = 0;

union
{ // 联合体存储接收到的数据
  uint8_t uint8_val;
  uint16_t uint16_val;
  uint32_t uint32_val;
  int8_t int8_val;
  int16_t int16_val;
  int32_t int32_val;
  float float_val;
} ANO_Protocol_RxPAR_Value;

void CMD_Func_Test(void)
{
  LED1_Toggle
}

/**
 * @brief  CMD基本信息设置函数，用户可以根据需要修改参数名称，信息和回调函数
 * @param  cmd: 命令结构体指针
 * @param  Name: 命令的名称
 * @param  Info: 命令的信息
 * @param  Callback: 上位机触发命令时的回调函数
 * @retval None
 * @example CMD_Set(&CMD1, "CMD1", "CMD1 Info", CMD_Func_Test); // 设置CMD1的名称为"CMD1"，信息为"CMD1 Info"，回调函数为CMD_Func_Test
 */
void CMD_Set(Command *cmd, const char *Name, const char *Info, void (*Callback)(void))
{
  memset(cmd->Name, 0, sizeof(cmd->Name));         // 清空数组
  memset(cmd->Info, 0, sizeof(cmd->Info));         // 清空数组
  strncpy(cmd->Name, Name, sizeof(cmd->Name) - 1); // 拷贝字符串
  strncpy(cmd->Info, Info, sizeof(cmd->Info) - 1); // 拷贝字符串
  cmd->Callback = Callback;                        // 设置回调函数
}

/**
 * @brief  CMD的接收值设置函数，用户可以根据需要设置接收值的类型和对应的ID
 * @param  cmd: 命令结构体指针
 * @param  VAL_type: 接收值的类型
 * @param  VAL_ID: 接收值的ID（共0~7）
 * @retval None
 * @example CMD_RxValue_Set(&CMD1, CMD_VAL_INT8, CMD_VAL_ID_NO1); // 设置CMD1的第CMD_VAL_ID_NO1个接收参数值的类型为int8_t
 */
void CMD_RxValue_Set(Command *cmd, CMD_VAL_type VAL_type, CMD_VAL_ID VAL_ID)
{
  cmd->types[VAL_ID] = VAL_type; // 设置命令值类型
}

/**
 * @brief  CMD的接收值读取函数，用户可以根据需要读取接收值，推荐在对应CMD的回调函数中调用此函数
 * @param  cmd: 命令结构体指针
 * @param  VAL_ID: 接收值的ID（共0~7）
 * @param  value: 接收值的指针
 * @retval None
 * @example CMD_RxValue_Read(&CMD1, CMD_VAL_ID_NO1, &value); // 读取CMD1的第CMD_VAL_ID_NO1个接收参数值，赋值到value中
 */
void CMD_RxValue_Read(Command *cmd, CMD_VAL_ID VAL_ID, void *value)
{
  if (cmd->types[VAL_ID] == CMD_VAL_NONE) // 如果没有设置类型，则返回
    return;
  switch (cmd->types[VAL_ID])
  {
  case CMD_VAL_INT8:
    *((int8_t *)value) = cmd->values[VAL_ID].int8_val;
    break;
  case CMD_VAL_UINT8:
    *((uint8_t *)value) = cmd->values[VAL_ID].uint8_val;
    break;
  case CMD_VAL_INT16:
    *((int16_t *)value) = cmd->values[VAL_ID].int16_val;
    break;
  case CMD_VAL_UINT16:
    *((uint16_t *)value) = cmd->values[VAL_ID].uint16_val;
    break;
  case CMD_VAL_INT32:
    *((int32_t *)value) = cmd->values[VAL_ID].int32_val;
    break;
  case CMD_VAL_UINT32:
    *((uint32_t *)value) = cmd->values[VAL_ID].uint32_val;
    break;
  case CMD_VAL_FLOAT:
    *((float *)value) = cmd->values[VAL_ID].float_val;
    break;
  default:
    break;
  }
}

/**
 * @brief  PAR设置函数，用户可以根据需要修改参数名称和信息
 * @param  param: 选择的参数结构体，提供PAR[PARNUMS]共PARNUMS个参数
 * @param  type: 选择的参数类型
 * @param  external_ptr: 指向外部变量的指针
 * @param  name: 参数名称
 * @param  info: 参数信息
 * @retval None
 * @example PAR_Set(&PAR[0], INT8, &Par[0], "PAR1", "PAR1 Info"); // 设置PAR1的名称为"PAR1"，信息为"PAR1 Info"，类型为int8_t，指向外部变量Par[0]
 */
void PAR_Set(Parameter *param, PAR_type type, void *external_ptr, const char *name, const char *info)
{
  param->type = type;
  param->external_ptr = external_ptr;
  memset(param->Name, 0, sizeof(param->Name));         // 清空数组
  memset(param->Info, 0, sizeof(param->Info));         // 清空数组
  strncpy(param->Name, name, sizeof(param->Name) - 1); // 拷贝字符串
  strncpy(param->Info, info, sizeof(param->Info) - 1); // 拷贝字符串
  switch (type)
  {
  case INT8:
    param->value.int8_val = *((int8_t *)external_ptr);
    break;
  case UINT8:
    param->value.uint8_val = *((uint8_t *)external_ptr);
    break;
  case INT16:
    param->value.int16_val = *((int16_t *)external_ptr);
    break;
  case UINT16:
    param->value.uint16_val = *((uint16_t *)external_ptr);
    break;
  case INT32:
    param->value.int32_val = *((int32_t *)external_ptr);
    break;
  case UINT32:
    param->value.uint32_val = *((uint32_t *)external_ptr);
    break;
  case FLOAT:
    param->value.float_val = *((float *)external_ptr);
    break;
  default:
    break;
  }
}

void PAR_Update(Parameter *param)
{
  if (param->external_ptr != NULL)
  {
    switch (param->type)
    {
    case INT8:
      *((int8_t *)param->external_ptr) = param->value.int8_val;
      break;
    case UINT8:
      *((uint8_t *)param->external_ptr) = param->value.uint8_val;
      break;
    case INT16:
      *((int16_t *)param->external_ptr) = param->value.int16_val;
      break;
    case UINT16:
      *((uint16_t *)param->external_ptr) = param->value.uint16_val;
      break;
    case INT32:
      *((int32_t *)param->external_ptr) = param->value.int32_val;
      break;
    case UINT32:
      *((uint32_t *)param->external_ptr) = param->value.uint32_val;
      break;
    case FLOAT:
      *((float *)param->external_ptr) = param->value.float_val;
      break;
    default:
      break;
    }
  }
}

void ANO_Protocol_00_Return(uint8_t RxID, uint8_t RxSC, uint8_t RxAC)
{
  uint8_t sumcheck = 0;
  uint8_t addcheck = 0;
  unsigned char buffer[11];
  buffer[0] = ANO_Protocol_Packet_Head;
  buffer[1] = ANO_Protocol_Address;
  buffer[2] = ANO_Protocol_Host_Address;
  buffer[3] = 0x00;
  buffer[4] = 0x03;
  buffer[5] = 0x00;
  buffer[6] = RxID;
  buffer[7] = RxSC;
  buffer[8] = RxAC;
  for (uint8_t i = 0; i < 9; i++)
  {
    sumcheck += buffer[i];
    addcheck += sumcheck;
  }
  buffer[9] = sumcheck;
  buffer[10] = addcheck;
  ANO_TX(buffer, 11);
}
void ANO_Protocol_E0_Address_Return(void)
{
  uint8_t sumcheck = 0;
  uint8_t addcheck = 0;
  unsigned char buffer[18];
  buffer[0] = ANO_Protocol_Packet_Head;
  buffer[1] = ANO_Protocol_Address;
  buffer[2] = ANO_Protocol_Host_Address;
  buffer[3] = 0xE3;
  buffer[4] = 0x0A;
  buffer[5] = 0x00;
  buffer[6] = ANO_Protocol_Address;
  buffer[7] = 0x01;
  buffer[8] = 0x00;
  buffer[9] = 0x01;
  buffer[10] = 0x00;
  buffer[11] = 0x01;
  buffer[12] = 0x00;
  buffer[13] = 0x01;
  buffer[14] = 0x00;
  buffer[15] = ANO_Protocol_Address;
  for (uint8_t i = 0; i < 16; i++)
  {
    sumcheck += buffer[i];
    addcheck += sumcheck;
  }
  buffer[16] = sumcheck;
  buffer[17] = addcheck;
  ANO_TX(buffer, 18);
}
void ANO_Protocol_E0_PARNums_Return(void)
{
  uint8_t sumcheck = 0;
  uint8_t addcheck = 0;
  unsigned char buffer[11];
  buffer[0] = ANO_Protocol_Packet_Head;
  buffer[1] = ANO_Protocol_Address;
  buffer[2] = ANO_Protocol_Host_Address;
  buffer[3] = 0xE0; // 帧头ID
  buffer[4] = 0x03; // Data数量
  buffer[5] = 0x00;
  buffer[6] = 0x01;    // CMD值
  buffer[7] = PARNums; // 参数数量
  buffer[8] = 0x00;
  for (uint8_t i = 0; i < 9; i++)
  {
    sumcheck += buffer[i];
    addcheck += sumcheck;
  }
  buffer[9] = sumcheck;
  buffer[10] = addcheck;
  ANO_TX(buffer, 11);
}
void ANO_Protocol_E1_Return(Parameter *param)
{
  uint8_t sumcheck = 0;
  uint8_t addcheck = 0;

  uint8_t Data_size = 0;

  unsigned char *byte_ptr0 = (unsigned char *)&param->PAR_ID;
  unsigned char *byte_ptr1 = (unsigned char *)&param->value;

  switch (param->type)
  {
  case INT8:
    Data_size = 1;
    break;
  case UINT8:
    Data_size = 1;
    break;
  case INT16:
    Data_size = 2;
    break;
  case UINT16:
    Data_size = 2;
    break;
  case INT32:
    Data_size = 4;
    break;
  case UINT32:
    Data_size = 4;
    break;
  case FLOAT:
    Data_size = 4;
    break;
  default:
    break;
  }

  unsigned char buffer[14];
  buffer[0] = ANO_Protocol_Packet_Head;
  buffer[1] = ANO_Protocol_Address;
  buffer[2] = ANO_Protocol_Host_Address;
  buffer[3] = 0xE1;          // 帧头ID
  buffer[4] = 2 + Data_size; // Data数量
  buffer[5] = 0x00;

  buffer[6] = byte_ptr0[0]; // PARx_ID
  buffer[7] = byte_ptr0[1];

  for (uint8_t i = 0; i < Data_size; i++)
  {
    buffer[8 + i] = byte_ptr1[i]; // PARx_Value
  }

  for (uint8_t i = 0; i < 8 + Data_size; i++)
  {
    sumcheck += buffer[i];
    addcheck += sumcheck;
  }
  buffer[8 + Data_size] = sumcheck;
  buffer[8 + Data_size + 1] = addcheck;
  ANO_TX(buffer, 8 + Data_size + 2);
}
void ANO_Protocol_E2_Return(Parameter *Param)
{
  uint8_t sumcheck = 0;
  uint8_t addcheck = 0;

  unsigned char *byte_ptr0 = (unsigned char *)&Param->PAR_ID;
  unsigned char *byte_ptr1 = (unsigned char *)&Param->Name;
  unsigned char *byte_ptr2 = (unsigned char *)&Param->Info;
  unsigned char buffer[81];
  buffer[0] = ANO_Protocol_Packet_Head;
  buffer[1] = ANO_Protocol_Address;
  buffer[2] = ANO_Protocol_Host_Address;
  buffer[3] = 0xE2; // 帧头ID
  buffer[4] = 73;   // Data数量
  buffer[5] = 0x00;

  buffer[6] = byte_ptr0[0]; // PARx_ID
  buffer[7] = byte_ptr0[1];

  buffer[8] = Param->type; // 数据类型

  for (uint8_t i = 0; i < 20; i++) // Name
    buffer[9 + i] = byte_ptr1[i];

  for (uint8_t i = 0; i < 50; i++) // Info
    buffer[29 + i] = byte_ptr2[i];

  for (uint8_t i = 0; i < 79; i++)
  {
    sumcheck += buffer[i];
    addcheck += sumcheck;
  }
  buffer[79] = sumcheck;
  buffer[80] = addcheck;
  ANO_TX(buffer, 81);
}
void ANO_Protocol_C1_Return(void)
{
  uint8_t sumcheck = 0;
  uint8_t addcheck = 0;
  unsigned char buffer[11];
  buffer[0] = ANO_Protocol_Packet_Head;
  buffer[1] = ANO_Protocol_Address;
  buffer[2] = ANO_Protocol_Host_Address;
  buffer[3] = 0xC1; // 帧头ID
  buffer[4] = 0x03; // Data数量
  buffer[5] = 0x00;
  buffer[6] = 0x00;    // CMD值
  buffer[7] = CMDNums; // 命令数量
  buffer[8] = 0x00;
  for (uint8_t i = 0; i < 9; i++)
  {
    sumcheck += buffer[i];
    addcheck += sumcheck;
  }
  buffer[9] = sumcheck;
  buffer[10] = addcheck;
  ANO_TX(buffer, 11);
}
void ANO_Protocol_C2_Return(Command *cmd)
{
  uint8_t sumcheck = 0;
  uint8_t addcheck = 0;
  unsigned char *byte_ptr0 = (unsigned char *)&cmd->CMD_ID;
  unsigned char *byte_ptr1 = (unsigned char *)&cmd->Name;
  unsigned char *byte_ptr2 = (unsigned char *)&cmd->Info;
  unsigned char buffer[91];
  buffer[0] = ANO_Protocol_Packet_Head;
  buffer[1] = ANO_Protocol_Address;
  buffer[2] = ANO_Protocol_Host_Address;
  buffer[3] = 0xC2; // 帧头ID
  buffer[4] = 83;   // Data数量
  buffer[5] = 0x00;
  buffer[6] = byte_ptr0[0]; // 第ID条命令
  buffer[7] = byte_ptr0[1];
  for (uint8_t i = 0; i < 3; i++)
    buffer[8 + i] = cmd->CID[i]; // CID值
  for (uint8_t i = 0; i < 8; i++)
    buffer[11 + i] = cmd->types[i]; // VAL[0]~VAL[7]参数类型
  for (uint8_t i = 0; i < 20; i++)
    buffer[19 + i] = byte_ptr1[i]; // 名称
  for (uint8_t i = 0; i < 50; i++)
    buffer[39 + i] = byte_ptr2[i]; // 介绍

  for (uint8_t i = 0; i < 89; i++)
  {
    sumcheck += buffer[i];
    addcheck += sumcheck;
  }
  buffer[89] = sumcheck;
  buffer[90] = addcheck;
  ANO_TX(buffer, 91);
}

void ANO_RX_Process(uint8_t rxdata)
{
  static uint8_t ANO_Protocol_RxState = 0;
  static uint8_t ANO_Protocol_pRxPacket = 0;
  static uint8_t ANO_Protocol_RxNum = 0;

  uint8_t RxData = rxdata;
  if (ANO_Protocol_RxState == 0)
  {
    if (RxData == ANO_Protocol_Packet_Head)
    {
      ANO_Protocol_RxNum = 0;
      ANO_Protocol_RxState = 10;
      ANO_Protocol_pRxPacket = 0;
    }
  }
  else if (ANO_Protocol_RxState == 10)
  {
    if (RxData == ANO_Protocol_Host_Address)
    {
      ANO_Protocol_RxState = 11;
    }
    else
    {
      ANO_Protocol_RxState = 0;
    }
  }
  else if (ANO_Protocol_RxState == 11)
  {
    if (RxData == ANO_Protocol_Broadcast_Address || RxData == ANO_Protocol_Address)
    {
      ANO_Protocol_RxState = 12;
    }
    else
    {
      ANO_Protocol_RxState = 0;
    }
  }
  else if (ANO_Protocol_RxState == 12)
  {
    ANO_Protocol_RxID = RxData;
    if (ANO_Protocol_RxID == ANO_Protocol_Parameter_Command || ANO_Protocol_RxID == ANO_Protocol_Parameter_Update)
    {
      ANO_Protocol_RxState = 20;
    }
    else if (ANO_Protocol_RxID == ANO_Protocol_CMD_Init || ANO_Protocol_RxID == ANO_Protocol_CMD_Enable)
    {
      ANO_Protocol_RxState = 30;
    }
    else
      ANO_Protocol_RxState = 0;
  }
  else if (ANO_Protocol_RxState == 20)
  {
    ANO_Protocol_RxNum = RxData;
    ANO_Protocol_RxState = 21;
  }
  else if (ANO_Protocol_RxState == 21)
  {
    ANO_Protocol_RxState = 22;
  }
  else if (ANO_Protocol_RxState == 22)
  {
    if (ANO_Protocol_pRxPacket < ANO_Protocol_RxNum)
    {
      ANO_Protocol_Temp[ANO_Protocol_pRxPacket] = RxData;
      ANO_Protocol_pRxPacket++;
    }
    else
    {
      ANO_Protocol_Temp[ANO_Protocol_pRxPacket] = RxData;
      ANO_Protocol_RxState = 23;
    }
  }
  else if (ANO_Protocol_RxState == 23) // ID = Ex
  {
    if (ANO_Protocol_RxID == ANO_Protocol_Parameter_Command)
    {
      ANO_Protocol_CMD = ANO_Protocol_Temp[0];
      *((char *)&ANO_Protocol_VAR) = ANO_Protocol_Temp[1];
      *((char *)&ANO_Protocol_VAR + 1) = ANO_Protocol_Temp[2];
      if (ANO_Protocol_CMD == 0)
      {
        ANO_Protocol_E0_Address_Return();
      }
      else if (ANO_Protocol_CMD == 0x01)
      {
        ANO_Protocol_E0_PARNums_Return();
      }
      else if (ANO_Protocol_CMD == 0x02)
      {
        ANO_Protocol_E1_Return(&PAR[ANO_Protocol_VAR]);
      }
      else if (ANO_Protocol_CMD == 0x03)
      {
        ANO_Protocol_E2_Return(&PAR[ANO_Protocol_VAR]);
      }
      else if (ANO_Protocol_CMD == 0x10)
      {

        ANO_Protocol_00_Return(ANO_Protocol_Parameter_Command, ANO_Protocol_Temp[ANO_Protocol_RxNum], RxData);
      }
    }
    else if (ANO_Protocol_RxID == ANO_Protocol_Parameter_Update)
    {

      *((char *)&ANO_Protocol_VAR) = ANO_Protocol_Temp[0];
      *((char *)&ANO_Protocol_VAR + 1) = ANO_Protocol_Temp[1];
      if (ANO_Protocol_VAR < PARNums)
      {
        switch (PAR[ANO_Protocol_VAR].type)
        {
        case INT8:
          ANO_Protocol_RxPAR_Datasize = 1;
          PAR[ANO_Protocol_VAR].value.int8_val = ANO_Protocol_Temp[2];
          break;
        case UINT8:
          ANO_Protocol_RxPAR_Datasize = 1;
          PAR[ANO_Protocol_VAR].value.uint8_val = ANO_Protocol_Temp[2];
          break;
        case INT16:
          ANO_Protocol_RxPAR_Datasize = 2;
          PAR[ANO_Protocol_VAR].value.int16_val = *((int16_t *)(&ANO_Protocol_Temp[2]));
          break;
        case UINT16:
          ANO_Protocol_RxPAR_Datasize = 2;
          PAR[ANO_Protocol_VAR].value.uint16_val = *((uint16_t *)(&ANO_Protocol_Temp[2]));
          break;
        case INT32:
          ANO_Protocol_RxPAR_Datasize = 4;
          PAR[ANO_Protocol_VAR].value.int32_val = *((int32_t *)(&ANO_Protocol_Temp[2]));
          break;
        case UINT32:
          ANO_Protocol_RxPAR_Datasize = 4;
          PAR[ANO_Protocol_VAR].value.uint32_val = *((uint32_t *)(&ANO_Protocol_Temp[2]));
          break;
        case FLOAT:
          ANO_Protocol_RxPAR_Datasize = 4;
          PAR[ANO_Protocol_VAR].value.float_val = *((float *)(&ANO_Protocol_Temp[2]));
          break;
        default:
          break;
        }
        PAR_Update(&PAR[ANO_Protocol_VAR]);
        ANO_Protocol_00_Return(ANO_Protocol_Parameter_Update, ANO_Protocol_Temp[ANO_Protocol_RxNum], RxData);
      }
    }

    ANO_Protocol_RxState = 0;
    ANO_Protocol_RxFlag = 1;
  }
  else if (ANO_Protocol_RxState == 30)
  {
    ANO_Protocol_RxNum = RxData;
    ANO_Protocol_RxState = 31;
  }
  else if (ANO_Protocol_RxState == 31)
  {
    ANO_Protocol_RxState = 32;
  }
  else if (ANO_Protocol_RxState == 32)
  {
    if (ANO_Protocol_pRxPacket < ANO_Protocol_RxNum)
    {
      ANO_Protocol_Temp[ANO_Protocol_pRxPacket] = RxData;
      ANO_Protocol_pRxPacket++;
    }
    else
    {
      ANO_Protocol_Temp[ANO_Protocol_pRxPacket] = RxData;
      ANO_Protocol_RxState = 33;
    }
  }
  else if (ANO_Protocol_RxState == 33) // ID = Cx
  {
    if (ANO_Protocol_RxID == ANO_Protocol_CMD_Init)
    {
      ANO_Protocol_CMD = ANO_Protocol_Temp[0];
      *((char *)&ANO_Protocol_VAR) = ANO_Protocol_Temp[1];
      *((char *)&ANO_Protocol_VAR + 1) = ANO_Protocol_Temp[2];
      if (ANO_Protocol_CMD == 0)
      {
        if (ANO_Protocol_StateSelect == 1)
          ANO_Protocol_C1_Return();
      }
      else if (ANO_Protocol_CMD == 1)
      {
        if (ANO_Protocol_StateSelect == 1)
          ANO_Protocol_C2_Return(&CMD[ANO_Protocol_VAR]);
      }
    }
    else if (ANO_Protocol_RxID == ANO_Protocol_CMD_Enable)
    {
      uint8_t datasize = 0;
      CID_V1 = ANO_Protocol_Temp[0];
      CMD[CID_V1].Flag = 1;
      if (ANO_Protocol_RxNum > 3)
      {
        for (uint8_t i = 0; i < 8; i++)
        {
          switch (CMD[CID_V1].types[i])
          {
          case CMD_VAL_INT8:
            CMD[CID_V1].values[i].int8_val = ANO_Protocol_Temp[3 + datasize];
            datasize += 1;
            break;
          case CMD_VAL_UINT8:
            CMD[CID_V1].values[i].uint8_val = ANO_Protocol_Temp[3 + datasize];
            datasize += 1;
            break;
          case CMD_VAL_INT16:
            CMD[CID_V1].values[i].int16_val = *((int16_t *)(&ANO_Protocol_Temp[3 + datasize]));
            datasize += 2;
            break;
          case CMD_VAL_UINT16:
            CMD[CID_V1].values[i].uint16_val = *((uint16_t *)(&ANO_Protocol_Temp[3 + datasize]));
            datasize += 2;
            break;
          case CMD_VAL_INT32:
            CMD[CID_V1].values[i].int32_val = *((int32_t *)(&ANO_Protocol_Temp[3 + datasize]));
            datasize += 4;
            break;
          case CMD_VAL_UINT32:
            CMD[CID_V1].values[i].uint32_val = *((uint32_t *)(&ANO_Protocol_Temp[3 + datasize]));
            datasize += 4;
            break;
          case CMD_VAL_FLOAT:
            CMD[CID_V1].values[i].float_val = *((float *)(&ANO_Protocol_Temp[3 + datasize]));
            datasize += 4;
            break;
          default:
            break;
          }
        }
      }
      ANO_Protocol_00_Return(ANO_Protocol_CMD_Enable, ANO_Protocol_Temp[ANO_Protocol_RxNum], RxData);
    }

    ANO_Protocol_RxState = 0;
    ANO_Protocol_RxFlag = 1;
  }
}

void ANO_Init(void)
{
  for (uint8_t i = 0; i < CMDNums; i++)
  {
    CMD[i].CMD_ID = i;
    CMD[i].CID[0] = i;
    CMD[i].CID[1] = 0;
    CMD[i].CID[2] = 0;
    for (uint8_t j = 0; j < 8; j++)
    {
      CMD[i].types[j] = CMD_VAL_NONE;
      CMD[i].values[j].uint8_val = 0;
    }
    CMD[i].Flag = 0;
    CMD[i].Callback = CMD_Func_Test;
    strncpy(CMD[i].Name, "测试名字", sizeof("测试名字"));
    strncpy(CMD[i].Info, "测试说明", sizeof("测试名字"));
  }

  for (uint8_t i = 0; i < PARNums; i++)
  {
    PAR[i].PAR_ID = i;
    PAR[i].type = FLOAT;
    PAR[i].value.float_val = 0.0f;
    strncpy(PAR[i].Name, "测试名字", sizeof("测试名字"));
    strncpy(PAR[i].Info, "测试说明", sizeof("测试名字"));
    PAR[i].external_ptr = NULL;
  }
}

void CMD_Func(void)
{
  uint8_t i;
  for (i = 0; i < CMDNums; i++)
  {
    if (CMD[i].Flag == 1)
    {
      CMD[i].Flag = 0;
      if (CMD[i].Callback != NULL)
      {
        CMD[i].Callback();
      }
    }
  }
}

// 协议帧最大长度 = 头6字节 + 数据12字节 (3个float) + 校验2字节 = 20字节
#define ANO_Protocol_MAX_PACKET_SIZE 100

void ANO_Protocol_Tx_Generic(FlexiDatas_ShowChannel channel, uint8_t data_length, const uint8_t *data_bytes)
{
  uint8_t sumcheck = 0;
  uint8_t addcheck = 0;
  uint8_t buffer[ANO_Protocol_MAX_PACKET_SIZE];

  // 构建协议头
  buffer[0] = 0xAB;
  buffer[1] = 0xFE;
  buffer[2] = 0xFF;
  buffer[3] = channel;
  buffer[4] = data_length;
  buffer[5] = 0x00;

  // 拷贝数据（带长度保护）
  uint8_t *data_ptr = &buffer[6];
  uint8_t copy_len = data_length;
  if (6 + data_length + 2 > ANO_Protocol_MAX_PACKET_SIZE)
  {                                                  // 头6+数据+校验2
    copy_len = ANO_Protocol_MAX_PACKET_SIZE - 6 - 2; // 自动截断超长数据
  }
  memcpy(data_ptr, data_bytes, copy_len);

  // 计算校验（6字节头 + 实际数据长度）
  for (uint8_t i = 0; i < 6 + copy_len; i++)
  {
    sumcheck += buffer[i];
    addcheck += sumcheck;
  }

  // 填充校验位
  buffer[6 + copy_len] = sumcheck;
  buffer[6 + copy_len + 1] = addcheck;

  // 发送实际数据长度：头6 + 数据copy_len + 校验2
  ANO_TX(buffer, 6 + copy_len + 2); // 调用发送函数
}

// 单值发送模板
#define DEFINE_TX_FUNC(type, func_name)                               \
  void func_name(type data, FlexiDatas_ShowChannel channel)                          \
  {                                                                   \
    ANO_Protocol_Tx_Generic(channel, sizeof(data), (uint8_t *)&data); \
  }

// 多值发送模板
#define DEFINE_MULTI_TX_FUNC(func_name, data_type, size)                             \
  void func_name(data_type data1, data_type data2, data_type data3, FlexiDatas_ShowChannel channel) \
  {                                                                                  \
    uint8_t buffer[size * 3];                                                        \
    memcpy(buffer, &data1, size);                                                    \
    memcpy(buffer + size, &data2, size);                                             \
    memcpy(buffer + size * 2, &data3, size);                                         \
    ANO_Protocol_Tx_Generic(channel, size * 3, buffer);                              \
  }

// 生成所有单值类型函数
DEFINE_TX_FUNC(uint8_t, ANO_Protocol_Tx_uint8)
DEFINE_TX_FUNC(uint16_t, ANO_Protocol_Tx_uint16)
DEFINE_TX_FUNC(uint32_t, ANO_Protocol_Tx_uint32)
DEFINE_TX_FUNC(int8_t, ANO_Protocol_Tx_int8)
DEFINE_TX_FUNC(int16_t, ANO_Protocol_Tx_int16)
DEFINE_TX_FUNC(int32_t, ANO_Protocol_Tx_int32)
DEFINE_TX_FUNC(float, ANO_Protocol_Tx_float)

// 生成多值类型函数
DEFINE_MULTI_TX_FUNC(ANO_Protocol_Tx_int8int8int8, int8_t, 1)
DEFINE_MULTI_TX_FUNC(ANO_Protocol_Tx_uint8uint8uint8, uint8_t, 1)
DEFINE_MULTI_TX_FUNC(ANO_Protocol_Tx_int16int16int16, int16_t, 2)
DEFINE_MULTI_TX_FUNC(ANO_Protocol_Tx_uint16uint16uint16, uint16_t, 2)
DEFINE_MULTI_TX_FUNC(ANO_Protocol_Tx_floatfloatfloat, float, 4)

// 特殊多参数函数（示例）

void ANO_Protocol_Tx_floatfloat(float data1, float data2, FlexiDatas_ShowChannel channel)
{
  uint8_t buffer[8];
  memcpy(buffer, &data1, 4);
  memcpy(buffer + 4, &data2, 4);
  ANO_Protocol_Tx_Generic(channel, 8, buffer);
}

void ANO_Protocol_Tx_floatfloatfloatfloat(float data1, float data2, float data3, float data4, FlexiDatas_ShowChannel channel)
{
  uint8_t buffer[16];
  memcpy(buffer, &data1, 4);
  memcpy(buffer + 4, &data2, 4);
  memcpy(buffer + 8, &data3, 4);
  memcpy(buffer + 12, &data4, 4);
  ANO_Protocol_Tx_Generic(channel, sizeof(buffer), buffer);
}

void ANO_Protocol_Tx_uint8uint8floatfloatfloat(uint8_t data1, uint8_t data2, float data3, float data4, float data5, FlexiDatas_ShowChannel channel)
{
  uint8_t buffer[14];
  memcpy(buffer, &data1, 1);
  memcpy(buffer + 1, &data2, 1);
  memcpy(buffer + 2, &data3, 4);
  memcpy(buffer + 6, &data4, 4);
  memcpy(buffer + 10, &data5, 4);
  ANO_Protocol_Tx_Generic(channel, sizeof(buffer), buffer);
}
