<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<!-- saved from url=(0046)http://www.webb.net/sites/log/src/misc/yahou.c -->
<HTML><HEAD>
<META content="text/html; charset=windows-1252" http-equiv=Content-Type>
<META content="MSHTML 5.00.2014.210" name=GENERATOR></HEAD>
<BODY><XMP>#include <stdio.h>
 
#define DAT "yahou.dat"

#if 0
#define TRACE(format,variable) \
{ \
char format1[1000]; \
        sprintf (format1, "%s(%d) %s : %s.\n", \
                __FILE__, __LINE__, #variable, format); \
        printf (format1, variable); \
}
#else
#define TRACE(format,variable) 
#endif

upcase (char *s, char *t)
{
int i, l;
        l = strlen (s);
        for (i=0; i<l; i++)
        {
                if (s[i] >= 'a' && s[i] <= 'z')
                        t[i] = s[i] - 'a' + 'A';
                else
                        t[i] = s[i];
        }
        t[l] = 0;
}

int contient (char *s, char *t)
{
int i, ls, lt;
        ls = strlen (s);
        lt = strlen (t);
        TRACE ("%s", s);
        TRACE ("%s", t);
        TRACE ("%d", lt);
        TRACE ("%d", ls-lt);
        for (i=0; i<=ls-lt; i++)
        {
                /*
                TRACE ("%d", i);
                TRACE ("%s", s+i);
                */

                if (!strncmp (s+i, t, lt))
                        return i;
        }
        return -1;
}

int flag_eof;

out_str (char *s)
{
        printf ("%s", s);
}

in_str (char *s)
{
        gets (s);
}

char in_chr ()
{
        return getchar();
}

int eof_in ()
{
        return feof(stdin);
}

char line[5000];

get_li (char *buf)
{
        *buf = 0;
        /* strcat (buf, line); */
        /* TRACE ("boucle get_li", 0); */
        for (;;)
        {
                in_str (line);
                TRACE ("%s", line); 
                if (eof_in())
                        break;
                if (contient (line, "<li>") >= 0)
                {
                        strcat (buf, line);
                        break;
                }
                if (contient (line, "</html>") >= 0)
                {
                        flag_eof = 1;
                        break;
                }
                strcat (buf, line);
        }
}


struct key_info
{
        int coef;
        char text[30];
};

int n_key;
struct key_info key[120];

int coef_str (char *s)
{
int i, coef;
        coef = 0;
        for (i=0; i<n_key; i++)
        {
                if (contient (s, key[i].text) >= 0)
                        coef += key[i].coef;
        }
        return coef;
}

main (int argc, char *argv[])
{
FILE *dat, *in;
char line[1000];
char buf[5000];
char buf1[5000];
int threshold;
int i, l;
int coef;

        TRACE ("test %d", 10+4);

        line[0] = 0;

        dat = fopen (DAT, "r");
        if (dat == NULL)
        {
                perror (DAT);
                return;
        }

        fgets (line, sizeof(line), dat);
        threshold = atoi (line);

        n_key = 0;
        for (;;)
        {
                fgets (line, sizeof(line), dat);
                if (feof(dat))
                        break;
                l = strlen(line)-1;
                line[l] = 0;
                key[n_key].coef = atoi(line);
                for (i=0; i<l && line[i]!=' '; i++);
                strcpy (key[n_key].text, line+i+1);
                n_key++;
        }

        for (i=0; i<n_key; i++)
        {
                printf ("%d\t%s\n", key[i].coef, key[i].text);
        }

        /*
        in = fopen (argv[1], "r");
        if (in == NULL)
        {
                perror (argv[1]);        
                return;
        }
        */

        out_str ("<HTML><HEAD><TITLE>Selection from Yahoo news</TITLE>\n");
        out_str ("<BASE HREF=\"http://www.yahoo.com/Complete_Listing/all.html\">\n");
        out_str ("</HEAD>\n");
        out_str ("<BODY><H2>Selection from Yahoo News</H2>\n<ul>\n");

        flag_eof = 0;

        for (;;)
        {
                get_li (buf);
                if (eof_in ())
                        break;
                if (flag_eof)
                        break;
                TRACE ("%s", buf);
                upcase (buf, buf1);
                TRACE ("%s", buf1);
                coef = coef_str (buf1);
                TRACE ("%d", coef);
                if (coef >= threshold)
                {
                        sprintf (line, "\n\n\n<p>coef %d : \n", coef);
                        out_str (line);
                        out_str (buf);
                }

        }

        out_str ("</ul>\n</BODY></HTML>");

}

</XMP></BODY></HTML>
