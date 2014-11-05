/*
* @Author: Comzyh
* @Date:   2014-11-05 23:06:14
* @Last Modified by:   Comzyh
* @Last Modified time: 2014-11-06 04:01:57
*/
//尝试BMP文件的读取
#include <iostream>
#include <cstdio>
#include <cstdlib>
using namespace std;
#pragma pack(push,2)
struct BMPFileHeader //14 byte
{
    uint16_t bfType;                //文件标识 2字节 必须为BM
    uint32_t bfSize;                //文件大小 4字节
    uint16_t bfReserved1;           //保留，每字节以"00"填写 2字节
    uint16_t bfReserved2;           //同上 2字节
    uint32_t bfOffBit;              //记录图像数据区的起始位置(图象数据相对于文件头字节的偏移量)。 4字节
};
#pragma pack(pop)
struct BMPInfoHeader //40byte
{
    uint32_t biSize;                //表示本结构的大小 4字节
    uint32_t biWidth;               //位图的宽度  4字节
    uint32_t biHeight;              //位图的高度  4字节
    uint16_t biPlanes;              //永远为1 ,   2字节
    uint16_t biBitCount;            //位图的位数  分为1 4 8 16 24 32   2字节
    uint32_t biCompression;         //压缩说明   4字节
    uint32_t biSizeImage;           //表示位图数据区域的大小以字节为单位  4字节
    uint32_t biXPelsPerMeter;       //用象素/米表示的水平分辨率   4字节
    uint32_t biYPelsPerMeter;       //用象素/米表示的垂直分辨率   4字节
    uint32_t biClrUsed;             //位图使用的颜色索引数   4字节
    uint32_t biClrImportant;        //对图象显示有重要影响的颜色索引的数目  4字节
};
int main(int argc, char const *argv[])
{
    char path[256];
    if (argc >= 1)
        strcpy(path, argv[1]);
    else
        return 0;
    printf("%s\n", path );
    FILE *file = fopen(path, "rb");
    fseek(file, 0L, 0);
    BMPFileHeader bmp;
    BMPInfoHeader bmpinfo;
    fread(&bmp, 14, 1, file);
    fread(&bmpinfo, 40, 1, file);
    fseek(file, bmp.bfOffBit, 0);
    for (int r = 0; r < bmpinfo.biHeight; r++ )
    {
        for (int c = 0; c < bmpinfo.biWidth; c++)
        {
            int32_t p = 0;
            fread(&p, 3, 1, file);
            printf("%06x ", p);
        }
        printf("\n");
    }
    return 0;
}
