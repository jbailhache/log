
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define DIM 30

main ()
{
register i, j, k;
int n, l;
int LU();
double a[DIM][DIM], lower[DIM][DIM], upper[DIM][DIM], resul[DIM][DIM];
int x;

	printf ("Ordre de la matrice ? ");
	scanf ("%d", &n);

	printf ("Elements ?\n");
	for (i=0; i<n; i++)
	for (j=0; j<n; j++)
	{
		/* scanf ("%lf", &a[i][j]); */
		scanf ("%d", &x);
		a[i][j] = x;
	}

	for (i=0; i<n; i++)
	for (j=0; j<n; j++)
		lower[i][j] = upper[i][j] = 0;

	l = LU (a, n, DIM, lower, upper);

	if (l)
	{
		printf ("Lower : \n");
		for (i=0; i<n; i++)
		{
			for (j=0; j<n; j++)
				printf ("%10.3g", lower[i][j]);
			putchar ('\n');
		}
		printf ("\nUpper : \n");
		for (i=0; i<n; i++)
		{
			for (j=0; j<n; j++)
				printf ("%10.3g", upper[i][j]);
			putchar ('\n');
		}
	}
	else
		printf ("La matrice n'a pu etre decomposee.\n");
}




