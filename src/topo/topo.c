
#include <stdio.h>
#include <math.h>

#define pi 3.1415926

#define real double

struct point
{
 real x;
 real y;
};

#define X(A) ((A).x)
#define Y(A) ((A).y)

typedef struct point point;

/*
real floor (real x)
{
 if (x>=0)
  return (real)(int)x;
 else if (x==(int)x)
  return x;
 else
  return (real)(int)x-1;
}
*/

real pm200 (real a)
{
real b, c;
 a = a+200;
 c = a/400;
 b = 400 * (c-floor(c));
 b = b-200;
 return b;
}

real angle (point A, point B)
{
real x, y, dx, dy;
 dx = X(B)-X(A);
 dy = Y(B)-Y(A);
 if (dy>0)
  return atan(dx/dy);
 else if (dy<0)
  return atan(dx/dy)+pi;
 else if (dx>0)
  return pi/2;
 else if (dx<0)
  return -pi/2;
 else
  return 0;
}

real angleg (point A, point B)
{
real a;
 a=angle(A,B)/pi*200;
 return a;
}

real dist (point A, point B)
{
real dx, dy;
 dx = X(B) - X(A);
 dy = Y(B) - Y(A);
 return sqrt (dx*dx + dy*dy);
}

real edist (point B, point A, real d, real v)
{
real u, r;
 u = (dist(A,B) - d) / v;
 r = u*u;
 return r;
}

int confondus (point A, point B)
{
 return X(A)==X(B) && Y(A)==Y(B);
}

real eangle (point B, point A1, point A2, real a)
{
real u, ba2, ba1;
/*
 u=pm200(angleg(B,A2)-angleg(B,A1)-a)/100;
*/
 if (confondus(B,A1) || confondus(B,A2))
  return 1;
 ba2 = angleg (B, A2);
 ba1 = angleg (B, A1);
 u = pm200 (ba2 - ba1 - a)/100;
 return u*u;
}

#define MAX 30
point A[MAX], C, B[MAX];
int n=2; /* nombre de points A connus */
int npt=3; /* nombre de points B inconnus */
#define M A[0]
#define N1 A[1]
#define B1 B[0]
#define B2 B[1]
#define B3 B[2]
real v;


/* points connus et inconnus */
int np;
struct
{
 char nom[30];
 point p;
 int ipi; /* indice dans le tableau B des points inconnus */
} tp[MAX];

/* donnees */
int nd;
struct
{
 char type; /* D=distance, A=angle */
 int ip[4];
 real val;
} td[60];


init ()
{
int k;
real x, y, dx, dy;
 X(M)=522257.68; Y(M)=150838.74;
 X(N1)=521205.59; Y(N1)=150804.31;
 /* X(C)=(X(M)+X(N))/2; Y(C)=(Y(M)+Y(N))/2; */
 x=0; y=0; v=0;
 for (k=0; k<n; k++)
 {
  x += X(A[k]);
  y += Y(A[k]);
 }
 X(C) = x/n;
 Y(C) = y/n;
 for (k=0; k<n; k++)
 {
  dx = X(A[k]) - X(C);
  dy = Y(A[k]) - Y(C);
  v += sqrt(dx*dx + dy*dy);
 }
 v = v/n;
 /*v = 372;*/ /*test*/
 printf ("v=%lf\n", v);
}

#define POINT(k) ((tp[td[i].ip[k]].ipi>=0)?B[tp[td[i].ip[k]].ipi]:tp[td[i].ip[k]].p)

real ecart (point B[])
{
real e, e1;
int i;
 e=0;
 for (i=0; i<nd; i++)
 {
  if (td[i].type == 'D')
  {
   /*
   e1 = edist (B[tp[td[i].ip[0]].ipi],
	       B[tp[td[i].ip[1]].ipi],
	       td[i].val, v);
   */
   e1 = edist (POINT(0), POINT(1), td[i].val, v);
   e += e1;
  }
  else if (td[i].type == 'A')
  {
   /*
   e1 = eangle (B[tp[td[i].ip[0]].ipi],
	       B[tp[td[i].ip[1]].ipi],
	       B[tp[td[i].ip[2]].ipi],
	       td[i].val);
   */
   e1 = eangle (POINT(0), POINT(1), POINT(2), td[i].val);
   e += e1;
  }
 }
 return e;
}

real ecart1 (point B[])
{
real e;
real ed12, ed23, ea1m2, ea21m, ea2m3, ea32m, ea2mn;
 ed12 = edist (B1, B2, 47.163, v);
 ed23 = edist (B2, B3, 73.3513, v);
 ea1m2 = eangle (B1, M, B2, 63.8763);
 ea21m = eangle (B2, B1, M, 105.4169);
 ea2m3 = eangle (B2, M, B3, 80.9726);
 ea32m = eangle (B3, B2, M, 66.2236);
 ea2mn = eangle (B2, M, N1, 320.5313);

 e = ed12 + ed23 + ea1m2 + ea21m + ea2m3 + ea32m + ea2mn;

/*
e = edist(B1,B2,47.163,v);
e=e+edist(B2,B3,73.3513,v);
e=e+eangle(B1,M,B2,63.8763);
e=e+eangle(B2,B1,M,105.4169);
e=e+eangle(B2,M,B3,80.9726);
e=e+eangle(B3,B2,M,66.2236);
e=e+eangle(B2,M,N1,320.5313);
*/


return e;
}

int suivant (point D[], int npt, int nq1)
{
int k;
 k = 0;
 for (;;)
 {
  if (k >= npt)
   return 0;
  if (X(D[k]) < nq1-1)
  {
   X(D[k])++;
   return 1;
  }
  else
  {
   X(D[k]) = 0;
   if (Y(D[k]) < nq1-1)
   {
    Y(D[k])++;
    return 1;
   }
   else
   {
    Y(D[k]) = 0;
    k++;
   }
  }
 }
}


real nq, nq1, eq;
int k, t, r, d;
point N[MAX]; /* nouveaux points B */
point D[MAX];
point T[MAX]; /* point teste */
real m, e;
char buf[100];

point Z[MAX];

resol ()
{
 /*
 X(Z[0]) = 522237.112;
 Y(Z[0]) = 150739.5329;
 X(Z[1]) = 522281.205;
 Y(Z[1]) = 150756.2686;
 X(Z[2]) = 522342.6934;
 Y(Z[2]) = 150796.2583;

 e = ecart (Z);
 printf ("\ne=%f\n", e);
 */

 nq = 0.5;
 nq1 = 2*nq+1;
 eq = v/nq;

 for (k=0; k<npt; k++)
  B[k] = C;

 for (t=1; t<20000; t++)
 {
  for (k=0; k<npt; k++)
   N[k] = B[k];
  for (k=0; k<npt; k++)
  {
   X(D[k]) = 0;
   Y(D[k]) = 0;
  }
  m = ecart (B);
  for (;;)
  {
   for (k=0; k<npt; k++)
   {
    X(T[k]) = X(B[k]) + (X(D[k]) - nq) * eq;
    Y(T[k]) = Y(B[k]) + (Y(D[k]) - nq) * eq;
   }
   e = ecart (T);
   if (e < m)
   {
    m = e;
    for (k=0; k<npt; k++)
     N[k] = T[k];
   }
   r = suivant (D, npt, nq1);
   if (!r)
    break;
  }
  d=0;
  for (k=0; k<npt; k++)
  {
   if (X(N[k]) != X(B[k]) || Y(N[k]) != Y(B[k]))
   {
    d=1;
    break;
   }
  }
  if (!d)
  {
   eq *= 0.5;
   if (eq < 0.001)
    break;
  }
  /*
  else
  {
   eq *= 1.5;
  }
  */

/*
  printf ("\nt=%d eq=%lf m=%lf\n",t,eq,m);
  for (k=0; k<npt; k++)
   printf (" %d : %lf %lf\n",k,X(N[k]),Y(N[k]));
  gets(buf);
*/
  for (k=0; k<npt; k++)
   B[k] = N[k];
 }
/*
 printf ("\nt=%d m=%lf\n", t, m);
 for (k=0; k<npt; k++)
  printf (" %d : %lf %lf\n", k, X(B[k]), Y(B[k]));
*/
}

test()
{
point a[MAX];
int k;
real e;
 for (k=0; k<npt; k++)
 {
  X(a[k]) = 522103.63;
  Y(a[k]) = 150449.53;
 }
 e = ecart(a);
 printf ("e(a)=%lf\n",e);
}

struct instruction
{
 char op[30];
 int nparam;
 char param[10][30];
 char val[30];
};

analyser (char *buf, struct instruction *instr)
{
int i, j, k;
 i=0; j=0;
 for (;;)
 {
  /*
  if (buf[j] == "(")
  {
   instr->op[i]=0;
   break;
  }
  instr->op[i++]=buf[j++];
  */
  if (buf[i]>='A' && buf[i]<='Z')
   instr->op[j++]=buf[i++];
  else
   break;
 }
 instr->op[j]=0;
 i++; /* ( */
 j=0;
 k=0;
 for (;;)
 {
  if ((buf[i]>='A' && buf[i]<='Z') || (buf[i]>='0' && buf[i]<='9'))
   instr->param[k][j++]=buf[i++];
  else if (buf[i]==',')
  {
   instr->param[k][j]=0;
   k++;
   j=0;
   i++;
  }
  else
  {
   instr->param[k][j]=0;
   instr->nparam=k+1;
   break;
  }
 }
 i++;
 if (buf[i]=='=')
  i++;
 strcpy (instr->val, buf+i);
}

#define NDEF -1000000000

int numpoint (char *nom)
{
int i;
 for (i=0; i<np; i++)
  if (!strcmp(tp[i].nom,nom))
   return i;
 strcpy(tp[np].nom,nom);
 tp[np].p.x = NDEF;
 tp[np].p.y = NDEF;
 tp[np].p.ipi = -1;
 np++;
 return np-1;
}

traiter (struct instruction *instr)
{
 if (!strcmp (instr->op, "X"))
  tp[numpoint(instr->param[0])].p.x = atof(instr->val);
 else if (!strcmp (instr->op, "Y"))
  tp[numpoint(instr->param[0])].p.y = atof(instr->val);
 else if (!strcmp (instr->op, "D"))
 {
  td[nd].type = 'D';
  td[nd].ip[0] = numpoint(instr->param[0]);
  td[nd].ip[1] = numpoint(instr->param[1]);
  td[nd].val = atof(instr->val);
  nd++;
 }
 else if (!strcmp (instr->op, "ANGLE"))
 {
  td[nd].type = 'A';
  td[nd].ip[0] = numpoint(instr->param[1]);
  td[nd].ip[1] = numpoint(instr->param[0]);
  td[nd].ip[2] = numpoint(instr->param[2]);
  td[nd].val = atof(instr->val);
  nd++;
 }
}

int connu (point p)
{
 return (X(p)!=NDEF && Y(p)!=NDEF);
}

init_resoudre ()
{
int i;
real x, y, dx, dy;
 x=0; y=0; n=0;
 for (i=0; i<np; i++)
 {
  if (connu(tp[i].p))
  {
   x += X(tp[i].p);
   y += Y(tp[i].p);
   n++;
  }
 }
 X(C)=x/n;
 Y(C)=y/n;
 v = 0;
 for (i=0; i<np; i++)
 {
  if (connu(tp[i].p))
  {
   dx = X(tp[i].p)-X(C);
   dy = Y(tp[i].p)-Y(C);
   v += sqrt(dx*dx+dy*dy);
  }
 }
 v=v/n;
}

resoudre ()
{
int i, ipi;
 init_resoudre ();
 npt = np - n;
 ipi=0;
 for (i=0; i<np; i++)
 {
  if (connu(tp[i].p))
   tp[i].ipi=-1;
  else
   tp[i].ipi=ipi++;
 }
 resol ();
 for (i=0; i<np; i++)
 {
  if (tp[i].ipi>=0)
   tp[i].p = B[tp[i].ipi];
 }
 for (i=0; i<np; i++)
 {
  if (tp[i].ipi>=0)
  {
   printf ("X(%s)=%lf\nY(%s)=%lf\n",
    tp[i].nom, X(tp[i].p), tp[i].nom, Y(tp[i].p));
  }
 }
 nd = 0;
}

interp ()
{
char buf[300];
struct instruction instr[1];
FILE *f;
 /*f = fopen ("topo.txt", "r");*/
 f = stdin;

 for (;;)
 {
  fgets (buf, sizeof(buf), f);
  buf[strlen(buf)-1]=0;
  if (!strcmp(buf,"TERMINER") || feof(f))
   break;
  if (!strcmp(buf,"RESOUDRE"))
   resoudre();
  else
  {
   analyser (buf, instr);
   traiter (instr);
  }
 }
}

main ()
{
struct instruction instr[1];
 analyser ("ANGLE(M,A,B)=63.8763", instr);
 interp();
}

main1()
{
 /* printf("%f %f %f",floor(5.2),floor(-5.2),floor(-4)); */
 init();
 /*test();*/
 resol();
}