
#include <stdio.h>

main ()
{
char in_name[256], out_name[256];
FILE *in, *out;
char l, h;
    printf ("Fichier ? ");
    gets(in_name);
    printf ("Resultat ? ");
    gets (out_name);
#ifdef VMS
    in = fopen (in_name, "r", "ctx=bin");
    out = fopen (out_name, "w", "ctx=bin");
#else
    in = fopen (in_name, "rb");
    out = fopen (out_name, "wb");
#endif
loop:
    do l = getc(in);
    while (!feof(in) && (l < 0x40 || l > 0x4F));
    do h = getc(in);
    while (!feof(in) &&(h < 0x40 || h > 0x4F));
    if (feof(in))
    {
	fclose(in);
	fclose(out);
	return;
    }
    putc (l-0x40+0x10*(h-0x40), out);
    goto loop;
}

