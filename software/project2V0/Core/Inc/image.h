#ifndef IMAGE_H_
#define IMAGE_H_

#include "main.h"

extern uint8_t img_thread; // ��ֵ����ֵ

#define WIDTH LCD_Width   // ͼ����
#define HEIGHT LCD_Height // ͼ��߶�
#define IMG_WHITE 255
#define IMG_BLACK 0

void Image_process(void);

#endif /* SWJ_H_ */
