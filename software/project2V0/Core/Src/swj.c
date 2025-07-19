/*
 * GL-Shw智能车上位机  qq群:297809802 qq1849057843
*说明 :spi-wifi模块单字节发送效率低，所以先将数据缓存到缓冲区 然后 再批量发送
*以下为常用功能
*
*示波器 :
*void put_int32(uint8_t name, int dat) void put_float(uint8_t name, int dat);
* 特殊说明:可以和图传一起用 边图传边看数据 数据会显示在上位机右侧数据栏里
*
*
*图传:
*     灰度
*  image要发送的图像  width图像的宽 height图像的高    调用:sendimg(mt9v03x_image[0],MT9V03X_W,MT9V03X_H);
*  void sendimg(uint8_t* image, uint8_t width, uint8_t height);
*  特殊说明:如果网络不稳定 则用sendimg_A(mt9v03x_image[0],MT9V03X_W,MT9V03X_H);
*      二值
*   ots是二值化阈值 如果image是二值化图像则给0  chk是校验值 越大越稳定 最大55
*  void sendimg_binary_CHK(uint8_t* image, uint8_t width, uint8_t height,uint8_t otu,uint8_t chk);// (自己创新的二值化 超快 蓝牙都能跑50FPS)
*
*
*边界和特殊点:(会单独显示在右侧图像框里 左侧是图像 右侧是边界 不在一起)
         清屏****(一定要注意，每帧都要调用一次，调用完再发边界  这个是用来清理覆盖上次边界的)******
  void sendline_clear(uint8_t color,uint8_t width, uint8_t height)
*   传统的边界(一行一个边界点)
* void sendline( uint8_t color,uint8_t *buff, uint32_t len)
*   特殊的边界(八邻域 迷宫)
* void sendline2( uint8_t color,uint8_t *linex,uint8_t *liney, uint32_t len)
*   关键点:例如拐点 截止行等等
* void sendpoint(uint8_t color,uint8_t x,uint8_t y, uint8_t type)
*
*
* 图像和边界在一起:             (不用再清屏了 图像会覆盖掉旧的边)
* 边
* void sendimgAndLine( uint8_t color,uint8_t *buff, uint32_t len)
* 点
* void sendimgAndLine_point(uint8_t color,uint8_t x,uint8_t y, uint8_t type)
* 图像
* sendimg_BinaryAndLine
*
*
 */
#include "swj.h"

#define swj_Buff_SIZE 1024 // 缓冲区大小
static uint8_t swj_Buffer[swj_Buff_SIZE] = {0};
int swj_Buffer_lon = 0;

#define Line_SIZE (300) // 边界缓冲区大小
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

void swj_init(void) //初始化函数
{
  SPI2_Init();
  sw_write_buffer("hello world", 12); //测试
}

//--------------普通灰度图传-------------------//
//*image图像地址 width图像宽 height图像高
// 例：sendimg(mt9v03x_image_dvp[0], MT9V03X_DVP_W, MT9V03X_DVP_H);
// 数据包大小:6+width * height(图传一帧的字节数)
void sendimg(uint8_t *image, uint8_t width, uint8_t height)
{
  uint8_t dat[6] = {0x21, 0x7A, width, height, 0x21, 0x7A};
  sw_write_buffer(dat, 6);
  sw_write_buffer(image, width * height);
}
//--------------抗干扰灰度图传-------------------//
// 当丢失数据的情况下，该协议能重新定位行来实现一定程度抗干扰能力
//*image图像地址 width图像宽 height图像高
// 例：sendimg(mt9v03x_image_dvp[0], MT9V03X_DVP_W, MT9V03X_DVP_H);
// 数据包大小:6+（width+3） * height(图传一帧的字节数)
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

// 带有校验的二值图传
// chk值越大 抗干扰越强 值0-55
// 请根据实际使用情况进行调整
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
//--------------二值化图传带边界-------------------//
//! 注意:先发边界 和 标志点 再图传发图像
//----------------------发送缓冲区------------------------------

// 边界 *边界类型定义需要是uint8_t
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
/*绘制边线   color边线颜色  uint8_t_t *buff 发送的边线数组地址  len发送的边线长度
 * 如果存放边界的数组用int定义 size_lon填4(int 占4字节)
 *如果存放边界的数组用uint6定义 size_lon填2(uint6 占2字节)
 * 如果存放边界的数组用uint8_t定义 size_lon填1(uint8_t 占1字节) 等同于sendimgAndLine( uint8_t color,uint8_t *buff, uint32_t len)
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
// #define swj_point_type1 1 //小十字 3x3
// #define swj_point_type2 2 //中十字 5x5
// #define swj_point_type3 3 //大十字 7x7
// #define swj_point_type4 4 //小  X  3x3
// #define swj_point_type5 5 //中  X  5x5
// #define swj_point_type6 6 //大  X  7x7
// #define swj_point_type7 7 //全屏十字
// #define swj_point_type8 8 //横向一条直线 y起作用 x不起作用
// #define swj_point_type9 9 //众向一条直线 x起作用 y不起作用
// 标志点（列如拐点 特殊补线的点）
// 例如 点（10,11）sendpoint(swj_RED,10,11,swj_point_type1);//（10，11）处出现红色小十字
// 颜色 坐标x 坐标y 点类型（见上面的宏定义）
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
// 图传
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
// 待校验的wifi二值化图传 将7个二值化数据压缩成一个byte 适用于wifi二值化图传
// 出现丢包的情况用这个
// 与上面二值化图传相比 优点:数据包长度固定 (width*height/7+8+6+6+width*height/350+缓冲区数据长度) 但不如上面速度快
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
// 将8个二值化数据压缩成一个byte 适用于wifi二值化图传(WIFI环境非常好用这个 否则还是用上面带校验的)
// 与上面二值化图传相比 优点:数据包长度固定 (width*height/8+8+6+6+缓冲区数据长度) 但不如上面速度快
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

// 彩色图传 image图像地址 lon数据长
void sendimg_JPEG(uint8_t *image, int lon)
{
  uint8_t dat[7] = {0x21, 0xFE, 0x7A, lon / 255, lon % 255, 0x21, 0x7A};
  sw_write_buffer(dat, 7);
  sw_write_buffer(image, lon);
}
//--------------数据标签（数据示波，数据颜色标定，数据监视，仪表盘）-------------------//
// 一个数据占一个地址,会直接出现在图传页面的右栏 点一下可以示波，右键可以设置颜色标定 设定好
// 阈值会颜色标定数据 快速监视条件有没有触发，也可以绑定表盘，速度更直观。录制时会同步录制
// 类似于黑匣子，小车什么状态一眼便知
// name数据标识(通道、地址)[0-255]  dat:数据
// 例:int a=0;put_int32(0,a);

// 带有校验的数据示波
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

//--------------传线-------------------//
/*
 * 可以将寻线得到的左右边线，拟的中线发送到上位机查看
 * 例如：
 * 处理 图像img[H][W];得到左右边线存放在zx[H] yx[H] 拟出来的中线为w[H]
 * sendline_clear(swj_BLACK,W,H);//清屏 背景黑色
 * sendline(swj_WHITE,zx,H);//发送左边线
 * sendline(swj_WHITE,yx,H);//发送右边线
 * sendline(swj_WHITE,wx,H);//发送中线
 * 例如：
 * sendline_clear(swj_BLACK,W,H);//清屏 背景黑色
 * sendline_xy(zx,yx,H)//发送左右边线
 * sendline(swj_WHITE,wx,H);//发送中线
 *
 * 如上两个方法效果一致的，但下面可以在上位机上对应使用赛道还原功能
 * 注意：
 * ①每发送完一帧的图像边线 就要调用sendline_clear进行清屏
 * ②如果调用sendline_xy函数并使用上位机赛道还原功能时，若再调用sendline 一定放在sendline_xy后面 防止被覆盖
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

// 清空线条   color清屏后的背景颜色  uint16_t width uint16_t height 图像的大小
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

/*绘制边线   color边线颜色  uint8_t_t *buff 发送的边线数组地址  len发送的边线长度
 * 如果存放边界的数组用int定义 size_lon填4(int 占4字节)
 *如果存放边界的数组用uint6定义 size_lon填2(uint6 占2字节)
 * 如果存放边界的数组用uint8_t定义 size_lon填1(uint8_t 占1字节) 等同于sendline( uint8_t color,uint8_t *buff, uint32_t len)
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
/*默认每行一个点*/
// 绘制边线   color边线颜色  uint8_t_t *buff 发送的边线数组地址  len发送的边线长度 *边界类型定义需要是uint8_t
void sendline(uint8_t color, uint8_t *buff, uint32_t len)
{
  swj_Buffer_UpDat(0x21);
  swj_Buffer_UpDat(color);
  swj_Buffer_UpDat(len);
  swj_Buffer_UpDat(255);
  swj_Buffer_UpDat(255);
  swj_Buffer_UpBuf(buff, len);
}
/*说明:
 * 例如有三个点 a(x1,y1)b(x2,y2)c(x3,y3)
 * 则 uint8_t x[3]={x1,x2,x3};uint8_t y[3]={y1,y2,y3};
 *  sendline2(swj_WHITE,x,y,3);
 *  sendline函数只能按顺序每行一点发送边界点
 *  sendline2函数针对于八邻域等 每行不固定点数的边界
 *           也适用于发送特殊点 例如拐点 灵活运用
 *
 * */
// 无顺序绘制边线  color边线颜色 linex对应点的x坐标集合 liney对应点的y坐标集合  len发送的边线长度
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
// 图传赛道边界  uint8_t_t *zx:左边界   uint8_t_t *yx:右边界, uint32_t_t len发送的边线长度
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
// #define swj_point_type1 1 //小十字 3x3
// #define swj_point_type2 2 //中十字 5x5
// #define swj_point_type3 3 //大十字 7x7
// #define swj_point_type4 4 //小  X  3x3
// #define swj_point_type5 5 //中  X  5x5
// #define swj_point_type6 6 //大  X  7x7
// #define swj_point_type7 7 //全屏十字
// #define swj_point_type8 8 //横向一条直线 y起作用 x不起作用
// #define swj_point_type9 9 //众向一条直线 x起作用 y不起作用
// 标志点（列如拐点 特殊补线的点）
// 例如 点（10,11）sendpoint(swj_RED,10,11,swj_point_type1);//（10，11）处出现红色小十字
// 颜色 坐标x 坐标y 点类型（见上面的宏定义）
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

// 内部调用
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
