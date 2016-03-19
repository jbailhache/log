
#include <math.h>
#include <stdio.h>

#define MAXDIM 3

typedef float real;

/* #define DIV(x,y) ((y)?((x)/(y)):fprintf (stderr, "%s(%d) : Division de %8.3f par 0\n", x, __FILE__, __LINE__)) */

#define DIV(x,y) divi(x,y,__FILE__,__LINE__)
/*
real divi (real x, real y, char *file, int line)
{
	if (y != 0.0)
		return x / y;
	fprintf (stderr, "%s(%d) : Division de %8.3f par 0.\n",
		file, line, x);
	return 1e10;
}
*/
struct matrix
{
	int ndim;
	int dim[MAXDIM];
	real *val;
};

typedef struct matrix *matrix;

#define SETBUF(a,buf) (a)->val = buf

#define MATRIX(a,size,n,p) \
	{ \
	struct matrix a[1]; \
	real buf_##a[size]; \
		a->val = buf_##a; \
		a->dim[0] = n; \
		a->dim[1] = p;

#define ENDMAT }

#define DIM(a,n) (a)->dim[n]
#define VAL1(a,i) (a)->val[i]
#define VAL2(a,i,j) ((a)->val[(i)*(a)->dim[1]+(j)])


