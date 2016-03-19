/* Analyse en ondelettes */

/* A(w,t) = sqrt(2/pi) * w/t S(-int(sh),int(sh) (exp(-wx/T)2/2 f(t+x) sin wx dx */

#include <stdio.h>
#include <dos.h>
#include <graphics.h>
#include <math.h>

union REGS reg;

int num_lpt = 0;

lprint1 (char c)
{
	reg.h.ah = 0;
	reg.h.al = c;
	reg.x.dx = 1;
	int86 (0x17, &reg, &reg);
/*	printf ("statut = 0x%X\n", reg.h.ah); */
}

lprint (char *s)
{
	while (*s)
		lprint1 (*s++);
}

test ()
{
char text[1024];
char buf[100];
int m, n;
	printf ("Numero imprimante ? ");
	gets (buf);
	sscanf (buf, "%d", &num_lpt);
	printf ("Texte ? ");
	gets (text);
	lprint (text);
	lprint ("\r\n");
	printf ("Mode ? ");
	gets (buf);
	sscanf (buf, "%d", &m);
	printf ("Largeur ? ");
	gets (buf);
	sscanf (buf, "%d", &n);
	lprint1 (0x1B);
	lprint1 ('*');
	lprint1 (m);
	lprint1 (n & 0xFF);
	lprint1 (n >> 8);
	lprint (text);
}

prin (int argc, char *argv[])
{
FILE *f;
char c;
	lprint1 (0x1B);
	lprint1 ('R');
	lprint1 (0);
	f = fopen (argv[1], "r");
loop:
	c = getc(f);
	if (!feof(f))
	{
		if (c == '\n') lprint1('\r');
		lprint1(c);
		goto loop;
	}
	fclose (f);
	lprint ("\r\n");
}

#define LARGEUR 700 /*350*/
#define HAUTEUR 340 /*250*/

hardcopy ()
{
int lig, col, pix;
char c;
	lprint1 (0x1B); lprint1 ('3'); lprint1(216*2/18);
	for (lig=0; lig<HAUTEUR; lig += 8)
	{

		lprint1 (0x1B);
		lprint1 ('*');
		lprint1 (1);
		lprint1 (LARGEUR & 0xFF);
		lprint1 (LARGEUR >> 8);
                for (col=0; col<LARGEUR; col++)
		{
			for (pix=0; pix<8; pix++)
			{
				c <<= 1;
				c |= getpixel (col, lig+pix);
			}
			lprint1(c);
		}
		lprint ("\r\n");
	}
}

#if 0
main ()
{
int status;
int t;
float x, y;
int driver, mode;
	/* initgraph (HERCMONO, HERCMONOHI, ""); */
	/* initgraph (EGA, EGA64LO, ""); */
	driver = EGA;
	mode = EGA64LO;
	driver = HERCMONO;
	mode = HERCMONOHI;
	initgraph (&driver, &mode, "");
	status = graphresult();
	if (status != grOk)
		printf ("Erreur initgraph %d 0x%X\n", status, status);
	for (t=1; t<1000; t++)
	{
		x = sin(t);
		y = sin(t*1.5);
		putpixel (100+60*x, 80+40*y, 1);
	}
	hardcopy();
	getchar();
	restorecrtmode();
}
#endif

/* #include <stdio.h> */
#include <stdlib.h>
/* #include <math.h> */
/* #include <graph.h> */

#define PI 3.1415926

int n_bandes;
float seuil;
long seuilf;

int couleur (double x)
{
double y, z, d, a;
int i;
	if (n_bandes == 0)
	{
		/* if (abs(x) >= seuil) return 1; */
		if (x >= seuil || x <= -seuil)
			return 1;
		return 0;
	}
	y = (x + 1.0) / 2.0;
	z = y * n_bandes;
	i = (int)z;
	d = z - (double)i;
	a = (double)i / (double)n_bandes;
	if (d < a) return 1;
	return 0;
}

/* Nombre de bits apres la virgule */
#define PRECISION 8

#define N_EXP (3<<PRECISION)
#define N_SIN (1<<PRECISION)

long table_exp [N_EXP];
long table_sin [N_SIN];

double facteur;
long fix_2pi, fix_1_sur_2pi, fix_rac_2_sur_pi;

init_tables ()
{
long x;
	facteur = pow(2.0,(double)PRECISION);
	fix_2pi = (long)(2.0*PI*facteur);
	fix_1_sur_2pi = (long)((1.0/(2.0*PI))*facteur);
	fix_rac_2_sur_pi = (long)(sqrt(2.0/PI)*facteur);
	for (x=0; x<N_EXP; x++)
		table_exp[x] = (long)(exp(-((double)x)/facteur)*facteur);
	for (x=0; x<N_SIN; x++)
		table_sin[x] = (long)(sin(2*PI*((double)x)/facteur)*facteur);
}

long exp_fix (long x)
{
	if (x > 0 || x <= -N_EXP)
		return (long)(exp(((double)x)/facteur)*facteur);
	return table_exp[-x];
}

long sin_2pi_fix (long x)
{
	x &= (1<<PRECISION) - 1;
	if (x < 0) return -sin_2pi_fix(-x);
	return table_sin[x];
}

long cos_2pi_fix (long x)
{
	return sin_2pi_fix(x+(1<<(PRECISION-2)));
}

#define mult_fix(x,y) (((x) * (y)) >> PRECISION)
#define prod_fix(x,y) (((x) * (y)) >> PRECISION)
#define div_fix(x,y) (((x)*(1<<PRECISION))/y)
#define int_fix(x) ((x)>>PRECISION)

#define fix(x) ((long)((x)*facteur))
#define flo(x) (((double)(x))/facteur)

long sin_fix (long x)
{
/*	return sin_2pi_fix ((x * fix_1_sur_2pi) >> PRECISION); */
	return sin_2pi_fix (mult_fix(x,fix_1_sur_2pi));
}

long cos_fix (long x)
{
	return cos_2pi_fix (mult_fix(x,fix_1_sur_2pi));
}

long sqrt_fix (long x)
{
	return fix(sqrt(flo(x)));
}

int couleurf (long xf)
{
double y, z, d, a, x;
int i;

	if (n_bandes == 0)
	{
		/* if (abs(x) >= seuil) return 1; */
		if (xf >= seuilf || xf <= -seuilf)
			return 1;
		return 0;
	}
	x = flo(xf);
	y = (x + 1.0) / 2.0;
	z = y * n_bandes;
	i = (int)z;
	d = z - (double)i;
	a = (double)i / (double)n_bandes;
	if (d < a) return 1;
	return 0;
}

main ()
{
char buf[120];
FILE *voc_file;
int status;
int offset;
char c;
long l;
char SR, pac;
double freq, duree;
long freqf, dureef;
char buf_son[40000];
unsigned int t;
int dt, dt1;
int x;
float tau, h, w_min, w_max, freq_min, freq_max, unite;
long tauf, hf, w_minf, w_maxf, freq_minf, freq_maxf, unitef;
int nw, num_w, num_w1;
double w, rw, somme_sin, somme_cos, sigma, y, z, e, coef, coef_sin, coef_cos;
long wf, rwf, somme_sinf, somme_cosf, sigmaf, yf, zf, ef, coeff, coef_sinf,
	coef_cosf;
double coef_ampl;
long coef_amplf;
int couleur_sin, couleur_cos, couleur_ampl, abcisse;
int driver, mode;
unsigned int pos;
char mode_aff, methode, sincos;
int ecart;
float ampl;
long amplf, ampl1f;
int i;
float periode, nepp;
long periodef, neppf;
int isigmah;
long w_sur_tauf, w_sur_tau2f, vf;
long w2f, tau2x2f;
long lt, lr;
long t0;
int flag_suite;
int n_bloc;

	printf ("Fichier a analyser ? ");
	gets (buf);
	voc_file = fopen (buf, "rb");
	if (voc_file == NULL)
	{
		perror (buf);
		return errno;
	}
	status = fseek (voc_file, 0x14, SEEK_SET); /* offset des donnees */
	if (status)
	{
	err_fseek:
		printf ("Fichier VOC incorrect\n");
		perror ("fseek");
		return errno;
	}
	status = fread (&offset, 2, 1, voc_file);
	if (status != 1)
	{
	err_fread:
		printf ("Fichier VOC incorrect\n");
		perror ("fread");
		return errno;
	}
	status = fseek (voc_file, offset, SEEK_SET); /* donnees */
	if (status) goto err_fseek;
	t0 = 0;
	flag_suite = 0;
	printf ("Analyse a partir du bloc numero [1] ? ");
	gets (buf);
	if (!*buf)
		n_bloc = 1;
	else
		sscanf (buf, "%d", &n_bloc);

loop:   /* recherche bloc donnees */
	c = getc (voc_file);
	if (feof(voc_file))
	{
		goto fin;
		printf ("Fichier VOC incorrect\n");
		return -1;
	}
	if (c == 0)
	{
		goto fin;
		printf ("Fichier VOC vide\n");
		return 0;
	}
	if (c != 1)
	{
	/* on saute le bloc */
		l = 0;
		status = fread (&l, 3, 1, voc_file);
		if (status != 1) goto err_fread;
		status = fseek (voc_file, l, SEEK_CUR);
		if (status) goto err_fseek;
		goto loop;
	}
	/* bloc de donnees */
	if (n_bloc > 1)
	{
		n_bloc --;
		goto loop;
	}
	l = 0;
	status = fread (&l, 3, 1, voc_file);
	if (status != 1) goto err_fread;
	status = fread (&SR, 1, 1, voc_file);
	if (status != 1) goto err_fread;
	status = fread (&pac, 1, 1, voc_file);
	if (status != 1) goto err_fread;

	l -= 2; /* Nombre d'octets de l'echantillon */
	if (l <= 0)
	{
		goto fin;
		printf ("Fichier VOC vide\n");
		return 0;
	}

	freq = 1000000.0 / ((double)(0x100-(SR&0xFF)));
	unite = 1/freq;
	duree = (double)l / freq;

	if (!flag_suite)
	{
		printf ("Initialisation en cours...\n");
		init_tables ();
	}

	freqf = fix(freq);
	unitef = fix(unite);
	dureef = fix(duree);

	if (flag_suite)
	{
		pos = 0;
		goto suite;
	}

	/* donnees correctes */
	printf ("Donnees: %ld octets, SR=0x%X, compactage=%d, frequence=%f, duree=%f\n",
		l, SR, pac, (float)freq, (float)duree);

lire_pos:
	printf("Methode de calcul (simplifiee, optimisee, complete) ? "); gets(buf);
	methode = buf[0];
/*	printf("Sinus et cosinus separes ? "); gets(buf); sincos = buf[0];*/

	printf ("Position de depart [0] ? 0x"); gets(buf);
	if (!*buf)
		pos = 0;
	else
		sscanf(buf,"%x",&pos);
/*
	status = fseek (voc_file, pos, SEEK_CUR);
	if (status)
	{
		printf("Position incorrecte.\n");
		goto lire_pos;
	}
*/
	printf("Intervalle d'affichage [500] ? "); gets(buf);
	if (!*buf)
		dt = 500;
	else
		sscanf(buf,"%d",&dt);
	if (methode == 'c' || methode == 'o')
	{
		/*printf("Intervalle d'integration ? "); gets(buf); sscanf(buf,"%d",&dt1);*/
		printf("Nombre d'echantillonages par periode [4.0] ? ");
		gets(buf);
		if (!*buf) nepp = 4.0; else sscanf(buf,"%f",&nepp);
		/* printf("Duree de base [30] ? "); gets(buf); */
		printf ("Largeur des ondelettes [30.0] ? "); gets (buf);
		if (!*buf) tau=30.0; else sscanf(buf,"%f",&tau);
		printf("Etalement [3.0] ? "); gets(buf);
		if (!*buf) h=3.0; else sscanf(buf,"%f",&h);
		printf("Frequence minimum [1319.0] ? "); gets(buf);
		if (!*buf) freq_min = 1319.0;
			else sscanf(buf,"%f",&freq_min);
		printf("Frequence maximum [2489.0] ? "); gets(buf);
		if (!*buf) freq_max = 2489.0;
			else sscanf(buf,"%f",&freq_max);
	}
	if (methode == 'o')
	{
		neppf = fix(nepp);
		tauf = fix(tau);
		hf = fix(h);
		freq_minf = fix(freq_min);
		freq_maxf = fix(freq_max);
	}
	printf("Nombre de frequences [12] ? "); gets(buf);
	if (!*buf) nw=12; else sscanf(buf,"%d",&nw);
	printf("Mode d'affichage : plan, horizontal, vertical ? ");
		gets(buf); mode_aff = buf[0];
	if (mode_aff != 'h' && mode_aff != 'v')
	{
		printf("Nombre de bandes ? "); gets(buf); sscanf(buf,"%d",&n_bandes);
		printf("Seuil ? "); gets(buf); sscanf(buf, "%f", &seuil);
	}
	else
	{
		printf("Ecart [5] ? "); gets(buf);
		if (!*buf) ecart = 5; else sscanf(buf,"%d",&ecart);
		printf("Amplitude [50.0] ? "); gets(buf);
		if (!*buf) ampl = 50.0; else sscanf(buf,"%f",&ampl);
	}
	if (methode == 'o')
	{
		seuilf = fix(seuil);
		amplf = fix(ampl);
		ampl1f = 1<<PRECISION;
	}

	if (methode == 'c' || methode == 'o')
	{
		w_min = 2 * PI * freq_min / freq;
		w_max = 2 * PI * freq_max / freq;
		/* nw = 80; */
		rw = pow(w_max/w_min,1/(double)nw);
	}
	if (methode == 'o')
	{
		w_minf = fix(w_min);
		w_maxf = fix(w_max);
		rwf = fix(rw);
	}

	driver = HERCMONO;
	mode = HERCMONOHI;
	initgraph (&driver, &mode, "");
	status = graphresult ();
	if (status != grOk)
	{
		printf ("Erreur: %s\n", grapherrormsg(status));
		return status;
	}

suite:
	lt = l;
	lr = lt;
	if (l > sizeof(buf_son)) l=sizeof(buf_son);
loop_read:
	if (l < lr) l = lr;
	status = fread (buf_son, 1, l, voc_file);
	lr -= l;
	if (status != l) goto err_fread;

	/* Analyse */

	for (t=pos; t<l && !kbhit(); t+=dt) /* pour chaque instant */
	{
		/* for (w=w_min; w<=w_max && !kbhit(); w*=rw) */
		/* pour chaque frequence */
	    if /* (methode != 'c') */ (methode == 's')
	    {
		w_max = 1/(8*PI);
		for (num_w=0; num_w<nw && !kbhit(); num_w++)
		{
			num_w1 = nw - num_w;
			w = w_max / (double)num_w1;
			/* sigma = tau / w; */
		/* integrale */
#define son(t) (((t) >= 0 && (t) < l) ? buf_son[t] : 0)
			somme_sin = (-son(t-num_w1)+son(t+num_w1)) / 2;
			somme_cos = (-son(t-2*num_w1) + 2*son(t) - son(t+2*num_w1)) / 4;
			coef = 1.0 / 128.0;
			coef_sin = coef * somme_sin;
			coef_cos = coef * somme_cos;
			coef_ampl = sqrt (coef_sin * coef_sin + coef_cos * coef_cos);
			/* abcisse = (t/dt) % 700; */
			if (mode_aff == 'v')
			{
				abcisse = (((t0+t)/dt) * ecart) % 700;
				putpixel (abcisse+(int)(coef_sin*ampl), 100-num_w, 1);
				putpixel (abcisse+(int)(coef_cos*ampl), 200-num_w, 1);
				for (i=0; i<=(int)(coef_ampl*ampl); i++)
					putpixel (abcisse+i, 300-num_w, 1);
			}
			else if (mode_aff == 'h')
			{
				abcisse = ((t0+t)/dt) % 700;
				putpixel (abcisse, 100-num_w*ecart/*+*/-(int)(coef_sin*ampl), 1);
				putpixel (abcisse, 200-num_w*ecart-(int)(coef_cos*ampl), 1);
				for (i=0; i<=(int)(coef_ampl*ampl); i++)
					putpixel (abcisse, 300-num_w*ecart-i, 1);
			}
			else
			{
				couleur_sin = couleur(coef_sin);
				couleur_cos = couleur(coef_cos);
				couleur_ampl = couleur(coef_ampl);
				abcisse = ((t0+t)/dt) % 700;
				putpixel (abcisse, 100-num_w, couleur_sin);
				putpixel (abcisse, 200-num_w, couleur_cos);
				putpixel (abcisse, 300-num_w, couleur_ampl);
			}
		}
	    }
	    else if (methode == 'c')
	    {
		w = w_min;
		for (num_w=0; num_w<nw && !kbhit(); num_w++, w*=rw)
		{
			periode = 2 * PI / w;
			sigma = tau / w;
			somme_sin = 0;
			somme_cos = 0;
		/* integrale */
			dt1 = (int) (periode / nepp);
			if (dt1 == 0) dt1 = 1;
			for (x=-(int)(sigma*h); x<=(int)(sigma*h); x+=dt1)
			{
				y = w * x / tau;
				if (t+x >= 0 && t+x < l) z=(buf_son[t+x]&0xFF)-0x80;
				else z=0;
				e = exp(-y*y/2.0) * z;
				somme_sin += e * sin(w*x);
				somme_cos += e * cos(w*x);
			}
			coef = sqrt(2.0/PI) * w * dt1 / tau / 128.0;
			coef_sin = coef * somme_sin;
			coef_cos = coef * somme_cos;
			coef_ampl = sqrt (coef_sin * coef_sin + coef_cos * coef_cos);
			/* abcisse = (t/dt) % 700; */
			if (mode_aff == 'v')
			{
				abcisse = (((t0+t)/dt) * ecart) % 700;
				putpixel (abcisse+(int)(coef_sin*ampl), 100-num_w, 1);
				putpixel (abcisse+(int)(coef_cos*ampl), 200-num_w, 1);
				for (i=0; i<=(int)(coef_ampl*ampl); i++)
					putpixel (abcisse+i, 300-num_w, 1);
			}
			else if (mode_aff == 'h')
			{
				abcisse = ((t0+t)/dt) % 700;
				putpixel (abcisse, 100-num_w*ecart-(int)(coef_sin*ampl), 1);
				putpixel (abcisse, 200-num_w*ecart-(int)(coef_cos*ampl), 1);
				for (i=0; i<=(int)(coef_ampl*ampl); i++)
					putpixel (abcisse, 300-num_w*ecart-i, 1);
			}
			else
			{
				couleur_sin = couleur(coef_sin);
				couleur_cos = couleur(coef_cos);
				couleur_ampl = couleur(coef_ampl);
				abcisse = ((t0+t)/dt) % 700;
				putpixel (abcisse, 100-num_w, couleur_sin);
				putpixel (abcisse, 200-num_w, couleur_cos);
				putpixel (abcisse, 300-num_w, couleur_ampl);
			}
		}
	    }
	    else
	    {
		wf = w_minf;
		for (num_w=0; num_w<nw && !kbhit(); num_w++, /*w*=rw*/
						wf=mult_fix(wf,rwf))
		{
			periodef = div_fix(fix_2pi,wf); /* 2 * PI / w; */
			sigmaf = div_fix(tauf,wf); /* sigma = tau / w; */
			somme_sinf = 0;
			somme_cosf = 0;
		/* integrale */
			/* dt1 = (int) (periode / nepp); */
			dt1 = int_fix(div_fix(periodef,neppf)); /*>>PRECISION;*/
			if (dt1 == 0) dt1 = 1;

			isigmah = int_fix(prod_fix(sigmaf,hf));
			/*for (x=-(int)(sigma*h); x<=(int)(sigma*h); x+=dt1)*/
			/*
			w_sur_tauf = div_fix (wf, tauf);
			w_sur_tau2f = prod_fix(w_sur_tauf,w_sur_tauf);
			vf = - (w_sur_tau2f >> 1);
			*/
			w2f = prod_fix (wf, wf);
			tau2x2f = prod_fix (tauf, tauf) * 2;

			for (x=-isigmah; x<=isigmah; x+=dt1)
			{
			/* Boucle critique a optimiser au maximum */
				/* y = w * x / tau; */
				/* yf = w_sur_tauf * x; */
				if (t+x >= 0 && t+x < l) z=(buf_son[t+x]&0xFF)-0x80;
				else z=0;
				/* e = exp(-y*y/2.0) * z; */
				/* ef = exp_fix(vf * x * x); */
				ef = exp_fix (-div_fix (w2f * x * x, tau2x2f)) * z;
				somme_sinf += prod_fix (ef, sin_fix(wf*x));
				somme_cosf += prod_fix (ef, cos_fix(wf*x));
			}
			/* coef = sqrt(2.0/PI) * w * dt1 / tau / 128.0; */
			/* coeff = div_fix (prod_fix (fix_rac_2_sur_pi, wf) * dt1,
					tauf) >> 7; */
			coeff = prod_fix(prod_fix(fix_rac_2_sur_pi,wf)*dt1,amplf);

			/*
			coef_sin = coef * somme_sin;
			coef_cos = coef * somme_cos;
			*/
			coef_sinf = div_fix (prod_fix (coeff, somme_sinf), tauf) >>7;
			coef_cosf = div_fix (prod_fix (coeff, somme_cosf), tauf) >>7;
			/* coef_ampl = sqrt (coef_sin * coef_sin + coef_cos * coef_cos); */
			coef_amplf = sqrt_fix (prod_fix(coef_sinf,coef_sinf) +
						prod_fix(coef_cosf,coef_cosf));
			/* abcisse = (t/dt) % 700; */
			/* amplf = 1<<PRECISION; */
			if (mode_aff == 'v')
			{
				abcisse = (((t0+t)/dt) * ecart) % 700;
				putpixel (abcisse+int_fix(prod_fix(coef_sinf,ampl1f)), 100-num_w, 1);
				putpixel (abcisse+int_fix(prod_fix(coef_cosf,ampl1f)), 200-num_w, 1);
				for (i=0; i<=int_fix(prod_fix(coef_amplf,ampl1f)); i++)
					putpixel (abcisse+i, 300-num_w, 1);
			}
			else if (mode_aff == 'h')
			{
				abcisse = ((t0+t)/dt) % 700;
				putpixel (abcisse, 100-num_w*ecart-int_fix(prod_fix(coef_sinf,ampl1f)), 1);
				putpixel (abcisse, 200-num_w*ecart-int_fix(prod_fix(coef_cosf,ampl1f)), 1);
				for (i=0; i<=int_fix(prod_fix(coef_amplf,ampl1f)); i++)
					putpixel (abcisse, 300-num_w*ecart-i, 1);
			}
			else
			{
				couleur_sin = couleurf(coef_sinf);
				couleur_cos = couleurf(coef_cosf);
				couleur_ampl = couleurf(coef_amplf);
				abcisse = ((t0+t)/dt) % 700;
				putpixel (abcisse, 100-num_w, couleur_sin);
				putpixel (abcisse, 200-num_w, couleur_cos);
				putpixel (abcisse, 300-num_w, couleur_ampl);
			}
		}
	    }

	}

	if (lr > 0) goto loop_read;
	t0 = t;
	flag_suite = 1;
	goto loop;
fin:
/*	printf ("Imprimer ? "); scanf ("%s", buf); */
	buf[0] = getch();
	/* if (buf[0] != 'n' && buf[0] != 'N') hardcopy (); */
	if (buf[0] == 'H' || buf[0] == 'h')
		hardcopy();
	getchar();
	restorecrtmode ();
}



