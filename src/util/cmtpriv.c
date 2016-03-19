
#include <stdio.h>
#include <errno.h>

main (int argc, char *argv[])
{
FILE *in, *out;
char buf[1000];
char *s;
int flag;
	in = fopen (argv[1], "r");
	if (in == NULL)
	{
		perror (argv[1]);
		return errno;
	}
	if (argc < 3)
		out = stdout;
	else
	{
		out = fopen (argv[2], "w");
		if (out == NULL)
		{
			perror (argv[2]);
			return errno;
		}
	}
	flag = 0;
	for (;;)
	{
		s = fgets (buf, sizeof(buf)-1, in);
		if (s == NULL)
		{
			fclose (in);
			fclose (out);
			return 0;
		}
		if (flag)
		{
			if (!strncmp (buf, "#endif /* PRIVATE_COMMENT */",
				26))
				flag = 0;
		}
		else
		{
			if (!strncmp (buf, "#ifdef PRIVATE_COMMENT", 21))
				flag = 1;
			else
				fprintf (out, "%s", buf);

		}
	}
}