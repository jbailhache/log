
#include <stdio.h>

main (int argc, char *argv[])
{
 FILE *in, *out;
 char c1, c2, c;
 in = fopen (argv[1],"r");
 out = fopen (argv[2], "wb");
 for (;;)
 {
  c1 = fgetc(in);
  if (feof(in))
   break;
  if (c1 >= '@' && c1 <= 'O')
  {
   c2 = fgetc(in);
   c = c1 - '@' + 16 * (c2 - '@');
   fputc (c, out);
  }
 }
}
