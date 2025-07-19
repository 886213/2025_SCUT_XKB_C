#ifndef IMAGE_H_
#define IMAGE_H_

#include "main.h"

extern uint8_t img_thread; // 二值化阈值

#define WIDTH LCD_Width   // 图像宽度
#define HEIGHT LCD_Height // 图像高度
#define IMG_WHITE 255
#define IMG_BLACK 0

void Image_process(void);

#endif /* SWJ_H_ */
