#include "Filter.h"
/*
A、名称：一阶滞后滤波法
B、方法：
    取a=0-1，本次滤波结果=(1-a)*本次采样值+a*上次滤波结果。
C、优点：
    对周期性干扰具有良好的抑制作用；
    适用于波动频率较高的场合。
D、缺点：
    相位滞后，灵敏度低；
    滞后程度取决于a值大小；
    不能消除滤波频率高于采样频率1/2的干扰信号。
*/
void FO_Filter_Init(FO_Filter* state, float a1, float a2, float a3)
{
	state->a_float = a1;
	state->a_uint16 = a2;
	state->a_int16 = a3;
	state->final_float = 0.0f;
	state->final_uint16 = 0;
	state->final_int16 = 0;
}	

float FO_Filter_float(FO_Filter* state, float in_data)
{
	state->final_float = state->a_float * in_data + (1 - state->a_float) * state->final_float;
	return(state->final_float);
}

uint16_t FO_Filter_uint16(FO_Filter* state, uint16_t in_data)
{
	state->final_uint16 = state->a_uint16 * in_data + (1 - state->a_uint16) * state->final_uint16;
	return(state->final_uint16);
}

int16_t FO_Filter_int16(FO_Filter* state, int16_t in_data)
{
	state->final_int16 = state->a_int16 * in_data + (1 - state->a_int16) * state->final_int16;
	return(state->final_int16);
}

/*
A、名称：中位值平均滤波法（又称防脉冲干扰平均滤波法）
B、方法：
    采一组队列去掉最大值和最小值后取平均值，
    相当于“中位值滤波法”+“算术平均滤波法”。
    连续采样N个数据，去掉一个最大值和一个最小值，
    然后计算N-2个数据的算术平均值。
    N值的选取：3-14。
C、优点：
    融合了“中位值滤波法”+“算术平均滤波法”两种滤波法的优点。
    对于偶然出现的脉冲性干扰，可消除由其所引起的采样值偏差。
    对周期干扰有良好的抑制作用。
    平滑度高，适于高频振荡的系统。
D、缺点：
    计算速度较慢，和算术平均滤波法一样。
    比较浪费RAM。
*/
float mid_Filter_float_data[10];
float mid_Filter_float(float in_data)
{
	float sum = 0;
	float temp[10];
	float change;
	int i,j;
	//记录数据
	for(i=0; i<9; i++)
	{
		mid_Filter_float_data[i]=mid_Filter_float_data[i+1];
	}
	mid_Filter_float_data[9] = in_data;
	//复制数据
	for(i=0; i<10; i++)
		temp[i] = mid_Filter_float_data[i];
	//冒泡法排序
	for(i=1; i<10; i++)
		for(j=0; j<10-i; j++)
		{
			if(temp[j] > temp[j+1])
			{
				change = temp[j];
				temp[j] = temp[j+1];
				temp[j+1] = change;
			}
		}
	//求和
	for(i=1; i<9; i++)
		sum = sum + temp[i];
	//返回平均值
	return(sum/8);
 
}

uint16_t mid_Filter_uint16_data[pool_len];
uint16_t mid_Filter_uint16(uint16_t in_data)
{
	uint16_t sum = 0;
	uint16_t temp[pool_len];
	int i;
	//记录数据
	for(i=0; i<(pool_len-1); i++)
	{
		mid_Filter_uint16_data[i]=mid_Filter_uint16_data[i+1];
	}
	mid_Filter_uint16_data[pool_len-1] = in_data;
	//复制数据
	for(i=0; i<pool_len; i++)
		temp[i] = mid_Filter_uint16_data[i];
	//冒泡法排序
//	for(i=1; i<pool_len; i++)
//		for(j=0; j<pool_len-i; j++)
//		{
//			if(temp[j] > temp[j+1])
//			{
//				change = temp[j];
//				temp[j] = temp[j+1];
//				temp[j+1] = change;
//			}
//		}
	//求和
//	for(i=1; i<(pool_len-1); i++)
//		sum = sum + temp[i];
	for(i=0; i<pool_len; i++)
	    sum = sum + temp[i];
	//返回平均值
//	return(sum/(pool_len-2));
	return(sum/(pool_len));
 
}

uint16_t mid_Filter2_uint16_data[pool_len2];
uint16_t mid_Filter2_uint16(uint16_t in_data)
{
    uint16_t sum = 0;
    uint16_t temp[pool_len2];
    uint16_t change;
    int i,j;
    //记录数据
    for(i=0; i<(pool_len2-1); i++)
    {
        mid_Filter_uint16_data[i]=mid_Filter_uint16_data[i+1];
    }
    mid_Filter_uint16_data[pool_len2-1] = in_data;
    //复制数据
    for(i=0; i<pool_len2; i++)
        temp[i] = mid_Filter_uint16_data[i];
    //冒泡法排序
  for(i=1; i<pool_len2; i++)
      for(j=0; j<pool_len2-i; j++)
      {
          if(temp[j] > temp[j+1])
          {
              change = temp[j];
              temp[j] = temp[j+1];
              temp[j+1] = change;
          }
      }
//    求和
  for(i=1; i<(pool_len2-1); i++)
      sum = sum + temp[i];
//    返回平均值
  return(sum/(pool_len2-2));
}

/*
A、名称：中位值滤波法
B、方法：
    连续采样N次（N取奇数），把N次采样值按大小排列，
    取中间值为本次有效值。
C、优点：
    能有效克服因偶然因素引起的波动干扰；
    对温度、液位的变化缓慢的被测参数有良好的滤波效果。
D、缺点：
    对流量、速度等快速变化的参数不宜。
*/
 
float middleFilter_float_data[11];
float middleFilter_float(float in_data)
{
	float temp[11];
	float change;
	int i,j;
	//记录数据
	for(i=0; i<10; i++)
	{
		middleFilter_float_data[i]=middleFilter_float_data[i+1];
	}
	middleFilter_float_data[10] = in_data;
	//复制数据
	for(i=0; i<10; i++)
		temp[i] = middleFilter_float_data[i];
	//冒泡法排序
	for(i=1; i<10; i++)
		for(j=0; j<10-i; j++)
		{
			if(temp[j] > temp[j+1])
			{
				change = temp[j];
				temp[j] = temp[j+1];
				temp[j+1] = change;
			}
		}
	return(temp[5]);
}

uint16_t middleFilter_uint16_data[11];
uint16_t middleFilter_uint16(uint16_t in_data)
{
	uint16_t end;
	uint16_t temp[11];
	uint16_t change;
	uint8_t i,j;
	//记录数据
	for(i=0; i<10; i++)
	{
		middleFilter_uint16_data[i]=middleFilter_uint16_data[i+1];
	}
	middleFilter_uint16_data[10] = in_data;
	//复制数据
	for(i=0; i<10; i++)
		temp[i] = middleFilter_uint16_data[i];
	//冒泡法排序
	for(i=1; i<10; i++)
		for(j=0; j<10-i; j++)
		{
			if(temp[j] > temp[j+1])
			{
				change = temp[j];
				temp[j] = temp[j+1];
				temp[j+1] = change;
			}
		}
		end = temp[5];
	return(end);
}
