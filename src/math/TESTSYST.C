
#include <stdio.h>

#define DIM 40

main ()
{
register i, j;
int n, l;
int G_syst ();
double a[DIM][DIM], b[DIM], x[DIM];

	printf ("Ordre de la matrice ? ");
	scanf ("%d", &n);

	printf ("Elements de la matrice ? \n");
	for (i=0; i<n; i++)
	for (j=0; j<n; j++)
		scanf ("%lf", &a[i][j]);

	printf ("Seconds membres ? \n");
	for (i=0; i<n; i++)
		scanf ("%lf", &b[i]);

	l = G_syst (a, b, n, DIM, x);

	if (l)
	{
		printf ("Solution : \n");
		for (i=0; i<n; i++)
			printf ("%8.3g", x[i]);
		/* putchar ('\n'); */
		printf ("\n");
	}
	else
		printf ("On ne peut resoudre le systeme\n");
}

