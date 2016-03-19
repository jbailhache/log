/* C program translated from Prolog code */
#include "coroutin.h"
#include "expr.h"
#include "prolog.h"
pl_lettre_1 (struct coroutine *k, expr a0)
{
expr nx[MAX_NEW_CONS];
int pnx, i;
struct process_list *alt_process;
	pnx = 0;
	begin_decl ();
	decl_expr (&a0);
	for (i=0; i<MAX_NEW_CONS; i++)
		dle (nx[i]);
#ifdef TRACE
	printf ("\nlettre: a0 = "); print_expr (a0);
#endif
	if (alt (k, 1, 0))
	{
	/* clause */
		alt_process = getpl (k) -> alt;
#ifdef TRACE
		printf ("\n\ta0 = "); print_expr (a0);
#endif
		unify (k, nx[pnx++] = cons (symbol("a"),
			0), a0);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		unify (k, a0, nx[pnx++] = cons (symbol("a"),
			0));
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
#ifdef TRACE
		printf ("\n\ta0 = "); print_expr (a0);
#endif
	} else
	if (alt (k, 1, 0))
	{
	/* clause */
		alt_process = getpl (k) -> alt;
#ifdef TRACE
		printf ("\n\ta0 = "); print_expr (a0);
#endif
		unify (k, nx[pnx++] = cons (symbol("b"),
			0), a0);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		unify (k, a0, nx[pnx++] = cons (symbol("b"),
			0));
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
#ifdef TRACE
		printf ("\n\ta0 = "); print_expr (a0);
#endif
	} else
	if (alt (k, 1, 0))
	{
	/* clause */
		alt_process = getpl (k) -> alt;
#ifdef TRACE
		printf ("\n\ta0 = "); print_expr (a0);
#endif
		unify (k, nx[pnx++] = cons (symbol("c"),
			0), a0);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		unify (k, a0, nx[pnx++] = cons (symbol("c"),
			0));
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
#ifdef TRACE
		printf ("\n\ta0 = "); print_expr (a0);
#endif
	} else
	if (alt (k, 1, 0))
	{
	/* clause */
		alt_process = getpl (k) -> alt;
#ifdef TRACE
		printf ("\n\ta0 = "); print_expr (a0);
#endif
		unify (k, nx[pnx++] = cons (symbol("d"),
			0), a0);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		unify (k, a0, nx[pnx++] = cons (symbol("d"),
			0));
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
#ifdef TRACE
		printf ("\n\ta0 = "); print_expr (a0);
#endif
	} else
	if (alt (k, 1, 0))
	{
	/* clause */
		alt_process = getpl (k) -> alt;
#ifdef TRACE
		printf ("\n\ta0 = "); print_expr (a0);
#endif
		unify (k, nx[pnx++] = cons (symbol("e"),
			0), a0);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		unify (k, a0, nx[pnx++] = cons (symbol("e"),
			0));
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
#ifdef TRACE
		printf ("\n\ta0 = "); print_expr (a0);
#endif
	} else
	if (alt (k, 1, 0))
	{
	/* clause */
		alt_process = getpl (k) -> alt;
#ifdef TRACE
		printf ("\n\ta0 = "); print_expr (a0);
#endif
		unify (k, nx[pnx++] = cons (symbol("f"),
			0), a0);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		unify (k, a0, nx[pnx++] = cons (symbol("f"),
			0));
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
#ifdef TRACE
		printf ("\n\ta0 = "); print_expr (a0);
#endif
	} else
	if (alt (k, 1, 0))
	{
	/* clause */
		alt_process = getpl (k) -> alt;
#ifdef TRACE
		printf ("\n\ta0 = "); print_expr (a0);
#endif
		unify (k, nx[pnx++] = cons (symbol("g"),
			0), a0);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		unify (k, a0, nx[pnx++] = cons (symbol("g"),
			0));
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
#ifdef TRACE
		printf ("\n\ta0 = "); print_expr (a0);
#endif
	} else
	if (alt (k, 1, 0))
	{
	/* clause */
		alt_process = getpl (k) -> alt;
#ifdef TRACE
		printf ("\n\ta0 = "); print_expr (a0);
#endif
		unify (k, nx[pnx++] = cons (symbol("h"),
			0), a0);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		unify (k, a0, nx[pnx++] = cons (symbol("h"),
			0));
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
#ifdef TRACE
		printf ("\n\ta0 = "); print_expr (a0);
#endif
	} else
	if (alt (k, 1, 0))
	{
	/* clause */
		alt_process = getpl (k) -> alt;
#ifdef TRACE
		printf ("\n\ta0 = "); print_expr (a0);
#endif
		unify (k, nx[pnx++] = cons (symbol("i"),
			0), a0);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		unify (k, a0, nx[pnx++] = cons (symbol("i"),
			0));
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
#ifdef TRACE
		printf ("\n\ta0 = "); print_expr (a0);
#endif
	} else
	end (k);
	free_expr ();
}
pl_voyelle_1 (struct coroutine *k, expr a0)
{
expr nx[MAX_NEW_CONS];
int pnx, i;
struct process_list *alt_process;
	pnx = 0;
	begin_decl ();
	decl_expr (&a0);
	for (i=0; i<MAX_NEW_CONS; i++)
		dle (nx[i]);
#ifdef TRACE
	printf ("\nvoyelle: a0 = "); print_expr (a0);
#endif
	if (alt (k, 1, 0))
	{
	/* clause */
		alt_process = getpl (k) -> alt;
#ifdef TRACE
		printf ("\n\ta0 = "); print_expr (a0);
#endif
		unify (k, nx[pnx++] = cons (symbol("a"),
			0), a0);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		unify (k, a0, nx[pnx++] = cons (symbol("a"),
			0));
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
#ifdef TRACE
		printf ("\n\ta0 = "); print_expr (a0);
#endif
	} else
	if (alt (k, 1, 0))
	{
	/* clause */
		alt_process = getpl (k) -> alt;
#ifdef TRACE
		printf ("\n\ta0 = "); print_expr (a0);
#endif
		unify (k, nx[pnx++] = cons (symbol("e"),
			0), a0);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		unify (k, a0, nx[pnx++] = cons (symbol("e"),
			0));
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
#ifdef TRACE
		printf ("\n\ta0 = "); print_expr (a0);
#endif
	} else
	if (alt (k, 1, 0))
	{
	/* clause */
		alt_process = getpl (k) -> alt;
#ifdef TRACE
		printf ("\n\ta0 = "); print_expr (a0);
#endif
		unify (k, nx[pnx++] = cons (symbol("i"),
			0), a0);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		unify (k, a0, nx[pnx++] = cons (symbol("i"),
			0));
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
#ifdef TRACE
		printf ("\n\ta0 = "); print_expr (a0);
#endif
	} else
	end (k);
	free_expr ();
}
pl_consonne_1 (struct coroutine *k, expr a0)
{
expr nx[MAX_NEW_CONS];
int pnx, i;
struct process_list *alt_process;
	pnx = 0;
	begin_decl ();
	decl_expr (&a0);
	for (i=0; i<MAX_NEW_CONS; i++)
		dle (nx[i]);
#ifdef TRACE
	printf ("\nconsonne: a0 = "); print_expr (a0);
#endif
	if (alt (k, 1, 0))
	{
	/* clause */
	expr val_X, var_X;
		alt_process = getpl (k) -> alt;
		dle(val_X) dle(var_X)
		val_X=UNDEF; var_X=mk_var(&val_X);
#ifdef TRACE
		printf ("\n\ta0 = "); print_expr (a0);
#endif
		unify (k, var_X, a0);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		pl_lettre_1 (k, var_X);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		pl_non_voyelle_1 (k, var_X);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		unify (k, a0, var_X);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
#ifdef TRACE
		printf ("\n\ta0 = "); print_expr (a0);
#endif
	} else
	end (k);
	free_expr ();
}
pl_non_voyelle_1 (struct coroutine *k, expr a0)
{
expr nx[MAX_NEW_CONS];
int pnx, i;
struct process_list *alt_process;
	pnx = 0;
	begin_decl ();
	decl_expr (&a0);
	for (i=0; i<MAX_NEW_CONS; i++)
		dle (nx[i]);
#ifdef TRACE
	printf ("\nnon_voyelle: a0 = "); print_expr (a0);
#endif
	if (alt (k, 1, 0))
	{
	/* clause */
	expr val_X, var_X;
		alt_process = getpl (k) -> alt;
		dle(val_X) dle(var_X)
		val_X=UNDEF; var_X=mk_var(&val_X);
#ifdef TRACE
		printf ("\n\ta0 = "); print_expr (a0);
#endif
		unify (k, var_X, a0);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		pl_voyelle_1 (k, var_X);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		pl_cut_0 (k);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		pl_fail_0 (k);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		unify (k, a0, var_X);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
#ifdef TRACE
		printf ("\n\ta0 = "); print_expr (a0);
#endif
	} else
	if (alt (k, 1, 0))
	{
	/* clause */
	expr val_X, var_X;
		alt_process = getpl (k) -> alt;
		dle(val_X) dle(var_X)
		val_X=UNDEF; var_X=mk_var(&val_X);
#ifdef TRACE
		printf ("\n\ta0 = "); print_expr (a0);
#endif
		unify (k, var_X, a0);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		unify (k, a0, var_X);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
#ifdef TRACE
		printf ("\n\ta0 = "); print_expr (a0);
#endif
	} else
	end (k);
	free_expr ();
}
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
	expr val_X, var_X;
		alt_process = getpl (k) -> alt;
		dle(val_X) dle(var_X)
		val_X=UNDEF; var_X=mk_var(&val_X);
#ifdef TRACE
#endif
		pl_consonne_1 (k, var_X);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		pl_printexpr_1 (k, var_X);
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
