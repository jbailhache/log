
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <math.h>

long diviser (long x, long y)
{
	if (y == 0)
		return 0x7FFFFFFF;
	return x / y;
}

long carre (long x)
{
	return x * x;
}

long mod (long m, long n)
{
long r;
	r = n % m;
	if (r < 0)
		r += m;
	return r;
}

int hasard ()
{
int x;
	x = 0;
	for (;;)
	{
		if (random(6) == 0)
			return x;
		x++;
	}
}

#define NEW

#define TEST
#define TESTF
/* #define TESTC */

#ifndef TEST
/*
char codex[] = { 105, 2, 106, 4, 112, 15, 96, 3, 114, 1, 109, 72, 23, 51,
	108, 16, 90, 89, 81, 56, 111, 6, 94, 63, 2, 98, 61, 56, 103, 7,
	27, 101, 13, 94, 55 };
*/

char codex[] = { 48, 51, 16, 29, 48, 10, 6, 56, 64, 13, 14, 5, 41, 44, 50,
	15, 39, 60, 48, 10, 2, 29, 66, 52, 40, 47, 36, 16, 67, 23 };

/*
char codex[] = { 16, 45, 11, 19, 78, 29, 32, 28, 25, 74, 9, 3, 85, 84, 12,
	63, 69, 19, 4, 76, 45, 3, 28, 11, 60, 28, 25, 29, 70, 34, 80, 24, 84,
	31, 11, 67, 19, 66, 54, 55, 73, 12, 47, 53, 62, 36, 10, 14, 43, 13, 80,
	34, 84, 18, 36 };
*/
#else

char codex[] = "BONJOURCECIESTUNTESTDEDECODAGEAUREVOIR";

/* transopsition rectangulaire */
/* char codex[] = "BRSSCAIOCTTOURNEUDDRZJCNEAEZOITDGVZUEEEEOZ"; */

/* transposition diagonale 4x3 */
/* char codex[] = "BNUEOOCIJRCESUEDTTTDNSEECDEROGUVRAEOIR"; */

/* char codex[] = "BOOEUNCRJICESTTDEUESNDTECGOREDEAAVUOIR"; */

/* transposition cle TRESOR */
/* char codex[] = "NEUDDROITDGVOCTTOURUEEEEOJCNEAEBRSSCAI"; */

/* transposition inverse cle TRESOR */
/* char codex[] = "USBORTRTODCDEUNAEEUNJGCDOTOEIEIEUAECRS"; */

#endif

char code[] = "QWERTYUIOPASDFGHJKLZXCVBNM";

#ifdef TEST
#define LCODEX (sizeof(codex)-1)
#else
#define LCODEX (sizeof(codex))
#endif

char codex_t[LCODEX];

int buf[1000];

init_codex ()
{
int i;
char buf[1000];

#ifdef TEST
	for (i=0; i<LCODEX; i++)
	{
		codex[i] = codex[i] - 'A' + 1;
#ifdef TESTF
		codex[i] = codex[i] * 2;
#endif
	}
#ifdef TESTF
	for (i=0; i<LCODEX; i++)
		buf[i] = codex[LCODEX-i-1];

	for (i=0; i<LCODEX; i++)
		codex[i] = buf[i];
#endif
#ifdef TESTC
	for (i=0; i<LCODEX; i++)
		codex[i] = (code[codex[i]-1] - 'A' + 1) % 26;

#endif
#endif
};

typedef unsigned char compte;
typedef char caractere;

#define SEP 0

#define NCAR (26+SEP)

#define LNG 3

#define NS (NCAR*NCAR*NCAR)

#define MAXCOMPTE 250

compte frequence[NS];

int frequence1 [NCAR];

int frequence2 [NCAR] [NCAR];

caractere esartin [NCAR];

long x1;

#define LFORMULE 60

#define NV 2

#define V_FREQUENCE 0
#define V_REPET 1

struct item
{
	double distance;
	/* long val; */
	long v[NV];
	caractere s [LCODEX];
	caractere t [LCODEX];
	char f [LFORMULE];
};

#define LT 30
#define LM 20

int limite = 0;

struct item tableau[LT], meilleur[LM];

int ptr_debut = 0;

char debut[LCODEX];

int val_opt;

int flag_anafreq = 0;

int lng_model = 0;

long /* v_codex[NV],*/ v_opt[NV];
/* float distance_codex; */

struct item it_codex[1];

#define v_codex (it_codex->v)
#define distance_codex (it_codex->distance)

#define MAXKEYS 10

int n_keys = 0;

char key[MAXKEYS][LCODEX+1];

caractere lire1 (FILE *f)
{
char c;
	for (;;)
	{
		c = fgetc (f);
		if (c == EOF)
			return EOF;
		if (c >= 'A' && c <= 'Z')
			return c - 'A';
		if (c >= 'a' && c <= 'z')
			return c - 'a';
#if SEP
		return NCAR-1;
#endif
	}
}

caractere lire (FILE *f)
{
caractere c;
	c = lire1 (f);
	if (c != EOF)
		lng_model++;
	return c;
}

caractere lire_fichier (void *p)
{
	return lire (p);
}

struct chaine
{
	char *s;
	int n;
};

caractere lire_chaine (struct chaine *p)
{
caractere c;
	if (p->n <= 0)
		return EOF;
	c = p->s[0];
	p->s++;
	p->n--;
	return c;
}

trier (caractere esartin[], int frequence1[])
{
int i, j, k;
	for (i=0; i<NCAR; i++)
	{
		for (j=0; j<i; j++)
		{
			if (frequence1[i] > frequence1[esartin[j]] ||
				(frequence1[i] == frequence1[esartin[j]] &&
				 random(2)))
			{
				for (k=i-1; k>=j; k--)
					esartin[k+1] = esartin[k];
				esartin[j] = i;
				goto suite;
			}
		}
		esartin[i] = i;
	suite: ;
	}
}

calcul_frequence (FILE *f, compte frequence[],
	int frequence1[NCAR], int frequence2[NCAR][NCAR], caractere esartin[],
	caractere (*lir)(/* void * */), void *p)
{
char buf[LNG];
int i, j, k;
long x;
caractere c;
	x = 0;
	for (i=0; i<LNG; i++)
	{
		/* c = lire (f); */
		c = (*lir)(p);
		/* buf[i] = c; */
		x *= NCAR;
		x += c;
	}
	x1 = x;
	for (;;)
	{
		frequence[x]++;
		if (frequence[x] >= MAXCOMPTE)
			break;
		c = lire (f);
		if (ptr_debut < LCODEX)
			debut[ptr_debut++] = c;
		if (c == EOF)
			break;
		x %= NS/NCAR;
		x *= NCAR;
		x += c;
	}
	for (i=0; i<NCAR; i++)
	{
		frequence1 [i] = 0;
		for (j=0; j<NS/NCAR; j++)
			frequence1[i] += frequence [i*(NS/NCAR)+j];
	}

	trier (esartin, frequence1);
/*
	printf ("esartin = ");
	for (i=0; i<NCAR; i++)
		printf ("%c", 'A' + esartin[i]);
	printf ("\n");
*/
	for (i=0; i<NCAR; i++)
	for (j=0; j<NCAR; j++)
	{
		frequence2 [i] [j] = 0;
		for (k=0; k<NS/(NCAR*NCAR); k++)
			frequence2 [i] [j] += frequence [i*(NS/NCAR)+j*(NS/(NCAR*NCAR))+k];
	}

#ifdef TRACE
	printf ("   ");
	for (i=0; i<NCAR; i++)
		printf (" %c ", 'A'+i);
	printf ("\n\n");
	for (i=0; i<NCAR; i++)
	{
		printf (" %c ", 'A'+i);
		for (j=0; j<NCAR; j++)
			printf ("%2d", frequence2[i][j]/10);
		printf ("\n");
	}
	gets (buf);
#endif
}

generer1 (compte frequence[], long x)
{
int n, i, r;
	n = 0;
	for (i=0; i<NCAR; i++)
	{
		n += frequence[x*NCAR+i];
	}
	if (n == 0)
		return random (NCAR);
	r = random (n);
	n = 0;
	for (i=0; i<NCAR; i++)
	{
		n += frequence[x*NCAR+i];
		if (n > r)
			return i;
	}
}

generer (compte frequence[], int n)
{
long x;
caractere c;
int i;
	/* x = random (NS/NCAR); */
	x = x1 / NCAR;
	for (i=0; i<n; i++)
	{
		c = generer1 (frequence, x);
#if SEP
		if (c == NCAR-1)
			printf (" ");
		else
#endif
		printf ("%c", 'A'+c);

		x %= NS/(NCAR*NCAR);
		x *= NCAR;
		x += c;
	}
	printf ("\n");

}

double distance_v (long v1[], long v2[])
{
int i;
double val;
double q, l, p;
	val = 0.0;
	for (i=0; i<NV; i++)
	{
		if (v1[i] == 0)
			val += 10000000000.0;
		else if (v2[i] == 0)
			val += 10000000000.0;
		else if (v1[i] == v2[i]) ;
		else
		{
			q = (double)(v1[i]) / (double)(v2[i]);
			/* printf ("q=%lf\n", q); */
			l = log (q);
			/* printf ("l=%lf\n", l); */
			p = pow (l, 2.0);
			/* printf ("p=%lf\n", p); */
			/* printf ("%lf %lf %lf\n", q, l, p); */
			val += p;
			/* val += pow (log (v1[i] / v2[i]), 2.0); */
		}
	}
	return val;
}

valeur_frequence_1 (char *s, int l)
{
int i;
long x, val;
	/* l = strlen (s); */
	x = 0;
	for (i=0; i<LNG; i++)
	{
		x *= NCAR;
		x += s[i];
	}
	/* x = s[0] * NCAR + s[1]; */
	val = 0;
	for (i=LNG; i<=l; i++)
	{
		val += frequence[x];
		x %= NS / NCAR;
		x *= NCAR;
		x += s[i];
	}
	return val;
}

long valeur_frequence_n (char *s, int l)
{
struct chaine ch;
compte freq[NS];
int freq1 [NCAR];
int freq2 [NCAR] [NCAR];
caractere esarti [NCAR];
int i;
long v;

	ch.s = s;
	ch.n = l;

	calcul_frequence (NULL, freq, freq1, freq2, esarti, lire_chaine, &ch);

	v = 0;

#if 0
	for (i=0; i<NS; i++)
	{
		/* v -= carre (frequence[i] * (LCODEX-2) - freq[i] * (lng_model-2)); */
		v -= carre (frequence[i] * (LCODEX-2) / (lng_model-2) -
			    freq[i] );
	}
#else

	for (i=0; i<NCAR; i++)
	{
		/* v -= frequence11[i] * LCODEX - freq1[i] * lng_model; */
		v -= carre (frequence1[i] * LCODEX / lng_model - freq1[i]);
	}
#endif
	v += 80 * valeur_frequence_1 (s, l);
	return v;
}

long valeur_frequence (char *s, int l)
{
long v;
int n, i, j;

	v = valeur_frequence_1 (s, l);
	n = 0;
	for (i=0; i<NCAR; i++)
	{
		for (j=0; j<LCODEX; j++)
		{
			if (s[j] == i)
			{
				n++;
				break;
			}
		}
	}
	return v * n;
}

enum operation { op_plus, op_moins, op_fois, op_div, op_mod,
	op_and, op_or, op_xor, op_arg1, op_not,
	op_zero, op_un, op_deux, op_moinsun,
	op_id,
	op_der, op_int, op_codex, op_plus1, op_moins1, op_double, op_demi,
	op_rev, op_decal, op_ind, op_mix, op_transpos, op_diag, op_transpos_cle,
#ifdef OP_ANAFREQ
					op_anafreq,
#endif
						op_fin };

int arite [] = {  2,   2,   2,   2,   2,   2,   2,   2,   1,   1,
	 0,   0,   0,   0,   0,   1,   1,   0,   1,   1,   1,   1,
	 1,   2,   2,   2,   1,   1,   1
#ifdef OP_ANAFREQ
			,   1
#endif
				};

char codop[] = { '+', '-', '*', '/', '%', '&', '|', 'e', '.', '~',
	'0', '1', '2', 'f', 'n', 'd', 's', 'c', 'p', 'm', 'P', 'M',
	'r', '>', ':', '@', 't', '\\', 'K'
#ifdef OP_ANAFREQ
			  , '?'
#endif
				};

char *codops[] = { "+", "-", "*", "/", "%", "&", "|", " XOR ", ".", "~",
	"0", "1", "2", "F", "N", "d", "s", "c", "(1+)", "(-1+)" , "2*", "1/2*",
	"r", ">", ":", "@", "t", "\\", "K"
#ifdef OP_ANAFREQ
			, "?"
#endif
				};

#define NBROP (sizeof(codop)/sizeof(codop[0]))

#if (sizeof(arite)/sizeof(arite[0]) != NBROP) || op_fin != NBROP
#error Erreur dans la definition des operateurs
#endif

#ifdef REEL
typedef double nombre;
#else
typedef long nombre;
#endif

anafreq_ordre (char *s, char *u, int ordre)
{
int i, j, i1, j1, k, l;
char s1[LCODEX];
int freq1[NCAR];
char esarti[NCAR];
int freq2[NCAR][NCAR];
/*long*/ nombre corresp [NCAR] [NCAR];
char tri_corresp[NCAR*NCAR][2];
char transcod [2][NCAR];
nombre frequence_rel, freq_rel;

		/*
		for (i=0; i<LCODEX; i++)
			u[i] = s[i];
		return;
		*/

		for (i=0; i<LCODEX; i++)
		{
			s1[i] = (s[i]-1) % NCAR;
			if (s1[i] < 0)
				s1[i] += NCAR;
		}
		for (i=0; i<NCAR; i++)
			freq1[i] = 0;
		for (i=0; i<LCODEX; i++)
			freq1[s1[i]]++;

		trier (esarti, freq1);

		if (ordre == 1)

		for (i=0; i<LCODEX; i++)
		{
			u[i] = s1[i] + 1;
			for (j=0; j<NCAR; j++)
			{
				if (esarti[j] == s1[i])
				{
					u[i] = esartin[j] + 1;
				}
			}
		}

		else
		{
		for (i=0; i<NCAR; i++)
		for (j=0; j<NCAR; j++)
			freq2[i][j] = 0;
		for (i=0; i<LCODEX-1; i++)
			freq2[s1[i]][s1[i+1]]++;

		for (i=0; i<NCAR; i++)
		for (j=0; j<NCAR; j++)
			corresp[i][j] = 0;

		for (i=0; i<NCAR; i++)
		for (j=0; j<NCAR; j++)
		{
/*
			corresp[i][j] += frequence1[i] * LCODEX -
					 freq1[j] * lng_model;
*/
#ifdef REEL
			frequence_rel = (double) frequence1[i] / (double) lng_model;
			freq_rel = (double) freq1[j] / (double) LCODEX;
#else
			frequence_rel = diviser (frequence1[i] * LCODEX, lng_model);
			freq_rel = freq1[j];
#endif
			corresp[i][j] -= diviser (frequence_rel * freq_rel,
				(1 + carre (frequence_rel - freq_rel)));

			for (i1=0; i1<NCAR; i1++)
			for (j1=0; j1<NCAR; j1++)
#if 1
			{
#ifdef REEL
				frequence_rel = (double) frequence2[i][i1] /
					(double) (lng_model-1);
				freq_rel = (double) freq2[j][j1] /
					(double) (LCODEX-1);
#else
				frequence_rel = diviser (frequence2[i][i1] * (LCODEX-1), (lng_model-1));
				freq_rel = freq2[j][j1];
#endif
				corresp[i][j] -= diviser (frequence_rel * freq_rel,
					(1 + carre (frequence_rel - freq_rel)));

			}
#define CH 30
#ifdef REEL
			corresp[i][j] *= exp ((hasard() - hasard()) / 20.0);
#else
			corresp[i][j] = diviser (corresp[i][j] * (CH + hasard() - hasard()), CH);
#endif
/*
				corresp[i][j] += (
					frequence2[i][i1] * (LCODEX-1) -
					freq2[j][j1] * (lng_model-1) );
*/
#else
				corresp[i][j] += carre (
					frequence2[i][i1] * (LCODEX-1) / (lng_model-1) -
					freq2[j][j1] );
#endif
		}

		for (i=0; i<NCAR; i++)
		for (j=0; j<NCAR; j++)
		{
			for (k=0; k<i*NCAR+j; k++)
			{
				if (corresp[i][j] < corresp[tri_corresp[k][0]][tri_corresp[k][1]] ||
				   (corresp[i][j] ==corresp[tri_corresp[k][0]][tri_corresp[k][1]] && random(2)))
				{
					for (l=i*NCAR+j-1; l>=k; l--)
					{
						tri_corresp[l+1][0] = tri_corresp[l][0];
						tri_corresp[l+1][1] = tri_corresp[l][1];
					}
					tri_corresp[k][0] = i;
					tri_corresp[k][1] = j;
					goto suite;

				}
			}
			tri_corresp[i*NCAR+j][0] = i;
			tri_corresp[i*NCAR+j][1] = j;
		suite: ;
		}

		for (i=0; i<2; i++)
		for (j=0; j<NCAR; j++)
			transcod[i][j] = -1;

		for (k=0, l=0; l<NCAR; k++)
		{
			if (transcod[0][tri_corresp[k][0]] == -1 &&
			    transcod[1][tri_corresp[k][1]] == -1)
			{
				transcod[0][tri_corresp[k][0]] = tri_corresp[k][1];
				transcod[1][tri_corresp[k][1]] = tri_corresp[k][0];
				l++;
			}
		}

		for (i=0; i<LCODEX; i++)
		{
			u[i] = transcod[1][s1[i]];
		}
		}
}

anafreq (char *s, char *u)
{
	return anafreq_ordre (s, u, 1);
}


/*
#include "stdlib.h"

char codex[] = "ABBDHORCJAETWYDPLXCDGFPKBTIDYGAWMPC";
int l;
*/

char cle[0x100];

decodage_manuel (char *cod, char *dec)
{
int i, n, l;
char buf[200];
	/* l = strlen (codex); */
	l = LCODEX;
	for (i=0; i<0x100; i++)
		cle[i] = '.' - 'A';
	for (i=0; i<l; i++)
		cle[cod[i]] = dec[i];
	for (;;)
	{
		for (i=0; i<l; i++)
			printf ("%c", 'A' + cod[i]);
		printf ("\n");
		for (i=0; i<l; i++)
			printf ("%c", 'A' + cle[cod[i]]);
		printf ("\n? ");
		gets (buf);
		if (*buf == '.')
		{
			for (i=0; i<0x100; i++)
				cle[i] = '.' - 'A';
			continue;
		}
		if (*buf == '*' || !*buf)
		{
			for (i=0; i<0x100; i++)
			{
				if (cle[i] == '.' - 'A' || buf[1] == '*' || !*buf)
					cle[i] = "ESARTINULOCMPD" [random(14)];
			}
			continue;
		}
		if (!strcmp (buf, "QUIT") || !strcmp (buf, "quit"))
			break;
		n = 0;
		for (i=0; i<0x100; i++)
			if (cle[i] == buf[1]-'A')
			{
				n++;
				printf ("*");
			}
		if (n)
			printf ("\n");
		cle[buf[0]-'A'] = buf[1]-'A';


	}
}

test_anafreq ()
{
char buf[LCODEX];
char buf1[200];
int i;
    for (;;)
    {
	anafreq_ordre (codex, buf, 2);
	for (i=0; i<LCODEX; i++)
		printf ("%c", 'A' + buf[i]);
	printf (" ? ");
	gets (buf1);
	if (*buf1 == 'Q' || *buf1 == 'q')
		break;
	if (*buf1 == 'M' || *buf1 == 'm')
	{
		printf ("Tapez une lettre du message code suivie de sa traduction\n");
		printf ("Tapez quit pour terminer\n");
		decodage_manuel (buf, buf);
		printf ("Decodage en cours...\n");
	}

    }
}

#define VIDE 0x100

diag (char *u, char *s, int x, int y, int flags)
{
int l, c, i, j, k;
    i = 0;
    k = 0;
    for (;;)
    {
	l = 0;
	c = 0;
	for (j=0; j<x*y; j++)
		buf[j] = VIDE;
	for (;;)
	{
		if (l < y && c < x)
		{
			buf[l*x+c] = s[i++];
			if (i >= LCODEX)
				break;
		}
		if (l == 0)
		{
			l = c+1;
			if (l > x + y)
				break;
			c = 0;
		}
		else
		{
			l--;
			c++;
		}
	}

	for (l=0; l<y; l++)
	for (c=0; c<x; c++)
	if (buf[l*x+c] != VIDE)
	{
		u[k++] = buf[l*x+c];
		if (k >= LCODEX)
			return;
	}
    }


}

det_cle_num (int *cle_num, int *cle_inv, char *cle)
{
int c;
int i, j, l;
	l = strlen (cle);
	j = 0;
	for (c=0; c<0x100; c++)
	{
		for (i=0; i<l; i++)
		{
			if (cle[i] == c)
			{
				cle_num[i] = j;
				cle_inv[j] = i;
				j++;
			}
		}
	}
}

transpos_cle (char *u, char *s, char *cle, int flag)
{
int cle_num[LCODEX];
int cle_inv[LCODEX];
int i, j, k, l, n;
	l = strlen (cle);
	det_cle_num (cle_num, cle_inv, cle);
	k = 0;
	for (i=0; i<l; i++)
	for (j=0; (n=j*l+cle_inv[i]) < LCODEX; j++)
	{
		if (flag & 1)
			u[k++] = s[n];
		else
			u[n] = s[k++];
	}
	/*
	for (i=0; i<l; i++)
	for (j=0; j*l<LCODEX; j++)
	{
		k = i+j*l;
		u[cle_inv[i]+j*l] = s[k];

	}
	*/
}

operation (int op, char *s, char *t, char *u, int x, int y, int z)
{
int i, j;
int total;
/*
char s1[LCODEX];
int freq1[NCAR];
char esarti[NCAR];
*/
	total = 0;

#ifdef OP_ANAFREQ
	if (op == op_anafreq)
	{
		anafreq_ordre (s, u, 2);
	}
	else
#endif

	for (i=0; i<LCODEX; i++)
	{
		total += s[i];
		switch (op)
		{
			case op_plus :
				u[i] = s[i] + t[i];
				break;
			case op_moins :
				u[i] = s[i] - t[i];
				break;
			case op_fois :
				u[i] = s[i] * t[i];
				break;
			case op_div :
				if (t[i] != 0)
					u[i] = s[i] / t[i];
				else
					u[i] = 0;
				break;
			case op_mod :
				if (t[i] != 0)
					u[i] = s[i] % t[i];
				else
					u[i] = 0;
				break;
			case op_and :
				u[i] = s[i] & t[i];
				break;
			case op_or :
				u[i] = s[i] | t[i];
				break;
			case op_xor :
				u[i] = s[i] ^ t[i];
				break;
			case op_arg1 :
				u[i] = s[i];
				break;
			case op_not :
				u[i] = ~s[i];
				break;
			case op_zero :
				u[i] = 0;
				break;
			case op_un :
				u[i] = 1;
				break;
			case op_deux :
				u[i] = 2;
				break;
			case op_moinsun :
				u[i] = -1;
				break;
			case op_id :
				u[i] = i;
				break;
			case op_der :
				if (i < LCODEX-1)
					u[i] = s[i+1] - s[i];
				else
					u[i] = s[0] - s[i];
				break;
			case op_int :
				u[i] = total;
				break;
			case op_codex :
				u[i] = codex[i];
				break;
			case op_plus1 :
				u[i] = s[i] + 1;
				break;
			case op_moins1 :
				u[i] = s[i] - 1;
				break;
			case op_double :
				u[i] = s[i] * 2;
				break;
			case op_demi :
				u[i] = s[i] / 2;
				break;
			case op_rev :
				u[i] = s[LCODEX-i-1];
				break;
			case op_decal :
				u[i] = s[(i+t[i])%LCODEX];
				break;
			case op_ind :
				u[i] = s[t[i]%LCODEX];
				break;
			case op_mix :
				u[i] = (i%2==0) ? s[i] : t[i];
				break;
			case op_transpos :
				/* u[i] = s[mod(LCODEX,i*mod(LCODEX,x))]; */
				u[i] = s[mod(LCODEX,i*(x+2))];
				break;
			case op_diag :
				diag (u, s, x+2, y+2, z);
				break;
			case op_transpos_cle :
				if (n_keys > 0)
				{
					transpos_cle (u, s, key[x%n_keys], z);
					return;
				}
				else
					u[i] = s[i];
				break;

			default :
				printf ("operation %d non definie\n", op);
				u[i] = 0;

		}
	}

}

long val_codex;

#define NUMA 1

#define N_CAR NCAR

long valeur (char *s)
{
char s1[LCODEX+1];
int i;
long val;
	for (i=0; i<LCODEX; i++)
	{
		s1[i] = (s[i]-NUMA) % N_CAR;
		if (s1[i] < 0)
			s1[i] += N_CAR;
	}
	s1[LCODEX] = 0;
	val = valeur_frequence (s1, LCODEX);
	return val;
}

long v_repet (char *s)
{
long v;
int p, i, j;
	v = 0;
#if 0
	for (p=1; p<LCODEX/2; p++)
	{
		for (i=0; i+2*p<LCODEX; i++)
			/* if (s[i] == s[i+p] && s[i] == s[i+2*p]) */
			if (s[i+p] - s[i] == s[i+2*p] - s[i+p])
				v++;

	}
#else
	for (p=1; p<LCODEX; p++)
	{
		for (i=0; i+p+1<LCODEX; i++)
			for (j=1; i+p+j<LCODEX; j++)
				if (s[i+j] - s[i] == s[i+p+j] - s[i+p])
					v++;
	}
#endif
	return v;
}

int n_transpos = 1;

calcul_v (struct item *it, char *s)
{
	it->v[V_FREQUENCE] = valeur (s);
	it->v[V_REPET] = v_repet (s) + 1;
	it->distance = distance_v (it->v, v_opt);
}

set_codex (struct item *it)
{
int i, j, k, lk;
char buf[1000];
#if 0
		memcpy (it->s, codex, LCODEX);
		/* memcpy (it->t, codex_t, LCODEX); */
		strcpy (it->f, "C");
#else
/*
		n_transpos++;
		if (n_transpos >= LCODEX)
			n_transpos = 2;
*/
		if (n_keys > 0 && random(2))
		{
			i = random (n_keys);
			lk = strlen (key[i]);
			for (j=0; j<LCODEX; j++)
				it->s[j] = key [i] [mod(lk,j)] - 'A' + 1;
			sprintf (it->f, "K%d", i);
			anafreq (it->s, it->t);
			calcul_v (it, it->t);
			return;
		}
		n_transpos = hasard ();
		if (random(2))
			n_transpos = 1;
		for (j=0; j<LCODEX; j++)
			it->s[j] = codex[mod(LCODEX,j*n_transpos)];
		sprintf (buf, "C%d", n_transpos);
		strcpy (it->f, buf);
#endif
		/* it->val = val_codex; */
		anafreq (it->s, it->t);
		if (flag_anafreq)
		{
			calcul_v (it, it->t);
			/*
			it->v[V_FREQUENCE] = valeur (it->t);
			it->v[V_REPET] = v_repet (it->t);
			*/
		}
		else
		{
			for (k=0; k<NV; k++)
				it->v[k] = v_codex[k];
			it->distance = distance_codex;
			/*
			it->v[V_FREQUENCE] = v_frequence_codex;
			it->v[V_REPET] = v_repet_codex;

			it->val = val_codex;
			*/
		}

}

init_tableau ()
{
int i;

	anafreq (codex, codex_t);

	if (flag_anafreq)
	{
		calcul_v (it_codex, codex_t);
		/*
		v_codex[V_FREQUENCE] = valeur (codex_t);
		v_codex[V_REPET] = v_repet (codex_t);
		distance_codex = distance_v (v_codex, v_opt);
		*/
	}
	else
	{
		calcul_v (it_codex, codex);
		/*
		v_codex[V_FREQUENCE] = valeur (codex);
		v_codex[V_REPET] = v_repet (codex);
		distance_codex = distance_v (v_codex, v_opt);
		*/
	}

	for (i=0; i<LT; i++)
	{
		set_codex (tableau+i);
		/*
		memcpy (tableau[i].s, codex, LCODEX);
		memcpy (tableau[i].t, codex_t, LCODEX);
		strcpy (tableau[i].f, "C");
		tableau[i].val = val_codex;
		*/

	}
	limite = 1;

	for (i=0; i<LM; i++)
	{
		set_codex (meilleur+i);
		/* u[i] = s[mod(LCODEX,i*(x+2))]; */

		/*
		memcpy (meilleur[i].s, codex, LCODEX);
		memcpy (meilleur[i].t, codex_t, LCODEX);
		strcpy (meilleur[i].f, "C");
		meilleur[i].val = val_codex;
		*/
		/*
		for (j=0; j<LCODEX; j++)
			meilleur[i].s[j] = codex[mod(LCODEX,j*i)];
		*/


	}
}

int inserer (struct item *it)
{
int i, j, l;
	l = strlen (it->f);
	for (i=0; i<l; i++)
	{
		if (it->f[i] == 'C' ||
		    it->f[i] == 'c' /*||
		    it->f[i] == 'k'*/)
			goto suite;
	}
	return  LM+1;
suite:
	for (i=0; i<LM; i++)
	{
		if (!memcmp (it->s, meilleur[i].s, LCODEX))
			return LM+1;
		/* if (it->val > meilleur[i].val) */
		/* if (abs(it->val - val_opt) < abs (meilleur[i].val - val_opt)) */
		/* if (abs (it->v[V_FREQUENCE] - v_opt[V_FREQUENCE]) <
			abs (meilleur[i].v[V_FREQUENCE] - v_opt[V_FREQUENCE])) */
		if (it->distance < meilleur[i].distance)
		{
		    /*if (memcmp (it->s, meilleur[i].s, LCODEX))
		    {*/
			for (j=LM-2; j>=i; j--)
				memcpy (meilleur+j+1, meilleur+j, sizeof(struct item));
			memcpy (meilleur+i, it, sizeof(struct item));
			return i;
		    /*}*/
		}
	}
	return LM+1;
}

#define NMA 6

int nma = NMA;

afficher_s (char *s)
{
int i;
char s1[LCODEX];
char c;

#if NMA > 6
	if (!flag_anafreq)
#endif
	for (i=0; i<LCODEX; i++)
	{
		c = 'A' + mod (N_CAR, s[i]-NUMA);
		if (c > 'Z')
			c = '.';
		printf ("%c", c /*'A' + (((s[i]-NUMA)%N_CAR)+N_CAR)%N_CAR*/);
	}
#if NMA <= 6
	printf ("\n");
#else
	/* printf (" "); */
#endif
	anafreq (s, s1);
#if NMA > 6
	if (flag_anafreq)
#endif
	for (i=0; i<LCODEX; i++)
	{
		c = 'A' + mod (N_CAR, s1[i]-NUMA);
		if (c > 'Z')
			c = '.';
		printf ("%c", c /*'A' + (((s1[i]-NUMA)%N_CAR)+N_CAR)%N_CAR*/);
	}
}

afficher_meilleurs ()
{
int i;
char buf[200];

	/* clrscr (); */
	gotoxy (1, 1);

	for (i=0; i<nma; i++)
	{
		/*printf ("\n%3d %4ld %4ld %4ld %s\n",
			i, meilleur[i].val,
			0L, 0L,*/
			/*
			valeur (meilleur[i].s),
			valeur (meilleur[i].t),*/
			/* meilleur[i].f); */
#if NMA <= 6
		printf ("\n%3d %lf %ld %ld %s",
			i,
			meilleur[i].distance,
			meilleur[i].v[0],
			meilleur[i].v[1],
			meilleur[i].f);
		clreol ();
		printf ("\n");
#else
		printf ("%2d %5.2lf %4ld %4ld %-16.16s ",
			i,
			meilleur[i].distance,
			meilleur[i].v[0],
			meilleur[i].v[1],
			meilleur[i].f);
#endif
		afficher_s (meilleur[i].s);
		printf ("   \n");
	}
	/*
	gets (buf);
	if (*buf == 'q')
		exit (0);
	*/
}

int evol_tableau ()
{
int i, j, k, op, status, x, y, z;
	i = random (LT);
	j = random (LT);
	k = limite + random (LT-limite);
	/* x = random (0x7FFF); */
	x = hasard ();
	y = hasard ();
	z = random (0x100);
	if (strlen(tableau[i].f)+strlen(tableau[j].f)+6>=LFORMULE)
	{
		set_codex (tableau+k);
		/*
		memcpy (tableau[k].s, codex, LCODEX);
		memcpy (tableau[k].t, codex_t, LCODEX);
		strcpy (tableau[k].f, "c");
		tableau[k].val = val_codex;
		*/
	}
	else
	{
		op = random (NBROP);
		operation (op, tableau[i].s, tableau[j].s, tableau[k].s,
			x, y, z);
		anafreq (tableau[k].s, tableau[k].t);
		switch (arite[op])
		{
			case 0 :
				sprintf (tableau[k].f, "%c", codop[op]);
				break;
			case 1:
				if (op == op_transpos)
					sprintf (tableau[k].f, "%c%d %s",
						codop[op], x, tableau[i].f);
				else if (op == op_diag)
					sprintf (tableau[k].f, "%c%X(%d,%d)%s",
						codop[op], z, x, y, tableau[i].f);
				else if (op == op_transpos_cle)
					sprintf (tableau[k].f, "%c%X(%d)%s",
						codop[op], z, x, tableau[i].f);
				else
					sprintf (tableau[k].f, "%c%s",
						codop[op], tableau[i].f);
				break;
			case 2 :
				sprintf (tableau[k].f, "(%s%c%s)",
					tableau[i].f, codop[op], tableau[j].f);
				break;
		}
		if (flag_anafreq)
		{
			calcul_v (tableau+k, tableau[k].t);
			/*
			tableau[k].v[V_FREQUENCE] = valeur (tableau[k].t);
			tableau[k].v[V_REPET] = v_repet (tableau[k].t);
			*/
		}
		else
		{
			calcul_v (tableau+k, tableau[k].s);
			/*
			tableau[k].v[V_FREQUENCE] = valeur (tableau[k].s);
			tableau[k].v[V_REPET] = v_repet (tableau[k].s]);
			*/
		}
	}
	status = inserer (tableau+k);
	if (status < NMA)
		afficher_meilleurs ();
	if (kbhit ())
		return 0;
	return 1;
}

int donnees[] = {
	 37, 0, 11, 28, 26, 65, 60, 13, 31, 13,
	 14, 25, 30, 4, 64, 111, 124, 43, 64, 8,
	 7, 65, 24, 23, 25, 20, 17, 12, 78, 16,
	 25, 0, 26, 16, 18, 12, 29, 65, 28, 12,
	 5, 21, 2, 26, 78, 17, 3, 16, 4, 65,
	 11, 78, 11, 21, 13, 21, 19, 65, 85, 69,
	 78, 11, 9, 69, 6, 4, 9, 26, 11, 65,
	 29, 16, 19, 65, 11, 8, 78, 23, 5, 9,
	 26, 4, 71, 25, 1, 20, 30, 23, 23, 8,
	 19, 99, 11, 21, 30, 0, 86, 39, 21, 12,
	 4, 20, 31, 75, 124, 107, 35, 78, 15, 17,
	 30, 0, 5, 65, 11, 12, 78, 6, 25, 12,
	 18, 4, 71, 46, 15, 13, 0, 12, 27, 0,
	 21, 13, 78, 18, 25, 23, 86, 13, 2, 73,
	 56, 0, 30, 73, 86, 5, 6, 7, 29, 65,
	 0, 4, 86, 2, 14, 29, 11, 65, 9, 21,
	 31, 18, 4, 6, 30, 0, 0, 0, 86, 5,
	 2, 73, 40, 19, 9, 15, 3, 18, 75, 99,
	 7, 13, 76, 28, 86, 0, 71, 28, 0, 65,
	 15, 9, 25, 8, 19, 27, 11, 65, 13, 19,
	 19, 2, 71, 13, 11, 18, 76, 21, 23, 15,
	 9, 12, 15, 20, 20, 69, 6, 4, 14, 7,
	 26, 18, 76, 23, 19, 17, 21, 12, 29, 4,
	 2, 17, 23, 15, 19, 73, 10, 4, 31, 69,
	 23, 15, 14, 4, 15, 20, 20, 111, 16, 0,
	 9, 29, 15, 18, 24, 12, 7, 20, 2, 26,
	 78, 4, 26, 10, 7, 20, 6, 7, 26, 65,
	 0, 66, 55, 17, 8, 10, 15, 13, 21, 21,
	 5, 4, 73, 99, 39, 13, 76, 28, 86, 0,
	 71, 8, 27, 18, 31, 12, 86, 17, 11, 28,
	 29, 8, 9, 16, 4, 18, 71, 26, 26, 0,
	 24, 16, 19, 18, 71, 13, 11, 65, 8, 23,
	 23, 6, 8, 7, 29, 65, 13, 69, 48, 19,
	 2, 3, 27, 18, 64, 69, 23, 20, 19, 27,
	 11, 65, 28, 10, 5, 18, 14, 11, 7, 13,
	 5, 17, 19, 65, 23, 6, 27, 19, 102, 9,
	 23, 65, 5, 12, 26, 4, 76, 8, 23, 13,
	 1, 8, 7, 18, 13, 11, 2, 4, 73, 99,
	 100, 40, 0, 69, 15, 65, 6, 73, 27, 15,
	 76, 31, 25, 14, 71, 8, 24, 4, 15, 69,
	 19, 15, 19, 27, 11, 65, 13, 16, 2, 19,
	 2, 26, 78, 5, 9, 22, 86, 7, 11, 8,
	 3, 0, 2, 17, 5, 65, 22, 28, 7, 65,
	 28, 10, 3, 19, 21, 8, 7, 4, 2, 17,
	 86, 4, 19, 27, 11, 65, 0, 0, 5, 65,
	 6, 7, 7, 12, 13, 16, 14, 107, 0, 27,
	 15, 2, 5, 0, 3, 25, 73, 99, 100, 37,
	 75, 4, 6, 19, 2, 26, 78, 67, 32, 0,
	 5, 65, 0, 27, 15, 15, 8, 22, 86, 12,
	 30, 26, 26, 4, 30, 0, 5, 65, 74, 73,
	 35, 24, 31, 17, 19, 19, 2, 26, 78, 5,
	 9, 22, 86, 17, 21, 12, 10, 8, 15, 17,
	 31, 14, 9, 26, 76, 65, 28, 9, 3, 18,
	 14, 12, 27, 19, 31, 69, 18, 4, 20, 99,
	 30, 19, 3, 21, 30, 4, 19, 0, 11, 18,
	 76, 1, 19, 65, 41, 6, 29, 21, 30, 4,
	 18, 0, 10, 28, 29, 65, 31, 0, 86, 19,
	 6, 25, 30, 14, 30, 17, 19, 19, 6, 0,
	 11, 15, 24, 69, 23, 65, 41, 8, 30, 14,
	 0, 0, 25, 15, 71, 43, 1, 15, 13, 21,
	 23, 19, 19, 12, 66, 65, 28, 4, 4, 65,
	 2, 17, 11, 12, 28, 9, 19, 65, 93, 99,
	 100, 65, 76, 69, 86, 65, 71, 73, 78, 37,
	 75, 16, 24, 65, 9, 6, 3, 65, 29, 16,
	 19, 65, 9, 28, 2, 65, 30, 10, 31, 65,
	 3, 12, 78, 39, 30, 4, 24, 2, 2, 73,
	 30, 14, 30, 17, 23, 65, 13, 8, 3, 0,
	 5, 22, 90, 107, 71, 73, 78, 65, 76, 69,
	 86, 65, 41, 78, 23, 65, 9, 16, 2, 65,
	 13, 8, 3, 0, 5, 22, 86, 7, 8, 28,
	 10, 19, 9, 69, 5, 8, 71, 29, 11, 19,
	 30, 12, 16, 8, 6, 7, 26, 4, 102, 69,
	 86, 65, 71, 73, 78, 65, 76, 44, 2, 0,
	 11, 0, 11, 77, 76, 32, 5, 17, 6, 14,
	 0, 4, 64, 69, 19, 21, 71, 5, 11, 18,
	 76, 36, 24, 6, 11, 8, 7, 18, 76, 0,
	 24, 65, 19, 27, 11, 12, 14, 9, 19, 15,
	 19, 69, 100, 104, 43, 23, 23, 15, 3, 12,
	 78, 0, 24, 17, 19, 15, 19, 0, 1, 15,
	 76, 21, 25, 19, 19, 12, 28, 0, 76, 4,
	 3, 25, 71, 15, 11, 12, 1, 0, 5, 65,
	 2, 29, 28, 0, 2, 2, 19, 19, 2, 26,
	 64, 107, 102, 33, 81, 0, 23, 27, 11, 18,
	 76, 9, 81, 32, 19, 5, 15, 18, 76, 13,
	 31, 18, 19, 6, 28, 8, 29, 16, 19, 65,
	 38, 27, 3, 0, 2, 1, 86, 34, 8, 5,
	 7, 15, 64, 69, 52, 14, 9, 8, 30, 0,
	 30, 17, 19, 65, 3, 12, 12, 0, 30, 20,
	 3, 4, 71, 8, 78, 39, 30, 0, 28, 20,
	 20, 73, 11, 15, 76, 84, 65, 88, 94, 99,
	 15, 20, 76, 23, 19, 21, 8, 28, 28, 65,
	 8, 66, 51, 6, 30, 25, 26, 4, 76, 0,
	 2, 65, 20, 78, 23, 65, 9, 8, 20, 0,
	 21, 24, 27, 4, 76, 0, 24, 65, 86, 81,
	 95, 85, 76, 21, 25, 20, 21, 73, 2, 70,
	 5, 9, 19, 65, 3, 78, 43, 13, 14, 0,
	 88, 107, 109, -1 };


int sl (char *str)
{
int i;
int s;
	s = 0;
	for (i=0; i<strlen(str); i++)
		s += str[i];
	return s;
}

afficher_donnees ()
{
int i, l;
int x[10];
char buf[200];

	printf ("Mot de passe ? ");
	gets (buf);
	if (!*buf)
		return;
	l = strlen (buf);

	for (i=0; donnees[i]>=0; i++)
	{
		putchar (donnees[i] ^ buf[i%l]);
	}
	printf ("Appuyer sur Entree pour continuer...\n");
	gets (buf);

}

main (int argc, char *argv[])
{
FILE *f;
int i;
char buf[1000], buf1[1000];
long val;
long v[NV];

	/* afficher_donnees (); */

	printf ("%d %d %d\n", NBROP, sizeof(arite)/sizeof(arite[0]), op_fin);
	init_codex ();
	randomize ();
	for (i=0; i<NS; i++)
		frequence[i] = 0;

	f = fopen (argv[1], "r");
	if (f == NULL)
	{
		perror (argv[1]);
		printf ("usage: decrypt filename\n");
		printf ("Indiquer en parametre le nom d'un fichier contenant\n");
		printf ("un texte dans la langue du message a decoder, pour le\n");
		printf ("calcul des frequences des lettres.\n");
		return errno;
	}
	calcul_frequence (f, frequence, frequence1, frequence2, esartin, lire_fichier, f);
	fclose (f);

	printf ("esartin = ");
	for (i=0; i<NCAR; i++)
		printf ("%c", 'A' + esartin[i]);
	printf ("\n");

	generer (frequence, 1000);

	for (i=0; i<LCODEX; i++)
		printf ("%c", 'A' + debut[i]);
	printf ("\n");

	v_opt[V_FREQUENCE] = valeur_frequence (debut, LCODEX);
	v_opt[V_REPET] = v_repet (debut);

	/* val_opt = 824; */
	printf ("Valeur optimale : %ld %ld\n", v_opt[0], v_opt[1]);
#ifndef NEW
#endif

    for (;;)
    {
	printf ("\n");
	printf ("M  : fonction mathematique\n");
	printf ("L  : correspondance lettre par lettre quelconque\n");
	printf ("ML : fonction mathematique et correspondance lettre par lettre\n");
	printf ("T  : test\n");
	printf ("Q  : quitter\n");
	printf ("Votre choix ? ");
	gets (buf);

	if (*buf == 'L' || *buf == 'l')
	{
		printf ("Apres affichage du resultat : \n");
		printf ("Appuyez sur Entree pour faire un nouvel essai de decodage.\n");
		printf ("Tapez M pour continuer le decodage manuellement.\n");
		printf ("Tapez Q pour retourner au menu.\n");
		printf ("Decodage en cours...\n");
		test_anafreq ();
	}
	if (*buf == 'M' || *buf == 'm')
	{
		printf ("Cles de decodage\n");
		n_keys = 0;
		for (i=0; i<MAXKEYS; i++)
		{
			printf ("? ");
			gets (key[i]);
			if (!key[i][0])
				break;
			n_keys++;
		}

		if (buf[1] == 'L' || buf[1] == 'l')
			flag_anafreq = 1;
		else
			flag_anafreq = 0;
		init_tableau ();

		clrscr ();

		afficher_meilleurs ();

		while (evol_tableau ());

		printf ("Appuyez sur Entree pour continuer...");
		gets (buf);
		gets (buf);
	}
	if (*buf == 'T' || *buf == 't')
	{
	for (;;)
	{
		printf ("? ");
		gets (buf);
		if (!*buf)
			break;
		for (i=0; i<strlen(buf); i++)
			buf1[i] = buf[i] - 'A';
		buf1[strlen(buf)] = 0;
		val = valeur_frequence (buf1, strlen(buf));
		v[0] = val;
		printf (" %ld ", val);
		val = v_repet (buf1);
		v[1] = val;
		printf (" %ld ", val);
		printf (" %lf \n", distance_v (v, v_opt));
	}

	}
	if (*buf == 'Q' || *buf == 'q')
		break;
    }
	/*
	nma = LM;
	afficher_meilleur ();
	*/
}
