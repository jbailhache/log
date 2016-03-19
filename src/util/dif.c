
#include <stdio.h>
#include <errno.h>

#define LINESIZE 150
#define MAXLINES 200

int ncar = 20;

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
int i, j, l[2], d[2];

    for (i=0; i<2; i++)
	l[i] = 0;

    for (;;)
    {
	if (l[0] >= nl[0] && l[1] >= nl[1])
		break;
	if (!strcmp (lines[0][l[0]], lines[1][l[1]]))
	{
		printf ("%s", lines[0][l[0]]);
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
				for (i=0; i<2; i++)
				{
					for (j=0; j<d[i]; j++)
						printf ("@%d:%s", i,
							lines[i][l[i]+j]);
				}
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
		printf ("%s", line[0]);
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

    memset (lines, 0, sizeof(lines));

    for (i=0; i<2; i++)
    {
	in[i] = fopen (argv[i+1], "r");
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

    if (argc > 3)
	sscanf (argv[3], "%d", &ncar);

    dif (lines, l);

    for (i=0; i<2; i++)
	fclose (in[i]);
}




