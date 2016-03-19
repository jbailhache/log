#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#define LINESIZE 1000 /* 150 */
#define MAXLINES 20000 /* 200 */

#define NFICH 2

int ncar = 20;

int nlig = 10;

int ncol = 40;

char format0[60], format1[60];

FILE *out;

int compare (char lines0[][LINESIZE+1], int nl0,
	     char lines1[][LINESIZE+1], int nl1)
{
int n, i, status;
	n = 0;
	for (i=0; i<=nl0 && i<=nl1; i++)
	{
		status = strcmp (lines0[i], lines1[i]);
		if (status)
			return status;
		n += strlen(lines0[i]);
		if (n >= ncar)
			return 0;
	}
	return 1;
}

dif (char lines [2] [MAXLINES+1] [LINESIZE+1], int nl[2])
{
int i, j, l[2], d[2], k, cont;

    for (i=0; i<2; i++)
	l[i] = 0;

    for (;;)
    {
	if (l[0] >= nl[0] && l[1] >= nl[1])
		break;
	if (!strcmp (lines[0][l[0]], lines[1][l[1]]))
	{
		fprintf (out, "  %s", lines[0][l[0]]);
		for (i=0; i<2; i++)
			l[i]++;
	}
	else
	{
		d[0] = 0;
		d[1] = 1;
		for (;;)
		{
			/*
			if (!strcmp (lines[0][l[0]+d[0]],
				lines[1][l[1]+d[1]]))
			*/
			if (!compare (lines[0]+l[0]+d[0], nl[0]-l[0]-d[0],
				lines[1]+l[1]+d[1], nl[1]-l[1]-d[1]))
			{
			trouve:
				/*
				for (i=0; i<2; i++)
				{
					for (j=0; j<d[i]; j++)
				    
						fprintf (out, "@%d:%s", i,
							lines[i][l[i]+j]);
				}
				*/
#if 1
				j = 0;
				boucle:				    
				    /*
				    for (k=0; k<NFICH; k++)
				    {
					if (j < d[k])
					    lines[k][l[k]+j][ncol] = 0; 
				    }   
				    */
				    cont = 0;
				    for (k=0; k<NFICH; k++)
				    {
					if (j<d[k])
					{
					    lines[k][l[k]+j][strlen(lines[k][l[k]+j])-1] = 0;
					    fprintf (out, format1, lines[k][l[k]+j]); 
					    cont = 1;
					}
					else
					{
					    fprintf (out, format0, "");
					}
				    }
				    fprintf (out, "\n");
				    j++;
				    if (cont)
					goto boucle;

#else
				for (i=0; i<NFICH; i++)
				    j[i] = 0;
				i = 0;
			boucle:    
				if (j[i] < d[i])
				{
				    fprintf (out, "@%d:%s", i, lines[i][l[i]+j[i]]);
				    j[i]++;
				}
				if ((j[i] % nlig) == 0 || j[i] >= d[i])
				    i = (i + 1) % NFICH;
				for (k=0; k<NFICH; k++)
				    if (j[k] < d[k])
					goto boucle;

#endif
				for (i=0; i<2; i++)
					l[i] += d[i];
				break;
			}
			if (d[1] == 0)
			{
				d[1] = d[0]+1;
				if (d[1] > nl[0] - l[0] + nl[1] - l[1])
				{
					d[0] = nl[0] - l[0];
					d[1] = nl[1] - l[1];
					goto trouve;
				}
				d[0] = 0;
			}
			else
			{
				d[0]++;
				d[1]--;
			}

		}
	}


    }


#if 0
    for (;;)
    {
	for (i=0; i<2; i++)
		fgets (line[i], sizeof(line[i])-1, in[i]);
	if (feof(f[0]) && feof(f[1]))
		break;
	if (!strcmp (line[0], line[1]))
	{
		fprintf (out, "%s", line[0]);
	}
	else
	{
		/*
		for (j=0; ; j++)
			for (k=0; k<=j; k++)
		*/
		j = 0;
		k = 1;
		for (;;)
		{
			if (k == 0)
			{
				k = j+1;
				j = 0;
			}
			else
			{
				j++;
				k--;
			}
		}


	}

    }
#endif
}

main (int argc, char *argv[])
{
FILE *in[2];
int i, j, l[2];
char lines [2] [MAXLINES+1] [LINESIZE+1];
char line [LINESIZE+1];
char buf[1000];
    
    memset (lines, 0, sizeof(lines));

    for (i=0; i<2; i++)
    {
#ifdef ARGS
	in[i] = fopen (argv[i+1], "r");
#else
	printf ("Fichier %d ? ", i);
	gets (buf);
	in[i] = fopen (buf, "r");
#endif
	if (in[i] == NULL)
	{
		perror (argv[i+1]);
		return 2;
	}
	for (j=0; j<MAXLINES; j++)
	{
		fgets (lines[i][j], LINESIZE, in[i]);
		/*
		fgets (line, LINESIZE, in[i]);
		strncpy (lines[i][j], line, LINESIZE);
		*/
		if (feof(in[i]))
			break;
	}
	l[i] = j;

    }

#ifdef ARGS
    out = stdout;

    if (argc > 3)
	sscanf (argv[3], "%d", &ncar);
#else
    printf ("Sortie ? ");
    gets (buf);
    out = fopen (buf, "w");
    if (out == NULL)
    {
	perror (buf);
	return;
    }

    printf ("Nombre de caracteres ? ");
    gets (buf);
    ncar = atoi (buf);

    printf ("Lignes par bloc ? ");
    gets (buf);
    nlig = atoi (buf);

    printf ("Nombre de colonnes ? ");
    gets (buf);
    ncol = atoi (buf);

    sprintf (format0, "  %%-%d.%ds  ", ncol, ncol);
    sprintf (format1, "@ %%-%d.%ds @", ncol, ncol);
#endif

    dif (lines, l);

    for (i=0; i<2; i++)
	fclose (in[i]);
}











