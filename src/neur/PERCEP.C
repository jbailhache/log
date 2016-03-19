
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define N_PATTERNS 5

#define N_INPUTS 15
#define N_OUTPUTS N_PATTERNS

#define eta 0.75

float w[N_INPUTS][N_OUTPUTS];

int o[N_PATTERNS][N_OUTPUTS];

float theta [N_OUTPUTS];

int th[N_PATTERNS][N_OUTPUTS];

int pattern [N_PATTERNS] [N_INPUTS] = {

 { 0,1,0,
   1,0,1,
   1,0,1,
   1,0,1,
   0,1,0 },

 { 0,1,0,
   0,1,0,
   0,1,0,
   0,1,0,
   0,1,0 },

 { 1,1,1,
   0,0,1,
   1,1,1,
   1,0,0,
   1,1,1 },

 { 1,1,1,
   0,0,1,
   1,1,1,
   0,0,1,
   1,1,1 },

 { 1,0,0,
   1,0,1,
   1,1,1,
   0,0,1,
   0,0,1 }

   };




main ()
{
int i, j, p, t, dif, modif;
float a;

	for (i=0; i<N_INPUTS; i++)
	for (j=0; j<N_OUTPUTS; j++)
		w[i][j] = (float)(random(200) - 100) / 100.0;

	for (j=0; j<N_OUTPUTS; j++)
		theta[j] = 0.0;

	for (p=0; p<N_PATTERNS; p++)
	for (j=0; j<N_OUTPUTS; j++)
		th[p][j] = p == j;

	modif = 1;

	for (t=0; modif; t++)
	{
		printf ("\nt = %d :\n", t);
		modif = 0;
		for (p=0; p<N_PATTERNS; p++)
		{
			for (j=0; j<N_OUTPUTS; j++)
			{
				a = 0;
				for (i=0; i<N_INPUTS; i++)
				{
					a += pattern[p][i] * w[i][j];
				}
				if (a <= theta[j])
					o[p][j] = 0;
				else
					o[p][j] = 1;
				printf (" %d", o[p][j]);
			}
			printf ("\n");

			for (j=0; j<N_OUTPUTS; j++)
			{
				dif = th[p][j] - o[p][j];
				if (dif)
					modif = 1;
				for (i=0; i<N_INPUTS; i++)
				{
					w[i][j] += eta * dif * pattern[p][i];
				}
			}

		}
	}
}
