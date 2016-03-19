
#include <stdlib.h>

#define MAX 1000

int premier[MAX];

calcul_premiers (int premier[])
{
int i, j;
	for (i=0; i<MAX; i++)
		premier[i] = 1;
	for (i=2; i<MAX; i++)
	{
		if (premier[i])
		{
			/* Les multiples de i ne sont pas premiers */
			for (j=2; i*j<MAX; j++)
			{
				premier[i*j] = 0;
			}
		}
	}
	j = 1;
	for (i=2; i<MAX; i++)
		if (premier[i])
		{
			/* i est le j-eme nombre premier */
			premier[i] = j++;
		}
}

test_premier ()
{
int i, j;
char buf[200];
	calcul_premiers (premier);
/*
	j = 1;
	for (i=2; i<MAX; i++)
		if (premier[i])
		{
			premier[i] = j;
			printf ("%3d %4d\n", j++, i);
		}
*/
	for (;;)
	{
		printf ("Nombre ? ");
		gets (buf);
		if (!*buf)
			break;
		printf ("%deme nombre premier\n", premier [atoi(buf)]);

	}
}

int valeur (char *nom)
{
int i, l, v;
	v = 0;
	l = strlen (nom);
	for (i=0; i<l; i++)
		v += nom[i] - 0x40;
	return v;
}

main ()
{
char nom[1000];
int val;
	calcul_premiers (premier);
	for (;;)
	{
		printf ("Nom en majuscules sans espaces ? ");
		gets (nom);
		if (!*nom)
			break;
		val = valeur (nom);
		printf ("%d = %deme nombre premier; %d = %deme nombre premier\n",
			val, premier[val], premier[val], premier[premier[val]]);

	}
}
