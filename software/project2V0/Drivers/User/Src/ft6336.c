// //////////////////////////////////////////////////////////////////////////////////
// // ������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
// // ����Ӳ������Ƭ��STM32F103C8T6,F103C8T6���Ŀ�����,��Ƶ72MHZ������8MHZ
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
// // ��������:2018/08/09
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
// #include "ft6336.h"
// #include "touch.h"
// #include "ctpiic.h"
// #include "delay.h"
// #include "string.h"
// #include "lcd.h"

// extern u8 touch_flag;

// I2C_HandleTypeDef hi2c2; // �޸�ΪI2C2���

// // GPIO��ʼ��������I2C2���ţ�
// void CPT_GPIO_Init(void)
// {
//   GPIO_InitTypeDef GPIO_InitStruct = {0};

//   // ʹ��GPIOB��I2C2ʱ��
//   __HAL_RCC_GPIOB_CLK_ENABLE();
//   __HAL_RCC_GPIOD_CLK_ENABLE();
//   __HAL_RCC_I2C2_CLK_ENABLE(); // �޸�ΪI2C2ʱ��

//   // PB10 - I2C2_SCL, PB11 - I2C2_SDA������ʵ��Ӳ������ѡ�����ţ�
//   GPIO_InitStruct.Pin = GPIO_PIN_10 | GPIO_PIN_11; 
//   GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
//   GPIO_InitStruct.Pull = GPIO_PULLUP;
//   GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
//   GPIO_InitStruct.Alternate = GPIO_AF4_I2C2; // ���ù��ܸ�ΪAF4_I2C2
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
//   hi2c2.Init.Timing = 0x00707CBB; // ��׼ģʽ100kHz�������ʵ��ϵͳʱ�ӵ�����
//   hi2c2.Init.OwnAddress1 = 0;
//   hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
//   hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
//   hi2c2.Init.OwnAddress2 = 0;
//   hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
//   hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
//   hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
//   HAL_I2C_Init(&hi2c2);
// }

// // ��ȡFT6336�Ĵ���ֵ���޸�Ϊhi2c2��
// HAL_StatusTypeDef FT6336_ReadReg(uint8_t reg, uint8_t *data, uint8_t len)
// {
//   return HAL_I2C_Mem_Read(&hi2c2, FT6336_I2C_ADDR << 1, reg, I2C_MEMADD_SIZE_8BIT, data, len, 100);
// }

// // д��FT6336�Ĵ������޸�Ϊhi2c2��
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

//   CTP_I2C2_Init();// ��ʼ����������I2C����

//   FT_RST(0);      // ��λ
//   delay_ms(10);
//   FT_RST(1); // �ͷŸ�λ
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
//   static u8 t = 0; // ���Ʋ�ѯ���,�Ӷ�����CPUռ����
//   t++;
//   if ((t % 10) == 0 || t < 10) // ����ʱ,ÿ����10��CTP_Scan�����ż��1��,�Ӷ���ʡCPUʹ����
//   {
//     FT6336_ReadReg(FT_REG_NUM_FINGER, &mode, 1); // ��ȡ�������״̬
//     if (mode && (mode < 3))
//     {
//       temp = 0XFF << mode; // ����ĸ���ת��Ϊ1��λ��,ƥ��tp_dev.sta����
//       tp_dev.sta = (~temp) | TP_PRES_DOWN | TP_CATH_PRES;
//       for (i = 0; i < CTP_MAX_TOUCH; i++)
//       {
//         FT6336_ReadReg(FT6336_TPX_TBL[i], buf, 4); // ��ȡXY����ֵ
//         if (tp_dev.sta & (1 << i))                // ������Ч?
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
//           // if((buf[0]&0XF0)!=0X80)tp_dev.x[i]=tp_dev.y[i]=0;//������contact�¼�������Ϊ��Ч
//           // printf("x[%d]:%d,y[%d]:%d\r\n",i,tp_dev.x[i],i,tp_dev.y[i]);
//         }
//       }
//       res = 1;
//       if (tp_dev.x[0] == 0 && tp_dev.y[0] == 0)
//         mode = 0; // ���������ݶ���0,����Դ˴�����
//       t = 0;      // ����һ��,��������������10��,�Ӷ����������
//     }
//   }
//   if (mode == 0) // �޴����㰴��
//   {
//     if (tp_dev.sta & TP_PRES_DOWN) // ֮ǰ�Ǳ����µ�
//     {
//       tp_dev.sta &= ~(1 << 7); // ��ǰ����ɿ�
//     }
//     else // ֮ǰ��û�б�����
//     {
//       tp_dev.x[0] = 0xffff;
//       tp_dev.y[0] = 0xffff;
//       tp_dev.sta &= 0XE0; // �������Ч���
//     }
//   }
//   if (t > 240)
//     t = 10; // ���´�10��ʼ����
//   return res;
// }
