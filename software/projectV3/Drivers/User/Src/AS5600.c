#include "AS5600.h"

float angles[4];

I2C_HandleTypeDef hi2c3;

// I2C3初始化
void AS5600_Init(void)
{
  MX_I2C3_Init();
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOD_CLK_ENABLE();

  // A0/A1 控制线
  GPIO_InitStruct.Pin = HC4052_A0_PIN | HC4052_A1_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(HC4052_A0_PORT, &GPIO_InitStruct);
}

void MX_I2C3_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  // 配置PA8为I2C3_SCL
  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF4_I2C3;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  // 配置PC9为I2C3_SDA
  GPIO_InitStruct.Pin = GPIO_PIN_9;
  GPIO_InitStruct.Alternate = GPIO_AF4_I2C3;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  __HAL_RCC_I2C3_CLK_ENABLE();

  hi2c3.Instance = I2C3;
  hi2c3.Init.Timing = 0x00707CBB; // 400kHz @ 100MHz APB1时钟
  hi2c3.Init.OwnAddress1 = 0;
  hi2c3.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c3.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c3.Init.OwnAddress2 = 0;
  hi2c3.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c3.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c3) != HAL_OK)
  {
    // Error_Handler();
  }

  // 配置时钟源（根据实际情况调整）
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c3, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    // Error_Handler();
  }
}

/* 多路复用器控制函数 ---------------------------------------------------*/
void Select_AS5600(uint8_t dev_num)
{
  // 验证输入参数
  if (dev_num > 3)
    return;

  // 设置地址线A0/A1
  HAL_GPIO_WritePin(HC4052_A0_PORT, HC4052_A0_PIN, (dev_num & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
  HAL_GPIO_WritePin(HC4052_A1_PORT, HC4052_A1_PIN, (dev_num & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET);

  Deselect_AS5600();

  // 等待多路复用器稳定
  HAL_Delay(1);
}

void Deselect_AS5600(void)
{
  // 禁用所有通道
  HAL_GPIO_WritePin(I2C3_SCL_PORT, I2C3_SCL_PIN, GPIO_PIN_SET);
  HAL_GPIO_WritePin(I2C3_SDA_PORT, I2C3_SDA_PIN, GPIO_PIN_SET);
}

/* AS5600 驱动函数 -----------------------------------------------------*/
float Read_AS5600_Angle_Single(uint8_t dev_num)
{
  Select_AS5600(dev_num);
  uint8_t angle_data[2] = {0};
  uint16_t raw_angle = 0;

  // 从寄存器0x0C读取两个字节
  HAL_I2C_Mem_Read(&hi2c3, AS5600_ADDR << 1, AS5600_ANGLE_REG_H, I2C_MEMADD_SIZE_8BIT, angle_data, 2, 1000);

  // 合并角度值（高8位在前，低8位在后，取高12位）
  raw_angle = ((uint16_t)angle_data[0] << 8) | angle_data[1];

  return (roundf((float)raw_angle * (3200.0f / 4096.0f)) * STEPMOTOR_ANGLE_PER_STEP);
}

void Read_AS5600_Angle_All(void)
{
  for (uint8_t i = 0; i < 2; i++)
  {
    angles[i] = Read_AS5600_Angle_Single(i);
  }
}
