// //////////////////////////////////////////////////////////////////////////////////	 
// //本程序只供学习使用，未经作者许可，不得用于其它任何用途
// //测试硬件：单片机STM32H743IIT6,正点原子Apollo STM32F4/F7开发板,主频400MHZ，晶振25MHZ
// //QDtech-TFT液晶驱动 for STM32 IO模拟
// //xiao冯@ShenZhen QDtech co.,LTD
// //公司网站:www.qdtft.com
// //淘宝网站：http://qdtech.taobao.com
// //wiki技术网站：http://www.lcdwiki.com
// //我司提供技术支持，任何技术问题欢迎随时交流学习
// //固话(传真) :+86 0755-23594567 
// //手机:15989313508（冯工） 
// //邮箱:lcdwiki01@gmail.com    support@lcdwiki.com    goodtft@163.com 
// //技术支持QQ:3002773612  3002778157
// //技术交流QQ群:324828016
// //创建日期:2018/08/22
// //版本：V1.0
// //版权所有，盗版必究。
// //Copyright(C) 深圳市全动电子技术有限公司 2018-2028
// //All rights reserved
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
//  /* @attention
//   *
//   * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
//   * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
//   * TIME. AS A RESULT, QD electronic SHALL NOT BE HELD LIABLE FOR ANY
//   * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
//   * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
//   * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
// **************************************************************************************************/	
// #include "touch.h" 
// #include "lcd.h"
// #include "delay.h"
// #include "stdlib.h"
// #include "math.h"
// #include "gui.h"	    

// _m_tp_dev tp_dev=
// {
// 	TP_Init,
// 	NULL,
// 	0,
// 	0,
//  	0,	
// };					
// //默认为touchtype=0的数据.
 
// /*****************************************************************************
//  * @name       :u8 TP_Init(void)
//  * @date       :2018-08-09 
//  * @function   :Initialization touch screen
//  * @parameters :None
//  * @retvalue   :0-no calibration
// 								1-Has been calibrated
// ******************************************************************************/  
// u8 TP_Init(void)
// {			    		   
// 	if(FT6336_Init())
// 	{
// 		return 1;
// 	}
// 	tp_dev.scan=FT6336_Scan;	//扫描函数指向GT911触摸屏扫描
// 	return 0;
// }


