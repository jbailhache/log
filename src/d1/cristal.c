
/* Figures de diffractions de cristaux */

#include "stdio.h"
#include "graphics.h"

#include "math.h"

#define real double

/* nombre d'atomes = ni*nj*nk */

#define ni 4
#define nj 3
#define nk 2

#define NLIN 350 	/* 300 */
#define NCOL 600        /* 500 */

#define Col0 500
#define Lin0 300

#define x_scale 100.0
#define y_scale -100.0

#define x_pixel(lin,col) (((real)((col)-Col0))/(x_scale)) /* 4.0 */
#define y_pixel(lin,col) (((real)((lin)-Lin0))/(y_scale))

#define col_x(x) ((Col0)+(x)*(x_scale))
#define lin_y(y) ((Lin0)+(y)*(y_scale))

/* cote du cristal */
#define ze 10.0

/* espacement des atomes */
#define dxyz 1.0

#define dx dxyz
#define dy dxyz
#define dz dxyz

/* position de la source */
#define xs 10.0
#define ys 8.0
#define zs 7.0

#define lambda 0.1

#define pi 3.1415926

real sqr (real x)
{
	return x * x;
}

#define distance(x1,y1,z1, x2,y2,z2) (sqrt(sqr((x2)-(x1))+sqr((y2)-(y1))+sqr((z2)-(z1))))

real fonction (real xe, real ye)
{
int i, j, k;
real x, y, z, d, d1, d2, phi, a;
	a = 0;
	for (i=0; i<ni; i++)
	for (j=0; j<nj; j++)
	for (k=0; k<nk; k++)
	{
	/* position de l'atome ijk */
		x = i * dx;
		y = j * dy;
		z = ze + k * dz;
	/* distance source-atome */
		d1 = distance (xs,ys,zs, x,y,z);
		/* d = sqrt (sqr(x-xe) + sqr(y-ye) + sqr(z)); */
	/* distance atome-ecran */
		d2 = distance (x,y,z, xe,ye,0);
		d = d1 + d2;
		phi = 2.0 * pi * d / lambda;
		a += sin(phi);
	}
	return a;
}

graph ()
{
int lin, col, bande;
real xe, ye, xe1, ye1, xe2, ye2, res;
int lin0, lin1, col0, col1;

/* trace de la projection du cristal */
	lin0 = lin_y (0);
	col0 = col_x (0);
	lin1 = lin_y ((nj-1)*dy);
	col1 = col_x ((ni-1)*dx);
	for (lin=lin0; lin>=lin1; lin--)
	{
		putpixel (col0, lin, 1);
		putpixel (col1, lin, 1);
	}
	for (col=col0; col<=col1; col++)
	{
		putpixel (col, lin0, 1);
		putpixel (col, lin1, 1);
	}
/*
	for (lin=0; lin<NLIN; lin++)
	for (col=0; col<NCOL; col++)
	{
		xe = x_pixel (lin, col);
		ye = y_pixel (lin, col);
		xe1 = x_pixel (lin, col+1);
		ye1 = y_pixel (lin, col+1);
		xe2 = x_pixel (lin-1, col);
		ye2 = y_pixel (lin-1, col);
		if ((xe * xe1 <= 0.0) || (xe * xe2 <= 0.0) ||
		    (ye * ye1 <= 0.0) || (ye * ye2 <= 0.0))
			putpixel (col, lin, 1);
	}
*/
/*
	for (lin=0; lin<NLIN; lin++)
	for (col=0; col<NCOL; col++)
*/
	for (bande=0; bande<NCOL; bande+=1)
	for (lin=0; lin<bande && lin<NLIN; lin+=1)
	{
		col = bande - lin;

	/* coordonnees du point de l'ecran */
		xe = x_pixel (lin, col);
		ye = y_pixel (lin, col);
	/* amplitude en ce point */
		res = fonction (xe, ye);
		if (fabs(res) >= (real)(ni*nj*nk)/12.0)
			putpixel (col, lin, 1);
		else
			putpixel (col, lin, 0);

	}

}

main()
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
	/*
	putpixel (120, 130, 1);
	{
	int i;
		for (i=0; i<150; i++)
		{
			putpixel (i, i, 1);
		}
	}
	*/
	graph ();
	gets (buf);
	sleep (1);
	restorecrtmode ();
	printf ("status = %d\n", status);
}

