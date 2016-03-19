
// #include <stdio.h>
#include <stdlib.h>

typedef long instr;

typedef instr *codeptr;

#ifdef CPLUSPLUS
typedef codeptr (*codefn) (codeptr, union any);
#else
typedef codeptr (*codefn)();
#endif

typedef union any
{
 long i;
 instr ins;
 long *p;
 codeptr cp;
 codefn f;
 void (*vf) (void);
 int (*intf)();
 struct word *pw;
 char *pchar;
} any;

extern instr cod[6000];

extern char awr[32];

typedef char chr;

struct word
{
 struct word *link;
 any code;
 any param;
 chr *pname;
 chr name[4];
};

typedef struct word *pword;

pword getword (void);

int f_scanhex (void);

#define code_mov_r0_im 0x1000F
#define code_q_mov_r0_imr0 0x101000F
#define code_mov_r1_im 0x1010F
#define code_q_mov_r1_imr0 0x101010F
#define code_mov_ra_im 0x10A0F
#define code_q_mov_ra_imr0 0x1010A0F
#define code_mov_r1_r0 0x10100
#define code_q_mov_r1_r0 0x1010100
#define code_mov_r2_r0 0x10200
#define code_mov_r0_r2 0x10002
#define code_mov_r1_r2 0x10102
#define code_mov_ra_r0 0x10A00
#define code_mov_rb_r0 0x10B00
#define code_mov_rc_r0 0x10C00
#define code_mov_r0_ra 0x1000A
#define code_mov_r0_rb 0x1000B
#define code_mov_r0_rc 0x1000C
#define code_mov_ra_r1 0x10A01
#define code_mov_rc_r1 0x10C01
#define code_mov_rc_ra 0x10C0A
#define code_mov_r1_rc 0x1010C
#define code_mov_r0_cont_ra 0x1001A
#define code_mov_ra_cont_ra 0x10A1A
#define code_mov_cont_ra_r0 0x11A00
#define code_mov_r0_cont_ra_plus_im 0x1009A
#define code_mov_cont_ra_plus_im_r0 0x19A00
#define code_mov_r0_cont_rb_inc 0x1005B
#define code_mov_cont_dec_rb_r0 0x13B00
#define code_mov_r1_cont_rb_inc 0x1015B
#define code_mov_ra_cont_rb_inc 0x10A5B
#define code_mov_cont_dec_rb_r1 0x13B01
#define code_mov_cont_rc_inc_r0 0x15C00
#define code_add_r0_r1 0x20001
#define code_sub_r0_r1 0x30001
#define code_bp_depl 0xC2FFF
#define code_bz_depl 0xC3FFF
#define code_bp_ra 0xC0AFF
#define code_call_depl 0xD2FFF
#define code_call_ra 0xD0AFF
#define code_beginf 0xE0001
#define code_endf 0xE0002
#define code_ret 0xEFFFF
#define code_ror_r0 0x600FF
#define code_asr_r0 0x700FF
#define code_and_r0_r1 0x80001
#define code_xor_r0_r1 0x90001
#define code_fillmem 0xE0000
#define code_callext 0xFFF0F
#define code_getchar 0x1000FF
#define code_putchar 0x11FF00
#define code_scanhex 0x1200FF
#define code_getword 0x1300FF
#define code_setbradr 0x140000
#define code_exit 0xFFFFFF

#define INSNAM(i,n) if (x==i) return n; else

const char *instr_name (instr x)
{
 INSNAM(code_mov_r0_im,"r0=")
 INSNAM(code_q_mov_r0_imr0,"r0={r0}")
 INSNAM(code_mov_r1_im,"r1=")
 INSNAM(code_q_mov_r1_imr0,"r1={r0}")
 INSNAM(code_mov_ra_im,"rA=")
 INSNAM(code_q_mov_ra_imr0,"rA={r0}")
 INSNAM(code_mov_r1_r0,"r1=r0")
 INSNAM(code_q_mov_r1_r0,"code r1=r0")
 INSNAM(code_mov_r2_r0,"r2=r0")
 INSNAM(code_mov_r0_r2,"r0=r2")
 INSNAM(code_mov_r1_r2,"r1=r2")
 INSNAM(code_mov_ra_r0,"rA=r0")
 INSNAM(code_mov_rb_r0,"rB=r0")
 INSNAM(code_mov_rc_r0,"rC=r0")
 INSNAM(code_mov_r0_ra,"r0=rA")
 INSNAM(code_mov_r0_rb,"r0=rB")
 INSNAM(code_mov_r0_rc,"r0=rC")
 INSNAM(code_mov_ra_r1,"rA=r1")
 INSNAM(code_mov_rc_r1,"rC=r1")
 INSNAM(code_mov_rc_ra,"rC=rA")
 INSNAM(code_mov_r1_rc,"r1=rC")
 INSNAM(code_mov_r0_cont_ra,"r0=[rA]")
 INSNAM(code_mov_ra_cont_ra,"rA=[rA]")
 INSNAM(code_mov_cont_ra_r0,"[rA]=r0")
 INSNAM(code_mov_r0_cont_ra_plus_im,"r0=[rA+]")
 INSNAM(code_mov_cont_ra_plus_im_r0,"[rA+]=r0")
 INSNAM(code_mov_r0_cont_rb_inc,"r0=[rB++]")
 INSNAM(code_mov_cont_dec_rb_r0,"[--rB]=r0")
 INSNAM(code_mov_r1_cont_rb_inc,"r1=[rB++]")
 INSNAM(code_mov_ra_cont_rb_inc,"rA=[rB++]")
 INSNAM(code_mov_cont_dec_rb_r1,"[--rB]=r1")
 INSNAM(code_mov_cont_rc_inc_r0,"[rC++]=r0")
 INSNAM(code_add_r0_r1,"r0=r0+r1")
 INSNAM(code_sub_r0_r1,"r0=r0-r1")
 INSNAM(code_bp_depl,"bp")
 INSNAM(code_bz_depl,"bz")
 INSNAM(code_bp_ra,"bp rA")
 INSNAM(code_call_depl,"call")
 INSNAM(code_call_ra,"call rA")
 INSNAM(code_beginf,"beginf")
 INSNAM(code_endf,"endf")
 INSNAM(code_ret,"ret")
 INSNAM(code_ror_r0,"ror r0")
 INSNAM(code_asr_r0,"asr r0")
 INSNAM(code_and_r0_r1,"r0=r0&r1")
 INSNAM(code_xor_r0_r1,"r0=r0^r1")
 INSNAM(code_fillmem,"fillmem")
 INSNAM(code_callext,"callext")
 INSNAM(code_getchar,"getchar")
 INSNAM(code_putchar,"putchar")
 INSNAM(code_scanhex,"scanhex")
 INSNAM(code_getword,"getword")
 INSNAM(code_setbradr,"setbradr")
 INSNAM(code_exit,"exit")
 return "";
}

extern any r0, r1, r2, ra, rb, rc;
#define STACKSIZE 1000
extern codeptr stack[STACKSIZE];
extern int sp;

void debug (void)
{
codeptr cp;
instr ins;
int depl;
any a;
char c;
char buf[100];
 cp = cod;
 sp = 0;
 ra.cp = cod;
 rb.cp = cod;
 for (;;)
 {
  printf ("\n");
  printf ("r0=%08lX\n",r0.i);
  printf ("r1=%08lX\n",r1.i);
  printf ("r2=%08lX\n",r2.i);
  printf ("rA=%08lX [rA]=%08lX\n",ra.i,*(ra.p));
  printf ("rB=%08lX [rB]=%08lX\n",rb.i,*(rb.p));
  printf ("rC=%08lX\n",rc.i);
  ins = *cp++;
  printf ("%X: %08lX %s %08lX\n", cp, ins, instr_name(ins), *cp);
  gets(buf);
  if (buf[0]=='q')
   return;
  /* printf ("%08X %08X\n", ins, *cp); */
  if (ins == code_mov_r0_im)
    r0.ins = *cp++;
  else if (ins == code_mov_r1_im)
    r1.ins = *cp++;
  else if (ins == code_mov_ra_im)
    ra.ins = *cp++;
  else if (ins == code_q_mov_r0_imr0)
  {
    *(rc.p++) = code_mov_r0_im;
    *(rc.p++) = r0.ins;
  }
  else if (ins == code_q_mov_r1_imr0)
  {
    *(rc.p++) = code_mov_r1_im;
    *(rc.p++) = r0.ins;
  }
  else if (ins == code_q_mov_ra_imr0)
  {
    *(rc.p++) = code_mov_ra_im;
    *(rc.p++) = r0.ins;
  }
  else if (ins == code_mov_r1_r0)
    r1 = r0;
  else if (ins == code_mov_r2_r0)
    r2 = r0;
  else if (ins == code_mov_r0_r2)
    r0 = r2;
  else if (ins == code_mov_r1_r2)
    r1 = r2;
  else if (ins == code_q_mov_r1_r0)
    *(rc.p++) = code_mov_r1_r0;
  else if (ins == code_mov_ra_r0)
    ra = r0;
  else if (ins == code_mov_rb_r0)
    rb = r0;
  else if (ins == code_mov_rc_r0)
    rc = r0;
  else if (ins == code_mov_r0_ra)
    r0 = ra;
  else if (ins == code_mov_r0_rb)
    r0 = rb;
  else if (ins == code_mov_r0_rc)
    r0 = rc;
  else if (ins == code_mov_ra_r1)
    ra = r1;
  else if (ins == code_mov_rc_r1)
    rc = r1;
  else if (ins == code_mov_rc_ra)
    rc = ra;
  else if (ins == code_mov_r1_rc)
    r1 = rc;
  else if (ins == code_mov_r0_cont_ra)
    r0.i = *(ra.p);
  else if (ins == code_mov_ra_cont_ra)
    ra.i = *(ra.p);
  else if (ins == code_mov_cont_ra_r0)
    *(ra.p) = r0.i;
  else if (ins == code_mov_r0_cont_ra_plus_im)
    r0.i = ra.p[((int)(*cp))/4];
  else if (ins == code_mov_cont_ra_plus_im_r0)
    ra.p[((int)(*cp))/4] = r0.i;
  else if (ins == code_mov_r0_cont_rb_inc)
   r0.i = *(rb.p++);
  else if (ins == code_mov_cont_dec_rb_r0)
   *(--rb.p) = r0.i;
  else if (ins == code_mov_r1_cont_rb_inc)
   r1.i = *(rb.p++);
  else if (ins == code_mov_ra_cont_rb_inc)
   ra.i = *(rb.p++);
  else if (ins == code_mov_cont_dec_rb_r1)
   *(--rb.p) = r1.i;
  else if (ins == code_mov_cont_rc_inc_r0)
   *(rc.p++) = r0.i;
  else if (ins == code_add_r0_r1)
    r0.i = r0.i + r1.i;
  else if (ins == code_sub_r0_r1)
    r0.i = r0.i - r1.i;
  else if (ins == code_bp_depl)
  {
    depl = (int) *cp++;
    if (r0.i >= 0)
     cp += depl;
  }
  else if (ins == code_bz_depl)
  {
    depl = (int) *cp++;
    if (r0.i == 0)
     cp += depl;
  }
  else if (ins == code_bp_ra)
  {
    if (r0.i >= 0)
     cp = ra.cp;
  }
  else if (ins == code_setbradr)
  {
   /* rA = adresse de l'instruction de saut bp
      r0 = adresse du saut */
   int depl;
   depl = r0.cp - ra.cp + 1;
   ra.cp[1] = depl;
  }
  else if (ins == code_call_depl)
  {
    if (sp>=STACKSIZE)
    {
     printf("\nstack overflow\n");
     break;
    }
    depl = (int) *cp++;
    stack[sp++] = cp;
    cp += depl;
  }
  else if (ins == code_call_ra)
  {
    if (sp >= STACKSIZE)
    {
     printf ("\nstack overflow\n");
     /* exit(0); */
     return;
    }
    stack[sp++] = cp;
    cp = ra.cp;
  }
  else if (ins==code_beginf);
  else if (ins == code_ret || ins==code_endf)
  {
    if (sp <= 0)
    {
     printf ("\nstack underflow\n");
     /* exit(0); */
     return;
    }
    cp = stack[--sp];
  }
  else if (ins == code_asr_r0)
    r0.i = r0.i >> 1;
  else if (ins == code_and_r0_r1)
    r0.i = r0.i & r1.i;
  else if (ins == code_xor_r0_r1)
    r0.i = r0.i ^r1.i;
  else if (ins == code_callext)
  {
    a.ins = *cp++;
    a.vf();
  }
  else if (ins == code_getchar)
    r0.i = getchar();
  else if (ins == code_putchar)
    putchar (r0.i);
  else if (ins == code_getword)
  {
    ra.pw = getword();
    r0.i = 0;
    sscanf (awr, "%x", &r0.i);
  }
  else if (ins == code_scanhex)
    r0.i = f_scanhex ();
  else if (ins == code_fillmem)
  {
   int n, i;
   n = (int)(*cp++);
   for (i=0; i<n; i++)
    *(rc.cp++) = *cp++;
  }
  else if (ins == code_exit)
   exit(r0.i);
  else
    printf ("Instruction %X not implemented\n", ins);

 }

}
