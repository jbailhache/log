
#include <stdio.h>
#include <errno.h>

int main (int argc, char *argv[])
{
FILE *in, *out;
char c, c1;
int flag_com;
	if (argc < 3)
	{
		printf ("usage: cppcom input_file output_file\n");
		return -1;
	}
	in = fopen (argv[1], "r");
	if (in == NULL)
	{
		perror (argv[1]);
		return errno;
	}
	out = fopen (argv[2], "w");
	if (out == NULL)
	{
		perror (argv[1]);
		return errno;
	}
	flag_com = 0;
	c = 0;
	for (;;)
	{
		c1 = c;
		c = getc (in);
		if (feof(in))
		{
			fclose (in);
			fclose (out);
			return 1;
		}
		if (c == '/' && c1 == '/' && !flag_com)
		{
			putc ('*', out);
			flag_com = 1;
		}
		else if (c == '\n')
		{
			if (flag_com)
			{
				putc (' ', out);
				putc ('*', out);
				putc ('/', out);
			}
			putc (c, out);
			flag_com = 0;
		}
		else
			putc (c, out);
	}
}