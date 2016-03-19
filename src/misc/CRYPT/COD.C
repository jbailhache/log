
#include <stdio.h>

char password[] = "vaginale";

main ()
{
int i, c;
	printf ("char donnees[] = {\n\t");
	for (i=0; ; i++)
	{
		if (i % 10 == 0)
			printf ("\n\t");
		c = getchar ();
		if (c == EOF)
			break;
		printf (" %d,", c ^ password[i%(sizeof(password)-1)]);
	}
	printf (" 0 };\n");
}
