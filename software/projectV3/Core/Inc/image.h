#ifndef IMAGE_H_
#define IMAGE_H_

#include "main.h"

extern uint8_t img_thread; // ��ֵ����ֵ

#define WIDTH LCD_W  // ͼ����
#define HEIGHT LCD_H // ͼ��߶�
#define IMG_WHITE 255
#define IMG_BLACK 0
#define A4Target_DotOfferset 2

void Image_process(void);

#endif /* SWJ_H_ */
