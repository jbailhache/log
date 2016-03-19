
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <sys/timeb.h>
#include "matr.h"

/* #define GRAPH  */

#ifndef GRAPH
#define width 300
#define height 200
#endif

#define pi 3.1415926

#define real double

#define DIM 3

#define NDEF -1000000000
struct point
{
 real xyz[DIM];
};

#define X(A) ((A).xyz[0])
#define Y(A) ((A).xyz[1])
#define Z(A) ((A).xyz[2])

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

point C;

real xmin, xmax, ymin, ymax, xspan, yspan, xscale, yscale, scale;
real echelle;

#ifdef GRAPH
extern unsigned int width, height;
#endif
/*
extern Display *display;
extern Window win;
extern GC gc;
*/

int xgraph (real x)
{
 return width/2 + scale * (x - X(C));
}
int ygraph (real y)
{
 return height/2 - scale * (y - Y(C));
}

#ifdef GRAPH

plotpoint (point p)
{
 plot (xgraph(X(p)), ygraph(Y(p)));
}

plotcircledpoint (point p)
{
 plotpoint(p);
 draw_circle (xgraph(X(p)), ygraph(Y(p)), 1);
}

plotpointname (point p, char *name)
{
 plotpoint (p);
 draw_string (xgraph(X(p)), ygraph(Y(p)), name);
}

#else

plotpoint (point p) {}
plotcircledpoint (point p) {}
plotpointname (point p, char *name) {}

#endif

real pm200 (real a)
{
real b, c;
 a = a+200;
 c = a/400;
 b = 400 * (c-floor(c));
 b = b-200;
 return b;
}

real angle0_400 (real a)
{
real b, c;
 c = a/400;
 b = 400 * (c-floor(c));
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

real angle3g (point A, point B, point C)
{
 return angle0_400(angleg(A,C)-angleg(A,B));
}

real dist (point A, point B)
{
real dx, dy;
 dx = X(B) - X(A);
 dy = Y(B) - Y(A);
 return sqrt (dx*dx + dy*dy);
}

/*#define NEW*/

real edist (point B, point A, real d, real v)
{
real u, r;
 /*printf ("_%lf_",v);*/
 u = (dist(A,B) - d) / v;
 r = u*u;
 return r;
}

int confondus (point A, point B)
{
 return X(A)==X(B) && Y(A)==Y(B);
}

real eangle (point B, point A1, point A2, real a, real sigma)
{
real u, ba2, ba1;
/*
 u=pm200(angleg(B,A2)-angleg(B,A1)-a)/100;
*/
 if (confondus(B,A1) || confondus(B,A2))
#ifdef NEW
  return 10000/(sigma*sigma);
#else
  return 1;
#endif
 ba2 = angleg (B, A2);
 ba1 = angleg (B, A1);
 /* u = pm200 (ba2 - ba1 - a)/100; */
 u = pm200 (ba2 - ba1 - a) / sigma;
 return u*u;
}

/* autre formule possible :
    e = k (1 - cos ((ba2 - ba1 - a) / 200 * pi))
*/

real egisement (point B, point A, real g, real sigma)
{
real u;
 if (confondus(B,A))
  return 1;
 u = pm200(angleg(B,A)-g)/sigma;
 return u*u;
}

#define MAX 1000
point A[MAX], /*C,*/ B[MAX];
int n=2; /* nombre de points A connus */
int npt=3; /* nombre de points B inconnus */
#define M A[0]
#define N1 A[1]
/*#define B1 B[0]*/
#define B2 B[1]
#define B3 B[2]
real v;

real h=0;
real CL=0;
real coef=1;
/*
real eps=1.5;
real Tx=20;
*/
real eps=0.0015;
real Tx=0.2;

real paires=2;
real sigmad = 0.002;
real sigmad1 = 0.00002;
real sigmaangle = 0.003;
real eqstop = 0.0001;
real deplinit = 0.0;

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
struct donnee
{
 char type; /* D=distance, A=angle */
 int ip[4];
 real val;
 real coef;
 real sigma;
} td[300];

#if 0
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
#endif

#define POINT(k) ((tp[td[i].ip[k]].ipi>=0)?B[tp[td[i].ip[k]].ipi]:tp[td[i].ip[k]].p)

real ecart (point B[])
{
real e, e1;
int i;
real sigma;
 e=0;
 for (i=0; i<nd; i++)
 {
  if (td[i].type == 'D' && connu(POINT(0)) && connu(POINT(1)))
  {
   /*
   e1 = edist (B[tp[td[i].ip[0]].ipi],
	       B[tp[td[i].ip[1]].ipi],
	       td[i].val, v);
   */
#ifdef NEW
   if (td[i].sigma)
    sigma = td[i].sigma;
   else
    sigma = sigmad + sigmad1 * td[i].val;
#else
   sigma = v;
#endif
   e1 = edist (POINT(0), POINT(1), td[i].val, sigma);

   /*e1 = edist (POINT(0), POINT(1), td[i].val, v);*/
   e += e1*td[i].coef;
  }
  else if (td[i].type == 'A'
	&& connu(POINT(0)) && connu(POINT(1)) && connu(POINT(2)))
  {

   /*
   e1 = eangle (B[tp[td[i].ip[0]].ipi],
	       B[tp[td[i].ip[1]].ipi],
	       B[tp[td[i].ip[2]].ipi],
	       td[i].val);
   */
#ifdef NEW
   if (td[i].sigma)
    sigma = td[i].sigma;
   else
    sigma = sigmaangle;
#else
   sigma = 100;
#endif
   e1 = eangle (POINT(0), POINT(1), POINT(2), td[i].val, sigma);
   e += e1*td[i].coef;
  }
  else if (td[i].type == 'G')
  {
#ifdef NEW
   if (td[i].sigma)
    sigma = td[i].sigma;
   else
    sigma = sigmaangle;
#else
   sigma = 100;
#endif
   e1 = egisement (POINT(0), POINT(1), td[i].val, sigma);
   e += e1*td[i].coef;
  }
 }
 return e;
}

#if 0
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
#endif

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

/* #define TEST1 */

int suivant1 (int *i, point D[], int npt, int nq1)
{
int k;

  k = *i;
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
#ifdef TEST1
   else
   {
    X(D[k]) = ((real)nq1-1.0)/2.0;
    Y(D[k]) = ((real)nq1-1.0)/2.0;
    return 0;
   }
#else
   else if (k < npt-1)
   {
    /*
    Y(D[k]) = 0;    
    k++;
    */  
    
    X(D[k]) = ((real)nq1-1.0)/2.0;
    Y(D[k]) = ((real)nq1-1.0)/2.0;
   
    (*i)++;

    X(D[k+1]) = 0;
    Y(D[k+1]) = 0;

   }
   else
   {
    return 0;
   }
#endif
  }

 if (*i >= npt)
 {
 
 } 
}

real nq, nq1, eq;
int k, t, r, d;
point N[MAX]; /* nouveaux points B */
point B1[MAX]; /* anciens points B */
point D[MAX];
point T[MAX]; /* point teste */
real m, e;
char buf[100];

point Z[MAX];

int flag_varsimul = 0;

#define TAILLEPOP 30
#define RN 1000


#ifdef GEN

resolgen ()
{
point pop[TAILLEPOP][MAX];
real tex[TAILLEPOP];
real te[TAILLEPOP];
int i, k, t;
real emin, emax;
int imin, imin1, imax, imax1;
real ex;
struct timeb tb;
int mut[TAILLEPOP];

 ftime(&tb);
 srandom (tb.millitm);
 
 ex = v;
 for (i=0; i<TAILLEPOP; i++)
  tex[i]=ex;

 for (i=0; i<TAILLEPOP; i++)
 for (k=0; k<npt; k++)
 {
  X(pop[i][k]) = X(C) + ex * (random()%RN)/RN - ex * (random()%RN)/RN;
  Y(pop[i][k]) = Y(C) + ex * (random()%RN)/RN - ex * (random()%RN)/RN;
  mut[i] = 1;
 }

 for (t=0; t<30000; t++)
 {
  
  emin=999999999;
  emax=0;
  imin=0;
  imin1=0;
  imax=0;
  imax1=0;
  for (i=0; i<TAILLEPOP; i++)
  {
   te[i] = ecart(pop[i]);
   if (te[i]>emax)
   {
    emax=te[i];
    imax1=imax;
    imax=i;
   }
   if (te[i]<emin)
   {
    emin=te[i];
    imin1=imin;
    imin=i;
   }
  }

  printf ("emin=%lf ex=%lf\n",emin,ex);

  for (k=0; k<npt; k++)
  {
   B[k] = pop[imin][k];
   plotpoint(B[k]);
   /* printf ("B[%d]: X=%lf Y=%lf\n", k, X(B[k]), Y(B[k])); */
  }

  if (emin < eqstop)
   break;
  /*ex = ex * emin / emax;*/
  ex = emax+emin+16*eqstop+ex/4; 
  if (mut[imin])
   ex *= 1.2;
  else
   ex /= 1.5;

  for (k=0; k<npt; k++)
  {
   X(pop[imax][k]) = X(pop[imin][k]) + tex[imin]*(random()%RN)/RN - tex[imin]*(random()%RN)/RN;
   Y(pop[imax][k]) = Y(pop[imin][k]) + tex[imin]*(random()%RN)/RN - tex[imin]*(random()%RN)/RN;
   tex[imax]=(ex + 2 * tex[imin] * 2 * (random()%RN)/RN)/3;
   mut[imax]=1;
   plotpoint (pop[imax][k]);
  }

  for (k=0; k<npt; k++)
  {
   X(pop[imax1][k]) = 0.5 * (X(pop[imin][k]) + X(pop[imin1][k]));
   Y(pop[imax1][k]) = 0.5 * (Y(pop[imin][k]) + Y(pop[imin1][k]));
   tex[imax1]=(tex[imin]+tex[imin1])/2;
   mut[imax1]=0;
  }

 }

  
}
#endif

resolexp ()
{
int pt;
real m, e, ex, sp;
int spm;
int i, j, k;
int d;
real ext[MAX];
int term; 
real exm;
int duree[MAX];
char buf[10];

/* ex = 10;  */
 ex = 2 * v; 
 printf ("# ex=%lf\n", ex);
 
 for (k=0; k<npt; k++)
 {
  ext[k]=ex;
  duree[k]=0;
  if (!connu(B[k]))
   B[k]=C;
 }

/* test */
 X(T[0]) = 513528.16; Y(T[0]) = 148410.07;
 X(T[1]) = 513743.91; Y(T[1]) = 149198.96;
 X(T[2]) = 513584.08; Y(T[2]) = 148785.01;
 X(T[3]) = 515033.54; Y(T[3]) = 149230.72;
 X(T[4]) = 514931.55; Y(T[4]) = 148388.74;
 X(T[5]) = 513526.03; Y(T[5]) = 148412.73;
 e = ecart(T);
 printf ("test: e=%lf\n", e);
 /*gets(buf);*/

 for (t=1; t<30000; t++)
 {
  for (k=0; k<npt; k++)
   B1[k]=B[k];

  spm = 0;
  /* for (sp=1; sp<=2; sp++) */
  for (pt=0; pt<npt; pt++)
  {
   m = ecart (B);
   for (k=0; k<npt; k++)
    N[k]=B[k];

   spm=0;
   for (sp=1; sp<=2; sp++)
   for (i=-1; i<=1; i+=2)
   for (j=-1; j<=1; j+=2)
   {
    for (k=0; k<npt; k++)
     T[k]=B[k];
    X(T[pt]) += i * sp * ext[pt];
    Y(T[pt]) += j * sp * ext[pt];
    /*printf ("\nexm=%lf m=%lf pt=%d sp=%d i=%2d j=%2d e=%lf ext=%lf",
     exm,m,pt,(int)sp,i,j,e,ext[pt]);*/
    e = ecart(T);
    if (e<m) 
    {
     m = e;
     spm = sp;
     for (k=0; k<npt; k++)
      N[k]=T[k];
    }
   }

   /*printf (" spm=%d ", spm);*/
   if (spm<1)
    duree[pt]++;
   else
    duree[pt]=0;
   /*
   if (spm<1 && duree[pt]<npt)
    spm=1;
   */
   /*if (spm == 2) spm = 4; */
    
   /*ext[pt] *= spm;*/

   switch ((int)spm)
   {
    case 0: ext[pt] /= 4; break;
    case 1: ext[pt] /= 2; break;
    case 2: ext[pt] *= 2; break;
   }

   for (k=0; k<npt; k++)
   {
    B[k]=N[k];
    plotpoint(B[k]);
   }
   
  }

  d = 0;
  for (k=0; k<npt; k++)
  {
   if (X(B[k]) != X(B1[k]) || Y(B[k]) != Y(B1[k]))
   {
    d += 1;
    /*break;*/
   }
  }
 
/*
  if (!d)
  {
   ex=ex/2;
   printf ("\rex=%lf ", ex);
   if (ex<0.0001)
    break;
  }
*/

/*
  ex = 0.4 * ex * (2.0 + (real)d / (real)npt);
  printf ("\rex=%lf m=%lf ",ex,m);
  fflush(stdout);
  if (ex < eqstop)
   break;
*/

/*
  term = 1;
  for (k=0; k<npt; k++)
  {
   if (ext[k] > eqstop)
    term=0;
  } 
  if (term)
   break;
*/


  exm=0;
  for (k=0; k<npt; k++)
  {
   if (ext[k]>exm)
    exm=ext[k];
  }
  if (exm<eqstop)
   break;
  printf ("\rexm=%lf m=%lf ",exm,m);
  fflush(stdout);

 }

}

void resol1 ()
{
int compteur_suivant;
char buf[10];
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
 if (deplinit > 0)
  eq = deplinit;
 else
  eq = v/nq;
 /*eq *= deplinit;*/


 for (k=0; k<npt; k++)
 {
  if (!connu(B[k]))
   B[k] = C;
 }
#ifdef TEST1
 compteur_suivant = -1; /*test*/
#endif

 for (t=1; t<20000; t++)
 {
#ifdef TEST1
  compteur_suivant++;		/********/
  if (compteur_suivant >= npt) 	/* test */
   compteur_suivant = 0;	/********/
  X(D[compteur_suivant]) = 0;
  Y(D[compteur_suivant]) = 0;
#endif

  for (k=0; k<npt; k++)
   N[k] = B[k];
/*
#define SUIVANT1
#ifndef SUIVANT1
*/
  if (flag_varsimul)
  {  
   for (k=0; k<npt; k++)
   {
    X(D[k]) = 0;
    Y(D[k]) = 0;
   }
  }
  else
  {
#ifndef TEST1
   compteur_suivant = 0; 
#endif
   for (k=0; k<npt; k++)
   {
    /*
    X(D[k]) = ((real)nq1+1.0)/2.0;
    Y(D[k]) = ((real)nq1+1.0)/2.0;
    */
    X(D[k]) = nq;
    Y(D[k]) = nq;
   }
   X(D[0]) = 0;
   Y(D[0]) = 0;
  }
  m = ecart (B);
  for (;;)
  {
   for (k=0; k<npt; k++)
   {
    X(T[k]) = X(B[k]) + (X(D[k]) - nq) * eq;
    Y(T[k]) = Y(B[k]) + (Y(D[k]) - nq) * eq;
    /*printf ("cs=%d T[%d]: X=%lf Y=%lf\n", 
     compteur_suivant, k, X(T[k]), Y(T[k]));*/
   }
   /*gets(buf);*/
   e = ecart (T);
   if (e < m)
   {
    m = e;
    for (k=0; k<npt; k++)
     N[k] = T[k];
   }
   if (flag_varsimul)
   {
    r = suivant (D, npt, nq1);
   }
   else
   {
    r = suivant1 (&compteur_suivant, D, npt, nq1);
    /*{int i; char buf[10];
     for (i=0; i<npt; i++) { printf ("%3.1lf,%3.1lf ",X(D[i]),Y(D[i]));}
     gets(buf);}*/
   }
   if (!r)
    break;
  }
  /*printf (" m=%lf ", m);*/
  d=0;
  for (k=0; k<npt; k++)
  {
   /* printf ("\n XN=%lf XB=%lf  ",X(N[k]), X(B[k])); */
   if (X(N[k]) != X(B[k]) || Y(N[k]) != Y(B[k]))
   {
    d=1;
    break;
   }
  }
  if (!d)
  {
   eq *= 0.5;
   /* printf (" eq=%lf ", eq); */
   if (eq < eqstop)
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
  {
   char *nom;
   int l;
   nom = "???";
   for (l=0; l<np; l++)
   {
    if (tp[l].ipi == k)
    {
     nom = tp[l].nom;
     break;
    }
   }
   /*
   printf ("N[%d]=%s: X=%9.2lf Y=%9.2lf\t", k, nom, X(N[k]), Y(N[k]));
   if (k%2 == 1) 
    printf ("\n");
   */
   B[k] = N[k];
   plotpoint (B[k]);
  }
  /*{char buf[10]; gets(buf);}*/
 }
 /* printf (" *** m=%lf ", m); */
/*
 printf ("\nt=%d m=%lf\n", t, m);
 for (k=0; k<npt; k++)
  printf (" %d : %lf %lf\n", k, X(B[k]), Y(B[k]));
*/
}

resol2 ()
{
int pc, px, py;

 nq = 0.5;
 nq1 = 2*nq+1;
 if (deplinit>0)
  eq=deplinit;
 else
  eq=v/nq;
 for (k=0; k<npt; k++)
 {
  if (!connu(B[k]))
   B[k] = C;
 }
 pc = -1;
 for (t=1; t<20000; t++)
 {
  pc++;
  if (pc >= npt)
   pc = 0;

  for (k=0; k<npt; k++)
   N[k]=B[k]; /* N = nouvelle position */
  m = ecart(B);

  for (px=0; px<2; px++) /* tester toutes les positions */
  for (py=0; py<2; py++)
  {
   /* T = position a tester */
   /* déplacer successivement chaque point
	point courant = pc
        +- eq/2
   */

   for (k=0; k<npt; k++)
    T[k]=B[k];
   X(T[pc]) += (px - nq) * eq;
   Y(T[pc]) += (py - nq) * eq;

   e = ecart (T);
   if (e < m)
   {
    m = e;
    for (k=0; k<npt; k++)
     N[k] = T[k];
   }
 
  }

  /* si meme position que precedente on resserre */
  d=0;
  for (k=0; k<npt; k++)
  {
   /* printf ("\n XN=%lf XB=%lf  ",X(N[k]), X(B[k])); */
   if (X(N[k]) != X(B[k]) || Y(N[k]) != Y(B[k]))
   {
    d=1;
    break;
   }
  }
  if (!d)
  {
   eq *= 0.5;
   /* printf (" eq=%lf ", eq); */
   if (eq < eqstop)
    break;
  }

  for (k=0; k<npt; k++)
  {
   char *nom;
   int l;
   nom = "???";
   for (l=0; l<np; l++)
   {
    if (tp[l].ipi == k)
    {
     nom = tp[l].nom;
     break;
    }
   }
   
   /*printf ("N[%d]=%s: X=%9.2lf Y=%9.2lf e=%lf\t", k, nom, X(N[k]), Y(N[k]), e);*/
   /*if (k%2 == 1) */
    /*printf ("\n");*/
   
   B[k]=N[k];
   plotpoint(B[k]);
  }
  /*{char buf[10];gets(buf);}*/
 }
}

real vitessedg = 0;

/* descente de gradient */
resoldg ()
{
int t, u, k, l;
point F;
/*point T[MAX];*/ /* config test avec 1 point de B déplacé */
real eps, s, s1;
real e0, ex, ey, e1, emin, em, sm; /* energie de B, de B avec point k décalé en x, en y, 
                        de la nouvelle position */
int stop[MAX];
int cont;
real nF;

 eps = sigmad;
 /* s = 0.001; */
 /*
 if (deplinit > 0)
  s = deplinit;
 else
 */
 if (vitessedg > 0)
  s = vitessedg;
 else
  s = v;

 for (k=0; k<npt; k++)
  if (!connu(B[k]))
   B[k]=C;

 for (t=1; ; t++)
 {
  /* successivement pour chaque point inconnu :
     déplacer le point de s * F
     s = constante
     F = force = - gradient de l'energie potentielle ou de l'ecart
   */
  emin = ecart(B);
  for (k=0; k<npt; k++)    
  {
   e0 = ecart(B);
   /* calcul de F */
   for (l=0; l<npt; l++)
    T[l] = B[l];
   X(T[k]) += eps; /* point k decale en X */
   ex = ecart(T);
   X(T[k]) -= eps; /* retour a la position initiale */
   Y(T[k]) += eps; /* decalage en Y */
   ey = ecart(T);
   /* F = - grad e */
   X(F) = -(ex-e0)/eps;
   Y(F) = -(ey-e0)/eps;
   nF = sqrt(X(F)*X(F) + Y(F)*Y(F));
   /* printf ("# nF=%lf\n", nF); */
   /* s1 = s * 1000 / nF; */
   /* s1 = s * emin / e0; */
   /* s1 = s * sqrt(emin/e0); */
   s1 = s;
   if (e0 < emin)
    emin = e0;

   /* deplacement dans la direction de F correspondant a ecart minimum */

   for (l=0; l<npt; l++)
    T[l]=B[l];

   /* on teste un deplacement de s1 * F pour commencer */
   X(T[k]) += s1 * X(F);
   Y(T[k]) += s1 * Y(F);
   e1 = ecart(T);
   
   /* si ecart plus petit on regarde si on peut aller plus loin */
   if (e1 < e0)
   {
    em = e1; /* ecart minimum */
    sm = s1; 
   
    for (u=0; u<10; u++)
    {
     u++;
     T[k] = B[k]; /* on revient */
     s1 *= 2; /* on va 2 fois plus loin */
     X(T[k]) += s1 * X(F);
     Y(T[k]) += s1 * Y(F);
     e1 = ecart(T);
     if (e1 > em || (deplinit > 0 && s1 * nF > deplinit))
      break;
     em = e1;
     sm = s1;
    }
    if (deplinit > 0 && sm * nF > deplinit)
     sm = deplinit / nF;
    X(B[k]) += sm * X(F);
    Y(B[k]) += sm * Y(F);
   } else
   /* si ecart plus grand on va moins loin */
   {

    if (deplinit && deplinit / nF < s1)
    {
     s1 = deplinit / nF;
     T[k] = B[k];
     X(T[k]) += s1 * X(F);
     Y(T[k]) += s1 * Y(F);
    }

    em = e1;
    sm = s1;
    if (e1>e0)
    {
     for (u=0; u<10; u++)
     {
      T[k] = B[k];
      s1 /= 2;
      X(T[k]) += s1 * X(F);
      Y(T[k]) += s1 * Y(F);
      e1 = ecart(T);
      if (e1 < e0 || sm*nF < eqstop)
      {
       em = e1;
       sm = s1;
       break;
      }
     }
    }
    X(B[k]) += sm * X(F);
    Y(B[k]) += sm * Y(F);
   }
   printf ("\recart=%lf ", em);
   fflush(stdout);

#if 0
   if (deplinit > 0 && s1 * nF > deplinit)
    s1 = deplinit / nF;  /* deplacement plafonne par deplinit */
   /* point inconnu  k deplace de s1 * F */
   X(B[k]) += s1 * X(F);
   Y(B[k]) += s1 * Y(F);
   e1 = ecart(B);
   
   /*
   printf ("\re1=%lf ",e1);
   fflush(stdout);
   */
   /* s *= e0 / e1; */

   /*
   if (e1 > e0)
    printf ("# e0=%lf e1=%lf variation=%lf\n", e0, e1, e1 - e0);
   */

   /* si l'ecart augmente on revient en arriere */
   if (e1 > e0)
   {
    /*
    s1 *= 1 - e0 / e1;
    X(B[k]) -= s1 * X(F);
    Y(B[k]) -= s1 * Y(F);
    e1 = ecart(B);
    if (s1 * nF < eqstop)
     break;
    */
    X(B[k]) -= s1 * X(F);
    Y(B[k]) -= s1 * Y(F);
    /* s1 *= (e0 / e1); */
    s1 /= 2;
    X(B[k]) += s1 * X(F);
    Y(B[k]) += s1 * Y(F);
    e1 = ecart(B);
    /*
    if (s1 * nF < eqstop)
     break;
    */
   }

   printf ("\re1=%lf ",e1);
   fflush(stdout);
#endif

   plotpoint(B[k]);
   if ((X(F)*X(F) + Y(F)*Y(F))*s1*s1 < eqstop*eqstop) 
    stop[k]=1;
   else
    stop[k]=0;
  }
  cont=0;
  for (k=0; k<npt; k++)
   if (!stop[k])
    cont=1;
  if (!cont)
  {    
    printf ("# e = %lf\n", ecart(B));
    return;     
  }
  
 }
}

#define POINTOBS(obs,k) \
 ((tp[obs->ip[k]].ipi>=0)?B[tp[obs->ip[k]].ipi]:tp[obs->ip[k]].p)

real obs_calc (point B[], struct donnee *obs)
{
real x, r;
 x = NDEF;
 if (obs->type == 'D')
 {
     /* td[i]. remplace par obs-> */
     /*
     if (obs->sigma)
      sigma = obs->sigma;
     else
      sigma = sigmad + sigmad1 * obs->val;
     */
     if (connu(POINTOBS(obs,0)) && connu(POINTOBS(obs,1)))
      x = dist (POINTOBS(obs,0),POINTOBS(obs,1));
    }
    else if (obs->type=='A')
    {
     /*
     if (obs->sigma)
      sigma = obs->sigma;
     else
      sigma = sigmaangle;
     */
     if (connu(POINTOBS(obs,0)) && connu(POINTOBS(obs,1)) && connu(POINTOBS(obs,2)))
      x = angle3g (POINTOBS(obs,0), POINTOBS(obs,1), POINTOBS(obs,2));
    }
    return x;
}

/* point approche */
resolpa ()
{
struct donnee td1[120];
struct matr A[1], P[1], D[1], tA[1], tAP[1], tAPA[1], itAPA[1], tAPD[1],  dX[1];
int i, j, k;
real sigma;
real f0, fx, fy;
real eps;
point T[MAX];

 eps = 0.001;

 /* 1ers points approches */
 for (k=0; k<npt; k++)
  if (!connu(B[k]))
   B[k]=C;

 for (t=1; ; t++)
 {
  /* N = points correspondant aux points approches B 
     N -> nouveau B */
  /* extraire les observations significatives ? */
  /* Aij = dfi/dxj avec fi(X) = valeur correspondant a l'observation i
                                calculee pour la position X
                              = obs_calc (X, td+i)
                                obs_calc (B', td+i)
                                avec B' obtenu a partir de B en augmentant
                                la j-eme coordonnee (X ou Y) de eps
     Pii = poids observation i = 1 / sigma^2
     Di = Li - L0i avec Li = valeur de l'observation i = td[i].val
                        L0i = valeur correspondante calculee pour B
                            = obs_calc (B, td+i)
     (D correspond au B du fascicule "Canevas planimetrique -
      methodes de calcul")
     N = B + dX (dX = X dans le fascicule) -> nouveau B
     dX solution de tA P A dX = tA P D
     dX = inv(tA P A) tA P D     
   */
   A->nl=nd; A->nc=npt*2;
   D->nl=nd; D->nc=1;
   
   P->nl=nd; P->nc=nd;
   for (i=0; i<nd; i++)
   for (j=0; j<nd; j++)
    coef(P,i,j)=0; 

   for (i=0; i<nd; i++)
   {
    if (td[i].sigma)
     sigma = td[i].sigma;
    else if (td[i].type == 'D')
     sigma = sigmad + sigmad1 * td[i].val;
    else if (td[i].type == 'A')
     sigma = sigmaangle;
    else
     sigma = 1;
    coef(P,i,i) = 1/(sigma*sigma);
    
    f0 = obs_calc (B, td+i);

    coef(D,i,0) = td[i].val - f0;

    for (j=0; j<npt; j++)
    {
     for (k=0; k<npt; k++)
      T[k] = B[k];
     X(T[j]) += eps;
     fx = obs_calc (T, td+i);
     X(T[j]) -= eps;
     Y(T[j]) += eps;
     fy = obs_calc (T, td+i);
     coef(A,i,j*2) = (fx-f0)/eps;
     coef(A,i,j*2+1) = (fy-f0)/eps;
    }
   }  

   transpose (A, tA);
   prodmatr (tA, P, tAP);
   prodmatr (tAP, A, tAPA);
   inverse (tAPA, itAPA);
   prodmatr (tAP, D, tAPD);
   prodmatr (itAPA, tAPD, dX);

   for (j=0; j<npt; j++)
   {
    X(B[j]) += coef(dX,j*2,0);
    Y(B[j]) += coef(dX,j*2+1,0);
    plotpoint (B[j]);
   }

   if (norme(dX) < eqstop)
    break;

 }

}

point depl (int k, int id)
{
point u, v, D1, PX, PY;
real d;
real eps;
int i, j;
real dax, day, ad, ac, acx, acy, dx, dy;

 X(D1) = 0;
 Y(D1) = 0;

 eps = 0.001;
 i = id;
 if (td[id].type == 'D')
 {
  if (tp[td[id].ip[0]].ipi == k)
  {
   d = dist (POINT(0), POINT(1));
   if (d < 0.0001)
    return D1;
   coef = 1 - td[id].val / d;
   X(D1) = (X(POINT(1)) - X(POINT(0))) * coef;
   Y(D1) = (Y(POINT(1)) - Y(POINT(0))) * coef;
  }
  else if (tp[td[id].ip[1]].ipi == k)
  {
   d = dist (POINT(0), POINT(1));
   if (d < 0.001) 
    return D1;
   coef = 1 - td[id].val / d;
   X(D1) = (X(POINT(0)) - X(POINT(1))) * coef;
   Y(D1) = (Y(POINT(0)) - Y(POINT(1))) * coef;
  }
#ifdef TRACEDXY
  printf ("distance DX=%lf DY=%lf\n", X(D1), Y(D1));
#endif
 }
 else if (td[id].type == 'A')
 {
  for (j=0; j<3; j++)
  {
   if (tp[td[id].ip[j]].ipi == k)
   {
    ad = td[id].val;
    ac = angle3g (POINT(0), POINT(1), POINT(2));
    PX = POINT(j);
    X(PX) += eps;
    PY = POINT(j);
    Y(PY) += eps;
    if (j==0)
    {
     acx = angle3g (PX, POINT(1), POINT(2));
     acy = angle3g (PY, POINT(1), POINT(2));
    }
    else if (j==1)
    {
     acx = angle3g (POINT(0), PX, POINT(2));
     acy = angle3g (POINT(0), PY, POINT(2));
    }
    else if (j==2)
    {
     acx = angle3g (POINT(0), POINT(1), PX);
     acy = angle3g (POINT(0), POINT(1), PY);
    }
    dax = acx - ac;
    day = acy - ac;
    if (fabs(dax)<0.000001 || fabs(day)<0.000001)
     return D1;
    dx = eps / dax * (td[id].val - ac);
    dy = eps / day * (td[id].val - ac);
    X(D1) = dx/10;
    Y(D1) = dy/10;
#ifdef TRACEDXY
    printf ("angle DX=%lf DY=%lf\n", X(D1), Y(D1));
#endif
    return D1;
   }
  }
 }
 return D1;
}

resol29 ()
{
int id;
point D, D1;
real s, sigma, p;
point Z;
real sd, zd;
char buf[10];
 X(Z)=0;
 Y(Z)=0;
 for (k=0; k<npt; k++)
 {
  if (!connu(B[k]))
   B[k] = C;
 }

 for (t=1; t<20000; t++)
 {
  sd = 0; 
  for (k=0; k<npt; k++)
  {
   X(D)=0;
   Y(D)=0;
   s = 0;
   for (id=0; id<nd; id++)
   {
    if (td[id].sigma)
     sigma = td[id].sigma;
    else if (td[id].type == 'D')
     sigma = sigmad;
    else if (td[id].type == 'A')
     sigma = sigmaangle; 
#ifdef TRACEDXY
    printf ("type=%c sigma=%lf\n", td[id].type, sigma);
    gets(buf); 
#endif
    p = td[id].coef / (sigma * sigma);
    s += p; 
    D1 = depl (k, id);  
    /*
    printf ("id=%d X(D1)=%lf Y(D1)=%lf\n", id, X(D1), Y(D1));
    gets(buf);
    */
    X(D) += p * X(D1);
    Y(D) += p * Y(D1);
   }
   X(D) /= s;
   Y(D) /= s;

   X(B[k]) += X(D);
   Y(B[k]) += Y(D);

#ifdef TRACERESOL
   printf ("B[%d] : X=%lf Y=%lf\n", k, X(B[k]), Y(B[k])); 
#endif 
   zd = dist(D,Z);
   sd += zd*zd;
  }
#ifdef TRACERESOL
  {char buf[10];gets(buf);}  
#endif
  /* terminer si D suffisemment petit */
  /*if (dist (D, Z) < 0.001)*/
  if (sd < 0.0001)
   break;
  
 }
 
}

char algo[120];

resol ()
{
 if (!strcmp (algo, "SIMULT"))
  resol1();
 else if (!strcmp (algo, "DG"))
  resoldg();
 else if (!strcmp (algo, "PA"))
  resolpa();
 else if (!strcmp (algo, "EXP"))
  resolexp();
#ifdef GEN
 else if (!strcmp (algo, "GEN"))
  resolgen();
#endif
 else if (!strcmp (algo, "2"))
  resol2();
 else if (!strcmp (algo, "29"))
  resol29();
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
 char param[40][30];
 char val[30];
 char sigma[30];
};

analyser (char *buf, struct instruction *instr)
{
int i, j, k, l;
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
  if ((buf[i]>='A' && buf[i]<='Z')
   || (buf[i]>='0' && buf[i]<='9'))
   instr->op[j++]=buf[i++];
  else
   break;
 }
 instr->op[j]=0;
 if (buf[i]=='=')
 {
  instr->nparam=0;
  strcpy (instr->val, buf+i+1);
  return;
 }
 else if (buf[i]=='?')
 {
  instr->nparam=0;
  strcpy (instr->val, buf+i);
  return;
 }
 i++; /* ( */
 j=0;
 k=0;
 for (;;)
 {
  if ((buf[i]>='A' && buf[i]<='Z')
   || (buf[i]>='a' && buf[i]<='z')
   || (buf[i]>='0' && buf[i]<='9')
   || buf[i]=='.'
   || buf[i]=='\\'
   || buf[i]=='/'
   || buf[i]=='-')
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
  /*strcpy (instr->val, buf+i);*/
  instr->sigma[0]=0;
  l=0;
  for (;;)
  {
   if (buf[i]=='~')
   {
    instr->val[l]=0;
    strcpy(instr->sigma,buf+i+1);
    break;
   }
   if (buf[i]==0)
   {
    instr->val[l]=0;
    strcpy(instr->sigma,"0");
    break;
   }
   instr->val[l++] = buf[i];
   i++;
  }
}

/* #define NDEF -1000000000 */

int numpoint (char *nom)
{
int i, d;
 for (i=0; i<np; i++)
  if (!strcmp(tp[i].nom,nom))
   return i;
 strcpy(tp[np].nom,nom);
 for (d=0; d<DIM; d++)
  tp[np].p.xyz[d] = NDEF;
 tp[np].ipi = -1;
 np++;
 return np-1;
}

int connu (point p)
{
 return (X(p)!=NDEF && Y(p)!=NDEF);
}

init_resoudre ()
{
int i;
real x, y, dx, dy;
 xmin=ymin=999999999;
 xmax=ymax=-999999999;
 x=0; y=0; n=0;
 for (i=0; i<np; i++)
 {
  if (connu(tp[i].p))
  {
   x += X(tp[i].p);
   y += Y(tp[i].p);
   if (X(tp[i].p) < xmin)
    xmin = X(tp[i].p);
   if (X(tp[i].p) > xmax)
    xmax = X(tp[i].p);
   if (Y(tp[i].p) < ymin)
    ymin = Y(tp[i].p);
   if (Y(tp[i].p) > ymax)
    ymax = Y(tp[i].p);
   n++;
  }
 }
 X(C)=x/n;
 Y(C)=y/n;
 
 xspan = xmax - xmin;
 yspan = ymax - ymin;
 
 xscale = (float)width / xspan;
 yscale = (float)height / yspan;

 if (xscale <= yscale)
  scale = xscale;
 else
  scale = yscale;

 /* scale *= 0.3;  */
 /*scale *= 3;*/
 scale *= echelle;

 v = 0;
 for (i=0; i<np; i++)
 {
  if (connu(tp[i].p))
  {
   dx = X(tp[i].p)-X(C);
   dy = Y(tp[i].p)-Y(C);
   v += sqrt(dx*dx+dy*dy);
   /*plotcircledpoint (tp[i].p);*/
   plotpointname (tp[i].p, tp[i].nom);

   /*XDrawArc (display, win, gc, X(tp[i].p)-3, Y(tp[i].p)-3, 3, 3, 0, 360*64);*/
   
  }
 }
 v=v/n;
 if (v==0)
  v=1;
}

FILE *f;

FILE *out=NULL;

aide ()
{
 printf ("Resolution de problemes de topographie\n");
 printf ("Introduction de donnees :\n");
 printf (" expression=valeur ou expression=valeur~ecart-type\n");
 printf ("Calcul :\n");
 printf (" expression?\n");
 printf ("Expressions :\n");
 printf (" X(A), Y(A) : abcisse et ordonnee du point A\n");
 printf (" D(A,B) : Distance reduite entre A et B\n");
 printf (" DILZ(A,B,LZ) : distance inclinee entre A et B avec lecture zenitale LZ\n");
 printf (" ANGLE(A,B,C) : angle de sommet B ; ");
 printf (" G(A,B) : gisement\n");
 printf (" SIGMAD, SIGMAD1, SIGMAANGLE : ecarts types pour les distances et les angles\n");
 printf (" H : altitude du chantier ; ");
 printf (" CL : correction Lambert\n");
 printf (" EPS, TX, PAIRES : tolerances pour le V0 et nombre de paires de sequences\n");
 printf (" VARSIM=1 : variation simultanée des points ; VARSIM=0 : variation successive\n");
 printf ("Autres commandes :\n");
 printf (" CALCULER(A,B,C) : calculer les coordonnees des points A, B, C\n");
 printf (" RESOUDRE : calculer les coordonnees de tous les points inconnus\n");
 printf (" NOUVEAU : initialiser les donnees pour resoudre un nouveau probleme\n");
 printf (" V0(station,reference,visee,...) : V0 pondere d'une station\n");
 printf (" V0C(station,reference,visee,...) : V0 pondere puis correction des angles\n");
 printf (" CHEMIN(O,A,C,D,...,B,F) : cheminement A(ref.O),C,D,...B(ref.F)\n");
 printf (" LIRE(fichier) : lire les commandes dans le fichier\n");
 printf (" ECRIRE(fichier) : ecrire les commandes et les resultats dans le fichier\n");
 printf (" AUTEUR : informations sur l'auteur de ce programme\n");
 printf (" TERMINER : quitter ce programme\n");

}

real distance_lambert (real di, real lz, real h, real cl)
{
real R, c, dh, d0, dl;
 R = 6378000;
 c = -0.21 * di * di /(R + h) * sin (2 * lz/200*pi);
 dh = di * sin(lz/200*pi) + c;
 d0 = dh * R / (R+h);
 dl = d0 * (1 + cl * 0.00001);
 return fabs(dl);
}

real poids (real d)
{
real t1, T, p;
 t1 = 2/pi*20/(d/1000);
 T = sqrt(t1*t1+1.5*1.5/2);
 p=1/(T*T);
 return p;
}

/*
real tolerance (real d)
{
real t1, T;
 t1 = 2/pi*Tx/(d/1000);
 T = sqrt(t1*t1+eps*eps/paires);
 return T;
}
*/

real tolerance (real d)
{
real t1, T;
 t1 = 2/pi*Tx*100/(d/1000);
 T = sqrt(t1*t1+eps*eps*1000000/paires);
 return T/1000;
}

int angle_donne (int a, int b, int c, real *v, real *s)
{
int i;
 for (i=0; i<nd; i++)
 {
  if (td[i].type == 'A'
   && td[i].ip[0] == a
   && td[i].ip[1] == b
   && td[i].ip[2] == c)
  {
   *v = td[i].val;
   *s = td[i].sigma;
   return 1;
  }
  if (td[i].type == 'A' 
   && td[i].ip[0] == a
   && td[i].ip[1] == c
   && td[i].ip[2] == b)
  {
   *v = -td[i].val;
   *s = td[i].sigma;
   return 1;
  }
 }
 return 0;
}

aj_angle (int a, int b, int c, real v, real s)
{
char buf[300];
 td[nd].type = 'A';
 td[nd].ip[0] = a;
 td[nd].ip[1] = b;
 td[nd].ip[2] = c;
 td[nd].val = v;
 td[nd].sigma = s;
 sprintf (buf, "ANGLE(%s,%s,%s)=%lf~%lf\n",
	tp[b].nom,
	tp[a].nom,
	tp[c].nom,
	v,
	s);
 printf (buf);
 if (out)
  fprintf (out, buf);
}

#define NDEFP -999.0

real convl (real x)
{
 if (x == NDEFP)
  return NDEF;
 return x;
}

lirecan (char *filename)
{
FILE *can;
char line[300];
real x, y, z;
char nom[100];
 can = fopen (filename, "r");
 if (can == NULL)
  perror(filename);
 else
 {
  fgets (line, sizeof(line), can);
  /*printf ("%s", line);*/
  for (;;)
  {
   fgets (line, sizeof(line), can);
   /*printf ("%s", line);*/
   if (feof(can))
    break;
   sscanf (line, "%s%lf%lf%lf", nom, &x, &y, &z);
   printf ("# Point %s: X=%lf Y=%lf Z=%lf\n", nom, x, y, z);
   X(tp[numpoint(nom)].p) = convl(x);
   Y(tp[numpoint(nom)].p) = convl(y);
   Z(tp[numpoint(nom)].p) = convl(z);
  }
  fclose(can);
 }
 init_resoudre();
}

lireobs (char *filename)
{
FILE *obs;
char noma[100], nomb[100], nomc[100];
real a, d, az, az1;
char line[300];
 obs = fopen (filename, "r");
 if (obs == NULL)
  perror(filename);
 else
 {
  for (;;)
  {
   fgets (line, sizeof(line), obs);
   if (feof(obs))
    break;
   sscanf (line, "%s%s%s%lf%lf%lf",
    noma, nomb, nomc, &a, &d, &az);
  a = convl(a);
  d = convl(d);
  az = convl(az); 
  printf ("# S:%s R:%s V:%s A:%lf D:%lf AZ:%lf\n",
    noma, nomb, nomc, a, d, az);
   if (d != NDEF)
   {
    td[nd].type = 'D';
    td[nd].ip[0] = numpoint(noma);
    td[nd].ip[1] = numpoint(nomc);
    if (az == NDEF)
     az1 = 100;
    else
     az1 = az;
    td[nd].val = distance_lambert (d, az1, h, CL);
    td[nd].sigma = sigmad + sigmad1 * td[nd].val;
    td[nd].coef = coef;
    nd++;
   }
   if (a != NDEF)
   {
    td[nd] .type = 'A';
    td[nd].ip[0] = numpoint(noma);
    td[nd].ip[1] = numpoint(nomb);
    td[nd].ip[2] = numpoint(nomc);
    td[nd].val = a;
    td[nd].sigma = sigmaangle;
    td[nd].coef = coef;
    nd++;
   }
  }
  fclose(obs);
 }
}

real conv (real x)
{
 if (x == NDEF)
  return NDEFP;
 return x;
}

ecrirecan (char *filename)
{
int i;
FILE *can;
 can = fopen (filename, "w");
 if (can == NULL)
  perror(filename);
 else
 {
  fprintf (can, "<PROTOM> Liste des points du canevas\n");
  for (i=0; i<np; i++)
  {
   fprintf (can, " %s %lf %lf %lf %lf %lf %lf \n",
    tp[i].nom, conv(X(tp[i].p)), conv(Y(tp[i].p)), conv(Z(tp[i].p)),
    NDEFP, NDEFP, NDEFP);
  }
  fclose(can);
 }
}

ecrireobs (char *filename)
{
int i;
FILE *obs;
 obs = fopen (filename, "w");
 if (obs == NULL)
  perror (filename);
 else
 {
  for (i=0; i<nd; i++)
  {
   if (td[i].type == 'D')
   {
    fprintf (obs, " %s %s %s %lf %lf %lf %lf %lf \n",
	tp[td[i].ip[0]].nom,
	tp[td[i].ip[1]].nom,
	tp[td[i].ip[1]].nom,
	NDEFP,
	td[i].val,
	100.0,
	NDEFP,
	NDEFP);
   }
   else if (td[i].type == 'A')
   {
    fprintf (obs, " %s %s %s %lf %lf %lf %lf %lf \n",
	tp[td[i].ip[0]].nom,
	tp[td[i].ip[1]].nom,
	tp[td[i].ip[2]].nom,
	td[i].val,
	NDEFP,
	NDEFP,
	NDEFP,
	NDEFP);
   }
  }
  fclose(obs);
 }
}

real traiter (struct instruction *instr)
{
real r;
int i;
#if 0
 printf ("%s(", instr->op);
 for (i=0; i<instr->nparam; i++)
 {
  if (i>0)
   printf (",");
  printf ("%s", instr->param[i]);
 }
 printf (")=");
 printf ("%lf", instr->val);
 printf ("~%lf\n", instr->sigma);
#endif

 if (!strcmp (instr->op, "X"))
 {
  if (instr->val[0]=='?')
  {
   if (tp[numpoint(instr->param[0])].p.xyz[0] == NDEF)
    resoudre(0);

   printf ("X(%s)=%lf\n",
    instr->param[0], tp[numpoint(instr->param[0])].p.xyz[0]);
   if (out!=NULL)
   fprintf (out,"X(%s)=%lf\n",
    instr->param[0], tp[numpoint(instr->param[0])].p.xyz[0]);

  }
  else
  {
   r = atof(instr->val);
   tp[numpoint(instr->param[0])].p.xyz[0] = r;
  }
 }
 else if (!strcmp (instr->op, "Y"))
 {
  if (instr->val[0]=='?')
  {
   if (tp[numpoint(instr->param[0])].p.xyz[1] == NDEF)
    resoudre(0);
   printf ("Y(%s)=%lf\n",
    instr->param[0], tp[numpoint(instr->param[0])].p.xyz[1]);
   if (out!=NULL)
   fprintf (out, "Y(%s)=%lf\n",
    instr->param[0], tp[numpoint(instr->param[0])].p.xyz[1]);
  }
  else
   tp[numpoint(instr->param[0])].p.xyz[1] = atof(instr->val);
 }
 else if (!strcmp (instr->op, "Z") && DIM>=3)
 {
  if (instr->val[0]=='?')
  {
   if (tp[numpoint(instr->param[0])].p.xyz[2] == NDEF)
    resoudre(0);
   printf ("Z(%s)=%lf\n",
    instr->param[0], tp[numpoint(instr->param[0])].p.xyz[2]);
   if (out!=NULL)
   fprintf (out, "Z(%s)=%lf\n",
    instr->param[0], tp[numpoint(instr->param[0])].p.xyz[2]);
  }
  else
   tp[numpoint(instr->param[0])].p.xyz[2] = atof(instr->val);
 }
 else if (!strcmp (instr->op, "D"))
 {
  if (instr->val[0]=='?')
  {
   if (!connu(tp[numpoint(instr->param[0])].p) ||
       !connu(tp[numpoint(instr->param[1])].p))
    resoudre(0);
   printf ("D(%s,%s)=%lf\n",
    instr->param[0],
    instr->param[1],
    dist (tp[numpoint(instr->param[0])].p,
	  tp[numpoint(instr->param[1])].p));
   if (out!=NULL)
   fprintf (out,"D(%s,%s)=%lf\n",
    instr->param[0],
    instr->param[1],
    dist (tp[numpoint(instr->param[0])].p,
	  tp[numpoint(instr->param[1])].p));

  }
  else
  {
   td[nd].type = 'D';
   td[nd].ip[0] = numpoint(instr->param[0]);
   td[nd].ip[1] = numpoint(instr->param[1]);
   td[nd].val = atof(instr->val);
   td[nd].sigma = atof(instr->sigma);
   td[nd].coef = coef;
   nd++;
  }
 }
 else if (!strcmp (instr->op, "DILZ"))
 {
  if (instr->val[0]=='?')
  {
   if (!connu(tp[numpoint(instr->param[0])].p) ||
       !connu(tp[numpoint(instr->param[1])].p))
    resoudre(0);
   printf ("D(%s,%s)=%lf\n",
    instr->param[0],
    instr->param[1],
    dist (tp[numpoint(instr->param[0])].p,
	  tp[numpoint(instr->param[1])].p));
   if (out!=NULL)
   fprintf (out,"D(%s,%s)=%lf\n",
    instr->param[0],
    instr->param[1],
    dist (tp[numpoint(instr->param[0])].p,
	  tp[numpoint(instr->param[1])].p));

  }
  else
  {
   td[nd].type = 'D';
   td[nd].ip[0] = numpoint(instr->param[0]);
   td[nd].ip[1] = numpoint(instr->param[1]);
   td[nd].val = distance_lambert (atof(instr->val),
	atof(instr->param[2]), h, CL);
   td[nd].sigma = atof(instr->sigma);
   td[nd].coef = coef;
   nd++;
  }
 }
 else if (!strcmp (instr->op, "ANGLE"))
 {
  if (instr->val[0]=='?')
  {
   if (!connu(tp[numpoint(instr->param[0])].p) ||
       !connu(tp[numpoint(instr->param[1])].p) ||
       !connu(tp[numpoint(instr->param[2])].p))
    resoudre(0);
   printf ("ANGLE(%s,%s,%s)=%lf\n",
    instr->param[0],
    instr->param[1],
    instr->param[2],
    angle3g (tp[numpoint(instr->param[1])].p,
	     tp[numpoint(instr->param[0])].p,
	     tp[numpoint(instr->param[2])].p));
   if (out!=NULL)
   fprintf (out,"ANGLE(%s,%s,%s)=%lf\n",
    instr->param[0],
    instr->param[1],
    instr->param[2],
    angle3g (tp[numpoint(instr->param[1])].p,
	     tp[numpoint(instr->param[0])].p,
	     tp[numpoint(instr->param[2])].p));

  }
  else
  {
   td[nd].type = 'A';
   td[nd].ip[0] = numpoint(instr->param[1]);
   td[nd].ip[1] = numpoint(instr->param[0]);
   td[nd].ip[2] = numpoint(instr->param[2]);
   td[nd].val = atof(instr->val);
   td[nd].sigma = atof(instr->sigma);
   td[nd].coef = coef;
   nd++;
  }
 }
#if 1
 else if (!strcmp (instr->op, "ANGLE1"))
 {
 int i, j;
 char buf[300];
 int a, b, c, d;
 real a1, s1, a2, s2;
  for (i=0; i<nd; i++)
  {
   if (td[i].type == 'A'
    && td[i].ip[0] == numpoint(instr->param[1])
    && td[i].ip[1] == numpoint(instr->param[0])
    && td[i].ip[2] == numpoint(instr->param[2]))
   {
    sprintf(buf,"ANGLE(%s,%s,%s)=%lf~%lf\n",
	instr->param[0],
	instr->param[1],
	instr->param[2],
	td[i].val,
	td[i].sigma);
    printf (buf);
    if (out)
     fprintf (out, buf); 
    return 1.0;
   }
   if (td[i].type == 'A'
    && td[i].ip[0] == numpoint(instr->param[1])
    && td[i].ip[1] == numpoint(instr->param[2])
    && td[i].ip[2] == numpoint(instr->param[0]))
   {
    td[nd].type = 'A';
    td[nd].ip[0] = td[i].ip[0];
    td[nd].ip[1] = td[i].ip[2];
    td[nd].ip[2] = td[i].ip[1];
    td[nd].val = -td[i].val;
    td[nd].sigma = td[i].sigma;
    nd++;
    sprintf(buf,"ANGLE(%s,%s,%s)=%lf~%lf\n",
	instr->param[0],
	instr->param[1],
	instr->param[2],
	-td[i].val,
	td[i].sigma);
    printf (buf);
    if (out)
     fprintf (out, buf);
    return 1.0;     
   }
  }
  a = numpoint(instr->param[0]);
  b = numpoint(instr->param[1]);
  c = numpoint(instr->param[2]);
  for (d=0; d<np; d++)
  {
   if (angle_donne (b, a, d, &a1, &s1)
    && angle_donne (b, d, c, &a2, &s2))
   {
    aj_angle (a, b, c, a1+a2, sqrt(s1*s1+s2*s2));
    return 1.0;
   }
  }
  return 0.0;
 }
#endif
 else if (!strcmp (instr->op, "G"))
 {
  if (instr->val[0]=='?')
  {
   if (!connu(tp[numpoint(instr->param[0])].p) ||
       !connu(tp[numpoint(instr->param[1])].p))
    resoudre(0);
   printf ("G(%s,%s)=%lf\n",
    instr->param[0],
    instr->param[1],
    pm200(angleg(tp[numpoint(instr->param[0])].p,
	  tp[numpoint(instr->param[1])].p)));
   if (out!=NULL)
   fprintf (out,"G(%s,%s)=%lf\n",
    instr->param[0],
    instr->param[1],
    pm200(angleg(tp[numpoint(instr->param[0])].p,
	  tp[numpoint(instr->param[1])].p)));

  }
  else
  {
   td[nd].type = 'G';
   td[nd].ip[0] = numpoint(instr->param[0]);
   td[nd].ip[1] = numpoint(instr->param[1]);
   td[nd].val = atof(instr->val);
   td[nd].sigma = atof(instr->sigma);
   td[nd].coef = coef;
   nd++;
  }
 }
 else if (!strcmp (instr->op, "V0NP"))
 {
 int i, j, n;
 real l, s, v0, e, e1, g;
 real v0i[30];
 int ip[30];

  n = instr->nparam-1;
  s=0;
  ip[0] = numpoint(instr->param[1]);
  if (n<1)
  {
   printf ("# Indiquer au moins la station et la reference du V0\n");
   return NDEF;
  }
  else if (n==1)
  {
   v0i[0] = angleg (tp[numpoint(instr->param[0])].p,
		    tp[numpoint(instr->param[1])].p);
   s = v0i[0];
   n=1;
   for (j=0; j<nd; j++)
   {
    if (td[j].type=='A'
     && td[j].ip[0]==numpoint(instr->param[0]))
    {
     if (td[j].ip[1]==numpoint(instr->param[1])
      && connu(tp[td[j].ip[2]].p))
     {
      ip[n] = td[j].ip[2];
      v0i[n] = pm200(angleg (tp[numpoint(instr->param[0])].p, tp[td[j].ip[2]].p)
		- td[j].val);
      s += v0i[n];
      n++;
     }
     else if (td[j].ip[2]==numpoint(instr->param[1])
      && connu(tp[td[j].ip[1]].p))
     {
      ip[n] = td[j].ip[1];
      v0i[n] = angleg (tp[numpoint(instr->param[0])].p, tp[td[j].ip[1]].p)
		+ td[j].val;
      s += v0i[n];
      n++;
     }
    }
   }
   v0=s/n;
  }
  else
  {
   v0i[0] = angleg (tp[numpoint(instr->param[0])].p,
		    tp[numpoint(instr->param[1])].p);
   s = v0i[0];
   for (i=1; i<n; i++)
   {
    l=NDEF;
    for (j=0; j<nd; j++)
    {
     if (td[j].type=='A'
      && td[j].ip[0]==numpoint(instr->param[0]))
     {
      if (td[j].ip[1]==numpoint(instr->param[1])
       && td[j].ip[2]==numpoint(instr->param[i+1]))
       {
	l=td[j].val;
	break;
       }
      else if (td[j].ip[2]==numpoint(instr->param[1])
       && td[j].ip[1]==numpoint(instr->param[i+1]))
       {
	l=-td[j].val;
	break;
       }
     }
    }
    if (l==NDEF)
    {
     l=0;
     printf ("# Lecture manquante pour le point %s\n",
		instr->param[i+1]);
     if (out!=NULL)
      fprintf (out, "# Lectures manquantes pour ce V0\n");
    }
    /* printf ("# L(%d)=%lf\n", i, l); */
    /* v0i[i] = pm200(angleg (tp[numpoint(instr->param[0])].p,
		     tp[numpoint(instr->param[i+1])].p) - l); */
    g = angleg (tp[numpoint(instr->param[0])].p,
		tp[numpoint(instr->param[i+1])].p);
    /* printf ("# G(%d)=%lf\n", i, g); */
    ip[i] = numpoint(instr->param[i+1]);
    v0i[i] = pm200 (g - l);
    s += v0i[i];
   }
  }
  v0 = s/n;
  e=0;
  for (i=0; i<n; i++)
  {
    e1 = fabs(pm200(v0i[i]-v0));
    if (e1 > e)
     e = e1;
  }
  printf ("V0(%s", instr->param[0]);
  for (i=1; i<instr->nparam; i++)
   printf (",%s", instr->param[i]);
  printf (")=%lf\n# E=%lf\n", v0, e);
  for (i=0; i<n; i++)
   printf ("# V0i(%s)=%lf\n", tp[ip[i]].nom, v0i[i]);
  if (out!=NULL)
  {
   fprintf (out,"V0(%s", instr->param[0]);
   for (i=0; i<n; i++)
    fprintf (out,",%s", instr->param[i+1]);
   fprintf (out,")=%lf\nE=%lf\n", v0, e);
   for (i=0; i<n; i++)
    fprintf (out, "# V0i(%s)=%lf\n", tp[ip[i]].nom, v0i[i]);
  }
 }
 else if (!strcmp (instr->op, "V0") || !strcmp(instr->op, "V0C"))
 {
 int i, j, k, n;
 real l, s, v0, e, e1, g, d, p, sp, Te, g01;
 real v0i[30];
 real T[30];
 int ip[30];

  n = instr->nparam-1;
  s=0;
  sp = 0;
  ip[0] = numpoint(instr->param[1]);
  if (n<1)
  {
   printf ("# Indiquer au moins la station et la reference du V0\n");
   return NDEF;
  }
  else if (n==1)
  {
   v0i[0] = angleg (tp[numpoint(instr->param[0])].p,
		    tp[numpoint(instr->param[1])].p);
   d = dist (tp[numpoint(instr->param[0])].p,
             tp[numpoint(instr->param[1])].p);
   T[0] = tolerance(d);
   p = 1/(T[0]*T[0]);
   s = p * v0i[0];
   sp = p;
   n=1;
   for (j=0; j<nd; j++)
   {
    if (td[j].type=='A'
     && td[j].ip[0]==numpoint(instr->param[0]))
    {
     if (td[j].ip[1]==numpoint(instr->param[1])
      && connu(tp[td[j].ip[2]].p))
     {
      ip[n] = td[j].ip[2];
      v0i[n] = pm200(angleg (tp[numpoint(instr->param[0])].p, tp[td[j].ip[2]].p)
		- td[j].val);
      d = dist (tp[numpoint(instr->param[0])].p, tp[td[j].ip[2]].p);
      T[n] = tolerance(d);
      p = 1/(T[n]*T[n]);
      sp += p;
      s += p * v0i[n];
      n++;
     }
     else if (td[j].ip[2]==numpoint(instr->param[1])
      && connu(tp[td[j].ip[1]].p))
     {
      ip[n] = td[j].ip[1];
      v0i[n] = pm200(angleg (tp[numpoint(instr->param[0])].p, tp[td[j].ip[1]].p)
		+ td[j].val);
      d = dist (tp[numpoint(instr->param[0])].p, tp[td[j].ip[1]].p);
      T[n] = tolerance(d);
      p = 1/(T[n]*T[n]);
      sp += p;
      s += p * v0i[n];
      n++;
     }
    }
   }
   v0=s/sp;
  }
  else
  {
   v0i[0] = angleg (tp[numpoint(instr->param[0])].p,
		    tp[numpoint(instr->param[1])].p);
   d = dist (tp[numpoint(instr->param[0])].p,
		    tp[numpoint(instr->param[1])].p);
   T[0] = tolerance(d);
   p = 1/(T[0]*T[0]);
   sp = p;
   s = p * v0i[0];
   for (i=1; i<n; i++)
   {
    l=NDEF;
    for (j=0; j<nd; j++)
    {
     if (td[j].type=='A'
      && td[j].ip[0]==numpoint(instr->param[0]))
     {
      if (td[j].ip[1]==numpoint(instr->param[1])
       && td[j].ip[2]==numpoint(instr->param[i+1]))
       {
	l=td[j].val;
	break;
       }
      else if (td[j].ip[2]==numpoint(instr->param[1])
       && td[j].ip[1]==numpoint(instr->param[i+1]))
       {
	l=-td[j].val;
	break;
       }
     }
    }
    if (l==NDEF)
    {
     l=0;
     printf ("# Lecture manquante pour le point %s\n",
		instr->param[i+1]);
     if (out!=NULL)
      fprintf (out, "# Lectures manquantes pour ce V0\n");
    }
    /* printf ("# L(%d)=%lf\n", i, l); */
    /* v0i[i] = pm200(angleg (tp[numpoint(instr->param[0])].p,
		     tp[numpoint(instr->param[i+1])].p) - l); */
    g = angleg (tp[numpoint(instr->param[0])].p,
		tp[numpoint(instr->param[i+1])].p);
    /* printf ("# G(%d)=%lf\n", i, g); */
    ip[i] = numpoint(instr->param[i+1]);
    v0i[i] = pm200 (g - l);
    d = dist (tp[numpoint(instr->param[0])].p,
		tp[numpoint(instr->param[i+1])].p);
    T[i] = tolerance(d);
    p = 1/(T[i]*T[i]);
    sp += p;
    s += p * v0i[i];
   }
  }
  v0 = s/sp;
  e=0;
  for (i=0; i<n; i++)
  {
    e1 = fabs(pm200(v0i[i]-v0));
    if (e1 > e)
     e = e1;
  }
  printf ("V0(%s", instr->param[0]);
  for (i=1; i<instr->nparam; i++)
   printf (",%s", instr->param[i]);
  printf (")=%lf\n# E=%lf\n", v0, e);
  for (i=0; i<n; i++)
  {
    Te = sqrt(T[i]*T[i]-1/sp);
    printf ("# V0i(%6s)=%lf e=%lf Te=%lf %s\n",
     tp[ip[i]].nom,
     v0i[i],
     fabs(v0i[i]-v0),
     Te,
     (fabs(v0i[i]-v0)<=Te) ? " " : "TOLERANCE DEPASSEE");
  }
  if (out!=NULL)
  {
   fprintf (out,"V0(%s", instr->param[0]);
   for (i=1; i<instr->nparam; i++)
    fprintf (out,",%s", instr->param[i]);
   fprintf (out,")=%lf\nE=%lf\n", v0, e);
   for (i=0; i<n; i++)
   {
    Te = sqrt(T[i]*T[i]-1/sp);
    fprintf (out, "# V0i(%s)=%lf e=%lf Te=%lf %s\n",
     tp[ip[i]].nom,
     v0i[i],
     fabs(v0i[i]-v0),
     Te,
     (fabs(v0i[i]-v0)<=Te) ? " " : "TOLERANCE DEPASSEE");
   }
  }
  /* correction des angles */
  if (!strcmp(instr->op,"V0C"))
  {
   g01 = angleg (tp[numpoint(instr->param[0])].p,
		 tp[numpoint(instr->param[1])].p);
   for (k=0; k<nd; k++)
   {
    if (td[k].type == 'A'
     && td[k].ip[0] == numpoint(instr->param[0])
     && td[k].ip[1] == numpoint(instr->param[1]))
    {
     td[k].val += v0 - g01;
    }
    else if (td[k].type == 'A'
     && td[k].ip[0] == numpoint(instr->param[0])
     && td[k].ip[2] == numpoint(instr->param[1]))
    {
     td[k].val -= v0 - g01;
    }
   }
  }
  return v0;
 }
#if 1
 else if (!strcmp (instr->op, "ALIGN"))
 {
 real distm, distc, facteur;
 point A, B, C, u, v;
 int n, i, dim;
  init_resoudre(); 
  if (!connu(tp[numpoint(instr->param[0])].p) 
   || !connu(tp[numpoint(instr->param[1])].p))
   resoudre(0);
  A = tp[numpoint(instr->param[0])].p;
  B = tp[numpoint(instr->param[1])].p;
  plotpointname (A, instr->param[0]);
  plotpointname (B, instr->param[1]);
 
  distc = dist(A,B);
  distm = atof (instr->param[2]);
  if (distm == 0)
   distm = distc;
  printf ("# Align. %s-%s: dist. mesuree=%lf, calculee=%lf, ecart=%lf\n",
   instr->param[0],
   instr->param[1],
   distm,
   distc,
   distm-distc);
  facteur = distc / distm;  
  for (dim=0; dim<DIM; dim++)
   u.xyz[dim] = (B.xyz[dim] - A.xyz[dim]) / distm;
  X(v) = -Y(u) * distm / distc;
  Y(v) = X(u) * distm / distc;
  Z(v) = 0;
  n = (instr->nparam-3)/3;
  for (i=0; i<n; i++)
  {
   for (dim=0; dim<2; dim++)
    tp[numpoint(instr->param[3*i+3])].p.xyz[dim] = 
     A.xyz[dim] + atof (instr->param[3*i+4]) * u.xyz[dim] 
                + atof (instr->param[3*i+5]) * v.xyz[dim];
   plotpointname (tp[numpoint(instr->param[3*i+3])].p, instr->param[3*i+3]);
   sprintf (buf, "X(%s)=%lf\nY(%s)=%lf\n",
    instr->param[3*i+3],
    X(tp[numpoint(instr->param[3*i+3])].p),
    instr->param[3*i+3],
    Y(tp[numpoint(instr->param[3*i+3])].p));
   /* printf (buf); */
   if (out)
    fprintf (out, buf);

  }
 }
 else if (!strcmp (instr->op, "CHEMIN"))
 {
 real V0A, V0B, GD, GF, GF1, theta[60], G[60], eG, Delta[60], d[60];
 struct point p[60];
 struct point B;
 struct point B1;
 struct point dB;
 struct instruction in[1];
 int i, j;
 char buf[300];

  strcpy (in->op, "V0");
  in->nparam = 2;
  strcpy (in->param[0], instr->param[1]);
  strcpy (in->param[1], instr->param[0]);
  V0A = traiter (in);
  GD = V0A + 200;

  strcpy (in->param[0], instr->param[instr->nparam-2]);
  strcpy (in->param[1], instr->param[instr->nparam-1]);
  V0B = traiter (in);
  GF = V0B;

  G[0] = GD;

  for (i=1; i<=instr->nparam-2; i++)
  {
   theta[i] = NDEF;
   G[i] = NDEF;
   for (j=0; j<nd; j++)
   {
    if (td[j].type == 'A'
     && td[j].ip[0] == numpoint(instr->param[i])
     && td[j].ip[1] == numpoint(instr->param[i-1])
     && td[j].ip[2] == numpoint(instr->param[i+1]))
    {
     theta[i] = td[j].val;
     G[i] = angle0_400 (G[i-1] + theta[i] + 200);
     break;
    }
    if (td[j].type == 'A'
     && td[j].ip[0] == numpoint(instr->param[i])
     && td[j].ip[1] == numpoint(instr->param[i+1])
     && td[j].ip[2] == numpoint(instr->param[i-1]))
    {
     theta[i] = -td[j].val;
     G[i] = angle0_400 (G[i-1] + theta[i] + 200);
     break;
    }

   }
  }
  GF1 = G[instr->nparam-2];
  eG = pm200(GF - GF1);
  sprintf (buf, "# ecart angle fermeture = %lf\n", eG);
  printf (buf);
  if (out)
   fprintf (out, buf);
  for (i=1; i<=instr->nparam-2; i++)
  {
   G[i] += i * eG / (instr->nparam-2);
  }

  p[1] = tp[numpoint(instr->param[1])].p;
  for (i=1; i<=instr->nparam-3; i++)
  {
   d[i] = NDEF;
   for (j=0; j<nd; j++)
   {
    if (td[j].type == 'D' &&
        ((td[j].ip[0] == numpoint(instr->param[i]) &&
          td[j].ip[1] == numpoint(instr->param[i+1])) ||
         (td[j].ip[1] == numpoint(instr->param[i]) &&
          td[j].ip[0] == numpoint(instr->param[i+1]))))
	{
	 d[i] = td[j].val;
	 break;
	}
   }
   X(p[i+1]) = X(p[i]) + d[i] * sin (G[i] / 200 * pi);
   Y(p[i+1]) = Y(p[i]) + d[i] * cos (G[i] / 200 * pi);
  }
  B = tp[numpoint(instr->param[instr->nparam-2])].p;
  B1 = p[instr->nparam-2];
  X(dB) = X(B) - X(B1);
  Y(dB) = Y(B) - Y(B1);
  sprintf (buf, "# ecart X = %lf\n# ecart Y = %lf\n", X(dB), Y(dB));
  printf (buf);
  if (out)
   fprintf (out, buf);
  for (i=2; i<=instr->nparam-3; i++)
  {
   X(p[i]) += i / (instr->nparam-3) * X(dB);
   Y(p[i]) += i / (instr->nparam-3) * Y(dB);
   tp[numpoint(instr->param[i])].p = p[i];
   sprintf (buf, "X(%s)=%lf\nY(%s)=%lf\n",
	instr->param[i], X(p[i]),
	instr->param[i], Y(p[i]));
   printf(buf);
   if (out)
    fprintf (out, buf);
  }
/*
  g = angleg (tp[numpoint(instr->param[0])].p,
	      tp[numpoint(instr->param[1])].p);
*/
 }
#endif
 else if (!strcmp (instr->op, "H"))
 {
  if (instr->val[0]=='?')
  {
   printf ("H=%lf\n", h);
   if (out!=NULL)
    fprintf (out, "H=%lf\n", h);
  }
  else
   h = atof(instr->val);
 }
 else if (!strcmp (instr->op, "CL"))
 {
  if (instr->val[0]=='?')
  {
   printf ("CL=%lf\n", CL);
   if (out!=NULL)
    fprintf (out, "CL=%lf\n", CL);
  }
  else
   CL = atof(instr->val);
 }
 else if (!strcmp (instr->op, "COEF"))
 {
  if (instr->val[0]=='?')
  {
   printf ("COEF=%lf\n", coef);
   if (out!=NULL)
    fprintf (out, "COEF=%lf\n", coef);
  }
  else
   coef = atof(instr->val);
 }
 else if (!strcmp (instr->op, "EPS"))
 {
  if (instr->val[0]=='?')
  {
   printf ("EPS=%lf\n", eps);
   if (out!=NULL)
    fprintf (out, "EPS=%lf\n", eps);
  }
  else
   eps = atof(instr->val);
 }
 else if (!strcmp (instr->op, "TX"))
 {
  if (instr->val[0]=='?')
  {
   printf ("TX=%lf\n", Tx);
   if (out!=NULL)
    fprintf (out, "TX=%lf\n", Tx);
  }
  else
   Tx = atof(instr->val);
 }
 else if (!strcmp (instr->op, "PAIRES"))
 {
  if (instr->val[0]=='?')
  {
   printf ("PAIRES=%lf\n", paires);
   if (out!=NULL)
    fprintf (out, "PAIRES=%lf\n", paires);
  }
  else
   paires = atof(instr->val);
 }
 else if (!strcmp (instr->op, "SIGMAD"))
 {
  if (instr->val[0]=='?')
  {
   printf ("SIGMAD=%lf\n", sigmad);
   if (out!=NULL)
    fprintf (out, "SIGMAD=%lf\n", sigmad);
  }
  else
   sigmad = atof(instr->val);
 }
 else if (!strcmp (instr->op, "SIGMAD1"))
 {
  if (instr->val[0]=='?')
  {
   printf ("SIGMAD1=%lf\n", sigmad1);
   if (out!=NULL)
    fprintf (out, "SIGMAD1=%lf\n", sigmad1);
  }
  else
   sigmad1 = atof(instr->val);
 }
 else if (!strcmp (instr->op, "SIGMAANGLE"))
 {
  if (instr->val[0]=='?')
  {
   printf ("SIGMAANGLE=%lf\n", sigmaangle);
   if (out!=NULL)
    fprintf (out, "SIGMAANGLE=%lf\n", sigmaangle);
  }
  else
   sigmaangle = atof(instr->val);
 }
 else if (!strcmp (instr->op, "ESTOP"))
 {
  if (instr->val[0]=='?')
  {
   printf ("ESTOP=%lf\n", eqstop);
   if (out!=NULL)
    fprintf (out, "ESTOP=%lf\n", eqstop);
  }
  else
   eqstop = atof(instr->val);
 }
 else if (!strcmp (instr->op, "DEPLINIT"))
 {
  if (instr->val[0]=='?')
  {
   printf ("DEPLINIT=%lf\n", deplinit);
   if (out!=NULL)
    fprintf (out, "DEPLINIT=%lf\n", deplinit);
  }
  else
   deplinit = atof(instr->val);
 }
 else if (!strcmp (instr->op, "VARSIM"))
 {
  if (instr->val[0]=='?')
  {
   printf ("VARSIM=%d\n", flag_varsimul);
   if (out!=NULL)
    fprintf (out, "VARSIM=%d\n", flag_varsimul);
  }
  else
   flag_varsimul = atoi(instr->val);
 }
 else if (!strcmp (instr->op, "ECHELLE"))
 {
  if (instr->val[0]=='?')
  {
   printf ("ECHELLE=%lf\n", echelle);
   if (out!=NULL)
    fprintf (out, "ECHELLE=%lf\n", echelle);
  }
  else
   echelle = atof(instr->val);
 }
 else if (!strcmp (instr->op, "VITESSEDG"))
 {
  if (instr->val[0]=='?')
  {
   printf ("VITESSEDG=%lf\n", vitessedg);
   if (out!=NULL)
    fprintf (out, "VITESSEDG=%lf\n", vitessedg);
  }
  else
   vitessedg = atof(instr->val);
 }
 else if (!strcmp (instr->op, "ALGO"))
 {
  if (instr->val[0]=='?')
  {
   printf ("ALGO=%s\n", algo);
   if (out != NULL)
    fprintf (out, "ALGO=%s\n", algo);
  }
  else 
   strcpy (algo, instr->val);
 }
 else if (!strcmp (instr->op, "CALCULER"))
 {
 int ipi, i;
  init_resoudre();
  npt = instr->nparam;
  ipi=0;
  for (i=0; i<np; i++)
   tp[i].ipi=-1;
  for (i=0; i<npt; i++)
  {
   tp[numpoint(instr->param[i])].ipi=i;
   B[i] = tp[numpoint(instr->param[i])].p;
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
    if (out!=NULL)
    fprintf (out,"X(%s)=%lf\nY(%s)=%lf\n",
     tp[i].nom, X(tp[i].p), tp[i].nom, Y(tp[i].p));
    plotpointname(tp[i].p,tp[i].nom);
   }
  }
  /* nd = 0; */
 }
 else if (!strcmp(instr->op,"CSPI"))
 {
 int i, j, k;
 int npi;
 int tpi[300];
  npi=0;
  for (i=0; i<np; i++)
  {
   if (!connu(tp[i].p))
   {
    printf ("### point %d=%s connu: %d\n", i, tp[i].nom, connu(tp[i].p));
    tpi[npi++]=i;
   }
  }
  for (j=0; j<npi; j++)
  {
   /*if (!connu(tp[i].p))*/
   i=tpi[j];
   printf ("### point %d=%s connu: %d\n", i, tp[i].nom, connu(tp[i].p));
   {
    printf ("# Calcul du point inconnu %s\n", tp[i].nom);

    init_resoudre();
    npt=1;

    for (k=0; k<np; k++)
     tp[k].ipi=-1;

    tp[i].ipi=0;
    B[0] = tp[i].p; 
   
    resol();
    printf ("# Calcul termine\n");

    tp[i].p = B[0];
    printf ("X(%s)=%lf\nY(%s)=%lf\n",
       tp[i].nom, X(tp[i].p), tp[i].nom, Y(tp[i].p));

/*
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
      if (out!=NULL)
      fprintf (out,"X(%s)=%lf\nY(%s)=%lf\n",
       tp[i].nom, X(tp[i].p), tp[i].nom, Y(tp[i].p));
      plotpointname(tp[i].p,tp[i].nom);
     }
    }
*/
   }
  } 
 }
 else if (!strcmp(instr->op,"CSIP1"))
 {
 int i, j, k, n;
 int npi;
 int tpi[300];
 int debut, fin;
 char nom[30];
  npi=0;

  printf ("# Calcul des points %s a %s\n", instr->param[0], instr->param[1]);
  debut = atoi(instr->param[0]);
  fin = atoi(instr->param[1]);
  printf ("# calcul des points %d a %d\n", debut, fin);
/*
  for (j=debut; j<=fin; j++)
  {
   sprintf (nom, "%d", j);
   i = numpoint(nom);
   printf ("# - point %d\n", i);
   tpi[npi++]=i;
  }
*/
  
  for (i=0; i<np; i++)
  {
   /*if (!connu(tp[i].p))*/
   n = atoi(tp[i].nom);
   printf ("# point %d=%s : ", n, tp[i].nom);
   if (n>=debut && n<=fin)
   {
    printf ("### point %d=%s connu: %d\n", i, tp[i].nom, connu(tp[i].p));
    tpi[npi++]=i;
   }
   else printf(" non\n");
  }
  
  for (j=0; j<npi; j++)
  {
   /*if (!connu(tp[i].p))*/
   i=tpi[j];
   printf ("### point %d=%s connu: %d\n", i, tp[i].nom, connu(tp[i].p));
   {
    printf ("# Calcul du point inconnu %s\n", tp[i].nom);

    init_resoudre();
    npt=1;

    for (k=0; k<np; k++)
     tp[k].ipi=-1;

    tp[i].ipi=0;
    B[0] = tp[i].p; 
   
    resol();
    printf ("# Calcul termine\n");

    tp[i].p = B[0];
    printf ("X(%s)=%lf\nY(%s)=%lf\n",
       tp[i].nom, X(tp[i].p), tp[i].nom, Y(tp[i].p));

/*
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
      if (out!=NULL)
      fprintf (out,"X(%s)=%lf\nY(%s)=%lf\n",
       tp[i].nom, X(tp[i].p), tp[i].nom, Y(tp[i].p));
      plotpointname(tp[i].p,tp[i].nom);
     }
    }
*/
   }
  } 
 }
 else if (!strcmp(instr->op,"CSIP"))
 {
 int i, j, k;
 int debut, fin;
 char nom[30];

  printf ("# Calcul des points %s a %s\n", instr->param[0], instr->param[1]);
  debut = atoi(instr->param[0]);
  fin = atoi(instr->param[1]);
  printf ("# calcul des points %d a %d\n", debut, fin);
  for (j=debut; j<=fin; j++)
  {
   sprintf (nom, "%d", j);
   i = numpoint(nom);

   {
    printf ("# Calcul du point %s=%s\n", nom, tp[i].nom);

    init_resoudre();
    npt=1;

    for (k=0; k<np; k++)
     tp[k].ipi=-1;

    tp[i].ipi=0;
    B[0] = tp[i].p; 
   
    resol();
    printf ("# Calcul termine\n");

    tp[i].p = B[0];
    printf ("X(%s)=%lf\nY(%s)=%lf\n",
       tp[i].nom, X(tp[i].p), tp[i].nom, Y(tp[i].p));

   }
  }
 }
 else if (!strcmp(instr->op,"LIRE"))
 {
  f = fopen(instr->param[0],"r");
  if (f == NULL)
   perror(instr->param[0]);
 }
 else if (!strcmp(instr->op,"ECRIRE"))
 {
  if (instr->param[0][0])
   out = fopen(instr->param[0],"a");
  else
   out=NULL;
 }
 else if (!strcmp(instr->op,"LIRECAN"))
  lirecan (instr->param[0]);
 else if (!strcmp(instr->op,"LIREOBS"))
  lireobs (instr->param[0]);
 else if (!strcmp(instr->op,"ECRIRECAN"))
  ecrirecan (instr->param[0]);
 else if (!strcmp(instr->op,"ECRIREOBS"))
  ecrireobs (instr->param[0]);
 else if (instr->op[0])
  aide();
 return 0.0;
}

resoudre (int af)
{
int i, ipi;
 init_resoudre ();
 npt = np - n;
 printf ("# %d points inconnus\n", npt);
 ipi=0;
 for (i=0; i<np; i++)
 {
  if (connu(tp[i].p))
   tp[i].ipi=-1;
  else
   tp[i].ipi=ipi++;
 }
 for (i=0; i<np; i++)
 {
  X(B[i])=NDEF;
  Y(B[i])=NDEF;
  Z(B[i])=NDEF;
 }
 resol ();
 for (i=0; i<np; i++)
 {
  if (tp[i].ipi>=0)
   tp[i].p = B[tp[i].ipi];
 }

 if (af)
 for (i=0; i<np; i++)
 {
  if (tp[i].ipi>=0)
  {
   printf ("X(%s)=%lf\nY(%s)=%lf\n",
    tp[i].nom, X(tp[i].p), tp[i].nom, Y(tp[i].p));
   if (out!=NULL)
   fprintf (out,"X(%s)=%lf\nY(%s)=%lf\n",
    tp[i].nom, X(tp[i].p), tp[i].nom, Y(tp[i].p));
   plotpointname(tp[i].p, tp[i].nom);  
  }
 }

 /* nd = 0; */
}

interp ()
{
char buf[300];
struct instruction instr[1];
int ic;
int i;
 /* f = fopen ("topo.txt", "r"); */
 f = stdin;

 for (;;)
 {
  fgets (buf, sizeof(buf), f);
  if (out!=NULL)
   fprintf(out,"%s",buf);
  buf[strlen(buf)-1]=0;
  if (buf[strlen(buf)-1]=='\r')
   buf[strlen(buf)-1]=0;
  for (ic=0; ic<strlen(buf); ic++)
   if (buf[ic]>='a' && buf[ic]<='z')
    buf[ic]+='A'-'a';
  if (feof(f) && f!=stdin)
  {
   f = stdin;
   /*init_resoudre();*/
   continue;
  }
  if (!strcmp(buf,"TERMINER") || feof(f))
   break;
  if (!strcmp(buf,"RESOUDRE"))
   resoudre(1);
  else if (!strcmp(buf,"NOUVEAU"))
   nd = 0;
  else if (!strcmp(buf,"DONNEES"))
  {
   for (i=0; i<nd; i++)
   {
    if (td[i].type=='D')
     sprintf (buf, "D(%s,%s)=%lf~%lf\n",
      tp[td[i].ip[0]].nom,
      tp[td[i].ip[1]].nom,
      td[i].val,
      td[i].sigma);
     else if (td[i].type=='A')
      sprintf (buf, "ANGLE(%s,%s,%s)=%lf~%lf\n",
       tp[td[i].ip[1]].nom,
       tp[td[i].ip[0]].nom,
       tp[td[i].ip[2]].nom,
       td[i].val,
       td[i].sigma);
     printf(buf);
     if (out)
      fprintf(out,buf);
   }
  }
  else if (!strcmp(buf,"RESIDUS"))
  {
  real x, r, sigma, s;
  char buf1[300];
   s = 0;
   for (i=0; i<nd; i++)
   {
    if (td[i].type=='D')
    {     
     if (td[i].sigma)
      sigma = td[i].sigma;
     else
      sigma = sigmad + sigmad1 * td[i].val;
     *buf1 = 0;
     if (connu(tp[td[i].ip[0]].p) && connu(tp[td[i].ip[1]].p))
     {
      x = dist (tp[td[i].ip[0]].p, tp[td[i].ip[1]].p);
      r = x-td[i].val;
      s += r*r;
      sprintf (buf1, " %lf cal. res=%lf %s",
       x, r, (fabs(r)<=2.58*sigma)?"":"TOL.DEP.");
     }
     sprintf (buf, "D(%s,%s)=%lf~%lf obs.%s\n",
      tp[td[i].ip[0]].nom,
      tp[td[i].ip[1]].nom,
      td[i].val,
      sigma,
      buf1);
    }
    else if (td[i].type=='A')
    {
     if (td[i].sigma)
      sigma = td[i].sigma;
     else
      sigma = sigmaangle;
     *buf1 = 0;
     if (connu(tp[td[i].ip[0]].p) && connu(tp[td[i].ip[1]].p))
     {
      x = angle3g (tp[td[i].ip[0]].p, tp[td[i].ip[1]].p, tp[td[i].ip[2]].p);
      r = x-td[i].val;
      s += r*r;
      sprintf (buf1, " %lf cal. res=%lf %s",
       x, r, (fabs(r)<=2.58*sigma)?"":"TOL.DEP.");
     }
      sprintf (buf, "ANGLE(%s,%s,%s)=%lf~%lf obs.%s\n",
       tp[td[i].ip[1]].nom,
       tp[td[i].ip[0]].nom,
       tp[td[i].ip[2]].nom,
       td[i].val,
       sigma,
       buf1);
     }
     printf(buf);
     if (out)
      fprintf(out,buf);
   }
   sprintf (buf, "# Residu global : %lf, ecart : %lf\n", sqrt(s), ecart(B));
   printf (buf);
   if (out)
    fprintf (out, buf);
  }
  else if (!strcmp(buf,"POINTS"))
  {
   for (i=0; i<np; i++)
   {    
    if (X(tp[i].p) != NDEF)
     printf ("X(%s)=%lf\n", tp[i].nom, X(tp[i].p));
    if (Y(tp[i].p) != NDEF)
     printf ("Y(%s)=%lf\n", tp[i].nom, Y(tp[i].p));
    if (Z(tp[i].p) != NDEF)
     printf ("Z(%s)=%lf\n", tp[i].nom, Z(tp[i].p));
    if (out)
    {
    if (X(tp[i].p) != NDEF)
     fprintf (out,"X(%s)=%lf\n", tp[i].nom, X(tp[i].p));
    if (Y(tp[i].p) != NDEF)
     fprintf (out,"Y(%s)=%lf\n", tp[i].nom, Y(tp[i].p));
    if (Z(tp[i].p) != NDEF)
     fprintf (out,"Z(%s)=%lf\n", tp[i].nom, Z(tp[i].p));
    }
   } 
  }
  else if (!strcmp(buf,"AIDE")||!strcmp(buf,"HELP"))
   aide();
  else if (!strcmp(buf,"AUTEUR"))
  {
   printf ("\nAuteur : Jacques Bailhache (2005)\n");
   printf ("email: jacquesbailhache@hotmail.com\n");
   printf ("\n");
  }
  else if (buf[0]=='#') ;
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

 echelle = 0.3;
 strcpy (algo, "SIMULT");
 /* printf ("%x %x\n", main, traiter); */
 /*analyser ("ANGLE(M,A,B)=63.8763", instr);*/
 /* printf ("# Resolution de problemes de topographie\n"); */
 printf ("# Logiciel de calculs topographiques\n");
 printf ("# Tapez aide pour avoir de l'aide\n\n");
#ifdef GRAPH
 init_graph();
#endif
 interp();
}

#if 0
main1()
{
 /* printf("%f %f %f",floor(5.2),floor(-5.2),floor(-4)); */
 init();
 /*test();*/
 resol();
}
#endif

