
#include <stdio.h>
#include <errno.h>

struct param_function
{
	int (*f) ();
	void *p;
};

typedef struct param_function *pfunc;


struct param
{
	FILE *fh;
};

putstr (pfunc put, char *s)
{
	while (*s)
		(*(put->f)) (put->p, *s++);
}

int fget (struct param *p)
{
int c;
	c = getc (p->fh);
	return c;
}

int fput (struct param *p, char c)
{
	putc (c, p->fh);
}

main1 (int argc, char *argv[], int (*f) ())
{
struct param_function get, put;
struct param pget, pput;

	get.f = fget; get.p = &pget;
	put.f = fput; put.p = &pput;

	pget.fh = fopen (argv[1], "rb");
	if (pget.fh == NULL)
	{
		perror (argv[1]);
		return errno;
	}

	pput.fh = fopen (argv[2], "wb");
	if (pput.fh == NULL)
	{
		perror (argv[2]);
		return errno;
	}

	printf ("Converting...\n");
	(*f) (&get, &put);
	printf ("Done.\n");

	fclose (pget.fh);
	fclose (pput.fh);
}

