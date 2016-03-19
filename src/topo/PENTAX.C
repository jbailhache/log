
#include <stdio.h>
#include <math.h>

main()
{
char buf[1000];
char st[100];
char ref[100];
int np;
char buf1[100];
char ah[100], av[100], d[100], npt[100];
char ter[100], obs[100], det[100];
FILE *fter, *fobs, *fdet;
char champ[10][100];
int i, j, k;
float X, Y, H;

 printf ("Fichier terrain ? ");
 gets(ter);
 fter = fopen(ter, "r");
 if (fter == NULL)
 {
  perror(ter);
  exit(0);
 }

 fgets (buf, sizeof(buf), fter);
 /* if (!strncmp (buf, "0000ENQ:", 8)) */
 if (strncmp (buf, "1,,", 3))
 {
 printf ("Fichier observations ? ");
 gets (obs);
 fobs = fopen (obs, "w");
 if (fobs == NULL)
 {
  perror(fobs);
  exit(0);
 }

 printf ("Station ? ");
 gets(st);
 printf ("Reference ? ");
 gets(ref);
/*
 printf ("Numero premier point ? ");
 gets (buf);
 np = atoi(buf);
*/

 np = 1;

 for (;;)
 {
  fgets (buf, sizeof(buf), fter);
  if (feof(fter))
   break;
  /* printf ("%s\n",buf); */
  if (!strncmp (buf+4, "PPD:", 4))
  {
   strncpy (npt, buf+8, strlen(buf)-10);
   npt[strlen(buf)-10]=0;
  }
  else if (!strncmp (buf+4, "RTH:", 4))
  {
   strncpy (ah, buf+8, 8);
   ah[8] = 0;
  }
  else if (!strncmp (buf+4, "RTV:", 4))
  {
   strncpy (av, buf+8, 8);
   av[8] = 0;
  }
  else if (!strncmp (buf+4, "RTD:", 4))
  {
   strncpy (d, buf+8, 8);
   d[8] = 0;
   if (npt[0]==0)
    sprintf (npt, "%d", np++);
   fprintf (fobs,"%s %s %s %s %s %s 0 0\n",
    st, ref, npt, ah, d, av);
   /*np++;*/

  }

 }
 }
 else
 {
  printf ("Fichier d‚tail ? ");
  gets (det);
  fdet = fopen (det, "w");
  if (fdet == NULL)
  {
   perror (det);
   exit(0);
  }
  /*fprintf (fdet, "<PROTOM> Points de d‚tails\n");*/
  fprintf (fdet, "<PROTOM V:06.01.2005> Points de d‚tails du chantier de ?? ( Calcul : 000000000 )\n\n");

  printf ("X station ? ");
  gets (buf);
  X = atoi(buf);

  printf ("Y station ? ");
  gets (buf);
  Y = atoi(buf);

  printf ("H station ? ");
  gets (buf);
  H = atoi(buf);

  for (;;)
  {
   fgets (buf, sizeof(buf), fter);
   if (feof(fter))
    break;
   j = 0;
   k = 0;
   for (i=0; i<strlen(buf); i++)
   {
    if (buf[i] == 0 || buf[i] == '\n')
    {
     champ[j][k] = 0;
     j++;
     break;
    }
    else if (buf[i] == ',')
    {
     champ[j][k] = 0;
     j++;
     k=0;
    }
    else
    {
     champ[j][k++] = buf[i];
    }
   }
   /* fprintf (fdet, " %s %s %s %s\n", champ[2], champ[4], champ[5], champ[6]); */
   fprintf (fdet, " %s %6.2f %6.2f %6.2f\n",
    champ[2], X+atof(champ[4]), Y+atof(champ[5]), H+atof(champ[6]));


  }


 }

}
