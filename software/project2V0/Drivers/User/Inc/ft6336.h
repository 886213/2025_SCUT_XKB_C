//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//测试硬件：单片机STM32F103C8T6,F103C8T6核心开发板,主频72MHZ，晶振8MHZ
//QDtech-TFT液晶驱动 for STM32 IO模拟
//xiao冯@ShenZhen QDtech co.,LTD
//公司网站:www.qdtft.com
//淘宝网站：http://qdtech.taobao.com
//wiki技术网站：http://www.lcdwiki.com
//我司提供技术支持，任何技术问题欢迎随时交流学习
//固话(传真) :+86 0755-23594567 
//手机:15989313508（冯工） 
//邮箱:lcdwiki01@gmail.com    support@lcdwiki.com    goodtft@163.com 
//技术支持QQ:3002773612  3002778157
//技术交流QQ群:324828016
//创建日期:2018/08/09
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 深圳市全动电子技术有限公司 2018-2028
//All rights reserved
/****************************************************************************************************
//=========================================电源接线================================================//
//     LCD模块                STM32单片机
//      VCC          接        DC5V/3.3V      //电源
//      GND          接          GND          //电源地
//=======================================液晶屏数据线接线==========================================//
//本模块默认数据总线类型为SPI总线
//     LCD模块                STM32单片机    
//    SDI(MOSI)      接          PB15         //液晶屏SPI总线数据写信号
//    SDO(MISO)      接          PB14         //液晶屏SPI总线数据读信号，如果不需要读，可以不接线
//=======================================液晶屏控制线接线==========================================//
//     LCD模块 					      STM32单片机 
//       LED         接          PD6          //液晶屏背光控制信号，如果不需要控制，接5V或3.3V
//       SCK         接          PB13         //液晶屏SPI总线时钟信号
//     LCD_RS        接          PD5          //液晶屏数据/命令控制信号
//     LCD_RST       接          PD12         //液晶屏复位控制信号
//     LCD_CS        接          PD11         //液晶屏片选控制信号
//=========================================触摸屏触接线=========================================//
//如果模块不带触摸功能或者带有触摸功能，但是不需要触摸功能，则不需要进行触摸屏接线
//	   LCD模块                STM32单片机 
//     CTP_INT       接          PH11         //电容触摸屏中断信号
//     CTP_SDA       接          PI3          //电容触摸屏IIC数据信号
//     CTP_RST       接          PI8          //电容触摸屏复位信号
//     CTP_SCL       接          PH6          //电容触摸屏IIC时钟信号
**************************************************************************************************/	
 /* @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, QD electronic SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
**************************************************************************************************/
#ifndef __FT6336_H
#define __FT6336_H	
#include "sys.h"	

#define CTP_MAX_TOUCH 2

//与电容触摸屏连接的芯片引脚(未包含IIC引脚) 
//IO操作函数	 
#define FT_RST(n)   (n?HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_RESET))	//FT6336复位引脚
#define FT_INT    	HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_7) 		//FT6336中断引脚	

//I2C读写命令	
#define FT_CMD_WR 				0X70    	//写命令
#define FT_CMD_RD 				0X71		//读命令

// FT6336 I2C地址（7位地址，左移1位后为0x70写，0x71读）
#define FT6336_I2C_ADDR 0x38
  
//FT5426 部分寄存器定义 
#define FT_DEVIDE_MODE 			0x00   		//FT6336模式控制寄存器
#define FT_REG_NUM_FINGER   0x02		  //触摸状态寄存器

#define FT_TP1_REG 				0X03	  	//第一个触摸点数据地址
#define FT_TP2_REG 				0X09		  //第二个触摸点数据地址

#define FT_ID_G_CIPHER_MID    0x9F      //芯片代号（中字节） 默认值0x26
#define FT_ID_G_CIPHER_LOW    0xA0      //芯片代号（低字节） 0x01: Ft6336G  0x02: Ft6336U 
#define	FT_ID_G_LIB_VERSION		0xA1		  //版本		
#define FT_ID_G_CIPHER_HIGH   0xA3      //芯片代号（高字节） 默认0x64 
#define FT_ID_G_MODE 			    0xA4   		//FT6636中断模式控制寄存器
#define FT_ID_G_FOCALTECH_ID  0xA8      //VENDOR ID 默认值为0x11
#define FT_ID_G_THGROUP			  0x80   		//触摸有效值设置寄存器
#define FT_ID_G_PERIODACTIVE	0x88   		//激活状态周期设置寄存器

extern I2C_HandleTypeDef hi2c2; // 修改为I2C2句柄

u8 FT6336_Init(void);
u8 FT6336_Scan(void);
void CTP_I2C2_Init(void);

#endif

















