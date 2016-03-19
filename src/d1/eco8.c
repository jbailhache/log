
#include "math.h"
#include "stdio.h"
#include "graphics.h"

float x0, x1, Dx, x, y, scale;

float y0, y1, Dy, scale_q, uq;

lect ()
{
	printf ("x0 ? ");	scanf ("%f", &x0);
	printf ("x1 ? ");	scanf ("%f", &x1);
	Dx = x1 - x0;
	printf ("y0 ? ");	scanf ("%f", &y0);
	printf ("y1 ? ");	scanf ("%f", &y1);
	Dy = y1 - y0;

	/*
	printf ("scale ? ");	scanf ("%f", &scale);
	printf ("scale q ? ");	scanf ("%f", &scale_q); */
	printf ("unite q ? "); 	scanf ("%f", &uq);


}

float f (float x)
{
float r;
	r = x*x - 3*x + 1;
	return r;
}

#define MAX_ABCISSE 200 /*600*/
#define MAX_ORDONNEE 80 /*250*/

graph1 ()
{
int c, l;
	for (c=0; c<MAX_ABCISSE; c++)
	{
		x = x0 + Dx * c/MAX_ABCISSE;
		y = f(x);
		l = MAX_ORDONNEE - scale * y;
		putpixel (c, l, 7);
	}
}

float Cv, Cf, CT, CM, CVM, Cm, CT1, x1, CM1, CVM1;

graph2 ()
{
int l, c;
int flag_CM, flag_CVM;
char buf [200];

	flag_CM = flag_CVM = 0;

	for (c=0; c<MAX_ABCISSE; c++)
	{
		x = x0 + Dx * c/MAX_ABCISSE;
		putpixel (c, MAX_ORDONNEE, 7);
		Cv = 0.5 * x * x * x - x * x + 4 * x;
		Cf = 4;
		CT = Cv + Cf;
		CM = CT / x;
		CVM = Cv / x;
		Cm = (CT - CT1) / (x - x1);

		putpixel (c, MAX_ORDONNEE - scale * Cv, 1);
		putpixel (c, MAX_ORDONNEE - scale * Cf, 1);
		putpixel (c, MAX_ORDONNEE - scale * CT, 1);
		putpixel (c, MAX_ORDONNEE - scale * CM, 1);
		putpixel (c, MAX_ORDONNEE - scale * CVM, 1);
		putpixel (c, MAX_ORDONNEE - scale * Cm, 1);

		if (c == MAX_ABCISSE / 2)
		{
			outtextxy (c, MAX_ORDONNEE - scale * Cv, "Cv");
			outtextxy (c, MAX_ORDONNEE - scale * Cf, "Cf");
			outtextxy (c, MAX_ORDONNEE - scale * CT, "CT");
			outtextxy (c, MAX_ORDONNEE - scale * CM, "CM");
			outtextxy (c, MAX_ORDONNEE - scale * CVM, "CVM");
			outtextxy (c, MAX_ORDONNEE - scale * Cm, "Cm");

		}

		if (!flag_CM && c>1)
		{
			if (CM >= CM1)
			{
				sprintf (buf, "(%1.1f,%1.1f)", CM, x);
				outtextxy (c, MAX_ORDONNEE - scale * CM, buf);
				flag_CM = 1;
			}
		}

		if (!flag_CVM && c>1)
		{
			if (CVM >= CVM1)
			{
				sprintf (buf, "(%1.1f,%1.1f)", CVM, x);
				outtextxy (c, MAX_ORDONNEE - scale * CVM, buf);
				flag_CVM = 1;
			}
		}

		CT1 = CT;
		x1 = x;
		CM1 = CM;
		CVM1 = CVM;

	}
}

#define plot(x,y) putpixel (x, MAX_ORDONNEE-(y), 1)

float q, sq, fq, a, b, q1, C, d, dqx, dqy, dCx, dCy;

float t[10];

float puissance (float x, float y)
{
	t[1] = log (x);
	t[2] = t[1] * y;
	t[3] = exp (t[2]);
	return t[3];
}

float q_ (float x, float y)
{
float q;
	q = pow (x, 0.5) * pow (y, 1.0 / 3.0);
	return q;
}

float C_ (float x, float y)
{
float C;
	C = 10.0 * x + 20.0 * y;
	return C;
}

#define EPS 0.01

graph ()
{
int c, l, c1, l1;
int flag_q;

	flag_q = 0;

	for (c=0; c<MAX_ABCISSE; c++)
		plot (c, 0);

	for (l=0; l<MAX_ORDONNEE; l++)
		plot (0, l);


	for (c=0; c<MAX_ABCISSE; c++)
		for (l=0; l<MAX_ORDONNEE; l++)
		{
			x = x0 + Dx * c/MAX_ABCISSE;
			y = y0 + Dy * l/MAX_ORDONNEE;
#if 1
			/* q = pow (x, 0.5) * pow (y, 1.0 / 3.0); */
			q = q_ (x, y);

			/* a = (float)pow ((double)x, (double)0.5); */
			/*
			a = puissance (x, 0.5);
			b = pow (y, 0.333333);
			q = a * b;
			*/
			sq = q / uq;
			fq = sq - (float)(long)sq;
			if (fq < 0.2)
				plot (c, l);
#endif
			C = C_ (x, y);

			if (fabs (C - 150.0) < 1.0)
			{
				plot (c, l);
				if (c > 1 && !flag_q && q <= q1 - 0.02)
				{
					for (c1=0; c1<MAX_ABCISSE; c1++)
						plot (c1, l);
					for (l1=0; l1<MAX_ORDONNEE; l1++)
						plot (c, l1);
					flag_q = 1;
				}
				q1 = q;
			}

			/* c = 10.0 * x + 20.0 * y; */


			dqx = (q - q_ (x+EPS, y)) / EPS;
			dqy = (q - q_ (x, y+EPS)) / EPS;
			dCx = (C - C_ (x+EPS, y)) / EPS;
			dCy = (C - C_ (x, y+EPS)) / EPS;

			d = dqx * dCy - dqy * dCx;
			if (fabs (d) < 0.2)
				plot (c, l);


		}

	for (c=0; c<MAX_ABCISSE; c++)
	{
		x = x0 + Dx * c/MAX_ABCISSE;
/*
		y = f(x);
		l = MAX_ORDONNEE - scale * y;
		putpixel (c, l, 7);
*/

	}

}

main()
{
int driver, mode;
int status;
char buf[1000];
int i;

	driver=HERCMONO;
	mode=HERCMONOHI;

	lect ();
	/*
	driver = VGA;
	mode = VGAHI;
	*/
	initgraph (&driver, &mode, "");
	status = graphresult ();
	if (status != grOk)
		printf ("Erreur initgraph %d 0x%X\n", status, status);
	/*
	putpixel (120, 130, 1);
	for (i=0; i<200; i++)
		putpixel (i, i, i);
	*/
	/* lect (); */
	graph ();
	gets (buf);
	gets (buf);
	restorecrtmode ();
}

