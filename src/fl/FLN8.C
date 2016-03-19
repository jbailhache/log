/* --- The following code comes from C:\lcc\lib\wizard\textmode.tpl. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void Usage(char *programName)
{
	fprintf(stderr,"%s usage:\n",programName);
	/* Modify here to add your usage message when the program is
	 * called without arguments */
}

/* returns the index of the first argument that is not an option; i.e.
   does not start with a dash or a slash
*/
int HandleOptions(int argc,char *argv[])
{
	int i,firstnonoption=0;

	for (i=1; i< argc;i++) {
		if (argv[i][0] == '/' || argv[i][0] == '-') {
			switch (argv[i][1]) {
				/* An argument -? means help is requested */
				case '?':
					Usage(argv[0]);
					break;
				case 'h':
				case 'H':
					if (!stricmp(argv[i]+1,"help")) {
						Usage(argv[0]);
						break;
					}
					/* If the option -h means anything else
					 * in your application add code here
					 * Note: this falls through to the default
					 * to print an "unknow option" message
					*/
				/* add your option switches here */
				default:
					fprintf(stderr,"unknown option %s\n",argv[i]);
					break;
			}
		}
		else {
			firstnonoption = i;
			break;
		}
	}
	return firstnonoption;
}

#define error(s) printf("%s\n",s)

#define MAXMEM 5000

/*typedef int typ;*/
/*typedef long typ;*/
typedef void *typ;

typ mem[MAXMEM];

#define ENDMEM (mem+MAXMEM)

typedef typ *expr;
/*typedef expr *expr;*/

typedef int flint;

#define MAXSYMBOLS 1000

expr symbols[MAXSYMBOLS];
int nsymbols;

typedef int exprsize;

#define PRESIZE sizeof(exprsize)

expr freeptr;

int isdigit (char c)
{
 if (c >= '0' && c <= '9')
  return 1;
 else
  return 0;
}

void init ()
{
 freeptr = mem+PRESIZE;
}

#define LEN(p) (*(((exprsize *)(p))-1))

void quit()
{
 exit(0);
}


/* #define NAME(s) (((char *)(s))+sizeof(void *)+sizeof(expr)) */
#define NAME(s) (((char *)(s))+3*sizeof(expr))

#define GET(x,n) (((expr *)(x))[(flint)n])

#define CAR(x) GET(x,0)
#define CDR(x) GET(x,1)


expr alloc (int n)
{
expr p1;
int i;
 p1 = freeptr;
 freeptr += n + PRESIZE;
 if (freeptr >= ENDMEM)
 {
  error("Memory overflow");
  quit();
 }
 LEN(p1) = n;
 for (i=0; i<n; i++)
  GET(p1,i) = 0;
 return p1;
}

expr store1 (typ *p, exprsize n)
{
 typ *p1;
 int n1;
 p1 = mem+PRESIZE;
 for (;;)
 {
  if (p1 >= freeptr)
  {
   LEN(p1) = n;
   memcpy (p1, p, n);
   freeptr = p1 + PRESIZE;
   if (freeptr >= ENDMEM)
   {
    error ("Memory overflow");
   }
   return p1;
  }
  n1 = LEN(p1);
  if (n == n1)
  {
   if (memcmp (p, p1, n))
    return p1;
  }
  p1 += n1 + PRESIZE;
 }
}

expr store0 (expr p, int i, int n)
{
 store1 (p+i, n*sizeof(typ));
}

expr store (expr p, int i, int n)
{
expr x, y;
int j;
 x = alloc (n * sizeof(expr));
 for (j=0; j<n; j++)
 {
  y = GET(p,i+j);
  GET(x,j) = y;
 }
 return x;
}

expr cons (expr a, expr b)
{
 expr c;
 c = alloc(2*sizeof(expr));
 CAR(c) = a;
 CDR(c) = b;
 return c;
}

/*
 ctx = (PC ret pile args)
 */

#define PHASE_RUN 1
#define PHASE_READ 2

#define PHASE(ctx) GET(ctx,0)
#define PC(ctx) GET(ctx,1)
#define RET(ctx) GET(ctx,2)
#define STK(ctx) GET(ctx,3)
#define ENV(ctx) GET(ctx,4)
#define BUF(ctx) GET(ctx,5)
#define ORG(ctx) ((flint)GET(ctx,6))
#define BGN(ctx) ((flint)GET(ctx,7))
#define END(ctx) ((flint)GET(ctx,8))

typedef int (*routine) (expr);

#define PRIM(s) ((routine)(GET(s,0)))
#define DEF(s) GET(s,1)
#define IMDF(s) GET(s,2)

#define SCONS cons
#define SCAR(x) CAR(x)
#define SCDR(x) CDR(x)

expr SNTH (int i, expr x)
{
 if (i <= 0)
  return SCAR(x);
 return SNTH (i-1, SCDR(x));
}


int def0 (expr ctx);

expr symbol (char *s)
{
 int i;
 expr x;

 for (i=0; i<nsymbols; i++)
 {
  if (!strcmp(s,NAME(symbols[i])))
   return symbols[i];
  if (nsymbols >= MAXSYMBOLS)
  {
   error ("Symbol overflow");
   quit();
  }
 }
  x = alloc (2*sizeof(void *)+strlen(s)+1);
  GET(x,0) = (expr)def0;
  GET(x,1) = x;
  GET(x,2) = 0;
  strcpy (NAME(x), s);
  symbols[nsymbols++] = x;
  return x;

}


expr lex[256];

char *ptr;

char curchar ()
{
 return *ptr;
}

void nextchar ()
{
 ptr++;
}


expr readexpr0 ()
{
 char buf[120];
 int i, j;
 int v;
 expr x;
 expr a[250];

 switch (curchar())
 {
  case '#':
   nextchar();
   i = 0;
   for (;;)
   {
    if (curchar() == '#')
    {
     buf[i] = 0;
     /*v = atoi (buf);*/
     sscanf (buf, "%d", &v);
     return (expr)v;
    }
    buf[i++] = curchar();
    nextchar();
   }
  case '[':
   nextchar();
   i = 0;
   for (;;)
   {
    if (curchar() == ']')
    {
     nextchar();
     x = alloc (i * sizeof(expr));
     for (j=0; j<i; j++)
      GET(x,j) = a[j];
     return x;
    }
    x = readexpr0();
    a[i++] = x;
   }
   break;
  default:
   i = 0;
   for (;;)
   {
    if (curchar() == ' ')
    {
     nextchar();
     buf[i] = 0;
     x = symbol(buf);
     return x;
    }
    buf[i++] = curchar();
    nextchar();
   }

 }
}

#define MAXINX 100
typ inxbuf[MAXINX];
int inxbegin = 0;
int inxend = 0;

expr readsym ()
{
char buf[1000];
int i;
expr x;
   i = 0;
   for (;;)
   {
    if (curchar() == ' ')
    {
     nextchar();
     buf[i] = 0;
     x = symbol(buf);
     return x;
    }
    buf[i++] = curchar();
    nextchar();
   }
}

expr readint (flint s, flint n)
{
flint n1;
 if (curchar() == ' ')
 {
  nextchar();
  return (expr)(s * n);
 }
 else if (curchar() == '-')
 {
  nextchar();
  return readint (-s, n);
 }
 n1 = 10*n + curchar() - '0';
 nextchar();
 return readint (s, n1);
}

readspaces()
{
 while (curchar() == ' ')
  nextchar();
}

int readexpr1 (expr *r)
{
expr x;
 switch (curchar())
 {
  case '[':
   nextchar();
   inxbuf[inxend++] = inxbegin;
   inxbegin = inxend;
   return 0;

  case ']':
   nextchar();
   x = store (inxbuf, inxbegin, inxend-inxbegin);
   inxend = inxbegin - 1;
   inxbegin = inxbuf[inxend];
   *r = x;
   return 1;

  default:
   if (isdigit (curchar()) || curchar()=='-')
   {
    *r = readint (1, 0);
    return 1;
   }
   else
   {
    *r = readsym ();
    return 1;
   }
 }
}

expr readexprloop ()
{
int status;
expr x;
 for (;;)
 {
  readspaces();
  status = readexpr1(&x);
  if (status)
  {
   if (inxend == 0)
    return x;
   else
    inxbuf[inxend++] = x;
  }
 }
}

/*
expr readexpr ()
{
 return readexpr0();
}
*/

expr readexpr ()
{
 inxbegin = 0;
 inxend = 0;
 return readexprloop();
}

expr compil (char *s)
{
 expr x;
 ptr = s;
 x = readexpr();
 return x;
}

void evolrun (expr ctx)
{
/* int (*routine)(expr);*/
 routine r;
 expr instr;
 int status;

 instr = GET(PC(ctx)++,0);
/* routine = (int (*)(expr)) GET(instr,0);*/
 r = PRIM(instr);
 status = (*r)(ctx);

 /*PC(ctx)++;*/

}

void retread (expr ctx, expr x)
{
 if (END(ctx) == ORG(ctx))
 {
  STK(ctx) = SCONS (x, STK(ctx));
  PHASE(ctx) = PHASE_RUN;
 }
 else
 {
  GET(BUF(ctx),END(ctx)++) = x;
 }
}

void evolread (expr ctx)
{
expr x;
 readspaces();
 switch (curchar())
 {
  case '&':
   nextchar();
   RET(ctx) = SCONS (PC(ctx), RET(ctx));
   PC(ctx) = GET(BUF(ctx),END(ctx)-1);
   END(ctx)--;
   PHASE(ctx) = PHASE_RUN;
   break;
  case '[': 
   nextchar();
   GET(BUF(ctx),END(ctx)++) = BGN(ctx);
   BGN(ctx) = END(ctx);
   break;

  case ']':
   nextchar();
   x = store (BUF(ctx), BGN(ctx), END(ctx)-BGN(ctx));
   END(ctx) = BGN(ctx)-1;
   BGN(ctx) = GET(BUF(ctx),END(ctx));
   retread (ctx, x);
   break;
  
  default:
   if ((curchar() >= '0' && curchar <= '9') || curchar == '-')
   {
    retread (ctx, readint (1, 0));
   }
   else
   {
    x = readsym();
    if (IMDF(x) == 0)
    {
     retread (ctx, x);
    }
    else
    {
     RET(ctx) = SCONS (PC(ctx), RET(ctx));
     PC(ctx) = IMDF(x);
     PHASE(ctx) = PHASE_RUN;
    }
    

   }
 }    
     
}


void evol (expr ctx)
{
 switch (PHASE(ctx))
 {
  case PHASE_RUN:
   evolrun (ctx);
   break;
  case PHASE_READ:
   evolread(ctx);
   break;
 }
}

int def0 (expr ctx)
{
 expr instr;
 instr = GET(PC(ctx),0);
 STK(ctx) = cons (instr, STK(ctx));
 return 0;
}

int instr_goto (expr ctx)
{
 expr x;
 x = GET(PC(ctx),0);
 PC(ctx) = x;
 return 0;
}

int instr_gosub (expr ctx)
{
 expr x;
 RET(ctx) = SCONS (PC(ctx), RET(ctx));
 x = GET(PC(ctx),0);
 PC(ctx) = x;
 return 0;
}

int instr_return (expr ctx)
{
 PC(ctx) = SCAR(RET(ctx));
 RET(ctx) = SCDR(RET(ctx));
 return 0;
}

int instr_eval (expr ctx)
{
 RET(ctx) = SCONS (PC(ctx), RET(ctx));
 PC(ctx) = SCAR(STK(ctx));
 STK(ctx) = SCDR(STK(ctx));
}

int instr_then (expr ctx)
{
 RET(ctx) = SCONS (PC(ctx)+2, RET(ctx));
 PC(ctx) = GET (PC(ctx), 1-(flint)SCAR(STK(ctx)));
 STK(ctx) = SCDR(STK(ctx));
}

int instr_getctx (expr ctx)
{
 STK(ctx) = SCONS (ctx, STK(ctx));
}

int instr_getctxfield (expr ctx)
{
 STK(ctx) = SCONS (GET(ctx,(flint)SCAR(STK(ctx))), SCDR(STK(ctx)));
}

int instr_setctxfield (expr ctx)
{
int i;
expr x;
 i = (flint) SCAR(STK(ctx));
 x = SCAR(SCDR(STK(ctx)));
 STK(ctx) = SCDR(SCDR(STK(ctx)));
 GET(ctx,i) = x;
}

int instr_print_symbol (expr ctx)
{
 printf ("%s", NAME(SCAR(STK(ctx))));
 STK(ctx) = SCDR(STK(ctx));
}

int instr_evol (expr ctx)
{
expr ctx1;
 ctx1 = SCAR(STK(ctx));
 evol(ctx1);
}

int instr_print_int (expr ctx)
{
 printf ("%d", SCAR(STK(ctx)));
 STK(ctx) = SCDR(STK(ctx));
}

int instr_plus (expr ctx)
{
/*
 STK(ctx) = cons ((expr)(((flint)GET(STK(ctx),0))+((flint)GET(STK(ctx),1))),
			CDR(CDR(STK(ctx))));
*/
flint a, b, c;
expr x;
 a = (flint) SCAR(STK(ctx));
 b = (flint) SCAR(SCDR(STK(ctx)));
 c = a + b;
 x = (expr)c;
 STK(ctx) = SCONS (x, SCDR(SCDR(STK(ctx))));
}

int instr_times (expr ctx)
{
 STK(ctx) = SCONS ((expr)((flint)SCAR(STK(ctx))*(flint)SCAR(SCDR(STK(ctx)))),
			SCDR(SCDR(STK(ctx))));
}

flint sign (flint x)
{
 if (x > 0)
  return 1;
 else if (x < 0)
  return -1;
 else
  return 0;
}

int instr_sign (expr ctx)
{
 STK(ctx) = SCONS ((expr)sign((flint)SCAR(STK(ctx))), SCDR(STK(ctx)));
}

int instr_eq (expr ctx)
{
 if (SCAR(STK(ctx))==SCAR(SCDR(STK(ctx))))
  STK(ctx) = SCONS ((expr)1, SCDR(SCDR(STK(ctx))));
 else
  STK(ctx) = SCONS ((expr)0, SCDR(SCDR(STK(ctx))));
}

int instr_get0 (expr ctx)
{
 STK(ctx) = SCONS (GET(SCAR(STK(ctx)),0), SCDR(STK(ctx)));
}

int instr_put0 (expr ctx)
{
 GET(SCAR(STK(ctx)),0) = SCAR(SCDR(STK(ctx)));
 STK(ctx)=SCDR(SCDR(STK(ctx)));
}

int instr_alloc (expr ctx)
{
 STK(ctx) = SCONS (alloc ((flint)SCAR(STK(ctx))), SCDR(STK(ctx)));
}

int instr_in (expr ctx)
{
 ENV(ctx) = SCONS (SCAR(STK(ctx)), ENV(ctx));
 STK(ctx) = SCDR(STK(ctx));
}

int instr_ex (expr ctx)
{
 ENV(ctx) = SCDR(ENV(ctx));
}

int instr_var (expr ctx)
{
 STK(ctx) = SCONS (SNTH ((flint)GET(PC(ctx)++,0), ENV(ctx)), STK(ctx));
}


int instr_quote (expr ctx)
{
 STK(ctx) = cons (GET(PC(ctx)++,0), STK(ctx));
}

int instr_curchar (expr ctx)
{
 STK(ctx) = SCONS ((expr)curchar(), STK(ctx));
}

int instr_nextchar (expr ctx)
{
 nextchar();
}

int instr_read_symbol (expr ctx)
{
 STK(ctx) = SCONS (readsym(), STK(ctx));
}

int instr_readexpr (expr ctx)
{
 STK(ctx) = SCONS (readexpr(), STK(ctx)); 
}

int instr_read_expr (expr ctx)
{
 BGN(ctx) = ORG(ctx);
 END(ctx) = ORG(ctx);
 PHASE(ctx) = PHASE_READ;
}

int instr_nest (expr ctx)
{
 GET(BUF(ctx),END(ctx)) = ORG(ctx);
 GET(BUF(ctx),END(ctx)+1) = BGN(ctx);
 ORG(ctx) = END(ctx)+2;
}

int instr_endnest (expr ctx)
{
 END(ctx) = ORG(ctx)-2;
 ORG(ctx) = GET(BUF(ctx),END(ctx));
 BGN(ctx) = GET(BUF(ctx),END(ctx)+1); 
}

int instr_place (expr ctx)
{
 GET(BUF(ctx),END(ctx)) = SCAR(STK(ctx));
 END(ctx)++;
 STK(ctx) = SCDR(STK(ctx)); 
}

int instr_readon (expr ctx)
{
 PHASE(ctx) = PHASE_READ;
}

int instr_quit (expr ctx)
{
 exit(0);
}




#define DEFPRIM(s,p) x = symbol(s); PRIM(x) = p;

void init_prim ()
{
 expr x;
 x = symbol("print_symbol");
 GET(x,0) = (expr)instr_print_symbol;
 DEFPRIM ("print_symbol", instr_print_symbol)
 DEFPRIM ("print_int", instr_print_int)
 DEFPRIM ("+", instr_plus)
 DEFPRIM ("*", instr_times)
 DEFPRIM ("sign", instr_sign)
 DEFPRIM ("=", instr_eq)
 DEFPRIM ("@", instr_get0)
 DEFPRIM ("!", instr_put0)
 DEFPRIM ("<", instr_in)
 DEFPRIM (">", instr_ex)
 DEFPRIM ("%", instr_var)
 DEFPRIM ("quote", instr_quote)
 DEFPRIM ("'", instr_quote)
 DEFPRIM ("eval", instr_eval)
 DEFPRIM ("goto", instr_goto)
 DEFPRIM ("gosub", instr_gosub)
 DEFPRIM ("then", instr_then)
 DEFPRIM ("return", instr_return)
 DEFPRIM ("getctx", instr_getctx)
 DEFPRIM ("getctxfield", instr_getctxfield)
 DEFPRIM ("setctxfield", instr_setctxfield)
 DEFPRIM ("evol", instr_evol)
 DEFPRIM ("curchar", instr_curchar)
 DEFPRIM ("nextchar", instr_nextchar)
 DEFPRIM ("read_symbol", instr_read_symbol)
 DEFPRIM ("readexpr", instr_readexpr)
 DEFPRIM ("read_expr", instr_read_expr)
 DEFPRIM ("nest", instr_nest)
 DEFPRIM ("endnest", instr_endnest)
 DEFPRIM ("place", instr_place)
 DEFPRIM ("readon", instr_readon)
 DEFPRIM ("quit", instr_quit)
}

int fl(int argc, char *argv[])
{
char buf[1000];
expr ctx;
  printf ("Bonjour !\n");
  init();
  init_prim();
  ctx = compil ("[1 ["
  "quote rebonjour print_symbol "
  "curchar print_int readexpr print_symbol "
  "getctx evol print_int "
  "' [testctx nil ] ' 3 setctxfield print_symbol "
  "' 0 then [' oui print_symbol return ] [' non print_symbol return ] "
  "' 1 then [' Oui print_symbol return ] [' Non print_symbol return ] "
  "' [' rerebonjour print_symbol return ] eval "
  "' hhh ' iii < % 0 print_symbol > print_symbol "
  "quote 3 quote 4 * quote 6 + print_int quote aaa print_symbol "
  "quote -3 sign print_int quote bbb print_symbol "
  "quote 0 sign print_int quote ccc print_symbol "
  "quote 5 sign print_int quote ddd print_symbol "
  "quote 6 quote 6 = print_int quote eee print_symbol "
  "quote 6 quote 8 = print_int quote fff print_symbol "
  "quote [xxx ] @ print_symbol "
  "quit ]nil nil nil nil 0 0 0 ]zzz ");
  BUF(ctx) = alloc(200);
  for (;;)
  {
   evol(ctx);
  }

  gets (buf);
  return 0;
}


int main(int argc,char *argv[])
{
	fl (argc, argv);
	exit(0);

	if (argc == 1) {
		/* If no arguments we call the Usage routine and exit */
		Usage(argv[0]);
		return 1;
	}
	/* handle the program options */
	HandleOptions(argc,argv);
	/* The code of your application goes here */
	return 0;
}
