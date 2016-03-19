
#include <math.h>

#define Z_DIV 1.e-30
#define Z_DIFF 1.e-14

int G_syst (a, b, n, p, x)
int n, p;
double *a, *b, *x;
{
register i, j, k;
int icl, im, jm;
int *tc;
double pivot, ro, ap, res;
double **pt_a, **calloc();

	pt_a = calloc (n, sizeof(double *));
	for (i=0; i<n; i++)
		pt_a[i] = a + i * p;

	tc = (int *) calloc (n, sizeof(int));

	for (icl=0; icl<n; icl++)
	{
		pivot = pt_a[icl][icl];
		im = jm = icl;
		for (i=icl; i<n; i++)
		for (j=icl; j<n; j++)
			if (fabs (pt_a[i][j]) > fabs(pivot))
			{
				pivot = pt_a[i][j];
				im = i;
				jm = j;
			}
		if (fabs(pivot) < Z_DIV)
			return 0;
		if (im != icl)
		{
			for (j=icl; j<n; j++)
				ro = pt_a[icl][j],
				pt_a[icl][j] = pt_a[im][j],
				pt_a[im][j] = ro;
			ro = b[icl],
			b[icl] = b[im],
			b[im] = ro;
		}

		if (jm != icl)
			for (i=0; i<n; i++)
				ro = pt_a[i][icl],
				pt_a[i][icl] = pt_a[i][jm],
				pt_a[i][jm] = ro;
		tc[icl] = jm;

		for (i = icl+1; i<n; i++)
		{
			ap = pt_a[i][icl] / pivot;
			for (pt_a[i][icl]=0, j=icl+1; j<n; j++)
			{
				res = pt_a[i][j] - ap * pt_a[icl][j];
				if (fabs(res) < fabs(pt_a[i][j] * Z_DIFF))
					res = 0;
				pt_a[i][j] = res;
			}
			res = b[i] - ap * b[icl];
			if (fabs(res) < fabs(b[i]) * Z_DIFF)
				res = 0;
			b[i] = res;
		}

	}

	x[n-1] = b[n-1] / pt_a[n-1][n-1];
	for (k=0; k<n-1; k++)
	{
		i = n - k - 2;
		for (res=0, j=i+1; j<n; j++)
			res -= pt_a[i][j] * x[j];
		x[i] = (b[i] + res) / pt_a[i][i];
	}

	for (k=n-1; k>=0; k--)
	{
		if (tc[k] != k)
			ro = x[k],
			x[k] = x[tc[k]],
			x[tc[k]] = ro;
	}

	free(pt_a);
	free(tc);
	return 1;
}
