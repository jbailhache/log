
#include <graphics.h>

int color (int c)
{
	if (c < 8)
		return 0;
	else
		return 1;

}

v_putimage (unsigned char *s, int x, int y, int nx, int ny)
{
int x1, y1;
	for (x1=0; x1<nx; x1++)
	for (y1=0; y1<ny; y1++)
		putpixel (x+x1, y+y1, color (s[x1 + nx*y1]));
}

v_putsprite (unsigned char *s, int x, int y, int nx, int ny, int a)
{
int x1, y1;
	for (x1=0; x1<nx; x1++)
	for (y1=0; y1<ny; y1++)
		putpixel (x+x1, y+y1, color (s[x1 + nx*y1]));
}

v_addsprite (unsigned char *s, int x, int y, int nx, int ny, int a)
{
int x1, y1;
	for (x1=0; x1<nx; x1++)
	for (y1=0; y1<ny; y1++)
		if (s[x1+nx*y1] != 0)
			putpixel (x+x1, y+y1, color (s[x1 + nx*y1]));
}

v_cprintf (int e, int a, int b, int y, int x, int d, char *s)
{
	outtextxy (x, y, s);
}

v_setmode13  ()
{
int driver, mode;
int status;
char buf[1000];

	driver=HERCMONO;
	mode=HERCMONOHI;
/*
	printf ("Driver ? ");
	scanf ("%d", &driver);
	printf ("Mode ? ");
	scanf ("%d", &mode);
*/
	initgraph (&driver, &mode, "");
	status = graphresult ();
	if (status != grOk)
	{
		printf ("Erreur initgraph %d 0x%X\n", status, status);
		return;
	}

}

v_reset_mode ()
{
	restorecrtmode ();
}

v_box (int bord, int fond, int y0, int x0, int y1, int x1)
{
int x, y;
int poly[8];

	setcolor (bord);
	rectangle (x0, y0, x1, y1);
/*
	setfillstyle (SOLID_FILL, color(fond));
	poly[0] = x0; poly[1] = y0;
	poly[2] = x1; poly[3] = y0;
	poly[4] = x1; poly[5] = y1;
	poly[6] = x0; poly[7] = y1;
	fillpoly (4, poly);
*/
	for (x=x0+1; x<x1; x++)
	for (y=y0+1; y<y1; y++)
		putpixel (x, y, fond);
}

