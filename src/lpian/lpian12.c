
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int sexpr;

#define NFLAGS 1
#define TYPESIZE 4

#define UTVAL(x) ((x)>>(NFLAGS+TYPESIZE))
#define TVAL(x) ((x)>>NFLAGS)
#define TYPE(x) (((x)>>NFLAGS)&((1<<TYPESIZE)-1))

#define SEXPR(t,v) (((t)<<NFLAGS)|((v)<<(NFLAGS+TYPESIZE)))

#define NSYSVARS 0x100
#define NPAIRS 0x1000
#define NPSEXPR 0x100
#define NNSEXPR 0x400
#define NSTRINGS 0x800
#define SIZESTRINGS 0x4000

#define TYPE_AS         0
#define TYPE_INSTR      1
#define TYPE_INT        2
#define TYPE_STRING     4
#define TYPE_GINT       5
#define TYPE_REAL       6
#define TYPE_COMPLEX    7
#define TYPE_PAIR       8
#define TYPE_VECTOR     12
#define TYPE_SYMBOL     14
#define TYPE_GENSYM	15

#define SYSVAR_PRINTLEVEL 21

int sysvars[NSYSVARS];

struct pair
{
 sexpr car;
 sexpr cdr;
};

int pairptr;
struct pair pairs[NPAIRS];

int npsexpr;
sexpr psexpr[NPSEXPR];

int nnsexpr;
sexpr nsexpr[NNSEXPR];

#define CAR(x) (pairs[UTVAL(x)].car)
#define CDR(x) (pairs[UTVAL(x)].cdr)

int nstrings;
char *string_ptr;

char *strings[NSTRINGS];
char strings_data[SIZESTRINGS];

sexpr defs[NSTRINGS];
#define DEF(x) (defs[UTVAL(x)])

sexpr nil, tru;

char rc;

int trace_step_on;

void printstd (sexpr x);

void error (char *s, sexpr x)
{
 printf ("\n*** Error : %s : ", s, x);
 printstd (x);
 printf ("\n");
 exit(0);
}

int symbolp (sexpr x)
{
 return (TYPE(x)==TYPE_SYMBOL) || (TYPE(x)==TYPE_GENSYM);
}

sexpr def (sexpr x)
{
 if (TYPE(x) == TYPE_SYMBOL)
  return DEF(x);
 error ("def of non symbol", x);
 return nil;
}

sexpr sexpr_int (int i)
{
 sexpr x;
 x = SEXPR(TYPE_INT,i);
 return x;
}

sexpr sexpr_logical (int l)
{
 if (l) 
  return sexpr_int(l);
 else
  return nil;
}

int int_sexpr (sexpr x)
{
 if (TYPE(x) == TYPE_INT)
  return UTVAL(x);
 else
  error ("int expected", x);
}

int new_string (char *s)
{
 int r;
 if (string_ptr + strlen(s) >= strings_data + SIZESTRINGS)
  error ("Strings overflow", 0);
 else
 {
  strcpy (string_ptr, s);
  strings[nstrings++] = string_ptr;
  string_ptr += strlen(s) + 1;
  r = nstrings-1;
  defs[r] = nil;
  /* printf (" new_string %d (%s) (%s) (%s) ", r, s, string_ptr, strings[r]);  */
  return r;
 }
}

int new_string_length (int n)
{
 int r;
 int i;
 if (string_ptr + n >= strings_data + SIZESTRINGS)
  error ("Strings overflow", 0);
 else
 {
  for (i=0; i<=n; i++)
   string_ptr[i] = 0;
  strings[nstrings++] = string_ptr;
  string_ptr += n + 1;
  r = nstrings-1;
  defs[r] = nil;
  /* printf (" new_string %d (%s) (%s) (%s) ", r, s, string_ptr, strings[r]);  */
  return r;
 }
}

sexpr sexpr_string (char *s)
{
 int i;
 sexpr x;
 i = new_string (s);
 x = SEXPR(TYPE_STRING,i);
 return x;
}

sexpr new_symbol (char *s)
{
 int i;
 sexpr x;
 i = new_string (s);
 x = SEXPR(TYPE_SYMBOL,i);
 DEF(x) = SEXPR(TYPE_INSTR,i);
 return x;
}

sexpr symbol (char *s)
{
 int i;
 sexpr x;
 for (i=0; i<nstrings; i++)
 {
  if (!strcmp(s,strings[i]))
  {
   x = SEXPR(TYPE_SYMBOL,i);
   return x;
  }
 }
 x = new_symbol(s);
 return x;
}

sexpr new_instr (char *s)
{
 int i;
 sexpr x;
 i = new_string (s);
 x = SEXPR(TYPE_INSTR,i);
 return x;
}

sexpr instruction (char *s)
{
 int i;
 sexpr x;
 for (i=0; i<nstrings; i++)
 {
  if (!strcmp(s,strings[i]))
  {
   x = SEXPR(TYPE_INSTR,i);
   return x;
  }
 }
 x = new_instr(s);
 return x;
}

sexpr string (char *s)
{
 int i;
 sexpr x;
 i = new_string (s);
 x = SEXPR(TYPE_STRING,i);
 return x;
}

char *str_sexpr (sexpr x)
{
 return strings[UTVAL(x)];
}

char *str_symbol (sexpr x)
{
 if (TYPE(x) == TYPE_SYMBOL)
  return strings[UTVAL(x)];
 error ("symbol expected", x);
}

char *str_instr (sexpr x)
{
 if (TYPE(x) == TYPE_INSTR)
  return strings[UTVAL(x)];
 error ("instruction expected", x);
}

char *str_string (sexpr x)
{
 if (TYPE(x) == TYPE_STRING)
  return strings[UTVAL(x)];
 error ("symbol expected", x);
}

int ngensym;

sexpr gensym ()
{
 sexpr x;
 ngensym++;
 x = SEXPR(TYPE_GENSYM,ngensym);
 return x;
}

int consp (sexpr x)
{
 return TYPE(x) == TYPE_PAIR;
}

int pairp (sexpr x)
{
  return TYPE(x) == TYPE_PAIR;
}

sexpr car (sexpr x)
{
 if (consp(x))
  return CAR(x);
 else
  error ("car of not cons", x);
}

sexpr cdr (sexpr x)
{
 if (consp(x))
  return CDR(x);
 else
  error ("cdr of not cons", x);
}

#define ISOCC(i) (pairs[i].car&1)
#define SETOCC(i) (pairs[i].car |= 1)
#define CLROCC(i) (pairs[i].car &= -2)

void addnsexpr (sexpr x)
{
 if (nnsexpr >= NNSEXPR-1)
  error ("nsexpr overflow", x);
 else
 {
  nsexpr[nnsexpr++] = x;
 }
}

void clrnsexpr ()
{
 nnsexpr = 0;
}

void mark (sexpr x)
{
 if (pairp(x))
 {
  if (!ISOCC(UTVAL(x)))
  {
   SETOCC(UTVAL(x));
   mark(car(x));
   mark(cdr(x));
  }
 } 
}

void gc ()
{
 int i;
 for (i=0; i<NPAIRS; i++)
  CLROCC(i);
 for (i=0; i<npsexpr; i++)
  mark (psexpr[i]);
 for (i=0; i<nnsexpr; i++)
  mark (nsexpr[i]);
 for (i=0; i<nstrings; i++)
  mark(defs[i]);
 clrnsexpr();
 pairptr = 0; 
}

sexpr cons (sexpr a, sexpr b)
{
 sexpr c;
 while (ISOCC(pairptr) && pairptr < NPAIRS-1)
  pairptr++;
 if (ISOCC(pairptr))
 {
  gc();
  while (ISOCC(pairptr) && pairptr < NPAIRS-1)
   pairptr++; 
  if (ISOCC(pairptr))
   error ("pair overflow", 0);
 }
 c = SEXPR (TYPE_PAIR, pairptr);
 CAR(c) = a;
 CDR(c) = b;
 SETOCC(pairptr);
 addnsexpr(c);
 return c;
}

typedef struct output
{
 int (*f) (struct output *out, char c);
} *output;

struct output stdo[1];

int stdof (output out, char c)
{
 putchar (c);
 fflush (stdout);
}

void pchar (output out, char c)
{
 (*(out->f))(out,c);
}

void pstr (output out, char *s)
{
 while (*s)
  pchar (out, *s++);
}

typedef struct input
{
 int (*f) (struct input *in);
} *input;

struct input stdi[1];

int stdif (input in)
{
 char c;
 c=getchar();
 return c;
}

typedef struct file_input
{
 int (*f) (struct file_input *in);
 FILE *fh;
} *file_input;

int fileif (file_input in)
{
 int c;
 c = fgetc (in->fh);
 return c;
}

int gchar (input in)
{
 return (*(in->f))(in);
}


void init ()
{
 int i;
 nstrings = 0;
 string_ptr = strings_data;
 ngensym = 0;
 pairptr = 0;
 nnsexpr = 0;
 npsexpr = 0;
 for (i=0; i<NPAIRS; i++)
  CLROCC(i);
 stdo->f = stdof;
 stdi->f = stdif;
 nil = symbol ("nil");
 tru = symbol ("t");
 rc = ' ';

}

void print (output out, sexpr x, int l)
{
 char buf[1000];
 sexpr y;
 if (l < 0)
  pstr (out, "...");
 else
 switch (TYPE(x))
 {
  case TYPE_INT:
   sprintf (buf, "%d", UTVAL(x));
   pstr (out, buf);
   break;
  case TYPE_GENSYM:
   pchar (out, '%');
   sprintf (buf, "%d", UTVAL(x));
   pstr (out, buf);
   break; 
  case TYPE_SYMBOL:
   /* printf (" print:symbol "); */
   pstr (out, str_symbol(x));
   break;

  case TYPE_INSTR:
   pchar (out, '#');
   pstr (out, str_instr(x));
   break;

  case TYPE_STRING:
   pchar (out, '"');
   pstr (out, str_string(x));
   pchar (out, '"');
   break;

  case TYPE_PAIR:
   pchar (out, '(');
   y = x;
   for (;;)
   {
    l--;
    if (l < -1) 
    {
     pchar (out, ')');
     break; 
    }
    print (out, car(y), l);
    if (TVAL(cdr(y)) == TVAL(nil))
    {
     pchar (out, ')');
     break;
    }
    else if (!pairp(cdr(y)))
    {
     pstr (out, " . ");
     print (out, cdr(y), l);
     pchar (out, ')');
     break;
    }
    else
    {
     pchar (out, ' ');
     y = cdr(y);
    }
   } 
   break;

  default:
   sprintf (buf, "<%d:%d>", TYPE(x), UTVAL(x));
   pstr (out, buf);
   break;

 }
}

void printstd (sexpr x)
{
 print (stdo, x, 8);
}

void nextchar (input in)
{
 rc = gchar (in);
 if (rc == '{')
 {
  while (rc != '}')
   rc = gchar (in);
  rc = gchar (in);
 }
 /* putchar (rc); */
}

void space (input in)
{
 while (rc == ' ' || rc == '\r' || rc == '\n' || rc == '\t')
  nextchar(in);
}

sexpr readlist (input in, int c)
{
 sexpr x, y, z;
 space (in);
 if (rc == ')')
 {
  if (!c) 
   nextchar(in);
  return nil;
 }
 if (rc == '.')
 {
  nextchar (in);
  x = read (in);
  space (in);
  if (rc != ')')
   error ("expected only one expression between '.' and ')'", x);
  if (!c) 
   nextchar (in);
  return x;
 }
 x = read (in);
 y = readlist (in, c);
 z = cons (x, y);
 return z;
}

sexpr read (input in)
{
 char buf[1000];
 int i;
 int n;
 sexpr x, y;
 space (in);
 if (rc == '-' || (rc >= '0' && rc <= '9'))
 {
  i = 0;
  buf[i++] = rc;
  nextchar (in);
  while (rc >= '0' && rc <= '9')
  {
   buf[i++] = rc;
   nextchar (in);
  }
  buf[i++] = 0;
  sscanf (buf, "%d", &n);
  x = SEXPR(TYPE_INT,n);
  return x;
 }
 else if (rc == '(')
 {
  nextchar (in);
  x = readlist (in, 0);
  return x;
 }
 else if (rc == ':')
 {
  nextchar (in);
  x = readlist (in, 1);
  return x;
 }
 else if (rc == '\'')
 {
  nextchar(in);
  x = read(in);
  y = cons (symbol("QUOTE"), cons(x, nil));
  return y;
 }
 else if (rc == '%')
 {
  nextchar(in);
  x = read(in);
  y = cons (symbol("GET"), cons(x, nil));
  return y;
 }
 else if (rc == '&')
 {
  nextchar(in);
  x = read(in);
  y = cons (symbol("VAR"), cons(x, nil));
  return y;
 }
 else if (rc == '#')
 {
  i=0;
  nextchar(in);
  while (rc!=' ' && rc!='\n' && rc!='\r' && rc!='\t' && rc!=')' && rc!='.')
  {
   buf[i++] = rc;
   nextchar(in);
  }
  buf[i++] = 0;
  x = instruction (buf);
  return x;
 }
 else if (rc == '"')
 {
  i=0;
  nextchar(in);
  while (rc != '"')
  {
   buf[i++] = rc;
   nextchar(in);
  }
  buf[i++] = 0;
  nextchar(in);
  x = string (buf);
  return x;
 }
 else
 {
  /* printf (" symbol "); */
  i=0;
  buf[i++] = rc;
  nextchar(in);
  while (rc!=' ' && rc!='\n' && rc!='\r' && rc!='\t' && rc!=')' && rc!='.')
  {
   buf[i++] = rc;
   nextchar(in);
  }
  buf[i++] = 0;
  /* printf ("buf=%s\n",buf); */
  x = symbol (buf);
  return x;
 } 
}

void test (void)
{
 sexpr x;
 init();
 for (;;)
 {
  printf ("\n? ");
  x = read (stdi);
  printf ("\ntype %d, value %d = ", TYPE(x), UTVAL(x));
  print (stdo, x, 8);
  /* printf ("\nstring=%s ", strings[UTVAL(x)]); */
 }
}

#define strat 	(psexpr[0])
#define prog 	(psexpr[1])
#define stack	(psexpr[2])
#define rstack	(psexpr[3])
#define envir	(psexpr[4])

int cont;

sexpr append (sexpr a, sexpr b)
{
 if (pairp(a))
  return cons (car(a), append (cdr(a), b));
 return b;
}

int eq (sexpr x, sexpr y)
{
 return (TVAL(x) == TVAL(y));
}

sexpr sexpr_eq (sexpr x, sexpr y)
{
 if (TVAL(x) == TVAL(y))
  return tru;
 else
  return nil;
}

sexpr not (sexpr x)
{
 if (TVAL(x) == TVAL(nil))
  return tru;
 else
  return nil;
}

int istrue (sexpr x)
{
 if (TVAL(x) == TVAL(nil))
  return 0;
 else
  return 1;
}

sexpr nth (sexpr x, int i)
{
 if (!pairp(x))
 {
  error ("nth of non pair", x);
  return nil;
 }
 else
 {
  if (i <= 0)
   return car(x);
  else
   return nth (cdr(x), i-1);
 }
}

int length (sexpr x)
{
 if (!consp(x))
  return 0;
 return 1 + length(cdr(x));
}

sexpr last (sexpr x)
{
 if (!consp(x))
  return nil;
 if (!consp(cdr(x)))
  return x;
 return last(cdr(x));
}

sexpr reverse (sexpr x)
{
 if (!consp(x))
  return x;
 return append (reverse(cdr(x)), cons(car(x),nil));
}

sexpr memq (sexpr x, sexpr l)
{
 if (!consp(l)) 
  return nil;
 if (eq(x,car(l)))
  return l;
 return memq(x,cdr(l));
}

int equal (sexpr x, sexpr y)
{
 if (eq(x,y))
  return 1;
 if (!consp(x))
  return 0;
 if (!consp(y))
  return 0;
 if (!equal(car(x),car(y)))
  return 0;
 if (!equal(cdr(x),cdr(y)))
  return 0;
 return 1;
}

sexpr member (sexpr x, sexpr l)
{
 if (!consp(l))
  return nil;
 if (equal(x,car(l)))
  return l;
 return member (x, cdr(l));
}

sexpr remq (sexpr x, sexpr l)
{
 if (!consp(l))
  return l;
 if (eq(x,car(l)))
  return cdr(l);
 return cons (car(l), remq(x,cdr(l)));
}

sexpr remov (sexpr x, sexpr l)
{
 if (!consp(l))
  return l;
 if (equal(x,car(l)))
  return cdr(l);
 return cons (car(l), remov(x,cdr(l)));
}

int inclq (sexpr a, sexpr b)
{
 sexpr x;
 if (!consp(a))
  return 1;
 x = memq (car(a), b);
 return istrue(x) && inclq (cdr(a), b);
}

int incl (sexpr a, sexpr b)
{
 sexpr x;
 if (!consp(a))
  return 1;
 x = member (car(a), b);
 return istrue(x) && incl (cdr(a), b);
}

sexpr getvenv (sexpr env, sexpr var)
{
 sexpr x, y;
 if (!consp(env))
  return nil;
 if (eq(car(car(env)),var))
  return car(cdr(car(env)));
 x = cdr(env);
 y = getvenv (x, var);
 return y;
 // return getvenv (cdr(env), var);
}

sexpr setvenv (sexpr env, sexpr var, sexpr val)
{
 if (!consp(env))
  return cons (cons(var, cons(val,nil)), tru);
 if (eq(car(car(env)),var))
  return cons(cons(var,cons(val,cdr(cdr(car(env))))),cdr(env));
 return cons (car(env), setvenv(cdr(env),var,val));
}

sexpr addvenv (sexpr env, sexpr var, sexpr val)
{
 if (!consp(env))
  return cons (cons(var, cons(val,nil)), tru);
 if (eq(car(car(env)),var))
  return cons(cons(var,cons(val,cdr(car(env)))),cdr(env));
 return cons (car(env), addvenv(cdr(env),var,val));
}

sexpr remvenv (sexpr env, sexpr var)
{
 if (!consp(env))
  return env;
 if (eq(car(car(env)),var))
 {
  sexpr x;
  x = cdr(cdr(car(env)));
  if (!consp(x))
   return cdr(env);
  return cons (cons(var,x), cdr(env));
 }
 return cons (car(env), remvenv (cdr(env), var));
}

sexpr getvsenv (sexpr env, sexpr vars)
{
 if (!consp(vars))
  return getvenv (env, vars);
 if (eq (car(vars), symbol("QUOTE")) && consp(cdr(vars)))
  return car(cdr(vars));
 return cons (getvsenv(env,car(vars)), getvsenv(env,cdr(vars)));
}

sexpr bindvenv (sexpr env, sexpr vars, sexpr vals)
{
 sexpr a, b;
 if (!istrue(vars))
  return env;
 if (!consp(vars))
  return addvenv (env, vars, vals);
 a = consp(vals) ? car(vals) : vals;
 b = consp(vals) ? cdr(vals) : vals;
 return bindvenv (bindvenv(env,car(vars),a),cdr(vars),b);
}

sexpr unbindvenv (sexpr env, sexpr vars)
{
 if (!istrue(vars))
  return env;
 if (!consp(vars))
  return remvenv (env, vars);
 return unbindvenv (unbindvenv(env,car(vars)), cdr(vars));
}

sexpr create_var (sexpr x)
{
 return cons (symbol("VAR"), cons(x, nil));
}

sexpr var_name (sexpr x)
{
 return car(cdr(x));
}

sexpr is_var (sexpr x)
{
 return (consp(x) && eq(symbol("VAR"),car(x)));
}

sexpr is_anovar (sexpr x)
{
 return is_var(x) && eq (nil, var_name(x));
}

int boundvenv (sexpr env, sexpr var)
{
 if (!consp(env))
  return 0;
 if (eq(car(car(env)),var)
  && consp(cdr(car(env)))
  && !eq(car(cdr(car(env))),symbol("UNDEF")))
  return 1;
 return boundvenv (cdr(env), var);
}

sexpr getrecvenv (sexpr env, sexpr var)
{
 sexpr val;
 if (!boundvenv(env,var))
  return create_var(var);
 val = getvenv (env, var);
 if (!is_var(val))
  return val;
 return getrecvenv (env, var_name(val));
}

sexpr set (sexpr l)
{
 if (!consp(l))
  return l;
 if (istrue(memq(car(l),cdr(l))))
  return set (cdr(l));
 return cons (car(l), set(cdr(l)));
}

sexpr variables1 (sexpr x)
{
 if (is_var(x))
  return cons (var_name(x), nil);
 if (!consp(x))
  return nil;
 return append (variables1(car(x)), variables1(cdr(x)));
}

sexpr variables (sexpr x)
{ 
 return set (variables1(x));
}

sexpr subst (sexpr x, sexpr y, sexpr z)
{
 if (eq(y,z))
  return x;
 if (!consp(z))
  return z;
 return cons (subst(x,y,car(z)), subst(x,y,cdr(z)));
}

sexpr rename1 (sexpr x, sexpr vars)
{
 if (!consp(vars))
  return x;
 return rename1 (subst(gensym(),car(vars),x),cdr(vars));
}

sexpr renam (sexpr x)
{
 return rename1 (x, variables(x));
}

sexpr build (sexpr env, sexpr x)
{
 if (is_var(x))
  return getrecvenv (env, var_name(x));
 if (!consp(x))
  return x;
 return cons (build(env,car(x)), build(env,cdr(x)));
}

sexpr link (sexpr env, sexpr var, sexpr val)
{
 return addvenv (env, var, val); 
}

sexpr unify (sexpr env, sexpr x, sexpr y);

#define OCCUR_CHECK 1

sexpr unify1 (sexpr env, sexpr x, sexpr y)
{
 if (is_anovar(x) || is_anovar(y) || equal(x,y))
  return env;
 if (is_var(x) && (!OCCUR_CHECK || !istrue(memq(var_name(x),variables(y)))))
  return link (env, var_name(x), y);
 if (is_var(y) && (!OCCUR_CHECK || !istrue(memq(var_name(y),variables(x)))))
  return link (env, var_name(y), x);
 if (!consp(x) || !consp(y))
  return nil;
 return unify (unify1(env,car(x),car(y)),cdr(x),cdr(y));
}

sexpr unify (sexpr env, sexpr x, sexpr y)
{
 if (!istrue(env))
  return env;
 return unify1 (env, build(env,x), build(env,y));
}

/* #include "definstr.h" */

#define DEFINSTR(str) } else if (!strcmp (str_sexpr(instr), str)) { 

void step (void);

void exec_instr (sexpr instr)
{
 sexpr x;
 int i;
 if (0) {

/* Combinators */
 DEFINSTR("I")
  ;
 DEFINSTR("K")
  prog = cons (car(prog), cdr(cdr(prog)));
 DEFINSTR("S")
  prog = cons (cons(car(prog),car(cdr(cdr(prog)))), cons(car(cdr(prog)), cons(car(cdr(cdr(prog))), cdr(cdr(cdr(prog))))));
 DEFINSTR("Y")
  prog = cons (car(prog), cons(cons(instr,cons(car(prog),nil)), cdr(prog)));
 DEFINSTR("B")
  prog = cons (car(prog), cons (cons(car(cdr(prog)), cons (car(cdr(cdr(prog))), nil)), cdr(cdr(prog))));
 DEFINSTR("C")
  prog = cons (car(prog), cons (car(cdr(cdr(prog))), cons (car(cdr(prog)), cdr(cdr(cdr(prog))))));
 DEFINSTR("W")
  prog = cons (car(prog), cons (car(cdr(prog)), cons (car(cdr(prog)), cdr(cdr(prog)))));
 DEFINSTR("APPLYTO")
  prog = cons (car(cdr(prog)), cons (car(prog), cdr(cdr(prog))));
 DEFINSTR("P")
  prog = cons (car(cdr(cdr(prog))), cons (car(prog), cons (car(cdr(prog)), cdr(cdr(cdr(prog))))));
 DEFINSTR("J")
  prog = cdr(prog);

/* Quote */
 DEFINSTR("QUOTE")
  stack = cons (car(prog), stack);
  prog = cdr(prog);
 DEFINSTR("Q")
  stack = cons (car(prog), stack);
  prog = cdr(prog);

/* Stack manipulation */
 DEFINSTR("DEP")
  stack = cdr(stack);
 DEFINSTR("REP")
  stack = cons (car(stack), stack);
 DEFINSTR("ECH")
  stack = cons (car(cdr(stack)), cons (car(stack), cdr(cdr(stack))));
 DEFINSTR("DES")
  rstack = cons (car(stack), rstack);
  stack = cdr(stack);
 DEFINSTR("MON")
  stack = cons (car(rstack), stack);
  rstack = cdr(rstack);
 DEFINSTR("MONDEP")
  rstack = cdr(rstack);
 DEFINSTR("GETH")
  stack = cons (nth(rstack,int_sexpr(car(prog))), stack);
  prog = cdr(prog);

/* Tests */
 DEFINSTR("THEN")
  if (istrue(car(stack)))
   prog = cons (car(prog), cdr(cdr(prog)));
  else
   prog = cdr(prog);
  stack = cdr(stack);
 DEFINSTR("NCONSPTHEN")
  if (!pairp(car(stack)))
   prog = cons (car(prog), cdr(cdr(prog)));
  else
   prog = cdr(prog);
  stack = cdr(stack);
 
/* Execution */
 DEFINSTR("EXEC")
  prog = cons (car(stack), prog);
  stack = cdr(stack);

/* Definition */
 DEFINSTR("GETDEF")
  if (TYPE(car(stack))!=TYPE_SYMBOL)
   error ("GETDEF of non symbol", car(stack));
  else
   stack = cons (DEF(car(stack)), cdr(stack));
 DEFINSTR("SETDEF")
  if (TYPE(car(stack))!=TYPE_SYMBOL)
   error ("SETDEF of non symbol", car(stack));
  else
  {
   DEF(car(stack)) = car(cdr(stack));
   stack = cdr(cdr(stack));
  }
 DEFINSTR("def")
  if (TYPE(car(prog))!=TYPE_SYMBOL)
   error ("def of non symbol", car(prog));
  else
  {
   DEF(car(prog)) = car(cdr(prog));
   prog = cdr(cdr(prog));
  }

/* Primitives */
 DEFINSTR("CAR")
  stack = cons (car(car(stack)), cdr(stack));
 DEFINSTR("CDR")
  stack = cons (cdr(car(stack)), cdr(stack));
 DEFINSTR("CONS")
  stack = cons (cons(car(stack),car(cdr(stack))), cdr(cdr(stack)));
 DEFINSTR("EQ")
  stack = cons (sexpr_eq(car(stack),car(cdr(stack))), cdr(cdr(stack)));
 DEFINSTR("GETTYPE")
  stack = cons (TYPE(car(stack)), cdr(stack));
 DEFINSTR("SETTYPE")
  stack = cons (SEXPR(int_sexpr(car(stack)),UTVAL(car(cdr(stack)))), cdr(cdr(stack)));
 DEFINSTR("RPLACA")
  if (!pairp(car(stack)))
   error ("RPLACA of non cons", car(stack));
  else
  {
   CAR(car(stack)) = car(cdr(stack));
   SETOCC(UTVAL(car(stack)));
   stack = cdr(cdr(stack));
  }
 DEFINSTR("RPLACD")
  if (!pairp(car(stack)))
   error ("RPLACD of non cons", car(stack));
  else
  {
   CDR(car(stack)) = car(cdr(stack));
   stack = cdr(cdr(stack));
  } 
 DEFINSTR("NOT")
  stack = cons (not(car(stack)), cdr(stack));
 DEFINSTR("PLUS")
  stack = cons (sexpr_int(int_sexpr(car(cdr(stack)))+int_sexpr(car(stack))), cdr(cdr(stack)));
 DEFINSTR("MOINS")
  stack = cons (sexpr_int(int_sexpr(car(cdr(stack)))-int_sexpr(car(stack))), cdr(cdr(stack)));
 DEFINSTR("FOIS")
  stack = cons (sexpr_int(int_sexpr(car(cdr(stack)))*int_sexpr(car(stack))), cdr(cdr(stack)));
 DEFINSTR("DIV")
  stack = cons (sexpr_int(int_sexpr(car(cdr(stack)))/int_sexpr(car(stack))), cdr(cdr(stack)));
 DEFINSTR("MOD")
  stack = cons (sexpr_int(int_sexpr(car(cdr(stack)))%int_sexpr(car(stack))), cdr(cdr(stack)));
 DEFINSTR("ETL")
  stack = cons (sexpr_int(int_sexpr(car(cdr(stack)))&int_sexpr(car(stack))), cdr(cdr(stack)));
 DEFINSTR("OUL")
  stack = cons (sexpr_int(int_sexpr(car(cdr(stack)))|int_sexpr(car(stack))), cdr(cdr(stack)));
 DEFINSTR("OXL")
  stack = cons (sexpr_int(int_sexpr(car(cdr(stack)))^int_sexpr(car(stack))), cdr(cdr(stack)));
 DEFINSTR("GENSYM")
  stack = cons (gensym(), stack);

/* Strings */
 DEFINSTR("NEWSTR")
  stack = cons (new_string_length(int_sexpr(car(stack))), cdr(stack));
 DEFINSTR("GETSTRCHAR")
  sexpr str;
  int pos;
  char *s;
  char c;
  str = car(stack);
  pos = int_sexpr(car(cdr(stack)));
  s = str_sexpr (str);
  c = s[pos];
  stack = cons (sexpr_int(c), cdr(cdr(stack))); 
 DEFINSTR("SETSTRCHAR")
  sexpr str;
  int pos;
  char c;
  char *s;
  str = car(stack);
  pos = int_sexpr(car(cdr(stack)));
  c = int_sexpr(car(cdr(cdr(stack))));
  s = str_sexpr (str);
  s[pos] = c;
  stack = cdr(cdr(cdr(stack)));
  
/* Arithmetic tests */
 DEFINSTR("ZEROP")
  if ((TYPE(car(stack)) == TYPE_INT) && (UTVAL(car(stack)) == 0))
   stack = cons (tru, cdr(stack));
  else
   stack = cons (nil, cdr(stack));
 DEFINSTR("POSP")
  if ((TYPE(car(stack)) == TYPE_INT) && (UTVAL(car(stack)) >= 0))
   stack = cons (tru, cdr(stack));
  else
   stack = cons (nil, cdr(stack));

/* Input Output */
 DEFINSTR("TYI")
  i = getchar();
  stack = cons (sexpr_int(i), stack);
 DEFINSTR("TYO")
  putchar (int_sexpr(car(stack)));
  stack = cdr(stack); 
 DEFINSTR("PRINTSTRING")
  printf ("%s", str_string(car(stack)));
  stack = cdr(stack);
 DEFINSTR("READ")
  x = read (stdi);
  stack = cons (x, stack);
 DEFINSTR("PRINT")
  x = car(stack);
  print (stdo, x, 16); 
 DEFINSTR("LOAD")
  char *filename;
  FILE *fh;
  x = car(stack);
  filename = str_sexpr(x);
  fh = fopen (filename, "r");
  if (fh == NULL)
  {
   printf ("\nLOAD : Cannot open file \"%s\"\n", filename);
   stack = cdr(stack);
  }
  else
  {
   struct file_input in[1];
   in->f = fileif;
   in->fh = fh;
   x = read ((input)in);
   prog = cons (x, prog);
   stack = cdr(stack);
  }

/* Context access */
 DEFINSTR("GETSTRAT")
  stack = cons (strat, stack);
 DEFINSTR("SETSTRAT")
  strat = car(stack);
  stack = cdr(stack);
 DEFINSTR("GETPROG")
  stack = cons (prog, stack);
 DEFINSTR("SETPROG")
  prog = car(stack);
  stack = cdr(stack);
 DEFINSTR("GETSTACK")
  stack = cons (stack, stack);
 DEFINSTR("SETSTACK")
  stack = car(stack);
 DEFINSTR("GETENV")
  stack = cons (envir, stack);
 DEFINSTR("SETENV")
  envir = car(stack);
  stack = cdr(stack);
 DEFINSTR("STEP")
  step ();

/* Environment */
 DEFINSTR("GETVENV")
  stack = cons (getvenv(car(stack),car(cdr(stack))), cdr(cdr(stack)));
 DEFINSTR("SETVENV")
  stack = cons (setvenv(car(stack),car(cdr(stack)),car(cdr(cdr(stack)))), cdr(cdr(cdr(stack))));
 DEFINSTR("ADDVENV")
  stack = cons (addvenv(car(stack),car(cdr(stack)),car(cdr(cdr(stack)))), cdr(cdr(cdr(stack))));
 DEFINSTR("REMVENV")
  stack = cons (remvenv(car(stack),car(cdr(stack))), cdr(cdr(stack)));
 DEFINSTR("GETVSENV")
  stack = cons (getvsenv(car(stack),car(cdr(stack))), cdr(cdr(stack)));
 DEFINSTR("BINDVEND")
  stack = cons (bindvenv(car(stack),car(cdr(stack)),car(cdr(cdr(stack)))), cdr(cdr(cdr(stack))));
 DEFINSTR("UNBINDVENV")
  stack = cons (unbindvenv(car(stack),car(cdr(stack))), cdr(cdr(stack)));
 DEFINSTR("GET")
  stack = cons (getvsenv(envir,car(prog)), stack);
  prog = cdr(prog);
 DEFINSTR("GETVSQ")
  stack = cons (getvsenv(envir,car(prog)), stack);
  prog = cdr(prog);
 DEFINSTR("SETVQ")
  envir = setvenv (envir, car(prog), car(stack));
  prog = cdr(prog);
  stack = cdr(stack);
 DEFINSTR("ARG")
  envir = bindvenv (envir, car(prog), car(stack));
  stack = cdr(stack);
  prog = cons (car(cdr(prog)), cons(cons(instruction("UNBINDVQ"),cons(car(prog),nil)), cdr(cdr(prog))));
 DEFINSTR("UNBINDVQ")
  envir = unbindvenv (envir, car(prog));
  prog = cdr(prog);

/* List utilities */
 DEFINSTR("LENGTH")
  stack = cons (sexpr_int(length(car(stack))), cdr(stack));
 DEFINSTR("LAST")
  stack = cons (last(car(stack)), cdr(stack));
 DEFINSTR("REVERSE")
  stack = cons (reverse(car(stack)), cdr(stack));
 DEFINSTR("EQUAL")
  stack = cons (sexpr_logical(equal(car(stack),car(cdr(stack)))), cdr(cdr(stack)));
 DEFINSTR("MEMQ")
  stack = cons (memq(car(stack),car(cdr(stack))), cdr(cdr(stack)));
 DEFINSTR("MEMBER")
  stack = cons (member(car(stack),car(cdr(stack))), cdr(cdr(stack)));
 DEFINSTR("REMQ")
  stack = cons (remq(car(stack),car(cdr(stack))), cdr(cdr(stack)));
 DEFINSTR("REMOVE")
  stack = cons (remov(car(stack),car(cdr(stack))), cdr(cdr(stack)));
 DEFINSTR("INCLQ")
  stack = cons (inclq(car(stack),car(cdr(stack))), cdr(cdr(stack)));
 DEFINSTR("INCL")
  stack = cons (incl(car(stack),car(cdr(stack))), cdr(cdr(stack)));

#define INSTR1(f) stack = cons (f(car(stack)), cdr(stack));
#define INSTR2(f) stack = cons (f(car(stack),car(cdr(stack))), cdr(cdr(stack)));
#define INSTR3(f) stack = cons (f(car(stack),car(cdr(stack)),car(cdr(cdr(stack)))), cdr(cdr(cdr(stack)))); 

 DEFINSTR("APPEND") INSTR2(append)

 DEFINSTR("BOUNDVENV") INSTR2(boundvenv)
 DEFINSTR("GETRECVENV") INSTR2(getrecvenv)
 DEFINSTR("ENSEMBLE") INSTR1(set)
 DEFINSTR("VARIABLES") INSTR1(variables)
 DEFINSTR("SUBST") INSTR3(subst)
 DEFINSTR("RENAME") INSTR1(renam)
 DEFINSTR("CONSTR") INSTR2(build)
 DEFINSTR("UNIF") INSTR3(unify)
 
  
/* Trace */
 DEFINSTR("TRACESTEP")
  trace_step_on = int_sexpr (car(stack));
  stack = cdr(stack);

/* Quit */
 DEFINSTR("QUIT")
  cont = 0;

 }
 else
 {
  error ("undefined instruction", instr);
 }
}

void trace_step (void)
{
 printf ("\n\tprog\t= ");
 print (stdo, prog, 8);
 printf ("\n\tstack\t= ");
 print (stdo, stack, 8);
 printf ("\n\trstack\t= ");
 print (stdo, rstack, 8);
 printf ("\n\tenvir\t= ");
 print (stdo, envir, 8);
 printf ("\n");
}

void step (void)
{
 sexpr instr;
 sexpr d;
 if (trace_step_on)
  trace_step ();
 instr = car(prog);
 prog = cdr(prog);
 switch (TYPE(instr))
 {
  case TYPE_INSTR:
   exec_instr (instr);
   break;
  case TYPE_PAIR:
   prog = append (instr, prog);
   break;
  case TYPE_SYMBOL:
   d = def(instr);
   prog = cons (d, prog);
   break;
  default:
   stack = cons (instr, stack);
   break;
 }
}

void init_interp (void)
{
 npsexpr = 5;
 strat = nil;
 prog = cons (instruction("Y"), cons (cons (instruction("READ"), cons (instruction("EXEC"), nil)), nil));
 prog = cons (sexpr_string("init.lpi"), 
  cons (instruction("LOAD"),
   cons (instruction("Y"),
    cons (cons(instruction("READ"),cons(instruction("EXEC"),nil)),
     nil))));
/*
 prog = cons (instruction("Y"), cons (cons  (instruction("READ"), cons (instruction("EXEC"), cons (sexpr_int(13), cons (instruction("TYO"), 
 cons (sexpr_int(10), cons (instruction("TYO"), nil)))))), nil)); 
*/
 stack = nil;
 rstack = nil;
 envir = tru;
 cont = 1;
 trace_step_on = 0;
}

void interp (void)
{
 init();
 init_interp();
 while (cont)
 {
  step ();
  clrnsexpr();
 }
}

main ()
{
 interp ();
}

