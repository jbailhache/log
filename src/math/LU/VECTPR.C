
#include <math.h>

#define ZER 1.e-12

#define ALLOCAT1 (double *) calloc (p*p, sizeof(double));
#define ALLOCAT2 calloc (n, sizeof(double *));
#define ALLOCAT3 (double *) calloc (n, sizeof(double));

#define LIBERAT \
	free(at); \
	free(b); \
	free(d); \
	free(pt_a); \
	free(pt_vect); \
	free(pt_at);

int vect_propre (a, val, n, p, l, vect)
int n, p, l;
double *a, *val, *vect;
{
register i, j, k;
int mult, G_syst();
double *at, *b, *d, **pt_a, **pt_vect, **pt_at, **calloc();
	if (n == 1)
	{
		*vect = 1;
		return 1;
	}
	at = ALLOCAT1

	b = ALLOCAT3
	d = ALLOCAT3

	pt_a = ALLOCAT2
	pt_vect = ALLOCAT2
	pt_at = ALLOCAT2

	for (i=0; i<n; i++)
	{
		mult = i * p;
		pt_a[i] = a + mult;
		pt_vect[i] = vect + mult;
		pt_at[i] = at + mult;
	}

	for (i=0; i<l; i++)
	{
		for (j=0; j<n-1; j++)
		for (k=0; k<n-1; k++)
			pt_at[j][k] = pt_a[j][k];
		for (k=0; k<n-1; k++)
		{
			pt_at[k][k] -= val[i];
			b[k] = -pt_at[k][n-1];
		}
		if (n == 2)
			if (fabs(pt_at[0][0]) > ZER)
				d[0] = b[0] / pt_at[0][0];
			else
				return 0;
		else if (!G_syst (at, b, n-1, p, d))
		{
			LIBERAT
			return 0;
		}

		for (k=0; k<n-1; k++)
			pt_vect[k][i] = d[k];
		pt_vect[n-1][i] = 1;
	}
	LIBERAT
	return 1;

}
