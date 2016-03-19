/* C program translated from Prolog code */
#include "coroutin.h"
#include "expr.h"
#include "prolog.h"
pl_goal_0 (struct coroutine *k)
{
expr nx[MAX_NEW_CONS];
int pnx, i;
struct process_list *alt_process;
	pnx = 0;
	begin_decl ();
	for (i=0; i<MAX_NEW_CONS; i++)
		dle (nx[i]);
#ifdef TRACE
#endif
	if (alt (k, 1, 0))
	{
	/* clause */
		alt_process = getpl (k) -> alt;
#ifdef TRACE
#endif
		pl_ccode_1 (k, { printf ("Bonjour !\n"); printf ("Rebonjour\n"); });
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
#ifdef TRACE
#endif
	} else
	end (k);
	free_expr ();
}

/* End of translation */
