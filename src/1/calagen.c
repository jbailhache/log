
/* Calage par transformation d'Helmert avec poids et correspondance point - droite */
/* Auteur : Jacques Bailhache (jacquesbailhache@hotmail.com), mai 2006 */

#include "stdio.h"
#include "math.h"
#include "matr.h"

struct point
{
 int n;
 float x, y;
};

struct corresp 
{
 int n;
 float p;
 int r1, r2, c1, c2;
};

struct point connu[1000];
struct point rayonne[1000];
struct corresp corresp[1000];

int n_connu, n_rayonne, n_corresp;

char nf_rayonnes[300];
char nf_connus[300];
char nf_corresp[300];
FILE *f_rayonnes, *f_connus, *f_corresp;

ouvrir_fichiers ()
{
lire_connus:
 printf ("Fichier des points connus (format NXY) ? ");
 gets (nf_connus);
 f_connus = fopen (nf_connus, "r");
 if (f_connus == NULL)
 {
  perror(nf_connus);
  goto lire_connus;
 }

lire_rayonnes:
 printf ("Fichier des points rayonnés (format NXY) ? ");
 gets (nf_rayonnes);
 f_rayonnes = fopen (nf_rayonnes, "r");
 if (f_rayonnes == NULL)
 {
  perror(nf_rayonnes);
  goto lire_rayonnes;
 }

lire_corresp:
 printf ("Fichier de correspondance au format P R1 R2 C1 C2 avec :");
 printf (" P = poids\n");
 printf (" R1 = numéro du 1er point rayonne\n");
 printf (" R2 = numéro du 2eme point rayonne (droite) ou 0\n");
 printf (" C1 = numéro 1er point connu\n");
 printf (" C2 = numéro 2eme point connu (droite) ou 0\n");
 printf ("Nom du fichier ?");
 gets (nf_corresp);
 f_corresp = fopen (nf_corresp, "r");
 if (f_corresp == NULL)
 {
  perror(nf_corresp);
  goto lire_corresp;
 }

}

lire_fichiers ()
{
char line[1000];
int n, a, b;
float x, y, p;
int r1, r2, c1, c2;

 n_connu = 0;
 n_rayonne = 0;
 n_corresp = 0;

 for (;;)
 {
  fgets (line, sizeof(line), f_connus);
  if (feof(f_connus))
   break;
  sscanf (line, "%d %f %f", &n, &x, &y);
  connu[n_connu].n = n;
  connu[n_connu].x = x;
  connu[n_connu].y = y;
  n_connu++;
 }

 for (;;)
 {
  fgets (line, sizeof(line), f_rayonnes);
  if (feof(f_rayonnes))
   break;
  sscanf (line, "%d %f %f", &n, &x, &y);
  rayonne[n_rayonne].n = n;
  rayonne[n_rayonne].x = x;
  rayonne[n_rayonne].y = y;
  n_rayonne++;
 }

 for (;;)
 {
  fgets (line, sizeof(line), f_corresp);
  if (feof(f_corresp))
   break;
  b=0;
  sscanf (line, "%f %d %d %d %d", &p, &r1, &r2, &c1, &c2);
  corresp[n_corresp].p = p;
  corresp[n_corresp].r1 = r1;
  corresp[n_corresp].r2 = r2;
  corresp[n_corresp].c1 = c1;
  corresp[n_corresp].c2 = c2;
  n_corresp++;
 }

}

afficher ()
{
int i;
 printf ("Points connus : \n");
 for (i=0; i<n_connu; i++)
  printf (" %d X=%f Y=%f\n", connu[i].n, connu[i].x, connu[i].y);

 printf ("Points rayonnés : \n");
 for (i=0; i<n_rayonne; i++)
  printf (" %d X=%f Y=%f\n", rayonne[i].n, rayonne[i].x, rayonne[i].y);

 printf ("Correspondance : \n");
 for (i=0; i<n_corresp; i++)
  printf (" poids %f, points rayonnés %d %d, points connus %d %d\n",
	corresp[i].p, corresp[i].r1, corresp[i].r2, corresp[i].c1, corresp[i].c2);

}

float coordx(int n)
{
int i;
 for (i=0; i<n_rayonne; i++)
 {
  if (rayonne[i].n == n)
   return rayonne[i].x;
 }
 /* printf ("Point %d inexistant\n", n); */
 return 0;
}

float coordy(int n)
{
int i;
 for (i=0; i<n_rayonne; i++)
 {
  if (rayonne[i].n == n)
   return rayonne[i].y;
 }
 /* printf ("Point %d inexistant\n", n); */
 return 0;
}

float coordX(int n)
{
int i;
 for (i=0; i<n_connu; i++)
 {
  if (connu[i].n == n)
   return connu[i].x;
 }
 /* printf ("Point %d inexistant\n", n); */
 return 0;
}

float coordY(int n)
{
int i;
 for (i=0; i<n_connu; i++)
 {
  if (connu[i].n == n)
   return connu[i].y;
 }
 /* printf ("Point %d inexistant\n", n); */
 return 0;
}

float distance2_point_droite (XC, YC, XA, YA, XB, YB)
{
float XAB, YAB, AB2, XAC, YAC, XE, YE, XCE, YCE, ABAC;
 XAB = XB-XA;
 YAB = YB-YA;
 AB2 = XAB*XAB + YAB*YAB;
 XAC = XC-XA;
 YAC = YC-YA;
 ABAC = XAB*XAC + YAB*YAC;
 XE = XA + ABAC/AB2 * XAB;
 YE = YA + ABAC/AB2 * YAB;
 XCE = XE-XC;
 YCE = YE-XC;
 return XCE*XCE + YCE*YCE;
}

float residu (float a, float b, float p, float q, int i)
{
float x1, y1, x2, y2, X1, Y1, X2, Y2, X3, Y3, X4, Y4, dX, dY;
 x1 = coordx(corresp[i].r1);
 y1 = coordy(corresp[i].r1);
 x2 = coordx(corresp[i].r2);
 y2 = coordy(corresp[i].r2);
 X3 = coordX(corresp[i].c1);
 Y3 = coordY(corresp[i].c2);
 X4 = coordX(corresp[i].c1);
 Y4 = coordY(corresp[i].c2);
 X1 = p + a*x1 - b*y1;
 Y1 = q + b*x1 + a*y1;
 X2 = p + a*x2 - b*y2;
 Y2 = p + b*x2 + a*y2;
 if (corresp[i].r2 == 0 && corresp[i].c2 == 0)
 { /* correspondance point - point */
  dX = X1 - X3;
  dY = Y1 - Y3;
  return dX*dX + dY*dY;
 }
 else if (corresp[i].r2 == 0)
 { /* correspondance point - droite */
  return distance2_point_droite (X1, Y1, X3, Y3, X4, Y4);
 }
 else if (corresp[i].c2 == 0)
 { /* correspondance droite - point */
  return distance2_droite_point (X3, Y3, X1, Y1, X2, Y2);
 }
}

float residus (float a, float b, float p, float q)
{
float r, r1;
int i;
 r = 0;
 for (i=0; i<n_corresp; i++)
 {
  r1 = residu (a, b, p, q, i);
  r += corresp[i].p * r1;
 }
 return r;
}

#define damin 0.0001
#define dbmin 0.0001
#define dpmin 0.01
#define dqmin 0.01

traiter ()
{
float a, b, p, q, da, db, dp, dq, r, t;
 a=1;
 b=0;
 p=0;
 q=0;
 da=0.1;
 db=0.1;
 dp=1;
 dq=1;
 r = residus (a, b, p, q);
 for (;;)
 {
  t = residus (a+da, b, p, q);
  if (t<r) 
  {
   a+=da;
   da*=2;
  }
  else
  {
   t = residus (a-da, b, p, q);
   if (t<r)
   {
    a-=da;
    da*=2;
   }
   else
    da/=2;
  }

  t = residus (a, b+db, p, q);
  if (t<r) 
  {
   b+=db;
   db*=2;
  }
  else
  {
   t = residus (a, b-db, p, q);
   if (t<r)
   {
    b-=db;
    db*=2;
   }
   else
    db/=2;
  }

  t = residus (a, b, p+dp, q);
  if (t<r) 
  {
   p+=dp;
   dp*=2;
  }
  else
  {
   t = residus (a, b, p-dp, q);
   if (t<r)
   {
    p-=dp;
    dp*=2;
   }
   else
    dp/=2;
  }

  t = residus (a, b, p, q+dq);
  if (t<r) 
  {
   q+=dq;
   dq*=2;
  }
  else
  {
   t = residus (a, b, p, q-dq);
   if (t<r)
   {
    q-=dq;
    dq*=2;
   }
   else
    dq/=2;
  }
 if (da<damin && db<dbmin && dp<dpmin && dq<dqmin)
  break;
 }
 
}

#if 0
traiter ()
{

struct matr A[1];
struct matr K[1];
struct matr tAPA[1];
struct matr itAPA[1];
struct matr tAPK[1];
struct matr abpq[1];
int i, j, k; 
float ab, dx, dy, xi, yi, Xa, Ya;
float a, b, p, q, c, d, e, f;

char buf[100];

/*
traiter ()
{
*/

 A->nl = 0;
 A->nc = 4;
 K->nl = 0;
 K->nc = 1;
 tAPA->nl = 4;
 tAPA->nc = 4;
 tAPK->nl = 4;
 tAPK->nc = 1;
 abpq->nl = 4;
 abpq->nc = 1;

 for (i=0; i<n_corresp; i++)
 {
  Xa = coordX(corresp[i].a);
  Ya = coordY(corresp[i].a);

  if (corresp[i].b)
  {
   dx = coordX(corresp[i].b)-Xa;
   dy = coordY(corresp[i].b)-Ya;
   ab = sqrt (dx*dx + dy*dy);
   c = dx*dx /ab/ab;
   d = e = dx*dy /ab/ab;
   f = dy*dy /ab/ab;
  }
  else
  {
   c=d=e=f=0;
  }
  xi = coordx(corresp[i].n);
  yi = coordy(corresp[i].n);

  A->nl+=2;
  coef(A,A->nl-2,0) = (1-c)*xi - e*yi;
  coef(A,A->nl-1,0) = -d*xi + (1-f)*yi;
  coef(A,A->nl-2,1) = -(1-c)*yi - e*xi;
  coef(A,A->nl-1,1) = d*yi + (1-f)*xi;
  coef(A,A->nl-2,2) = 1-c;
  coef(A,A->nl-1,2) = -d;
  coef(A,A->nl-2,3) = -e;
  coef(A,A->nl-1,3) = 1-f;

  K->nl+=2;
  coef(K,K->nl-2,0) = (1-c)*Xa - e*Ya;
  coef(K,K->nl-1,0) = -d*Xa+(1-f)*Ya;

 }

 printf ("Matrice A : ");
 printmatr(A);
 printf ("Matrice K : ");
 printmatr(K);

 for (i=0; i<4; i++)
 for (j=0; j<4; j++)
 {
  coef(tAPA,i,j)=0;
  for (k=0; k<2*n_corresp; k++)
   coef(tAPA,i,j) += corresp[k/2].p * coef(A,k,i) * coef(A,k,j);
 }

 gets(buf);
 printf ("Matrice tAPA : ");
 printmatr (tAPA);

 for (i=0; i<4; i++)
 {
  coef(tAPK,i,0) = 0;
  for (k=0; k<2*n_corresp; k++)
   coef(tAPK,i,0) += corresp[k/2].p * coef(A,k,i) * coef(K,k,0);
 }

 printf ("Matrice tAPK : ");
 printmatr (tAPK);

 inverse (tAPA, itAPA);

 printf ("Matrice itAPA : ");
 printmatr (itAPA);

 prodmatr (itAPA, tAPK, abpq);

 printf ("Matrice abpq : ");
 printmatr(abpq);

 a = coef(abpq,0,0);
 b = coef(abpq,1,0);
 p = coef(abpq,2,0);
 q = coef(abpq,3,0);

 printf ("a=%f b=%f p=%f q=%f\n", a, b, p, q);

/*
}

resultats()
*/
{
int i;
float x, y, X, Y;
FILE *fr, *fc;
 fr = fopen ("residus.txt","w");
 if (fr==NULL)
  perror("residus.txt");
 else
 {
  for (i=0; i<A->nl; i++)
   fprintf (fr, "%f\n", a*coef(A,i,0) + b*coef(A,i,1) + p*coef(A,i,2) + q*coef(A,i,3));
  fclose(fr);
 }

 fc = fopen("coord.txt","w");
 if (fc==NULL)
  perror("coord.txt");
 else
 {
  for (i=0; i<n_rayonne; i++)
  {
   x = rayonne[i].x;
   y = rayonne[i].y;
   X = p + a*x - b*y;
   Y = q + b*x + a*y;
   fprintf (fc, "%d %f %f\n", rayonne[i].n, X, Y);
  }
 }
}
}
#endif

main ()
{
 ouvrir_fichiers();
 lire_fichiers();
 afficher();
 traiter();
 /*resultats();*/
}

