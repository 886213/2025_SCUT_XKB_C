#ifndef __FILTER_H
#define __FILTER_H           

#include "main.h"

#define uint16_t unsigned short int
#define pool_len Scan_Num
#define pool_len2 Scan_Num

typedef struct {
	float a_float;
	float a_uint16;
	float a_int16;
	
	float final_float;
	uint16_t final_uint16;
	int16_t final_int16;
}FO_Filter;

void FO_Filter_Init(FO_Filter* state, float a1, float a2, float a3);
float FO_Filter_float(FO_Filter* state, float in_data);
uint16_t FO_Filter_uint16(FO_Filter* state, uint16_t in_data);
int16_t FO_Filter_int16(FO_Filter* state, int16_t in_data);

float mid_Filter_float(float in_data);
uint16_t mid_Filter_uint16(uint16_t in_data);
uint16_t mid_Filter2_uint16(uint16_t in_data);

float middleFilter_float(float in_data);
uint16_t middleFilter_uint16(uint16_t in_data);

#endif
