
#include "rpt.h"

point inc[MAX];

#define MAXM 30

real obs_calc (point *B, int i)
{
real r;
real dx, dy;
 if (td[i].type == 'D')
 {
  dx = X(POINT(1)) - X(POINT(0));
  dy = Y(POINT(1)) - Y(POINT(0));
  r = sqrt (dx*dx + dy*dy);
 }
 else if (td[i].type == 'A')
 {
  r = angle3g (POINT(0), POINT(1), POINT(2));
 }
 return r;
}

/*
ecart_bloc (point *B, int i)
{
  if (td[i].type == 'D' && connu(POINT(0)) && connu(POINT(1)))
  {
   if (td[i].sigma)
    sigma = td[i].sigma;
   else
    sigma = sigmad;

   e1 = edist (POINT(0), POINT(1), td[i].val, sigma);
   e += e1*td[i].coef;
  }
  else if (td[i].type == 'A'
	&& connu(POINT(0)) && connu(POINT(1)) && connu(POINT(2)))
  {
   if (td[i].sigma)
    sigma = td[i].sigma;
   else
    sigma = sigmaangle;

   e1 = eangle (POINT(0), POINT(1), POINT(2), td[i].val, sigma);
   e += e1*td[i].coef;
  }

}

*/

/* DX = inv(tA P A) tA P (L-L0) */

#define EPS 0.001

bloc1 ()
{
real A[MAXM*MAX];
real P[MAXM*MAXM];
real L[MAXM];
real L0[MAXM];
real f;
point inc1[MAX];
real tA[MAX*MAXM];
real tAP[MAX*MAXM];
real B[MAXM];
real tAPA[MAX*MAX];
real itAPA[MAX*MAX];
real tAPB[MAX];
real DX[MAX];

int i, j, k, h;

 for (i=0; i<nd; i++)
 for (j=0; j<nd; j++)
 {
  if (i==j)
  {
   /*P[i*nd+j] = 1;*/ /* poids de la mesure i */
   if (td[i].sigma == 0)
   {
    if (td[i].type == 'D')
     td[i].sigma = sigmad;
    else if (td[i].type == 'A')
     td[i].sigma = sigmaangle;
   }
   P[i*nd+j] = td[i].coef / (td[i].sigma * td[i].sigma);
  }
  else
   P[i*nd+j] = 0;
 }

 for (i=0; i<nd; i++)
  L[i] = td[i].val;

 for (i=0; i<nd; i++)
  L0[i] = obs_calc (inc, i);

 for (i=0; i<nd; i++)
 for (j=0; j<npt; j++)
 for (k=0; k<2; k++)
 {
  /* Ai(j*2+k) = derivee mesure i par rapport a coord j*2+k
     avec k=0 pour X et 1 pour Y */
  for (h=0; h<npt; h++)
   inc1[h] = inc[h];
  if (k==0)
   X(inc1[k]) += EPS;
  else if (k==1)
   Y(inc1[k]) += EPS;
  f = obs_calc (inc1, i);
  A[i*2*npt+2*j+k] = (L0[i]-f)/EPS;
 }

 transpose (nd, 2*npt, A, tA);
 prodmat (2*npt, nd, nd, tA, P, tAP);
 prodmat (2*npt, nd, 2*npt, tAP, A, tAPA);
 inv (2*npt, tAPA, itAPA);
 comblinmat (nd, 1, 1, L, -1, L0, B);
 prodmat (2*npt, nd, 1, tAP, B, tAPB);
 prodmat (2*npt, 2*npt, 1, itAPA, tAPB, DX);

 for (i=0; i<npt; i++)
 {
  X(inc[i]) += DX[2*i];
  Y(inc[i]) += DX[2*i+1];
 }

}

bloc (struct instruction *instr)
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
   inc[i] = tp[numpoint(instr->param[i])].p;
  }
  bloc1 ();
  for (i=0; i<np; i++)
  {
   if (tp[i].ipi>=0)
    tp[i].p = inc[tp[i].ipi];
  }

}

