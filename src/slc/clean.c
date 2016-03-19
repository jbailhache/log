
#include <stdio.h>

main()
{
int c;
 for (;;)
 {
  c = getchar();
  if (c == EOF || feof(stdin))
   break;
  if (c=='\t' || c=='\n' || (c>=' ' && c<0x7F))
   putchar(c);
 }
}

