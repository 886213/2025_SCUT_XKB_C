#include "buzzer.h"

/* ȫ�ֱ��� */
TIM_HandleTypeDef htim2;
static uint8_t current_volume = 5;  // Ĭ������10%
static uint32_t current_freq = 800; // Ĭ��Ƶ��1000Hz
static uint8_t buzzer_state = 0;     // ������״̬��0Ϊ�رգ�1Ϊ����

/* ��������ʼ������ */
void Buzzer_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  // GPIOʱ��ʹ�ܼ�����
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_TIM2_CLK_ENABLE();

  GPIO_InitStruct.Pin = GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF1_TIM2; // TIM2_CH1���ù���
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  // ��ʱ����������
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 239; // ��Ƶϵ��240��ʱ��=240MHz/240=1MHz
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 370; // 2700Hz: 1MHz/371 �� 2695Hz
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }

  // PWMͨ������
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 185;                      // ��ʼռ�ձ�50% (370+1)*50% = 185.5 �� 185
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH; // �ߵ�ƽ��Ч
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }

  Buzzer_SetFrequency(current_freq); // ����Ĭ��Ƶ��2700Hz
  Buzzer_Off();              // Ĭ�Ϲرշ�����
}

/* ����Ƶ�ʺ��� */
void Buzzer_SetFrequency(uint32_t freq)
{
  if (freq == 0)
    return; // ��ֹ�������

  // ����ARRֵ������1MHzʱ�ӣ�
  uint32_t arr = (1000000 / freq) - 1;
  __HAL_TIM_SET_AUTORELOAD(&htim2, arr);

  // ����ռ�ձȱ���ԭ�б���
  Buzzer_SetVolume(current_volume);
}

/* ��������������ͨ��ռ�ձȣ� */
void Buzzer_SetVolume(uint8_t volume)
{
  if (volume > 100)
    volume = 100;
  current_volume = volume;

  uint32_t arr = __HAL_TIM_GET_AUTORELOAD(&htim2);
  uint32_t ccr = (arr + 1) * volume / 100;
  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, ccr);
}

/* ���������� */
void Buzzer_On(void)
{
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
  buzzer_state = 1; // ����״̬
}
/* �رշ����� */
void Buzzer_Off(void)
{
  HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_1);
  buzzer_state = 0; // ����״̬
}

void Buzzer_Swich(void)
{
  // �л�������״̬
  if (buzzer_state == 0)
  {
    CMD_RxValue_Read(&BuzzerSwich_CMD, CMD_VAL_ID_NO1, &Buzzer_Volum);
    if (Buzzer_Volum != 0)            // ��ֹ����Ϊ0
      Buzzer_SetVolume(Buzzer_Volum); // ��������
    CMD_RxValue_Read(&BuzzerSwich_CMD, CMD_VAL_ID_NO2, &Buzzer_Frequency);
    if (Buzzer_Frequency != 0)               // ��ֹƵ��Ϊ0
      Buzzer_SetFrequency(Buzzer_Frequency); // ����Ƶ��
    // �����ǰ�ǹر�״̬������������
    Buzzer_On();
  }
  else
  {
    Buzzer_Off();
  }
}
