
#include <stdio.h>

main ()
{
FILE *f;
int c, i, j;
char line[200];

	f = fopen ("c:\\jacques\\mslpia\\mslpias.c", "rb");
	for (i=0; i<0x100; i++)
	{
		if (i % 16 == 0)
		{
			line[16] = 0;
			for (j=0; j<16; j++)
			{
				if (line[j] > 32 && line[j] < 127)
					printf ("%c", line[j]);
				else
					printf (".");
			}
			printf ("\n%4X : ", i);
		}
		c = fgetc (f);
		printf ("%02X ", c);
		line[i%16] = c;
	}
}
