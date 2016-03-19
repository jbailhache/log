
#include <stdio.h>

enum node { node_ap, node_transym, node_symbol, node_var, node_lambda,
	node_subst, node_axiom };

typedef struct dem *dem;

struct dem
{
	enum node _node;
	int _level;
	char *_name;
	dem _subdem[2], _left, _right;
	char _buf[10];
};

#define node(d) ((d)->_node)
#define sd0(d) ((d)->_subdem[0])
#define sd1(d) ((d)->_subdem[1])

#define MAX_DEMS 500

struct memory
{
	int ndems;
	int max_dems;
	struct dem dems[MAX_DEMS];
} _memory;

typedef struct memory *memory;

init (memory pm)
{
	pm->ndems = 0;
	pm->max_dems = MAX_DEMS;
}

dem mkdem (memory pm, enum node node, int level, char *name, dem sd0, dem sd1)
{
int i;
	for (i=0; i<pm->ndems; i++)
	{
		if (pm->dems[i]._node == node &&
		    pm->dems[i]._level == level &&
		    (pm->dems[i]._name == name || !strcmp (pm->dems[i]._name, name)) &&
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
	strcpy (pm->dems[pm->ndems]._buf, name);
	pm->dems[pm->ndems]._name = pm->dems[pm->ndems]._buf;
	pm->dems[pm->ndems]._subdem[0] = sd0;
	pm->dems[pm->ndems]._subdem[1] = sd1;
	pm->dems[pm->ndems]._left = NULL;
	pm->dems[pm->ndems]._right = NULL;

}

dem read (memory pm)
{

	return NULL;
}

print (memory pm, dem d)
{
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

		 var lambda subst axiom

	}

}



main ()
{
	init (&_memory);

}





