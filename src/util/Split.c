
#include <stdio.h>

main (int argc, char *argv[])
{
char line[300];
char filename[200];
FILE *out;

	if (argc > 1)
		freopen (argv[1], "r", stdin);

	for (;;)
	{
		gets (line);
		if (feof (stdin))
			break;
		if (!strncmp (line, "/*SPLIT ", 8))
		{
			fclose (out);
			sscanf (line+8, "%s", filename);
			out = fopen (filename, "a");
			if (out == NULL)
			{
				perror (filename);
				break;
			}
		}
		else
		{
			fprintf (out, "%s\n", line);
		}
	}

}
