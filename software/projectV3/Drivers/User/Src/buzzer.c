#include "buzzer.h"

/* 全局变量 */
TIM_HandleTypeDef htim2;
static uint8_t current_volume = 5;  // 默认音量10%
static uint32_t current_freq = 800; // 默认频率1000Hz
static uint8_t buzzer_state = 0;     // 蜂鸣器状态，0为关闭，1为开启

/* 蜂鸣器初始化函数 */
void Buzzer_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  // GPIO时钟使能及配置
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_TIM2_CLK_ENABLE();

  GPIO_InitStruct.Pin = GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF1_TIM2; // TIM2_CH1复用功能
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  // 定时器基础配置
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 239; // 分频系数240，时钟=240MHz/240=1MHz
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 370; // 2700Hz: 1MHz/371 ≈ 2695Hz
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }

  // PWM通道配置
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 185;                      // 初始占空比50% (370+1)*50% = 185.5 → 185
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH; // 高电平有效
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }

  Buzzer_SetFrequency(current_freq); // 设置默认频率2700Hz
  Buzzer_Off();              // 默认关闭蜂鸣器
}

/* 设置频率函数 */
void Buzzer_SetFrequency(uint32_t freq)
{
  if (freq == 0)
    return; // 防止除零错误

  // 计算ARR值（基于1MHz时钟）
  uint32_t arr = (1000000 / freq) - 1;
  __HAL_TIM_SET_AUTORELOAD(&htim2, arr);

  // 更新占空比保持原有比例
  Buzzer_SetVolume(current_volume);
}

/* 设置音量函数（通过占空比） */
void Buzzer_SetVolume(uint8_t volume)
{
  if (volume > 100)
    volume = 100;
  current_volume = volume;

  uint32_t arr = __HAL_TIM_GET_AUTORELOAD(&htim2);
  uint32_t ccr = (arr + 1) * volume / 100;
  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, ccr);
}

/* 开启蜂鸣器 */
void Buzzer_On(void)
{
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
  buzzer_state = 1; // 更新状态
}
/* 关闭蜂鸣器 */
void Buzzer_Off(void)
{
  HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_1);
  buzzer_state = 0; // 更新状态
}

void Buzzer_Swich(void)
{
  // 切换蜂鸣器状态
  if (buzzer_state == 0)
  {
    CMD_RxValue_Read(&BuzzerSwich_CMD, CMD_VAL_ID_NO1, &Buzzer_Volum);
    if (Buzzer_Volum != 0)            // 防止音量为0
      Buzzer_SetVolume(Buzzer_Volum); // 设置音量
    CMD_RxValue_Read(&BuzzerSwich_CMD, CMD_VAL_ID_NO2, &Buzzer_Frequency);
    if (Buzzer_Frequency != 0)               // 防止频率为0
      Buzzer_SetFrequency(Buzzer_Frequency); // 设置频率
    // 如果当前是关闭状态，则开启蜂鸣器
    Buzzer_On();
  }
  else
  {
    Buzzer_Off();
  }
}
