
#include "stdio.h"
#include "errno.h"


strq (char *r, char *s)
{
	for (; *s; s++)
	{
		switch (*s)
		{
			case '"': case '\\':
				*r++ = '\\';
			default:
				*r++ = *s;
		}
	}
	*r = 0;
}

preproc (char *r, char *s)
{
char s1[1000];

	if (*s == '%')
	{
		strcpy (r, s+1);
		return;
	}
	strq (s1, s);
	sprintf (r, "p(\"%s\");", s1);
}

main (int argc, char *argv[])
{
FILE *in;
char l[1000];
char r[1000];
	if (argc == 1)
		in = stdin;
	else
	in = fopen (argv[1], "r");
	if (in == NULL)
	{
		perror (argv[1]);
		return errno;
	}
	for (;;)
	{
		fgets (l, sizeof l, in);
		if (feof(in))
		{
			fclose (in);
			return 0;
		}
		l[strlen(l)-1] = 0;
		preproc (r, l);
		printf ("%s\n", r);
	}
}

