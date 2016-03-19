
#include "matrix.h"

#define MAX 10

eq_gs1 (matrix A, matrix Y, matrix X, int n)
{
int i, j;
real s;
	for (i=0; i<n; i++)
	{
		s = 0;
		for (j=0; j<n; j++)
		{
			if (j != i)
				s += VAL2(A,i,j) * VAL1(X,j);
		}
		VAL1(X,i) = (VAL1(Y,i) - s) / VAL2(A,i,i);
	}

}

eq_gs (matrix A, matrix Y, matrix X)
{
int i, n;
	n = DIM(A,0);
	MATRIX (X1, MAX, n, 1)
	for (i=0; i<n; i++)
		VAL1(X,i) = 1;
	for (;;)
	{
		/* print_matrix (X); */
		copy_matrix (X, X1);
		eq_gs1 (A, Y, X1, n);
		print_matrix (X1);
		for (i=0; i<n; i++)
		{
			if (fabs((VAL1(X1,i)-VAL1(X,i)) / VAL1(X1,i)) > 1e-4)
				goto goon;
		}
		break;
	goon:
		copy_matrix (X1, X);
	}
	ENDMAT
}

main ()
{
int n;
	MATRIX (A, MAX*MAX, MAX, MAX)
	MATRIX (Y, MAX, MAX, 1)

	read_matrix (A);
	if (DIM(A,0) != DIM(A,1))
	{
		printf ("La matrice doit etre carree\n");
		return;
	}
	n = DIM(A,0);

	read_matrix (Y);
	if (DIM(Y,0) != n)
	{
		printf ("Ce vecteur doit avoir %d lignes\n", n);
		return;
	}

	MATRIX (X, MAX, n, 1)

	eq_gs (A, Y, X);

	printf ("Solution : ");
	print_matrix (X);

	ENDMAT ENDMAT ENDMAT
}
