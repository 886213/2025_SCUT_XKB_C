#include "image.h"
#include <stdio.h>
#include <math.h>

uint8_t img_thread = 50; // ��ֵ����ֵ

#define M_PI 3.141562f;

typedef struct
{
  float x;
  float y;
} Point;

float calculate_angle(RedDot_LCD p, RedDot_LCD a, RedDot_LCD b)
{

  Point P, A, B;
  P.x = (float)p.x;
  P.y = (float)p.y;
  A.x = (float)a.x;
  A.y = (float)a.y;
  B.x = (float)b.x;
  B.y = (float)b.y;

  // ��������PA��PB
  Point PA = {A.x - P.x, A.y - P.y};
  Point PB = {B.x - P.x, B.y - P.y};

  // ������
  float dot_product = PA.x * PB.x + PA.y * PB.y;

  // ��������ģ��
  float mod_PA = sqrt(PA.x * PA.x + PA.y * PA.y);
  float mod_PB = sqrt(PB.x * PB.x + PB.y * PB.y);

  // ���ģ���Ƿ�Ϊ�㣨��������㣩
  if (mod_PA == 0 || mod_PB == 0)
  {
    // printf("���󣺹�������ĳһ���غϣ��޷�����Ƕ�\n");
    return -1.0;
  }

  // ��������ֵ��������[-1, 1]��Χ�ڣ���ֹ������
  float cos_theta = dot_product / (mod_PA * mod_PB);
  cos_theta = fmax(-1.0, fmin(1.0, cos_theta));

  // ���㻡�Ȳ�ת��Ϊ�Ƕ�
  float theta_rad = acos(cos_theta);
  float theta_deg = theta_rad * 180.0 / M_PI;

  return theta_deg;
}

void rgb565_to_grayscale(void)
{
  for (uint32_t i = 0; i < Display_Width * Display_Height; i++)
  {
    uint16_t pixel = *(uint16_t *)(Camera_Buffer + 2 * i);
    uint8_t *p = (uint8_t *)(Camera_Buffer + i);

    uint8_t r5 = (pixel >> 11) & 0x1F; // ��ȡ��ɫ5λ
    uint8_t g6 = (pixel >> 5) & 0x3F;  // ��ȡ��ɫ6λ
    uint8_t b5 = pixel & 0x1F;         // ��ȡ��ɫ5λ

    uint8_t r8 = (r5 << 3) | (r5 >> 2); // 5λ��8λ
    uint8_t g8 = (g6 << 2) | (g6 >> 4); // 6λ��8λ
    uint8_t b8 = (b5 << 3) | (b5 >> 2); // 5λ��8λ

    uint8_t gray = (r8 * 77 + g8 * 150 + b8 * 29) >> 8; // ���ٽ���
    *p = gray;

    // HAL_UART_Transmit(&huart3, p, 1, 1000); // ����ͼ������
  }
}

void rgb565_to_Binary(void)
{
  for (uint16_t i = 0; i < Display_Height; i++)
  {
    for (uint16_t j = 0; j < Display_Width; j++)
    {
      uint16_t pixel = *(uint16_t *)(Camera_Buffer + Display_Width * i * 2 + 2 * j);
      uint8_t *p = (uint8_t *)(Camera_Buffer + Display_Width * i + j);

      uint8_t r5 = (pixel >> 11) & 0x1F; // ��ȡ��ɫ5λ
      uint8_t g6 = (pixel >> 5) & 0x3F;  // ��ȡ��ɫ6λ
      uint8_t b5 = pixel & 0x1F;         // ��ȡ��ɫ5λ

      uint8_t r8 = (r5 << 3) | (r5 >> 2); // 5λ��8λ
      uint8_t g8 = (g6 << 2) | (g6 >> 4); // 6λ��8λ
      uint8_t b8 = (b5 << 3) | (b5 >> 2); // 5λ��8λ

      uint8_t gray = (r8 * 77 + g8 * 150 + b8 * 29) >> 8; // ���ٽ���

      if (gray <= img_thread && i > Screen_LeftBottom.y + 1 && i < Screen_LeftTop.y - 1 && j > Screen_RightBottom.x + 1 && j < Screen_LeftBottom.x - 1)
      {
        *p = IMG_BLACK;
      }
      else
      {
        *p = IMG_WHITE;
      }
      // HAL_UART_Transmit(&huart3, p, 1, 1000); // ����ͼ������
    }
  }
}

// ������߿�߽�
uint8_t findOuterFrame(void)
{
  RedDot_LCD Dot[4];

  // ���ұ�������ɨ��
  for (uint16_t i = Screen_RightBottom.x; i < Screen_LeftBottom.x; i++)
  {
    for (uint16_t j = Screen_RightBottom.y; j < Screen_RightTop.y; j++)
    {
      uint8_t *p = (uint8_t *)(Camera_Buffer + i + WIDTH * j);
      if (*p == IMG_BLACK && (*(p + 1) == IMG_BLACK || *(p + WIDTH) == IMG_BLACK))
      {
        Dot[0].x = i;
        Dot[0].y = j;
        goto found2;
      }
    }
  }
  printf("Error1");
  return 0;

found2:
  // ���ϱ�������ɨ��
  for (uint16_t j = Screen_LeftBottom.x; j > Screen_RightBottom.x; j--)
  {
    for (uint16_t i = Screen_RightBottom.y; i < Screen_RightTop.y; i++)
    {
      uint8_t *p = (uint8_t *)(Camera_Buffer + i + WIDTH * j);
      if (*p == IMG_BLACK && (*(p + 1) == IMG_BLACK || *(p - WIDTH) == IMG_BLACK))
      {
        Dot[1].x = i;
        Dot[1].y = j;
        goto found3;
      }
    }
  }
  printf("Error2");
  return 0;

found3:
  // �����������ɨ��
  for (uint16_t i = Screen_LeftTop.x; i > Screen_RightBottom.x; i--)
  {
    for (uint16_t j = Screen_LeftTop.y; j > Screen_LeftBottom.y; j--)
    {
      uint8_t *p = (uint8_t *)(Camera_Buffer + i + WIDTH * j);
      if (*p == IMG_BLACK && (*(p - 1) == IMG_BLACK || *(p - WIDTH) == IMG_BLACK))
      {
        Dot[2].x = i;
        Dot[2].y = j;
        goto found4;
      }
    }
  }
  printf("Error3");
  return 0;

found4:
  // ���±�������ɨ��
  for (uint16_t j = Screen_LeftBottom.y; j < Screen_LeftTop.y; j++)
  {
    for (uint16_t i = Screen_LeftTop.x; i > Screen_RightBottom.x; i--)
    {
      uint8_t *p = (uint8_t *)(Camera_Buffer + i + WIDTH * j);
      if (*p == IMG_BLACK && (*(p - 1) == IMG_BLACK || *(p + WIDTH) == IMG_BLACK))
      {
        Dot[3].x = i;
        Dot[3].y = j;
        goto detect;
      }
    }
  }
  printf("Error4");
  return 0;

detect:
  A4Target_RightBottom = Dot[0];
  int line1, line2, line3;
  line1 = ((int)Dot[0].x - (int)Dot[1].x) * ((int)Dot[0].x - (int)Dot[1].x) + ((int)Dot[0].y - (int)Dot[1].y) * ((int)Dot[0].y - (int)Dot[1].y);
  line2 = ((int)Dot[0].x - (int)Dot[2].x) * ((int)Dot[0].x - (int)Dot[2].x) + ((int)Dot[0].y - (int)Dot[2].y) * ((int)Dot[0].y - (int)Dot[2].y);
  line3 = ((int)Dot[0].x - (int)Dot[3].x) * ((int)Dot[0].x - (int)Dot[3].x) + ((int)Dot[0].y - (int)Dot[3].y) * ((int)Dot[0].y - (int)Dot[3].y);

  if (line1 > line2 && line1 > line3)
  {
    A4Target_LeftTop = Dot[1];
    A4Target_LeftBottom = Dot[2];
    A4Target_RightTop = Dot[3];
  }
  else if (line2 > line1 && line2 > line3)
  {
    A4Target_LeftTop = Dot[2];
    A4Target_LeftBottom = Dot[3];
    A4Target_RightTop = Dot[1];
  }
  else if (line3 > line2 && line3 > line1)
  {
    A4Target_LeftTop = Dot[3];
    A4Target_LeftBottom = Dot[1];
    A4Target_RightTop = Dot[2];
  }

  float angel1 = calculate_angle(A4Target_RightBottom, A4Target_LeftBottom, A4Target_RightTop);
  float angel2 = calculate_angle(A4Target_LeftTop, A4Target_LeftBottom, A4Target_RightTop);

  if (angel1 < 75 || angel1 > 100 || angel2 < 75 || angel2 > 100)
  {
    printf("Error 5\n");
    printf("%f,%f\n", angel1, angel2);
    RedDot_LCD Temp;
    if(angel2 < 75)
    {
      Temp.x = (A4Target_LeftTop.x + A4Target_RightBottom.x) / 2;
      Temp.y = (A4Target_LeftTop.y + A4Target_RightBottom.y) / 2;
      A4Target_RightTop.x = Temp.x + Temp.x - A4Target_LeftBottom.x;
      A4Target_RightTop.y = Temp.y + Temp.y - A4Target_LeftBottom.y;
      return 1;
    }

    return 0;
  }

  A4Target_RightBottom.x = A4Target_RightBottom.x + A4Target_DotOfferset;
  A4Target_RightBottom.y = A4Target_RightBottom.y + A4Target_DotOfferset;
  A4Target_LeftBottom.x = A4Target_LeftBottom.x - A4Target_DotOfferset;
  A4Target_LeftBottom.y = A4Target_LeftBottom.y + A4Target_DotOfferset;
  A4Target_RightTop.x = A4Target_RightTop.x + A4Target_DotOfferset;
  A4Target_RightTop.y = A4Target_RightTop.y - A4Target_DotOfferset;
  A4Target_LeftTop.x = A4Target_LeftTop.x - A4Target_DotOfferset;
  A4Target_LeftTop.y = A4Target_LeftTop.y - A4Target_DotOfferset;

  // printf("DOT0:x=%d,y=%d\n", Dot[0].x, Dot[0].y);
  // printf("DOT1:x=%d,y=%d\n", Dot[1].x, Dot[1].y);
  // printf("DOT2:x=%d,y=%d\n", Dot[2].x, Dot[2].y);
  // printf("DOT3:x=%d,y=%d\n", Dot[3].x, Dot[3].y);

  return 1;
}

void Image_process(void)
{
  // RGBͼ��ת��Ϊ�Ҷ�ͼ��
  // rgb565_to_grayscale();

  // RGBͼ��ת��Ϊ��ֵ��ͼ��
  rgb565_to_Binary();

  if (findOuterFrame() == 1)
  {
    LCD_DrawLine2(LCD_W - A4Target_RightBottom.x, LCD_H - A4Target_RightBottom.y, LCD_W - A4Target_RightTop.x, LCD_H - A4Target_RightTop.y, 1, RED);
    LCD_DrawLine2(LCD_W - A4Target_RightBottom.x, LCD_H - A4Target_RightBottom.y, LCD_W - A4Target_LeftBottom.x, LCD_H - A4Target_LeftBottom.y, 1, RED);
    LCD_DrawLine2(LCD_W - A4Target_LeftTop.x, LCD_H - A4Target_LeftTop.y, LCD_W - A4Target_RightTop.x, LCD_H - A4Target_RightTop.y, 1, RED);
    LCD_DrawLine2(LCD_W - A4Target_LeftTop.x, LCD_H - A4Target_LeftTop.y, LCD_W - A4Target_LeftBottom.x, LCD_H - A4Target_LeftBottom.y, 1, RED);
  }
}
