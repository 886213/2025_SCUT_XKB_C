/*
 * GL-Shw���ܳ���λ��  qqȺ:297809802 qq1849057843
*˵�� :spi-wifiģ�鵥�ֽڷ���Ч�ʵͣ������Ƚ����ݻ��浽������ Ȼ�� ����������
*����Ϊ���ù���
*
*ʾ���� :
*void put_int32(uint8_t name, int dat) void put_float(uint8_t name, int dat);
* ����˵��:���Ժ�ͼ��һ���� ��ͼ���߿����� ���ݻ���ʾ����λ���Ҳ���������
*
*
*ͼ��:
*     �Ҷ�
*  imageҪ���͵�ͼ��  widthͼ��Ŀ� heightͼ��ĸ�    ����:sendimg(mt9v03x_image[0],MT9V03X_W,MT9V03X_H);
*  void sendimg(uint8_t* image, uint8_t width, uint8_t height);
*  ����˵��:������粻�ȶ� ����sendimg_A(mt9v03x_image[0],MT9V03X_W,MT9V03X_H);
*      ��ֵ
*   ots�Ƕ�ֵ����ֵ ���image�Ƕ�ֵ��ͼ�����0  chk��У��ֵ Խ��Խ�ȶ� ���55
*  void sendimg_binary_CHK(uint8_t* image, uint8_t width, uint8_t height,uint8_t otu,uint8_t chk);// (�Լ����µĶ�ֵ�� ���� ����������50FPS)
*
*
*�߽�������:(�ᵥ����ʾ���Ҳ�ͼ����� �����ͼ�� �Ҳ��Ǳ߽� ����һ��)
         ����****(һ��Ҫע�⣬ÿ֡��Ҫ����һ�Σ��������ٷ��߽�  ����������������ϴα߽��)******
  void sendline_clear(uint8_t color,uint8_t width, uint8_t height)
*   ��ͳ�ı߽�(һ��һ���߽��)
* void sendline( uint8_t color,uint8_t *buff, uint32_t len)
*   ����ı߽�(������ �Թ�)
* void sendline2( uint8_t color,uint8_t *linex,uint8_t *liney, uint32_t len)
*   �ؼ���:����յ� ��ֹ�еȵ�
* void sendpoint(uint8_t color,uint8_t x,uint8_t y, uint8_t type)
*
*
* ͼ��ͱ߽���һ��:             (������������ ͼ��Ḳ�ǵ��ɵı�)
* ��
* void sendimgAndLine( uint8_t color,uint8_t *buff, uint32_t len)
* ��
* void sendimgAndLine_point(uint8_t color,uint8_t x,uint8_t y, uint8_t type)
* ͼ��
* sendimg_BinaryAndLine
*
*
 */
#include "swj.h"

#define swj_Buff_SIZE 1024 // ��������С
static uint8_t swj_Buffer[swj_Buff_SIZE] = {0};
int swj_Buffer_lon = 0;

#define Line_SIZE (300) // �߽绺������С
static uint8_t Line_Buffer[Line_SIZE] = {0};
int Line_lon = 0;
void swj_Buffer_put()
{
  sw_write_buffer(swj_Buffer, swj_Buffer_lon);
  swj_Buffer_lon = 0;
}
void swj_Buffer_UpDat(uint8_t dat)
{
  swj_Buffer[swj_Buffer_lon++] = dat;
  if (swj_Buffer_lon == swj_Buff_SIZE)
    swj_Buffer_put();
}
void swj_Buffer_UpBuf(uint8_t *dat, int lon)
{
  for (int i = 0; i < lon; i++)
    swj_Buffer_UpDat((*(dat + i)));
}

void swj_init(void) //��ʼ������
{
  SPI2_Init();
  sw_write_buffer("hello world", 12); //����
}

//--------------��ͨ�Ҷ�ͼ��-------------------//
//*imageͼ���ַ widthͼ��� heightͼ���
// ����sendimg(mt9v03x_image_dvp[0], MT9V03X_DVP_W, MT9V03X_DVP_H);
// ���ݰ���С:6+width * height(ͼ��һ֡���ֽ���)
void sendimg(uint8_t *image, uint8_t width, uint8_t height)
{
  uint8_t dat[6] = {0x21, 0x7A, width, height, 0x21, 0x7A};
  sw_write_buffer(dat, 6);
  sw_write_buffer(image, width * height);
}
//--------------�����ŻҶ�ͼ��-------------------//
// ����ʧ���ݵ�����£���Э�������¶�λ����ʵ��һ���̶ȿ���������
//*imageͼ���ַ widthͼ��� heightͼ���
// ����sendimg(mt9v03x_image_dvp[0], MT9V03X_DVP_W, MT9V03X_DVP_H);
// ���ݰ���С:6+��width+3�� * height(ͼ��һ֡���ֽ���)
void sendimg_A(uint8_t *image, uint8_t width, uint8_t height)
{

  swj_Buffer_UpDat(0x21);
  swj_Buffer_UpDat(0x7A);
  swj_Buffer_UpDat(width);
  swj_Buffer_UpDat(height);
  swj_Buffer_UpDat((width + height) / 2);
  swj_Buffer_UpDat(0x7A);

  uint8_t line = 0, col = 0;
  for (line = 0; line < width; line++)
  {
    swj_Buffer_UpDat(21);
    swj_Buffer_UpDat(line);
    swj_Buffer_UpDat(133);
    for (col = 0; col < height; col++)
    {
      swj_Buffer_UpDat(*(image + line * height + col));
    }
  }
  swj_Buffer_put();
}

// ����У��Ķ�ֵͼ��
// chkֵԽ�� ������Խǿ ֵ0-55
// �����ʵ��ʹ��������е���
void sendimg_binary_CHK(uint8_t *image, uint8_t width, uint8_t height, uint8_t otu, uint8_t chk)
{
  chk = chk > 0 ? chk : 0;
  chk = chk < 56 ? chk : 55;
  uint8_t dat[7] = {0x7A, 0x21, width, height, 0x7A, 0x21, 200 + chk};
  swj_Buffer_UpBuf(dat, 7);
  int databool = 255;
  uint8_t lon = 0;
  int data = 255;
  uint8_t line = 0, col = 0;
  int imglon = 0;
  int imgdatlo = width * height / chk;
  uint8_t CHK = 0;
  for (line = 0; line < width; line++)
  {
    for (col = 0; col < height; col++)
    {
      imglon++;

      if (*(image + line * height + col) > otu)
        data = 255;
      else
        data = 0;
      if (data == databool)
      {
        lon++;
      }
      else
      {
        swj_Buffer_UpDat(lon);
        lon = 1;
      }

      if (imglon == imgdatlo)
      {
        CHK++;
        swj_Buffer_UpDat(lon);
        data = 255;
        databool = 255;
        swj_Buffer_UpDat(200 + CHK);
        lon = 0;
        imglon = 0;
      }
      if (lon == 190)
      {
        swj_Buffer_UpDat(lon);
        swj_Buffer_UpDat(0);
        lon = 0;
      }
      databool = data;
    }
  }
  swj_Buffer_put();
}
//--------------��ֵ��ͼ�����߽�-------------------//
//! ע��:�ȷ��߽� �� ��־�� ��ͼ����ͼ��
//----------------------���ͻ�����------------------------------

// �߽� *�߽����Ͷ�����Ҫ��uint8_t
void sendimgAndLine(uint8_t color, uint8_t *buff, uint32_t len)
{
  Line_Buffer[Line_lon] = (0x21);
  Line_lon++;
  Line_Buffer[Line_lon] = (color);
  Line_lon++;
  Line_Buffer[Line_lon] = (len);
  Line_lon++;
  Line_Buffer[Line_lon] = (255);
  Line_lon++;
  Line_Buffer[Line_lon] = (255);
  Line_lon++;
  for (int i = 0; i < len; i++)
  {
    Line_Buffer[Line_lon] = *(buff + i);
    Line_lon++;
  }
}
/*���Ʊ���   color������ɫ  uint8_t_t *buff ���͵ı��������ַ  len���͵ı��߳���
 * �����ű߽��������int���� size_lon��4(int ռ4�ֽ�)
 *�����ű߽��������uint6���� size_lon��2(uint6 ռ2�ֽ�)
 * �����ű߽��������uint8_t���� size_lon��1(uint8_t ռ1�ֽ�) ��ͬ��sendimgAndLine( uint8_t color,uint8_t *buff, uint32_t len)
 */
void sendimgAndLine_type(uint8_t color, uint8_t *buff, uint32_t len, uint8_t size_lon)
{
  Line_Buffer[Line_lon] = (0x21);
  Line_lon++;
  Line_Buffer[Line_lon] = (color);
  Line_lon++;
  Line_Buffer[Line_lon] = (len);
  Line_lon++;
  Line_Buffer[Line_lon] = (255);
  Line_lon++;
  Line_Buffer[Line_lon] = (255);
  Line_lon++;
  for (int i = 0; i < len * size_lon; i += size_lon)
  {
    Line_Buffer[Line_lon] = *(buff + i);
    Line_lon++;
  }
}
// #define swj_point_type1 1 //Сʮ�� 3x3
// #define swj_point_type2 2 //��ʮ�� 5x5
// #define swj_point_type3 3 //��ʮ�� 7x7
// #define swj_point_type4 4 //С  X  3x3
// #define swj_point_type5 5 //��  X  5x5
// #define swj_point_type6 6 //��  X  7x7
// #define swj_point_type7 7 //ȫ��ʮ��
// #define swj_point_type8 8 //����һ��ֱ�� y������ x��������
// #define swj_point_type9 9 //����һ��ֱ�� x������ y��������
// ��־�㣨����յ� ���ⲹ�ߵĵ㣩
// ���� �㣨10,11��sendpoint(swj_RED,10,11,swj_point_type1);//��10��11�������ֺ�ɫСʮ��
// ��ɫ ����x ����y �����ͣ�������ĺ궨�壩
void sendimgAndLine_point(uint8_t color, uint8_t x, uint8_t y, uint8_t type)
{
  Line_Buffer[Line_lon++] = (0x22);
  Line_Buffer[Line_lon++] = (color);
  Line_Buffer[Line_lon++] = (type);
  Line_Buffer[Line_lon++] = (254);
  Line_Buffer[Line_lon++] = (x);
  Line_Buffer[Line_lon++] = (y);
  Line_Buffer[Line_lon++] = (255);
}
// ͼ��
void sendimg_BinaryAndLine(uint8_t *image, uint8_t width, uint8_t height, uint8_t otu)
{
  uint8_t chk = 50;
  uint8_t dat2[6] = {0x77, 0x22, 0x77, 0x22, Line_lon / 255 % 255, Line_lon % 255};
  swj_Buffer_UpBuf(dat2, 6);
  uint8_t dat[7] = {0x7A, 0x21, width, height, 0x7A, 0x21, 200 + chk};
  swj_Buffer_UpBuf(dat, 7);
  int databool = 255;
  uint8_t lon = 0;
  int data = 255;
  uint8_t line = 0, col = 0;
  int imglon = 0;
  int imgdatlo = width * height / chk;
  uint8_t CHK = 0;
  for (line = 0; line < width; line++)
  {
    for (col = 0; col < height; col++)
    {
      imglon++;

      if (*(image + line * height + col) > otu)
        data = 255;
      else
        data = 0;
      if (data == databool)
      {
        lon++;
      }
      else
      {
        swj_Buffer_UpDat(lon);
        lon = 1;
      }

      if (imglon == imgdatlo)
      {
        CHK++;
        swj_Buffer_UpDat(lon);
        data = 255;
        databool = 255;
        swj_Buffer_UpDat(200 + CHK);
        lon = 0;
        imglon = 0;
      }
      if (lon == 190)
      {
        swj_Buffer_UpDat(lon);
        swj_Buffer_UpDat(0);
        lon = 0;
      }
      databool = data;
    }
  }
  swj_Buffer_put();
  uint8_t dat3[6] = {0x34, 0xFC, 0x34, 0xFC, 0x34, 0xFC};
  sw_write_buffer(dat3, 6);
  sw_write_buffer(Line_Buffer, Line_lon);
  Line_lon = 0;
}
// ��У���wifi��ֵ��ͼ�� ��7����ֵ������ѹ����һ��byte ������wifi��ֵ��ͼ��
// ���ֶ�������������
// �������ֵ��ͼ����� �ŵ�:���ݰ����ȹ̶� (width*height/7+8+6+6+width*height/350+���������ݳ���) �����������ٶȿ�
void sendimg_7binaryTobyteANDline_CHK(uint8_t *image, uint8_t width, uint8_t height, uint8_t otu)
{
//  uint8_t dat2[6] = {0x77, 0x22, 0x77, 0x22, Line_lon / 255 % 255, Line_lon % 255};
//  swj_Buffer_UpBuf(dat2, 6);
//  uint8_t dat[8] = {253, 0x7A, 0x21, width, height, 0x7A, 0x21, 252};
//  swj_Buffer_UpBuf(dat, 8);

//  // sw_write_buffer(image, width * height);
//  for (int i = 0; i < width * height; i += 7)
//  {
//    if (i % 350 == 0)
//      sw_write_byte(128 + i / 350);
//    uint8_t image_dat = 0;
//    for (int j = 0; j < 7; j++)
//    {
//      uint8_t image_byte = *(image + i + j);
//      image_dat = image_dat << 1;
//      if (image_byte > otu)
//      {
//        image_dat |= 1;
//      }
//    }
//    swj_Buffer_UpDat(image_dat);
//  }
//  swj_Buffer_put();
//  uint8_t dat3[6] = {0x34, 0xFC, 0x34, 0xFC, 0x34, 0xFC};
//  sw_write_buffer(dat3, 6);
//  sw_write_buffer(Line_Buffer, Line_lon);
//  Line_lon = 0;
}
// ��8����ֵ������ѹ����һ��byte ������wifi��ֵ��ͼ��(WIFI�����ǳ�������� �������������У���)
// �������ֵ��ͼ����� �ŵ�:���ݰ����ȹ̶� (width*height/8+8+6+6+���������ݳ���) �����������ٶȿ�
void sendimg_8binaryTobyteANDline(uint8_t *image, uint8_t width, uint8_t height, uint8_t otu)
{
  uint8_t dat2[6] = {0x77, 0x22, 0x77, 0x22, Line_lon / 255 % 255, Line_lon % 255};
  swj_Buffer_UpBuf(dat2, 6);
  uint8_t dat[8] = {253, 0x7A, 0x21, width, height, 0x7A, 0x21, 254};
  swj_Buffer_UpBuf(dat, 8);
  // sw_write_buffer(image, width * height);
  for (int i = 0; i < width * height; i += 8)
  {
    uint8_t image_dat = 0;
    for (int j = 0; j < 7; j++)
    {
      uint8_t image_byte = *(image + i + j);
      image_dat = image_dat << 1;
      if (image_byte > otu)
      {
        image_dat |= 1;
      }
    }
    swj_Buffer_UpDat(image_dat);
  }
  swj_Buffer_put();
  uint8_t dat3[6] = {0x34, 0xFC, 0x34, 0xFC, 0x34, 0xFC};
  sw_write_buffer(dat3, 6);
  sw_write_buffer(Line_Buffer, Line_lon);
  Line_lon = 0;
}

// ��ɫͼ�� imageͼ���ַ lon���ݳ�
void sendimg_JPEG(uint8_t *image, int lon)
{
  uint8_t dat[7] = {0x21, 0xFE, 0x7A, lon / 255, lon % 255, 0x21, 0x7A};
  sw_write_buffer(dat, 7);
  sw_write_buffer(image, lon);
}
//--------------���ݱ�ǩ������ʾ����������ɫ�궨�����ݼ��ӣ��Ǳ��̣�-------------------//
// һ������ռһ����ַ,��ֱ�ӳ�����ͼ��ҳ������� ��һ�¿���ʾ�����Ҽ�����������ɫ�궨 �趨��
// ��ֵ����ɫ�궨���� ���ټ���������û�д�����Ҳ���԰󶨱��̣��ٶȸ�ֱ�ۡ�¼��ʱ��ͬ��¼��
// �����ں�ϻ�ӣ�С��ʲô״̬һ�۱�֪
// name���ݱ�ʶ(ͨ������ַ)[0-255]  dat:����
// ��:int a=0;put_int32(0,a);

// ����У�������ʾ��
void put_int32(uint8_t name, int dat)
{
  uint8_t datc[10] = {197, name, 1, 0, 0, 0, 0, 0, 0, 198};
  datc[3] = (uint8_t)(dat & 0xFF);
  datc[4] = (uint8_t)((dat & 0xFF00) >> 8);
  datc[5] = (uint8_t)((dat & 0xFF0000) >> 16);
  datc[6] = (uint8_t)((dat >> 24) & 0xFF);
  uint8_t crc[6] = {name, 1, datc[3], datc[4], datc[5], datc[6]};
  uint16_t CRC16 = swj_CRC(crc, 0, 6);
  datc[7] = (uint8_t)(CRC16 & 0xff);
  datc[8] = (uint8_t)(CRC16 >> 8);
  sw_write_buffer(datc, 10);
}

void put_float(uint8_t name, float dat)
{
  uint8_t datc[10] = {197, name, 2, 0, 0, 0, 0, 0, 0, 198};
  char farray[4] = {0};

  *(float *)farray = dat;
  unsigned char *p = (unsigned char *)&dat + 3;
  datc[3] = *(p - 3);
  datc[4] = *(p - 2);
  datc[5] = *(p - 1);
  datc[6] = *p;

  uint8_t crc[6] = {name, 2, datc[3], datc[4], datc[5], datc[6]};
  uint16_t CRC16 = swj_CRC(crc, 0, 6);
  datc[7] = (uint8_t)(CRC16 & 0xff);
  datc[8] = (uint8_t)(CRC16 >> 8);
  sw_write_buffer(datc, 10);
}

//--------------����-------------------//
/*
 * ���Խ�Ѱ�ߵõ������ұ��ߣ�������߷��͵���λ���鿴
 * ���磺
 * ���� ͼ��img[H][W];�õ����ұ��ߴ����zx[H] yx[H] �����������Ϊw[H]
 * sendline_clear(swj_BLACK,W,H);//���� ������ɫ
 * sendline(swj_WHITE,zx,H);//���������
 * sendline(swj_WHITE,yx,H);//�����ұ���
 * sendline(swj_WHITE,wx,H);//��������
 * ���磺
 * sendline_clear(swj_BLACK,W,H);//���� ������ɫ
 * sendline_xy(zx,yx,H)//�������ұ���
 * sendline(swj_WHITE,wx,H);//��������
 *
 * ������������Ч��һ�µģ��������������λ���϶�Ӧʹ��������ԭ����
 * ע�⣺
 * ��ÿ������һ֡��ͼ����� ��Ҫ����sendline_clear��������
 * ���������sendline_xy������ʹ����λ��������ԭ����ʱ�����ٵ���sendline һ������sendline_xy���� ��ֹ������
 * */
// #define swj_BLACK 0
// #define swj_WHITE 1
// #define swj_RED 2
// #define swj_GREEN 3
// #define swj_BLUE 4
// #define swj_PURPLE 5
// #define swj_YELLOW 6
// #define swj_CYAN 7
// #define swj_ORANGE 8

// �������   color������ı�����ɫ  uint16_t width uint16_t height ͼ��Ĵ�С
void sendline_clear(uint8_t color, uint8_t width, uint8_t height)
{
  swj_Buffer_put();
  swj_Buffer_UpDat(0x21);
  swj_Buffer_UpDat(0x7A);
  swj_Buffer_UpDat(width);
  swj_Buffer_UpDat(height);
  swj_Buffer_UpDat(color);
  swj_Buffer_UpDat(0x21);
}

/*���Ʊ���   color������ɫ  uint8_t_t *buff ���͵ı��������ַ  len���͵ı��߳���
 * �����ű߽��������int���� size_lon��4(int ռ4�ֽ�)
 *�����ű߽��������uint6���� size_lon��2(uint6 ռ2�ֽ�)
 * �����ű߽��������uint8_t���� size_lon��1(uint8_t ռ1�ֽ�) ��ͬ��sendline( uint8_t color,uint8_t *buff, uint32_t len)
 */
void sendline_type(uint8_t color, uint8_t *buff, uint32_t len, uint8_t size_lon)
{
  swj_Buffer_UpDat(0x21);
  swj_Buffer_UpDat(color);
  swj_Buffer_UpDat(len);
  swj_Buffer_UpDat(255);
  swj_Buffer_UpDat(255);
  for (int i = 0; i < len * size_lon; i += size_lon)
  {
    swj_Buffer_UpDat(*(buff + i));
  }
}
/*Ĭ��ÿ��һ����*/
// ���Ʊ���   color������ɫ  uint8_t_t *buff ���͵ı��������ַ  len���͵ı��߳��� *�߽����Ͷ�����Ҫ��uint8_t
void sendline(uint8_t color, uint8_t *buff, uint32_t len)
{
  swj_Buffer_UpDat(0x21);
  swj_Buffer_UpDat(color);
  swj_Buffer_UpDat(len);
  swj_Buffer_UpDat(255);
  swj_Buffer_UpDat(255);
  swj_Buffer_UpBuf(buff, len);
}
/*˵��:
 * ������������ a(x1,y1)b(x2,y2)c(x3,y3)
 * �� uint8_t x[3]={x1,x2,x3};uint8_t y[3]={y1,y2,y3};
 *  sendline2(swj_WHITE,x,y,3);
 *  sendline����ֻ�ܰ�˳��ÿ��һ�㷢�ͱ߽��
 *  sendline2��������ڰ������ ÿ�в��̶������ı߽�
 *           Ҳ�����ڷ�������� ����յ� �������
 *
 * */
// ��˳����Ʊ���  color������ɫ linex��Ӧ���x���꼯�� liney��Ӧ���y���꼯��  len���͵ı��߳���
void sendline2(uint8_t color, uint8_t *linex, uint8_t *liney, uint32_t len)
{
  swj_Buffer_UpDat(0x21);
  swj_Buffer_UpDat(color);
  swj_Buffer_UpDat(len);
  swj_Buffer_UpDat(254);
  swj_Buffer_UpDat(255);
  swj_Buffer_UpBuf(linex, len);
  swj_Buffer_UpBuf(liney, len);
}
// ͼ�������߽�  uint8_t_t *zx:��߽�   uint8_t_t *yx:�ұ߽�, uint32_t_t len���͵ı��߳���
void sendline_xy(uint8_t *line_zx, uint8_t *line_yx, uint32_t len)
{
  swj_Buffer_UpDat(0x21);
  swj_Buffer_UpDat(9);
  swj_Buffer_UpDat(len);
  swj_Buffer_UpDat(255);
  swj_Buffer_UpDat(255);
  swj_Buffer_UpBuf(line_zx, len);
  swj_Buffer_UpBuf(line_yx, len);
}
// #define swj_point_type1 1 //Сʮ�� 3x3
// #define swj_point_type2 2 //��ʮ�� 5x5
// #define swj_point_type3 3 //��ʮ�� 7x7
// #define swj_point_type4 4 //С  X  3x3
// #define swj_point_type5 5 //��  X  5x5
// #define swj_point_type6 6 //��  X  7x7
// #define swj_point_type7 7 //ȫ��ʮ��
// #define swj_point_type8 8 //����һ��ֱ�� y������ x��������
// #define swj_point_type9 9 //����һ��ֱ�� x������ y��������
// ��־�㣨����յ� ���ⲹ�ߵĵ㣩
// ���� �㣨10,11��sendpoint(swj_RED,10,11,swj_point_type1);//��10��11�������ֺ�ɫСʮ��
// ��ɫ ����x ����y �����ͣ�������ĺ궨�壩
void sendpoint(uint8_t color, uint8_t x, uint8_t y, uint8_t type)
{
  swj_Buffer_UpDat(0x22);
  swj_Buffer_UpDat(color);
  swj_Buffer_UpDat(type);
  swj_Buffer_UpDat(254);
  swj_Buffer_UpDat(x);
  swj_Buffer_UpDat(y);
  swj_Buffer_UpDat(255);
}

void swj_stop()
{
  uint8_t buff[8] = {0xFB, 0xFE, 0x00, 0x20, 0xFA, 0x20, 0xFA, 0x20};
  sw_write_buffer(buff, 8);
}

void swj_start()
{
  uint8_t buff[8] = {0xFB, 0xFE, 0x01, 0xEC, 0xFA, 0x20, 0xFA, 0x20};
  sw_write_buffer(buff, 8);
}

// �ڲ�����
uint16_t swj_CRC(uint8_t *Buf, uint8_t CRC_sta, uint8_t CRC_CNT)
{
  uint16_t CRC_Temp;
  uint8_t i, j;
  CRC_Temp = 0xffff;

  for (i = CRC_sta; i < CRC_CNT; i++)
  {
    CRC_Temp ^= Buf[i];
    for (j = 0; j < 8; j++)
    {
      if (CRC_Temp & 0x01)
        CRC_Temp = (CRC_Temp >> 1) ^ 0xa001;
      else
        CRC_Temp = CRC_Temp >> 1;
    }
  }
  return (CRC_Temp);
}
