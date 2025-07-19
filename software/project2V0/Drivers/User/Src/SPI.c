#include "spi.h"

SPI_HandleTypeDef SPI4_Handler; // SPI4���
SPI_HandleTypeDef SPI2_Handler; // SPI4���

// ������SPIģ��ĳ�ʼ�����룬���ó�����ģʽ
// SPI�ڳ�ʼ��
// �������Ƕ�SPI2�ĳ�ʼ��
void SPI4_Init(void)
{
  SPI4_Handler.Instance = SPI4;                                          // SPI4
  SPI4_Handler.Init.Mode = SPI_MODE_MASTER;                              // ����SPI����ģʽ������Ϊ��ģʽ
  SPI4_Handler.Init.Direction = SPI_DIRECTION_2LINES;                    // ����SPI�������˫�������ģʽ:SPI����Ϊ˫��ģʽ
  SPI4_Handler.Init.DataSize = SPI_DATASIZE_8BIT;                        // ����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
  SPI4_Handler.Init.CLKPolarity = SPI_POLARITY_HIGH;                     // ����ͬ��ʱ�ӵĿ���״̬Ϊ�͵�ƽ
  SPI4_Handler.Init.CLKPhase = SPI_PHASE_2EDGE;                          // ����ͬ��ʱ�ӵĵ�һ�������أ��������½������ݱ�����
  SPI4_Handler.Init.NSS = SPI_NSS_SOFT;                                  // NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
  SPI4_Handler.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;                    // NSS�ź�����ʧ��
  SPI4_Handler.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_ENABLE; // SPI��ģʽIO״̬����ʹ��
  SPI4_Handler.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;         // ���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ4
  SPI4_Handler.Init.FirstBit = SPI_FIRSTBIT_MSB;                         // ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
  SPI4_Handler.Init.TIMode = SPI_TIMODE_DISABLE;                         // �ر�TIģʽ
  SPI4_Handler.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;         // �ر�Ӳ��CRCУ��
  SPI4_Handler.Init.CRCPolynomial = 7;                                   // CRCֵ����Ķ���ʽ
  HAL_SPI_Init(&SPI4_Handler);

  __HAL_SPI_ENABLE(&SPI4_Handler); // ʹ��SPI4
  SPI4_ReadWriteByte(0Xff);        // ��������
}

void SPI2_Init(void)
{
  SPI2_Handler.Instance = SPI2;                                          // SPI2
  SPI2_Handler.Init.Mode = SPI_MODE_MASTER;                              // ����SPI����ģʽ������Ϊ��ģʽ
  SPI2_Handler.Init.Direction = SPI_DIRECTION_2LINES;                    // ����SPI�������˫�������ģʽ:SPI����Ϊ˫��ģʽ
  SPI2_Handler.Init.DataSize = SPI_DATASIZE_8BIT;                        // ����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
  SPI2_Handler.Init.CLKPolarity = SPI_POLARITY_HIGH;                     // ����ͬ��ʱ�ӵĿ���״̬Ϊ�͵�ƽ
  SPI2_Handler.Init.CLKPhase = SPI_PHASE_2EDGE;                          // ����ͬ��ʱ�ӵĵ�һ�������أ��������½������ݱ�����
  SPI2_Handler.Init.NSS = SPI_NSS_SOFT;                                  // NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
  SPI2_Handler.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;                    // NSS�ź�����ʧ��
  SPI2_Handler.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_ENABLE; // SPI��ģʽIO״̬����ʹ��
  SPI2_Handler.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;        // ���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ4
  SPI2_Handler.Init.FirstBit = SPI_FIRSTBIT_LSB;                         // ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
  SPI2_Handler.Init.TIMode = SPI_TIMODE_DISABLE;                         // �ر�TIģʽ
  SPI2_Handler.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;         // �ر�Ӳ��CRCУ��
  SPI2_Handler.Init.CRCPolynomial = 7;                                   // CRCֵ����Ķ���ʽ
  HAL_SPI_Init(&SPI2_Handler);

  __HAL_SPI_ENABLE(&SPI2_Handler); // ʹ��SPI2
}

// SPI2�ײ�������ʱ��ʹ�ܣ���������
// �˺����ᱻHAL_SPI_Init()����
// hspi:SPI���
// void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
// {
//   if (hspi->Instance == SPI4)
//   {
//     GPIO_InitTypeDef GPIO_Initure;

//     __HAL_RCC_GPIOE_CLK_ENABLE(); // ʹ��GPIOFʱ��
//     __HAL_RCC_SPI4_CLK_ENABLE();  // ʹ��SPI2ʱ��

//     GPIO_Initure.Pin = GPIO_PIN_12 | GPIO_PIN_14;   // SCK��MOSI����
//     GPIO_Initure.Mode = GPIO_MODE_AF_PP;            // �����������
//     GPIO_Initure.Pull = GPIO_NOPULL;                // ��������
//     GPIO_Initure.Speed = GPIO_SPEED_FREQ_VERY_HIGH; // ����ٶȵȼ�
//     GPIO_Initure.Alternate = GPIO_AF5_SPI4;         // ���õ�SPI4��������1
//     HAL_GPIO_Init(GPIOE, &GPIO_Initure);

//     GPIO_Initure.Pin = GPIO_PIN_13;                 // MISO����
//     GPIO_Initure.Mode = GPIO_MODE_INPUT;            // �����������
//     GPIO_Initure.Pull = GPIO_NOPULL;                // ��������
//     GPIO_Initure.Speed = GPIO_SPEED_FREQ_VERY_HIGH; // ����ٶȵȼ�
//     GPIO_Initure.Alternate = GPIO_AF5_SPI4;         // ���õ�SPI4��������1
//     HAL_GPIO_Init(GPIOE, &GPIO_Initure);
//   }
//   else if(hspi->Instance == SPI2)
//   {
//     GPIO_InitTypeDef GPIO_Initure;

//     __HAL_RCC_GPIOB_CLK_ENABLE(); // ʹ��GPIOFʱ��
//     __HAL_RCC_SPI2_CLK_ENABLE();  // ʹ��SPI2ʱ��

//     GPIO_Initure.Pin = GPIO_PIN_13 | GPIO_PIN_15;   // SCK��MOSI����
//     GPIO_Initure.Mode = GPIO_MODE_AF_PP;            // �����������
//     GPIO_Initure.Pull = GPIO_NOPULL;                // ��������
//     GPIO_Initure.Speed = GPIO_SPEED_FREQ_VERY_HIGH; // ����ٶȵȼ�
//     GPIO_Initure.Alternate = GPIO_AF5_SPI2;         // ���õ�SPI4��������1
//     HAL_GPIO_Init(GPIOB, &GPIO_Initure);

//     GPIO_Initure.Pin = GPIO_PIN_14;                 // MISO����
//     GPIO_Initure.Mode = GPIO_MODE_INPUT;            // �����������
//     GPIO_Initure.Pull = GPIO_NOPULL;                // ��������
//     GPIO_Initure.Speed = GPIO_SPEED_FREQ_VERY_HIGH; // ����ٶȵȼ�
//     GPIO_Initure.Alternate = GPIO_AF5_SPI2;         // ���õ�SPI4��������1
//     HAL_GPIO_Init(GPIOB, &GPIO_Initure);
//   }
// }

// SPI�ٶ����ú���
// SPI�ٶ�=PLL1Q/��Ƶϵ��
//@ref SPI_BaudRate_Prescaler:SPI_BAUDRATEPRESCALER_2~SPI_BAUDRATEPRESCALER_256
// PLL1Qʱ��һ��Ϊ200Mhz��
void SPI4_SetSpeed(u32 SPI_BaudRatePrescaler)
{
  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler)); // �ж���Ч��
  __HAL_SPI_DISABLE(&SPI4_Handler);                               // �ر�SPI
  SPI4_Handler.Instance->CFG1 &= ~(0X7 << 28);                    // λ30-28���㣬�������ò�����
  SPI4_Handler.Instance->CFG1 |= SPI_BaudRatePrescaler;           // ����SPI�ٶ�
  __HAL_SPI_ENABLE(&SPI4_Handler);                                // ʹ��SPI
}

// SPI2 ��дһ���ֽ�
// TxData:Ҫд����ֽ�
// ����ֵ:��ȡ�����ֽ�
u8 SPI4_ReadWriteByte(u8 TxData)
{
  u8 Rxdata;
  HAL_SPI_TransmitReceive(&SPI4_Handler, &TxData, &Rxdata, 1, 1000);
  return Rxdata; // �����յ�������
}
