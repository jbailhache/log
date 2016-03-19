
#include "matrix.h"

#define MAXIT 60

#define ERR(status) ((status)<0)

int n;
/* real v1; */

#define SIZE 20

struct matrix x[1];
real buf_x[SIZE];

int verif (matrix a, matrix v, real vp)
{
int i, j;
real z;
	/* printf ("Verification valeur propre %8.3f : \n", vp); */
	for (i=0; i<DIM(a,0); i++)
	{
		z = 0;
		for (j=0; j<DIM(a,1); j++)
			z += VAL2(a,i,j) * VAL1(v,j);
		if (fabs (z - vp * VAL1(v,i)) > 1e-4)
		{
			printf (" %d %8.3f\n", i, z - vp * VAL1(v,i));
			return -4;
		}
	}
	return 1;
}

/* Puissance iteree :
	La matrice A a pour valeurs propres \1, \2, ... \n
	et vecteurs propres V1, V2 ... Vn
	X0 dans la base des vecteurs propres = c1 V1 + ... + cn Vn
	A X0 = A c1 V1 + ... + A cn Vn
	A c1 V1 = c1 A V1 = c1 \1 V1
	donc A X0 = c1 \1 V1 + ... + cn \n Vn
	A^k X0 = c1 \1^k V1 + ... + cn \n^k Vn
	= \1^k (c1 V1 + c2 (\2 / \1)^k V2 + ... + cn (\n / \1)^k vn)
	\i / \1 -> 0 quand k -> oo
	Donc A^k X0 -> \1^k c1 V1
	     A^(k+1) X0 -> \1^(k+1) c1 V1
	(A^(k+1) X0) / (A^k X0) -> \1
*/

iter (matrix m, real *pv1)
{
int i, j, k, status;
real er, vp, z, ay, y1;
/* MATRIX(x,SIZE,n,1) */

/* #define MATRIX(a,size,n,p) \
	struct matrix x[1];
	real buf_x[SIZE];
	{
		x->val = buf_x;
		x->dim[0] = n;
		x->dim[1] = 1;
				*/
MATRIX(y,SIZE,n,1)

	for (i=0; i<n; i++)
		VAL1(x,i) = 1;
	/* vp = 1.23456789; */
	vp = 1.0;
	k = 0;
	er = 1;
	while ((k < MAXIT) && (er > 1e-5))
	{
		prod_matrix (m, x, y);
		/*for (i=0; i<n; i++)
		{
			VAL1(y,i) = 0;
			for (j=0; j<n; j++)
				VAL1(y,i) += VAL2(m,i,j) * VAL1(x,j);
		} */
		*pv1 = DIV (VAL1(y,0), VAL1(x,0));
		z = 0;
		k++;
		for (i=0; i<n; i++)
		{
			y1 = VAL1(y,i);
			ay = fabs(y1);
			if (ay > z)
				z = VAL1(y,i);
		}
		for (i=0; i<n; i++)
			VAL1(x,i) = DIV (VAL1(y,i), z);
		er = fabs(vp - *pv1);
		vp = *pv1;
	}
	if (k > MAXIT)
		return -6;
	return 1;

ENDMAT

}

/* Deflation
	V1 vecteur propre de A associe a \1
	W1 vecteur propre de AT associe a \1
	A1 = A - \1 / (W1T . V1) . V1 W1T
	A1 V1 = A V1 - \1 / (W1T . V1) .V1 W1T V1
	= A V1 - \1 V1 = 0
	A1 admet donc la valeur propre 0
	A1 V2 = A V2 - \1 / (W1T . V1) V1 W1T V2
	= A V2 - \1 V1 (W1T V2 / W1T V1)
	\1 W1 = AT W1 => \1 W1T = W1T A
	\1 W1T V2 = W1T A V2 = W1T \2 V2
	(\1 - \2) W1T V2 = 0
	W1T V2 = 0
	Donc A1 V2 = A V2 = \2 V2
	\2 est la plus grande valeur propre de A1
*/

#define MAX 10

int vpdefla (matrix a1, matrix vd, matrix vv)
{
int l, i, j, status;
real va, ps;
real v1;
	MATRIX(a,MAX*MAX,n,n)
	copy_matrix (a1, a);
	MATRIX(at,MAX*MAX,n,n)

	x->val = buf_x;
	x->dim[0] = n;
	x->dim[1] = 1;

	for (l=0; l<n; l++)
	{
		status = iter (a, &v1);
		/* if (k >= MAXIT)
			printf ("Non convergence %8.3f\n", v1); */
		if (ERR(status))
			return status;
		/* else
		{
			printf ("Valeur propre %8.3f\nVecteur propre associe\n", v1);
			print_matrix (x);
		}
		*/
		status = verif (a, x, v1);
		if (ERR(status))
			return status;
		VAL1(vd,l) = va = v1;
		for (i=0; i<n; i++)
			VAL2(vv,l,i) = VAL1(x,i);

		transpose (a, at);
		status = iter (at, &v1);
		/* if (k >= MAXIT)
			printf ("Non convergence at %8.3f\n", v1); */
		if (ERR(status))
			return status;
		else
		{
			if (fabs(va-v1) > 1e-4)
			{
				printf ("Erreur %8.3f %8.3f\n", va, v1);
				/* return -3; */
			}
			/* else */
			{
				ps = 0;
				for (i=0; i<n; i++)
					ps += VAL1(x,i) * VAL2(vv,l,i);
				ps = v1/ps;
				for (i=0; i<n; i++)
				for (j=0; j<n; j++)
					VAL2(a,i,j) -= ps * VAL2(vv,l,i) * VAL1(x,j);
			}
		}

	}
	ENDMAT ENDMAT
	return 1;
}

main ()
{
int status;
	/*
	printf ("Ordre de la matrice ? ");
	scanf ("%d", &n);
	*/
	MATRIX (a, MAX*MAX, MAX, MAX)

	read_matrix (a);
	print_matrix (a);
	if (DIM(a,0) != DIM(a,1))
	{
		printf ("La matrice doit etre carree\n");
		return;
	}
	n = DIM(a,0);

	/*MATRIX(a,MAX*MAX,n,n)*/
	/* MATRIX(at,MAX*MAX,n,n) */
	MATRIX(vd,MAX,n,1)
	MATRIX(vv,MAX*MAX,n,n)

	status = vpdefla (a, vd, vv);

	printf ("Status = %d\n", status);

	if (ERR(status))
	{
		printf ("Erreur\n");
		return;
	}

	/* for (l=0; l<n; l++)
	{
		printf ("Valeur propre %8.3f :\n", VAL1(vd,l));
		print_matrix (vv);
	}
	      */
	printf ("Valeurs propres : ");
	/* for (l=0; l<n; l++)
		printf (" %8.3f ", VAL1(vd,l)); */
	print_matrix (vd);

	printf ("\nVecteurs propres (en lignes) :\n");
	print_matrix (vv);

	ENDMAT ENDMAT ENDMAT


}
