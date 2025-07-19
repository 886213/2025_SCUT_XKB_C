// //////////////////////////////////////////////////////////////////////////////////
// // 本程序只供学习使用，未经作者许可，不得用于其它任何用途
// // 测试硬件：单片机STM32F103C8T6,F103C8T6核心开发板,主频72MHZ，晶振8MHZ
// // QDtech-TFT液晶驱动 for STM32 IO模拟
// // xiao冯@ShenZhen QDtech co.,LTD
// // 公司网站:www.qdtft.com
// // 淘宝网站：http://qdtech.taobao.com
// // wiki技术网站：http://www.lcdwiki.com
// // 我司提供技术支持，任何技术问题欢迎随时交流学习
// // 固话(传真) :+86 0755-23594567
// // 手机:15989313508（冯工）
// // 邮箱:lcdwiki01@gmail.com    support@lcdwiki.com    goodtft@163.com
// // 技术支持QQ:3002773612  3002778157
// // 技术交流QQ群:324828016
// // 创建日期:2018/08/09
// // 版本：V1.0
// // 版权所有，盗版必究。
// // Copyright(C) 深圳市全动电子技术有限公司 2018-2028
// // All rights reserved
// /****************************************************************************************************
// //=========================================电源接线================================================//
// //     LCD模块                STM32单片机
// //      VCC          接        DC5V/3.3V      //电源
// //      GND          接          GND          //电源地
// //=======================================液晶屏数据线接线==========================================//
// //本模块默认数据总线类型为SPI总线
// //     LCD模块                STM32单片机
// //    SDI(MOSI)      接          PB15         //液晶屏SPI总线数据写信号
// //    SDO(MISO)      接          PB14         //液晶屏SPI总线数据读信号，如果不需要读，可以不接线
// //=======================================液晶屏控制线接线==========================================//
// //     LCD模块 					      STM32单片机
// //       LED         接          PD6          //液晶屏背光控制信号，如果不需要控制，接5V或3.3V
// //       SCK         接          PB13         //液晶屏SPI总线时钟信号
// //     LCD_RS        接          PD5          //液晶屏数据/命令控制信号
// //     LCD_RST       接          PD12         //液晶屏复位控制信号
// //     LCD_CS        接          PD11         //液晶屏片选控制信号
// //=========================================触摸屏触接线=========================================//
// //如果模块不带触摸功能或者带有触摸功能，但是不需要触摸功能，则不需要进行触摸屏接线
// //	   LCD模块                STM32单片机
// //     CTP_INT       接          PH11         //电容触摸屏中断信号
// //     CTP_SDA       接          PI3          //电容触摸屏IIC数据信号
// //     CTP_RST       接          PI8          //电容触摸屏复位信号
// //     CTP_SCL       接          PH6          //电容触摸屏IIC时钟信号
// **************************************************************************************************/
// /* @attention
//  *
//  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
//  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
//  * TIME. AS A RESULT, QD electronic SHALL NOT BE HELD LIABLE FOR ANY
//  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
//  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
//  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
//  **************************************************************************************************/
// #include "ft6336.h"
// #include "touch.h"
// #include "ctpiic.h"
// #include "delay.h"
// #include "string.h"
// #include "lcd.h"

// extern u8 touch_flag;

// I2C_HandleTypeDef hi2c2; // 修改为I2C2句柄

// // GPIO初始化（配置I2C2引脚）
// void CPT_GPIO_Init(void)
// {
//   GPIO_InitTypeDef GPIO_InitStruct = {0};

//   // 使能GPIOB和I2C2时钟
//   __HAL_RCC_GPIOB_CLK_ENABLE();
//   __HAL_RCC_GPIOD_CLK_ENABLE();
//   __HAL_RCC_I2C2_CLK_ENABLE(); // 修改为I2C2时钟

//   // PB10 - I2C2_SCL, PB11 - I2C2_SDA（根据实际硬件连接选择引脚）
//   GPIO_InitStruct.Pin = GPIO_PIN_10 | GPIO_PIN_11; 
//   GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
//   GPIO_InitStruct.Pull = GPIO_PULLUP;
//   GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
//   GPIO_InitStruct.Alternate = GPIO_AF4_I2C2; // 复用功能改为AF4_I2C2
//   HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

//   // PB12 - RST
//   GPIO_InitStruct.Pin = GPIO_PIN_12; 
//   GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//   GPIO_InitStruct.Pull = GPIO_PULLUP;
//   GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
//   HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

//   // PB13 - INT
//   GPIO_InitStruct.Pin = GPIO_PIN_7; 
//   GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
//   GPIO_InitStruct.Pull = GPIO_PULLUP;
//   GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
//   HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

// }

// void CTP_I2C2_Init(void)
// {
//   hi2c2.Instance = I2C2;
//   hi2c2.Init.Timing = 0x00707CBB; // 标准模式100kHz（需根据实际系统时钟调整）
//   hi2c2.Init.OwnAddress1 = 0;
//   hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
//   hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
//   hi2c2.Init.OwnAddress2 = 0;
//   hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
//   hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
//   hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
//   HAL_I2C_Init(&hi2c2);
// }

// // 读取FT6336寄存器值（修改为hi2c2）
// HAL_StatusTypeDef FT6336_ReadReg(uint8_t reg, uint8_t *data, uint8_t len)
// {
//   return HAL_I2C_Mem_Read(&hi2c2, FT6336_I2C_ADDR << 1, reg, I2C_MEMADD_SIZE_8BIT, data, len, 100);
// }

// // 写入FT6336寄存器（修改为hi2c2）
// HAL_StatusTypeDef FT6336_WriteReg(uint8_t reg, uint8_t *data, uint8_t len)
// {
//   return HAL_I2C_Mem_Write(&hi2c2, FT6336_I2C_ADDR << 1, reg, I2C_MEMADD_SIZE_8BIT, data, len, 100);
// }

// /*****************************************************************************
//  * @name       :u8 FT5426_Init(void)
//  * @date       :2020-05-13
//  * @function   :Initialize the ft5426 touch screen
//  * @parameters :none
//  * @retvalue   :0-Initialization successful
//                 1-initialization failed
// ******************************************************************************/
// u8 FT6336_Init(void)
// {
//   u8 temp[2];

//   CPT_GPIO_Init();

//   CTP_I2C2_Init();// 初始化电容屏的I2C总线

//   FT_RST(0);      // 复位
//   delay_ms(10);
//   FT_RST(1); // 释放复位
//   delay_ms(500);

//   FT6336_ReadReg(FT_ID_G_FOCALTECH_ID, &temp[0], 1);
//   if (temp[0] != 0x11)
//   {
//     return 1;
//   }
//   FT6336_ReadReg(FT_ID_G_CIPHER_MID, &temp[0], 2);
//   if (temp[0] != 0x26)
//   {
//     return 1;
//   }
//   if ((temp[1] != 0x00) && (temp[1] != 0x01) && (temp[1] != 0x02))
//   {
//     return 1;
//   }
//   FT6336_ReadReg(FT_ID_G_CIPHER_HIGH, &temp[0], 1);
//   if (temp[0] != 0x64)
//   {
//     return 1;
//   }
//   return 0;
// }

// const u16 FT6336_TPX_TBL[2] = {FT_TP1_REG, FT_TP2_REG};

// /*****************************************************************************
//  * @name       :u8 FT5426_Scan(void)
//  * @date       :2020-05-13
//  * @function   :Scan touch screen (query mode)
//  * @parameters :none
//  * @retvalue   :Current touch screen status
//                 0-No touch
//                 1-With touch
// ******************************************************************************/
// u8 FT6336_Scan(void)
// {
//   u8 buf[4];
//   u8 i = 0;
//   u8 res = 0;
//   u8 temp;
//   u8 mode;
//   static u8 t = 0; // 控制查询间隔,从而降低CPU占用率
//   t++;
//   if ((t % 10) == 0 || t < 10) // 空闲时,每进入10次CTP_Scan函数才检测1次,从而节省CPU使用率
//   {
//     FT6336_ReadReg(FT_REG_NUM_FINGER, &mode, 1); // 读取触摸点的状态
//     if (mode && (mode < 3))
//     {
//       temp = 0XFF << mode; // 将点的个数转换为1的位数,匹配tp_dev.sta定义
//       tp_dev.sta = (~temp) | TP_PRES_DOWN | TP_CATH_PRES;
//       for (i = 0; i < CTP_MAX_TOUCH; i++)
//       {
//         FT6336_ReadReg(FT6336_TPX_TBL[i], buf, 4); // 读取XY坐标值
//         if (tp_dev.sta & (1 << i))                // 触摸有效?
//         {
//           switch (lcddev.dir)
//           {
//           case 0:
//             tp_dev.x[i] = ((u16)(buf[0] & 0X0F) << 8) + buf[1];
//             tp_dev.y[i] = ((u16)(buf[2] & 0X0F) << 8) + buf[3];
//             break;
//           case 1:
//             tp_dev.y[i] = lcddev.height - (((u16)(buf[0] & 0X0F) << 8) + buf[1]);
//             tp_dev.x[i] = ((u16)(buf[2] & 0X0F) << 8) + buf[3];
//             break;
//           case 2:
//             tp_dev.x[i] = lcddev.width - (((u16)(buf[0] & 0X0F) << 8) + buf[1]);
//             tp_dev.y[i] = lcddev.height - (((u16)(buf[2] & 0X0F) << 8) + buf[3]);
//             break;
//           case 3:
//             tp_dev.y[i] = ((u16)(buf[0] & 0X0F) << 8) + buf[1];
//             tp_dev.x[i] = lcddev.width - (((u16)(buf[2] & 0X0F) << 8) + buf[3]);
//             break;
//           }
//           // if((buf[0]&0XF0)!=0X80)tp_dev.x[i]=tp_dev.y[i]=0;//必须是contact事件，才认为有效
//           // printf("x[%d]:%d,y[%d]:%d\r\n",i,tp_dev.x[i],i,tp_dev.y[i]);
//         }
//       }
//       res = 1;
//       if (tp_dev.x[0] == 0 && tp_dev.y[0] == 0)
//         mode = 0; // 读到的数据都是0,则忽略此次数据
//       t = 0;      // 触发一次,则会最少连续监测10次,从而提高命中率
//     }
//   }
//   if (mode == 0) // 无触摸点按下
//   {
//     if (tp_dev.sta & TP_PRES_DOWN) // 之前是被按下的
//     {
//       tp_dev.sta &= ~(1 << 7); // 标记按键松开
//     }
//     else // 之前就没有被按下
//     {
//       tp_dev.x[0] = 0xffff;
//       tp_dev.y[0] = 0xffff;
//       tp_dev.sta &= 0XE0; // 清除点有效标记
//     }
//   }
//   if (t > 240)
//     t = 10; // 重新从10开始计数
//   return res;
// }
