
/* #define VMS */

#include <stdio.h>
#include <errno.h>

main (int argc, char *argv[])
{
FILE *f, *out;
char c, c1;
int i;
char filename[256], filename_out[256];
	if (argc > 1)
	{
	        f = fopen (argv[1], "rb");
	    if (f == NULL) 
	    {
                perror("fopen");
                printf("\"%s\"\n",argv[1]);
	    }
	}
	else
	{
	    printf ("Fichier ? ");
	    gets (filename);
#ifdef VMS
	    f = fopen (filename, "r", "ctx=bin");
#else
	    f = fopen (filename, "rb");
#endif
	    if (f == NULL)
	    {
		perror (filename);
		return (errno);
	    }
	}

#ifdef VMS
	printf ("Resultat ? ");
	gets (filename_out);
	out = fopen (filename_out, "w");
	if (out == NULL)
	{
	    perror (filename_out);
	    return errno;
	}
#endif 

        i=0;
loop:
        c = getc (f);
        if (feof(f))
        {
                fclose(f);
                return;
        }
#ifdef VMS
	putc (0x40+(c&0xF), out);
	putc (0x40+((c&0xF0)>>4), out);
#else
        putchar (0x40+(c&0xF));
        putchar (0x40+((c&0xF0)>>4)); 
#endif
        /*
        c1 = c & 0xF0;
        c1 = c1 >> 4;
        c1 += 0x40;
        putchar (c1);
        */
        i++;
        if ((i&0xF)==0) 
#ifdef VMS
	putc ('\n', out);
#else
	    printf("\n");
#endif
        goto loop;
}

