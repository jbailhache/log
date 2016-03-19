
#include <stdio.h>
#include <math.h>

#define HEADER \
 "\x4D\x54\x68\x64\x00\x00\x00\x06\x00\x01\x00\x02\x00\xF0\x4D\x54\x72\x6B\x00\x00\x00\x13\x00\xFF\x58\x04\x04\x02\x18\x08\x00\xFF\x51\x03\x07\xA1\x20\x00\xFF\x2F\x00"

varlength (char *buf, long x)
{
 if (x < 128)
 {
  buf[0] = x;
  return 1;
 }
 if (x < 128*128)
 {
  buf[0] = 128+x/128;
  buf[1] = x%128;
  return 2; 
 }
 buf[0] = 128+x/(128*128);
 buf[1] = 128+(x/128)%128;
 buf[2] = x%128;
 return 3; 
}

#define delay varlength

int setinstr (char *buf, int canal, int i)
{
 buf[0] = 0xC0 + canal;
 buf[1] = i;
 return 2;
}

int noteon (char *buf, int c, int n, int v)
{
 buf[0] = 0x90 + c;
 buf[1] = n;
 buf[2] = v;
 return 3;
}

int noteoff (char *buf, int c, int n)
{
 buf[0] = 0x80 + c;
 buf[1] = n;
 buf[2] = 0;
 return 3;
}

int track (char *buf, char *m, int l)
{
 memcpy (buf, "MTrk\x00\x00", 6);
 buf[6] = l/256;
 buf[7] = l%256;
 memcpy (buf+8, m, l);
 return 8+l;
}

float frequence (int note, int octave)
{
int note1;
int ecart;
float ecartoct, ecarte, rapport, freq;
 note1 = note + 12 * octave;
 ecart = note1 - 57;
 ecartoct = (float)ecart / 12.0;
 ecarte = log(2.0) * ecartoct;
 rapport = exp(ecarte);
 freq = 440.0 * rapport;
 return freq;
}

int num_note (int note, int octave)
{
int note1;
 note1 = note + 12 * octave;
 return note1;
}

/*
# A à P : canal
# :nnn : instrument n
# a à l : jouer note
# 0 à 9 : octave
# _ : double durée
# ' : demi durée
# +n : durée + n * 0.1 s
# -n : durée - n * 0.1 s
# ; : silence
# T : tempo
# ( : note qui dure
# *...*) : fin note qui dure

# exemples : aaac_e_ca_ecc_abc'def(afj_h)c
#  :018!1005(afj_h(ce*)h)f
*/

int canal, tempo, tempo1, octave, garde, niveau, silence, volume, numnote1;
struct { int canal; int freq; } pile[300];
int sp;
int shift, vt, va, accords;
int accord[300];
int naccord;
int freq, canal1, note, numnote, instrument, numnote2, v, mode; /***/

init ()
{
 canal = 0;
 tempo = 100;
 tempo1 = tempo;
 octave = 4;
 garde = 0;
 sp = 0;
 niveau = 0;
 silence = 0;
 volume = 0x40;
 numnote1 = 0;
 shift = 0;
 vt = 1;
 va = 1;
 accords = 0;
 naccord = 0;
}

int cremid (char *buf, char *partition)
{
int i, n, rec;
char cmd;
char *ptr;
 ptr = buf;
 for (i=0; i<strlen(partition); i++)
 {
  cmd = partition[i];

  if (cmd >= 'A' && cmd <= 'P')
   canal = cmd - 'A';
  else if (cmd >= '0' && cmd <= '9')
   octave = cmd - '0';
  else if (cmd == '(')
   garde = 1;
  else if (cmd == '*')
   niveau++;
  else if (cmd == '$')
  {
   n = sp;
   canal = pile[n-1-niveau].canal;
   freq = pile[n-1-niveau].freq;
   ptr += delay (ptr, 0);
   ptr += noteoff (ptr, canal, freq);
   niveau = 0;
  }
  else if (cmd == ';')
  {
   ptr += delay (ptr, tempo1);
   ptr += noteoff (ptr, canal1, numnote1);
   numnote1 = -1;
  }
  else if (cmd == ')')
  {
   ptr += delay (ptr, tempo1);
   ptr += noteoff (ptr, canal1, numnote1);
   numnote1 = 0;
   i++;
   note = partition[i] - 'a';
   numnote = num_note (note, octave);
   ptr += delay (ptr, 0);
   ptr += noteoff (ptr, canal, numnote);
  }
  else if (cmd == ':')
  {
   instrument = 100 * (partition[i+1]-'0') + 10 * (partition[i+2]-'0') + partition[i+3]-'0';
   ptr += delay (ptr, 0);
   ptr += setinstr (ptr, canal, instrument);
   i += 3;
  }
  else if (cmd == '!')
  {
   volume = 100 * (partition[i+1]-'0') + 10 * (partition[i+2]-'0') + partition[i+3]-'0';
   i += 3;   
  }
  else if (cmd == ',')
  {
   tempo *= vt;
   volume *= va;
   if (garde == 0)
   {
    int j;
    for (j=0; j<naccord; j++)
    {
     numnote2 = accord[j];
     if (numnote2 > 0)
     {
      char d[10]; 
      int ld;
      if (j == 0)
       ld = delay (d, tempo);
      else
      {
       ld = 1;
       d[0] = 0;
      }
      memcpy (ptr, d, ld);
      ptr += ld;
     }
    }
   }
   naccord = 0;
  }
  else if (cmd >= 'a' && cmd <= 'z')
  {
   note = cmd - 'a' + shift;
   if (!accords)
   {
    tempo *= vt;
    volume *= va;
   }
   numnote = num_note (note, octave);
   if (!garde && numnote1 > 0 && !accords)
   {
    ptr += delay (ptr, tempo1);
    ptr += noteoff (ptr, canal1, numnote1);
   }
   if (numnote1 == -1)
    silence = tempo1;
   ptr += delay (ptr, silence);
   ptr += noteon (ptr, canal, numnote, volume); /***/
   if (accords == 1)
   {
    accord[naccord++] = numnote;
   }
   if (garde)
   {
    pile[sp].canal = canal;
    pile[sp].freq = numnote;
    sp++;
   }
   tempo1 = tempo;
   garde = 0;
   silence = 0;
   canal1 = canal;
   numnote1 = numnote;
  }
  else if (cmd == 'W')
  {
   silence = tempo1;
   tempo1 = tempo;
  }
  else if (cmd == 'T')
  {
   tempo = 100 * (partition[i+1]-'0') + 10 * (partition[i+2]-'0') + partition[i+3]-'0';
   i += 3;
  }
  else if (cmd == 'S')
  {
   shift = 100 * (partition[i+1]-'0') + 10 * (partition[i+2]-'0') + partition[i+3]-'0';
   i += 3;
  }
  else if (cmd == 'V')
  {
   char b[6];
   float x;
   v = partition[i+1];
   memcpy (b, partition+i+2, 5);
   b[5] = 0;
   sscanf (b, "%f", &x);
   if (v == 'T')
    vt = x;
   else if (v == '!')
    va = x;
   i += 6;
  }
  else if (cmd == '^')
  {
   mode = partition[i+1];
   i++;
   if (mode == 'A')
    accords = 1;
   else if (mode == '1')
    accords = 0;
  }
  else if (cmd == 'U')
  {
   i++;
   if (partition[i] == '_')
    tempo *= 2;
   else if (partition[i] == '\'')
    tempo /= 2;
   else if (partition[i] == '+')
   {
    i++;
    tempo += 100 * (partition[i] - '0');
   }
   else if (partition[i] == '-')
   {
    i++;
    tempo -= 100 * (partition[i] - '0');
   }
  }
  else if (cmd == '_')
   tempo1 *= 2;
  else if (cmd == '\'')
   tempo1 /= 2;
  else if (cmd == '+')
  {
   i++;
   tempo1 += 100 * (partition[i] - '0');
  }
  else if (cmd == '-')
  {
   i++;
   tempo1 -= 100 * (partition[i] - '0');
  }

 }

 ptr += delay (ptr, tempo1);
 ptr += noteoff (ptr, canal1, numnote1);

 ptr += delay (ptr, 0);

 memcpy (ptr, "\xFF\x2F\x00", 3);
 ptr += 3;

 return ptr-buf;
}

char mid1[10000];
char mid[10000];
int size;
FILE *f;

main ()
{
 init();
 /* size = cremid (mid1, "aaace_c_aecca_"); */
 size = cremid (mid1,
  ":056!060T1503h(4c(h_fe_jh_jjf_ef_jh__j_lm_qr_qqm_lm_qo__v_rq_rv_yyx_rq_vt__t_qo_mo_qqm_lj_fh__m_lf_ej_mml_fe_jh___:102!100T1505chmo_omjhfh_hfcac_4omhfh_hhfcach_hfca3hfh_h4ach_hhcaef_fca3hfh:056!060T1503h(4c(h_hffefhh_lhhfh_jmo_ortqomo_oljlhfh_efjjhfh_hlo_ort_tqo_omo_o_oljhfhh_hfefjjhfh_hlmojlf_efh__:110!100T1003h(4c(lmoh_hfh_ojfhl_lojlefh_lhfh_mmjflmh_fefh_lheh_jlomlmooo_ljlmh_fefh_lhgh_gfgh_lorqmor_qojmh_fefh_o'm'l'h_fh_feh__:019!100T1503h(c(4lm_o_mlmoj_hf_h_fhlmj_hj_hf_h_jjfeh_holjm_ljfh_hort_rqmo_jfeh__:061!080T15043h(4c(hmort_tomjhfh_hhmhlmo_omljjh_fh_hqqhmo_ojjhfh_hohfh_hottqmj_jjhhfeh(qrtt_qo)hol_jhh_f_oh_fh_qr_tt_rq_om_oo_lj_hf_hh_ho_rt_tqo_mo_ol_jf_efj_lh_fh_ho_hh_fe_fj_lm_jj_eh__:019!100T1503j(4c(l_m_mlj_jlmmljh_hfh_m_o_omj_jmoomjhfh_r_t_tqo_ojo_oqrrqoomo_o_olj_jfh_hmoort_tqormlo_oljjhj_lmhfj_hfh__:061!080T1503h(4c(fjlm_mfjh_jmr_qmqo(o_)hljjfh(h_)otr_qmmlmo(o_)hlffefh(h_)ott_rqoomo(o_)hjml_fjh(h_)otrq_ml_o(o_)hlh_j_lm_h(h_)omrq_ml_o_rtq_mlo(o_)jfe_flj_fef_jl_h__:019!100T1503j(4c(jjf_h(m_rrqmo(o)hjlmml_jjhfh(h)ooqrtt_rqomo_jf_hmrqmo_hjlml_jhfho_qrt_rqomo_jef_hmrttqmo_hjmjl_jhfhco_qtr_rqomo___");

 size = track (mid, mid1, size);
 f = fopen ("1.mid", "wb");
 /*fwrite ("TEST", 4, 1, f);*/
 fwrite (HEADER, 41, 1, f);
 fwrite (mid, size, 1, f);
}