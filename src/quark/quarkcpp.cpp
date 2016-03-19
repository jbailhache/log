
#include <stdlib.h>
#include <stdio.h>

typedef unsigned char chr;
typedef char byte;
typedef int instr;
typedef instr *codeptr;
typedef codeptr (*codefn) (codeptr, union any);

typedef union any
{
 int i;
 codeptr cp;
 codefn f;
 void (*vf) (void);
} any;
 
any a;

struct codeparam
{
 any code;
 any param;
};

struct codeparam instbl[256];

instr codezone[30000];
 
struct codeparam *lastcp;

chr getchr ()
{
 chr c;
 c = getchar();
 lastcp = instbl+c;
 return c;
}

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

codeptr code_nop (codeptr cp, any param);

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

void dumpcode (codeptr cp)
{
 codeptr p;
 printf("\n");
 for (p=codezone; p<=cp; p++)
  printf ("%X: %08X\n", p, *p);
}

codeptr code_dumpcode (codeptr cp, any p)
{
 dumpcode(cp);
 return cp;
}

codeptr compile_pushint (codeptr cp, int x)
{
	// TRACE2 "compile_pushint %X\n", x ENDTRACE2
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
 // printf(" lpar ");
 accu = 0;
 return cp;
}

codeptr code_pushaccu (codeptr cp, any param)
{
 cp = compile_pushint (cp, accu);
 return cp;
}

codeptr code_putcode (codeptr cp, any param)
{
 *cp++ = accu;
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
 // printf(" trois ");
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
	*ptrcode++ = 0xE8BB0001; // POP RB 1
	*ptrcode++ = 0xE5900000; // R0 = [R0]
	*ptrcode++ = 0xE92B0001; // PUSH RB 1
	return ptrcode;
}

instr *code_put (instr *ptrcode, any p)
{
	*ptrcode++ = 0xE8BB0003; // POP RB 3
	*ptrcode++ = 0xE5801000; // [R0] = R1
	return ptrcode;
}

instr *code_cget (instr *ptrcode, any p)
{
	*ptrcode++ = 0xE8BB0001; // POP RB 1
	*ptrcode++ = 0xE5500000; // R0 = $[R0]
	*ptrcode++ = 0xE92B0001; // PUSH RB 1
	return ptrcode;
}

instr *code_cput (instr *ptrcode, any p)
{
	*ptrcode++ = 0xE8BB0003; // POP RB 3
	*ptrcode++ = 0xE5401000; // $[R0] = R1
	return ptrcode;
}

instr *code_hget (instr *ptrcode, any p)
{
	*ptrcode++ = 0xE8BB0001; // POP RB 1
	*ptrcode++ = 0xE15000B0; // R0 = $$[R0]
	*ptrcode++ = 0xE92B0001; // PUSH RB 1
	return ptrcode;
}

instr *code_hput (instr *ptrcode, any p)
{
	*ptrcode++ = 0xE8BB0003; // POP RB 3
	*ptrcode++ = 0xE14010B0; // $$[R0] = R1
	return ptrcode;
}

instr * code_add (instr *ptrcode, any p)
{
	*ptrcode++ = 0xE8BB0003; // POP RB 3
	*ptrcode++ = 0xE0800001; // R0 = R0 + R1
	*ptrcode++ = 0xE92B0001; // PUSH RB 1
	return ptrcode;
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
 // dumpcode (ptrcode);
 prog.cp = codezone;
 // (*(prog.f))((codeptr)0,(any)0);
 (*(prog.vf))();
 return ptrcode;
}

instr *code_run (instr *ptrcode, any param)
{
 chr c;
 // dumpcode(ptrcode);
 c=getchr();
 (*(instbl[c].param.vf))();
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

instr *calldef (instr *ptrcode, any param)
{
	ptrcode = compile_call (ptrcode, (int(*)()) param.f, 0, 0);
	return ptrcode;
}

instr * code_def (instr *ptrcode, any param) 
{
 chr c;
 c = getchr();
 instbl[c].code.f = calldef;
 instbl[c].param.cp = ptrcode;
 // freespace.pcode = compile_begindef(freespace.pcode);
 ptrcode = compile_begindef (ptrcode);
 return ptrcode;
}

instr * code_defmacro (instr *ptrcode, any p)
{
 chr c;
 c = getchr();
 instbl[c].code.cp = ptrcode;
 instbl[c].param.i = 0;
 ptrcode = compile_begindef (ptrcode);	
 return ptrcode;
}

instr * code_enddef (instr *ptrcode, any param)
{
	ptrcode = compile_enddef(ptrcode);
	return ptrcode;
}

instr * getparam (instr *ptrcode, any param)
{
	// TRACE "getparam ptrcode=%X param=%X\n", ptrcode, param ENDTRACE
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
 chr c;
 c = getchr();
 instbl[c].code.f = getparam;
 instbl[c].param.cp = ptrcode;
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

instr * code_does (instr *ptrcode, any p)
{
	any u;
	// R1 = &lastcp
	*ptrcode++ = 0xE59F1000;
	*ptrcode++ = 0xEA000000;
	*ptrcode++ = (instr)&lastcp;

	// R2 = ptrcode + 4 
	*ptrcode++ = 0xE59F2000;
	*ptrcode++ = 0xEA000000;
	*ptrcode = (instr)(ptrcode+4);
	ptrcode++;

	// R1 = [R1] 
	*ptrcode++ = 0xE5911000;

	// [R1] = R2 
	*ptrcode++ = 0xE5812000;

	ptrcode = compile_enddef(ptrcode);
	ptrcode = compile_begindef(ptrcode);

	// *ptrcode++ = 0xE92B0001; // PUSH RB 1
	// a verifier : compile R0=ptrcode ?
	ptrcode = compile_call (ptrcode, (int(*)())getparam,0,0);

	u.cp = ptrcode+5;
	ptrcode = compile_movr (ptrcode, 1, u.i);
	ptrcode = compile_call (ptrcode,(int(*)())calldef,0,0);

	ptrcode = compile_enddef(ptrcode);
	ptrcode = compile_begindef(ptrcode);
}

instr *code_tick (instr *ptrcode, any p)
{
        chr c;
        c = getchr();
	*ptrcode++ = 0xE59F0000; // R0=[RF]
	*ptrcode++ = 0xEA000000; // GOTO RF
	*ptrcode++ = (instr)(instbl+c);     
	*ptrcode++ = 0xE92B0001; // PUSH RB 1
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
 chr c;
 c = getchr();
 ptrcode = compile_movr (ptrcode, 1, instbl[c].param.i);
 ptrcode = compile_call (ptrcode, (int(*)())instbl[c].code.f, 0, 0);
 return ptrcode;
}

instr *code_compileim (instr *ptrcode, any param)
{
	chr c;
        c = getchr();
	ptrcode = (*(instbl[c].code.f)) (ptrcode, instbl[c].param);
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
	*ptrcode++ = 0xE28F0004; // R0 = RF + 4
	*ptrcode++ = 0xE92B0001; // PUSH RB 1
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

instr *code_bye (instr *ptrcode, any p)
{
 printf("\n");
 exit(0);
 return ptrcode;
}

#define defins(c,cf,p) instbl[c].code.f=cf; instbl[c].param.i=p;

void init (void)
{
 int c;
 a.i=0;

 for(c=0;c<256;c++)
 {
  labels[c].from = NULL;
  labels[c].to = NULL;
  defins((char)c,code_nop,0);
 }
 
 defins('_',code_nop,0);
 defins(':',code_def,0);
 defins(';',code_enddef,0);
 defins('M',code_defmacro,0);
 defins('$',code_icreate,0);
 defins('W',code_allotw,0);
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
}

void mainloop (void)
{
 chr c;
 codeptr cp;
 codefn f;
 cp = codezone;
 // *cp++ = 0xE92D4000;
 for (;;)
 {
  c = getchr();
  printf("%c",c);
  cp = (*(instbl[c].code.f)) (cp, instbl[c].param);
  // cp=code_lpar(cp, instbl[c].param);
 }
}

void hello(void)
{
 printf("test\n");
}

main ()
{
 codeptr cp;
 any a;
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
  init();
  mainloop();
}

