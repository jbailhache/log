
#define ALLOC1 (double *) calloc (p * p, sizeof(double));
#define ALLOC2 calloc (n, sizeof(double *));

#define LIB \
	free(a); \
	free(lower); \
	free(upper); \
	free (pt_mat); \
	free (pt_a); \
	free (pt_l); \
	free (pt_u);

int LU_Rutishauser (mat, n, p, it, val)
int n, p, it;
double *mat, *val;
{
register i, j, k;
int mult, compt=0, LU();
double *a, *lower, *upper, **pt_mat, **pt_a, **pt_l, **pt_u, **calloc();

	a = ALLOC1
	lower = ALLOC1
	upper = ALLOC1

	pt_mat = ALLOC2
	pt_a = ALLOC2
	pt_l = ALLOC2
	pt_u = ALLOC2

	for (i=0; i<n; i++)
	{
		mult = i * p;
		pt_mat[i] = mat + mult;
		pt_a[i] = a + mult;
		pt_l[i] = lower + mult;
		pt_u[i] = upper + mult;
	}
	for (i=0; i<n; i++)
	for (j=0; j<n; j++)
		pt_a[i][j] = pt_mat[i][j];

	do
	{
		if (!(LU(a,n,p,lower,upper)))
		{
			LIB
			return 0;
		}

		for (i=0; i<n; i++)
		for (k=0; k<n; k++)
		for (pt_a[i][k]=j=0; j<n; j++)
			pt_a[i][k] += pt_u[i][k] * pt_l[j][k];
	} while (++compt < it);

	for (i=0; i<n; i++)
		val[i] = pt_a[i][i];
	LIB
	return 1;

}
