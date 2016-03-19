
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
       ld = delay (d, tempo1);
      else
      {
       ld = 1;
       d[0] = 0;
      }
      memcpy (ptr, d, ld);
      ptr += ld;
      ptr += noteoff (ptr, canal, numnote2);
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

main (int argc, char *argv[])
{
 init();
 /* size = cremid (mid1, "aaace_c_aecca_"); */
 /* size = cremid (mid1,
  ":056!060T1503h(4c(h_fe_jh_jjf_ef_jh__j_lm_qr_qqm_lm_qo__v_rq_rv_yyx_rq_vt__t_qo_mo_qqm_lj_fh__m_lf_ej_mml_fe_jh___:102!100T1505chmo_omjhfh_hfcac_4omhfh_hhfcach_hfca3hfh_h4ach_hhcaef_fca3hfh:056!060T1503h(4c(h_hffefhh_lhhfh_jmo_ortqomo_oljlhfh_efjjhfh_hlo_ort_tqo_omo_o_oljhfhh_hfefjjhfh_hlmojlf_efh__:110!100T1003h(4c(lmoh_hfh_ojfhl_lojlefh_lhfh_mmjflmh_fefh_lheh_jlomlmooo_ljlmh_fefh_lhgh_gfgh_lorqmor_qojmh_fefh_o'm'l'h_fh_feh__:019!100T1503h(c(4lm_o_mlmoj_hf_h_fhlmj_hj_hf_h_jjfeh_holjm_ljfh_hort_rqmo_jfeh__:061!080T15043h(4c(hmort_tomjhfh_hhmhlmo_omljjh_fh_hqqhmo_ojjhfh_hohfh_hottqmj_jjhhfeh(qrtt_qo)hol_jhh_f_oh_fh_qr_tt_rq_om_oo_lj_hf_hh_ho_rt_tqo_mo_ol_jf_efj_lh_fh_ho_hh_fe_fj_lm_jj_eh__:019!100T1503j(4c(l_m_mlj_jlmmljh_hfh_m_o_omj_jmoomjhfh_r_t_tqo_ojo_oqrrqoomo_o_olj_jfh_hmoort_tqormlo_oljjhj_lmhfj_hfh__:061!080T1503h(4c(fjlm_mfjh_jmr_qmqo(o_)hljjfh(h_)otr_qmmlmo(o_)hlffefh(h_)ott_rqoomo(o_)hjml_fjh(h_)otrq_ml_o(o_)hlh_j_lm_h(h_)omrq_ml_o_rtq_mlo(o_)jfe_flj_fef_jl_h__:019!100T1503j(4c(jjf_h(m_rrqmo(o)hjlmml_jjhfh(h)ooqrtt_rqomo_jf_hmrqmo_hjlml_jhfho_qrt_rqomo_jef_hmrttqmo_hjmjl_jhfhco_qtr_rqomo___");
*/

#if 0
 size = cremid (mid1, argv[1]);
#else
 size = cremid (mid1,
   "T150^Aaeh,aeh,aeh,cfj,ehl_,cfj_,aeh,ehl,cfj,cfj,aeh_"
#if 0
  ":048!060T1503h(4c_m_l_mo_qr_qmo_mlhj_fj_lh_ohoqmo_mlfj_lh_lmo_mljlmo_ljh_fh_ohotrqo_mqo_omljlmj_hfh_lh_fh_fhmo_mlj_fef_jlh_jfh__3)h"
  ":056!060T1503i(4d(i_iggii_ipigi_kiknpnp_s_wsu_uspnkigkni_b_3k4g_kgi_igknpwsu_upnkgi___kgignpi_gk_igi(suwspnu_)iu_pnkgi_gnpgkni_kgi(su_suusu_inkgkni___)i3)i4)d"
/*  ":094!030T1503h(4c(:006h_lm_o_o_lj_c_hl_m_o_ml_j_h_fe_h_j_ml_h_m_qr_o_t_rq_m_l_jl_o_j_fe_h__"
    ":056!060T1503h(4c(lfjlmh_jmlfhj_jefjlh_jfeh_flmo_mlfh_fmol_jfeh_lfjloljfjh_qeh__"
    ":056!060T1503h(4c(h_lmo_qtrmlqro_jlfeh_mqrt_tromo_jlh_feh_lm_qo_moo_lj_hf_efh__"
    ":056!060T1503i(4d(pnkgi_gii_ginkp_nswsu_usnpkgi_npw_wusuu_usnpkgi__"
    ":056!060T1503h(4c(lfjlmh_jmlfhj_jefjlh_jfeh_flmo_mlfh_fmol_jfeh_lfjloljfjh_qeh__"
*/
    ":056!060T1503h(4c(h_lmo_qtrmlqro_jlfeh_mqrt_tromo_jlh_feh_lm_qo_moo_lj_hf_efh__"
    ":041!060T1503h(4c(h_j_jf_fh_l_m_jh_fh_o_m_ljl_mj_l_j_efe_jh_m_r_qml_mo_m_j_fej_lh_r_t_qro_qm_l_j_hfe_fh_m_r_qom_oo_q_l_mjl_fh__"
    ":056!060T150m_ljj_hfh_mqr_qoo_mql_jfh_mfjlo_qrt_rqo_mljfh_y_vxt_rv_x_yvr_vt_rqt__"
    ":040!060T1503h(4c(h(orqm_qrmo_t_rqm_q_rmo_l_jlm_r_qmo_m_ljl_j_mqo_)hm_ljf_j_lmh_f_efh(o_trq_m_lmo_)hm_ljf_e_fjh__"
    "h(m_lh_jlaj_fh_o_lj_lmjh_fh_q_rt_rqm_or_qmo_lj_hjj___"
    ":104!060T150hjlm_ljfh_hlmq_rqmo_mljf_efjh_lmrq_omlo_mljf_jlmh__"
    ":048!060T1503h(4c(t_rqo_mo_qr_qm(mo_o_lj_hlm_jf_eh_h_lm_orq_mo_o_rq_mlj_fh__t_rqo_mo_rq_om_oqr_qom_oo_o_lf_jl_mjf_ef_jlh_fh_h_mq_ro_omm_moq_qom_oo_o_lm_ojl_mj_fef_jl_mjf_jjh_hfh__"

    ":048!060T1503c(h(4cceffeac_ehjmmlfh___aefh_efjh_ah_fec_eac_o_ljh_jfh_feac(lmqo_ljh_eh)cefc_ac___mmmlmoq_rrrqmlo_ooo_mlfj_mmm_ljfh___mmlfjmlfh_mmqrqmqrqm_mlkfh___qqrttrqqomo___hmo_hmqmo_vomo___jmmljhj_jmmljh_fh___"

    ":006!060T1503c(h(4ooqr3c(h(4m_m_3c(h(4rrqmo_o_3c(h(4oolj3c(h(4f_f_3c(h(4eefjh_h_3c(h(4hhlm3c(h(4q_3c(h(4rrqmo_molf3c(h(4h_trqmo_3c(h(4qrom3c(h(4o_omljf_3c(h(4efjl3c(h(4h_lmqr3c(h(o_r3c(h(4qlmo_mljf3c(h(4h_y_xv_xy_xvvtr3c(h(4t_rqom3c(h(4o_3c(h(4lmjf3c(h(4h_mlmqr3c(h(4o_3c(h(4qr3c(h(4omo_mljf3c(h(4h_3c(h(mqrt3c(h(o_qrmlo_3c(h(4mqrv3c(h(4t_rxyv3c(h(4t_qrom3c(h(4o_3c(h(4mljf3c(h(4h__"

    ":048!060T1504h_fjjfh_lmo_rqqmo_oljmfeh_mqtroqmlo_ljfeh_lmqro_qmo_mjlmh_jfh_hecac3o_mljlfh_4eefhefac_omljfh_hjmqo_lmjfh_ecac_hlmrqo_mljhfh_j_l_m_q_o__m_l_h_e_a_c___"
#endif
  );
#endif

 size = track (mid, mid1, size);
 f = fopen ("1.mid", "wb");
 /*fwrite ("TEST", 4, 1, f);*/
 fwrite (HEADER, 41, 1, f);
 fwrite (mid, size, 1, f);
}