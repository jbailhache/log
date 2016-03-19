#include <stdio.h>
#include <graphics.h>
#include <math.h>

#define float double
#define int double

int fact (int n, int p)
{
int f;
int i;
	f = 1;
	for (i=n; i>p; i--)
		f *= i;
	return f;
}

float proba (int n, int q, float p)
{
float pr;
float t[10];
	/* pr = pow(p,q) * pow(1-p,q) * fact(n,n-q) / fact(q,0); */
	t[1] = pow (p, q);
	t[2] = pow (1-p, n-q);
	t[3] = fact (n, n-q);
	t[4] = fact (q, 0);
	t[5] = t[1] * t[2] * t[3] / t[4];
	return t[5];
}

float probasp (int n, int q, float p)
{
float s;
int q1;
	s = 0;
	for (q1 = q; q1 <= n; q1++)
		s += proba (n, q1, p);
	return s;
}

float probasm (int n, int q, float p)
{
float s;
int q1;
	s = 0;
	for (q1 = 0; q1 <= q; q1++)
		s += proba (n, q1, p);
	return s;
}

#define LARGEUR 400
#define HAUTEUR 200

int n, q;

graph ()
{
int i, j;
float x, y;
	for (i=1; i<=LARGEUR; i++)
	{
		putpixel (i, 1, 1);
		putpixel (i, HAUTEUR+1, 1);
	}
	for (i=1; i<=HAUTEUR; i++)
	{
		putpixel (1, i, 1);
		putpixel (LARGEUR+1, i, 1);
	}
	for (i=1; i<=LARGEUR-1; i++)
	{
		x = (float)i / (float)LARGEUR;

		y = probasp (n, q, x);
		j = HAUTEUR + 1 - HAUTEUR * y;
		putpixel (i, j, 1);

		y = probasm (n, q, x);
		j = HAUTEUR + 1 - HAUTEUR * y;
		putpixel (i, j, 1);

		y = proba (n, q, x);
		j = HAUTEUR + 1 - HAUTEUR * y;
		putpixel (i, j, 1);
	}

}

main()
{
int driver, mode;
int status;
char buf[1000];

	printf ("Nombre d'experiences ? ");
	scanf ("%lf", &n);
	printf ("Nombre de resultats positifs ? ");
	scanf ("%lf", &q);

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
	gets (buf);
	sleep (1);
	restorecrtmode ();
	printf ("status = %d\n", status);
}



