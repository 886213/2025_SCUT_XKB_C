#ifndef IMAGE_H_
#define IMAGE_H_

#include "main.h"

extern uint8_t img_thread; // 二值化阈值

#define WIDTH LCD_W  // 图像宽度
#define HEIGHT LCD_H // 图像高度
#define IMG_WHITE 255
#define IMG_BLACK 0
#define A4Target_DotOfferset 2

void Image_process(void);

#endif /* SWJ_H_ */
