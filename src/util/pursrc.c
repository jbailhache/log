#include <stdio.h>
#include <string.h>

tab2space (char *line_in, char *line_out)
{
int i, j, k, n;
    for (i=0, j=0; line_in[i]; i++)
    {
        if (line_in[i] == '\t')
        {
            n = 8 - (j % 8);
            for (k=0; k<n; k++)
               line_out[j++] = ' ';
        }
        else
            line_out[j++] = line_in[i];
    }
    line_out[j] = 0;
}

main ()
{
char filename_in[200], filename_out[200];
FILE *in, *out;
char line_in[1000], line_out[1000];
int flag_delete;
int flags, flags1;
char buf[80];

    printf ("Input file ? ");
    gets (filename_in);
    in = fopen (filename_in, "r");
    if (in == NULL)
    {
        perror (filename_in);
        return;
    }

    printf ("Output file ? ");
    gets (filename_out);
    out = fopen (filename_out, "w");
    if (out == NULL)
    {
        perror (filename_out);
        return;
    }

    printf ("Flags ? ");
    gets (buf);
    sscanf (buf, "%x", &flags);

    flag_delete = 0;

    for (;;)
    {
        fgets (line_in, sizeof(line_in), in);
        if (feof(in))
            break;
	if (!strncmp (line_in, "/*[", 3))
	{
	    sscanf (line_in+3, "%x", &flags1);
	    if (flags1 & flags)
		flag_delete++;
        }
	else if (!strncmp (line_in, "/*]", 3))
	{
	    sscanf (line_in+3, "%x", &flags1);
	    if (flags1 & flags)
		flag_delete--;
	}
        else if (flag_delete <= 0)
	{   
	    tab2space (line_in, line_out);
	    fprintf (out, "%s", line_out);
	}
    }
}





