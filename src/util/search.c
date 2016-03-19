
#include <stdio.h>
#include <dir.h>
#include <dos.h>

char string[1000];

int search1 (char *data, char *string)
{
int n;
	for (n=0; data[n]; n++)
	{
		if (!strncmp (data+n, string, strlen(string)))
			return n;
	}
	return -1;
}

int search (char *filename, char *string)
{
FILE *f;
char line[1000];
int ln;
int n;
	f = fopen (filename, "r");
	if (f == NULL)
		perror (filename);
	else
	{
		n = 0;
		for (ln=1; ; ln++)
		{
			fgets (line, sizeof(line)-1, f);
			if (feof(f))
				break;
			if (search1 (line, string) >= 0)
			{
				n++;
				printf ("%s (%d) : %s", filename, ln, line);
			}
		}
		fclose (f);
	}
}

main (int argc, char *argv[])
{
	strcpy (string, argv[argc-1]);
	argc--;
	if (argc < 3)
		ou_est (argv[1], "", 0);
	else if (argc < 4)
		ou_est (argv[1], argv[2], 0);
	else
		ou_est (argv[1], argv[2], atoi(argv[3]));
}

ou_est (char *name, char *dir, int date)
{
struct ffblk blk;
int attrib, status;
char attributs[10];
char path[200];
char fdate[60], ftime[60];
char buf[1000];
int n;
char filename[200];

	sprintf (path, "%s\\%s", dir, name);

	printf ("Searching in %s ...\n", path);

	attrib = -1;
	status = findfirst (path, &blk, attrib);

	while (status >= 0)
	{
		strcpy (attributs, "         ");
		if (blk.ff_attrib & FA_RDONLY) attributs[0] = 'R';
		if (blk.ff_attrib & FA_HIDDEN) attributs[1] = 'H';
		if (blk.ff_attrib & FA_SYSTEM) attributs[2] = 'S';
		if (blk.ff_attrib & FA_LABEL)  attributs[3] = 'L';
		if (blk.ff_attrib & FA_DIREC)  attributs[4] = 'D';
		if (blk.ff_attrib & FA_ARCH)   attributs[5] = 'A';
		/*
		sprintf (ftime, asctime (localtime (blk.ff_ftime)));
		sprintf (fdate, asctime (localtime (blk.ff_fdate)));
		*/
		if (blk.ff_fdate >= date && !(blk.ff_attrib & FA_DIREC))
		{
		    sprintf (filename, "%s\\%s", dir, blk.ff_name);
		    n = search (filename, string);
		    if (n > 0)
		    sprintf (buf, "%04X %s %d %-12.12s %s : %n occurences\n",
			blk.ff_attrib,
			attributs,
			blk.ff_fdate,
			blk.ff_name,
			dir,
			n);
		}
		status = findnext (&blk);
	}

	sprintf (path, "%s\\*.*", dir);

	attrib = -1;
	status = findfirst (path, &blk, attrib);

	while (status >= 0)
	{
		strcpy (attributs, "         ");
		if (blk.ff_attrib & FA_RDONLY) attributs[0] = 'R';
		if (blk.ff_attrib & FA_HIDDEN) attributs[1] = 'H';
		if (blk.ff_attrib & FA_SYSTEM) attributs[2] = 'S';
		if (blk.ff_attrib & FA_LABEL)  attributs[3] = 'L';
		if (blk.ff_attrib & FA_DIREC)
		{
			attributs[4] = 'D';
			sprintf (path, "%s\\%s", dir, blk.ff_name);
			if (blk.ff_name[0] != '.')
				ou_est (name, path, date);
		}
		if (blk.ff_attrib & FA_ARCH)   attributs[5] = 'A';
		/*
		printf ("%04X %s %s %s\n",
			blk.ff_attrib, attributs, dir, blk.ff_name);
		*/

		status = findnext (&blk);
	}

}