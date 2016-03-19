/* Matrice fonctionnelle
	p -> { f, ... }
	f (p, i, j)
*/

#define real float

struct fmatrix
{
	real (*f) ();
};

typedef struct fmatrix *fmatrix;

real f0 (fmatrix a)
{
	return 0.0;
}

struct fmatrix fm0[1] = { { f0 } };

real fId (fmatrix a, int i, int j)
{
	if (i == j)
		return 1.0;
	else
		return 0.0;
}

struct fmatrix fmId[1] = {{ fId }};

real fmget (fmatrix a, int i, int j)
{
	return (*(a->f)) (a, i, j);
}

#define DIM 6

print_fmatrix (fmatrix a)
{
int i, j;
	printf ("\n");
	for (i=0; i<DIM; i++)
	{
		for (j=0; j<DIM; j++)
			printf (" %8.3f", fmget (a, i, j));
		printf ("\n");
	}
}

struct data_fmatrix
{
	real (*f) ();
	int dim[2];
	real *data;
};

real data_f (struct data_fmatrix *a, int i, int j)
{
	if (i < a->dim[0] && j < a->dim[1])
		return a->data [ i * a->dim[1] + j ];
	else
		return 0.0;
}

read_fmatrix (struct data_fmatrix *a)
{
int i, j;
	a->f = data_f;
	printf ("Nombre de lignes ? ");
	scanf ("%d", &(a->dim[0]));
	printf ("Nombre de colonnes ? ");
	scanf ("%d", &(a->dim[1]));
	for (i=0; i<a->dim[0]; i++)
	for (j=0; j<a->dim[1]; j++)
		scanf ("%f", &(a->data[i*(a->dim[1])+j]));

}

struct op_fmatrix
{
	real (*f) ();
	fmatrix a[2];
	int dim;
	int i[2];
	real fact;
};

real plus_f (struct op_fmatrix *a, int i, int j)
{
	return fmget (a->a[0], i, j) + fmget (a->a[1], i, j);
}

real mult_scal_f (struct op_fmatrix *a, int i, int j)
{
real x;
	x = fmget (a->a[0], i, j);
	return a->fact * x;
}

real mult_f (struct op_fmatrix *a, int i, int j)
{
int k;
real s;
	s = 0.0;
	for (k=0; k<DIM; k++)
	{
		s += fmget (a->a[0], i, k) * fmget (a->a[1], k, j);
	}
	return s;
}

real transpose_f (struct op_fmatrix *a, int i, int j)
{
	return fmget (a->a[0], j, i);
}

int com_i (int i, int j)
{
	if (i<j)
		return i;
	else
		return i+1;
}

real com_f (struct op_fmatrix *a, int i, int j)
{
	return fmget (a->a[0], com_i(i,a->i[0]), com_i(j,a->i[1]));
}

real det (fmatrix a, int dim)
{
int i;
real d, d1;
int signe;
struct op_fmatrix b[1];

	if (dim == 1)
		return fmget (a, 0, 0);

	d = 0;
	signe = 1;
	for (i=0; i<dim; i++)
	{
		b->f = com_f;
		b->a[0] = a;
		b->i[0] = 0;
		b->i[1] = i;
		/* printf ("."); */
		d1 = det (b, dim-1);
		/* printf ("<"); */
		d += signe * fmget (a, 0, i) * d1;
		/* printf (">"); */
		signe = -signe;
	}
	return d;
}

real comat_f (struct op_fmatrix *a, int i, int j)
{
real x;
struct op_fmatrix b[1];
	b->f = com_f;
	b->a[0] = a->a[0];
	b->i[0] = i;
	b->i[1] = j;
	x = det (b, a->dim-1);
	x *= (1 - 2*((i+j)%2));
	return x;
}

main ()
{
real buf[60];
struct data_fmatrix a[1];
struct op_fmatrix b[1], c[1], e[1], h[1];
real d;

	print_fmatrix (fmId);

	a->data = buf;
	read_fmatrix (a);
	print_fmatrix (a);

	printf ("det a = %f\n", d = det (a, DIM));

	b->a[0] = a;
	b->dim = DIM;
	b->f = comat_f;
	print_fmatrix (b);

	c->a[0] = b;
	c->f = transpose_f;
	print_fmatrix (c);

	e->a[0] = c;
	e->fact = 1 / d;
	e->f = mult_scal_f;
	print_fmatrix (e);

	h->a[0] = a;
	h->a[1] = e;
	h->f = mult_f;
	print_fmatrix (h);

	b->a[0] = a;
	b->a[1] = a;
	b->f = plus_f;
	print_fmatrix (b);

	c->a[0] = a;
	c->a[1] = b;
	c->f = mult_f;
	print_fmatrix (c);
}

