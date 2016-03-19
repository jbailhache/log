
main (int argc, char *argv[])
{
int i;
int s;
	s = 0;
	for (i=0; i<strlen(argv[1]); i++)
		s += argv[1][i] - 0x40;
	printf ("%d\n", s);
}
