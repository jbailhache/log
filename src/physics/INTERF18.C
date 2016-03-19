
#include "stdio.h"
#include "graphics.h"
#include "stdlib.h"
#include "math.h"

#define GRAPH

#define DIFFRACT

/* #define TABSOURCES */

#define REDUC 1.5

#define MAX_X (300/REDUC)
#define MAX_Y (200/REDUC)

float dz = 20/REDUC;
float lambda = /*1*/ /*3*/ /*10*/ /*0.3*/ 0.4/REDUC;
#define xs1 (100/REDUC)
#define ys1 (80/REDUC)
#define rs (1/REDUC)
#define esp (0.02/REDUC)

#ifndef DIFFRACT
#define NSOURCES 2

int sources[NSOURCES][2] = { { 100, 80 }, { 200, 110 } };

#else
#define MAXSOURCES 5000

float sources[MAXSOURCES][2];

int NSOURCES;
#endif

defsources ()
{
#ifdef DIFFRACT
float x, y, dx, dy, d2;
long i;
float d1;

	i = 0;
	/*for (x=1; x<MAX_X; x++)
	for (y=1; y<MAX_Y; y++)*/
	for (x=xs1-rs; x<xs1+rs; x+=esp)
	for (y=ys1-rs; y<ys1+rs; y+=esp)
	{
		dx = x - xs1;
		dy = y - ys1;
		d2 = dx*dx + dy*dy;
		d1 = sqrt((float)d2);
		if (d1 <= rs)
		{
#ifdef TABSOURCES
			sources[i][0] = x;
			sources[i][1] = y;
#endif
			i++;
		}
/*
		dx = x - 200;
		dy = y - 110;
		d2 = dx*dx + dy*dy;
		d1 = sqrt((float)d2);
		if (d1 <= 1.5)
		{
			sources[i][0] = x;
			sources[i][1] = y;
			i++;
		}
*/
	}
	NSOURCES = i;
#endif
}

plot (int x, int y, float a)
{
	putpixel (x, y, a*16/NSOURCES);
}

plot1 (int x, int y, float a)
{
#ifdef GRAPH
	/* if (random(100) > a*a*25) */
	/* if (a*a > 2) */
#ifdef DIFFRACT
	/* if (2*a > NSOURCES) */
	/* if (1.01*a > NSOURCES) */
	/* if (a*2 > NSOURCES) */
	/* if (a*20 > random (NSOURCES*20)) */
	/* if (a < 6.5) */
	/* if (a < 1.5) */
	/* if (a < 3) */
	if (a < 1)
#else
	if (a*2 > NSOURCES)
#endif
	{
		putpixel (x, y, 1);
	}
#endif
}

interf ()
{
long x, y, dx, dy, i, d2;
float a, b, d1, fd2;
float xs, ys, dxs, dys, d1s, d2s;


	for (y=1; y<MAX_Y; y++)
	for (x=1; x<MAX_X; x++)
	{
#ifdef TRACE
		printf ("%ld %ld\n", x, y);
#endif
		a = 0;
		b = 0;
#ifdef TABSOURCES
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
#else
		for (xs=xs1-rs; xs<xs1+rs; xs+=esp)
		for (ys=ys1-rs; ys<ys1+rs; ys+=esp)
		{
			dxs = xs - xs1;
			dys = ys - ys1;
			d2s = dxs*dxs + dys*dys;
			d1s = sqrt((float)d2s);
			if (d1s <= rs)
			{
				/*
				sources[i][0] = x;
				sources[i][1] = y;
				i++;
				*/
				dx = x - xs;
				dy = y - ys;
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

		}
#endif
		plot (x, y, sqrt (a*a + b*b));
	}
}

main()
{
int driver, mode;
int status;
char buf[1000];

	defsources ();

/*
	driver=HERCMONO;
	mode=HERCMONOHI;
*/
/*
	printf ("Driver ? ");
	scanf ("%d", &driver);
	printf ("Mode ? ");
	scanf ("%d", &mode);
*/
	detectgraph (&driver, &mode);
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