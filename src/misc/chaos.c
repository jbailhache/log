<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<!-- saved from url=(0046)http://www.webb.net/sites/log/src/misc/chaos.c -->
<HTML><HEAD>
<META content="text/html; charset=windows-1252" http-equiv=Content-Type>
<META content="MSHTML 5.00.2014.210" name=GENERATOR></HEAD>
<BODY><XMP>#include "stdio.h"
#include "graphics.h"

chaos ()
{
float k;
float x;
int t;
	for (k=1; k<=20; k+=0.01)
	{
		x = 0.5;
		for (t=0; t<100; t++)
			x = k * x * (1 - x);
		for ( ; t<200; t++)
		{
			x = k * x * (1 - x);
			putpixel (k*100, x*200, 1);
		}
		
	}
}

main()
{
int driver, mode;
int status;
char buf[1000];
	driver=HERCMONO;
	mode=HERCMONOHI;
	initgraph (&driver, &mode, "");
	status = graphresult ();
	if (status != grOk)
		printf ("Erreur initgraph %d 0x%X\n", status, status);
	/* putpixel (120, 130, 1); */
	chaos ();
	gets (buf);
	restorecrtmode ();
}

</XMP></BODY></HTML>
