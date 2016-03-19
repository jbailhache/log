
#include <stdio.h>
#include <math.h>
#include <graphics.h>

#define INVERSION

#define MAX 1000

/* float cours [MAX]; */

struct s_cours { char *nom; float cours[120]; };

struct s_cours tab_cours[] = {
/* Castorama */
/* float cours[] = */
 { "Castorama", { 248.8, 248.8, 249, 249, 249, 249.5, 250, 249.5, 249.5, 250, 249.5, 250, 249.7, 249.5, 249.4,
	249.3, 249.6, 249.3, 249.7, 249.1, 249.7, 249.7, 249.8, 250, 250, 250, 250.4, 250.5, 250.9, 250.9, 250.4, 249.4, 249, 249, 48.8, 248.6, 248.4, 248.2,
	248, 248, 247.5, 247.5, 247.1, 246.9, 247.1, 247.5, 248, 247.5, 247, 247.5, 246.9, 247.5, 247.7, 248, 248,
	248, 248, 247.6, 247.2, 247.1, 248, 247.5, 248, 248, 248, 248.4, 249, 248.3, 248, 248.3, 248.7, 0 }},

 { "Bull 30-8",
  { 7.44, 7.44, 7.44, 7.42, 7.41, 7.44, 7.44, 7.42, 7.45, 7.44, 7.44, 7.44, 7.42,
	7.45, 7.44, 7.45, 7.44, 7.47, 7.48, 7.48, 7.5, 7.5, 7.46, 7.48, 7.5, 7.48, 7.5, 7.5, 7.51, 7.5, 7.49, 7.5, 7.48,
	7.5, 7.49, 7.48, 7.47, 7.46, 7.47, 7.5, 7.49, 7.5, 7.52, 7.53, 7.52, 7.55, 7.5, 7.54, 7.52, 7.53, 7.5,
	7.53, 7.51, 7.54, 0 }},

 { "CFF 30-8",
  { 13.77, 13.74, 13.71, 13.68, 13.65, 13.62,
	13.6, 13.72, 13.84, 13.6, 13.84, 13.6, 13.6, 13.62,
	13.66, 13.7, 13.74, 13.78, 13.82, 13.84, 13.8,
	13.7, 13.69, 13.68, 13.67, 13.66, 13.65, 13.64,
	13.63, 13.62, 13.61, 13.6, 13.59, 13.58,
	13.5, 13.5, 13.5, 13.5, 13.5, 13.4,
	13.3, 13.3, 13.31, 13.32, 0 }},

 { "Metaleurop 30-8",
  { 7.4, 7.39, 7.395, 7.4, 7.41, 7.42, 7.43, 7.44, 7.44, 7.42, 7.4, 7.41, 7.413,
	7.414, 7.415, 7.416, 7.417, 7.418, 7.419, 7.42,
	7.4, 7.4, 7.42, 7.44, 7.41, 7.425, 7.44, 7.41, 7.44, 7.44, 7.42, 7.415,
	7.41, 7.415, 7.42, 7.425, 7.43,
	7.435, 7.44, 7.445, 7.45, 7.43,
	7.44, 7.45, 7.43, 7.41, 7.41, 7.425, 7.44, 7.41, 0 }},


 { "Trader 30-8",
  {13, 12.95, 12.9, 12.93, 12.97, 12.94, 12.91, 13.09, 13, 12.92,
	12.92, 13.09, 13.08, 13.05, 13, 13, 13,
	13.05, 13.025, 13, 13, 13, 12.96, 12.96, 12.91,
	12.91, 12.91, 12.95, 13, 13.04, 13,
	12.95, 12.91, 12.91, 12.91, 12.93, 12.95, 12.96, 12.95, 12.91, 12.91,
	12.92, 12.92, 12.98, 13.04, 12.98, 12.92, 12.935, 12.95, 0 }} };




int n;

#define Euro 6.55
/* #define capital_init (2000000/Euro) */
#define frais 0 /*18*/
#define fraisp 0
#define dt 0

FILE *out;

float cont (struct s_cours *pcours)
#define cours (pcours->cours)
{
int t;
float s;
int n;
	s = 0;
	for (t=1; cours[t+1]>0; t++)
	{
		s += ((cours[t] - cours[t-1]) * (cours[t+1] - cours[t])) / (cours[t] * cours[t]);
		n++;
	}
	return s / n;
}

float discont (struct s_cours *pcours)
{
int t;
float s;
int n;
float d;
	s = 0;
	n = 0;
	for (t=1; cours[t+1]>0; t++)
	{
		n++;
		d = (cours[t-1] - 2 * cours[t] + cours[t+1]) / cours[t];
		s += d * d;
	}
	return s / n;
}

simul (float d, float capital_init, struct s_cours *pcours)
{
long t, nt, nta;
float capital;

	/* printf ("\n%s\n", pcours->nom); */
	capital = capital_init;
	nt = 0;

	for (t=1; /* t<n-1 */ cours[t+1] > 0; t++)
	{
#ifndef INVERSION
		if ((cours[t] - cours[t-1]) > 0.02) /* ‡a monte : acheter */
#else
		if ((cours[t-1] - cours[t]) > d)
#endif
		{
			nta = (capital - frais) / (cours[t] * 1.1);
			if (nta > 0)
			{
				capital -= nta * cours[t+dt/*+1*/];
				capital -= frais;
				capital -= fraisp * nta * cours[t+dt];
				nt += nta;
				fprintf (out, "%3ld : Achat %6ld … %6.2f -> %6ld %10.2f %10.2f\n",
					t, nta, cours[t+dt], nt, capital, capital+nt*cours[t+dt]);
			}
		}
#ifndef INVERSION
		else if (((cours[t-1] - cours[t]) > 0.02) && (nt > 0)) /* ‡a baisse : vendre */
#else
		else if (((cours[t] - cours[t-1]) > d) && (nt > 0))
#endif
		{
			capital += nt * cours[t+dt];
			capital -= frais;
			capital -= fraisp * nt * cours[t+dt];
			fprintf (out, "%3ld : Vente %6ld … %6.2f -> %6ld %10.2f %10.2f\n",
				t, nt, cours[t+dt], 0L, capital, capital+0*cours[t+dt]);
			nt = 0;
		}
	}

	capital += nt * cours[/*n-1*/t];
	printf ("capital %10.2f d=%6.4f Rendement : %f\n",
		capital_init, d, capital / capital_init);
/*	printf ("On a gagn‚ %f francs pour %f francs investis soit %f %%\n",
		(capital - capital_init) * Euro, capital_init * Euro,
		(capital - capital_init) / capital_init * 100 );
*/
}

main ()
{
char filename[200];
FILE *f;
float x;
int i;
int driver, mode, status;
float c, d;
float co, dis;

	out = fopen ("out.lis", "w");
	if (out == NULL)
	{
		perror ("out.lis");
		out = stdout;
	}
/*
	printf ("Fichier cours? ");
	gets (filename);
	f = fopen (filename, "r");
	if (f == NULL)
	{
		perror (filename);
		return;
	}

	for (i=0; i<MAX; i++)
	{
		if (feof(f))
			break;
		fscanf (f, "%f", &x);
		cours[i] = x;
	}

	n = i;
*/
	/* n = sizeof (cours) / sizeof (cours[0]); */

/*
	for (i=0; i<n; i++)
	{
		printf ("%04d : %4.2f\n", i, cours[i]);
	}
*/

/*
	driver = DETECT;
	mode = 0;

	initgraph (&driver, &mode, "c:\\tc");
	status = graphresult ();
	if (status != grOk)
		printf ("Erreur initgraph %d 0x%X\n", status, status);

	for (i=0; i<n; i++)
		putpixel (i+1, (8-cours[i])*100, 10);

	restorecrtmode ();
*/

/*
	for (d=0.001; d<0.1; d*=1.1)
		simul (d);

*/

#if 1
	for (i=0; i<sizeof(tab_cours)/sizeof(tab_cours[0]); i++)
	{
		co = cont (tab_cours+i);
		dis = discont (tab_cours+i);
		printf ("\n%s %f %f\n", tab_cours[i].nom, co, dis);
		/*for (c=1000/Euro; c<2000000/Euro; c*=2)
			simul (0.045, c, tab_cours+i);*/
		for (c=1000/Euro; c<2000000/Euro; c*=4)
		for (d=0.001; d<0.1; d*=2)
			simul (d, c, tab_cours+i);
	}
#else
/*
	i=0;
	c=2442;
	d=0.01;
	simul (d, c, tab_cours+i);
*/
	simul (0.01, 152.67, tab_cours+4);
#endif
	fflush (out);

}
