/*
* @Author: Comzyh
* @Date:   2014-11-08 01:30:56
* @Last Modified by:   Comzyh
* @Last Modified time: 2014-11-14 19:33:18
*/
#include <iostream>
#include <cstdio>
#include <cstdlib>
using namespace std;
uint16_t endian_16(uint16_t x)
{
    return (x >> 8) | (x << 8);
}
uint32_t endian_32(uint32_t x)
{
    return (x >> 24) | ((x & 0x00FF0000) >> 8  ) | ((x & 0x0000FF00) << 8 ) | (x << 24);
}
void write_block(FILE *file, void *ptr, int count)
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
    fwrite(&x,2,1,file);
}
void write_32(FILE *file, uint32_t x)
{
    x = endian_32(x);
    fwrite(&x,4,1,file);
}
void write_jpeg_header(FILE *file, int height, int width)
{
	write_16(file,0xFFD8);	//SOI = Start of Image

	//APP0，Application，应用程序保留标记0
	write_16(file,0xFFE0);		//标记代码=固定值0xFFE0
	write_16(file,18); 			//数据长度
	write_block(file,"JFIF",5);	//标识符
	write_16(file,0x0102);		// 版本号
	write_8(file,0);			//X和Y的密度单位
	write_16(file,1);			//X方向像素密度
	write_16(file,1);			//Y方向像素密度
	write_8(file,0);			//缩略图水平像素数目
	write_8(file,0);			//缩略图垂直像素数目
}
int main()
{

    return 0;
}
