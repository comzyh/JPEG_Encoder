/*
* @Author: Comzyh
* @Date:   2014-11-08 01:30:56
* @Last Modified by:   Comzyh
* @Last Modified time: 2014-11-16 04:33:41
*/
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include "bmp.h"
using namespace std;

//ZigZag 的数组下标表
//Usage: target[ZigZag[i*8+j]] = f(i,j)
const uint8_t ZigZag[64] =
{
    0,  1,  5,  6,  14, 15, 27, 28,
    2,  4,  7,  13, 16, 26, 29, 42,
    3,  8,  12, 17, 25, 30, 41, 43,
    9,  11, 18, 24, 31, 40, 44, 53,
    10, 19, 23, 32, 39, 45, 52, 54,
    20, 22, 33, 38, 46, 51, 55, 60,
    21, 34, 37, 47, 50, 56, 59, 61,
    35, 36, 48, 49, 57, 58, 62, 63
};

//亮度(Y)量化表
const uint8_t Luminance_Quantization_Table[64] =
{
    16,  11,  10,  16,  24,  40,  51,  61,
    12,  12,  14,  19,  26,  58,  60,  55,
    14,  13,  16,  24,  40,  57,  69,  56,
    14,  17,  22,  29,  51,  87,  80,  62,
    18,  22,  37,  56,  68, 109, 103,  77,
    24,  35,  55,  64,  81, 104, 113,  92,
    49,  64,  78,  87, 103, 121, 120, 101,
    72,  92,  95,  98, 112, 100, 103,  99
};

//色差(CbCr)量化表
const uint8_t Chromiance_Quantization_Table[64] =
{
    17,  18,  24,  47,  99,  99,  99,  99,
    18,  21,  26,  66,  99,  99,  99,  99,
    24,  26,  56,  99,  99,  99,  99,  99,
    47,  66,  99,  99,  99,  99,  99,  99,
    99,  99,  99,  99,  99,  99,  99,  99,
    99,  99,  99,  99,  99,  99,  99,  99,
    99,  99,  99,  99,  99,  99,  99,  99,
    99,  99,  99,  99,  99,  99,  99,  99
};
uint16_t endian_16(uint16_t x)
{
    return (x >> 8) | (x << 8);
}
uint32_t endian_32(uint32_t x)
{
    return (x >> 24) | ((x & 0x00FF0000) >> 8  ) | ((x & 0x0000FF00) << 8 ) | (x << 24);
}
void write_block(FILE *file, const void *ptr, int count)
{
    fwrite(ptr, count, 1, file);
}
void write_8(FILE *file, uint8_t x)
{
    fwrite(&x, 1, 1, file);
}
void write_16(FILE *file, uint16_t x)
{
    x = endian_16(x);
    fwrite(&x, 2, 1, file);
}
void write_32(FILE *file, uint32_t x)
{
    x = endian_32(x);
    fwrite(&x, 4, 1, file);
}
class JPEG_Encoder
{
private:
    uint32_t img_width;
    uint32_t img_height;
    uint8_t *bitmap;
    uint8_t m_YTable[64];
    uint8_t m_CbCrTable[64];
public:
    JPEG_Encoder()
    {

    }
    ~JPEG_Encoder()
    {

    }
    void clear()
    {
        delete[] bitmap;
    }
    void read_bmp_file(const char *path)
    {
        clear();
        FILE *file = fopen(path, "rb");
        fseek(file, 0L, 0);
        BMPFileHeader bmp;
        BMPInfoHeader bmpinfo;
        fread(&bmp, 14, 1, file);
        fread(&bmpinfo, 40, 1, file);
        fseek(file, bmp.bfOffBit, 0);
        img_width = bmpinfo.biWidth;
        img_height = bmpinfo.biHeight;
        int bitmap_size = img_width * img_height * 3;
        bitmap = new uint8_t[bitmap_size];
        for (int i = 0; i < img_height; i++)
            fread(bitmap + (img_height - 1 - i)*img_width * 3, 3, img_width, file);
    }
    void print_bitmap()
    {
        for (int i = 0; i < img_height; i++)
        {
            for (int j = 0; j < img_width; j++)
                printf("%06x ", *((uint32_t*)(bitmap + (i * img_width + j) * 3)) &0xffffff);
            printf("\n");
        }
    }
};
void write_jpeg_header(FILE *file, int height, int width)
{
    write_16(file, 0xFFD8); //SOI = Start of Image

    //APP0，Application，应用程序保留标记0
    write_16(file, 0xFFE0);     //标记代码=固定值0xFFE0
    write_16(file, 18);         //数据长度
    write_block(file, "JFIF", 5); //标识符
    write_16(file, 0x0102);     // 版本号
    write_8(file, 0);           //X和Y的密度单位
    write_16(file, 1);          //X方向像素密度
    write_16(file, 1);          //Y方向像素密度
    write_8(file, 0);           //缩略图水平像素数目
    write_8(file, 0);           //缩略图垂直像素数目

    //DQT，Define Quantization Table，定义量化表
    write_16(file, 0xFFDB);     //标记代码=固定值0xFFDB
    write_16(file, 2 + 1 + 64 + 1 + 64); //数据长度
    write_8(file, 0x00);
}
int main()
{
    JPEG_Encoder je = JPEG_Encoder();
    je.read_bmp_file("resource\\bmp_test.bmp");
    je.print_bitmap();
    return 0;
}
