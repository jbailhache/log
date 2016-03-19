
/* Coroutines PC / Linux */

#include <stdio.h>
#include <setjmp.h>

#ifdef DOS
#include <dos.h>
#endif

#include "coroutin.h"

#define NACR_MESSAGE "\nNo more result\n"

/*
struct coroutine
{
    jmp_buf *calling;
    jmp_buf *env;
};

#define end end_cr
*/

/* int options = 1; */
#define OPTIONS 0 /* 1 pour traces */

int start_coroutine (struct coroutine *cr,
    int (*f) (/* void *p, struct coroutine *cr */),
    void *p, int *stack)
{
int x, y;
struct coroutine *calling;
int _SP;
    calling = malloc (sizeof(*calling));
    x = setjmp (*(cr->calling));
#if (OPTIONS & 1)
    printf ("start_coroutine: x = %d\n", x);
    print_jmp_buf (*(cr->calling));
#endif
    if (x == 0)
    {
	calling->calling = cr->env;
	calling->env = cr->calling;
	_SP = 1234;
	_SP = stack;
	y = (*f) (p, calling);
    }
    else
	return x;
}

