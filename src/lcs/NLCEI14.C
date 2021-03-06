
#include <stdio.h>
#include <errno.h>
#include <io.h>
#include <fcntl.h>
#include <dos.h>

#define DEM dem

#define _read _read_dem

enum node { node_ap, node_transym, node_symbol, node_var, node_lambda,
	node_subst, node_axiom };

typedef struct dem *dem;

struct dem
{
	enum node _node;
	int _level;
	char *_name;
	dem _subdem[2], _lr[2], _value, _red[2];
	char _buf[10];
};

#define node(d) ((d)->_node)
#define level(d) ((d)->_level)
#define name(d) ((d)->_name)
#define subdem(n,d) ((d)->_subdem[(n)])
#define sd0(d) ((d)->_subdem[0])
#define sd1(d) ((d)->_subdem[1])
#define fnc(d) ((d)->_subdem[0])
#define arg(d) ((d)->_subdem[1])

#define ap(f,a) mkdem (node_ap, 0, NULL, f, a)
#define lambda(a) mkdem (node_lambda, 0, NULL, a, NULL)
#define var(n) mkdem (node_var, n, NULL, NULL, NULL)
#define mksubst(a,b) mkdem (node_subst, 0, NULL, a, b)

#define MAX_DEMS 500

struct memory
{
	int ndems;
	int max_dems;
	struct dem dems[MAX_DEMS];
} _memory;

typedef struct memory *memory;

int mygetchar1 ()
{
char c;
int status;
/*
	_AH = 1;
	geninterrupt (0x21);
	c = _AL;
	return c;
*/
	status = read (0, &c, 1);
	if (status == 1)
		return c;
	else
		return -1;
}

#define HANDLE

#ifdef FCB
struct myfcb
{
	char lecteur;
	char nom[8];
	char ext[3];
	int bloc;
	int taille_enreg;
	long taille_fichier;
	int date;
	int heure;
	int reserve;
	char nec;
	long ner;

};

movename (char *filename, char *dest)
{
int i;
char *s, *d;
	s = filename;
	d = dest;
	memcpy (dest, "           ", 11);
	for (;;)
	{
		if (*s == 0)
			break;
		if (*s == '.')
		{
			d = dest+8;
			s++;
		}
		else
		{
			*d++ = *s++;
		}
	}
}

int myopen (struct myfcb *fcb, char *filename)
{
int status;
	movename (filename, fcb->nom);
	fcb->taille_enreg = 1;
	_AH = 15;
	_DX = fcb;
	geninterrupt (0x21);
	status = _AL;
	return status;
}

int myclose (struct myfcb *fcb)
{
int status;
	_AH = 16;
	_DX = fcb;
	geninterrupt (0x21);
	status = _AL;
	return status;
}

int myfgetc (struct myfcb *fcb)
{
int c;
int status;
char *adr;
/*
char buf[2048];
	_AH = 0x1A;
	_DX = buf;
	geninterrupt (0x21);
*/
	_AH = 0x14;
	_DX = fcb;
	geninterrupt (0x21);
	status = _AL;
	if (status == 0)
	{
		_AH = 0x2F;
		geninterrupt (0x21);
		adr = _BX;
		return *adr;
	}
	else
		return -1;
}
#endif

#ifdef HANDLE

struct myfcb
{
	int fh;
};

int myopen (struct myfcb *fcb, char *filename)
{
	fcb->fh = open (filename, O_RDONLY);
	return errno;
}

int myclose (struct myfcb *fcb)
{
	close (fcb->fh);
	return errno;
}

int myfgetc (struct myfcb *fcb)
{
char c;
int status;
	status = read (fcb->fh, &c, 1);
	if (status == 1)
		return c;
	else
		return -1;
}




#endif

#ifdef STD

struct myfcb
{
	FILE *f;
};

int myopen (struct myfcb *fcb, char *filename)
{
	fcb->f = fopen (filename, "r");
	return errno;
}

int myclose (struct myfcb *fcb)
{
	fclose (fcb->f);
	return errno;
}

int myfgetc (struct myfcb *fcb)
{
int status;
char c;
	status = fread (&c, 1, 1, fcb->f);
	if (status == 1)
		return c;
	else
		return -1;
}

#endif

/* typedef FILE *desc; */

typedef struct myfcb desc;

desc files[30];

int file_level = 0;

int mygetchar2 ()
{
int c;

debut:
	if (file_level <= 0)
		c = mygetchar1 ();
	else
	{
		c = myfgetc (files+file_level);
		if (c == EOF)
		{
			myclose (files+file_level);
			file_level--;
			goto debut;
		}
	}
	return c;
}

int mygetchar ()
{
char filename[200];
int i;
int c;
FILE *f;
struct myfcb fcb;
int status;

debut:
	c = mygetchar2 ();
	if (c == '"')
	{
		for (i=0; ; i++)
		{
			c = mygetchar2 ();
			if (c == '"')
			{
				filename[i] = 0;
				break;
			}
			filename[i] = c;
		}
		/*
		f = fopen (filename, "r");
		if (f == NULL)
		{
			perror (filename);
		}
		else
		{
			file_level++;
			files[file_level] = f;
		}
		*/
		file_level++;
		/*
		files[file_level].taille_enreg = 1;
		movename (filename, files[file_level].nom);
		*/
		status = myopen (files+file_level, filename);
		if (status)
		{
			perror (filename);
			file_level--;
		}
		goto debut;
	}
	return c;
}

init (memory pm)
{
	pm->ndems = 0;
	pm->max_dems = MAX_DEMS;
}

dem _mkdem (memory pm, enum node node, int level, char *name, dem sd0, dem sd1)
{
int i;
	for (i=0; i<pm->ndems; i++)
	{
		if ((name != NULL) && !strcmp(name,pm->dems[i]._name))
			return pm->dems+i;
		if (pm->dems[i]._node == node &&
		    pm->dems[i]._level == level &&
		    ((node != node_symbol) || (pm->dems[i]._name == name || !strcmp (pm->dems[i]._name, name))) &&
		    pm->dems[i]._subdem[0] == sd0 &&
		    pm->dems[i]._subdem[1] == sd1)
			return pm->dems+i;
	}
	if (pm->ndems >= pm->max_dems)
	{
		printf ("Memory full\n");
		exit (0);
	}
	pm->dems[pm->ndems]._node = node;
	pm->dems[pm->ndems]._level = level;
	if (name == NULL)
	{
		pm->dems[pm->ndems]._name = NULL;
		pm->dems[pm->ndems]._buf[0] = 0;
	}
	else
	{
		strcpy (pm->dems[pm->ndems]._buf, name);
		pm->dems[pm->ndems]._name = pm->dems[pm->ndems]._buf;
	}
	pm->dems[pm->ndems]._subdem[0] = sd0;
	pm->dems[pm->ndems]._subdem[1] = sd1;
	pm->dems[pm->ndems]._lr[0] = NULL;
	pm->dems[pm->ndems]._lr[1] = NULL;
	pm->dems[pm->ndems]._value = NULL;
	pm->dems[pm->ndems]._red[0] = NULL;
	pm->dems[pm->ndems]._red[1] = NULL;
	return pm->dems+pm->ndems++;
}

dem mkdem (enum node node, int level, char *name, dem sd0, dem sd1)
{
	return _mkdem (&_memory, node, level, name, sd0, sd1);
}

dem shift (int m, int n, dem x)
{
    if (x == NULL)
        return NULL;
    if (node(x) == node_var)
        if (level(x) >= m)
	    return var (level(x)+n);
        else
            return x;
    if (node(x) == node_lambda)
	return lambda (shift (m+1, n, sd0(x)));
    if (node(x) == node_var || node(x) == node_symbol)
	return mkdem (node(x), level(x), name(x),
	       NULL, NULL);
    return mkdem (node(x), level(x), NULL,
	   shift (m, n, sd0(x)),
	   shift (m, n, sd1(x)));
}

dem subst (int n, dem y, dem z)
{
    if (y == NULL)
        return NULL;
    if (node(y) == node_var)
    {
        if (level(y) == n)
        	return z;
	if (level(y) > n)
		return mkdem (node_var, level(y)-1, NULL, NULL, NULL);
    }
    if (node(y) == node_ap)
	return ap (subst (n, sd0(y), z),
		   subst (n, sd1(y), z));
    if (node(y) == node_lambda)
	return lambda (subst (n+1, sd0(y), shift (0, 1, z)));
    /* return y; */
    return mkdem (node(y), level(y), name(y),
			subst (n, sd0(y), z),
			subst (n, sd1(y), z));
}

dem DBname (int n, dem x, dem y)
{
    if (y == NULL)
        return NULL;
    if (x == y)
	return var(n);
    if (node(y) == node_ap)
	return ap (DBname (n, x, sd0(y)),
		   DBname (n, x, sd1(y)));
    if (node(y) == node_lambda)
	return lambda (DBname (n+1, x, sd0(y)));
    /* return y; */
    return mkdem (node(y), level(y), name(y),
	DBname (n, x, sd0(y)),
	DBname (n, x, sd1(y)));
}

dem vlambda (dem x, dem y)
{
	return lambda (DBname (0, x, y));
}

#if 0
DEM DB_lambda (DEM x, DEM y)
{
    return DBLambda (DBname (0, x, y));
}

DEM dbextens (DEM x, DEM y)
{
    if (node(y) == _ap && subdem(1,y) == x && !in (x, subdem(0,y)))
        return subdem(0,y);
    return lambda (x, y);
}
#endif

#define left(d) lr (0, d)
#define right(d) lr (1, d)

dem lr (int r, dem d)
{
dem d1;
	if (d->_lr[r] != NULL)
		return d->_lr[r];
	switch (node(d))
	{
		case node_ap :
			d1 = mkdem (node_ap, 0, NULL, lr (r, sd0(d)), lr (r, sd1(d)));
			break;
		case node_transym :
			if (lr (0, sd0(d)) == lr (0, sd1(d)))
				d1 = lr (1, d->_subdem[r]);
			else
				d1 = mkdem (node_var, 0, NULL, NULL, NULL);
				/* d1 = lr (r, sd0(d)); */
			break;
		case node_symbol :
		case node_var :
			d1 = d;
			break;
		case node_lambda :
			d1 = mkdem (node_lambda, 0, NULL, lr (r, sd0(d)), NULL);
			break;
		case node_subst :
			if (r == 0)
				d1 = mkdem (node_ap, 0, NULL,
					mkdem (node_lambda, 0, NULL, sd0(d), NULL),
					sd1(d));
			else
				d1 = subst (0, sd0(d), sd1(d));
			break;
		case node_axiom :
			d1 = d->_subdem[r];
			break;
		default :
			d1 = d;
			break;
	}
	d->_lr[r] = d1;
	return d1;
}

int atom (dem d)
{
	switch (node(d))
	{
		case node_ap:
			return 0;
		default:
			return 1;
        }
}

dem transym1 (dem ab, dem ac)
{
dem bc;
	bc = mkdem (node_transym, 0, NULL, ab, ac);
	return bc;
}

dem sym (dem ab)
{
dem aa, ba;
	aa = left (ab);
	ba = transym1 (ab, aa);
	return ba;
}

dem trans (dem ab, dem bc)
{
dem ba, ac;
	ba = sym (ab);
	ac = transym1 (ba, bc);
	return ac;
}

DEM used = NULL;

#define trace_dem(s,d) /* printf ("trace : %s ", s); printlr (d); printf ("\n"); */


dem no_red[60];

int nnr = 0;

int flags = 0;

#define FLAG_RED_LAMBDA 1



DEM red1 (DEM x, int red_arg);

DEM red (DEM x)
{
DEM r;
	r = red1 (x, 1);
	return r;
}

DEM red2 (DEM x, int red_arg);

DEM red1 (DEM x, int red_arg)
{
DEM d;
int i;
    trace_dem ("red1", x);
    if (red_arg != 0 && red_arg != 1)
        printf (" ERROR %d ", red_arg);

    for (i=0; i<nnr; i++)
    {
        /* printf ("i=%d\n", i); */
        trace_dem ("", no_red[i]);
        trace_dem ("", x);
        if (no_red[i] == x)
            return x;
    }
    if (used == NULL && x->_red[red_arg] != NULL)
        return x->_red[red_arg];
    else
    {
	d = red2 (x, red_arg);
	trace_dem ("red1 ", x);
        trace_dem ("red1 returns", d);
        if (used == NULL)
        	x->_red[red_arg] = d;
        return d;
    }
}

DEM red3 (DEM x, int red_arg);

DEM red2 (DEM x, int red_arg)
{
DEM t1, t2, t3, t4;
int i;
    t1 = red3 (x, red_arg);
    trace_dem ("red3 ", x);
    trace_dem (" = ", t1);
    for (;;)
    {
        for (i=0; i<nnr; i++)
	{
            trace_dem ("", no_red[i]);
            trace_dem ("", right(t1));
            if (no_red[i] == right(t1))
                return t1;
        }
	t2 = red3 (right(t1), red_arg);
	trace_dem ("red3 ", right(t1));
	trace_dem (" = ", t2);
        if (left(t2) == right(t2))
                return t1;
	trace_dem ("trans of", t1);
	trace_dem ("and", t2);
	t4 = trans (t1, t2);
	trace_dem ("is", t4);
	if (left(t4) == left(t1))
		t1 = t4;
	else
		return t1;
    }
}

DEM red3 (DEM x, int red_arg)
{
DEM t, t1, t2, t3, t4, t5;
int i;
	trace_dem ("red3", x);
        for (i=0; i<nnr; i++)
        {
            trace_dem ("", no_red[i]);
            trace_dem ("", x);
            if (no_red[i] == x)
                return x;
        }

        if (used != NULL && x == left(used))
            return used;

	if ((flags & FLAG_RED_LAMBDA) && node(x) == node_lambda)
        {
	    t1 = red1 (sd0(x), red_arg);
            trace_dem ("DBLambda", t1);
	    t2 = lambda (t1);
            trace_dem ("DBLambda", t2);
            return t2;
        }
	if (atom(x))
            return x;
	if (node(fnc(x)) == node_lambda)
        {
	    return mksubst (subdem(0,fnc(x)), arg(x));
        }
        if (used == NULL && atom (fnc(x)))
	{
                t1 = fnc(x);
                if (red_arg)
                    t2 = red1 (arg(x), red_arg);
                else
                    t2 = arg(x);
		t = ap (t1, t2);
		return t;
	}
	t1 = red1 (fnc(x), red_arg);
        if (red_arg)
        	t2 = red1 (arg(x), red_arg);
        else
            t2 = arg(x);
	t3 = ap (t1, t2);
	if (left(t3) == right(t3))
		return t3;
	t4 = red1 (right(t3), red_arg);
	if (left(t4) == right(t4))
		return t3;
	t = trans (t3, t4);
	return t;
}

/*
    d : a=b
    red a: a=c
    red b: b=d
    reduc d: c=d
*/
DEM reduc (DEM ab, int red_arg)
{
DEM ac, bd, cb, cd;
    ac = red1 (left(ab), red_arg);
    bd = red1 (right(ab), red_arg);
    cb = transym1 (ac, ab);
    cd = trans (cb, bd);
    return cd;
}

#if 0
DEM redu1 (DEM x);

DEM redu (DEM x)
{
int forme_Sfg;
DEM rf, ra, f1, a1, x1, r1, r2, t1, t2, t3, t4, t5, t6, r, rr1;
	trace_dem ("redu", x);
	if (atom(x))
        {
                trace_dem ("atom: return", x);
		return x;
        }
        trace_dem ("not atom", x);
        rf = redu (fnc(x)); trace_dem ("", rf);
        ra = redu (arg(x)); trace_dem ("", ra);
	f1 = right (rf); trace_dem ("", f1);
	a1 = right (ra); trace_dem ("", a1);
        trace_dem ("", x);
        if (fnc(x)==f1 && arg(x)==a1)
	{
		r1 = redu1 (x); trace_dem ("", r1);
                rr1 = right(r1); trace_dem ("", rr1);
		if (rr1 == x)
                {
                        trace_dem ("return", r1);
			return r1;
                }
                trace_dem ("", rr1);
		r2 = redu (rr1); trace_dem ("", r2);
		if (left(r2) == right(r2))
			return r1;
		if (left(r1) == right(r1))
			return r2;
		r = trans (r1, r2); trace_dem ("", r);
		return r;
	}
	x1 = ap (f1, a1); trace_dem ("", x1);
	r1 = redu1 (x1); trace_dem ("", r1);
	if (right(r1) == x)
		r2 = r1;
	r2 = redu (right(r1)); trace_dem ("", r2);
	t3 = ap (rf, ra); trace_dem ("", t3);
	t4 = trans (t3, r1); trace_dem ("", t4);
	t5 = trans (t4, r2); trace_dem ("", t5);
	return t5;
}

DEM redu1 (DEM x)
{
DEM t1, t2, t3, t4, t5, t6, t7, t8, a1, a2;
int forme_SKfg;
        trace_dem ("redu1", x);
        a1 = Ext4;
        a2 = Ext5;
        forme_SKfg = !atom(x) && !atom(fnc(x))
                        && fnc(fnc(x))==S
                        && !atom(arg(fnc(x))) 
                        && fnc(arg(fnc(x)))==K;
        if (forme_SKfg && arg(x)==I)
	{
	/* S(Kf)I = I */
                trace_dem ("S(Kf)I", x);
                t1 = arg(arg(fnc(x)));  trace_dem ("", t1);
                t2 = ap (a1, t1);       trace_dem ("", t2);
                t3 = defI (t1);         trace_dem ("", t3);
                t4 = trans (t2, t3);    trace_dem ("", t4);
                t5 = red (left(t4));    trace_dem ("", t5);
                t7 = transym1 (t5, t4);  trace_dem ("", t7);
		return t7;
	}
        if (forme_SKfg && !atom(arg(x)) && fnc(arg(x))==K)
	{
	/* S(Kf)(Ka) = K(fa) */
                trace_dem ("S(Kf)(Ka)", x);
                t1 = arg(arg(fnc(x)));  /* trace_dem ("", t1); */
                t2 = arg(arg(x));       /* trace_dem ("", t2); */
                t3 = app(a2, t1, t2);   /* trace_dem ("", t3); */
                t4 = red (left(t3));    /* trace_dem ("", t4); */
                t6 = transym1 (t4, t3);  /* trace_dem ("", t6); */
                t7 = red (right(t3));   /* trace_dem ("", t6); */
                t8 = trans (t6, t7);    /* trace_dem ("", t8); */
		return t8;
	}
	return x;
}

#endif



dem _read (memory pm)
{
int c;
dem sd0, sd1, d;
enum node node;
char buf[30];
int i;
dem used1;

debut:
	/* c = getchar (); */
	/* c = fgetc (stdin); */
	c = mygetchar ();
/*
	pm = &_memory;
	_memory.max_dems = MAX_DEMS;
*/
	switch (c)
	{
		case '.':
			exit (0);
		case '-':
			node = node_ap;
		read01:
			sd0 = _read (pm);
			sd1 = _read (pm);
			d = _mkdem (pm, node, 0, NULL, sd0, sd1);
			break;
		case '/':
			node = node_transym;
			goto read01;
		case '%':
			node = node_subst;
			goto read01;
		case '#':
			node = node_axiom;
			goto read01;
		case '\\':
			sd0 = _read (pm);
			d = _mkdem (pm, node_lambda, 0, NULL, sd0, NULL);
			break;
		case '^' :
			sd0 = _read (pm);
			sd1 = _read (pm);
			d = vlambda (sd0, sd1);
			break;
		case ' ' :
		case '\t' :
		case '\n' :
		case '\r' :
		case 0 :
			goto debut;
		case ':' :
			sd0 = _read (pm);
			sd1 = _read (pm);
			sd0->_value = sd1;
			sd1->_name = sd0->_name;
			d = sd1;
			break;
		case '<' :
			sd0 = _read (pm);
			d = lr (0, sd0);
			break;
		case '>' :
			sd0 = _read (pm);
			d = lr (1, sd0);
			break;
		case '$' :
			sd0 = _read (pm);
			d = red (sd0);
			break;
		case '@' :
			sd0 = _read (pm);
			d = reduc (sd0, 1);
			break;
		case '`':
		    used1 = used;
		    used = _read (pm);
		    d = _read (pm);
                    used = used1;
		    break;

		default :
			if ((c >= '0') && (c <= '9'))
				d = _mkdem (pm, node_var, c-'0', NULL, NULL, NULL);
			else
			{
				buf[0] = c;
				for (i=1; ; i++)
				{
					c = mygetchar ();
					if (c == ' ' || c == '\t' || c == '\n' || c == '\r')
					{
						buf[i] = 0;
						break;
					}
					buf[i] = c;
				}
				/*buf[1] = 0;*/
				d = _mkdem (pm, node_symbol, 0, buf, NULL, NULL);
				if (d->_value != NULL)
					d = d->_value;
			}
	}
	return d;
}


print (dem d)
{
	if (d == NULL)
	{
		printf ("<NULL>");
		return;
	}
	if (d->_name != NULL)
	{
		printf ("%s ", d->_name);
		return;
	}
	switch (node(d))
	{
		case node_ap :
			printf ("-");
		sd01:
			print (sd0(d));
			print (sd1(d));
			break;

		case node_transym :
			printf ("/");
			goto sd01;

		case node_symbol :
			printf ("%s ", d->_name);
			break;

		case node_var :
			printf ("%d ", d->_level);
			break;

		case node_lambda :
			printf ("\\");
			print (sd0(d));
			break;

		case node_subst :
			printf ("%");
			goto sd01;

		case node_axiom :
			printf ("#");
			goto sd01;

		default :
			printf ("?%d,", node(d));
			goto sd01;
	}
}

printlr (dem x)
{
	printf (" ");
	print (x);
	printf (" : ");
	print (left(x));
	printf (" = ");
	print (right(x));
	printf (" ");
}

main ()
{
dem d, l, r;
char c;
	/* read (0, &c, 1); */
	printf ("%x %x %x\n", _read, &_memory, MAX_DEMS);
	init (&_memory);
	for (;;)
	{
		printf ("\n? ");
		d = _read (&_memory);
		printf ("\n");
		printlr (d);
		/*
		printf ("\n  ");
		print (d);
		l = lr (0, d);
		printf ("\n");
		print (l);
		printf (" = ");
		r = lr (1, d);
		print (r);
		*/
	}
}



/*
main ()
{
int c;
	for (;;)
	{
		c = getchar ();
		if (c == EOF)
			break;
		putchar (c);
	}
}
*/




