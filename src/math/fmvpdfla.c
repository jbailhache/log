/* Matrice fonctionnelle
	p -> { f, ... }
	f (p, i, j)
*/

#define real double

#include <math.h>

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

#define MAXDIM 10

print_fmatrix (fmatrix a, int dim[])
{
int i, j;
real x;
	printf ("\n");
	for (i=0; i<dim[0]; i++)
	{
		for (j=0; j<dim[1]; j++)
		{
			x = fmget (a, i, j);
			printf (" %8.3f", x);
		}
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
		scanf ("%lf", &(a->data[i*(a->dim[1])+j]));

}

set_fmatrix (struct data_fmatrix *a, int i, int j, real x)
{
	a->data[i*(a->dim[1])+j] = x;
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
		/*a->data[i*(a->dim[1])+j] = x;*/
		set_fmatrix (a, i, j, x);
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
	for (k=0; k<MAXDIM; k++)
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

real bufA2[16] = {
	3, 2, 1, 0,
	4, 6, 3, 1,
	2, 3, 7, 2,
	1, 3, 5, 7
	};

int dimscal[] = { 1 , 1 };

vppuissance (fmatrix A, int dim[], real *valpr, struct data_fmatrix *vectpr)
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
char buf[10];

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

	while (i < 10000 && ((i < 5) || (fabs(l-l1) > 1e-16 /*0.0001*/)))
	{
		i++;

		op->a[0] = A;
		op->a[1] = u;
		op->f = mult_f;
		copy_fmatrix (w, op, dim);

		/*
		printf ("\nAu%d:", i);
		print_fmatrix (w, dimvect);
		*/

		l1 = l;

		op->a[0] = w;
		op->a[1] = v;
		op->f = mult_f;
		copy_fmatrix (t, op, dimscal);
		l = fmget (t, 0, 0);
		/* printf ("l%d = %lf\n", i, l); */

		t->dim[0] = 1;
		t->dim[1] = 1;
		t->data[0] = 1/l;
		op->a[0] = w;
		op->a[1] = t;
		op->f = mult_f;
		copy_fmatrix (u, op, dimvect);

		/*
		printf ("\nu%d :", i);
		print_fmatrix (u, dimvect);
		*/
		/*gets (buf);*/

	}
	printf ("\nValeur propre : %lf\n", l);
	print_fmatrix (u, dimvect);

	*valpr = l;
	copy_fmatrix (vectpr, u, dimvect);

}

vpdefla (fmatrix A, int *dim, struct data_fmatrix *valp, struct data_fmatrix *vecp)
{
int i, j, n;
struct data_fmatrix Ai[1];
real bufAi[MAXDIM*MAXDIM];
real l, l1;
struct data_fmatrix v[1], t[1], x1[1];
real bufv[MAXDIM], buft[MAXDIM*MAXDIM], bufx1[MAXDIM];
struct op_fmatrix vt[1], v2[1], w[1], wt[1], xwt[1], mlxwt[1], Ai1[1],
	x[1], y[1], wv[1];
int dimvect[2];
char buf[10];

	dimvect[0] = dim[0];
	dimvect[1] = 1;

	n = dim[0];

	valp->f = data_f;
	valp->dim[0] = n;
	valp->dim[1] = 1;

	vecp->f = data_f;
	vecp->dim[0] = n;
	vecp->dim[1] = n;

	Ai->data = bufAi;
	copy_fmatrix (Ai, A, dim);

	v->data = bufv;

	t->f = data_f;
	t->dim[0] = dim[0];
	t->dim[1] = dim[1];
	t->data = buft;

	x1->f = data_f;
	x1->dim[0] = dim[0];
	x1->dim[1] = dim[1];
	x1->data = bufx1;

	for (i=0; i<n; i++)
	{
		print_fmatrix (Ai, dim);

		l1 = l;
		vppuissance (Ai, dim, &l, v);

		printf ("Valeur propre %d = %lf\n", i, l);
		print_fmatrix (v, dimvect);
		gets (buf);

		set_fmatrix (valp, 0, i, l);

		vt->a[0] = v;
		vt->f = transpose_f;

		v2->a[0] = vt;
		v2->a[1] = v;
		v2->f = mult_f;

		w->a[0] = v;
		w->fact = 1 / fmget (v2, 0, 0);
		w->f = mult_scal_f;

		print_fmatrix (w, dimvect);

		if (i == 0)
		{
			x->a[0] = v;
			x->f = mult_scal_f;
			x->fact = 1.0;
		}
		else
		{
			/* x2 = v2 + (l1/(l2-l1)) <w,v2> x1 */

			x->a[0] = v;
			x->f = plus_f;
			x->a[1] = y;

			wv->a[1] = w;
			wv->f = mult_f;
			wv->a[0] = vt;

			y->fact = (l1 / (l - l1)) * fmget (wv, 0, 0);
			y->f = mult_scal_f;
			y->a[0] = x1;
		}

		copy_fmatrix (x1, x, dimvect);

		print_fmatrix (x1, dimvect);

		for (j=0; j<n; j++)
			set_fmatrix (vecp, j, i, fmget (x, j, 0));

		mlxwt->fact = -l;
		mlxwt->f = mult_scal_f;
		mlxwt->a[0] = xwt;

		xwt->a[0] = x;
		xwt->f = mult_f;
		xwt->a[1] = wt;

		wt->a[0] = w;
		wt->f = transpose_f;

		Ai1->a[0] = Ai;
		Ai1->f = plus_f;
		Ai1->a[1] = mlxwt;

		print_fmatrix (Ai1, dim);
		gets (buf);

		copy_fmatrix (t, Ai1, dim);
		copy_fmatrix (Ai, t, dim);

	}
}

vpdefla_bad (fmatrix A, int *dim, struct data_fmatrix *valp, struct data_fmatrix *vecp)
{
int i, j, n;
struct data_fmatrix Ai[1];
real bufAi[MAXDIM*MAXDIM];
real l;
struct data_fmatrix u[1];
real bufu[MAXDIM];
struct op_fmatrix tu[1], u2[1], w[1], wtu[1], lwtu[1], Ai1[1];

	n = dim[0];

	valp->f = data_f;
	valp->dim[0] = n;
	valp->dim[1] = 1;

	vecp->f = data_f;
	vecp->dim[0] = n;
	vecp->dim[1] = n;

	Ai->data = bufAi;
	copy_fmatrix (Ai, A, dim);

	u->data = bufu;

	for (i=0; i<n; i++)
	{
		vppuissance (Ai, dim, &l, u);
		set_fmatrix (valp, 0, i, l);
		for (j=0; j<n; j++)
			set_fmatrix (vecp, j, i, fmget (u, j, 0));

		tu->a[0] = u;
		tu->f = transpose_f;

		u2->a[0] = tu;
		u2->a[1] = u;
		u2->f = mult_f;

		w->a[0] = u;
		w->fact = 1 / fmget (u2, 0, 0);
		w->f = mult_scal_f;

		wtu->a[0] = w;
		wtu->a[1] = tu;
		wtu->f = mult_f;

		lwtu->fact = -l;
		lwtu->f = mult_scal_f;
		lwtu->a[0] = wtu;

		Ai1->a[0] = Ai;
		Ai1->f = plus_f;
		Ai1->a[1] = lwtu;

		copy_fmatrix (Ai, Ai1, dim);

	}
}
/* x2 = v2 + l1/(l2-l1) (w,v2) x1 */

#define tprint_fmatrix(a,d)
#define tprintf(f,x)
#define tgets(b)

vppuissance_sym (fmatrix A, int *dim, struct data_fmatrix *v)
{
struct data_fmatrix x[1], t[1];
real bufx[MAXDIM*MAXDIM], buft[MAXDIM*MAXDIM];
struct op_fmatrix Ax[1], Axt[1], Ax2[1], x1[1], d[1], dt[1], d2[1], mx[1];
int i, n;
int dimvect[2];
char buf[10];

	print_fmatrix (A, dim);

	n = dim[0];

	dimvect[0] = n;
	dimvect[1] = 1;

	x->f = data_f;
	x->dim[0] = n;
	x->dim[1] = 1;
	x->data = bufx;

	t->f = data_f;
	t->dim[0] = n;
	t->dim[1] = 1;
	t->data = buft;

	for (i=0; i<n; i++)
		set_fmatrix (x, i, 0, 1);

	for (;;)
	{
		/*
		print_fmatrix (x, dimvect);
		gets (buf);
		*/

		Ax->a[0] = A;
		Ax->f = mult_f;
		Ax->a[1] = x;

		tprint_fmatrix (Ax, dimvect);

		Axt->a[0] = Ax;
		Axt->f = transpose_f;

		Ax2->a[0] = Axt;
		Ax2->f = mult_f;
		Ax2->a[1] = Ax;

		tprintf ("%lf\n", fmget (Ax2, 0, 0));

		x1->a[0] = Ax;
		x1->f = mult_scal_f;
		x1->fact = 1 / sqrt (fmget (Ax2, 0, 0));

		tprint_fmatrix (x1, dimvect);

		d->a[0] = x1;
		d->f = plus_f;
		d->a[1] = mx;

		mx->a[0] = x;
		mx->f = mult_scal_f;
		mx->fact = -1;

		dt->a[0] = d;
		dt->f = transpose_f;

		d2->a[0] = dt;
		d2->f = mult_f;
		d2->a[1] = d;

		if (fmget (d2, 0, 0) < 1e-15)
		{
			copy_fmatrix (v, x1, dimvect);
			break;
		}

		tprint_fmatrix (x1, dimvect);

		copy_fmatrix (t, x1, dimvect);

		copy_fmatrix (x, t, dimvect);

		tprint_fmatrix (x, dimvect);
		tgets (buf);

	}
}

vpdefla_sym (fmatrix A0, int *dim, struct data_fmatrix *valp, struct data_fmatrix *vecp)
{
int i, j, n;
real l;
struct data_fmatrix A[1], A1[1], v[1];
real bufA[MAXDIM*MAXDIM], bufv[MAXDIM*MAXDIM], bufA1[MAXDIM*MAXDIM];
struct op_fmatrix t[1], vt[1], vvt[1], mlvvt[1], Amlvvt[1];
char buf[10];

	n = dim[0];

	A->data = bufA;
	copy_fmatrix (A, A0, dim);

	v->f = data_f;
	v->dim[0] = n;
	v->dim[1] = 1;
	v->data = bufv;

	A1->f = data_f;
	A1->dim[0] = dim[0];
	A1->dim[1] = dim[1];
	A1->data = bufA1;

	for (i=0; i<n; i++)
	{
		print_fmatrix (A, dim);
		gets (buf);

		vppuissance_sym (A, dim, v);

		t->a[0] = A;
		t->f = mult_f;
		t->a[1] = v;

		l = fmget (t, 0, 0) / fmget (v, 0, 0);

		set_fmatrix (valp, i, 0, l);
		for (j=0; j<n; j++)
			set_fmatrix (vecp, j, i, fmget (v, j, 0));

		/* A = A - l v vt */

		vt->a[0] = v;
		vt->f = transpose_f;

		vvt->a[0] = v;
		vvt->f = mult_f;
		vvt->a[1] = vt;

		mlvvt->fact = -l;
		mlvvt->f = mult_scal_f;
		mlvvt->a[0] = vvt;

		printf ("%lf\n", l);
		print_fmatrix (A, dim);
		print_fmatrix (mlvvt, dim);

		Amlvvt->a[0] = A;
		Amlvvt->f = plus_f;
		Amlvvt->a[1] = mlvvt;

		print_fmatrix (Amlvvt, dim);

		copy_fmatrix (A1, Amlvvt, dim);

		copy_fmatrix (A, A1, dim);
	}
}

main ()
{
real bufa[60], bufm[60], bufvalp[MAXDIM], bufvecp[MAXDIM*MAXDIM];
struct data_fmatrix a[1], m[1], A[1], valp[1], vecp[1];
struct op_fmatrix b[1], c[1], e[1], h[1];
real d;
int i, j;

/*
	print_fmatrix (fmId, dim1);

	a->data = bufa;
	m->data = bufm;

	read_fmatrix (a);
	print_fmatrix (a, a->dim);

	printf ("det a = %lf\n", d = det (a, a->dim[0]));

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
*/
	A->data = bufA;
	A->f = data_f;
	A->dim[0] = 4;
	A->dim[1] = 4;
	print_fmatrix (A, A->dim);

	/*vppuissance (A, A->dim);*/

	valp->f = data_f;
	valp->dim[0] = 4;
	valp->dim[1] = 1;
	valp->data = bufvalp;

	vecp->f = data_f;
	vecp->dim[0] = 4;
	vecp->dim[1] = 4;
	vecp->data = bufvecp;

	vpdefla (A, A->dim, valp, vecp);

	printf ("\nR‚sultats : \n");
	print_fmatrix (valp, valp->dim);
	print_fmatrix (vecp, vecp->dim);

	b->a[0] = A;
	b->f = mult_f;
	b->a[1] = vecp;
	print_fmatrix (b, A->dim);

	a->data = bufa;
	a->dim[0] = A->dim[0];
	a->dim[1] = A->dim[1];
	a->f = data_f;
	for (i=0; i<A->dim[0]; i++)
	for (j=0; j<A->dim[0]; j++)
		set_fmatrix (a, i, j, fmget (valp, j, 0) * fmget (vecp, i, j));
	print_fmatrix (a, A->dim);
}