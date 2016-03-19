
#include "stdio.h"
#include "graphics.h"
#include "stdlib.h"
#include "math.h"

#define GRAPH

#define MAX_X 300
#define MAX_Y 200

int dz = 100;
float lambda = /*1*/ 3 /*10*/;


#define NSOURCES 2

int sources[NSOURCES][2] = { { 100, 80 }, { 200, 110 } };

/*
#define MAXSOURCES 200

int sources[MAXSOURCES][2];

int NSOURCES;
*/

defsources ()
{
/*
long x, y, dx, dy, i, d2;
float d1;

	i = 0;
	for (x=1; x<MAX_X; x++)
	for (y=1; y<MAX_Y; y++)
	{
		dx = x - 100;
		dy = y - 80;
		d2 = dx*dx + dy*dy;
		d1 = sqrt((float)d2);
		if (d1 <= 2.1)
		{
			sources[i][0] = x;
			sources[i][1] = y;
			i++;
		}
	}
	NSOURCES = i;
*/
}

plot (int x, int y, float a)
{
#ifdef GRAPH
	/* if (random(100) > a*a*25) */
	/* if (a*a > 2) */
	/* if (2*a > NSOURCES) */
	if (1.07*a > NSOURCES)
	{
		putpixel (x, y/2, 1);
	}
#endif
}

interf ()
{
long x, y, dx, dy, i, d2;
float a, b, d1, fd2;
	for (y=2; y<MAX_Y; y+=2)
	for (x=1; x<MAX_X; x++)
	{
#ifdef TRACE
		printf ("%ld %ld\n", x, y);
#endif
		a = 0;
		b = 0;
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
#ifdef TRACE
			printf ("%f\n", d1);
#endif
			a += sin (d1 / lambda);
			b += cos (d1 / lambda);
		}
		plot (x, y, sqrt (a*a + b*b));
	}
}

main()
{
int driver, mode;
int status;
char buf[1000];

	defsources ();

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