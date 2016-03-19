/* C program translated from Prolog code */
#include "coroutin.h"
#include "expr.h"
#include "prolog.h"
pl_append_3 (struct coroutine *k, expr a0, expr a1, expr a2)
{
expr nx[MAX_NEW_CONS];
int pnx, i;
struct process_list *alt_process;
	pnx = 0;
	begin_decl ();
	decl_expr (&a0);
	decl_expr (&a1);
	decl_expr (&a2);
	for (i=0; i<MAX_NEW_CONS; i++)
		dle (nx[i]);
#ifdef TRACE
	printf ("\nappend: a0 = "); print_expr (a0);
	printf ("\nappend: a1 = "); print_expr (a1);
	printf ("\nappend: a2 = "); print_expr (a2);
#endif
	if (alt (k, 1, 0))
	{
	/* clause */
	expr val_L, var_L;
		alt_process = getpl (k) -> alt;
		dle(val_L) dle(var_L)
		val_L=UNDEF; var_L=mk_var(&val_L);
#ifdef TRACE
		printf ("\n\ta0 = "); print_expr (a0);
		printf ("\n\ta1 = "); print_expr (a1);
		printf ("\n\ta2 = "); print_expr (a2);
#endif
		unify (k, 0, a0);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		unify (k, var_L, a1);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		unify (k, var_L, a2);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		unify (k, a0, 0);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		unify (k, a1, var_L);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		unify (k, a2, var_L);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
#ifdef TRACE
		printf ("\n\ta0 = "); print_expr (a0);
		printf ("\n\ta1 = "); print_expr (a1);
		printf ("\n\ta2 = "); print_expr (a2);
#endif
	} else
	if (alt (k, 1, 0))
	{
	/* clause */
	expr val_X, var_X;
	expr val_A, var_A;
	expr val_B, var_B;
	expr val_C, var_C;
		alt_process = getpl (k) -> alt;
		dle(val_X) dle(var_X)
		dle(val_A) dle(var_A)
		dle(val_B) dle(var_B)
		dle(val_C) dle(var_C)
		val_X=UNDEF; var_X=mk_var(&val_X);
		val_A=UNDEF; var_A=mk_var(&val_A);
		val_B=UNDEF; var_B=mk_var(&val_B);
		val_C=UNDEF; var_C=mk_var(&val_C);
#ifdef TRACE
		printf ("\n\ta0 = "); print_expr (a0);
		printf ("\n\ta1 = "); print_expr (a1);
		printf ("\n\ta2 = "); print_expr (a2);
#endif
		unify (k, nx[pnx++] = cons (symbol("cons"),
			nx[pnx++] = cons (var_X,
			nx[pnx++] = cons (var_A,
			0))), a0);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		unify (k, var_B, a1);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		unify (k, nx[pnx++] = cons (symbol("cons"),
			nx[pnx++] = cons (var_X,
			nx[pnx++] = cons (var_C,
			0))), a2);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		pl_append_3 (k, var_A, var_B, var_C);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		unify (k, a0, nx[pnx++] = cons (symbol("cons"),
			nx[pnx++] = cons (var_X,
			nx[pnx++] = cons (var_A,
			0))));
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		unify (k, a1, var_B);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		unify (k, a2, nx[pnx++] = cons (symbol("cons"),
			nx[pnx++] = cons (var_X,
			nx[pnx++] = cons (var_C,
			0))));
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
#ifdef TRACE
		printf ("\n\ta0 = "); print_expr (a0);
		printf ("\n\ta1 = "); print_expr (a1);
		printf ("\n\ta2 = "); print_expr (a2);
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
	expr val_A, var_A;
	expr val_B, var_B;
		alt_process = getpl (k) -> alt;
		dle(val_A) dle(var_A)
		dle(val_B) dle(var_B)
		val_A=UNDEF; var_A=mk_var(&val_A);
		val_B=UNDEF; var_B=mk_var(&val_B);
#ifdef TRACE
#endif
		pl_printstring_1 (k, "\nTest append : \n");
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		pl_append_3 (k, var_A, var_B, nx[pnx++] = cons (symbol("cons"),
			nx[pnx++] = cons (nx[pnx++] = cons (symbol("a"),
			0),
			nx[pnx++] = cons (nx[pnx++] = cons (symbol("cons"),
			nx[pnx++] = cons (nx[pnx++] = cons (symbol("b"),
			0),
			nx[pnx++] = cons (nx[pnx++] = cons (symbol("cons"),
			nx[pnx++] = cons (nx[pnx++] = cons (symbol("c"),
			0),
			nx[pnx++] = cons (0,
			0))),
			0))),
			0))));
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		pl_printstring_1 (k, "A = ");
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		pl_printexpr_1 (k, var_A);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		pl_printstring_1 (k, ", B = ");
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		pl_printexpr_1 (k, var_B);
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
