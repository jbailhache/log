/* C program translated from Prolog code */
#include "coroutin.h"
#include "expr.h"
#include "prolog.h"
pl_simpl_2 (struct coroutine *k, expr a0, expr a1)
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
	printf ("\nsimpl: a0 = "); print_expr (a0);
	printf ("\nsimpl: a1 = "); print_expr (a1);
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
		printf ("\n\ta1 = "); print_expr (a1);
#endif
		unify (k, nx[pnx++] = cons (symbol("plus"),
			nx[pnx++] = cons (var_X,
			nx[pnx++] = cons (0,
			0))), a0);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		unify (k, var_X, a1);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		unify (k, a0, nx[pnx++] = cons (symbol("plus"),
			nx[pnx++] = cons (var_X,
			nx[pnx++] = cons (0,
			0))));
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		unify (k, a1, var_X);
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
	expr val_X, var_X;
		alt_process = getpl (k) -> alt;
		dle(val_X) dle(var_X)
		val_X=UNDEF; var_X=mk_var(&val_X);
#ifdef TRACE
		printf ("\n\ta0 = "); print_expr (a0);
		printf ("\n\ta1 = "); print_expr (a1);
#endif
		unify (k, nx[pnx++] = cons (symbol("plus"),
			nx[pnx++] = cons (0,
			nx[pnx++] = cons (var_X,
			0))), a0);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		unify (k, var_X, a1);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		pl_cut_0 (k);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		unify (k, a0, nx[pnx++] = cons (symbol("plus"),
			nx[pnx++] = cons (0,
			nx[pnx++] = cons (var_X,
			0))));
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		unify (k, a1, var_X);
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
	expr val_X, var_X;
		alt_process = getpl (k) -> alt;
		dle(val_X) dle(var_X)
		val_X=UNDEF; var_X=mk_var(&val_X);
#ifdef TRACE
		printf ("\n\ta0 = "); print_expr (a0);
		printf ("\n\ta1 = "); print_expr (a1);
#endif
		unify (k, nx[pnx++] = cons (symbol("times"),
			nx[pnx++] = cons (var_X,
			nx[pnx++] = cons (0,
			0))), a0);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		unify (k, 0, a1);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		pl_cut_0 (k);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		unify (k, a0, nx[pnx++] = cons (symbol("times"),
			nx[pnx++] = cons (var_X,
			nx[pnx++] = cons (0,
			0))));
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		unify (k, a1, 0);
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
	expr val_X, var_X;
		alt_process = getpl (k) -> alt;
		dle(val_X) dle(var_X)
		val_X=UNDEF; var_X=mk_var(&val_X);
#ifdef TRACE
		printf ("\n\ta0 = "); print_expr (a0);
		printf ("\n\ta1 = "); print_expr (a1);
#endif
		unify (k, nx[pnx++] = cons (symbol("times"),
			nx[pnx++] = cons (0,
			nx[pnx++] = cons (var_X,
			0))), a0);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		unify (k, 0, a1);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		pl_cut_0 (k);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		unify (k, a0, nx[pnx++] = cons (symbol("times"),
			nx[pnx++] = cons (0,
			nx[pnx++] = cons (var_X,
			0))));
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		unify (k, a1, 0);
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
	expr val_X, var_X;
		alt_process = getpl (k) -> alt;
		dle(val_X) dle(var_X)
		val_X=UNDEF; var_X=mk_var(&val_X);
#ifdef TRACE
		printf ("\n\ta0 = "); print_expr (a0);
		printf ("\n\ta1 = "); print_expr (a1);
#endif
		unify (k, nx[pnx++] = cons (symbol("times"),
			nx[pnx++] = cons (var_X,
			nx[pnx++] = cons (1,
			0))), a0);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		unify (k, var_X, a1);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		pl_cut_0 (k);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		unify (k, a0, nx[pnx++] = cons (symbol("times"),
			nx[pnx++] = cons (var_X,
			nx[pnx++] = cons (1,
			0))));
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		unify (k, a1, var_X);
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
	expr val_X, var_X;
		alt_process = getpl (k) -> alt;
		dle(val_X) dle(var_X)
		val_X=UNDEF; var_X=mk_var(&val_X);
#ifdef TRACE
		printf ("\n\ta0 = "); print_expr (a0);
		printf ("\n\ta1 = "); print_expr (a1);
#endif
		unify (k, nx[pnx++] = cons (symbol("times"),
			nx[pnx++] = cons (1,
			nx[pnx++] = cons (var_X,
			0))), a0);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		unify (k, var_X, a1);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		pl_cut_0 (k);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		unify (k, a0, nx[pnx++] = cons (symbol("times"),
			nx[pnx++] = cons (1,
			nx[pnx++] = cons (var_X,
			0))));
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		unify (k, a1, var_X);
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
	expr val_X, var_X;
		alt_process = getpl (k) -> alt;
		dle(val_X) dle(var_X)
		val_X=UNDEF; var_X=mk_var(&val_X);
#ifdef TRACE
		printf ("\n\ta0 = "); print_expr (a0);
		printf ("\n\ta1 = "); print_expr (a1);
#endif
		unify (k, nx[pnx++] = cons (symbol("plus"),
			nx[pnx++] = cons (var_X,
			nx[pnx++] = cons (nx[pnx++] = cons (symbol("minus"),
			nx[pnx++] = cons (var_X,
			0)),
			0))), a0);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		unify (k, 0, a1);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		pl_cut_0 (k);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		unify (k, a0, nx[pnx++] = cons (symbol("plus"),
			nx[pnx++] = cons (var_X,
			nx[pnx++] = cons (nx[pnx++] = cons (symbol("minus"),
			nx[pnx++] = cons (var_X,
			0)),
			0))));
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		unify (k, a1, 0);
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
	expr val_X, var_X;
		alt_process = getpl (k) -> alt;
		dle(val_X) dle(var_X)
		val_X=UNDEF; var_X=mk_var(&val_X);
#ifdef TRACE
		printf ("\n\ta0 = "); print_expr (a0);
		printf ("\n\ta1 = "); print_expr (a1);
#endif
		unify (k, nx[pnx++] = cons (symbol("plus"),
			nx[pnx++] = cons (nx[pnx++] = cons (symbol("minus"),
			nx[pnx++] = cons (var_X,
			0)),
			nx[pnx++] = cons (var_X,
			0))), a0);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		unify (k, 0, a1);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		pl_cut_0 (k);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		unify (k, a0, nx[pnx++] = cons (symbol("plus"),
			nx[pnx++] = cons (nx[pnx++] = cons (symbol("minus"),
			nx[pnx++] = cons (var_X,
			0)),
			nx[pnx++] = cons (var_X,
			0))));
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		unify (k, a1, 0);
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
	expr val_X, var_X;
	expr val_Y, var_Y;
	expr val_X1, var_X1;
	expr val_Y1, var_Y1;
		alt_process = getpl (k) -> alt;
		dle(val_X) dle(var_X)
		dle(val_Y) dle(var_Y)
		dle(val_X1) dle(var_X1)
		dle(val_Y1) dle(var_Y1)
		val_X=UNDEF; var_X=mk_var(&val_X);
		val_Y=UNDEF; var_Y=mk_var(&val_Y);
		val_X1=UNDEF; var_X1=mk_var(&val_X1);
		val_Y1=UNDEF; var_Y1=mk_var(&val_Y1);
#ifdef TRACE
		printf ("\n\ta0 = "); print_expr (a0);
		printf ("\n\ta1 = "); print_expr (a1);
#endif
		unify (k, nx[pnx++] = cons (symbol("plus"),
			nx[pnx++] = cons (var_X,
			nx[pnx++] = cons (var_Y,
			0))), a0);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		unify (k, nx[pnx++] = cons (symbol("plus"),
			nx[pnx++] = cons (var_X1,
			nx[pnx++] = cons (var_Y1,
			0))), a1);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		pl_simpl_2 (k, var_X, var_X1);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		pl_simpl_2 (k, var_Y, var_Y1);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		unify (k, a0, nx[pnx++] = cons (symbol("plus"),
			nx[pnx++] = cons (var_X,
			nx[pnx++] = cons (var_Y,
			0))));
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		unify (k, a1, nx[pnx++] = cons (symbol("plus"),
			nx[pnx++] = cons (var_X1,
			nx[pnx++] = cons (var_Y1,
			0))));
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
	expr val_X, var_X;
	expr val_Y, var_Y;
	expr val_X1, var_X1;
	expr val_Y1, var_Y1;
		alt_process = getpl (k) -> alt;
		dle(val_X) dle(var_X)
		dle(val_Y) dle(var_Y)
		dle(val_X1) dle(var_X1)
		dle(val_Y1) dle(var_Y1)
		val_X=UNDEF; var_X=mk_var(&val_X);
		val_Y=UNDEF; var_Y=mk_var(&val_Y);
		val_X1=UNDEF; var_X1=mk_var(&val_X1);
		val_Y1=UNDEF; var_Y1=mk_var(&val_Y1);
#ifdef TRACE
		printf ("\n\ta0 = "); print_expr (a0);
		printf ("\n\ta1 = "); print_expr (a1);
#endif
		unify (k, nx[pnx++] = cons (symbol("times"),
			nx[pnx++] = cons (var_X,
			nx[pnx++] = cons (var_Y,
			0))), a0);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		unify (k, nx[pnx++] = cons (symbol("times"),
			nx[pnx++] = cons (var_X1,
			nx[pnx++] = cons (var_Y1,
			0))), a1);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		pl_simpl_2 (k, var_X, var_X1);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		pl_simpl_2 (k, var_Y, var_Y1);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		unify (k, a0, nx[pnx++] = cons (symbol("times"),
			nx[pnx++] = cons (var_X,
			nx[pnx++] = cons (var_Y,
			0))));
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		unify (k, a1, nx[pnx++] = cons (symbol("times"),
			nx[pnx++] = cons (var_X1,
			nx[pnx++] = cons (var_Y1,
			0))));
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
	expr val_X, var_X;
		alt_process = getpl (k) -> alt;
		dle(val_X) dle(var_X)
		val_X=UNDEF; var_X=mk_var(&val_X);
#ifdef TRACE
		printf ("\n\ta0 = "); print_expr (a0);
		printf ("\n\ta1 = "); print_expr (a1);
#endif
		unify (k, var_X, a0);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		unify (k, var_X, a1);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		unify (k, a0, var_X);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		unify (k, a1, var_X);
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
pl_simplif_2 (struct coroutine *k, expr a0, expr a1)
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
	printf ("\nsimplif: a0 = "); print_expr (a0);
	printf ("\nsimplif: a1 = "); print_expr (a1);
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
		printf ("\n\ta1 = "); print_expr (a1);
#endif
		unify (k, var_X, a0);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		unify (k, var_X, a1);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		unify (k, a0, var_X);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		unify (k, a1, var_X);
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
	expr val_X, var_X;
	expr val_Z, var_Z;
	expr val_Y, var_Y;
		alt_process = getpl (k) -> alt;
		dle(val_X) dle(var_X)
		dle(val_Z) dle(var_Z)
		dle(val_Y) dle(var_Y)
		val_X=UNDEF; var_X=mk_var(&val_X);
		val_Z=UNDEF; var_Z=mk_var(&val_Z);
		val_Y=UNDEF; var_Y=mk_var(&val_Y);
#ifdef TRACE
		printf ("\n\ta0 = "); print_expr (a0);
		printf ("\n\ta1 = "); print_expr (a1);
#endif
		unify (k, var_X, a0);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		unify (k, var_Z, a1);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		pl_simpl_2 (k, var_X, var_Y);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		pl_cut_0 (k);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		pl_simplif_2 (k, var_Y, var_Z);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		pl_cut_0 (k);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		unify (k, a0, var_X);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		unify (k, a1, var_Z);
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
		pl_simplif_2 (k, nx[pnx++] = cons (symbol("times"),
			nx[pnx++] = cons (nx[pnx++] = cons (symbol("a"),
			0),
			nx[pnx++] = cons (nx[pnx++] = cons (symbol("plus"),
			nx[pnx++] = cons (nx[pnx++] = cons (symbol("minus"),
			nx[pnx++] = cons (nx[pnx++] = cons (symbol("b"),
			0),
			0)),
			nx[pnx++] = cons (nx[pnx++] = cons (symbol("b"),
			0),
			0))),
			0))), var_X);
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
pl_goal1_0 (struct coroutine *k)
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
	expr val_Y, var_Y;
		alt_process = getpl (k) -> alt;
		dle(val_X) dle(var_X)
		dle(val_Y) dle(var_Y)
		val_X=UNDEF; var_X=mk_var(&val_X);
		val_Y=UNDEF; var_Y=mk_var(&val_Y);
#ifdef TRACE
#endif
		pl_simpl_2 (k, nx[pnx++] = cons (symbol("times"),
			nx[pnx++] = cons (nx[pnx++] = cons (symbol("a"),
			0),
			nx[pnx++] = cons (nx[pnx++] = cons (symbol("plus"),
			nx[pnx++] = cons (nx[pnx++] = cons (symbol("minus"),
			nx[pnx++] = cons (nx[pnx++] = cons (symbol("b"),
			0),
			0)),
			nx[pnx++] = cons (nx[pnx++] = cons (symbol("b"),
			0),
			0))),
			0))), var_X);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		pl_simpl_2 (k, var_X, var_Y);
		for (i=0; i<pnx; i++)
			nx[i] = 0;
		pnx=0;
		pl_printexpr_1 (k, var_Y);
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
pl_goal2_0 (struct coroutine *k)
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
		pl_simpl_2 (k, nx[pnx++] = cons (symbol("plus"),
			nx[pnx++] = cons (0,
			nx[pnx++] = cons (nx[pnx++] = cons (symbol("a"),
			0),
			0))), var_X);
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
