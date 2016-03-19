
#include "rpt.h"

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
   printf ("Y(%s)=%lf\n",
    instr->param[0], tp[numpoint(instr->param[0])].p.xyz[2]);
   if (out!=NULL)
   fprintf (out, "Y(%s)=%lf\n",
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
 else if (!strcmp(instr->op,"BLOC"))
 {
  bloc (instr);
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
 else if (instr->op[0])
  aide();
 return 0.0;
}

