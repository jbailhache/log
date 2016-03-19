// flv.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "base.h"

//#define error(s) printf("%s\n",s)
#define error(s) print(s)

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

char buf[1000];

void * memcpy(void *dst, const void *src, size_t len)
{
	int i;
trace ("memcpy",len);
	for (i=0; i<len; i++)
		((char *)dst)[i] = ((char *)src)[i];
trace("ret",0);
	return dst;
}

int memcmp (const void *p1, const void *p2, size_t n)
{
	int i;
trace("memcmp",0);
    for (i=0; i<n; i++)
	{
		if (((char *)p1)[i] != ((char *)p2)[i])
			return ((char *)p1)[i]-((char *)p2)[i];
	}
	return 0;
}

size_t strlen (const char *s)
{
	int i;
trace(s,0);
	for (i=0; s[i]!=0; i++);
trace("strlen",i);
	return i;
}

int strcmp (const char *s1, const char *s2)
{
		int i;
trace("strcmp",0);
		for (i=0; ; i++)
		{
			if (s1[i] == 0 && s2[i] == 0)
				return 0;
			if (s1[i] != s2[i])
				return s1[i]-s2[i];
		}
}

char * strcpy (char *dst, const char *src)
{
	int i;
trace("strcpy",0);
	for (i=0; i<strlen(src); i++)
		dst[i] = src[i];
trace("ret",0);
	return dst;
}

void init ()
{
 freeptr = mem+PRESIZE;
 nsymbols=0;
}

#define LEN(p) (*(((exprsize *)(p))-1))

void quit()
{
 exit(0);
}

expr alloc (int n)
{
 expr p;
trace("alloc",n);
 p = freeptr;
 freeptr += n + PRESIZE;
 if (freeptr >= ENDMEM)
 {
  error("Memory overflow");
  quit();
 }
 LEN(p) = n;
trace("alloc ret",0);
 return p;
}

expr store (typ *p, exprsize n)
{
 typ *p1;
 int n1;
trace("store",n);
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
   if (!memcmp (p, p1, n))
    return p1;
  }
  p1 += n1 + PRESIZE;
 }
}


#define NAME(s) (((char *)(s))+sizeof(void *)+sizeof(expr))

/* 
#define GET(x,n) ((expr *)(x))[n]
*/

expr GET (expr x, int n)
{
 return ((expr*)x)[n];
}

#define CAR(x) GET(x,0)
#define CDR(x) GET(x,1)

expr cons (expr a, expr b)
{
 expr c;
trace("cons",0);
 c = alloc(2*sizeof(expr));
 /*
 CAR(c) = a;
 CDR(c) = b;
*/
 ((expr*)c)[0]=a;
 ((expr *)c)[1]=b;
trace("cons ret",0);
 return c;
}

/*
 ctx = (PC ret pile args)
 */

#define PC(ctx) GET(ctx,0)
#define RET(ctx) GET(ctx,1)
#define STK(ctx) GET(ctx,2)
#define ENV(ctx) GET(ctx,3)

#define pPC 0
#define pRET 1
#define pSTK 2
#define pENV 3

typedef int (*routine) (expr);

#define PRIM(s) ((routine)(GET(s,0)))

void evol (expr ctx)
{
/* int (*routine)(expr);*/
 routine r;
 expr instr;
 int status;
trace("evol",0);
  instr = GET(PC(ctx),0);
  /*instr = ((expr *)ctx)[0];*/
/* routine = (int (*)(expr)) GET(instr,0);*/
 r = PRIM(instr);
 status = (*r)(ctx);

 /*PC(ctx)++;*/
 ((expr*)ctx)[pPC]++;
trace("end evol",0);
}

int def0 (expr ctx)
{
 expr instr;
trace("def0",0);
 instr = GET(PC(ctx),0);
/* instr = ((expr*)ctx)[0];*/
 /*STK(ctx)*/
 ((expr*)ctx)[pSTK] = cons (instr, STK(ctx));
 return 0;
}

int instr_goto (expr ctx)
{
 expr x;
 x = GET(PC(ctx),1);
 /* PC(ctx)*/
 ((expr*)ctx)[pPC] = x-1;
 return 0;
}

int instr_gosub (expr ctx)
{
 expr x;
 /*RET(ctx) */
 ((expr*)ctx)[pRET]= cons (PC(ctx), RET(ctx));
 x = GET(PC(ctx),1);
 /*PC(ctx)*/ 
 ((expr*)ctx)[pPC] = x-1;
 return 0;
}

int instr_return (expr ctx)
{
/* PC(ctx) */
 ((expr*)ctx)[pPC]= CAR(RET(ctx));
 return 0;
}

int instr_print_symbol (expr ctx)
{
 print( NAME(GET(STK(ctx),0)));
 input(buf);
 return 0;
}

int instr_quit (expr ctx)
{
 exit(0);
}


expr symbol (char *s)
{
 trace(s,1);
 int i;

 for (i=0; i<nsymbols; i++)
 {
  trace("boucle symbol",i);
  if (!strcmp(s,NAME(symbols[i])))
   return symbols[i];
  if (nsymbols >= MAXSYMBOLS)
  {
   error ("Symbol overflow");
   quit();
  }
 }
trace("non trouvé",0);
 expr x = alloc (2*sizeof(void *)+strlen(s)+1);
 /*GET(x,0)*/ ((expr*)x)[0] = (expr)def0;
 /*GET(x,1)*/ ((expr*)x)[1] = x;
 strcpy (NAME(x), s);
 symbols[nsymbols++] = x;
trace("ret sym",0);
 return x;
 
}


/* #define DEFPRIM(s,p) x = symbol(s); PRIM(x) = p; */
#define DEFPRIM(s,p) x=symbol(s); ((expr*)x)[0]=(expr)p;

void init_prim ()
{
 expr x;
 /*
 x = symbol("print_symbol");
 GET(x,0) = (expr)instr_print_symbol;
 */
 /*PRIM(x) = instr_print_symbol;*/
 DEFPRIM ("print_symbol", instr_print_symbol)
 DEFPRIM ("quit", instr_quit)

}

expr lex[256];

char *ptr;

char curchar ()
{
trace(ptr,3);
 return *ptr;
}

void nextchar ()
{
 ptr++;
 trace(ptr,4);
}
 
expr a[30][120];
int n[30];

expr readexpr ()
{
 int i, j, v, l;
 expr x;
 trace ("readexpr begin",0);
 l = -1;

 for (;;)
 {
  trace ("Level",l);
  while (curchar() == ' ')
   nextchar();
  trace ("switch",0);
  x = NULL;
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
      v = atoi (buf);
      /*sscanf (buf, "%d", &v);*/
      x = (expr)v;
      break;
     }
     buf[i++] = curchar();
     nextchar();
    }         
    
   case '[':
    trace("begin vector",0);
    nextchar();
    l++;
    n[l] = 0;
    break;

   case ']':
    trace("end vector",n[l]);
    nextchar();
    x = alloc(n[l]*sizeof(expr));
    for (j=0; j<n[l]; j++)
    {
trace("copy vector",j);
     ((expr*)x)[j]=a[l][j];
    }
    l--;
trace("new level --",l);
    break;
    

   default:
    i = 0;
    for (;;)
    {
trace("read symbol",i);
     if (curchar() == ' ')
     {
      nextchar();
      buf[i] = 0;
trace(buf,0);
      x = symbol(buf);
      break;
     }
     buf[i++] = curchar();
     nextchar();
    }

  }
  trace("level",l);
  if (x != NULL)
  {
   if (l<0)
    return x;
   else
   {
    a[l][n[l]++]=x;   
   }
  }
 }
}
 
expr readexpr1 ()
{
 int i, j;
 int v;
 expr x;
 expr a[250];
trace("readexpr",0);
 switch (curchar())
 {
  case ' ':
   nextchar();
   x = readexpr();
   return x;
  case '#':
   nextchar();
   i = 0;
   for (;;)
   {
    if (curchar() == '#')
    {
     buf[i] = 0;
     v = atoi (buf);
     /*sscanf (buf, "%d", &v);*/
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
trace("read vect",i);
    if (curchar() == ']')
    {
trace("end vect",0);
	 nextchar();
trace("alloc vect",i);
     x = alloc (i * sizeof(expr));
trace("copy vect...",i);
     for (j=0; j<i; j++) {
trace("copy vect",j);
      /*GET(x,j) */ ((expr*)x)[j]= a[j];
     }
     return x;
    }
trace("suite",0);
    x = readexpr();
    a[i++] = x;
   }
   break;
  default:
   i = 0;
   for (;;)
   {
trace("read symbol",i);
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

expr compil (char *s)
{
 expr x;
 ptr = s;
 x = readexpr();
 return x;
}

 

int fv(int argc, char *argv[])
{
 
expr ctx;
print("debut fv"); 
input(buf);
trace("fv",0);
  print ("Bonjour !\n");
  trace("init",0);
  init();
  trace ("init_prim",0);
  init_prim();
  trace ("compil",0);
  ctx = compil ("[[rebonjour print_symbol quit ] nil nil nil ]");
  for (;;)
  {
   evol(ctx);
  }

  gets (buf);
  return 0;
}
 
int main (int argc, char* argv[])
{
	print("Hello World!\n");
          input(buf);
trace("Test",1234);
print("appel fv");
input(buf);
	fv(argc,argv);
	return 0;
}

