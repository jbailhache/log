
#include "matrix.h"

#define MAX 10

char buf[60];

#define trace_matrix(str,M) printf("%s : ", str); print_matrix (M);\
 gets (buf);

pivot (matrix A1, matrix B)
{
int n, i, j, k;
real p, q;

	n = DIM (A1, 0);

	MATRIX (A, MAX*MAX, n, n);
	copy_matrix (A1, A);

	for (i=0; i<n; i++)
	for (j=0; j<n; j++)
	{
		VAL2 (B, i, j) = i == j;
	}
	trace_matrix ("B initiale", B);

	for (i=0; i<n; i++)
	{
		p = VAL2 (A, i, i);
		/* ajouter traitement cas p = 0 */
		/* on divise la 1ere ligne par le pivot */
		for (j=0; j<n; j++)
		{
			VAL2 (A, i, j) /= p;
			VAL2 (B, i, j) /= p;
		}
		trace_matrix ("A", A);
		trace_matrix ("B", B);

		for (k=0; k<n; k++)
		if (k != i)
		{
			q = VAL2 (A, k, i);
			for (j=0; j<n; j++)
			{
				VAL2 (A, k, j) -= q * VAL2 (A, i, j);
				VAL2 (B, k, j) -= q * VAL2 (B, i, j);
			}
		}
		trace_matrix ("A", A);
		trace_matrix ("B", B);
	}
	ENDMAT
}

main ()
{
int n;
	MATRIX (A, MAX*MAX, MAX, MAX)

	read_matrix (A);

	if (DIM(A,0) != DIM(A,1))
	{
		printf ("La matrice doit etre carree\n");
		return;
	}
	n = DIM(A,0);

	MATRIX (B, MAX*MAX, n, n)

	printf ("Inversion de la matrice : ");
	print_matrix (A);

	pivot (A, B);

	printf ("Matrice inverse : ");
	print_matrix (B);

	ENDMAT ENDMAT
}
