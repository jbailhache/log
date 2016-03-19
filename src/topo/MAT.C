
#include <math.h>

#define real double

int inv1 (int n, real a[], real b[])
{
int h, i, j, k;
real t;
real p[300];
 for (i=0; i<n; i++)
 for (j=0; j<n; j++)
  p[i*n+j]=a[i*n+j];
 for (j=0; j<n; j++)
 {
  for (i=0; i<n; i++)
   b[i*n+j]=0;
  b[j*n+j]=1;
 }
 t = 0;
 for (j=0; j<n; j++)
 {
  t = 1/p[j*n+j];
  for (i=0; i<n; i++)
  {
   p[j*n+i] *= t;
   b[j*n+i] *= t;
  }
  for (h=0; h<n; h++)
  {
   if (h != j)
    t = -p[i*n+j];
   for (i=0; i<n; i++)
   {
    p[h*n+i] += t * p[j*n+i];
    b[h*n+i] += t * b[j*n+i];
   }
  }
 }
}

real normeligne (int n, real *a)
{
int i, j;
real m, s;
 m = 0;
 for (i=0; i<n; i++)
 {
  s = 0;
  for (j=0; j<n; j++)
   s += fabs(a[i*n+j]);
  if (s > m)
   m = s;
 }
 return m;
}

real normecolonne (int n, real *a)
{
int i, j;
real m, s, a1, f1;
 m = 0;
 for (j=0; j<n; j++)
 {
  s = 0;
  for (i=0; i<n; i++)
  {
   a1 = a[i*n+j];
   f1 = fabs(a1);
   s = s + f1;
   /* s += fabs(a[i*n+j]); */
  }
  if (s > m)
   m = s;
 }
 return m;
}

transpose (int n, int p, real *a, real *b)
{
int i, j;
 for (i=0; i<n; i++)
 for (j=0; j<p; j++)
  b[j*n+i] = a[i*p+j];
}

id (int n, real *a)
{
int i, j;
 for (i=0; i<n; i++)
 for (j=0; j<n; j++)
 {
  if (i==j)
   a[i*n+j]=1;
  else
   a[i*n+j]=0;
 }
}

real norme (int n, real *a)
{
int i, j;
real s;
 s = 0;
 for (i=0; i<n; i++)
 for (j=0; j<n; j++)
  s += a[i*n+j] * a[i*n+j];
 return sqrt(s);
}

comblinmat (int n, int p, real x, real *a, real y, real *b, real *c)
{
int i, j;
 for (i=0; i<n; i++)
 for (j=0; j<p; j++)
  c[i*p+j] = x * a[i*p+j] + y * b[i*p+j];
}

prodscalmat (real x, int n, int p, real *a, real *b)
{
int i, j;
 for (i=0; i<n; i++)
 for (j=0; j<n; j++)
  b[i*p+j] = x * a[i*p+j];
}

prodmat (int n, int p, int q, real *a, real *b, real *c)
{
int i, j, k;
 for (i=0; i<n; i++)
 for (k=0; k<q; k++)
 {
  c[i*q+k] = 0;
  for (j=0; j<p; j++)
   c[i*q+k] += a[i*p+j] * b[j*q+k];
 }
}

int inv (int n, real *A, real *B)
{
real E[300], T[300], I[300], U[300];
real x;
int i, j, t;
real e, e1;
/*
 printf ("Matrice a inverser : \n");
 for (i=0; i<n; i++)
 {
  for (j=0; j<n; j++)
   printf (" %8.1lf", A[i*n+j]);
  printf ("\n");
 }
*/
 id (n, I);
 transpose (n, n, A, B);
 x = 1 / (normeligne(n,A) * normecolonne(n,A));
 comblinmat (n, n, x, B, 0, B, B);
 prodmat (n, n, n, B, A, E);
 for (i=0; i<n; i++)
 for (j=0; j<n; j++)
 {
  if (i==j)
   E[i*n+j] = 1-E[i*n+j];
  else
   E[i*n+j] = -E[i*n+j];
 }
 e = norme (n, E);
 t = 0;
 while (norme(n,E)>0.0000001)
 {
  comblinmat (n, n, 1, I, 1, E, T);
  prodmat (n, n, n, T, B, U);
  comblinmat (n, n, 1, U, 0, U, B);
  prodmat (n, n, n, B, A, T);
  comblinmat (n, n, 1, I, -1, T, E);
  e1 = e;
  e = norme (n, E);
  t++;
  if (t > 6 && e > e1)
   break;
  if (t > 30000)
   return 0;
 }
 return 1;
}

printmat (int n, int p, real *a)
{
int i, j;
char buf[10];

 printf (" Matrice %dx%d : \n", n, p);
 for (i=0; i<n; i++)
 {
  for (j=0; j<p; j++)
   printf (" %7.4lf", a[i*p+j]);
  printf ("\n");
 }
 gets(buf);

}


real a[3][3] = { {1,2,3},{4,5,6},{7,8,10}};
real b[9];

main1 ()
{
 inv(3,a,b);
}
