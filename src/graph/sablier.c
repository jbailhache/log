
#pragma pack(1)

#include <stdio.h>
#include <math.h>

typedef short int2;
typedef int int4;

#define PI 3.1415926

struct bfh
{
 int2 bfType;
 int4 bfSize;
 int2 bfReserved1;
 int2 bfReserved2;
 int4 bfOffbits;
 } bfh;

struct bih
{
 int4 biSize;
 int4 biWidth;
 int4 biHeight;
 int2 biPlanes;
 int2 biBitCount;
 int4 biCompression;
 int4 biSizeImage;
 int4 biXpelsPerMeter;
 int4 biYpelsPerMeter;
 int4 biClrUsed;
 int4 biClrImportant;

} bih;


main()
{
FILE *f;
int x, y;
float r, g, b;
unsigned char rgb[4];
float a;
bfh.bfType = 'MB';
 bfh.bfSize = 12342;
 bfh.bfReserved1 = 0;
 bfh.bfReserved2 = 0;
 bfh.bfOffbits = 54;

 bih.biSize = 40;
 bih.biWidth = 64;
 bih.biHeight = 64;
 bih.biPlanes = 1;
 bih.biBitCount = 24;
 bih.biCompression = 0;
 bih.biSizeImage = 0x3000;
 bih.biXpelsPerMeter = 0xB13;
 bih.biYpelsPerMeter = 0xB13;
 bih.biClrUsed = 0;
 bih.biClrImportant = 0;

 f = fopen("test.bmp","wb");

 fwrite (&bfh, sizeof(bfh), 1, f);
 fwrite (&bih, sizeof(bih), 1, f);

 for (y=0; y<64; y++)
 for (x=0; x<64; x++)
 {
  b = y * 4;
  a = (cos (2*PI*(float)y/64.0) + 1) / 2.0;
  r = 128 + 127 * a * cos(2*PI*(float)x/64.0);
  g = 128 + 127 * a * sin(2*PI*(float)x/64.0);
  rgb[0] = b;
  rgb[1] = g;
  rgb[2] = r;
  fwrite (rgb, 3, 1, f);
 }
 fclose(f);
}

