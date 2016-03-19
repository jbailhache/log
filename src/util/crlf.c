
#include <stdio.h>
#include <errno.h>

main (int argc, char *argv[])
{
FILE *f, *out;
char c;
	f = fopen (argv[1], "rb");
	if (f == NULL)
	{
		perror (argv[1]);
		return errno;
	}
/*
	out = fopen (argv[2], "wb");
	if (out == NULL)
	{
		perror (argv[2]);
		return errno;
	}
*/
	for (;;)
	{
		c = getc (f);
		if (feof(f))
		{
			fclose (f);
			return;
		}
		/*
		if (c == '\n')
			putc ('\r', out);
		*/
		putchar (c);
	}
}
