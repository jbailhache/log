
#include <stdio.h>
#include <math.h>
#include <graphics.h>

#define MAX 1000

/* float cours [MAX]; */

/* Castorama
float cours[] = { 248.8, 248.8, 249, 249, 249, 249.5, 250, 249.5, 249.5, 250, 249.5, 250, 249.7, 249.5, 249.4,
	249.3, 249.6, 249.3, 249.7, 249.1, 249.7, 249.7, 249.8, 250, 250, 250, 250.4, 250.5, 250.9, 250.9, 250.4, 249.4, 249, 249, 48.8, 248.6, 248.4, 248.2,
	248, 248, 247.5, 247.5, 247.1, 246.9, 247.1, 247.5, 248, 247.5, 247, 247.5, 246.9, 247.5, 247.7, 248, 248,
	248, 248, 247.6, 247.2, 247.1, 248, 247.5, 248, 248, 248, 248.4, 249, 248.3, 248, 248.3, 248.7 };
*/

/* Bull 30-8
float cours[] = { 7.44, 7.44, 7.44, 7.42, 7.41, 7.44, 7.44, 7.42, 7.45, 7.44, 7.44, 7.44, 7.42,
	7.45, 7.44, 7.45, 7.44, 7.47, 7.48, 7.48, 7.5, 7.5, 7.46, 7.48, 7.5, 7.48, 7.5, 7.5, 7.51, 7.5, 7.49, 7.5, 7.48,
	7.5, 7.49, 7.48, 7.47, 7.46, 7.47, 7.5, 7.49, 7.5, 7.52, 7.53, 7.52, 7.55, 7.5, 7.54, 7.52, 7.53, 7.5,
	7.53, 7.51, 7.54 };
*/

/* CFF 30-8 
float cours[] = { 13.77, 13.74, 13.71, 13.68, 13.65, 13.62,
	13.6, 13.72, 13.84, 13.6, 13.84, 13.6, 13.6, 13.62,
	13.66, 13.7, 13.74, 13.78, 13.82, 13.84, 13.8,
	13.7, 13.69, 13.68, 13.67, 13.66, 13.65, 13.64,
	13.63, 13.62, 13.61, 13.6, 13.59, 13.58,
	13.5, 13.5, 13.5, 13.5, 13.5, 13.4,
	13.3, 13.3, 13.31, 13.32 };
*/

/* Metaleurop 30-8 
float cours [] = { 7.4, 7.39, 7.395, 7.4, 7.41, 7.42, 7.43, 7.44, 7.44, 7.42, 7.4, 7.41, 7.413,
	7.414, 7.415, 7.416, 7.417, 7.418, 7.419, 7.42, 
	7.4, 7.4, 7.42, 7.44, 7.41, 7.425, 7.44, 7.41, 7.44, 7.44, 7.42, 7.415,
	7.41, 7.415, 7.42, 7.425, 7.43,
	7.435, 7.44, 7.445, 7.45, 7.43,
	7.44, 7.45, 7.43, 7.41, 7.41, 7.425, 7.44, 7.41 };
	*/

/* Trader 30-8 */

float cours[] = {13, 12.95, 12.9, 12.93, 12.97, 12.94, 12.91, 13.09, 13, 12.92,
	12.92, 13.09, 13.08, 13.05, 13, 13, 13,
	13.05, 13.025, 13, 13, 13, 12.96, 12.96, 12.91,
	12.91, 12.91, 12.95, 13, 13.04, 13,
	12.95, 12.91, 12.91, 12.91, 12.93, 12.95, 12.96, 12.95, 12.91, 12.91,
	12.92, 12.92, 12.98, 13.04, 12.98, 12.92, 12.935, 12.95 };




int n;

#define Euro 6.55
#define capital_init (2000000/Euro)
#define frais 18
#define fraisp 0

FILE *out;

simul ()
{
long t, nt, nta;
float capital;

	capital = capital_init;
	nt = 0;

	for (t=1; t<n-1; t++)
	{
		if ((cours[t] - cours[t-1]) > 0.02) /* ‡a monte : acheter */
		{
			nta = (capital - frais) / (cours[t] * 1.1);
			if (nta > 0)
			{
				capital -= nta * cours[t+1];
				capital -= frais;
				capital -= fraisp * nta * cours[t+1];
				nt += nta;
				fprintf (out, "%3ld : Achat %6ld … %6.2f -> %6ld %10.2f %10.2f\n",
					t, nta, cours[t+1], nt, capital, capital+nt*cours[t+1]);
			}
		}
		else if (((cours[t-1] - cours[t]) > 0.02) && (nt > 0)) /* ‡a baisse : vendre */
		{
			capital += nt * cours[t+1];
			capital -= frais;
			capital -= fraisp * nt * cours[t+1];
			fprintf (out, "%3ld : Vente %6ld … %6.2f -> %6ld %10.2f %10.2f\n",
				t, nt, cours[t+1], 0L, capital, capital+0*cours[t+1]);
			nt = 0;
		}
	}

	capital += nt * cours[n-1];
	printf ("Rendement : %f\n", capital / capital_init);
	printf ("On a gagn‚ %f francs pour %f francs investis soit %f %%\n",
		(capital - capital_init) * Euro, capital_init * Euro,
		(capital - capital_init) / capital_init * 100 );

}

main ()
{
char filename[200];
FILE *f;
float x;
int i;
int driver, mode, status;

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
	n = sizeof (cours) / sizeof (cours[0]);

	for (i=0; i<n; i++)
	{
		printf ("%04d : %4.2f\n", i, cours[i]);
	}

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

	simul ();

	fflush (out);

}
