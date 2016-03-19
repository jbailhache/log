
#include <stdio.h>

#define NL 20
#define NC 76

char space[NL][NC];
char space1[NL][NC];

display (char space[NL][NC])
{
int l, c;
	gotoxy (1, 1);
	printf ("\n");
	for (l=0; l<NL; l++)
	{
		for (c=0; c<NC; c++)
		{
			if (space[l][c] > 0)
				printf ("#");
			else
				printf (".");
		}
		printf ("\n");
	}
}

int n_voisins (char space[NL][NC], int l, int c)
{
int n, i, j;
	n = 0;
	for (i=-1; i<=1; i++)
	for (j=-1; j<=1; j++)
		n += space[l+i][c+j];
	return n;
}

step (char space[NL][NC])
{
int l, c, n;
/* char space1[NL][NC]; */
	for (l=1; l<NL-1; l++)
		for (c=1; c<NC-1; c++)
		{
			n = n_voisins (space, l, c);
			if (space[l][c] <= 0)
			{
				if (n == 3)
					space1[l][c] = 1;
				else
					space1[l][c] = 0;
			}
			else
			{
				if (n == 3 || n == 4)
					space1[l][c] = 1;
				else
					space1[l][c] = 0;
			}

		}
	memcpy (space, space1, NL*NC /*sizeof(space)*/);
}

main (int argc, char *argv[])
{
FILE *f;
int l, c;
int ch;

	if (argc != 2)
	{
		printf ("Usage: LIFE filename\n");
		return;
	}

	for (l=0; l<NL; l++)
		for (c=0; c<NC; c++)
		{
			space[l][c] = 0;
			space1[l][c] = 0;
		}

	f = fopen (argv[1], "r");
	if (f == NULL)
	{
		perror (argv[1]);
	}
	else
	{
		l = 1;
		c = 1;
		for (;;)
		{
			ch = getc (f);
			if (feof(f))
				break;
			if (ch == '\n')
			{
				l++;
				c = 1;
			}
			else if (ch == '.' || ch == ' ')
			{
				space[l][c++] = 0;
			}
			else
			{
				space[l][c++] = 1;
			}
		}

		clrscr ();

		for (;;)
		{
			display (space);

			while (!kbhit());
			getch();

			step (space);
		}

	}


}
