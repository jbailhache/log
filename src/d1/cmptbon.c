
#include <stdio.h>

#define ND 6
#define NO 4
#define NA 2

/* #define NIL 0 */

#define SOLSTR

#ifdef SOLPTR

#define NIL 0

typedef struct solution *solution;

struct solution
{
	int res;
	char op;
	solution d[NA];
};

#define is_nil(sol) ((sol)==NIL)
#define resultat(sol) ((sol)->res)

liberer (solution sol)
{
	free (sol);
}

imprimer (solution sol)
{
int i;
	if (sol->op == '=')
		/* printf ("%d\n", res)*/;
	else
	{
		for (i=0; i<NA; i++)
			imprimer (sol->d[i]);
		printf ("%3d %c %3d = %3d\n",
			sol->d[0]->res,
			sol->op,
			sol->d[1]->res,
			sol->res);

	}
}

#endif

#ifdef SOLSTR

struct solution
{
	int res;
	char s[300];
};

typedef struct solution solution;

struct solution nil = {-1, {0}};

#define NIL nil

/*
int is_nil (solution sol)
{
	return (sol.res == -1);
}
*/

#define is_nil(sol) ((sol).res == -1)
#define resultat(sol) ((sol).res)

liberer (solution sol)
{
}

imprimer (solution sol)
{
	/* printf ("%3d = %s\n", sol.res, sol.s); */
	printf ("\nSolution pour %3d:\n%s\n", sol.res, sol.s);
}

#endif

char heap[20000];

char *ptr = heap;

char *myalloc (int n)
{
char *p;
	return malloc(n);
	p = ptr;
	ptr += n;
	if (ptr >= heap + sizeof(heap))
		return NULL;
	return p;
}

#ifdef SOLPTR

solution donnee (int n)
{
solution sol;
	sol = myalloc (sizeof(*sol));
	if (sol == NULL)
	{
		printf ("Memoire insuffisante\n");
		exit (0);
	}
	sol->op = '=';
	sol->res = n;
	return sol;
}

solution operation (int no, solution d0, solution d1)
{
solution sol, p, g;
	sol = myalloc (sizeof(*sol));
	if (sol == NULL)
	{
		printf ("Memoire insuffisante\n");
		exit (0);
	}
	if (d0->res < d1->res)
	{
		p = d0;
		g = d1;
	}
	else
	{
		p = d1;
		g = d0;
	}
	sol->d[0] = g;
	sol->d[1] = p;
#define DO(n,c,o) case n: sol->op = c; sol->res = g->res o p->res; break;
	switch (no)
	{
		/*
		case 1:
			sol->op = '+';
			sol->res = d0->res + d1->res;
		*/
		DO (1, '+', +)
		DO (2, '*', *)
		DO (3, '-', -)
		/* DO (4, '/', /) */
		case 4:
			if (p->res != 0 && (g->res % p->res == 0))
			{
				sol->op = '/';
				sol->res = g->res / p->res;
			}
			else
				return NIL;
			break;
		default:
			printf ("Erreur\n");
			exit (0);
	}
	return sol;
}

#endif

#ifdef SOLSTR

solution donnee (int n)
{
solution sol;
	sol.res = n;
	/* sprintf (sol.s, "[%d]\n", n); */
	sol.s[0] = 0;
	return sol;
}

solution operation (int no, solution d0, solution d1)
{
solution sol, p, g;
char op;
int res;
	if (d0.res < d1.res)
	{
		p = d0;
		g = d1;
	}
	else
	{
		p = d1;
		g = d0;
	}
	/*
	sol->d[0] = g;
	sol->d[1] = p;
	*/

#define DO(n,c,o) case n: op = c; res = g.res o p.res; break;
	switch (no)
	{
		/*
		case 1:
			sol->op = '+';
			sol->res = d0->res + d1->res;
		*/
		DO (1, '+', +)
		DO (2, '*', *)
		DO (3, '-', -)
		/* DO (4, '/', /) */
		case 4:
			if (p.res != 0 && (g.res % p.res == 0))
			{
				op = '/';
				res = g.res / p.res;
			}
			else
				return NIL;
			break;
		default:
			printf ("Erreur\n");
			exit (0);
	}
	sol.res = res;
	/*
	sprintf (sol.s, "%s%s%d = %d %c %d\n",
		g.s, p.s, res, g.res, op, p.res);
	*/
	sprintf (sol.s, "%s%s\t%3d %c %3d = %3d\n",
		g.s, p.s, g.res, op, p.res, res);
	return sol;
}

#endif

solution chercher (int res, int nd, solution d[])
{
int i, j, k, l, o;
solution d1[ND], sol;
	for (i=0; i<nd; i++)
	{
		if (resultat(d[i]) == res)
			return d[i];
	}
	if (nd <= 1)
		return NIL;
	for (i=0; i<nd-1; i++)
	for (j=i+1; j<nd; j++)
	for (o=1; o<=NO; o++)
	{
		if (nd >= 5)
			printf ("\r %d %d %d %d ", nd, i, j, o);
		d1[0] = operation (o, d[i], d[j]);
		if (is_nil(d1[0]))
			continue;
		for (k=0, l=1; k<nd, l<nd-1; k++)
			if (k!=i && k!=j)
				d1[l++] = d[k];
		sol = chercher (res, nd-1, d1);
		if (!is_nil(sol)) /*(sol != NIL)*/
			return sol;
		else
			liberer (d1[0]);
	}
	return NIL;

}

main ()
{
int res, di, i;
solution d[ND], sol;
	printf ("Resultat ? ");
	scanf ("%d", &res);
	printf ("Donnees ? ");
	for (i=0; i<ND; i++)
	{
		scanf ("%d", &di);
		d[i] = donnee(di);
	}
	printf ("Veuillez patienter, je reflechis...\n");
	sol = chercher (res, ND, d);
	if (!is_nil(sol)) /*(sol != NIL)*/
		imprimer (sol);
	else for (i=1; i<res; i++)
	{
		sol = chercher (res+i, ND, d);
		if (!is_nil(sol)) /*(sol != NIL)*/
		{
			imprimer (sol);
			break;
		}
		sol = chercher (res-i, ND, d);
		if (!is_nil(sol)) /*(sol != NIL)*/
		{
			imprimer (sol);
			break;
		}

	}


}
