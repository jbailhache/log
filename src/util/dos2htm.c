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
			cv ('Ö', "&agrave;")
			cv ('É', "&acirc;")
			cv ('Ñ', "&auml;")
			cv ('á', "&ccedil;")
			cv ('Ç', "&eacute;")
			cv ('ä', "&egrave;")
			cv ('à', "&ecirc;")
			cv ('â', "&euml;")
			cv ('å', "&icirc;")
			cv ('ã', "&iuml;")
			cv ('ì', "&ocirc;")
			cv ('î', "&ouml;")
			cv ('ó', "&ugrave;")
			cv ('ñ', "&ucirc;")
			cv ('Å', "&uuml;")

			/* cv (10, "<br>") */

			cv (10, "\r\n");

			default: putchar (c);
		}
	}

}



