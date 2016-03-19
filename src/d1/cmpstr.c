
cmpstr (char *s1, char *s2)
{
int x, i1, i2, l1, l2, k;
	l1 = strlen (s1);
	l2 = strlen (s2);
	x = 0;
	for (i1=0; i1<l1; i1++)
	for (i2=0; i2<l2; i2++)
	{
		for (k=0; i1+k<l1 && i2+k<l2 && s1[i1+k]==s2[i2+k]; k++)
			x += k+1;
	}
	return x;
}

main (int argc, char *argv[])
{
int x, l1, l2;
float y, l;
	x = cmpstr (argv[1], argv[2]);
	l1 = strlen(argv[1]);
	l2 = strlen(argv[2]);
	l = (l1 + l2) / 2;
	y = x * 6 / (l * (l+1) * (l+2));
	printf ("%d %f %f\n", x, y, x*y);
}