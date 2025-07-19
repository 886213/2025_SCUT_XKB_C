// //////////////////////////////////////////////////////////////////////////////////
// // ������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
// // ����Ӳ������Ƭ��STM32H743IIT6,����ԭ��Apollo STM32F4/F7������,��Ƶ400MHZ������25MHZ
// // QDtech-TFTҺ������ for STM32 IOģ��
// // xiao��@ShenZhen QDtech co.,LTD
// // ��˾��վ:www.qdtft.com
// // �Ա���վ��http://qdtech.taobao.com
// // wiki������վ��http://www.lcdwiki.com
// // ��˾�ṩ����֧�֣��κμ������⻶ӭ��ʱ����ѧϰ
// // �̻�(����) :+86 0755-23594567
// // �ֻ�:15989313508���빤��
// // ����:lcdwiki01@gmail.com    support@lcdwiki.com    goodtft@163.com
// // ����֧��QQ:3002773612  3002778157
// // ��������QQȺ:324828016
// // ��������:2018/08/22
// // �汾��V1.0
// // ��Ȩ���У�����ؾ���
// // Copyright(C) ������ȫ�����Ӽ������޹�˾ 2018-2028
// // All rights reserved
// /****************************************************************************************************
// //=========================================��Դ����================================================//
// //     LCDģ��                STM32��Ƭ��
// //      VCC          ��        DC5V/3.3V      //��Դ
// //      GND          ��          GND          //��Դ��
// //=======================================Һ���������߽���==========================================//
// //��ģ��Ĭ��������������ΪSPI����
// //     LCDģ��                STM32��Ƭ��
// //    SDI(MOSI)      ��          PB15         //Һ����SPI��������д�ź�
// //    SDO(MISO)      ��          PB14         //Һ����SPI�������ݶ��źţ��������Ҫ�������Բ�����
// //=======================================Һ���������߽���==========================================//
// //     LCDģ�� 					      STM32��Ƭ��
// //       LED         ��          PD6          //Һ������������źţ��������Ҫ���ƣ���5V��3.3V
// //       SCK         ��          PB13         //Һ����SPI����ʱ���ź�
// //     LCD_RS        ��          PD5          //Һ��������/��������ź�
// //     LCD_RST       ��          PD12         //Һ������λ�����ź�
// //     LCD_CS        ��          PD11         //Һ����Ƭѡ�����ź�
// //=========================================������������=========================================//
// //���ģ�鲻���������ܻ��ߴ��д������ܣ����ǲ���Ҫ�������ܣ�����Ҫ���д���������
// //	   LCDģ��                STM32��Ƭ��
// //     CTP_INT       ��          PH11         //���ݴ������ж��ź�
// //     CTP_SDA       ��          PI3          //���ݴ�����IIC�����ź�
// //     CTP_RST       ��          PI8          //���ݴ�������λ�ź�
// //     CTP_SCL       ��          PH6          //���ݴ�����IICʱ���ź�
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

// // ����LCD��Ҫ����
// // Ĭ��Ϊ����
// _lcd_dev lcddev;

// // ������ɫ,������ɫ
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
//   LCD_SetCursor(x, y); // ���ù��λ��
//   Lcd_WriteData_16Bit(POINT_COLOR);
// }

// u16 LCD_ReadPoint(u16 x, u16 y)
// {
//   u16 color;
//   LCD_SetCursor(x, y); // ���ù��λ��
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
//   // �޸�Ϊ16λ���ݿ�ȣ�д�����ݸ���Ч�ʣ�����Ҫ���
//   LCD_SPI.Init.DataSize = SPI_DATASIZE_16BIT; //	16λ���ݿ��
//   HAL_SPI_Init(&LCD_SPI);

//   LCD_SPI_Transmit(&LCD_SPI, BACK_COLOR, lcddev.width * lcddev.height); // ��������

//   // �Ļ�8λ���ݿ�ȣ���Ϊָ��Ͳ������ݶ��ǰ���8λ�����
//   LCD_SPI.Init.DataSize = SPI_DATASIZE_8BIT; //	8λ���ݿ��
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

//   __HAL_RCC_GPIOE_CLK_ENABLE(); // ʹ��GPIOEʱ��

//   GPIO_Initure.Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11; // PF6
//   GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;                                // �������
//   GPIO_Initure.Pull = GPIO_PULLUP;                                        // ����
//   GPIO_Initure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;                         // ����
//   HAL_GPIO_Init(GPIOE, &GPIO_Initure);                                    // ��ʼ��
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
//   SPI4_Init();    // ��ʼ��SPI
//   LCD_GPIOInit(); // LCD GPIO��ʼ��
//   LCD_RESET();    // LCD ��λ
//   //*************4.0 ST7796S TN��ʼ��**********//
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

//   // ����LCD���Բ���
//   LCD_direction(USE_HORIZONTAL); // ����LCD��ʾ����
//   LCD_Clear(WHITE);              // ��ȫ����ɫ
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

//   LCD_WriteRAM_Prepare(); // ��ʼд��GRAM
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
//  *	�� �� ��: LCD_CopyBuffer
//  *
//  *	��ڲ���: x - ��ʼˮƽ����
//  *				 y - ��ʼ��ֱ����
//  *			 	 width  - Ŀ�������ˮƽ���
//  *				 height - Ŀ������Ĵ�ֱ���
//  *				*pImage - ���ݴ洢�����׵�ַ
//  *
//  *	��������: ��ָ�����괦��ֱ�ӽ����ݸ��Ƶ���Ļ���Դ�
//  *
//  *	˵    ��: �������ƺ�������������ֲ LVGL ���߽�����ͷ�ɼ���ͼ����ʾ����
//  *
//  *****************************************************************************************************************************************/

// void LCD_CopyBuffer(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t *DataBuff)
// {
//   LCD_SetWindows(x, y, x + width - 1, y + height - 1);
//   int i;
//   LCD_CS_CLR;
//   LCD_RS_SET;

//   // // �޸�Ϊ16λ���ݿ�ȣ�д�����ݸ���Ч�ʣ�����Ҫ���
//   // LCD_SPI.Init.DataSize = SPI_DATASIZE_16BIT; //	16λ���ݿ��
//   // SPI4_Handler.Init.FirstBit = SPI_FIRSTBIT_LSB;
//   // HAL_SPI_Init(&LCD_SPI);

//   // // LCD_SPI_TransmitBuffer(&LCD_SPI, DataBuff, width * height);
//   // for (i = 0; i < width; i++)
//   //   HAL_SPI_Transmit(&LCD_SPI, DataBuff + 2 * i * height, height, 1000);

//   // // �Ļ�8λ���ݿ�ȣ���Ϊָ��Ͳ������ݶ��ǰ���8λ�����
//   // LCD_SPI.Init.DataSize = SPI_DATASIZE_8BIT; //	8λ���ݿ��
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

// // // �޸�Ϊ16λ���ݿ�ȣ�д�����ݸ���Ч�ʣ�����Ҫ���
// //  LCD_SPI.Init.DataSize 	= SPI_DATASIZE_16BIT;   //	16λ���ݿ��
// // //  SPI4_Handler.Init.FirstBit = SPI_FIRSTBIT_LSB;
// //  HAL_SPI_Init(&LCD_SPI);

// // LCD_SPI_TransmitBuffer(&LCD_SPI, DataBuff,width * height) ;

// // // �Ļ�8λ���ݿ�ȣ���Ϊָ��Ͳ������ݶ��ǰ���8λ�����
// // LCD_SPI.Init.DataSize 	= SPI_DATASIZE_8BIT;    //	8λ���ݿ��
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
//  * @brief  רΪ��Ļ�������޸ģ�����Ҫ��������ɫ��������
//  * @param  hspi   : spi�ľ��
//  * @param  pData  : Ҫд�������
//  * @param  Size   : ���ݴ�С
//  * @retval HAL status
//  */

// HAL_StatusTypeDef LCD_SPI_Transmit(SPI_HandleTypeDef *hspi, uint16_t pData, uint32_t Size)
// {
//   uint32_t tickstart;
//   uint32_t Timeout = 1000;  // ��ʱ�ж�
//   uint32_t LCD_pData_32bit; // ��32λ����ʱ������
//   uint32_t LCD_TxDataCount; // �������
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

//   LCD_TxDataCount = Size;                  // ��������ݳ���
//   LCD_pData_32bit = (pData << 16) | pData; // ��32λ����ʱ���ϲ�2�����ص����ɫ

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

//   // ��ʹ��Ӳ�� TSIZE ���ƣ��˴�����Ϊ0���������ƴ�������ݳ���
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

//   SET_BIT((hspi)->Instance->CR1, SPI_CR1_CSUSP); // �������SPI����
//   /* �ȴ�SPI���� */
//   if (LCD_SPI_WaitOnFlagUntilTimeout(hspi, SPI_FLAG_SUSP, RESET, tickstart, Timeout) != HAL_OK)
//   {
//     SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_FLAG);
//   }
//   LCD_SPI_CloseTransfer(hspi); /* Call standard close procedure with error check */

//   SET_BIT((hspi)->Instance->IFCR, SPI_IFCR_SUSPC); // ��������־λ

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
//  * @brief  רΪ����д�������޸ģ�ʹ֮���޳��ȵĴ�������
//  * @param  hspi   : spi�ľ��
//  * @param  pData  : Ҫд�������
//  * @param  Size   : ���ݴ�С
//  * @retval HAL status
//  */
// HAL_StatusTypeDef LCD_SPI_TransmitBuffer(SPI_HandleTypeDef *hspi, uint16_t *pData, uint32_t Size)
// {
//   uint32_t tickstart;
//   uint32_t Timeout = 1000;  // ��ʱ�ж�
//   uint32_t LCD_TxDataCount; // �������
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

//   LCD_TxDataCount = Size; // ��������ݳ���

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

//   // ��ʹ��Ӳ�� TSIZE ���ƣ��˴�����Ϊ0���������ƴ�������ݳ���
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

//   SET_BIT((hspi)->Instance->CR1, SPI_CR1_CSUSP); // �������SPI����
//   /* �ȴ�SPI���� */
//   if (LCD_SPI_WaitOnFlagUntilTimeout(hspi, SPI_FLAG_SUSP, RESET, tickstart, Timeout) != HAL_OK)
//   {
//     SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_FLAG);
//   }
//   LCD_SPI_CloseTransfer(hspi); /* Call standard close procedure with error check */

//   SET_BIT((hspi)->Instance->IFCR, SPI_IFCR_SUSPC); // ��������־λ

//   /* Process Unlocked */
//   __HAL_UNLOCK(hspi);

//   hspi->State = HAL_SPI_STATE_READY;

//   if (hspi->ErrorCode != HAL_SPI_ERROR_NONE)
//   {
//     return HAL_ERROR;
//   }
//   return errorcode;
// }