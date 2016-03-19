
#include "matrix.h"

#define MAX 10

/* Algorithme de Rutishauser
	On decompose la matrice A en 2 matrices L et R, L etant une
	matrice triangulaire inferieure a diagonale unite et R
	une matrice triangulaire superieure telles que A = L R
	On calcule A1 = R L qui a les memes valeurs propres que A
	puis on decompose A1 = L1 R1
	on calcule A2 = R1 L1, etc...
	Dans certaines conditions,
	on obtient une matrice qui devient triangulaire superieure
	ou les valeurs propres se trouvent sur la diagonale.

*/

char buf[60];

int lr (matrix A, matrix L, matrix R)
{
int i, j, k, n;
real tr, tl;

	n = DIM(A,0);

	init_matrix (L, 0.0);
	init_matrix (R, 0.0);

	for (i=0; i<n; i++)
		VAL2(L,i,i) = 1;
	for (j=0; j<n; j++)
		VAL2(R,0,j) = VAL2(A,0,j);
	for (i=1; i<n; i++)
		VAL2(L,i,0) = VAL2(A,i,0) / VAL2(A,0,0);

	for (k=1; k<n; k++)
	{
		for (j=k; j<n; j++)
		{
			tr = 0;
			for (i=0; i<k; i++)
				tr += VAL2(R,i,j) * VAL2(L,k,i);
			VAL2(R,k,j) = VAL2(A,k,j) - tr;
		}
		if (k != n)
		{
			for (i=k+1; i<n; i++)
			{
				tl = 0;
				for (j=0; j<k; j++)
					tl += VAL2(L,i,j) * VAL2(R,j,k);
				VAL2(L,i,k) = (VAL2(A,i,k)-tl) / VAL2(R,k,k);
			}
		}
	}
}

vprut1 (matrix A)
{
int n, i, j, m, lf;
real er, x;
	n = DIM(A,0);

	MATRIX (VP, MAX, n, 1)
	MATRIX (L, MAX*MAX, n, n)
	MATRIX (R, MAX*MAX, n, n)

	m = 0;
	for (i=0; i<n; i++) VAL1(VP,i) = 0;
	er = 1;
	lf = 1;
	while ((m < 50) && /* (er > 0.5) */ lf )
	{
		lr (A, L, R);
		printf ("L : "); print_matrix (L);
		printf ("R : "); print_matrix (R);
		prod_matrix (L, R, A);
		printf ("LR: "); print_matrix (A);
		prod_matrix (R, L, A);
		printf ("A : "); print_matrix (A);
		gets (buf);
		er = 0;
		m++;
		for (i=0; i<n; i++)
		for (j=0; j<n; j++)
		{
			if (j < i)
			{
				x = fabs (VAL2(A,i,j));
				if (x > er)
					er = x;
			}
		}
		lf = 0;
		for (i=0; i<n; i++)
		{
			if (fabs(VAL2(A,i,i)-VAL1(VP,i)) > 1e-4)
				lf = 1;
		}
		for (i=0; i<n; i++)
			VAL1(VP,i) = VAL2(A,i,i);
	}
	if (m == 50)
		printf ("Non convergence\n");
	else
	{
		print_matrix (A);
	}

	ENDMAT ENDMAT ENDMAT
}

vprut (matrix A1, matrix VP)
{
int n, i, status;
	n = DIM(A1,0);
	MATRIX(A,MAX*MAX,n,n);
	status = vprut1 (A);
	for (i=0; i<n; i++)
		VAL1(VP,i) = VAL2(A,i,i);
	return status;
	ENDMAT
}

main ()
{
	MATRIX (A, MAX*MAX, MAX, MAX);
	read_matrix (A);
	print_matrix (A);
	vprut1 (A);
	print_matrix (A);

	ENDMAT
}

