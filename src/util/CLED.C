
#include <stdio.h>

#define MAXLINES 100
#define MAXCOLS 300

main ()
{
char lines[MAXLINES][MAXCOLS];
int l;
int c;
int ch;
int n;
char cln[300];

	/*
	for (c=0; c<79; c++)
		cln[c] = ' ';
	cln[c] = 0;
	*/
	cln[0] = 12;
	cln[1] = 0;

	for (l=0; l<MAXLINES; l++)
		lines[l][0] = 0;
	l = 0;
	c = 0;
	n = 1;
	printf ("\n");
	system ("cd");
	printf ("> ");
	for (;;)
	{
		ch = getche ();
		if (ch == 17)
			break;
		/*printf ("%02X ", ch);*/
		if (ch == 0)
		{
			ch = getch();
			switch (ch)
			{
				case 'H':
					l--;
					if (l == -1)
						l = MAXLINES-1;
				chline:
					c = strlen(lines[l]);
					printf ("\r> %s", lines[l]);
					clreol();
					break;

				case 'P':
					l++;
					if (l == MAXLINES)
						l = 0;
					goto chline;

				case 'M':
					putchar ('\b');
					putchar (lines[l][c]);
					c++;
					break;

				case 'K': ;
					c--;
					printf ("\b\b");
					break;

			}

		}
		else if (ch == '\b')
		{
			c--;
		}
		else if (ch == '\r')
		{
			lines[l][c] = 0;
			printf ("*** %s ***\n", lines[l]);
			system (lines[l]);
			system ("cd");
			printf ("> ");
			if (l >= n)
				n++;
			l = n;
			if (n == MAXLINES)
				n = 0;
			c = 0;
		}
		else
		{
			lines[l][c] = ch;
			c++;
		}
	}

}
