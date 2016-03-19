
#pragma pack(1)

#include <stdio.h>
#include <float.h>

#define PI 3.1415926

#define xsize 64
#define ysize 64

typedef short int2;
typedef long int4;

struct fileheader
{
 int2 bfType;
 int4 bfSize;
 int2 bfReserved1;
 int2 bfReserved2;
 int4 bfOffBits;
} fileheader;

struct infoheader
{
 int4 biSize, biWidth, biHeight;
 int2 biPlanes, biBitCount;
 int4 biCompression, biSizeImage, biXPelsPerMeter, biYPelsPerMeter,
  biClrUsed, biClrImportant;
} infoheader;

struct rgbquad
{
 unsigned char rgbBlue, rgbGreen, rgbRed, rgbReserved;
};


main ()
{
FILE *f;
int x, y;
unsigned char color[3];
char buf[10];
float a, r;

 printf ("%d\n", (char *)&(fileheader.bfSize) - (char *)&fileheader);
 /* printf ("%d %d %d\n", sizeof(int), sizeof(short), sizeof(long));*/
 fileheader.bfType = 19778;
 fileheader.bfSize = 12342;
 fileheader.bfReserved1 = 0;
 fileheader.bfReserved2 = 0;
 fileheader.bfOffBits = 54;

 infoheader.biSize = 40;
 infoheader.biWidth = xsize;
 infoheader.biHeight = ysize;
 infoheader.biPlanes = 1;
 infoheader.biBitCount = 24;
 infoheader.biCompression = 0;
 infoheader.biSizeImage = 0;
 infoheader.biXPelsPerMeter = 0;
 infoheader.biYPelsPerMeter = 0;
 infoheader.biClrUsed = 0;
 infoheader.biClrImportant = 0;

 f = fopen ("testform.bmp", "wb");
 fwrite (&fileheader, sizeof(fileheader), 1, f);
 fwrite (&infoheader, sizeof(infoheader), 1, f);

 for (y=0; y<xsize; y++)
 for (x=0; x<ysize; x++)
 {
  color[0] = y*4;
  a = 2.0 * PI * (float)x/64.0;
  r = (cos (2.0 * PI * (float)y / 64.0) + 1.0) / 2.0;
  color[1] = 128.0 + 127.0 * r * sin (a);
  color[2] = 128.0 + 127.0 * r * cos (a);

  /* printf ("x=%d a=%2.6f color[1]=%d ",x,a,(int)color[1]);
  gets(buf);
  */
  
  fwrite (color, 3, 1, f);
 }

 fclose(f);

 
}
