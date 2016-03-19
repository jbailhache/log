
#include <stdio.h>
#include <conio.h>

#define MAX 50 /* sous-arbres par noeud */

/*
typedef int noeud;
*/

#define noeud int

#define NSA (-1)

#define n_sous_arbres(x) arbre(x,NSA)
#define sous_arbre(x,a) (noeud) arbre(x,a)

int arbre (noeud x, int n)
{
	switch (x)
	{
		case 1:
			switch (n)
			{
				case NSA: return 3;
				case 0: return 2;
				case 1: return 3;
				case 2: return 4;
			}
		case 2:
			switch (n)
			{
				case NSA: return 1;
				case 0: return  5;
			}
		case 3:
			switch (n)
			{
				case NSA: return 3;
				case 0: return 1;
				case 1: return 7;
				case 2: return 8;
			}
		/*
		case 4:
			switch (n)
			{
				case NSA: return 0;
			}
		case 5:
			switch (n)
			{
				case NSA: return 0;
			}
		*/
		case 6:
			switch (n)
			{
				case NSA: return 1;
				case 0: return 9;
			}
		case 8:
			switch (n)
			{
				case NSA: return 4;
				case 0: return 10;
				case 1: return 8;
				case 2: return 11;
				case 3: return 12;
			}
		default:
			switch (n)
			{
				case NSA: return 0;
				default:
					printf (
	"Erreur: le noeud %d est une feuille et n'a pas de sous-arbre %d.\n",
								x, n);
			}
	}
}

struct eliste
{
	noeud premier;
	struct eliste *suivants;
};

typedef struct eliste *liste;

#define NIL (-1)

#define prem(l) ((l)->premier)
#define suiv(l) ((l)->suivants)

liste cons (noeud p, liste s)
{
liste c;
	c = malloc (sizeof(struct eliste));
	prem(c) = p;
	suiv(c) = s;
	return c;
}

int member (noeud x, liste l)
{
	if (l == NIL)
		return 0;
	if (prem(l) == x)
		return l;
	return member (x, suiv(l));
}

/*

0           |
1		 racine
2			|
3   +-----+-+-------+
0   |     |         |
1	a     b         c
2

*/

int h = 4; /* hauteur niveau */
int l = 5; /* largeur feuille */

#define NLIN 20
#define NCOL 70

/*
#define VERTICAL '|'
#define HORIZONTAL '-'
#define JONCTION '+'
#define POIN