#include <stdio.h>

int is_letter (int c)
{
    if (c >= 'A' && c <= 'Z')
	return 1;
    if (c >= 'a' && c <= 'z')
	return 1;
    if (c == '_' || c == '$')
	return 1;
    return 0;
}

int is_alphanum (int c)
{
    if (is_letter(c))
	return 1;
    if (c >= '0' && c <= '9')
	return 1;
    return 0;
}

convertl (FILE *in, FILE *out)
{
int c;
char buf[1000], buf1[1000];
char rec[300];
char field[200];
char ind[200];
int i, j, k;
    for (;;)
    {
	c = fgetc (in);
	if (feof(in) || c == EOF)
	    break;
        if (!is_letter (c))
	    fputc (c, out);
	else
	{
	    i = 0;
	    for (;;)
	    {
		if (is_alphanum(c))
		{		
		    rec[i++] = c;
		    c = fgetc (in);
		}
		else 
		{
		    rec[i] = 0;
		    strcpy (buf, rec);
		loop:
		    if (c != '-')
		    {
			fprintf (out, "%s%c", buf, c);
			break;
                    }
		    else
		    {
			c = fgetc (in);
			if (c != '>')
			{
			    fprintf (out, "%s-%c", buf, c);   
			    break;
			}
			else
			{
			    c = fgetc (in);
			    j = 0;
			    for (;;)
			    {
				if (is_alphanum(c))
				{
				    field[j++] = c;
				    c = fgetc (in);
				}
				else
				{
				    field[j] = 0; 
				    if (c != '[')
				    {
					sprintf (buf1, "%s(%s)", field, buf);
					strcpy (buf, buf1);
					goto loop;
				    }
				    else
				    {
					k = 0;
					for (;;)
					{
					    c = fgetc (in);
					    if (c != ']')
						ind[k++] = c;
					    else
					    {
						ind[k] = 0;
						sprintf (buf1, "%s(%s,%s)",
						    field, ind, buf);
						strcpy (buf, buf1);
						c = fgetc (in);
						goto loop;
					    }				    
					}				
				    }
				}				    
			    }
			}
		    }
		}	    		
	    }  
	}
    }
}

main ()
{
FILE *in, *out;
char in_filename[200], out_filename[200];

get_in:
    printf ("Input filename ? ");
    gets (in_filename);
    in = fopen (in_filename, "r");
    if (in == NULL)
    {
	perror (in_filename);
	goto get_in;
    }  

get_out:
    printf ("Output filename ? ");
    gets (out_filename);
    out = fopen (out_filename, "w");
    if (out == NULL)
    {
	perror (out_filename);
	goto get_out;
    }  

    convertl (in, out);
  
}





