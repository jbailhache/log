
#include "stdio.h"
#include "graphics.h"
#include "stdlib.h"
#include "math.h"

#define GRAPH

#define MAX_X 300
#define MAX_Y 200

int dz = 100;
float lambda = 0.1;

#define NSOURCES 2

int sources[NSOURCES][2] = { { 100, 80 }, { 200, 110 } };

plot (int x, int y, float a)
{
#ifdef GRAPH
	if (random(100) > a*a*20)
		putpixel (x, y, 1);
#endif
}

interf ()
{
long x, y, dx, dy, i, d2;
float a, d1, fd2;
	for (x=1; x<MAX_X; x++)
	for (y=1; y<MAX_Y; y++)
	{
#ifdef TRACE
		printf ("%ld %ld\n", x, y);
#endif
		a = 0;
		for (i=0; i<NSOURCES; i++)
		{
			dx = x - sources[i][0];
			dy = y - sources[i][1];
			d2 = dx*dx + dy*dy + dz*dz;
#ifdef TRACE
			printf ("%ld\n", d2);
#endif
			fd2 = (float) d2;
			d1 = sqrt (fd2);
			a += sin (lambda * d1);
		}
		plot (x, y, a);
	}
}

main()
{
int driver, mode;
int status;
char buf[1000];

	driver=HERCMONO;
	mode=HERCMONOHI;
/*
	printf ("Driver ? ");
	scanf ("%d", &driver);
	printf ("Mode ? ");
	scanf ("%d", &mode);
*/
#ifdef GRAPH
	initgraph (&driver, &mode, "");
	status = graphresult ();
	if (status != grOk)
	{
		printf ("Erreur initgraph %d 0x%X\n", status, status);
		return;
	}
#endif
/*
	putpixel (120, 130, 1);
	{
	int i;
		for (i=0; i<150; i++)
		{
			putpixel (i, i, 1);
		}
	}
*/
	interf ();

	gets (buf);
	sleep (1);
	restorecrtmode ();
	printf ("status = %d\n", status);
}

