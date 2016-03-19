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

real fvect1 (fmatrix a, int i, int j)
{
	if (j == 0)
		return 1;
	else
		return 0;
}

struct fmatrix fmvect1[1] = {{ fvect1 }};

real fscal1 (fmatrix a, int i, int j)
{
	if (i == 0 && j == 0)
		return 1;
	else
		return 0;
}

struct fmatrix fmscal1[1] = {{ fscal1 }};

real fmget (fmatrix a, int i, int j)
{
real x;
	x = (*(a->f)) (a, i, j);
	return x;
}

#define DIM 4

print_fmatrix (fmatrix a, int dim[])
{
int i, j;
	printf ("\n");
	for (i=0; i<dim[0]; i++)
	{
		for (j=0; j<dim[1]; j++)
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

copy_fmatrix (struct data_fmatrix *a, fmatrix b, int dim[]) /* a = b */
{
int i, j, k;
real x;
	a->f = data_f;
	for (k=0; k<2; k++)
		a->dim[k] = dim[k];
	for (i=0; i<a->dim[0]; i++)
	for (j=0; j<a->dim[1]; j++)
	{
		x = fmget (b, i, j);
		a->data[i*(a->dim[1])+j] = x;
	}
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
int dim1[] = { 6, 5 };

real bufA[16] = {
	-183, 53, -12, 28,
	-392, 113, -38, 65,
	-192, 56, -1, 25,
	-538, 157, -7, 71
	};

#define MAXDIM 8

int dimscal[] = { 1 , 1 };

vppuissance (fmatrix A, int dim[])
{
real l, l1;
int i;
struct data_fmatrix u[1], v[1];
real bufu[MAXDIM*MAXDIM], bufv[MAXDIM*MAXDIM], buft[MAXDIM*MAXDIM],
	bufw[MAXDIM*MAXDIM];
int dimvect[2];
struct op_fmatrix op[1];
struct data_fmatrix t[1];
struct data_fmatrix w[1];

	dimvect[0] = dim[0];
	dimvect[1] = 1;

	u->data = bufu;
	v->data = bufv;
	t->data = buft;
	w->data = bufw;

	copy_fmatrix (u, fmvect1, dimvect);
	copy_fmatrix (v, fmscal1, dimvect);

	l = 1;
	l1 = 0;
	i = 0;

	while (i < 10000 && ((i < 5) || (abs(l-l1) > /*1e-16*/ 0.0001)))
	{
		i++;

		op->a[0] = A;
		op->a[1] = u;
		op->f = mult_f;
		copy_fmatrix (w, op, dim);

		l1 = l;

		op->a[0] = w;
		op->a[1] = v;
		op->f = mult_f;
		copy_fmatrix (t, op, dimscal);
		l = fmget (t, 0, 0);

		t->dim[0] = 1;
		t->dim[1] = 1;
		t->data[0] = 1/l;
		op->a[0] = w;
		op->a[1] = t;
		op->f = mult_f;
		copy_fmatrix (u, op, dimvect);

	}
	printf ("Valeur propre : %f\n", l);
	print_fmatrix (u, dimvect);

}

main ()
{
real bufa[60], bufm[60];
struct data_fmatrix a[1], m[1], A[1];
struct op_fmatrix b[1], c[1], e[1], h[1];
real d;

	print_fmatrix (fmId, dim1);

	a->data = bufa;
	m->data = bufm;

	read_fmatrix (a);
	print_fmatrix (a, a->dim);

	printf ("det a = %f\n", d = det (a, a->dim[0]));

	b->a[0] = a;
	b->dim = a->dim[0];
	b->f = comat_f;
	print_fmatrix (b, a->dim);

	c->a[0] = b;
	c->f = transpose_f;
	print_fmatrix (c, a->dim);

	e->a[0] = c;
	e->fact = 1 / d;
	e->f = mult_scal_f;
	print_fmatrix (e, a->dim);

	h->a[0] = a;
	h->a[1] = e;
	h->f = mult_f;
	print_fmatrix (h, a->dim);

	b->a[0] = a;
	b->a[1] = a;
	b->f = plus_f;
	print_fmatrix (b, a->dim);

	c->a[0] = a;
	c->a[1] = b;
	c->f = mult_f;
	print_fmatrix (c, a->dim);

	copy_fmatrix (m, c, a->dim);
	print_fmatrix (m, a->dim);

	print_fmatrix (fmvect1, dim1);

	print_fmatrix (fmscal1, dim1);

	A->data = bufA;
	A->f = data_f;
	A->dim[0] = 4;
	A->dim[1] = 4;
	print_fmatrix (A, A->dim);

	vppuissance (A, A->dim);


}