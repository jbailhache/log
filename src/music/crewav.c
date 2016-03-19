
#include <stdio.h>
#include <math.h>

#define pi 3.1415926

double s (double t)
{
 return sin (2 * pi * 440 * t);
}

main ()
{
FILE *w;
int t;
long sps;
long nbd;
long size;
long i;
char c;
double t1, c1;

 w = fopen ("1.wav", "wb");
 if (w == NULL)
  perror("fopen");
 t = 10;
 sps = 0x1000;
 nbd = t * sps;
 size = nbd + 36;

 fwrite ("RIFF", 4, 1, w);
 fwrite (&size, 4, 1, w);
 fwrite ("WAVEfmt \x10\x00\x00\x00\x01\x00\x01\x00", 16, 1, w);
 fwrite (&sps, 4, 1, w);
 fwrite (&sps, 4, 1, w);
 fwrite ("\x01\x00\x08\x00", 4, 1, w);
 fwrite ("data", 4, 1, w);
 fwrite (&nbd, 4, 1, w);

 for (i=0; i<t*sps; i++)
 {
  /* printf (" %d ", i); */
  if (i % 0x100 == 0)
   printf ("%X\r", i);
  t1 = (double)i / (double)sps;
  c1 = s (t1);
  c = 0x80 + 0x7E * c1;
  fwrite (&c, 1, 1, w);
 }

 c = 0;
 fwrite (&c, 1, 1, w);
}
