
#include <math.h>

#define MAX 200
#define EPS 0.01

float moyenne (float valeur[], int n)
{
float coef[MAX];
float scoef;
int i, j;
float s, d;
	scoef = 0.0;
	for (i=0; i<n; i++)
	{
		coef[i] = 0.0;
		for (j=0; j<n; j++)
		{
			if (i != j)
			{
				d = valeur[i] - valeur[j];
				coef[i] += 1.0 / (d * d + EPS);
			}
		}
		scoef += coef[i];
		printf ("Coef %d = %f\n", i, coef[i]);
	}
	s = 0;
	for (i=0; i<n; i++)
	{
		s += valeur[i] * coef[i];
	}
	return s / scoef;
}

main ()
{
float valeur[MAX];
char buf[60];
int n;
float m;
	n = 0;
	for (;;)
	{
		gets (buf);
		if (!buf[0])
			break;
		valeur[n] = atof (buf);
		n++;
	}
	m = moyenne (valeur, n);
	printf ("Moyenne : %f\n", m);
}
