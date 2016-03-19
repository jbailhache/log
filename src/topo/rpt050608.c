
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define pi 3.1415926

#define real double

#define DIM 3

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

real egisement (point B, point A, real g, real sigma)
{
real u;
 if (confondus(B,A))
  return 1;
 u = pm200(angleg(B,A)-g)/sigma;
 return u*u;
}

#define MAX 30
point A[MAX], C, B[MAX];
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
struct
{
 char type; /* D=distance, A=angle */
 int ip[4];
 real val;
 real coef;
 real sigma;
} td[60];

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
   B[k] = N[k];
 }
 /* printf (" *** m=%lf ", m); */
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
   || buf[i]=='/')
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

#define NDEF -1000000000

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
 if (v==0)
  v=1;
}

FILE *f;

FILE *out=NULL;

aide ()
{
 printf ("Resolution de problemes de topographie\n");
 printf ("\Introduction de donnees :\n");
 printf (" expression=valeur ou expression=valeur~ecart-type\n");
 printf ("Calcul :\n");
 printf (" expression?\n");
 printf ("Expressions :\n");
 printf (" X(A), Y(A) : abcisse et ordonnee du point A\n");
 printf (" D(A,B) : Distance reduite entre A et B\n");
 printf (" DILZ(A,B,LZ) : distance inclinee entre A et B avec lecture zenitale LZ\n");
 printf (" ANGLE(A,B,C) : angle de sommet B\n");
 printf (" G(A,B) : gisement\n");
 printf (" SIGMAD, SIGMAD1, SIGMAANGLE : ecarts types pour les distances et les angles\n");
 printf (" H : altitude du chantier ; ");
 printf (" CL : correction Lambert\n");
 printf (" EPS, TX, PAIRES : tolerances pour le V0 et nombre de paires de sequences\n");
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
   }
  }
  /* nd = 0; */
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
  real x, r, sigma;
  char buf1[300];
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
 /* printf ("%x %x\n", main, traiter); */
 /*analyser ("ANGLE(M,A,B)=63.8763", instr);*/
 printf ("# Resolution de programmes de topographie\n");
 printf ("# Tapez aide pour avoir de l'aide\n\n");
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

