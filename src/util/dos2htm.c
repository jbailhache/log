#include <stdio.h>

main ()
{
unsigned int c;
	for (;;)
	{
		c = getchar ();
		if (c == EOF)
			break;
		switch ((char)c)
		{
#define cv(car,str) case car: printf ("%s", str); break;
			cv ('�', "&agrave;")
			cv ('�', "&acirc;")
			cv ('�', "&auml;")
			cv ('�', "&ccedil;")
			cv ('�', "&eacute;")
			cv ('�', "&egrave;")
			cv ('�', "&ecirc;")
			cv ('�', "&euml;")
			cv ('�', "&icirc;")
			cv ('�', "&iuml;")
			cv ('�', "&ocirc;")
			cv ('�', "&ouml;")
			cv ('�', "&ugrave;")
			cv ('�', "&ucirc;")
			cv ('�', "&uuml;")

			/* cv (10, "<br>") */

			cv (10, "\r\n");

			default: putchar (c);
		}
	}

}



