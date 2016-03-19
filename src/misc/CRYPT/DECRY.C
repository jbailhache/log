
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <math.h>

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

/* #define TEST */
#define TESTF

#ifndef TEST
char codex[] = { 105, 2, 106, 4, 112, 15, 96, 3, 114, 1, 109, 72, 23, 51,
	108, 16, 90, 89, 81, 56, 111, 6, 94, 63, 2, 98, 61, 56, 103, 7,
	27, 101, 13, 94, 55 };
#else
char codex[] = "BONJOURCECIESTUNTESTDEDECODAGEAUREVOIR";
/* char codex[] = "BRSSCAIOCTTOURNEUDDRZJCNEAEZOITDGVZUEEEEOZ"; */

#endif

char code[] = "QWERTYUIOPASDFGHJKLZXCVBNM";

#define LCODEX (sizeof(codex))

char codex_t[LCODEX];

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

long /*v_codex[NV],*/ v_opt[NV];
/*float distance_codex;*/

struct item it_codex[1];

#define v_codex (it_codex->v)
#define distance_codex (it_codex->distance)

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
	op_rev, op_decal, op_ind, op_mix, op_transpos,
#ifdef OP_ANAFREQ
					op_anafreq,
#endif
						op_fin };

int arite [] = {  2,   2,   2,   2,   2,   2,   2,   2,   1,   1,
	 0,   0,   0,   0,   0,   1,   1,   0,   1,   1,   1,   1,
	 1,   2,   2,   2,   1
#ifdef OP_ANAFREQ
			,   1
#endif
				};

char codop[] = { '+', '-', '*', '/', '%', '&', '|', 'e', '.', '~',
	'0', '1', '2', 'f', 'n', 'd', 's', 'k', 'p', 'm', 'P', 'M',
	'r', '>', ':', '@', 't'
#ifdef OP_ANAFREQ
			  , '?'
#endif
				};

char *codops[] = { "+", "-", "*", "/", "%", "&", "|", " XOR ", ".", "~",
	"0", "1", "2", "F", "N", "d", "s", "k", "(1+)", "(-1+)" , "2*", "1/2*",
	"r", ">", ":", "@", "t"
#ifdef OP_ANAFREQ
			, "?"
#endif
				};

#define NBROP (sizeof(codop)/sizeof(codop[0]))

#if (sizeof(arite)/sizeof(arite[0]) != NBROP) || op_fin != NBROP
#error Erreur dans la definition des operateurs
#endif

anafreq_ordre (char *s, char *u, int ordre)
{
int i, j, i1, j1, k, l;
char s1[LCODEX];
int freq1[NCAR];
char esarti[NCAR];
int freq2[NCAR][NCAR];
long corresp [NCAR] [NCAR];
char tri_corresp[NCAR*NCAR][2];
char transcod [2][NCAR];

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
			corresp[i][j] += frequence1[i] * LCODEX -
					 freq1[j] * lng_model;

			for (i1=0; i1<NCAR; i1++)
			for (j1=0; j1<NCAR; j1++)
#if 1
				corresp[i][j] += (
					frequence2[i][i1] * (LCODEX-1) -
					freq2[j][j1] * (lng_model-1) );
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

test_anafreq ()
{
char buf[LCODEX];
int i;
    for (;;)
    {
	anafreq_ordre (codex, buf, 2);
	for (i=0; i<LCODEX; i++)
		printf ("%c", 'A' + buf[i]);
	printf (" ? ");
	gets (buf);
	if (*buf)
		break;
    }
}

operation (int op, char *s, char *t, char *u, int x)
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
				u[i] = (i%2) ? s[i] : t[i];
				break;
			case op_transpos :
				/* u[i] = s[mod(LCODEX,i*mod(LCODEX,x))]; */
				u[i] = s[mod(LCODEX,i*(x+2))];
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
int p, i;
	v = 0;
	for (p=1; p<LCODEX; p++)
	{
		for (i=0; i+2*p<LCODEX; i++)
			if (s[i] == s[i+p] && s[i] == s[i+2*p])
				v++;
	}
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
int j, k;
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
		    it->f[i] == 'c' ||
		    it->f[i] == 'k')
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

	for (i=0; i<LCODEX; i++)
	{
		c = 'A' + mod (N_CAR, s[i]-NUMA);
		if (c > 'Z')
			c = '.';
		printf ("%c", c /*'A' + (((s[i]-NUMA)%N_CAR)+N_CAR)%N_CAR*/);
	}
	printf ("\n");
	anafreq (s, s1);
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

	clrscr ();

	for (i=0; i<nma; i++)
	{
		/*printf ("\n%3d %4ld %4ld %4ld %s\n",
			i, meilleur[i].val,
			0L, 0L,*/
			/*
			valeur (meilleur[i].s),
			valeur (meilleur[i].t),*/
			/* meilleur[i].f); */
		printf ("\n%3d %lf %ld %ld %s\n",
			i,
			meilleur[i].distance,
			meilleur[i].v[0],
			meilleur[i].v[1],
			meilleur[i].f);
		afficher_s (meilleur[i].s);
		printf ("\n");
	}
	/*
	gets (buf);
	if (*buf == 'q')
		exit (0);
	*/
}

int evol_tableau ()
{
int i, j, k, op, status, x;
	i = random (LT);
	j = random (LT);
	k = limite + random (LT-limite);
	/* x = random (0x7FFF); */
	x = hasard ();

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
		operation (op, tableau[i].s, tableau[j].s, tableau[k].s, x);
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

main (int argc, char *argv[])
{
FILE *f;
int i;
char buf[1000], buf1[1000];
long val;
long v[NV];

	printf ("%d %d %d\n", NBROP, sizeof(arite)/sizeof(arite[0]), op_fin);
	init_codex ();
	randomize ();
	for (i=0; i<NS; i++)
		frequence[i] = 0;

	f = fopen (argv[1], "r");
	if (f == NULL)
	{
		perror (argv[1]);
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
#endif

    for (;;)
    {
	printf ("\n");
	printf ("M  : fonction mathematique\n");
	printf ("L  : correspondance lettre par lettre quelconque\n");
	printf ("ML : fonction mathematique et correspondance lettre par lettre\n");
	printf ("Q  : quitter\n");
	printf ("Votre choix ? ");
	gets (buf);

	if (*buf == 'L' || *buf == 'l')
	{
		printf ("Apres affichage du resultat : \n");
		printf ("Appuyez sur Entree pour faire un nouvel essai de decodage.\n");
		printf ("Tapez q pour retourner au menu.\n");
		printf ("Decodage en cours...\n");
		test_anafreq ();
	}
	if (*buf == 'M' || *buf == 'm')
	{
		if (buf[1] == 'L' || buf[1] == 'l')
			flag_anafreq = 1;
		else
			flag_anafreq = 0;
		init_tableau ();

		afficher_meilleurs ();

		while (evol_tableau ());

		printf ("Appuyez sur Enter pour continuer...");
		gets (buf);
		gets (buf);
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