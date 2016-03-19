
#include <stdio.h>
#include <dos.h>

enum node { node_ap, node_transym, node_symbol, node_var, node_lambda,
	node_subst, node_axiom };

typedef struct dem *dem;

struct dem
{
	enum node _node;
	int _level;
	char *_name;
	dem _subdem[2], _lr[2], _value;
	char _buf[10];
};

#define node(d) ((d)->_node)
#define level(d) ((d)->_level)
#define name(d) ((d)->_name)
#define sd0(d) ((d)->_subdem[0])
#define sd1(d) ((d)->_subdem[1])

#define ap(f,a) mkdem (node_ap, 0, NULL, f, a)
#define lambda(a) mkdem (node_lambda, 0, NULL, a, NULL)
#define var(n) mkdem (node_var, n, NULL, NULL, NULL)

#define MAX_DEMS 500

struct memory
{
	int ndems;
	int max_dems;
	struct dem dems[MAX_DEMS];
} _memory;

typedef struct memory *memory;

char mygetchar ()
{
char c;
	_AH = 1;
	geninterrupt (0x21);
	c = _AL;
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

lr (int r, dem d)
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

dem _read (memory pm)
{
int c;
dem sd0, sd1, d;
enum node node;
char buf[30];

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
		default :
			if ((c >= '0') && (c <= '9'))
				d = _mkdem (pm, node_var, c-'0', NULL, NULL, NULL);
			else
			{
				buf[0] = c;
				buf[1] = 0;
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
		printf ("%s", d->_name);
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
			printf ("%s", d->_name);
			break;

		case node_var :
			printf ("%d", d->_level);
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

main ()
{
dem d, l, r;
	printf ("%x %x %x\n", _read, &_memory, MAX_DEMS);
	init (&_memory);
	for (;;)
	{
		printf ("\n? ");
		d = _read (&_memory);
		printf ("\n  ");
		print (d);
		l = lr (0, d);
		printf ("\n");
		print (l);
		printf (" = ");
		r = lr (1, d);
		print (r);

	}
}





