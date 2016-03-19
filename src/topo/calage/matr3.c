
#include "matr.h"

/*
#include <math.h>

#define real double

struct matr
{
 int nl, nc;
 real data[256];
};

typedef struct matr *matr;

#define coef1(m,i,j) ((i<0||i>m->nl||j<0||j>m->nc)?\
 (printf("%s(%d): out of range of %s\n",__FILE__,__LINE__,#m),0):\
 m->data[i*m->nc+j])

#define coef(m,i,j) ((m)->data[(i)*(m)->nc+(j)])

*/

transpose (matr a, matr b)
{
int i, j;
 b->nl = a->nc;
 b->nc = a->nl;
 for (i=0; i<b->nl; i++)
 for (j=0; j<a->nc; j++)
  coef(b,i,j)=coef(a,j,i);
}

addmatr (matr a, matr b, matr c)
{
int i, j;
 c->nl = a->nl;
 c->nc = a->nc;
 for (i=0; i<c->nl; i++)
 for (j=0; j<c->nc; j++)
  coef(c,i,j) = coef(a,i,j) + coef(b,i,j);
}

psm (real x, matr a, matr b)
{
int i, j;
 b->nl = a->nl;
 b->nc = a->nc;
 for (i=0; i<b->nl; i++)
 for (j=0; j<b->nc; j++)
  coef(b,i,j) = x * coef(a,i,j);
}

real norme (matr a)
{
int i, j;
real s;
 s = 0;
 for (i=0; i<a->nl; i++)
 for (j=0; j<a->nc; j++)
  s += coef(a,i,j) * coef(a,i,j);
 /*intf("s=%lf\n");*/
 return sqrt(s);
}

id (int n, matr a)
{
int i, j;
 a->nl = n;
 a->nc = n;
 for (i=0; i<a->nl; i++)
 for (j=0; j<a->nc; j++)
  coef(a,i,j) = (i==j) ? 1 : 0;
}

prodmatr (matr a, matr b, matr c)
{
int i,j,k;
 if (a->nc != b->nl)
  printf ("prodmatr: incompatible size\n");
 c->nl = a->nl;
 c->nc = b->nc;
 for (i=0; i<c->nl; i++)
 for (j=0; j<c->nc; j++)
 {
  coef(c,i,j) = 0;
  for (k=0; k<a->nc; k++)
   coef(c,i,j) += coef(a,i,k) * coef(b,k,j);
 }
}

real normeligne (matr a)
{
real max, s;
int i, j;
 max = 0;
 for (i=0; i<a->nl; i++)
 {
  s = 0;
  for (j=0; j<a->nc; j++)
   s += fabs(coef(a,i,j));
  if (s>max)
   max=s;
 }
 return max;
}

real normecolonne (matr a)
{
real max, s, aij, absaij;
int i, j;
 max = 0;
 for (j=0; j<a->nc; j++)
 {
  s = 0;
  for (i=0; i<a->nl; i++)
  /* s += fabs(coef(a,i,j)); */
  {
   aij = coef(a,i,j);
   absaij = fabs(aij);
   s += absaij;
  }
  if (s>max)
   max=s;
 }
 return max;
}

invpr (matr A, matr B)
{
real d, x;
struct matr I[1], tA[1], BA[1], mBA[1], E[1], IpE[1], B1[1];
real e, e1;
int i;

 d = A->nl;
 id (d, I);
 x = 1/(normeligne(A)*normecolonne(A));
 transpose (A, tA);
 psm (x, tA, B);
 prodmatr (B, A, BA);
 psm (-1, BA, mBA);
 addmatr (I, mBA, E);
 e = norme(E);
 i = 0;
 while (norme(E) > 0.0000001)
 {
  addmatr (I, E, IpE);
  prodmatr (IpE, B, B1);
  psm (1, B1, B);
  prodmatr (B, A, BA);
  psm (-1, BA, mBA);
  addmatr (I, mBA, E);
  e1 = e;
  e = norme (E);
  i++;
  if (i>6 && e>=e1)
   break;
 }
}

inverse (matr a, matr b)
{
 if (a->nl == 0 || a->nc == 0 || a->nl != a->nc)
 {
  b->nl = 0;
  b->nc = 0;
  printf ("Matrice non inversible\n");
 }
 else
 {
  invpr (a, b);
 }
}

printmatr (matr a)
{
int i, j;
 printf ("%d lignes, %d colonnes\n", a->nl, a->nc);
 for (i=0; i<a->nl; i++)
 {
  for (j=0; j<a->nc; j++)
   printf (" %9.3lf", coef(a,i,j));
  printf ("\n");
 }
}

testmatr ()
{
struct matr A[1], B[1];
real b00, b01, b10, b11;

 A->nl=2;
 A->nc=2;
 coef(A,0,0)=1;
 coef(A,0,1)=2;
 coef(A,1,0)=3;
 coef(A,1,1)=4;
 inverse(A,B);

 /*
 printf ("%lf %lf\n%lf %lf\n",
  coef(B,0,0),coef(B,0,1),coef(B,1,0),coef(B,1,1));
 */

 b00 = coef(B,0,0);
 b01 = coef(B,0,1);
 b10 = coef(B,1,0);
 b11 = coef(B,1,1);

 printf ("%lf %lf\n%lf %lf\n", b00, b01, b10, b11);
}
