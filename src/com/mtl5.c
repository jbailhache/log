
#include "stdio.h"
#include "dos.h"
#include "stdlib.h"

union REGS reg;

int port;

#define in_mtl IN_MTL

/* extern void in_mtl(); */

in_mtl ()
{
	;
}

test_mtl ()
{
int config;
	for (config=0; config<=0xFF; config++)
	{
		reg.h.ah = 0;
		reg.h.al = config;
		reg.x.dx = 0;
		int86 (0x14, &reg, &reg);
		printf ("Config 0x%02X : etat 0x%04X\n", config, reg.x.ax);
/*		getchar(); */
		if ((out_car_mtl(7) & 0x80) == 0)
			printf ("statut out correct pour config 0x%X\n",
				config);
	}
}

test_ports ()
{
int adr;
    for (adr=0x3F8; adr<=0x3FF; adr++)
	printf(" %02X", adr&0xFF);
    printf("\n\n");
    while (!kbhit())
    {
	for (adr=0x3F8; adr<=0x3FF; adr++)
	{
		printf(" %02X",inp(adr));
	}
	printf("\n");
    }
    while (!kbhit())
	outp(0x3F8,random(0x100));
}

init_mtl()
{
	reg.h.ah = 0;
/*	reg.h.al = 0x82; */
/*
	printf ("Config ? "); scanf ("%x", &(reg.h.al));
	printf ("Port ? ") ; scanf ("%x", &port);
*/
	reg.h.al = 0x9A;
	port = 0;

	reg.x.dx = port;
	int86 (0x14, &reg, &reg);
	printf ("Etat : 0x%X\n", reg.x.ax);
	out_car_mtl_1(0);
}

int delai;

out_car_mtl (char c)
{
int parite, bit, i;
	parite = 1;
	for (bit=0; bit<=7; bit++)
		parite ^= ((c>>bit)&1);
	outp(0x3F8,c|(parite<<7));
	for (i=0; i<=delai; i++);
}

char in_car_mtl ()
{
char c;
int adr;
	while ((inp(0x3FD) & 8) == 0)
	{
	printf("0");
	for (adr=0x3F8; adr<=0x3FF; adr++)
	{
		printf(" %02X",inp(adr));
	}
	printf("\n");
	}
	/* c = inp(0x3F8); */
	while ((inp(0x3FD) & 8) != 0)
	{
	printf("1");
	for (adr=0x3F8; adr<=0x3FF; adr++)
	{
		printf(" %02X",inp(adr));
	}
	printf("\n");
	}
	c = inp(0x3F8);
	printf(" %X",c);
	return c;
}

out_car_mtl_1 (char c)
{
	printf ("Out 0x%X ",c);
	reg.h.ah = 1;
	reg.h.al = c;
	reg.x.dx = port;
	int86 (0x14, &reg, &reg);
	printf ("Statut out : 0x%X\n",reg.h.ah);
	return reg.h.ah;
}

char in_car_mtl_1 ()
{
	reg.h.ah = 2;
	reg.x.dx = port;
	int86 (0x14, &reg, &reg);
	printf ("Statut in : 0x%X\n", reg.x.ax);
	return reg.h.al;
}

out_ch_mtl (char *s)
{
	while (*s)
		out_car_mtl(*s++);
}

char in_ch_mtl (char *s)
{
char c;
	for (;;)
	{
		if (kbhit()) break;
		c = in_car_mtl();
		if (c == 19)
		{
			*s = 0;
			return in_car_mtl();
		}
		*s++ = c;
	}
}

convert (char *s, char *r)
{
	while (*s)
	{
		if (*s == '$')
		{
			*r++ = s[1]-'@' + 0x10 * (s[2] - '@');
			s += 3;
		}
		else
		{
			*r++ = *s++;
		}
	}
	*r = 0;
}

main ()
{
char r[100];
char c;
int i;
char buf[1000];
char buf_out[1000];

	delai = 1700;

	init_mtl ();

	for (;;)
	{
		printf ("? ");
		gets (buf);
		if (!*buf) return;
		convert (buf, buf_out);
		out_ch_mtl (buf_out);
	}

	/* test_mtl(); */
    for (;;)
    {
	printf ("Commande ? ");
	scanf("%s",r);
	switch (r[0])
	{
	case 'q' : exit(0);
	case 'T' : test_mtl(); break;
	case 't' :
	test_ports();
		break;
	case 'c' :
	init_mtl();
		break;
	case 'b' :
	out_ch_mtl("Bonjour !\r\n");
		break;
	case 'o' :
		scanf("%x",&c);
		outp(0x3F8,c);
		break;
	case 'O' :
		c=0x20;
		loop:
			outp(0x3F8,c);
			for (i=0; i<delai; i++);
			outp(0x3F8,c|0x80);
			for (i=0; i<delai; i++);
			c++;
			if (c<0x7F) goto loop;
		break;
	case 'r' :
            while (!kbhit())
		outp(0x3F8,random(0x100));
	    break;
	case 'i' :
	in_ch_mtl(r);
	printf("Vous avez tape : <%s>\n",r);
		break;
	case 'I' :
		in_mtl(r);
		printf ("Vous avez tape : %X %X\n", r[0], r[1]);
		break;
	case 'd' :
		scanf ("%d",&delai);
		break;
	default:
		printf("q t c b o i d\n");
	}
    }
}