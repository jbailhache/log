
#include <math.h>

#define ZERO 1.e-13

#define ALLOCATION calloc (n, sizeof(double *));

#define LIBERATION \
	free (pt_a); \
	free (pt_l); \
	free (pt_u);

int LU (a, n, p, lower, upper)
int n, p;
double *a, *lower, *upper;
{
register i, j, k;
int mult;
double som;
double **pt_a, **pt_l, **pt_u, **calloc();

	if (n <= 0)
		return 0;

	if (fabs (*a) < ZERO)
		return 0;

	pt_a = ALLOCATION
	pt_l = ALLOCATION
	pt_u = ALLOCATION
	for (i=0; i<n; i++)
	{
		mult = p * i;
		pt_a[i] = a + mult;
		pt_l[i] = lower + mult;
		pt_u[i] = upper + mult;
	}
	for (i=0; i<n; i++)
		pt_l[i][0] = pt_a[i][0];
	for (j=0; j<n; j++)
		pt_u[0][j] = pt_a[0][j] / pt_l[0][0];

	for (j=1; j<n; j++)
	{
		for (i=j; i<n; i++)
		{
			for (som=k=0; k<j; k++)
				som += pt_l[i][k] * pt_u[k][j];
			pt_l[i][j] = pt_a[i][j] - som;
			if (i==j && fabs(pt_l[i][j]) < ZERO)
			{
				LIBERATION
				return 0;
			}
		}
		pt_u[j][j] = 1;
		if (j != n-1)
			for (i=j+1; i<n; i++)
			{
				for (som=k=0; k<j; k++)
					som += pt_l[j][k] * pt_u[k][i];
				pt_u[j][i] = (pt_a[j][i] - som)
					/ pt_l[j][j];
			}
	}
	LIBERATION
	return 1;
}
