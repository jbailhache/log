
#ifdef VAX_C

#include <ssdef.h>
#include <descrip.h>

#endif

#include <stdio.h>

#define init_desc(desc,str,len) \
{ \
    desc.dsc$b_class = DSC$K_CLASS_S; \
    desc.dsc$b_dtype = DSC$K_DTYPE_T; \
    desc.dsc$a_pointer = str; \
    desc.dsc$w_length = len; \
}

#define APPLY(pr,c) ( ((pr)->f) ((pr)->p, c) )

typedef struct get_fnct
{
	int (*f) (/* void *p */);
	void *p;
} get_fnct;

typedef struct put_fnct
{
	void (*f) (/* void *p, char c */);
	void *p;
} put_fnct;

#define cget(get) ((*((get)->f))((get)->p))
#define cput(put,c) ((*((put)->f))((put)->p,c))

#include <stdio.h>

struct param_file
{
	FILE *fd;
};

struct param_str
{
    char *s;
};

extern int f_get_file (struct param_file *);
extern int f_put_file (struct param_file *, char);

sput (struct put_fnct *, char *);
/*
#include "stream.h"
*/
sput (struct put_fnct *put, char *s)
{
	while (*s)
		cput (put, *s++);
}

int f_get_file (struct param_file *p)
{
	return getc (p->fd);
}

int f_put_file (struct param_file *p, char c)
{
	return putc (c, p->fd);
}

int f_get_str (struct param_str *p)
{
char c;
    c = *((p->s)++);
    if (c == 0)
	return EOF;
    return c;
}

int f_put_str (struct param_str *p, char c)
{
    return *((p->s)++) = c;
}

struct get_fnct *mk_get (int (*f) (), void *p)
{
struct get_fnct *get;
    get = malloc (sizeof(*get));
    if (get == NULL)
    {
	perror ("malloc");
	exit (-1);
    }
    get->f = f;
    get->p = p;
    return get;
}

struct put_fnct *mk_put (int (*f) (), void *p)
{
struct put_fnct *put;
    put = malloc (sizeof(*put));
    if (put == NULL)
    {
	perror ("malloc");
	exit (-1);
    }
    put->f = f;
    put->p = p;
    return put;
}

struct put_fnct *mk_get_str (char *s)
{
struct param_str *p;
    p = malloc (sizeof (*p));
    if (p == NULL)
    {
	perror ("malloc");
	exit (-1);
    }
    p->s = s;
    return mk_get (f_get_str, p);

}

struct put_fnct *mk_put_str (char *s)
{
struct param_str *p;
    p = malloc (sizeof (*p));
    if (p == NULL)
    {
	perror ("malloc");
	exit (-1);
    }
    p->s = s;
    return mk_put (f_put_str, p);

}

typedef struct
{
    struct put_fnct *put1, *put2;
} *param_tee;

int f_tee (param_tee p_tee, char c)
{
    APPLY (p_tee->put1, c);
    APPLY (p_tee->put2, c);
    return c;
}
 
struct put_fnct *tee (struct put_fnct *put1, struct put_fnct *put2)
{
param_tee p_tee;
    p_tee = malloc (sizeof(*p_tee));
    if (p_tee == NULL)
    {
	perror ("malloc");
	exit (-1);
    }
    p_tee->put1 = put1;
    p_tee->put2 = put2;
    return mk_put (f_tee, p_tee);
}

int f_sink (void *p, char c)
{
    return c;
}


typedef struct
{
    int (*f) (void *, char);
    void *p;
} *pred_car;

pred_car creer_pred_car (int (*f) (), void *p)
{
pred_car pred;
    pred = malloc (sizeof (*pred));
    if (pred == NULL)
    {
	perror ("malloc");
	exit (-1);
    }
    pred->f = f;
    pred->p = p;
    return pred;
}

int carac (char *s, int c)
{
    return (c == *s);
}

int non_car (char *s, int c)
{
    return (c != *s && c != EOF);
}

int quelconque (void *p, int c)
{
    return (c!=EOF);
}

enum type_filtre
{
    FILTRE_VIDE, 
    FILTRE_CAR,
    FILTRE_SEQ,
    FILTRE_ALT,
    FILTRE_REP,
    FILTRE_VAR,
    FILTRE_NON,
    FILTRE_COND
};

typedef struct filtre_info *filtre;

struct filtre_info
{
    enum type_filtre type;
    pred_car pred;
    char *s;
    filtre f1, f2, f3;
};

filtre creer_filtre (enum type_filtre type,
    pred_car pred,
    char *s,
    filtre f1, filtre f2, filtre f3)
{
filtre f;
    f = malloc (sizeof (*f));
    if (f == NULL)
    {
	perror ("malloc");
	exit (-1);
    }
    f->type = type;
    f->pred = pred;
    f->s = s;
    f->f1 = f1;
    f->f2 = f2;
    f->f3 = f3;
    return f;
}

#define F_VIDE creer_filtre (FILTRE_VIDE, NULL, NULL, NULL, NULL, NULL)

#define F_CAR(p) creer_filtre (FILTRE_CAR, p, NULL, NULL, NULL, NULL)

#define F_VAR(s,f) creer_filtre (FILTRE_VAR, NULL, s, f, NULL, NULL)

#define F_SEQ(f1,f2) creer_filtre (FILTRE_SEQ, NULL, NULL, f1, f2, NULL)

#define F_ALT(f1,f2) creer_filtre (FILTRE_ALT, NULL, NULL, f1, f2, NULL)

#define F_REP(f) creer_filtre (FILTRE_REP, NULL, NULL, f, NULL, NULL)

#define F_NON(f) creer_filtre (FILTRE_NON, NULL, NULL, f, NULL, NULL)

#define F_COND(c,a,b) creer_filtre (FILTRE_COND, NULL, NULL, c, a, b)

int filtrer (int *pc, struct get_fnct *get, struct put_fnct *put, filtre f)
{
int c;
int status;
struct put_fnct *put1, *putstr;

    switch (f->type)
    {
	case FILTRE_VIDE:
	    return 1;
	case FILTRE_CAR:
	    /*
	    c = cget (get);
	    if (c != EOF && APPLY (f->pred, c))
	    */
	    if (APPLY (f->pred, *pc))
	    {
		cput (put, *pc);
		*pc = cget (get);
		return 1;
	    }
	    else
		return 0;
	case FILTRE_VAR:
	    /* return filtrer (get, tee (put, mk_put(f_put_str,f->s)), f->f1);*/
	    put1 = tee (put, putstr=mk_put_str(f->s));
	    status = filtrer (pc, get, put1, f->f1);
	    cput (putstr, 0);
	    return status;
	case FILTRE_SEQ:
	    if (!filtrer (pc, get, put, f->f1))
		return 0;
	    return filtrer (pc, get, put, f->f2);
	/* case FILTRE_ALT: */
	case FILTRE_COND:
	    /*
	    c = cget (get);
	    cput (put, c);
	    if (APPLY (f->pred, c))
	    */
	    if (filtrer (pc, get, put, f->f1))
		return filtrer (pc, get, put, f->f2);
	    else
		return filtrer (pc, get, put, f->f3);
	case FILTRE_NON:
	    return !filtrer (pc, get, put, f->f1);
	case FILTRE_REP:
	    while (filtrer (pc, get, put, f->f1));
	    return 1;

    }
}

int decomp_nom (char *noms, char *nom, char *prenom)
{
#if 0
    return filtrer (/*mk_get(f_get_str,noms)*/
			mk_get_str(noms), mk_put(f_sink,NULL),
	F_SEQ (F_VAR (nom, F_REP (F_CAR (creer_pred_car(non_car," ")))),
	    F_SEQ (F_CAR (creer_pred_car(carac," ")),
		F_SEQ (F_REP (F_CAR (creer_pred_car(non_car,","))),
		    F_SEQ (F_CAR (creer_pred_car(carac,","),
			F_SEQ (F_CAR (creer_pred_car(carac," ")),
			    F_VAR (prenom,
				F_CAR (creer_pred_car(quelconque,NULL)))
				) ) ) ) ) );
#endif
filtre f[50];
struct get_fnct *get;
int c;
    f[1] = F_VAR (nom, F_REP (F_CAR (creer_pred_car(non_car," "))));
    f[2] = F_CAR (creer_pred_car(carac," "));
    f[3] = F_REP (F_CAR (creer_pred_car(non_car,",")));
    f[4] = F_CAR (creer_pred_car(carac,","));
    f[5] = F_CAR (creer_pred_car(carac," "));
    f[6] = F_VAR (prenom, F_REP (F_CAR (creer_pred_car(quelconque,NULL))));
    f[7] = F_SEQ (f[1], F_SEQ (f[2], F_SEQ (f[3], F_SEQ (f[4], F_SEQ (f[5],
	    f[6])))));
    get = mk_get_str (noms);
    c = cget (get);
    return filtrer (&c, get, mk_put(f_sink,NULL), f[7]);



}

#ifdef VAX_C

int decompose_nom (struct dsc$descriptor_s *desc_noms,
    struct dsc$descriptor_s *desc_prenom)
{
char noms[50], nom[50], prenom[50];
int flag;

    strncpy (noms, desc_noms->dsc$a_pointer, desc_noms->dsc$w_length);
    noms[desc_noms->dsc$w_length] = 0;
    return decomp_nom (noms, nom, prenom);
}
#endif

main ()
{
char noms[200], nom[200], prenom[200];
int status;

    printf ("f_sink = %d\n", f_sink);
    printf ("f_put_str = %d\n", f_put_str);
    printf ("f_tee = %d\n", f_tee);

    printf ("Nom ? ");
    gets (noms);

    status = decomp_nom (noms, nom, prenom);

    if (status)
	printf ("Nom = <%s>, prenom = <%s>\n", nom, prenom);
    else
	printf ("non decomposable\n");
    
}
