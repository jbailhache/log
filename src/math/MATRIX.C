
#include "matrix.h"

init_matrix (matrix a, real x)
{
int i, j;
	for (i=0; i<DIM(a,0); i++)
	for (j=0; j<DIM(a,1); j++)
		VAL2(a,i,j) = x;
}

real divi (real x, real y, char *file, int line)
{
	if (y != 0.0)
		return x / y;
	fprintf (stderr, "%s(%d) : Division de %8.3f par 0.\n",
		file, line, x);
	return 1e10;
}

print_matrix (matrix a)
{
int i, j;
    printf ("Matrix %d x %d : \n", DIM(a,0), DIM(a,1));
    for (i=0; i<DIM(a,0); i++)
    {
	for (j=0; j<DIM(a,1); j++)
		printf (" %8.3f", VAL2(a,i,j));
	printf ("\n");
    }
}

read_matrix (matrix a)
{
int i, j, n, p;
real x;
	printf ("Nombre de lignes ? ");
	scanf ("%d", &n);
	printf ("Nombre de colonnes ? ");
	scanf ("%d", &p);
	DIM(a,0) = n;
	DIM(a,1) = p;
	printf ("Valeurs ? ");
	for (i=0; i<n*p; i++)
	{
		scanf ("%f", &x);
		VAL1(a,i) = x;
	}
}
/*
verif (matrix a, matrix v, real vp)
{
int i, j;
real z;
	printf ("Verification valeur propre %8.3f : \n", vp);
	for (i=0; i<DIM(a,0); i++)
	{
		z = 0;
		for (j=0; j<DIM(a,1); j++)
			z += VAL2(a,i,j) * VAL1(v,j);
		printf (" %d %8.3f\n", i, z - vp * VAL1(v,i));
	}
}
*/

copy_matrix (matrix a, matrix b)
{
int n, p, i, j;
	n = DIM(b,0) = DIM(a,0);
	p = DIM(b,1) = DIM(a,1);
	for (i=0; i<n; i++)
	for (j=0; j<p; j++)
		VAL2(b,i,j) = VAL2(a,i,j);
}

transpose (matrix a, matrix at)
{
int n, p, i, j;
	n = DIM(at,1) = DIM(a,0);
	p = DIM(at,0) = DIM(a,1);
	for (i=0; i<n; i++)
	for (j=0; j<p; j++)
		VAL2(at,j,i) = VAL2(a,i,j);

}

int prod_matrix (matrix a, matrix b, matrix c)
{
int n, p, q, i, j, k;
	if (DIM(a,1) != DIM(b,0))
	{
		printf ("Bad dimension for product\n");
		return -5;
	}
	n = DIM(a,0);
	p = DIM(a,1);
	q = DIM(b,1);
	DIM(c,0) = n;
	DIM(c,1) = q;
	for (i=0; i<n; i++)
	for (j=0; j<q; j++)
	{
		VAL2(c,i,j) = 0;
		for (k=0; k<p; k++)
			VAL2(c,i,j) += VAL2(a,i,k) * VAL2(b,k,j);
	}
	return 1;

}



