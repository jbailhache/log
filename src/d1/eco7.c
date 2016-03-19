
#include "stdio.h"
#include "graphics.h"

float x0, x1, Dx, x, y, scale;

lect ()
{
	printf ("x0 ? ");	scanf ("%f", &x0);
	printf ("x1 ? ");	scanf ("%f", &x1);
	Dx = x1 - x0;
	printf ("scale ? ");	scanf ("%f", &scale);

}

float f (float x)
{
float r;
	r = x*x - 3*x + 1;
	return r;
}

#define MAX_ABCISSE 600
#define MAX_ORDONNEE 250

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

graph ()
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