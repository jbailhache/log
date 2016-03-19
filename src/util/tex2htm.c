
#include <stdio.h>

int c;
FILE *in, *out;

/* #define out stdout */

#define output(str) fprintf (out, "%s", str)

char *ptr = NULL;

int next_char ()
{
    if (ptr == NULL || *ptr == 0)
	return fgetc (in);
    /* printf ("0x%X\n", ptr); */
    return *ptr++;   
}

scan_word (char *buf)
{
int i;
		i = 0;
		for (;;)
		{
		    /* buf[i++] = c; */
		    buf[i++] = c;
		    c = next_char ();
		    if (! ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')))
			break; 
		}
		buf[i] = 0;
}

scan_expr ()
{
char buf[1000];
char buf_out[2000];
int i;    
int status;
        if (c == EOF)
	    return 0;
        switch (c)
	{
	    case '_':
		output ("<sub>");
		c = next_char ();
		scan_expr ();
		output ("</sub>");
		return 1;
	    case '^':
		output ("<sup>");
		c = next_char ();
		scan_expr ();
		output ("</sup>");
		return 1;
	    case '{':
		c = next_char ();
		for (;;)
		{
		    status = scan_expr ();
		    if (!status || c == '}')
			break; 
		}
		c = next_char ();
		return 1;
            case '\\':
		c = next_char ();
		scan_word (buf);
/*
		if (strcmp (buf, "alpha"))
		    fprintf (out, "<font face=\"symbol\">a</font>");
		else if (strcmp (buf, "beta"))
		    fprintf (out, "<font face=\"symbol\">b</font>");
		if (strcmp (buf, "gamma"))
		    fprintf (out, "<font face=\"symbol\">g</font>");
		else
*/
#define ds(name,str) \
		if (!strcmp (buf, name)) \
		{ \
		    sprintf (buf_out, "<font face=\"symbol\">%s</font>", str); \
		    output (buf_out); \
		} \
		else
#define da(name,str) \
		if (!strcmp (buf, name)) \
		{ \
		    output (str); \
		} \
		else
#define ign(name) \
		if (!strcmp (buf, name)); else

		ds ("alpha", "a")
		ds ("beta", "b")
		ds ("khi", "c")
		ds ("delta", "d")
		ds ("Delta", "D")
		ds ("epsilon", "e")
		ds ("phi", "f")
		ds ("Phi", "F")		
		ds ("gamma", "g")
		ds ("Gamma", "G")
		ds ("eta", "h")
		ds ("iota", "i")
		ds ("kappa", "k")
		ds ("lambda", "l")
		ds ("mu", "m")
		ds ("nu", "n")
		ds ("pi", "p")
		ds ("Pi", "P")
		ds ("theta", "q")
		ds ("Theta", "Q")
		ds ("rho", "r")
		ds ("sigma", "s")
		ds ("Sigma", "S")
		ds ("tau", "t")
		ds ("omega", "w")
		ds ("Omega", "W")
		ds ("xi", "x")
		ds ("psi", "y")
		ds ("Psi", "Y")
		ds ("zeta", "z")

		ds ("partial", "¶")
		ds ("int", "ò")
		ds ("sqrt", "Ö")
		ds ("Box", " ")
		ds ("nabla", "Ñ")    
		ds ("leftarrow", "¬")
		ds ("rightarrow", "®")
		ds ("infty", "¥")
		ds ("sum", "S")
		ds ("Sum", "S")

		da ("hat", "^")
		da ("\\", "<br>")
		/* da ("frac", "/") */

		da ("section", "<p>")
		da ("subsection", "<p>")
		da ("vskip", "<p>")
		da ("stackreldef", "[def]")

		ign ("begin")
		ign ("end")
		ign ("nonumber")
		ign ("displaystyle")
		ign ("limits")
		{	
		    sprintf (buf_out, "\\%s", buf);	
		    output (buf_out);
		}    
		return 1;

	    case '&':
		c = next_char ();
	    default:
		fputc (c, out);
		c = next_char ();
		return 1;
	}
    
}

scan_tex ()
{
int status;
    for (;;)
    {
	status = scan_expr ();
	if (!status)
	    break;
	if (c == '$' /* || c == '"' */ )
	    break;
    }
}

scan_html1 ()
{
char buf[1000];
char buf_out[1500];
char buf_com[2000];
int i, j;    
int status;
        switch (c)
	{
	    case '<':
		c = next_char ();
		if (c == '!')
		{
		    j = 0;
		    for (;;)
		    {
			buf_com[j++] = c;
			c = next_char ();
			if (c == '>' || c == EOF)
			    break;
		    }
		    /* buf_com[j++] = '$'; */
		    buf_com[j] = 0;
		    output ("<");
		    output (buf_com);
		    /* output (">"); */
		    return 1;
		}
		scan_word (buf);
		if (!strcmp (buf, "IMG") || !strcmp (buf, "img"))
		{
		    sprintf (buf_out, "<%s", buf);
		    for (;;)
		    {
			while (! ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')))
			{
			    sprintf (buf_out+strlen(buf_out), "%c", c);
			    if (c == '>')
			    {
				output (buf_out);
				return 1;			    
			    }
			    c = next_char ();
			}
			scan_word (buf);
			strcat (buf_out, buf);
			if (!strcmp (buf, "ALT") || !strcmp (buf, "alt"))
			{
			    j = 0;
			    while (c != /*'$'*/'"' && c != EOF)
			    {
				c = next_char ();
				if (c == '"')
				    j++;
				if (j > 1)
				{
				    output (buf_out);
				    return 1;
				}
			    }
			    c = next_char ();
			    if (c == '$')
				c = next_char ();
			    else
			    {
				for (;;)
				{
				    if (c == '"')
					break;
				    c = next_char ();
				}
				ptr = buf_com;
			    }
			    scan_tex ();
			    if (c == EOF)
				return 0;
			    while (c != '>' && c != EOF)
				c = next_char ();
			    output ("<!--");
			    output (buf_out+1);
			    output ("-->");
			    c = next_char ();
			    return 1;
			}
			else
			    printf ("%s not rec\n", buf);
		    }    
		}
		else    
		{	
		    /*
                    if (c == '>')
			sprintf (buf_out, "<%s", buf);
		    else
		        sprintf (buf_out, "<%s%c", buf, c);	
		    */
                    sprintf (buf_out, "<%s", buf);
                    output (buf_out);
		}    
		break;		
	    default:
		fputc (c, out);
		c = next_char ();
		if (c == EOF)
		    return 0;
	}
}

scan_html ()
{
    for (;;)
    { 
	if (c == EOF)
	    return 0;
	scan_html1();
    }
}

main ()
{
char in_filename[200], out_filename[200];

    printf ("Input file ? ");
    gets (in_filename);
    in = fopen (in_filename, "r");
    if (in == NULL)
    {
	perror (in_filename);
	return;
    }

    printf ("Output file ? ");
    gets (out_filename);
    out = fopen (out_filename, "w");
    if (out == NULL)
    {
	perror (out_filename);
	return;
    }

/*
    for (;;)
    {
	c = next_char ();
	if (c == EOF)
	    break;
	printf ("%c", c);
    }
*/

    c = next_char ();
    /* scan_html (); */
    scan_tex ();

    close (in);
    close (out);
}
