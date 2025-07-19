// //////////////////////////////////////////////////////////////////////////////////
// // 本程序只供学习使用，未经作者许可，不得用于其它任何用途
// // 测试硬件：单片机STM32H743IIT6,正点原子Apollo STM32F4/F7开发板,主频400MHZ，晶振25MHZ
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
// // 创建日期:2018/08/22
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
#include "lcd.h"
// #include "stdlib.h"
// #include "delay.h"
// #include "spi.h"

// #define LCD_SPI SPI4_Handler

// // 管理LCD重要参数
// // 默认为竖屏
// _lcd_dev lcddev;

// // 画笔颜色,背景颜色
// u16 POINT_COLOR = 0x0000, BACK_COLOR = 0xFFFF;
// u16 DeviceCode;

// HAL_StatusTypeDef LCD_SPI_Transmit(SPI_HandleTypeDef *hspi, uint16_t pData, uint32_t Size);
// HAL_StatusTypeDef LCD_SPI_TransmitBuffer(SPI_HandleTypeDef *hspi, uint16_t *pData, uint32_t Size);

// /*****************************************************************************
//  * @name       :void LCD_WR_REG(u8 data)
//  * @date       :2018-08-09
//  * @function   :Write an 8-bit command to the LCD screen
//  * @parameters :data:Command value to be written
//  * @retvalue   :None
//  ******************************************************************************/
// void LCD_WR_REG(u8 data)
// {
//   LCD_CS_CLR;
//   LCD_RS_CLR;
//   SPI4_ReadWriteByte(data);
//   LCD_CS_SET;
// }

// /*****************************************************************************
//  * @name       :void LCD_WR_DATA(u8 data)
//  * @date       :2018-08-09
//  * @function   :Write an 8-bit data to the LCD screen
//  * @parameters :data:data value to be written
//  * @retvalue   :None
//  ******************************************************************************/
// void LCD_WR_DATA(u8 data)
// {
//   LCD_CS_CLR;
//   LCD_RS_SET;
//   SPI4_ReadWriteByte(data);
//   LCD_CS_SET;
// }

// u8 LCD_RD_DATA(void)
// {
//   u8 data;
//   LCD_CS_CLR;
//   LCD_RS_SET;
//   SPI4_SetSpeed(SPI_BAUDRATEPRESCALER_32);
//   data = SPI4_ReadWriteByte(0xFF);
//   SPI4_SetSpeed(SPI_BAUDRATEPRESCALER_8);
//   LCD_CS_SET;
//   return data;
// }

// /*****************************************************************************
//  * @name       :void LCD_WriteReg(u8 LCD_Reg, u16 LCD_RegValue)
//  * @date       :2018-08-09
//  * @function   :Write data into registers
//  * @parameters :LCD_Reg:Register address
//                 LCD_RegValue:Data to be written
//  * @retvalue   :None
// ******************************************************************************/
// void LCD_WriteReg(u8 LCD_Reg, u16 LCD_RegValue)
// {
//   LCD_WR_REG(LCD_Reg);
//   LCD_WR_DATA(LCD_RegValue);
// }

// u8 LCD_ReadReg(u8 LCD_Reg)
// {
//   LCD_WR_REG(LCD_Reg);
//   return LCD_RD_DATA();
// }

// /*****************************************************************************
//  * @name       :void LCD_WriteRAM_Prepare(void)
//  * @date       :2018-08-09
//  * @function   :Write GRAM
//  * @parameters :None
//  * @retvalue   :None
//  ******************************************************************************/
// void LCD_WriteRAM_Prepare(void)
// {
//   LCD_WR_REG(lcddev.wramcmd);
// }

// void LCD_ReadRAM_Prepare(void)
// {
//   LCD_WR_REG(lcddev.rramcmd);
// }

// /*****************************************************************************
//  * @name       :void Lcd_WriteData_16Bit(u16 Data)
//  * @date       :2018-08-09
//  * @function   :Write an 16-bit command to the LCD screen
//  * @parameters :Data:Data to be written
//  * @retvalue   :None
//  ******************************************************************************/
// void Lcd_WriteData_16Bit(u16 Data)
// {
//   LCD_CS_CLR;
//   LCD_RS_SET;
//   SPI4_ReadWriteByte(Data >> 8);
//   SPI4_ReadWriteByte(Data);
//   LCD_CS_SET;
// }

// u16 Lcd_ReadData_16Bit(void)
// {
//   u16 r, g;
//   LCD_CS_CLR;
//   LCD_RS_CLR;
//   SPI4_ReadWriteByte(lcddev.rramcmd);
//   SPI4_SetSpeed(SPI_BAUDRATEPRESCALER_32);
//   LCD_RS_SET;
//   SPI4_ReadWriteByte(0xFF);
//   r = SPI4_ReadWriteByte(0xFF);
//   g = SPI4_ReadWriteByte(0xFF);
//   SPI4_SetSpeed(SPI_BAUDRATEPRESCALER_8);
//   LCD_CS_SET;
//   r <<= 8;
//   r |= g;
//   return r;
// }

// /*****************************************************************************
//  * @name       :void LCD_DrawPoint(u16 x,u16 y)
//  * @date       :2018-08-09
//  * @function   :Write a pixel data at a specified location
//  * @parameters :x:the x coordinate of the pixel
//                 y:the y coordinate of the pixel
//  * @retvalue   :None
// ******************************************************************************/
// void LCD_DrawPoint(u16 x, u16 y)
// {
//   LCD_SetCursor(x, y); // 设置光标位置
//   Lcd_WriteData_16Bit(POINT_COLOR);
// }

// u16 LCD_ReadPoint(u16 x, u16 y)
// {
//   u16 color;
//   LCD_SetCursor(x, y); // 设置光标位置
//   color = Lcd_ReadData_16Bit();
//   return color;
// }

// /*****************************************************************************
//  * @name       :void LCD_Clear(u16 Color)
//  * @date       :2018-08-09
//  * @function   :Full screen filled LCD screen
//  * @parameters :color:Filled color
//  * @retvalue   :None
//  ******************************************************************************/
// void LCD_Clear(u16 Color)
// {
//   unsigned int i, m;
//   LCD_SetWindows(0, 0, lcddev.width - 1, lcddev.height - 1);
//   LCD_CS_CLR;
//   LCD_RS_SET;
//   // 修改为16位数据宽度，写入数据更加效率，不需要拆分
//   LCD_SPI.Init.DataSize = SPI_DATASIZE_16BIT; //	16位数据宽度
//   HAL_SPI_Init(&LCD_SPI);

//   LCD_SPI_Transmit(&LCD_SPI, BACK_COLOR, lcddev.width * lcddev.height); // 启动传输

//   // 改回8位数据宽度，因为指令和部分数据都是按照8位传输的
//   LCD_SPI.Init.DataSize = SPI_DATASIZE_8BIT; //	8位数据宽度
//   HAL_SPI_Init(&LCD_SPI);
//   LCD_CS_SET;
// }

// /*****************************************************************************
//  * @name       :void LCD_Clear(u16 Color)
//  * @date       :2018-08-09
//  * @function   :Initialization LCD screen GPIO
//  * @parameters :None
//  * @retvalue   :None
//  ******************************************************************************/
// void LCD_GPIOInit(void)
// {
//   GPIO_InitTypeDef GPIO_Initure;

//   __HAL_RCC_GPIOE_CLK_ENABLE(); // 使能GPIOE时钟

//   GPIO_Initure.Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11; // PF6
//   GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;                                // 推挽输出
//   GPIO_Initure.Pull = GPIO_PULLUP;                                        // 上拉
//   GPIO_Initure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;                         // 快速
//   HAL_GPIO_Init(GPIOE, &GPIO_Initure);                                    // 初始化
//   HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11, GPIO_PIN_SET);
//   LCD_LED(1);
// }

// /*****************************************************************************
//  * @name       :void LCD_RESET(void)
//  * @date       :2018-08-09
//  * @function   :Reset LCD screen
//  * @parameters :None
//  * @retvalue   :None
//  ******************************************************************************/
// void LCD_RESET(void)
// {
//   LCD_RST_CLR;
//   delay_ms(100);
//   LCD_RST_SET;
//   delay_ms(50);
// }

// /*****************************************************************************
//  * @name       :void LCD_RESET(void)
//  * @date       :2018-08-09
//  * @function   :Initialization LCD screen
//  * @parameters :None
//  * @retvalue   :None
//  ******************************************************************************/
// void LCD_Init(void)
// {
//   SPI4_Init();    // 初始化SPI
//   LCD_GPIOInit(); // LCD GPIO初始化
//   LCD_RESET();    // LCD 复位
//   //*************4.0 ST7796S TN初始化**********//
//   LCD_WR_REG(0xF0);
//   LCD_WR_DATA(0xC3);
//   LCD_WR_REG(0xF0);
//   LCD_WR_DATA(0x96);
//   LCD_WR_REG(0x36);
//   LCD_WR_DATA(0x48);
//   LCD_WR_REG(0x3A);
//   LCD_WR_DATA(0x05);
//   LCD_WR_REG(0xB0);
//   LCD_WR_DATA(0x80);
//   LCD_WR_REG(0xB6);
//   LCD_WR_DATA(0x00);
//   LCD_WR_DATA(0x02);
//   LCD_WR_REG(0xB5);
//   LCD_WR_DATA(0x02);
//   LCD_WR_DATA(0x03);
//   LCD_WR_DATA(0x00);
//   LCD_WR_DATA(0x04);
//   LCD_WR_REG(0xB1);
//   LCD_WR_DATA(0x80);
//   LCD_WR_DATA(0x10);
//   LCD_WR_REG(0xB4);
//   LCD_WR_DATA(0x00);
//   LCD_WR_REG(0xB7);
//   LCD_WR_DATA(0xC6);
//   LCD_WR_REG(0xC5);
//   LCD_WR_DATA(0x1C);
//   LCD_WR_REG(0xE4);
//   LCD_WR_DATA(0x31);
//   LCD_WR_REG(0xE8);
//   LCD_WR_DATA(0x40);
//   LCD_WR_DATA(0x8A);
//   LCD_WR_DATA(0x00);
//   LCD_WR_DATA(0x00);
//   LCD_WR_DATA(0x29);
//   LCD_WR_DATA(0x19);
//   LCD_WR_DATA(0xA5);
//   LCD_WR_DATA(0x33);
//   LCD_WR_REG(0xC2);
//   LCD_WR_REG(0xA7);

//   LCD_WR_REG(0xE0);
//   LCD_WR_DATA(0xF0);
//   LCD_WR_DATA(0x09);
//   LCD_WR_DATA(0x13);
//   LCD_WR_DATA(0x12);
//   LCD_WR_DATA(0x12);
//   LCD_WR_DATA(0x2B);
//   LCD_WR_DATA(0x3C);
//   LCD_WR_DATA(0x44);
//   LCD_WR_DATA(0x4B);
//   LCD_WR_DATA(0x1B);
//   LCD_WR_DATA(0x18);
//   LCD_WR_DATA(0x17);
//   LCD_WR_DATA(0x1D);
//   LCD_WR_DATA(0x21);

//   LCD_WR_REG(0XE1);
//   LCD_WR_DATA(0xF0);
//   LCD_WR_DATA(0x09);
//   LCD_WR_DATA(0x13);
//   LCD_WR_DATA(0x0C);
//   LCD_WR_DATA(0x0D);
//   LCD_WR_DATA(0x27);
//   LCD_WR_DATA(0x3B);
//   LCD_WR_DATA(0x44);
//   LCD_WR_DATA(0x4D);
//   LCD_WR_DATA(0x0B);
//   LCD_WR_DATA(0x17);
//   LCD_WR_DATA(0x17);
//   LCD_WR_DATA(0x1D);
//   LCD_WR_DATA(0x21);

//   LCD_WR_REG(0xF0);
//   LCD_WR_DATA(0x3C);
//   LCD_WR_REG(0xF0);
//   LCD_WR_DATA(0x69);
//   LCD_WR_REG(0X13);
//   LCD_WR_REG(0X11);
//   LCD_WR_REG(0X29);

//   // 设置LCD属性参数
//   LCD_direction(USE_HORIZONTAL); // 设置LCD显示方向
//   LCD_Clear(WHITE);              // 清全屏白色
// }

// /*****************************************************************************
//  * @name       :void LCD_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd)
//  * @date       :2018-08-09
//  * @function   :Setting LCD display window
//  * @parameters :xStar:the bebinning x coordinate of the LCD display window
//                 yStar:the bebinning y coordinate of the LCD display window
//                 xEnd:the endning x coordinate of the LCD display window
//                 yEnd:the endning y coordinate of the LCD display window
//  * @retvalue   :None
// ******************************************************************************/
// void LCD_SetWindows(u16 xStar, u16 yStar, u16 xEnd, u16 yEnd)
// {
//   LCD_WR_REG(lcddev.setxcmd);
//   LCD_WR_DATA(xStar >> 8);
//   LCD_WR_DATA(0x00FF & xStar);
//   LCD_WR_DATA(xEnd >> 8);
//   LCD_WR_DATA(0x00FF & xEnd);

//   LCD_WR_REG(lcddev.setycmd);
//   LCD_WR_DATA(yStar >> 8);
//   LCD_WR_DATA(0x00FF & yStar);
//   LCD_WR_DATA(yEnd >> 8);
//   LCD_WR_DATA(0x00FF & yEnd);

//   LCD_WriteRAM_Prepare(); // 开始写入GRAM
// }

// /*****************************************************************************
//  * @name       :void LCD_SetCursor(u16 Xpos, u16 Ypos)
//  * @date       :2018-08-09
//  * @function   :Set coordinate value
//  * @parameters :Xpos:the  x coordinate of the pixel
//                 Ypos:the  y coordinate of the pixel
//  * @retvalue   :None
// ******************************************************************************/
// void LCD_SetCursor(u16 Xpos, u16 Ypos)
// {
//   LCD_SetWindows(Xpos, Ypos, Xpos, Ypos);
// }

// /*****************************************************************************
//  * @name       :void LCD_direction(u8 direction)
//  * @date       :2018-08-09
//  * @function   :Setting the display direction of LCD screen
//  * @parameters :direction:0-0 degree
//                           1-90 degree
//                           2-180 degree
//                           3-270 degree
//  * @retvalue   :None
// ******************************************************************************/
// void LCD_direction(u8 direction)
// {
//   lcddev.setxcmd = 0x2A;
//   lcddev.setycmd = 0x2B;
//   lcddev.wramcmd = 0x2C;
//   lcddev.rramcmd = 0x2E;
//   lcddev.dir = direction % 4;
//   switch (lcddev.dir)
//   {
//   case 0:
//     lcddev.width = LCD_W;
//     lcddev.height = LCD_H;
//     LCD_WriteReg(0x36, (1 << 3) | (1 << 6));
//     break;
//   case 1:
//     lcddev.width = LCD_H;
//     lcddev.height = LCD_W;
//     LCD_WriteReg(0x36, (1 << 3) | (1 << 5));
//     break;
//   case 2:
//     lcddev.width = LCD_W;
//     lcddev.height = LCD_H;
//     LCD_WriteReg(0x36, (1 << 3) | (1 << 7));
//     break;
//   case 3:
//     lcddev.width = LCD_H;
//     lcddev.height = LCD_W;
//     LCD_WriteReg(0x36, (1 << 3) | (1 << 7) | (1 << 6) | (1 << 5));
//     break;
//   default:
//     break;
//   }
// }

// /***************************************************************************************************************************************
//  *	函 数 名: LCD_CopyBuffer
//  *
//  *	入口参数: x - 起始水平坐标
//  *				 y - 起始垂直坐标
//  *			 	 width  - 目标区域的水平宽度
//  *				 height - 目标区域的垂直宽度
//  *				*pImage - 数据存储区的首地址
//  *
//  *	函数功能: 在指定坐标处，直接将数据复制到屏幕的显存
//  *
//  *	说    明: 批量复制函数，可用于移植 LVGL 或者将摄像头采集的图像显示出来
//  *
//  *****************************************************************************************************************************************/

// void LCD_CopyBuffer(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t *DataBuff)
// {
//   LCD_SetWindows(x, y, x + width - 1, y + height - 1);
//   int i;
//   LCD_CS_CLR;
//   LCD_RS_SET;

//   // // 修改为16位数据宽度，写入数据更加效率，不需要拆分
//   // LCD_SPI.Init.DataSize = SPI_DATASIZE_16BIT; //	16位数据宽度
//   // SPI4_Handler.Init.FirstBit = SPI_FIRSTBIT_LSB;
//   // HAL_SPI_Init(&LCD_SPI);

//   // // LCD_SPI_TransmitBuffer(&LCD_SPI, DataBuff, width * height);
//   // for (i = 0; i < width; i++)
//   //   HAL_SPI_Transmit(&LCD_SPI, DataBuff + 2 * i * height, height, 1000);

//   // // 改回8位数据宽度，因为指令和部分数据都是按照8位传输的
//   // LCD_SPI.Init.DataSize = SPI_DATASIZE_8BIT; //	8位数据宽度
//   // SPI4_Handler.Init.FirstBit = SPI_FIRSTBIT_MSB;
//   // HAL_SPI_Init(&LCD_SPI);
  
//   // for (i = 0; i < width * height; i++)
//   // {
    
//   //   HAL_SPI_Transmit(&LCD_SPI, DataBuff + i * 2 + 1, 1, 100);
//   //   HAL_SPI_Transmit(&LCD_SPI, DataBuff + i * 2, 1, 100);
//     for (i = 0; i < width; i++)
//       HAL_SPI_Transmit(&LCD_SPI, DataBuff + i * height * 2, height * 2, 1000);
//   // }
//   LCD_CS_SET;
//   LCD_SetWindows(x, y, x + width - 1, y + height - 1);
// }
// // LCD_CS_CLR;
// // LCD_RS_SET;

// // // 修改为16位数据宽度，写入数据更加效率，不需要拆分
// //  LCD_SPI.Init.DataSize 	= SPI_DATASIZE_16BIT;   //	16位数据宽度
// // //  SPI4_Handler.Init.FirstBit = SPI_FIRSTBIT_LSB;
// //  HAL_SPI_Init(&LCD_SPI);

// // LCD_SPI_TransmitBuffer(&LCD_SPI, DataBuff,width * height) ;

// // // 改回8位数据宽度，因为指令和部分数据都是按照8位传输的
// // LCD_SPI.Init.DataSize 	= SPI_DATASIZE_8BIT;    //	8位数据宽度
// // // SPI4_Handler.Init.FirstBit = SPI_FIRSTBIT_MSB;
// //  HAL_SPI_Init(&LCD_SPI);

// // LCD_CS_SET;
// // }

// u16 LCD_Read_ID(void)
// {
//   u8 i, val[3] = {0};
//   LCD_WR_REG(0xF0); // Command Set Control
//   LCD_WR_DATA(0xC3);

//   LCD_WR_REG(0xF0);
//   LCD_WR_DATA(0x96);
//   LCD_CS_CLR;
//   for (i = 1; i < 4; i++)
//   {
//     LCD_RS_CLR;
//     SPI4_ReadWriteByte(0xFB);
//     LCD_RS_SET;
//     SPI4_ReadWriteByte(0x10 + i);
//     LCD_RS_CLR;
//     SPI4_ReadWriteByte(0xD3);
//     SPI4_SetSpeed(SPI_BAUDRATEPRESCALER_32);
//     LCD_RS_SET;
//     val[i - 1] = SPI4_ReadWriteByte(0xFF);
//     LCD_RS_CLR;
//     SPI4_ReadWriteByte(0xFB);
//     LCD_RS_SET;
//     SPI4_ReadWriteByte(0x00);
//   }
//   SPI4_SetSpeed(SPI_BAUDRATEPRESCALER_8);
//   LCD_CS_SET;
//   LCD_WR_REG(0xF0); // Command Set Control
//   LCD_WR_DATA(0x3C);
//   LCD_WR_REG(0xF0);
//   LCD_WR_DATA(0x69);
//   lcddev.id = val[1];
//   lcddev.id <<= 8;
//   lcddev.id |= val[2];
//   return lcddev.id;
// }

// /**
//  * @brief Handle SPI Communication Timeout.
//  * @param hspi: pointer to a SPI_HandleTypeDef structure that contains
//  *              the configuration information for SPI module.
//  * @param Flag: SPI flag to check
//  * @param Status: flag state to check
//  * @param Timeout: Timeout duration
//  * @param Tickstart: Tick start value
//  * @retval HAL status
//  */
// HAL_StatusTypeDef LCD_SPI_WaitOnFlagUntilTimeout(SPI_HandleTypeDef *hspi, uint32_t Flag, FlagStatus Status,
//                                                  uint32_t Tickstart, uint32_t Timeout)
// {
//   /* Wait until flag is set */
//   while ((__HAL_SPI_GET_FLAG(hspi, Flag) ? SET : RESET) == Status)
//   {
//     /* Check for the Timeout */
//     if ((((HAL_GetTick() - Tickstart) >= Timeout) && (Timeout != HAL_MAX_DELAY)) || (Timeout == 0U))
//     {
//       return HAL_TIMEOUT;
//     }
//   }
//   return HAL_OK;
// }

// /**
//  * @brief  Close Transfer and clear flags.
//  * @param  hspi: pointer to a SPI_HandleTypeDef structure that contains
//  *               the configuration information for SPI module.
//  * @retval HAL_ERROR: if any error detected
//  *         HAL_OK: if nothing detected
//  */
// void LCD_SPI_CloseTransfer(SPI_HandleTypeDef *hspi)
// {
//   uint32_t itflag = hspi->Instance->SR;

//   __HAL_SPI_CLEAR_EOTFLAG(hspi);
//   __HAL_SPI_CLEAR_TXTFFLAG(hspi);

//   /* Disable SPI peripheral */
//   __HAL_SPI_DISABLE(hspi);

//   /* Disable ITs */
//   __HAL_SPI_DISABLE_IT(hspi, (SPI_IT_EOT | SPI_IT_TXP | SPI_IT_RXP | SPI_IT_DXP | SPI_IT_UDR | SPI_IT_OVR | SPI_IT_FRE | SPI_IT_MODF));

//   /* Disable Tx DMA Request */
//   CLEAR_BIT(hspi->Instance->CFG1, SPI_CFG1_TXDMAEN | SPI_CFG1_RXDMAEN);

//   /* Report UnderRun error for non RX Only communication */
//   if (hspi->State != HAL_SPI_STATE_BUSY_RX)
//   {
//     if ((itflag & SPI_FLAG_UDR) != 0UL)
//     {
//       SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_UDR);
//       __HAL_SPI_CLEAR_UDRFLAG(hspi);
//     }
//   }

//   /* Report OverRun error for non TX Only communication */
//   if (hspi->State != HAL_SPI_STATE_BUSY_TX)
//   {
//     if ((itflag & SPI_FLAG_OVR) != 0UL)
//     {
//       SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_OVR);
//       __HAL_SPI_CLEAR_OVRFLAG(hspi);
//     }
//   }

//   /* SPI Mode Fault error interrupt occurred -------------------------------*/
//   if ((itflag & SPI_FLAG_MODF) != 0UL)
//   {
//     SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_MODF);
//     __HAL_SPI_CLEAR_MODFFLAG(hspi);
//   }

//   /* SPI Frame error interrupt occurred ------------------------------------*/
//   if ((itflag & SPI_FLAG_FRE) != 0UL)
//   {
//     SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_FRE);
//     __HAL_SPI_CLEAR_FREFLAG(hspi);
//   }

//   hspi->TxXferCount = (uint16_t)0UL;
//   hspi->RxXferCount = (uint16_t)0UL;
// }

// /**
//  * @brief  专为屏幕清屏而修改，将需要清屏的颜色批量传输
//  * @param  hspi   : spi的句柄
//  * @param  pData  : 要写入的数据
//  * @param  Size   : 数据大小
//  * @retval HAL status
//  */

// HAL_StatusTypeDef LCD_SPI_Transmit(SPI_HandleTypeDef *hspi, uint16_t pData, uint32_t Size)
// {
//   uint32_t tickstart;
//   uint32_t Timeout = 1000;  // 超时判断
//   uint32_t LCD_pData_32bit; // 按32位传输时的数据
//   uint32_t LCD_TxDataCount; // 传输计数
//   HAL_StatusTypeDef errorcode = HAL_OK;

//   /* Check Direction parameter */
//   assert_param(IS_SPI_DIRECTION_2LINES_OR_1LINE_2LINES_TXONLY(hspi->Init.Direction));

//   /* Process Locked */
//   __HAL_LOCK(hspi);

//   /* Init tickstart for timeout management*/
//   tickstart = HAL_GetTick();

//   if (hspi->State != HAL_SPI_STATE_READY)
//   {
//     errorcode = HAL_BUSY;
//     __HAL_UNLOCK(hspi);
//     return errorcode;
//   }

//   if (Size == 0UL)
//   {
//     errorcode = HAL_ERROR;
//     __HAL_UNLOCK(hspi);
//     return errorcode;
//   }

//   /* Set the transaction information */
//   hspi->State = HAL_SPI_STATE_BUSY_TX;
//   hspi->ErrorCode = HAL_SPI_ERROR_NONE;

//   LCD_TxDataCount = Size;                  // 传输的数据长度
//   LCD_pData_32bit = (pData << 16) | pData; // 按32位传输时，合并2个像素点的颜色

//   /*Init field not used in handle to zero */
//   hspi->pRxBuffPtr = NULL;
//   hspi->RxXferSize = (uint16_t)0UL;
//   hspi->RxXferCount = (uint16_t)0UL;
//   hspi->TxISR = NULL;
//   hspi->RxISR = NULL;

//   /* Configure communication direction : 1Line */
//   if (hspi->Init.Direction == SPI_DIRECTION_1LINE)
//   {
//     SPI_1LINE_TX(hspi);
//   }

//   // 不使用硬件 TSIZE 控制，此处设置为0，即不限制传输的数据长度
//   MODIFY_REG(hspi->Instance->CR2, SPI_CR2_TSIZE, 0);

//   /* Enable SPI peripheral */
//   __HAL_SPI_ENABLE(hspi);

//   if (hspi->Init.Mode == SPI_MODE_MASTER)
//   {
//     /* Master transfer start */
//     SET_BIT(hspi->Instance->CR1, SPI_CR1_CSTART);
//   }

//   /* Transmit data in 16 Bit mode */
//   while (LCD_TxDataCount > 0UL)
//   {
//     /* Wait until TXP flag is set to send data */
//     if (__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_TXP))
//     {
//       if ((hspi->TxXferCount > 1UL) && (hspi->Init.FifoThreshold > SPI_FIFO_THRESHOLD_01DATA))
//       {
//         *((__IO uint32_t *)&hspi->Instance->TXDR) = (uint32_t)LCD_pData_32bit;
//         LCD_TxDataCount -= (uint16_t)2UL;
//       }
//       else
//       {
//         *((__IO uint16_t *)&hspi->Instance->TXDR) = (uint16_t)pData;
//         LCD_TxDataCount--;
//       }
//     }
//     else
//     {
//       /* Timeout management */
//       if ((((HAL_GetTick() - tickstart) >= Timeout) && (Timeout != HAL_MAX_DELAY)) || (Timeout == 0U))
//       {
//         /* Call standard close procedure with error check */
//         LCD_SPI_CloseTransfer(hspi);

//         /* Process Unlocked */
//         __HAL_UNLOCK(hspi);

//         SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_TIMEOUT);
//         hspi->State = HAL_SPI_STATE_READY;
//         return HAL_ERROR;
//       }
//     }
//   }

//   if (LCD_SPI_WaitOnFlagUntilTimeout(hspi, SPI_SR_TXC, RESET, tickstart, Timeout) != HAL_OK)
//   {
//     SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_FLAG);
//   }

//   SET_BIT((hspi)->Instance->CR1, SPI_CR1_CSUSP); // 请求挂起SPI传输
//   /* 等待SPI挂起 */
//   if (LCD_SPI_WaitOnFlagUntilTimeout(hspi, SPI_FLAG_SUSP, RESET, tickstart, Timeout) != HAL_OK)
//   {
//     SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_FLAG);
//   }
//   LCD_SPI_CloseTransfer(hspi); /* Call standard close procedure with error check */

//   SET_BIT((hspi)->Instance->IFCR, SPI_IFCR_SUSPC); // 清除挂起标志位

//   /* Process Unlocked */
//   __HAL_UNLOCK(hspi);

//   hspi->State = HAL_SPI_STATE_READY;

//   if (hspi->ErrorCode != HAL_SPI_ERROR_NONE)
//   {
//     return HAL_ERROR;
//   }
//   return errorcode;
// }

// /**
//  * @brief  专为批量写入数据修改，使之不限长度的传输数据
//  * @param  hspi   : spi的句柄
//  * @param  pData  : 要写入的数据
//  * @param  Size   : 数据大小
//  * @retval HAL status
//  */
// HAL_StatusTypeDef LCD_SPI_TransmitBuffer(SPI_HandleTypeDef *hspi, uint16_t *pData, uint32_t Size)
// {
//   uint32_t tickstart;
//   uint32_t Timeout = 1000;  // 超时判断
//   uint32_t LCD_TxDataCount; // 传输计数
//   HAL_StatusTypeDef errorcode = HAL_OK;

//   /* Check Direction parameter */
//   assert_param(IS_SPI_DIRECTION_2LINES_OR_1LINE_2LINES_TXONLY(hspi->Init.Direction));

//   /* Process Locked */
//   __HAL_LOCK(hspi);

//   /* Init tickstart for timeout management*/
//   tickstart = HAL_GetTick();

//   if (hspi->State != HAL_SPI_STATE_READY)
//   {
//     errorcode = HAL_BUSY;
//     __HAL_UNLOCK(hspi);
//     return errorcode;
//   }

//   if (Size == 0UL)
//   {
//     errorcode = HAL_ERROR;
//     __HAL_UNLOCK(hspi);
//     return errorcode;
//   }

//   /* Set the transaction information */
//   hspi->State = HAL_SPI_STATE_BUSY_TX;
//   hspi->ErrorCode = HAL_SPI_ERROR_NONE;

//   LCD_TxDataCount = Size; // 传输的数据长度

//   /*Init field not used in handle to zero */
//   hspi->pRxBuffPtr = NULL;
//   hspi->RxXferSize = (uint16_t)0UL;
//   hspi->RxXferCount = (uint16_t)0UL;
//   hspi->TxISR = NULL;
//   hspi->RxISR = NULL;

//   /* Configure communication direction : 1Line */
//   if (hspi->Init.Direction == SPI_DIRECTION_1LINE)
//   {
//     SPI_1LINE_TX(hspi);
//   }

//   // 不使用硬件 TSIZE 控制，此处设置为0，即不限制传输的数据长度
//   MODIFY_REG(hspi->Instance->CR2, SPI_CR2_TSIZE, 0);

//   /* Enable SPI peripheral */
//   __HAL_SPI_ENABLE(hspi);

//   if (hspi->Init.Mode == SPI_MODE_MASTER)
//   {
//     /* Master transfer start */
//     SET_BIT(hspi->Instance->CR1, SPI_CR1_CSTART);
//   }

//   /* Transmit data in 16 Bit mode */
//   while (LCD_TxDataCount > 0UL)
//   {
//     /* Wait until TXP flag is set to send data */
//     if (__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_TXP))
//     {
//       if ((LCD_TxDataCount > 1UL) && (hspi->Init.FifoThreshold > SPI_FIFO_THRESHOLD_01DATA))
//       {
//         *((__IO uint32_t *)&hspi->Instance->TXDR) = *((uint32_t *)pData);
//         pData += 2;
//         LCD_TxDataCount -= 2;
//       }
//       else
//       {
//         *((__IO uint16_t *)&hspi->Instance->TXDR) = *((uint16_t *)pData);
//         pData += 1;
//         LCD_TxDataCount--;
//       }
//     }
//     else
//     {
//       /* Timeout management */
//       if ((((HAL_GetTick() - tickstart) >= Timeout) && (Timeout != HAL_MAX_DELAY)) || (Timeout == 0U))
//       {
//         /* Call standard close procedure with error check */
//         LCD_SPI_CloseTransfer(hspi);

//         /* Process Unlocked */
//         __HAL_UNLOCK(hspi);

//         SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_TIMEOUT);
//         hspi->State = HAL_SPI_STATE_READY;
//         return HAL_ERROR;
//       }
//     }
//   }

//   if (LCD_SPI_WaitOnFlagUntilTimeout(hspi, SPI_SR_TXC, RESET, tickstart, Timeout) != HAL_OK)
//   {
//     SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_FLAG);
//   }

//   SET_BIT((hspi)->Instance->CR1, SPI_CR1_CSUSP); // 请求挂起SPI传输
//   /* 等待SPI挂起 */
//   if (LCD_SPI_WaitOnFlagUntilTimeout(hspi, SPI_FLAG_SUSP, RESET, tickstart, Timeout) != HAL_OK)
//   {
//     SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_FLAG);
//   }
//   LCD_SPI_CloseTransfer(hspi); /* Call standard close procedure with error check */

//   SET_BIT((hspi)->Instance->IFCR, SPI_IFCR_SUSPC); // 清除挂起标志位

//   /* Process Unlocked */
//   __HAL_UNLOCK(hspi);

//   hspi->State = HAL_SPI_STATE_READY;

//   if (hspi->ErrorCode != HAL_SPI_ERROR_NONE)
//   {
//     return HAL_ERROR;
//   }
//   return errorcode;
// }