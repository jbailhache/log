#include <dir.h>
#include <dos.h>

tabul (int level)
{
int i;
	for (i=0; i<level; i++)
		printf ("\t");
}

map (char *pathname, char *relpath, int level)
{
struct ffblk blk;
int attrib;
int status;
char pathname1[300], relpath1[300];
char path[300];

	sprintf (path, "%s\\*.*", pathname);
	attrib = -1;
	status = findfirst (path, &blk, attrib);
	tabul (level);
	printf ("<ul>\n");

	while (status >= 0)
	{
		if (strcmp (blk.ff_name, ".") && strcmp (blk.ff_name, ".."))
		{
			tabul (level);
			if (!(blk.ff_attrib & FA_DIREC))
				printf ("<li><a href=\"%s/%s\">%s</a>\n",
					relpath, blk.ff_name, blk.ff_name);
			else
			{
				printf ("<li>%s\n", blk.ff_name);
				sprintf (pathname1, "%s\\%s", pathname, blk.ff_name);
				sprintf (relpath1, "%s/%s", relpath, blk.ff_name);
				map (pathname1, relpath1, level+1);
			}
		}

		status = findnext (&blk);
	}

	tabul (level);
	printf ("</ul>\n");

}

main (int argc, char *argv[])
{
	map (argv[1], ".", 0);
}


