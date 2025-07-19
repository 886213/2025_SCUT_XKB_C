#ifndef SWJ_H_
#define SWJ_H_
#endif /* SWJ_H_ */
#include "main.h"
#include "SPI.h"
#include "usart.h"

// #define  sw_write_buffer(dat,len)  wifi_spi_send_buffer(dat,len)//���ߴ��ڷ�������򴮿ڷ�������
#define  sw_write_buffer(dat, len)  HAL_UART_Transmit(&huart3, dat, len, 1000)

//��ɫ color���� [ͼ���߽�������ǵ���]
#define swj_BLACK 0
#define swj_WHITE 1
#define swj_RED 2
#define swj_GREEN 3
#define swj_BLUE 4
#define swj_PURPLE 5
#define swj_YELLOW 6
#define swj_CYAN 7
#define swj_ORANGE 8

//��־λtype
#define swj_point_type1 1 //Сʮ�� 3x3
#define swj_point_type2 2 //��ʮ�� 5x5
#define swj_point_type3 3 //��ʮ�� 7x7
#define swj_point_type4 4 //С  X  3x3
#define swj_point_type5 5 //��  X  5x5
#define swj_point_type6 6 //��  X  7x7
#define swj_point_type7 7 //ȫ��ʮ��
#define swj_point_type8 8 //����һ��ֱ�� y������ x��������
#define swj_point_type9 9 //����һ��ֱ�� x������ y��������

void swj_init(void); //��ʼ������

/*[  ��ͼ��    ]*/
//�Ҷ�
void sendimg(uint8_t* image, uint8_t width, uint8_t height);
//��У��ĻҶ�
void sendimg_A( uint8_t* image, uint8_t width, uint8_t height);
//�ߴ�ѹ���Ҷ�
void sendimg_zoom(uint8_t* image, uint8_t width, uint8_t height, uint8_t dis_width, uint8_t dis_height);
//��ֵ��ͼ�� (��ɫ ������ʵ��ʵʱͼ�� �ȻҶȿ�40��)
void sendimg_binary( uint8_t* image, uint8_t width, uint8_t height,uint8_t otu);
//�ߴ�ѹ����ֵ��ͼ��
void sendimg_binary_zoom( uint8_t* image, uint8_t width, uint8_t height, uint8_t dis_width, uint8_t dis_height,uint8_t otu);
//��У���ѹ��ͼ��(*�Ƽ�)
void sendimg_binary_CHK(uint8_t* image, uint8_t width, uint8_t height,uint8_t otu,uint8_t chk);
//��jpeg��ɫͼ��
void sendimg_JPEG(uint8_t* image,int lon);

/*[  ���߽�    ]*/
//���� ͼ���߽�ǰҪ���� ��Ȼ���������
void sendline_clear( uint8_t color,uint8_t width, uint8_t height);
//�������͵ı߽�ͼ�� ����int���鴢��߽���Ϣ
void sendline_type(uint8_t color,uint8_t *buff, uint32_t len,uint8_t size_lon);
//���߽� uint8_t�������͵� ÿһλ��Ӧһ�еı߽��
void sendline( uint8_t color,uint8_t *buff, uint32_t len);
//��˳��߽� ������������
void sendline2( uint8_t color,uint8_t *linex,uint8_t *liney, uint32_t len);
//�����ұ��� �Ƽ��������
void sendline_xy( uint8_t *line_zx,uint8_t *line_yx, uint32_t len);
//���������ǵ� ����յ�
void sendpoint(uint8_t color,uint8_t x,uint8_t y, uint8_t type);

/*[  ���߽�+ͼ��    ]*/
//˵��: �����ô��߽�+ͼ��    ��ͼ���봫�߽� ���ͼ��ֿ��������ͼ�� �ұ��Ǳ߽� ���Ե��ں� �߽���ں�ͼ����
//�ŵ��ǿ��Կ�ԭʼ��ֵ��ͼ�� �� ���д������
//������Ϊ���ʱ���ȶ��� ���ٶȺܿ��ʱ�� �ᷢ�ֱ߽���ͼ���Ӧ������
//�����ٶȿ��� ���߽�+ͼ�� �߽���Ȼ����ڻ����� ͼ�����ѱ߽�ֱ����ʾͼ���� ��������(ͼ��Ḳ��)

//�߽� �÷�������һ��
void sendimgAndLine( uint8_t color,uint8_t *buff, uint32_t len);
void sendimgAndLine_type(uint8_t color,uint8_t *buff, uint32_t len,uint8_t size_lon);
void sendimgAndLine_point(uint8_t color,uint8_t x,uint8_t y, uint8_t type);
//ͼ��
void sendimg_BinaryAndLine(uint8_t* image, uint8_t width, uint8_t height,uint8_t otu);

//����������ֵ��ͼ�����wifi�Ķ�ֵ��ͼ�� ���ݰ����ȹ̶� �ܶ�wifi������ģ����Ҫ���չ�1024 2048�ȵȹ̶����ݳ��Ⱥ�ſ�ʼת��
//����Ķ�ֵ��ͼ��ѹ���ʺܸ� �����ݰ����Ȳ��� ÿ��ͬѧ200-800�ֽ� Ҳ�����wifi��Щģ�� һ�����ݰ����溬�кü�֡��ֵ��ͼ��
//֡�ʸߵ���ʾЧ����ʵʱ�Բ��� ������������ר�����wifi�Ķ�ֵ��ͼ��
//�����У�� ���粻�ѻ�����ֻ�����ʱ����
void sendimg_7binaryTobyteANDline_CHK(uint8_t* image, uint8_t width, uint8_t height,uint8_t otu);
void sendimg_8binaryTobyteANDline(uint8_t* image, uint8_t width, uint8_t height,uint8_t otu);



/*[  ���ݱ�ǩ    ]*/
//*���ݱ�ǩ  ¼�ƻط� ��bug���ջ�������־λ ����Ҫ
//nameͨ�� dat���� ����ʾ���Ҳ��ǩ�� ���ʾ���� �Ҽ��Ǳ��̰� ��ɫ�궨
void put_int32(uint8_t name, int dat);
void put_float(uint8_t name, float dat);


//������Щѡ�ֻ����ʹ��vofa+ �������������λ��
//������ܾ�������ת����������� ʵ�ֹ�ͬЭ��ʹ�õĹ���
//ֹͣ��λ���������� ��������ת��*
//*��Ҫ����λ�� ͼ���� ���� ������ת �����������
void swj_stop();
//������λ���������� �ر�����ת��
void swj_start();

//�ڲ�����
uint16_t swj_CRC(uint8_t *Buf,uint8_t CRC_sta, uint8_t CRC_CNT);
//int32 ByteToInt(int8 *ndsk);
//float ByteToFloat(unsigned char* byteArry);
#define sendline_uint8_t(color,buff,len) sendline_type(color,buff,len,1)
#define sendline_int(color,buff,len) sendline_type(color,buff,len,4)
#define sendline_uint16_t(color,buff,len) sendline_type(color,buff,len,2)
