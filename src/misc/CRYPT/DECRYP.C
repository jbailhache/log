
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
			for (j=2; i*j<MAX; j++)
			{
				premier[i*j] = 0;
			}
		}
	}
}

int codex[] = { 105, 2, 106, 4, 112, 15, 96, 3, 114, 1, 109, 72, 23, 51,
	108, 16, 90, 89, 81, 56, 111, 6, 94, 63, 2, 98, 61, 56, 103, 7,
	27, 101, 13, 94, 55 };

main ()
{
int n, i, j, m, x;
char buf[200];
	calcul_premiers (premier);
	n = sizeof(codex) / sizeof(codex[0]);

	for (m=1; m<114; m++)
	{
		printf ("%4d : ", m);
		for (i=0; i<n; i++)
			printf ("%c", 0x40 + codex[i] / m);
		gets (buf);
	}

	for (i=0; i<n; i++)
	{
		x = codex[i];
		printf ("%4d : ", x);
		for (m=8; m>=0; m--)
			printf ("%d", (x>>m)&1);
		printf (" ");
		for (m=2; m<=114 && x>1; m++)
		{
			if (premier[m])
			{
				for (j=0; x%m==0; j++)
					x/=m;
				printf ("%2d ", j);
			}
		}
		gets (buf);
	}

	for (m=1; m<=114; m++)
	{
		printf ("%4d : ", m);
		for (i=0; i<n; i++)
			printf ("%c", 'A' + ((codex[i]-1) % m));
		printf ("\n");
		gets (buf);
	}
}
