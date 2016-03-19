/*
	Simulation d'un reseau de neurones
	Me 11-3-87

Constantes :
	N_NEUR : nombre de neurones = N_LIG * N_COL (16 * 16 = 256)

Etat du reseau :
	pointeur rn -> structure :
	connex (C): tableau N_NEUR x N_NEUR d'entiers courts
		connex[i][j] = connexion du neurone j vers i
	neur (excit) : tableau de N_NEUR entiers : 0=au repos, 1=excite
		neur[i]&1=excitation
		neur[i]&2=variabilite
		0: fixe au repos
		1: fixe excite
		2: variable au repos
		3: variable excite
		ou tableau de structures de 2 caracteres: excit et var
		ou 2 tableaux de caracteres excit (X) et var (V)
	pexcit : etat precedent (P)
	coef_rand (alpha)
	energie (E) : entier
	temps (t) : entier
	ligc, colc : position courante [0..N_LIG-1] [0..N_COL-1]
	neurc : neurone courant = N_LIG*ligc+colc [0..N_NEUR-1]

Fonctions de manipulation :
	energie_neur (rn) = E
	evol_neur (rn) -> nouvel etat du reseau rn'
	recomp_neur (rn, n) -> reseau reconmpense de n
	mut_neur (neur, n) -> etat apres mutation aleatoire
	f_rand )alpha, y) = fonction aleatoire (f)
		fonction valant p=1/2 pour y=0
				p=3/4 pour y=alpha
				p=1/4 pour y=-alpha
				p->1  pour y->infini
				p->0  pour y->-infini
		avec p = proba (x'=1) (sinon x'=0)
		p = g_rand (alpha, y)
		exemple : p = 1/2 + 1/pi * arc tg (y/alpha)

Affichage :
	Tableau N_LIG * N_COL des neurones
		0->= 1->#
		2->- 3->+
	coef_rand (alpha = ...)
	temps (t = ...)
	energie (E = ...)
	curseur sur le neurone courant

Commandes : (mode cbreak sans echo)
	, -> evol
	r -> recomp 1
	p -> recomp -1 (punition)
	m -> mutation
	hjkl -> deplacement
	01fv ou asdf, =#-+: changement de l'etat du neurone courant
	a -> modif coef_rand
	s -> sauvegarde rn dans sv_rn
	l -> rechargement rn a partir de sv_rn
	q -> sortie

Mise a jour automatique :
	energie
	temps

Implementation :
	calcul energie :
		E = (transposee de X) * C * X
		= somme(i,j=0..N_NEUR-1) (C[i,j] * X[i] * X[j])
	evol :
		Y = C * X	Y[i] = somme(j)(C[i,j]*X[j])
		X'=F(Y)		X'[i] = f(Y[i]) si variable, inchange sinon
	recompense r :
		C' = C + r * X * transposee de X
		C'[i,j] = C[i,j] + r*X[i]*X[j]
	energie dynamique :
		ED = (transposee de X) * C * P (P = precedent X)
		= somme(i,j) (C[i,j] * X[i] * P[j])
	energie du neurone i :
		e = C * X
		e[i] = somme(j) (C[i,j] * X[i])
	recompense dynamique r :
		C' = C + r * X * transposee de P
		C'[i,j] = C[i,j] + r * X[i] * P[j]
		association P->X

Utilisation :
	memoire associative :
		auto-association X -> X :
			etat X, recompense ||X|| puissance 2
				= nombre de bits a 1
			"creuse un trou"
		variante :
			fixer une partie de X
			repeter evol -> equilibre thermique
			continuer un peu en recompensant a chaque fois
			le premier cas peut etre vu comme un cas particulier
			ou la partie de X fixee est X tout entier.
	systeme cybernetique :
		decomposer X en E (entrees fixes), S (sorties variables)
		et I (intermediaires variables)
		etat (C,X) initial aleatoire
		fixer E
		repeter evol -> equilibre
		-> sorties S
		jugement -> recompense r
		mutations aleatoires
	machine de Boltzmann :
		d'abord
			fixer E et S voulue
			repeter evol -> equilibre
			continuer un peu et recompenser
		puis
			fixer E, laisser S varier
			repeter evol -> equilibre
			continuer un peu et punir
		interpretation :
			deplacement etat actuel -> etat voulu
			= etat voulu - etat actuel.

*/

#include <stdio.h>
#include <math.h>

/* #define TY_FACIT */
#define TY_PC

/* terminal virtuel facit */

#ifdef TY_FACIT

ty_up (int n) 		{ printf ("\x1B\x5B%d\x41", n); }
ty_dowm (int n)		{ printf ("\x1B\x5B%d\x42", n); }
ty_right (int n)        { printf ("\x1B\x5B%d\x43", n); }
ty_left (int n)		{ printf ("\x1B\x5B%d\x44", n); }
ty_locate (int lig, int col)
			{ printf ("\x1B\x5B%d\x3B%d\x48", lig, col); }
ty_cls ()		{ printf ("\x1B\x5B\x31\x58"); ty_locate(1,1); }
ty_cleol ()		{ printf ("\x1B\x5B\x30\x4B"); }

int getchar_raw ()
{
	return getchar ();
}

#endif

/* terminal virtuel PC */

#ifdef TY_PC

ty_cls () { clrscr (); }
/* ty_home () { gotoxy (1, 1); } */
ty_up (int n) { gotoxy (wherex(), wherey()-n); }
ty_down (int n) { gotoxy (wherex(), wherey()+n); }
ty_right (int n) { gotoxy (wherex()+n, wherey()); }
ty_left (int n) { gotoxy (wherex()-n, wherey()); }
ty_locate (int lig, int col) { gotoxy (col, lig); }
ty_cleol () { clreol (); }

int getchar_raw ()
{
	return getch ();
}

#endif

typedef int integer;
#define short char

ty_home () { ty_locate (1, 1); }

#define N_LIG 8
#define N_COL 8
#define N_NEUR (N_LIG * N_COL)

#define FOR_NEUR(i) for (i=0; i<N_NEUR; i++)

double source_rnd;

init_rnd (int n)
{
	source_rnd = n;
}

#define RND1 24298.0
#define RND2 99991.0
#define RND3 199017.0

double rnd ()
{
double x;
	x = source_rnd * RND1 + RND2;
	source_rnd = x - RND3 * (integer) (x / RND3);
	return source_rnd / RND3;
}

int pof ()
{
	return rnd() > 0.5;
}

int sauv_dec;

init ()
{
	srand (1);
	init_rnd (1.0);
	sauv_dec = 0;
}

struct s_reseau_neur
{
	integer coef_rand, energie, temps, ligc, colc, neurc,
		ergdyn, seuil_global, bas, haut, par, compar,
		rep, marque, i1;
	short s1;
	char loc, c1;
	short excit[N_NEUR];
	short pexcit[N_NEUR];
	short var[N_NEUR];
	short seuil[N_NEUR];
	short connex[N_NEUR][N_NEUR];
} r_neur;

struct s_reseau_neur *rn = &r_neur;
typedef struct s_reseau_neur *reseau_neur;

int maj, maj_eff;

init_neur ()
{
int i, j;
	rn = &r_neur;
	rn->coef_rand = 0;
	rn->temps = 0;
	rn->ligc = 0;
	rn->colc = 0;
	rn->neurc = 0;
	rn->seuil_global = 0; /* 1 */
	rn->bas = 0;
	rn->haut = 1;
	rn->par = 1;
	rn->compar = 0; /* 0: >, 1: >= */
	rn->rep = 1;
	rn->loc = '#';
	for (i=0; i<N_NEUR; i++)
	{
		rn->excit[i] = rn->bas;
		rn->pexcit[i] = rn->bas;
		rn->var[i] = 1;
		rn->seuil[i] = 0;
		for (j=0; j<N_NEUR; j++)
			rn->connex[i][j] = 0;
	}
	maj = 1;
	maj_neur ();
	/* maj = 0; */
}

init_es_neur ()
{
#ifdef TY_FACIT
	system ("stty -echo cbreak");
#endif
	ty_cls ();
	aff_neur ();
	maj_aff_neur ();
}

term_es_neur ()
{
#ifdef TY_FACIT
	system ("stty echo -cbreak");
#endif
	ty_locate (6+N_LIG,1);
}

/*
maj_neur ()
{
int i, j;
	rn->energie = 0;
	for (i=0; i<N_NEUR; i++)
	for (j=0; j<N_NEUR; j++)
		rn->energie += rn->connex[i][j] * rn->excit[i] * rn->excit[j];
}
*/

int erg_neur (int dyn)
{
int i, j, e, ei;
	e = 0;
	/* for (i=0; i<N_NEUR; i++) */
	FOR_NEUR(i)
	{
		ei = 0;
		for (j=0; j<N_NEUR; j++)
			ei += rn->connex[i][j] *
				(dyn?(rn->pexcit):(rn->excit))[i] *
				((rn->var[j]&2)?0:1);
		e += rn->excit[i] * (ei - rn->seuil_global - rn->seuil[i]);
	}
	return e;
}

int energie_neur ()
{
	return erg_neur (0);
}

int ergdyn_neur ()
{
	return erg_neur (1);
}

maj_energie_neur ()
{
	rn->energie = energie_neur();
}

maj_ergdyn_neur ()
{
	rn->ergdyn = ergdyn_neur();
}

maj_neur ()
{
	if (!maj)
		maj_eff = 0;
	else
	{
		maj_energie_neur ();
		maj_ergdyn_neur ();
		maj_eff = 1;
	}
}

locate_neur (int lig, int col)
{
	ty_locate (5+lig, 2+2*col);
}

int erg_neurc (int dyn)
{
int e, i;
	e = 0;
	FOR_NEUR(i)
		e += rn->connex[rn->neurc][i]
			/* * rn->excit[rn->neurc] */
			* (dyn?(rn->pexcit):(rn->excit))[i]
			* ((rn->var[i]&2)?0:1);
	return e;
}

int energie_neurc ()
{
	return erg_neurc (0);
}

int ergdyn_neurc ()
{
	return erg_neurc (1);
}

int seuil_neurc ()
{
	return rn->seuil_global + rn->seuil[rn->neurc];
}

maj_aff_neur ()
{
int e, d, th, i;
	e = energie_neurc ();
	d = ergdyn_neurc ();
	th = seuil_neurc ();

	ty_locate (2, 1);
	printf (" t=%d a=%d p=%d c=%d x0=%d x1=%d r=%d fs=%d %c ",
		rn->temps,
		rn->coef_rand,
		rn->par,
		rn->compar,
		rn->bas,
		rn->haut,
		rn->rep,
		sauv_dec,
		rn->loc);
	ty_cleol ();
	printf ("\n E=%d ED=%d th=%d E[i]=%d ED[i]=%d th[i]=%d ",
		rn->energie,
		rn->ergdyn,
		rn->seuil_global,
		e,
		d,
		th);
	if (!maj)
	{
		/*
		printf ("{");
		if (maj_eff) printf ("}");
		printf (" ");
		*/
		if (maj_eff)
			printf ("A ");
		else
			printf ("P ");
	}
	ty_cleol ();
	locate_neur (rn->ligc, rn->colc);
	aff_i_neur (rn->neurc);
	locate_neur (rn->ligc, rn->colc);
}


#define CNFR '='
#define CNFE '#'
#define CNVR '-'
#define CNVE '+'
#define CNMR '.'
#define CNME '*'

int excite (int i)
{
	return rn->excit[i] >= rn->haut;
}

aff_i_neur (int i)
{
	if (rn->var[i])
	{
		if ((rn->var[i]&2)?0:1)
		{
			/*if (rn->excit[i]==rn->haut)*/
			if (excite(i))
				putchar (CNVE);
			else
				putchar (CNVR);
		}
		else
		{
			if (excite(i))
				putchar (CNME);
			else
				putchar (CNMR);
		}
	}
	else
	{
		if (excite(i))
			putchar (CNFE);
		else
			putchar (CNFR);
	}
}

aff_neur1 (int mod)
{
int lig, col, i;
	for (lig=0; lig<N_LIG; lig++)
	for (col=0; col<N_COL; col++)
	{
		i = N_COL * lig + col;
		if (!mod || rn->excit[i]!=rn->pexcit[i])
		{
			locate_neur (lig, col);
			aff_i_neur (i);
		}
	}
}

aff_neur ()
{
	aff_neur1 (0);
}

aff_mod_neur ()
{
	aff_neur1 (1);
}

depl_gauche ()
{
	if (rn->colc > 0)
	{
		rn->colc--;
		rn->neurc--;
		maj_aff_neur();
	}
}

depl_droite ()
{
	if (rn->colc < N_COL-1)
	{
		rn->colc++;
		rn->neurc++;
		maj_aff_neur();
	}
}

depl_haut ()
{
	if (rn->ligc > 0)
	{
		rn->ligc--;
		rn->neurc -= N_COL;
		maj_aff_neur();
	}
}

depl_bas ()
{
	if (rn->ligc < N_LIG-1)
	{
		rn->ligc++;
		rn->neurc += N_COL;
		maj_aff_neur();
	}
}

fix_neurc ()
{
int flag;
	flag = rn->var[rn->neurc] & 2;
	rn->var[rn->neurc] = 0;
	if (excite(rn->neurc))
		putchar (CNFE);
	else
		putchar (CNFR);
	ty_left (1);
	if (flag)
	{
		maj_neur();
		maj_aff_neur();
	}
}

var_neurc ()
{
int flag;
	flag = rn->var[rn->neurc] & 2;
	rn->var[rn->neurc] = 1;
	if (excite(rn->neurc))
		putchar (CNVE);
	else
		putchar (CNVR);
	ty_left (1);
	if (flag)
	{
		maj_neur();
		maj_aff_neur();
	}
}

nm_neurc ()
{
	rn->var[rn->neurc] = 3;
	if (excite(rn->neurc))
		putchar (CNME);
	else
		putchar (CNMR);
	ty_left (1);
	maj_neur();
	maj_aff_neur();
}

repos_neurc ()
{
	rn->excit[rn->neurc] = rn->bas;
	maj_neur();
	maj_aff_neur();
}

excit_neurc ()
{
	rn->excit[rn->neurc] = rn->haut;
	maj_neur();
	maj_aff_neur();
}

recomp_neur1 (int dyn, int r)
{
int i, j;
	FOR_NEUR(i) FOR_NEUR(j)
		rn->connex[i][j] += r * rn->excit[i] *
			(dyn?rn->pexcit:rn->excit)[j];
	maj_neur();
}

recomp_neur (int r)
{
	recomp_neur1 (0, r);
}

recomp_dyn_neur (int r)
{
	recomp_neur1 (1, r);
}

int f_rand (int seuil, int y, int p)
{
	if (y >= seuil + rn->coef_rand)
	{
		if (y != seuil)
			return rn->haut;
		else if (rn->compar == 2)
			return p;
		else if (rn->compar == 1)
			return rn->haut;
		else
			return rn->bas;
	}
	else if (y <= seuil - rn->coef_rand)
		return rn->bas;
	else if (pof())
		return rn->haut;
	else
		return rn->bas;
}

evol_neur_seqpar (int par, int n)
{
int t, i, j, y;
	for (t=0; t<n; t++)
	{
		if (par)
		{
			FOR_NEUR (i)
				rn->pexcit[i] = rn->excit[i];
		}
		FOR_NEUR (i)
		{
			y = 0;
			FOR_NEUR (j)
				y += rn->connex[i][j] *
					(par?(rn->pexcit):(rn->excit))[j] *
					((rn->var[j]&2)?0:1);
			if (rn->var[i])
				rn->excit[i] = f_rand (rn->seuil_global+
					rn->seuil[i], y,
					(par?(rn->pexcit):(rn->excit))[i]);
		}
	}
	rn->temps += t;
	maj_neur();
}

evol_neur_seq (int n)
{
	evol_neur_seqpar (0, n);
}

evol_neur_par (int n)
{
	evol_neur_seqpar (1, n);
}

evol_neur (int n)
{
	evol_neur_seqpar (rn->par, n);
}

av_neur ()
{
int i;
	FOR_NEUR (i)
		rn->pexcit[i] = rn->excit[i];
}

ar_neur ()
{
int i;
	FOR_NEUR (i)
		 rn->excit[i] = rn->pexcit[i];
}

ech_neur ()
{
int i, x;
	FOR_NEUR (i)
	{
		x = rn->pexcit[i];
		rn->pexcit[i] = rn->excit[i];
		rn->excit[i] = x;
	}
}

rab_neur ()
{
int i;
	FOR_NEUR (i)
		if (pof())
			rn->excit[i] = rn->bas;
}

rah_neur ()
{
int i;
	FOR_NEUR (i)
		if (pof())
			rn->excit[i] = rn->haut;
}

#define FNEUR "neur.sav"

sauv_bin_neur ()
{
FILE *fneur;
int status;
	fneur = fopen (FNEUR, "wb");
	if (fneur == NULL)
		perror ("Error opening binary file for writing");
	else
	{
		status = fwrite (rn, 1, sizeof(*rn), fneur);
		if (status != sizeof(*rn))
			perror ("Error writing binary file");
		fclose (fneur);
	}
}

charg_bin_neur ()
{
FILE *fneur;
int status;
	fneur = fopen (FNEUR, "rb");
	if (fneur == NULL)
		perror ("Error opening binary file for reading");
	else
	{
		status = fread (rn, 1, sizeof(*rn), fneur);
		if (status != sizeof(*rn))
			perror ("Error reading binary file");
		fclose (fneur);
	}
}

sauv_dec_neur ()
{
FILE *fneur;
int i, j;
	fneur = fopen (FNEUR, "w");
	if (fneur == NULL)
		perror ("Error opening ASCII file for writing");
	else
	{
		fprintf (fneur, " %d %d %d %d %d %d %d %d %d %d %d %d \n",
			rn->temps, rn->coef_rand, rn->par, rn->compar,
			rn->bas, rn->haut, rn->energie, rn->ergdyn,
			rn->seuil_global, rn->ligc, rn->colc, rn->neurc);
		FOR_NEUR (i)
		{
			fprintf (fneur, " %d %d %d %d \n",
				rn->excit[i], rn->pexcit[i], rn->var[i],
				rn->seuil[i]);
			FOR_NEUR (j)
				fprintf (fneur, " %d ", rn->connex[i][j]);
			fprintf (fneur, "\n");
		}
		fflush (fneur);
		fclose (fneur);
	}
}

charg_dec_neur ()
{
FILE *fneur;
int i, j;
int x;
	fneur = fopen (FNEUR, "r");
	if (fneur == NULL)
		perror ("Error opening ASCII file for reading");
	else
	{
#define RN(champ) fscanf (fneur, "%d", &x); rn->champ = x;
		RN (temps)
		RN (coef_rand)
		RN (par)
		RN (compar)
		RN (bas)
		RN (haut)
		RN (energie)
		RN (ergdyn)
		RN (seuil_global)
		RN (ligc)
		RN (colc)
		RN (neurc)
	}
	FOR_NEUR (i)
	{
		RN (excit[i])
		RN (pexcit[i])
		RN (var[i])
		RN (seuil[i])
		FOR_NEUR (j)
		{
			RN (connex[i][j])
		}
	}
	fclose (fneur);
}

sauv_neur ()
{
	if (sauv_dec)
		sauv_dec_neur ();
	else
		sauv_bin_neur ();
}

charg_neur ()
{
	if (sauv_dec)
		charg_dec_neur ();
	else
		charg_bin_neur ();
}

mut_neur (int n)
{
int i, j;
	FOR_NEUR(i) FOR_NEUR(j)
	{
		if (pof())
		{
			if (pof())
				rn->connex[i][j] += n;
			else
				rn->connex[i][j] -= n;
		}
	}
	maj_neur ();
}

loc_neur (char c)
{
int i, j;
	FOR_NEUR(i) FOR_NEUR(j)
		switch (c)
		{
			case '#':
				break;
			case '+':
				if (j-i==1 || j-i==-1 || j-i==N_COL || j-i==-N_COL
					|| i<N_COL || j<N_COL);
				else rn->connex[i][j] = 0;
				break;
			case '*':
				if (j-i==1 || j-i==-1
				 || j-i==N_COL || j-i==-N_COL
				 || j-i==N_COL-1 || j-i==N_COL+1
				 || j-i==-N_COL-1 || j-i==-N_COL+1
				 || i<N_COL || j<N_COL);
				else rn->connex[i][j] = 0;
				break;
		}
	maj_neur ();
}

sym_neur ()
{
int i, j;
	FOR_NEUR(i) FOR_NEUR(j)
		if (j<=i)
			rn->connex[i][j] += rn->connex[j][i];
		else
			rn->connex[i][j] = rn->connex[j][i];
	maj_neur ();
}

#define CMDN_GAUCHE 	'h'
#define CMDN_DROITE 	'l'
#define CMDN_HAUT 	'k'
#define CMDN_BAS 	'j'

#define CMDN_FIX 	'f'
#define CMDN_VAR 	'v'
#define CMDN_NM 	'w'

#define CMDN_REPOS 	'0'
#define CMDN_EXCIT 	'1'

#define CMDN_RECOMP	'r'
#define CMDN_PUNIT	'p'

#define CMDN_EVOL	','

#define CMDN_QUIT 	'Q'

#define CMDN_PAR        '|'
#define CMDN_SEQ 	'_'

#define CMDN_RREPOS	'O'
#define CMDN_REXCIT	'I'

#define CMDN_RVREPOS 	'%'
#define CMDN_RVEXCIT	'$'

#define CMDN_RECDYN	'R'
#define CMDN_PUNDYN	'P'

#define CMDN_SAUV	'S'
#define CMDN_CHARG	'C'

#define CMDN_AV 	'>'
#define CMDN_AR		'<'
#define CMDN_ECH	'E'

#define CMDN_MUT	'M'

#define CMDN_AA		'a'	/* augmenter coef_rand */
#define CMDN_DA		'A'	/* diminuer */

#define CMDN_DSG	't'	/* seuil global */
#define CMDN_ASG	'y'
#define CMDN_DSC	'T'	/* seuil local */
#define CMDN_ASC	'Y'

#define CMDN_AB		'b'	/* niveau bas */
#define CMDN_DB		'B'
#define CMDN_AH		'n'	/* niveau haut */
#define CMDN_DH		'N'

#define CMDN_RAB	'Z'	/* remplissage aleatoire : 1 neurone sur 2 au
					niveau bas (au repos) */
#define CMDN_RAH	'X'	/* 	niveau haut (excite) */
#define CMDN_RAC	'D'	/*	change */

#define CMDN_SUP	'3'
#define CMDN_SUPEG	'4'

#define CMDN_BIN	'5'
#define CMDN_DEC	'6'

#define CMDN_REAF	':'

#define CMDN_REP	'*'

#define CMDN_RND	'&'

#define CMDN_LOC	'@'

#define CMDN_MARQUE	'('
#define CMDN_COPIE	')'

#define CMDN_INIT	('R'-64)

#define CMDN_MAJ	'/'
#define CMDN_MAJMAN 	'{'
#define CMDN_MAJAUTO	'}'

#define CMDN_SYM 	'^'


top_neur ()
{
char c;
	c = getchar_raw ();
	while (c != CMDN_QUIT)
	{
		cmd_neur (c);
		c = getchar_raw ();
	}
}

cmd_neur (char c)
{
int i;
	switch (c)
	{
		case CMDN_GAUCHE:	depl_gauche();	break;
		case CMDN_DROITE:	depl_droite();	break;
		case CMDN_HAUT:		depl_haut();	break;
		case CMDN_BAS:		depl_bas();	break;
		case CMDN_FIX:		fix_neurc();	break;
		case CMDN_VAR:		var_neurc();	break;
		case CMDN_NM:		nm_neurc();	break;
		case CMDN_REPOS:	repos_neurc();	break;
		case CMDN_EXCIT:	excit_neurc();	break;

		case CMDN_RECOMP:
			recomp_neur (rn->rep);
		loc:
			loc_neur (rn->loc);
		maj:
			maj_aff_neur();
			break;
		case CMDN_PUNIT:
			recomp_neur (-rn->rep);
			goto loc;
		case CMDN_EVOL:
			evol_neur (1);
			aff_mod_neur();
			goto maj;
		case CMDN_PAR:
			rn->par = 1;
			goto maj;
		case CMDN_SEQ:
			rn->par = 0;
			goto maj;
		case CMDN_SUP:
			rn->compar--;
			goto maj;
		case CMDN_SUPEG:
			rn->compar++;
			goto maj;
		case CMDN_RREPOS:
			FOR_NEUR (i)
				rn->excit[i] = rn->bas;
		majneur:
			maj_neur();
			aff_neur();
			goto maj;
		case CMDN_REXCIT:
			FOR_NEUR (i)
				rn->excit[i] = rn->haut;
			goto majneur;
		case CMDN_RVREPOS:
			FOR_NEUR (i)
			if (rn->var[i])
				rn->excit[i] = rn->bas;
			goto majneur;
		case CMDN_RVEXCIT:
			FOR_NEUR (i)
			if (rn->var[i])
				rn->excit[i] = rn->haut;
			goto majneur;
		case CMDN_RECDYN:
			recomp_dyn_neur (rn->rep);
			goto loc;
		case CMDN_PUNDYN:
			recomp_dyn_neur (-rn->rep);
			goto loc;
		case CMDN_SAUV:
			sauv_neur();
			break;
		case CMDN_CHARG:
			charg_neur();
			aff_neur();
			goto maj;
		case CMDN_AV:
			av_neur();
			goto majneur;
		case CMDN_AR:
			ar_neur();
			goto majneur;
		case CMDN_ECH:
			ech_neur();
			goto majneur;
		case CMDN_RAB:
			rab_neur();
			goto majneur;
		case CMDN_RAH:
			rah_neur();
			goto majneur;
		case CMDN_AA:
			rn->coef_rand++;
			goto maj;
		case CMDN_DA:
			rn->coef_rand--;
			goto maj;
		case CMDN_AB:
			rn->bas++;
		majn:
			maj_neur();
			goto maj;
		case CMDN_DB:
			rn->bas--;
			goto majn;
		case CMDN_AH:
			rn->haut++;
			goto majn;
		case CMDN_DH:
			rn->haut--;
			goto majn;
		case CMDN_ASG:
			rn->seuil_global += rn->rep;
			goto majn;
		case CMDN_DSG:
			rn->seuil_global -= rn->rep;
			goto majn;
		case CMDN_ASC:
			rn->seuil[rn->neurc] += rn->rep;
			goto majn;
		case CMDN_DSC:
			rn->seuil[rn->neurc] -= rn->rep;
			goto majn;
		case CMDN_BIN:
			sauv_dec = 0;
			goto maj;
		case CMDN_DEC:
			sauv_dec = 1;
			goto maj;
		case CMDN_REAF:
			maj_neur();
			ty_cls();
			aff_neur();
			goto maj;
		case CMDN_REP:
			c = getchar_raw ();
			rn->rep = 10 * (c - '0') + getchar_raw() - '0';
			goto maj;
		case CMDN_MUT:
			mut_neur (rn->rep);
			loc_neur (rn->loc);
			goto maj;
		case CMDN_RND:
			init_rnd (getchar_raw());
			break;
		case CMDN_MARQUE:
			rn->marque = rn->neurc;
			break;
		case CMDN_COPIE:
			for (i=0; i<N_COL; i++)
				rn->excit[rn->neurc+i] = rn->excit[rn->marque+i];
		case CMDN_LOC:
			c = getchar_raw ();
			rn->loc = c;
			loc_neur (c);
			goto maj;
		case CMDN_INIT:
			init_neur ();
			ty_cls ();
			aff_neur ();
			goto maj;
		case CMDN_MAJ:
			{
			int maj1;
				maj1 = maj;
				maj = 1;
				maj_neur ();
				maj = maj1;
				maj_aff_neur ();
			}
			break;
		case CMDN_MAJAUTO:
			maj = 1;
			goto majn;
		case CMDN_MAJMAN:
			maj = 0;
			goto majn;
		case CMDN_SYM:
			sym_neur ();
			goto maj;

		default:
			;
	}
}

main ()
{
	init ();
	init_neur ();
	init_es_neur ();
	top_neur ();
	term_es_neur ();

}