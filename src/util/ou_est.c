
#include <dir.h>
#include <dos.h>

main (int argc, char *argv[])
{
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

	sprintf (path, "%s\\%s", dir, name);

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
		if (blk.ff_fdate >= date)
		    printf ("%04X %s %d %-12.12s %s\n",
			blk.ff_attrib,
			attributs,
			blk.ff_fdate,
			blk.ff_name,
			dir);
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