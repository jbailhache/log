#include <stdio.h>

#define error(s) printf("%s\n",s)

#define MAXMEM 5000

typedef int typ;

typ mem[MAXMEM];

#define ENDMEM (mem+MAXMEM)

typedef typ *expr;

#define MAXSYMBOLS 1000

expr symbols[MAXSYMBOLS];
int nsymbols;

typedef int exprsize;

#define PRESIZE sizeof(exprsize)

expr freeptr;

void init ()
{
 freeptr = mem+PRESIZE;
}

#define LEN(p) (*(((exprsize *)(p))-1))

void quit()
{
 exit(0);
}

expr alloc (int n)
{
 expr p1;
 p1 = freeptr;
 freeptr += n + PRESIZE;
 if (freeptr >= ENDMEM)
 {
  error("Memory overflow");
  quit();
 }
 LEN(p1) = n;
 return p1;
}

expr store (typ *p, exprsize n)
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


#define NAME(s) (((char *)(s))+sizeof(void *)+sizeof(expr))

#define GET(x,n) (((expr *)(x))[n])

#define CAR(x) GET(x,0)
#define CDR(x) GET(x,1)

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

#define PC(ctx) GET(ctx,0)
#define RET(ctx) GET(ctx,1)
#define STK(ctx) GET(ctx,2)
#define ENV(ctx) GET(ctx,3)

typedef int (*routine) (expr);

#define PRIM(s) ((routine)(GET(s,0)))

void evol (expr ctx)
{
/* int (*routine)(expr);*/
 routine r;
 expr instr;
 int status;

 instr = GET(PC(ctx),0);
/* routine = (int (*)(expr)) GET(instr,0);*/
 r = PRIM(instr);
 status = (*r)(ctx);

 PC(ctx)++;

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
 x = GET(PC(ctx),1);
 PC(ctx) = x-1;
 return 0;
}

int instr_gosub (expr ctx)
{
 expr x;
 RET(ctx) = cons (PC(ctx), RET(ctx));
 x = GET(PC(ctx),1);
 PC(ctx) = x-1;
 return 0;
}

int instr_return (expr ctx)
{
 PC(ctx) = CAR(RET(ctx));
 return 0;
}

int instr_print_symbol (expr ctx)
{
 printf ("%s", NAME(GET(STK(ctx),0)));
}

int instr_quit (expr ctx)
{
 exit(0);
}


expr symbol (char *s)
{
 int i;

 for (i=0; i<nsymbols; i++)
 {
  if (!strcmp(s,NAME(symbols[i])))
   return symbols[i];
  if (nsymbols >= MAXSYMBOLS)
  {
   error ("Symbol overflow");
   quit();
  }
  expr x = alloc (2*sizeof(void *)+strlen(s)+1);
  GET(x,0) = (expr)def0;
  GET(x,1) = x;
  strcpy (NAME(x), s);
  return x;
 }
}


#define DEFPRIM(s,p) x = symbol(s); PRIM(s) = p;

void init_prim ()
{
 expr x;
 DEFPRIM ("print_symbol", instr_print_symbol)
 DEFPRIM ("quit", instr_quit)

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

expr readexpr ()
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
  case '(':
   nextchar();
   i = 0;
   for (;;)
   {
    if (curchar() == ')')
    {
     x = alloc (i * sizeof(expr));
     for (j=0; j<i; j++)
      GET(x,j) = a[j];
     return x;
    }
    x = readexpr();
    a[i++] = x;
   }
   break;
  default:
   i = 0;
   for (;;)
   {
    if (curchar() == ' ')
    {
     buf[i] = 0;
     x = symbol(buf);
     return x;
    }
    buf[i++] = curchar();
    nextchar();
   }

 }
}

expr compil (char *s)
{
 expr x;
 ptr = s;
 x = readexpr();
 return x;
}


int main(int argc, char *argv[])
{
char buf[1000];
expr ctx;
  printf ("Bonjour !\n");
  init();
  init_prim();
  ctx = compil ("[[rebonjour print_symbol quit] nil nil nil]");
  for (;;)
  {
   evol(ctx);
  }

  gets (buf);
  return 0;
}
