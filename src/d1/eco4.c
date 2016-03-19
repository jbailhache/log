
#include "stdio.h"
#include "graphics.h"
#include "math.h"

float alpha, beta, a, y, n, n0, n1, Dn, k, scale, scale_d, scale_PI, p, w, C,
	n_opt, w0, w1, Dw, scale_n;

float f (float n, float k)
{
float na, kb, y;
	na = pow (n, alpha);
	kb = pow (k, beta);
	y = a * na * kb;
	return y;
	/* return a * pow (n, alpha) * pow (k, beta); */
}


#define MAX_ABCISSE 500
#define MAX_ORDONNEE 200

graph ()
{
int X, Y, dY;
float y1, n1, PI;
	y1 = 0;
	n1 = -1.0;
	for (X=0; X<MAX_ABCISSE; X++)
	{
		n = n0 + Dn * (float)X / (float)MAX_ABCISSE;
		y = f (n, k);
		Y = scale * y;
		dY = scale_d * (y - y1) / (n - n1);
		putpixel (X, MAX_ORDONNEE - Y, 1);
		if (X%2)
			putpixel (X, MAX_ORDONNEE - dY, 1);
		putpixel (X, MAX_ORDONNEE - scale_d * w / p, 1);
		PI = p * y - w * n - C;
		putpixel (X, MAX_ORDONNEE - scale_PI * PI, 1);
		/* if (fabs (n - p * alpha * y / w) < 0.001) */
		if (fabs (n - n_opt) < 0.001)
		{
			for (Y=0; Y<MAX_ORDONNEE; Y++)
				putpixel (X, Y, 1);
		}
		y1 = y;
		n1 = n;
	}
}

graph_w_n ()
{
int X;
	for (X=0; X<MAX_ABCISSE; X++)
	{
		w = w0 + Dw * (float)X / (float)MAX_ABCISSE;
		n = pow (w / (p * alpha * a * pow(k,beta)), 1 / (alpha - 1));
		putpixel (X, MAX_ORDONNEE - scale_n * n, 1);

	}
}


main()
{
int driver, mode;
int status;
char buf[1000];

	printf ("a ? ");
	scanf ("%f", &a);

	printf ("alpha ? ");
	scanf ("%f", &alpha);

	printf ("k ? ");
	scanf ("%f", &k);

	printf ("quantite de travail initiale ? ");
	scanf ("%f", &n0);

	printf ("quantite de travail finale ? ");
	scanf ("%f", &n1);

	Dn = n1 - n0;

	printf ("Echelle y ? ");
	scanf ("%f", &scale);

	printf ("Echelle derivee ? ");
	scanf ("%f", &scale_d);

	printf ("Echelle profit ? ");
	scanf ("%f", &scale_PI);

	printf ("prix ? ");
	scanf ("%f", &p);

	printf ("salaire ? ");
	scanf ("%f", &w);

	printf ("salaire initial ? ");
	scanf ("%f", &w0);

	printf ("salaire final ? ");
	scanf ("%f", &w1);

	Dw = w1 - w0;

	printf ("Echelle n ? ");
	scanf ("%f", &scale_n);

	printf ("couts fixes ? ");
	scanf ("%f", &C);

/*
	printf ("beta ? ");
	scanf ("%f", &beta);
*/

	beta = 1.0 - alpha;

	n_opt = pow (w / (p * alpha * a * pow(k,beta)), 1 / (alpha - 1));

	driver=HERCMONO;
	mode=HERCMONOHI;
/*
	printf ("Driver ? ");
	scanf ("%d", &driver);
	printf ("Mode ? ");
	scanf ("%d", &mode);
*/
	initgraph (&driver, &mode, "");
	status = graphresult ();
	if (status != grOk)
	{
		printf ("Erreur initgraph %d 0x%X\n", status, status);
		return;
	}
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
	graph ();

	gets (buf);
	/* gets (buf); */
	/* while (getch() != 'q'); */
	/* sleep (1); */
	/* bioskey (0); */
	bioskey (0);

	cleardevice ();
	graph_w_n ();
	bioskey (0);

	restorecrtmode ();
	printf ("status = %d\n", status);
}

