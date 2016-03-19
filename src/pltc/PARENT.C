/* C program translated from Prolog code */
#include "coroutin.h"
#include "expr.h"
#include "prolog.h"
pl_grandparent_2 (struct coroutine *k, expr a0, expr a1)
{
expr nx[MAX_NEW_CONS];
int pnx, i;
struct process_list *alt_process;
	pnx = 0;
	begin_decl ();
	decl_expr (&a0);
	decl_expr (&a1);
	for (i=0; i<MAX_NEW_CONS; i++)
		dle (nx[i]);
#ifdef TRACE
	printf ("\ngrandparent: a0 = "); print_expr (a0);
	printf ("\ngrandparent: a1 = "); print_expr (a1);
#endif
	if (alt (k, 1, 0))
	{
	/* clause */
	expr val_A, var_A;
	expr val_C, var_C;
	expr val_B, var_B;
		alt_process = getpl (k) -> alt;
		dle(val_A) dle(var_A)
		dle(val_C) dle(var_C)
		dle(val_B) dle(var_B)
		val_A=UNDEF; var_A=mk_var(&val_A);
		val_C=UNDEF; var_C=mk_var(&val_C);
		val_B=UNDEF; var_B=mk_var(&val_B);
#ifdef TRACE
		printf ("\n\ta0 = "); print_expr (a0);
		printf ("\n\ta1 = "); print_expr (a1);
#endif
		unify (k, var_A, a0);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		unify (k, var_C, a1);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		pl_parent_2 (k, var_A, var_B);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		pl_parent_2 (k, var_B, var_C);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		unify (k, a0, var_A);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		unify (k, a1, var_C);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
#ifdef TRACE
		printf ("\n\ta0 = "); print_expr (a0);
		printf ("\n\ta1 = "); print_expr (a1);
#endif
	} else
	end (k);
	free_expr ();
}
pl_parent_2 (struct coroutine *k, expr a0, expr a1)
{
expr nx[MAX_NEW_CONS];
int pnx, i;
struct process_list *alt_process;
	pnx = 0;
	begin_decl ();
	decl_expr (&a0);
	decl_expr (&a1);
	for (i=0; i<MAX_NEW_CONS; i++)
		dle (nx[i]);
#ifdef TRACE
	printf ("\nparent: a0 = "); print_expr (a0);
	printf ("\nparent: a1 = "); print_expr (a1);
#endif
	if (alt (k, 1, 0))
	{
	/* clause */
		alt_process = getpl (k) -> alt;
#ifdef TRACE
		printf ("\n\ta0 = "); print_expr (a0);
		printf ("\n\ta1 = "); print_expr (a1);
#endif
		unify (k, nx[pnx++] = cons (symbol("alphonse"),
			0), a0);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		unify (k, nx[pnx++] = cons (symbol("barnabe"),
			0), a1);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		unify (k, a0, nx[pnx++] = cons (symbol("alphonse"),
			0));
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		unify (k, a1, nx[pnx++] = cons (symbol("barnabe"),
			0));
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
#ifdef TRACE
		printf ("\n\ta0 = "); print_expr (a0);
		printf ("\n\ta1 = "); print_expr (a1);
#endif
	} else
	if (alt (k, 1, 0))
	{
	/* clause */
		alt_process = getpl (k) -> alt;
#ifdef TRACE
		printf ("\n\ta0 = "); print_expr (a0);
		printf ("\n\ta1 = "); print_expr (a1);
#endif
		unify (k, nx[pnx++] = cons (symbol("alphonse"),
			0), a0);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		unify (k, nx[pnx++] = cons (symbol("brigitte"),
			0), a1);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		unify (k, a0, nx[pnx++] = cons (symbol("alphonse"),
			0));
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		unify (k, a1, nx[pnx++] = cons (symbol("brigitte"),
			0));
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
#ifdef TRACE
		printf ("\n\ta0 = "); print_expr (a0);
		printf ("\n\ta1 = "); print_expr (a1);
#endif
	} else
	if (alt (k, 1, 0))
	{
	/* clause */
		alt_process = getpl (k) -> alt;
#ifdef TRACE
		printf ("\n\ta0 = "); print_expr (a0);
		printf ("\n\ta1 = "); print_expr (a1);
#endif
		unify (k, nx[pnx++] = cons (symbol("barnabe"),
			0), a0);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		unify (k, nx[pnx++] = cons (symbol("catherine"),
			0), a1);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		unify (k, a0, nx[pnx++] = cons (symbol("barnabe"),
			0));
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		unify (k, a1, nx[pnx++] = cons (symbol("catherine"),
			0));
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
#ifdef TRACE
		printf ("\n\ta0 = "); print_expr (a0);
		printf ("\n\ta1 = "); print_expr (a1);
#endif
	} else
	if (alt (k, 1, 0))
	{
	/* clause */
		alt_process = getpl (k) -> alt;
#ifdef TRACE
		printf ("\n\ta0 = "); print_expr (a0);
		printf ("\n\ta1 = "); print_expr (a1);
#endif
		unify (k, nx[pnx++] = cons (symbol("brigitte"),
			0), a0);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		unify (k, nx[pnx++] = cons (symbol("claude"),
			0), a1);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		unify (k, a0, nx[pnx++] = cons (symbol("brigitte"),
			0));
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		unify (k, a1, nx[pnx++] = cons (symbol("claude"),
			0));
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
#ifdef TRACE
		printf ("\n\ta0 = "); print_expr (a0);
		printf ("\n\ta1 = "); print_expr (a1);
#endif
	} else
	if (alt (k, 1, 0))
	{
	/* clause */
		alt_process = getpl (k) -> alt;
#ifdef TRACE
		printf ("\n\ta0 = "); print_expr (a0);
		printf ("\n\ta1 = "); print_expr (a1);
#endif
		unify (k, nx[pnx++] = cons (symbol("brigitte"),
			0), a0);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		unify (k, nx[pnx++] = cons (symbol("chloe"),
			0), a1);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		unify (k, a0, nx[pnx++] = cons (symbol("brigitte"),
			0));
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		unify (k, a1, nx[pnx++] = cons (symbol("chloe"),
			0));
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
#ifdef TRACE
		printf ("\n\ta0 = "); print_expr (a0);
		printf ("\n\ta1 = "); print_expr (a1);
#endif
	} else
	if (alt (k, 1, 0))
	{
	/* clause */
		alt_process = getpl (k) -> alt;
#ifdef TRACE
		printf ("\n\ta0 = "); print_expr (a0);
		printf ("\n\ta1 = "); print_expr (a1);
#endif
		unify (k, nx[pnx++] = cons (symbol("didier"),
			0), a0);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		unify (k, nx[pnx++] = cons (symbol("evelyne"),
			0), a1);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		unify (k, a0, nx[pnx++] = cons (symbol("didier"),
			0));
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		unify (k, a1, nx[pnx++] = cons (symbol("evelyne"),
			0));
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
#ifdef TRACE
		printf ("\n\ta0 = "); print_expr (a0);
		printf ("\n\ta1 = "); print_expr (a1);
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
		pl_grandparent_2 (k, nx[pnx++] = cons (symbol("alphonse"),
			0), var_X);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		pl_printexpr_1 (k, var_X);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		pl_printstring_1 (k, "\n");
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
