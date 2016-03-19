
#include <stdio.h>
#include <errno.h>

cmpstr (char *s1, char *s2)
{
int x, i1, i2, l1, l2, k;
	l1 = strlen (s1);
	l2 = strlen (s2);
	x = 0;
	for (i1=0; i1<l1; i1++)
	for (i2=0; i2<l2; i2++)
	{
		for (k=0; i1+k<l1 && i2+k<l2 && s1[i1+k]==s2[i2+k]; k++);
		x += k;
	}
	return x;
}

main1 (int argc, char *argv[])
{
int x;
	x = cmpstr (argv[1], argv[2]);
	printf ("%d\n", x);
}

#define MAXLINE 300
#define N_SIMIL 20

main ()
{
FILE *f;
char filename[200];
char line[MAXLINE], line1[MAXLINE];
struct
{
	int sim;
	char line[MAXLINE];
} simil[N_SIMIL];
int sim, i, j;
char buf[300];

get_filename:
	printf ("Nom du fichier ? ");
	gets (filename);
	f = fopen (filename, "r+");
	if (f == NULL)
	{
		f = fopen (filename, "w");
		if (f == NULL)
		{
			perror (filename);
			goto get_filename;
		}
		fclose (f);
		f = fopen (filename, "r+");
		if (f == NULL)
		{
			perror (filename);
			goto get_filename;
		}
	}

	for (;;)
	{
		printf ("Texte ? ");
		gets (line);
		if (!*line)
			break;
		for (i=0; i<N_SIMIL; i++)
		{
			simil[i].sim = 0;
			simil[i].line[0] = 0;
		}
		fseek (f, 0, SEEK_SET);
		for (;;)
		{
			fgets (line1, sizeof(line1), f);
			line1[strlen(line1)-1] = 0;
			if (feof(f))
				break;
			sim = cmpstr (line, line1);
			for (i=0; i<N_SIMIL; i++)
			{
				if (sim >= simil[i].sim)
				{
					for (j=N_SIMIL-2; j>=i; j--)
					{
#ifdef TRACE
						printf (" %d->%d ", j, j+1);
#endif
						memcpy (simil+j+1, simil+j, sizeof(simil[j]));
					}
					simil[i].sim = sim;
					strcpy (simil[i].line, line1);
					break;
				}
			}
#ifdef TRACE
		for (i=0; i<N_SIMIL && simil[i].sim; i++)
		{
			printf ("%4d %s\n", simil[i].sim, simil[i].line);
		}
		printf ("\n");
		gets (buf);
#endif
		}
		for (i=0; i<N_SIMIL && simil[i].sim; i++)
		{
			printf ("%4d %s\n", simil[i].sim, simil[i].line);
		}
		printf ("Enregistrer ? ");
		gets (buf);
		if (*buf != 'n' && *buf != 'N')
		{
			fseek (f, 0, SEEK_END);
			fprintf (f, "%s\n", line);
		}

	}
	fclose (f);
}