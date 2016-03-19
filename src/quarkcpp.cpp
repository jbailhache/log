
// #include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CPLUSPLUS 

void printasm (char *, unsigned int, unsigned int);

typedef unsigned char chr;
#define carac chr
typedef char byte;
typedef int instr;
typedef instr *codeptr;

#ifdef CPLUSPLUS
typedef codeptr (*codefn) (codeptr, union any);
#else
typedef codeptr (*codefn)();
#endif

typedef struct word *pword;

pword (*getwordf) (void);

typedef union any
{
 int i;
 codeptr cp;
 codefn f;
 void (*vf) (void);
 void (*mainloopf) (codeptr, pword(*)(void));
 chr *s;
} any;
 
any a;

#define WORDSIZE 32
#define DICSIZE 1000

struct codeparam
{
 any code;
 any param;
};

struct codeparam instbl[256];

struct word
{
 any code;
 any param;
 chr *pname;
 chr name[WORDSIZE];
};

/* typedef struct word *pword; */

int nwords = 0;

struct word dic[DICSIZE];

instr codezone[30000];
 
struct codeparam *lastcp;

#define STACKSIZE 5000
int stack[STACKSIZE+16];
int *ptrstack;

instr syscodebuf[5000];

/*
union cpf
{
 instr *cp;
 codefn f;
} syscode; 
*/

codeptr scp;

any syscode, cp_mainloop, cp_add;

chr getchr1 ()
{
 chr c;
 c = getchar();
 lastcp = instbl+c;
 return c;
}

chr (*pgetchr) (void) = getchr1;

#define getchr (*pgetchr)

void putchr (chr c)
{
 putchar(c);
}

void printdec (int i)
{
 printf (" %d ", i);
}

void printhex (int i)
{
 printf (" %X ", i);
}

/*

int writecharmem (any *p, carac c)
{
	*(p->pchar)++ = c;
}


int writecaracmem (any *p, carac c)
{
	*(p->pcarac)++ = c;
}

int writecaracfile (any *p, carac c)
{
	fputc (c, p->fd);
	fflush (p->fd);
}

int writecarac (carac c)
{
	(*(out.f)) (&(out.p), c);
}

int writestring (carac *s)
{
	TRACE "writestring <%s>\n", s ENDTRACE
	while (*s)
	{
		TRACE "writecarac '%c' %X\n", *s, *s ENDTRACE
		writecarac (*s++);
	}
}

int readcharmem (any *p)
{
	cr = *(p->pchar)++;
}

int readcaracmem (any *p)
{
	cr = *(p->pcarac)++;
	// swprintf (bufm, L"readcaracmem -> '%c' %X", cr, cr);
	// MessageBox (0, bufm, L"message", MB_OK);
}

int readcaracfile (any *p)
{
	cr = fgetc (p->fd);
	if (feof(p->fd))
		cr = 0;
}

carac readcarac (void)
{
loop:
	(*(instack[inlevel].f)) (&(instack[inlevel].p));
	// TRACE "read carac %X <%c>\n", cr, (char)cr ENDTRACE
	if (cr)
		return cr;	
	else if (inlevel > 0)
	{
		inlevel--;
		TRACE "inlevel %d\n", inlevel ENDTRACE
		goto loop;
	}
	else
		return 0;
}

*/

chr cr;
carac wr[WORDSIZE];
char awr[WORDSIZE];

chr readcarac (void)
{
 cr = getchr();
 return cr; 
}

int isblnk (carac c)
{
	if (c == ' ' || c == '\t' || c == '\n' || c == '\r')
		return 1;
	else
		return 0;
}

int iswordcarac (carac c)
{
	return (!isblnk(c) && c);
}

void readto (char delim, char *ptr)
{
 for (;;)
 {
  readcarac();
  if (cr==delim)
   break; 
  *ptr++ = cr;
 }
 *ptr++ = 0;
}
 
void readword (void)
{
	int i;
	readcarac();
	/* TRACE "readword: 1st cr = '%c' %X\n", cr, cr ENDTRACE */
	while (isblnk (cr))
	{
		readcarac();
		/* TRACE "skip blanks: cr = '%c' %X\n", cr, cr ENDTRACE */
	}
	i = 0;
	while (iswordcarac(cr))
	{
		/* TRACE "read cr = '%c' %X\n", cr, cr ENDTRACE */
		wr[i] = cr;
		awr[i] = cr;
		/* TRACE "awr = <%s>\n", awr ENDTRACE */
		i++;
		readcarac();
	}
	wr[i] = 0;
	awr[i] = 0;
	/* TRACE "Word read: <%s>\n", awr ENDTRACE */
}

void testreadword (void)
{
 for (;;)
 {
  readword();
  printf (" <%s> ", wr);
  if (!strcmp((char*)wr,"BYE"))
   break;
 }
}

pword lastword;

int scmp (chr *s1, chr *s2)
{
 return strcmp ((char*)s1, (char*)s2);
}

chr * scpy (chr *s1, chr *s2)
{
 return (chr*) strcpy ((char*)s1, (char*)s2);
}

codeptr compile_pushint (codeptr cp, int x);

instr *code_undef (instr *ptrcode, any p)
{
int i;
 i = 0;
 sscanf ((char *)p.s, "%d", &i);
 if (i == 0)
 {
  i = 1;
  sscanf ((char *)p.s, "%d", i);
  if (i == 1)
  {
   printf ("Undefined word: <%s>\n", (char *)p.s);
   return ptrcode;
  }
  else
  {
   ptrcode = compile_pushint (ptrcode, i);
   return ptrcode;
  }
 }
 else
 {
  ptrcode = compile_pushint (ptrcode, i);
  return ptrcode;
 }
}

pword getwordstr (chr *wr)
{
int i;
 /* printf("getwordstr(%s)\n",wr); */
 for (i=0; i<nwords; i++)
 {
  /* printf("%d %X <%s>\n", i, dic+i, dic[i].pname); */
  if (!scmp(wr,dic[i].pname))
  {
   /* printf("found\n"); */
   lastword = dic+i;
   return lastword;
  }
 }
 /* printf("not found\n"); */
 dic[nwords].pname = dic[nwords].name;
 scpy (dic[nwords].pname, wr);
 dic[nwords].code.f = code_undef;
 dic[nwords].param.s = dic[nwords].pname;
 lastword = dic+nwords;
 nwords++;
 return lastword;
}

pword getword (void)
{
 readword();
 return getwordstr(wr);
}

void dumpdic(void)
{
int i;
 for(i=0;i<5;i++)
  printf("%s %X %X %X\n", dic[i].pname, dic+i, dic[i].code.i, dic[i].param.i);
}

codeptr code_dumpdic (codeptr cp, any p)
{
 dumpdic();
 return cp;
}

void dump_asm (instr *begin, instr *end)
{
	char bufasm[300];
	instr *p;
	int i;
	printf ("\n");
	i=0;
	for (p=begin; p<=end; p++)
	{
		i++;
		if (i%16==0) getchar();
		printasm (bufasm, *p, (unsigned int)p);
		printf ("%2d %X: %08X %s\n", i, p, *p, bufasm);
	}
}

codeptr code_nop (codeptr cp, any param);
/*
void dumpdic (void)
{
 int c;
 int i;
 i=0;
 printf("\n");
 for (c=0; c<256; c++)
  if (instbl[c].code.f != code_nop)
  {
   printf ("%c %X %X\n",c,instbl[c].code.i,instbl[c].param.i);
   i++;
   if(i%8==0) getchar();
  }
}

codeptr code_dumpdic (codeptr cp, any p)
{
 dumpdic();
 return cp;
}
*/

void dumpcode (codeptr cp)
{
 codeptr p;
 char bufasm[300];
 printf("\n");
 for (p=codezone; p<=cp; p++)
 {
  /* printf ("%X: %08X\n", p, *p); */
printasm (bufasm, (unsigned int)(*p), (unsigned int)(p));
  printf ("%X: %08X %s\n", p, *p, bufasm);
 }
}

codeptr code_dumpcode (codeptr cp, any p)
{
 dumpcode(cp);
 return cp;
}

codeptr compile_pushint (codeptr cp, int x)
{
	/* TRACE2 "compile_pushint %X\n", x ENDTRACE2 */
	*cp++ = 0xE59F0000;
	*cp++ = 0xEA000000;
	*cp++ = x;
	*cp++ = 0xE92B0001;
	return cp;
}

instr *compile_movr (instr *ptrcode, int n, int x)
{
	*ptrcode++ = 0xE59F0000 + n * 0x1000;
	*ptrcode++ = 0xEA000000;
	*ptrcode++ = (instr)x;
	return ptrcode;
}

instr *compile_begindef (instr *ptrcode)
{
	*ptrcode++ = 0xE92D4000;
	return ptrcode;
}

instr *compile_enddef (instr *ptrcode)
{
	*ptrcode++ = 0xE8BD8000;
	return ptrcode;
}

instr instr_call (instr *ptrcode, int (*f)())
{
	union
	{
		int (*f)();
		int i;
	} uf;
	union
	{
		instr *pc;
		int i;
	} upc;
	unsigned int ofs;
	uf.f = f;
	upc.pc = ptrcode;
	ofs = (uf.i-(upc.i+8))/4;
	ofs &= 0xFFFFFF;
	return 0xEB000000 | ofs;
}

instr * compile_call (instr *ptrcode, int (*f)(), int regs, int res)
{
	if (regs)
		*ptrcode++ = 0xE8BB0000 | regs;
	*ptrcode = instr_call (ptrcode, f);
	ptrcode++;
	if (res)
		*ptrcode++ = 0xE92B0000 | res;
	return ptrcode;
}


int accu = 0;
int base = 10;

codeptr code_nop (codeptr cp, any param)
{
 return cp;
}

codeptr code_accu0 (codeptr cp, any param)
{
 /* printf(" lpar "); */
 accu = 0;
 return cp;
}

codeptr code_pushaccu (codeptr cp, any param)
{
 cp = compile_pushint (cp, accu);
 return cp;
}

/*
codeptr code_putcode (codeptr cp, any param)
{
 *cp++ = accu;
 return cp;
}
*/

codeptr code_putcode (codeptr cp, any param)
{
 pword pw;
 int i;
 pw = getword();
 sscanf ((char *)(pw->pname), "%X", &i);
 *cp++ = (instr)i;
 return cp;
}

codeptr code_here (codeptr cp, any param)
{
 cp = compile_pushint (cp, (int)cp);
 return cp;
}

codeptr code_lastcp(codeptr cp, any param)
{
 cp = compile_pushint (cp, (int)lastcp);
 return cp;
}

codeptr code_spget (codeptr cp, any param)
{
 *cp++ = 0xE92B0800;
 return cp;
}

codeptr code_spset (codeptr cp, any param)
{
 *cp++ = 0xE8BB0800;
 return cp;
}

codeptr code_hexnbr (codeptr cp, any param)
{
 pword pw;
 int i;
 pw = getword();
 sscanf ((char *)(pw->pname), "%x", &i);
 cp = compile_pushint (cp, i);
 return cp;
}

codeptr code_base (codeptr cp, any param)
{
 chr c;
 c = getchr();
 if (c<='9')
  base=c-'0';
 else
  base=10+c-'A';
 return cp;
}

codeptr code_0 (codeptr cp, any param)
{
 accu = accu * base;
 return cp;
}

codeptr code_1 (codeptr cp, any param)
{
 accu = accu * base + 1;
 return cp;
}

codeptr code_2 (codeptr cp, any param)
{
 accu = accu * base + 2;
 return cp;
}

codeptr code_3 (codeptr cp, any param)
{
 /* printf(" trois "); */
 accu = accu * base + 3;
 return cp;
}

codeptr code_4 (codeptr cp, any param)
{
 accu = accu * base + 4;
 return cp;
}

codeptr code_5 (codeptr cp, any param)
{
 accu = accu * base + 5;
 return cp;
}

codeptr code_6 (codeptr cp, any param)
{
 accu = accu * base + 6;
;
 return cp;
}

codeptr code_7 (codeptr cp, any param)
{
 accu = accu * base + 7;
 return cp;
}

codeptr code_8 (codeptr cp, any param)
{
 accu = accu * base + 8;
 return cp;
}

codeptr code_9 (codeptr cp, any param)
{
 accu = accu * base + 9;
 return cp;
}

codeptr code_A (codeptr cp, any param)
{
 accu = accu * base + 10;
 return cp;
}

codeptr code_B (codeptr cp, any param)
{
 accu = accu * base + 11;
 return cp;
}

codeptr code_C (codeptr cp, any param)
{
 accu = accu * base + 12;
 return cp;
}

codeptr code_D (codeptr cp, any param)
{
 accu = accu * base + 13;
 return cp;
}

codeptr code_E (codeptr cp, any param)
{
 accu = accu * base + 14;
 return cp;
}

codeptr code_F (codeptr cp, any param)
{
 accu = accu * base + 15;
 return cp;
}


instr *code_get (instr *ptrcode, any p)
{
	*ptrcode++ = 0xE8BB0001; /* POP RB 1 */
	*ptrcode++ = 0xE5900000; /* R0 = [R0] */
	*ptrcode++ = 0xE92B0001; /* PUSH RB 1 */
	return ptrcode;
}

instr *code_put (instr *ptrcode, any p)
{
	*ptrcode++ = 0xE8BB0003; /* POP RB 3 */
	*ptrcode++ = 0xE5801000; /* [R0] = R1 */
	return ptrcode;
}

instr *code_cget (instr *ptrcode, any p)
{
	*ptrcode++ = 0xE8BB0001; /* POP RB 1 */
	*ptrcode++ = 0xE5500000; /* R0 = $[R0] */
	*ptrcode++ = 0xE92B0001; /* PUSH RB 1 */
	return ptrcode;
}

instr *code_cput (instr *ptrcode, any p)
{
	*ptrcode++ = 0xE8BB0003; /* POP RB 3 */
	*ptrcode++ = 0xE5401000; /* $[R0] = R1 */
	return ptrcode;
}

instr *code_hget (instr *ptrcode, any p)
{
	*ptrcode++ = 0xE8BB0001; /* POP RB 1 */
	*ptrcode++ = 0xE15000B0; /* R0 = $$[R0] */
	*ptrcode++ = 0xE92B0001; /* PUSH RB 1 */
	return ptrcode;
}

instr *code_hput (instr *ptrcode, any p)
{
	*ptrcode++ = 0xE8BB0003; /* POP RB 3 */
	*ptrcode++ = 0xE14010B0; /* $$[R0] = R1 */
	return ptrcode;
}

instr * code_add (instr *ptrcode, any p)
{
	*ptrcode++ = 0xE8BB0003; /* POP RB 3 */
	*ptrcode++ = 0xE0800001; /* R0 = R0 + R1 */
	*ptrcode++ = 0xE92B0001; /* PUSH RB 1 */
	return ptrcode;
}

instr *compile_code_add (instr *cp, any p)
{
 // *cp++ = 0xE1A0F00E;
 cp = compile_begindef (cp);
 // cp = compile_enddef (cp);
 *cp++ = 0xE59F2000; /* R2 = E8BB0003 */
 *cp++ = 0xEA000000;
 *cp++ = 0xE8BB0003;
 // *cp++ = 0xE5802004; /* [R0++] = R2 */
 // *cp++ = 0xE9A02004;
 *cp++ = 0xE5802000;
 *cp++ = 0xE2800004;
 *cp++ = 0xE59F2000; /* R2 = E0800001 */
 *cp++ = 0xEA000000;
 *cp++ = 0xE0800001;
 // *cp++ = 0xE5802004; /* [R0++] = R2 */
 *cp++ = 0xE5802000;
 *cp++ = 0xE2800004;
 *cp++ = 0xE59F2000; /* R2 = E92B0001 */
 *cp++ = 0xEA000000;
 *cp++ = 0xE92B0001;
 // *cp++ = 0xE5802004; /* [R0++] = R2 */
 *cp++ = 0xE5802000;
 *cp++ = 0xE2800004;
 cp = compile_enddef (cp);
 return cp;
}

instr *code_sub (instr *ptrcode, any p)
{
	*ptrcode++ = 0xE8BB0003;
	*ptrcode++ = 0xE0410000;
	*ptrcode++ = 0xE92B0001;
	return ptrcode;
}

instr *code_bitand (instr *ptrcode, any p)
{
	*ptrcode++ = 0xE8BB0003;
	*ptrcode++ = 0xE0010000;
	*ptrcode++ = 0xE92B0001;
	return ptrcode;
}

instr *code_biteor (instr *ptrcode, any p)
{
	*ptrcode++ = 0xE8BB0003;
	*ptrcode++ = 0xE0110000;
	*ptrcode++ = 0xE92B0001;
	return ptrcode;
}

instr *code_mul (instr *ptrcode, any p)
{
	*ptrcode++ = 0xE8BB0003;
	*ptrcode++ = 0xE0000091;
	*ptrcode++ = 0xE92B0001;
	return ptrcode;
}

int divi (int b, int a)
{
	return a/b;
}

instr *code_divi (instr *ptrcode, any p)
{
	ptrcode = compile_call (ptrcode, (int(*)())divi, 3, 1);
	return ptrcode;
}

int modu (int b, int a)
{
	return a%b;
}

instr *code_modu (instr *ptrcode, any p)
{
	ptrcode = compile_call (ptrcode, (int(*)())modu, 3, 1);
	return ptrcode;
}

int logand (int a, int b)
{
	return a && b;
}

instr *code_logand (instr *ptrcode, any p)
{
	ptrcode = compile_call (ptrcode, (int(*)())logand, 3, 1);
	return ptrcode;
}

int logor (int a, int b)
{
	return a || b;
}

instr *code_logor (instr *ptrcode, any p)
{
	ptrcode = compile_call (ptrcode, (int(*)())logor, 3, 1);
	return ptrcode;
}

/*
int bitor (int a, int b)
{
	return a | b;
}

instr *code_bitor(instr *ptrcode, any p)
{
	ptrcode = compile_call(ptrcode, (int(*)())bitor, 3, 1);
	return ptrcode;
}
*/

int lognot (int a)
{
	return !a;
}

instr *code_lognot (instr *ptrcode, any p)
{
	ptrcode = compile_call (ptrcode, (int(*)())lognot, 1, 1);
	return ptrcode;
}

instr *code_printdec (instr *ptrcode, any p)
{
	ptrcode = compile_call (ptrcode, (int(*)())printdec, 1, 0);
	return ptrcode;
}

instr *code_printhex (instr *ptrcode, any p)
{
	ptrcode = compile_call (ptrcode, (int(*)())printhex, 1, 0);
	return ptrcode;
}

instr *code_run1 (instr *ptrcode, any p)
{
 any prog;
 *ptrcode++ =  0xE8BD8000;
 /* dumpcode (ptrcode); */
 prog.cp = codezone;
 /* (*(prog.f))((codeptr)0,(any)0); */
 (*(prog.vf))();
 return ptrcode;
}

instr *code_run (instr *ptrcode, any param)
{
 /* chr c; */
 pword pw;
 /* dumpcode(ptrcode); */
 /*
 c=getchr();
 (*(instbl[c].param.vf))();
 */
 pw = getword();
 (*(pw->param.vf))();
 return ptrcode;
}
 
instr *compile_init (instr *ptrcode)
{
	*ptrcode++ = 0xE92D4000; // PUSH RD 4000
	
	*ptrcode++ = 0xE59F0000; // R0 = ptrstack
	*ptrcode++ = 0xEA000000;
	*ptrcode++ = (instr)&ptrstack;
	*ptrcode++ = 0xE590B000; // RB = [R0]

	/*
	*ptrcode++ = 0xE59F0000; // R0 = ptrpstack
	*ptrcode++ = 0xEA000000;
	*ptrcode++ = (instr)&ptrpstack;
	*ptrcode++ = 0xE590A000; // RA = [R0]
	*/

	*ptrcode++ = 0xE8BD8000; // POP RD 8000
	return ptrcode;
}

instr *code_init (instr *ptrcode, any p)
{
	*ptrcode++ = 0xE59F0000; // R0 = ptrstack
	*ptrcode++ = 0xEA000000;
	*ptrcode++ = (instr)&ptrstack;
	*ptrcode++ = 0xE590B000; // RB = [R0]
	return ptrcode;

}

int myexit (int i)
{
	exit(i);
}

instr *code_exit (instr *ptrcode, any p)
{
	ptrcode = compile_call (ptrcode, (int(*)())myexit, 1, 0);
	return ptrcode;
}

instr *calldef (instr *ptrcode, any param)
{
	ptrcode = compile_call (ptrcode, (int(*)()) param.f, 0, 0);
	return ptrcode;
}

instr * code_def (instr *ptrcode, any param) 
{
 /*
 chr c;
 c = getchr();
 instbl[c].code.f = calldef;
 instbl[c].param.cp = ptrcode;
 */
 pword pw;
 /* printf("def\n"); */
 pw = getword();
 /* printf(" 1 "); */
 pw->code.f = calldef;
 /* printf(" 2 "); */
 pw->param.cp = ptrcode;
 /* printf(" 3 "); */
 /* freespace.pcode = compile_(freespace.pcode); */
 ptrcode = compile_begindef (ptrcode);
 /* printf(" 4 "); */
 return ptrcode;
}

instr * code_defmacro (instr *ptrcode, any p)
{
 /*
 chr c;
 c = getchr();
 instbl[c].code.cp = ptrcode;
 instbl[c].param.i = 0;
 */
 pword pw;
 pw = getword();
 pw->code.cp = ptrcode;
 pw->param.i = 0;
 ptrcode = compile_begindef (ptrcode);	
 return ptrcode;
}

instr * code_enddef (instr *ptrcode, any param)
{
	ptrcode = compile_enddef(ptrcode);
	return ptrcode;
}

any adrim;

instr *code_beginim (instr *ptrcode, any p)
{
 adrim.cp=ptrcode;
 ptrcode=compile_begindef(ptrcode);
 return ptrcode;
}

instr *code_endim (instr *ptrcode, any p)
{
  ptrcode=compile_enddef(ptrcode);
  dumpcode(ptrcode);
  (*(adrim.vf))();
  /* printf("\nendim: returned\n"); */
  return adrim.cp;
}

instr * getparam (instr *ptrcode, any param)
{
	/* TRACE "getparam ptrcode=%X param=%X\n", ptrcode, param ENDTRACE */
	/* PUSH RB param */
	*ptrcode++ = 0xE59F0000; 
	*ptrcode++ = 0xEA000000;
	*ptrcode++ = (instr)param.i;
	*ptrcode++ = 0xE92B0001;
	return ptrcode;
}

instr *code_icreate (instr *ptrcode, any p)
{
	/*
	pheader ph;
	readword();
	ph = getword(wr);
	ph->value.f = getparam;
	TRACE "code_icreate: freespace=%X ptrcode=%X\n", freespace, ptrcode ENDTRACE
	ph->param = freespace;
	*/
 /*
 chr c;
 c = getchr();
 instbl[c].code.f = getparam;
 instbl[c].param.cp = ptrcode;
 */
 pword pw;
 pw = getword();
 pw->code.f = getparam;
 pw->param.cp = ptrcode;
 return ptrcode;
}

instr * code_create (instr *ptrcode, any p)
{
	ptrcode = compile_call (ptrcode, (int(*)())code_icreate, 0, 0);
	return ptrcode;
}

instr *code_allotw (instr *ptrcode, any p)
{
 return ptrcode + accu; 
}

instr *code_allotwlit (instr *ptrcode, any p)
{
 pword pw;
 int i;
 pw = getword();
 sscanf ((char *)(pw->pname), "%d", &i);
 return ptrcode + i;
}

instr * code_does (instr *ptrcode, any p)
{
	any u;
	/* R1 = &lastcp */
	*ptrcode++ = 0xE59F1000;
	*ptrcode++ = 0xEA000000;
	*ptrcode++ = (instr)&lastcp;

	/* R2 = ptrcode + 4 */
	*ptrcode++ = 0xE59F2000;
	*ptrcode++ = 0xEA000000;
	*ptrcode = (instr)(ptrcode+4);
	ptrcode++;

	/* R1 = [R1] */
	*ptrcode++ = 0xE5911000;

	/* [R1] = R2 */
	*ptrcode++ = 0xE5812000;

	ptrcode = compile_enddef(ptrcode);
	ptrcode = compile_begindef(ptrcode);

	/*
	// *ptrcode++ = 0xE92B0001; // PUSH RB 1
	// a verifier : compile R0=ptrcode ?
	*/
	ptrcode = compile_call (ptrcode, (int(*)())getparam,0,0);

	u.cp = ptrcode+5;
	ptrcode = compile_movr (ptrcode, 1, u.i);
	ptrcode = compile_call (ptrcode,(int(*)())calldef,0,0);

	ptrcode = compile_enddef(ptrcode);
	ptrcode = compile_begindef(ptrcode);
}

instr *code_tick (instr *ptrcode, any p)
{
        /*
        chr c;
        c = getchr();
        */
        pword pw;
        pw = getword();
        *ptrcode++ = 0xE59F0000; /* R0=[RF] */
        *ptrcode++ = 0xEA000000; /* GOTO RF */
        /* *ptrcode++ = (instr)(instbl+c); */
        *ptrcode++ = (instr)pw;
        *ptrcode++ = 0xE92B0001; /* PUSH RB 1 */
        return ptrcode;
}

/*
instr *code_compile (instr *ptrcode, any param)
{
	pheader ph;
	readword();
	ph = getword (wr);
	ptrcode = compile_movr (ptrcode, 3, &freespace); // ou gptrcode ?
	*ptrcode++ = 0xE5930000; // R0 = [R3]
	ptrcode = compile_movr (ptrcode, 1, ph->param.i);
	ptrcode = compile_movr (ptrcode, 2, (int)ph);
	ptrcode = compile_call (ptrcode, ph->value.f, 0, 0);
	ptrcode = compile_movr (ptrcode, 3, (int)&freespace);
	*ptrcode++ = 0xE5830000; // [R3] = R0;
	return ptrcode;
}
*/

instr *code_compile (instr *ptrcode, any param)
{
 /*
 chr c;
 c = getchr();
 ptrcode = compile_movr (ptrcode, 1, instbl[c].param.i);
 ptrcode = compile_call (ptrcode, (int(*)())instbl[c].code.f, 0, 0);
 */
 pword pw;
 pw = getword();
 ptrcode = compile_movr (ptrcode, 1, pw->param.i);
 ptrcode = compile_call (ptrcode, (int(*)())(pw->code.f), 0, 0);
 return ptrcode;
}

instr *code_compileim (instr *ptrcode, any param)
{
	/*
	chr c;
        c = getchr();
	ptrcode = (*(instbl[c].code.f)) (ptrcode, instbl[c].param);
	*/
	pword pw;
	pw = getword();
	ptrcode = (*(pw->code.f)) (ptrcode, pw->param);
	return ptrcode;
}

/*
instr *code_push (instr *ptrcode)
{
	int regs;
	readword();
	sscf(wr,L"%X",&regs);
	*ptrcode++ = 0xE92B0000 | regs;
	return ptrcode;
}

instr *code_pop (instr *ptrcode)
{
	int regs;
	readword();
	sscf(wr,L"%X",&regs);
	*ptrcode++ = 0xE8BB0000 | regs;
	return ptrcode;
}
*/

instr *code_pushr0r1 (instr *ptrcode, any p)
{
 *ptrcode++ = 0xE92B0003;
 return ptrcode;
}

instr *code_popr0 (instr *ptrcode, any p)
{
 *ptrcode++ = 0xE8BB0001;
 return ptrcode;
}

instr *code_calladr (instr *ptrcode, any p)
{
	/* POP RB 80 */	*ptrcode++ = 0xE8BB0080;
	/* RE = RF   */	*ptrcode++ = 0xE1A0E00F;
	/* RF = R7   */ *ptrcode++ = 0xE1A0F007; 	
	return ptrcode;
}


int levellambda = -1;
instr *adrblambda[0x100];

instr *code_lambda (instr *ptrcode, any p)
{
	levellambda++;
	*ptrcode++ = 0xE28F0004; /* R0 = RF + 4 */
	*ptrcode++ = 0xE92B0001; /* PUSH RB 1 */
	adrblambda[levellambda] = ptrcode;
	*ptrcode++ = 0xEA000000;
	ptrcode = compile_begindef (ptrcode);
	return ptrcode;
}

instr * code_endlambda (instr *ptrcode, any p)
{
	int ofs;
	ptrcode = compile_enddef (ptrcode);
	ofs = ptrcode - (adrblambda[levellambda]+2);
	*(adrblambda[levellambda]) |= ofs;
	levellambda--;
	return ptrcode;
}

int readchar (void)
{
 return getchar();
}

int printchar (int x)
{
 printf("%c",x);
}

instr *code_printchar (instr *ptrcode, any p)
{
	ptrcode = compile_call (ptrcode, (int(*)())printchar, 1, 0);
	return ptrcode;
}

instr *code_readchar (instr *ptrcode, any p)
{
	ptrcode = compile_call (ptrcode, (int(*)())readchar, 0, 1);
	return ptrcode;
}

/*
instr *code_ifge0 (instr *ptrcode)
{
	iflevel++;
	*ptrcode++ = 0xE8BB0001; // POP RB 1	
	*ptrcode++ = 0xE1B00000; // R0 ?= R0
	adrif[iflevel] = ptrcode;
	adrelse[iflevel] = 0;
	*ptrcode++ = 0x4A000000; // >=0 GOTO
	return ptrcode;
}
*/

struct fromto
{
 instr *from;
 instr *to;
} labels[256];

void setbradr (instr *from, instr *to)
{
 int ofs;
 ofs = to - (from + 2);
 *from |= ofs;
}

instr *code_brifge0 (instr *ptrcode, any p)
{
 chr c;
 c = getchr();
 *ptrcode++ = 0xE8BB0001;
 *ptrcode++ = 0xE1B00000;
 labels[c].from = ptrcode;
 *ptrcode++ = 0x5A000000;
 if (labels[c].to != NULL)
  setbradr (labels[c].from, labels[c].to);
 return ptrcode;
}

instr *code_label (instr *ptrcode, any p)
{
 chr c;
 c = getchr();
 labels[c].to = ptrcode;
 if (labels[c].from != NULL)
  setbradr (labels[c].from, labels[c].to);
 return ptrcode;
}

/*
instr *code_else (instr *ptrcode)
{
	adrelse[iflevel] = ptrcode;
	*ptrcode++ = 0xEA000000;	// GOTO ...
	ofs = ptrcode - (adrif[iflevel] + 2);
	*(adrif[iflevel]) |= ofs;
	return ptrcode;
}
*/

void dump_labels ()
{
 int c;
 printf ("\n");
 for (c=0; c<256; c++)
  if (labels[c].from != NULL || labels[c].to != NULL)
   printf ("%c : %X -> %X\n", c, labels[c].from, labels[c].to);
}

instr *code_dump_labels (instr *ptrcode, any p)
{
 dump_labels();
 return ptrcode;
}

instr *code_wr (instr *ptrcode, any p)
{
 ptrcode = compile_pushint (ptrcode, (int)wr);
 return ptrcode;
}

instr *code_readword (instr *ptrcode, any p)
{
 ptrcode = compile_call (ptrcode, (int(*)())readword, 0, 0);
 return ptrcode;
}

instr *code_getwordstr (instr *ptrcode, any p)
{
 ptrcode = compile_call (ptrcode, (int(*)())getwordstr, 1, 1);
 return ptrcode;
}

instr *code_getword (instr *ptrcode, any p)
{
 ptrcode = compile_call (ptrcode, (int(*)())getword, 0, 1);
 return ptrcode;
}

instr *code_bye (instr *ptrcode, any p)
{
 printf("\n");
 exit(0);
 return ptrcode;
}

/* #define defins(c,cf,p) instbl[c].code.f=cf; instbl[c].param.i=p; */

#define defword(w,cf) pw=getwordstr((chr*)w); pw->code.f=cf; pw->param.i=0;

/*
#define defword(w,cf) printf("defword <%s> cf=%X ",w,cf); pw=getwordstr((chr*)w); printf(" pw=%X ",pw); pw->code.f=cf; printf(" cf=%X ",pw->code.f); pw->param.i=0;
*/

instr *compile_mainloop (instr *cp, any p)
{
 /* cp = compile_begindef (cp); */
/*
 *cp++ = 0xE1A03000;
 *cp++ = 0xE1A04001;
 *cp++ = 0xE1A0E00F;
 *cp++ = 0xE1A0F004;
 *cp++ = 0xE1A05000;
 *cp++ = 0xE1A00003;
 *cp++ = 0xE5951004;
 *cp++ = 0xE1A0E00F;
 *cp++ = 0xE595F000;
 *cp++ = 0xE1A03000;
 *cp++ = 0xEAFFFFF6;
*/
 *cp++ = 0xE58F0024;
 *cp++ = 0xE58F1024;
 *cp++ = 0xE1A0E00F;
 *cp++ = 0xE59FF01C;
 *cp++ = 0xE1A05000;
 *cp++ = 0xE59F0010;
 *cp++ = 0xE5951004;
 *cp++ = 0xE1A0E00F;
 *cp++ = 0xE595F000;
 *cp++ = 0xE58F0000;
 *cp++ = 0xEAFFFFF6;
 *cp++ = 0;
 *cp++ = 0;
 /* cp = compile_enddef (cp); */
 return cp;
}

void init (void)
{
 int c;
 pword pw;
 any a;
 char bufasm[300];
 int i;

 printf("init\n");

 ptrstack = stack+STACKSIZE;

 a.i=0;

 for(c=0;c<256;c++)
 {
  labels[c].from = NULL;
  labels[c].to = NULL;
  /* defins((char)c,code_nop,0);*/
 }
 
 printf("init 2\n");

 syscode.cp=syscodebuf;
 printf("init 21\n");
 scp=syscode.cp;
 printf("init 22\n");
 cp_mainloop.cp=scp;
 printf("init 23 scp=%X\n",scp);
 scp=compile_mainloop(scp,a);
 printf("init 24 scp=%X\n",scp);
 cp_add.cp=scp;
 printf("init 25\n");
 scp=compile_code_add (scp, a);

 dump_asm (cp_mainloop.cp, cp_add.cp);

 printf("add: %X\n", cp_add.i);
/*
 a.cp = syscode.cp;
 for (i=0; i<20; i++)
 {
  // printf("%X\n",a.cp[i]); 
  printasm (bufasm, (unsigned int)(a.cp[i]), (unsigned int)(a.cp+i));
  printf ("%X: %08X %s\n", a.cp+i, a.cp[i], bufasm);
 }
*/

 printf("init 3\n");

/*
 defins('_',code_nop,0);
 defins(':',code_def,0);
 defins(';',code_enddef,0);
 defins('M',code_defmacro,0);
 defins('$',code_icreate,0);
 defins('Z',code_create,0);
 defins('W',code_allotw,0);
 defins('>',code_does,0);
 defins('#',code_accu0,0);
 defins('S',code_pushaccu,0);
 defins(',',code_putcode,0);
 defins('H',code_here,0);
 defins('?',code_lastcp,0);
 defins('^',code_spget,0);
 defins('V',code_spset,0);
 defins('\'',code_tick,0);
 defins('\\',code_compile,0);
 defins('I',code_compileim,0);
 defins('[',code_pushr0r1,0);
 defins(']',code_popr0,0);
 defins('%',code_calladr,0);
 defins('{',code_lambda,0);
 defins('}',code_endlambda,0);
 defins('L',code_label,0);
 defins('J',code_brifge0,0);
 defins('Y',code_dump_labels,0);
 defins('+',code_add,0);
 defins('-',code_sub,0);
 defins('N',code_bitand,0);
 defins('O',code_biteor,0);
 defins('@',code_get,0);
 defins('!',code_put,0);
 defins('.',code_printdec,0);
 defins('X',code_printhex,0);
 defins('P',code_printchar,0);
 defins('G',code_readchar,0);
 defins('&',code_run,0);
 defins('T',code_dumpdic,0);
 defins('K',code_dumpcode,0);
 defins('R',code_base,0);
 defins('0',code_0,0);
 defins('1',code_1,0);
 defins('2',code_2,0);
 defins('3',code_3,0);
 defins('4',code_4,0);
 defins('5',code_5,0);
 defins('6',code_6,0);
 defins('7',code_7,0);
 defins('8',code_8,0);
 defins('9',code_9,0);
 defins('A',code_A,0);
 defins('B',code_B,0);
 defins('C',code_C,0);
 defins('D',code_D,0);
 defins('E',code_E,0);
 defins('F',code_F,0);
 defins('Q',code_bye,0);
*/

 defword("_",code_nop);
 defword("INIT",code_init);
 defword("EXIT",code_exit);
 defword(":",code_def);
 defword(";",code_enddef);
 defword("[",code_beginim);
 defword("]",code_endim);
 defword("MACRO:",code_defmacro);
 defword("[CREATE]",code_icreate);
 defword("CREATE",code_create);
 defword("ALLOTWLIT",code_allotwlit);
 defword("DOES>",code_does);
 defword("NBR",code_accu0);
 defword(">STACK",code_pushaccu);
 defword("PUTCODE",code_putcode);
 defword("HERE",code_here);
 defword("LASTCP",code_lastcp);
 defword("SP@",code_spget);
 defword("SP!",code_spset);
 defword("'",code_tick);
 defword("COMPILE",code_compile);
 defword("[COMPILE]",code_compileim);
 defword("[[",code_pushr0r1);
 defword("]]",code_popr0);
 defword("CALLADR",code_calladr);
 defword("LAMBDA",code_lambda);
 defword("ENDLAMBDA",code_endlambda);
 defword("LABEL",code_label);
 defword(">=0BRANCH",code_brifge0);
 defword("DUMPLABELS",code_dump_labels);
 defword("+",code_add);
 defword("+T",cp_add.f);
 defword("-",code_sub);
 defword("/",code_divi);
 defword("AND",code_bitand);
 defword("XOR",code_biteor);
 defword("@",code_get);
 defword("!",code_put);
 defword(".",code_printdec)
 defword("PRINTHEX",code_printhex);
 defword("PRINTCHAR",code_printchar);
 defword("READCHAR",code_readchar);
 defword("RUN",code_run);
 defword("DUMPDIC",code_dumpdic);
 defword("DUMPCODE",code_dumpcode);
 defword("HEXNBR",code_hexnbr);
 defword("BASE",code_base);
/*
 defword("0",code_0);
 defword("1",code_1);
 defword("2",code_2);
 defword("3",code_3);
 defword("4",code_4);
 defword("5",code_5);
 defword("6",code_6);
 defword("7",code_7);
 defword("8",code_8);
 defword("9",code_9);
 defword("A",code_A);
 defword("B",code_B);
 defword("C",code_C);
 defword("D",code_D);
 defword("E",code_E);
 defword("F",code_F);
*/
 defword("WR",code_wr);
 defword("READWORD",code_readword);
 defword("GETWORDSTR",code_getwordstr);
 defword("GETWORD",code_getword);
 defword("BYE",code_bye);

}

void mainloop (codeptr cp, pword (*gw)())
{
 /* chr c; */
 pword pw;
 /* codeptr cp; */
 codefn f; 
 FILE *fi;
 int i;

 /* cp = codezone; */
 /* *cp++ = 0xE92D4000; */
 for (;;)
 {
  /*
  c = getchr();
  printf("%c",c);
  cp = (*(instbl[c].code.f)) (cp, instbl[c].param);
  */
  pw = (*gw)();
  /* printf("<%s> pw=%X code=%X p=%X",awr,pw,pw->code.i,pw->param.i); */
  // dumpcode(cp); 
  /*
  fi=fopen("f.txt","w");
  fclose(fi);
  */

/*
  for(i=0;i<4;i++) 
   // printf("[%X: %08X]\n", pw->code.i+i,  pw->code.cp[i]  );
   dump_asm (pw->code.cp, pw->code.cp+20);
  printf("\n");
*/

  cp = (*(pw->code.f)) (cp, pw->param);
  // printf("\nmainloop: returned\n");
  // dumpcode(cp);
  /* printf(" cp=%X ",cp); */
  /* cp=code_lpar(cp, instbl[c].param); */
 }
}

void hello(void)
{
 printf("test\n");
}

instr *code_test (instr *ptrcode, any p)
{
 printf("test\n");
 return ptrcode;
}

void test(void)
{
 pword pw;
 codeptr cp;
 char w[10];
 strcpy(w,"TEST");
 /* defword("TEST",code_test); */
 printf("defword <%s> cf=%X \n",w,code_test); 
 pw=getwordstr((chr*)w); 
 printf(" pw=%X ",pw); 
 pw->code.f=code_test; 
 printf(" cf=%X \n",pw->code.f); 
 pw->param.i=0;

 pw=getwordstr((chr*)"TEST");
  printf("<%s> <%s> pw=%X code=%X p=%X\n",awr,pw->pname,pw,pw->code.i,pw->param.i);
  cp = (*(pw->code.f)) (cp, pw->param);
}

main ()
{
 codeptr cp;
 any a;
 int i;
 char bufasm[300];

/*
 syscode.cp=syscodebuf;
 compile_code_add (syscodebuf, a);
*/

/*
 dump_asm ((instr*)mainloop, (instr*)hello);
*/

// dump_asm (cp_mainloop.cp, cp_add.cp);

/*
 a.cp = syscode.cp;
 for (i=0; i<20; i++)
 {
  // printf("%X\n",a.cp[i]); 
  printasm (bufasm, (unsigned int)(a.cp[i]), (unsigned int)(a.cp+i));
  printf ("%X: %08X %s\n", a.cp+i, a.cp[i], bufasm);
 }

 a.f=code_endim;
 for(i=0;i<20;i++)
 {
  // printf("%X\n",a.cp[i]); 
  printasm (bufasm, (unsigned int)(a.cp[i]), (unsigned int)(a.cp+i));
  printf ("%X: %08X %s\n", a.cp+i, a.cp[i], bufasm);
 }
*/

 a.i=0;
/*
 void(*pf)(void);
 pf=hello;
 (*pf)();
 
 instbl[9].code.vf=hello;
 (*(instbl[9].code.vf))();

 instbl[9].code.f=code_lpar;
 instbl[9].param.i=0;
 (*(instbl[9].code.f))(cp,instbl[9].param);
*/

  /* testreadword(); */
/*
 test();
 return 0;
*/
  init();
/*
  dumpdic();
  dumpdic();
*/
   // mainloop (codezone, getword);
  (*cp_mainloop.mainloopf) (codezone, getword);
}

