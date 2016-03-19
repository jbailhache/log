
#include <stdio.h>

#define DIM 15

main ()
{
int n, i, j, it, k;
double mat[DIM][DIM], val[DIM], vect[DIM][DIM];
int LU_Rutishauser(), vect_propre();

	printf ("Ordre de la matrice ? ");
	scanf ("%d", &n);

	printf ("Elements de la matrice par lignes ? ");
	for (i=0; i<n; i++)
	for (j=0; j<n; j++)
		scanf ("%lf", &mat[i][j]);

	printf ("Nombre d'iterations ? ");
	scanf ("%d", &it);

	printf ("La matrice a traiter est : \n");
	for (i=0; i<n; i++)
	{
		for (j=0; j<n; j++)
			printf ("%10g", mat[i][j]);
		putchar ('\n');
	}
	printf ("\nLe nombre d'iterations est%5d\n", it);

	if (LU_Rutishauser (mat, n, DIM, it, val))
	{
		printf ("\nLes valeurs propres sont : \n");
		for (i=0; i<n; i++)
			printf (" \t %12g\n", val[i]);
		printf ("\n");
	}
	else
	{
		printf ("On n'a pu atteindre les valeurs propres<\n");
		exit();
	}

	printf ("Entrez le nombre de vecteurs propres que l'on souhaite obtenir : ");
	scanf ("%d", &k);
	printf (" %d\n", k);
	if (!k)
		exit();

	if (vect_propre (mat, val, n, DIM, k, vect))
	{
		printf ("Les vecteurs propres sont : \n");
		for (i=0; i<n; i++)
		{
			for (j=0; j<k; j++)
				printf ("%10.4g", vect[i][j]);
			putchar ('\n');
		}
	}
	else
	{
		printf ("On n'a pu obtenir les vecteurs propres.\n");
	}
}
