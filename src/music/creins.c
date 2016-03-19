/* Creation d'instruments CMF */

#define OLD

#ifdef OLD

#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <conio.h>

#else

#include "cmftool.h"

erreur ()
{
	printf ("Erreur %d ", cmf_err_stat);
	_print_cmf_errmessage ();
	exit (cmf_err_stat);
}
#endif

dump (char *s, int n)
{
int i;
	for (i=0; i<n; i++)
	{
		if ((i & 0xF) == 0)
			printf ("\n %4X: ", i);
		printf ("%02X ", s[i]);
	}
	printf ("\n");
}

union REGS reg;

#define CMFDRV 0x80

struct instrument
{
	struct
	{
		unsigned int mult_freq: 4;
		unsigned int duree_dep_ton: 1;
		unsigned int sustain: 1;
		unsigned int vibr_freq: 1;
		unsigned int vibr_ampl: 1;
	} prop_mod, prop_por;
	struct
	{
		unsigned int magn: 6;
		unsigned int dim_aigu: 2;
	} vol_mod, vol_por;
	struct
	{
		unsigned int decay: 4;
		unsigned int attack: 4;
	} ad_mod, ad_por;
	struct
	{
		unsigned int release: 4;
		unsigned int sustain: 4;
	} sr_mod, sr_por;
	unsigned char distor_mod, distor_por;
	unsigned int parallele: 1;
	unsigned int dephasage: 3;
	int align: 4;
	char extension[5];
};

long read_delta (FILE *f)
{
long delta;
char c;
	delta = 0;
loop:
	c = getc (f);
	if (feof(f)) return -1;
	delta <<= 7;
	delta |= (c & 0x7F);
	if (c & 0x80)
		goto loop;
	return delta;
}

write_delta (unsigned long delta, FILE *f)
{
char c;
int n;
	c = delta >>  28;
	delta <<= 4;
	n = 4;
	while (c == 0 && n > 0)
	{
		c = delta >> 25;
		delta <<= 7;
		n--;
	}
	while (n > 0)
	{
		putc (c | 0x80, f);
		c = delta >> 25;
		delta <<= 7;
		n--;
	}
	putc (c, f);
}

int swrite_delta (unsigned long delta, char *s)
{
char c;
int n;
int nbytes;
	nbytes = 0;
	c = delta >>  28;
	delta <<= 4;
	n = 4;
	while (c == 0 && n > 0)
	{
		c = delta >> 25;
		delta <<= 7;
		n--;
	}
	while (n > 0)
	{
		*s++ = (c | 0x80);
		nbytes++;
		c = delta >> 25;
		delta <<= 7;
		n--;
	}
	/*putc (c, f);*/
	*s++ = c;
	nbytes++;
	return nbytes;
}

#define CM 30
#define CO 40

#define N_CHAMPS 29

int coord_champ [N_CHAMPS] [2] =
{
	{3, CM}, {3, CO},
	{4, CM}, {4, CO},
	{5, CM}, {5, CO},
	{6, CM}, {6, CO},
	{7, CM}, {7, CO},
	{8, CM}, {8, CO},
	{9, CM}, {9, CO},
	{10, CM}, {10, CO},
	{11, CM}, {11, CO},
	{12, CM}, {12, CO},
	{13, CM}, {13, CO},
	{14, CM}, {14, CO},
	{16, CM},
	{17, CM},
	{19, CM},
	{20, CM},
	{21, CM}
};

int prev = 0;

int clavier (char c)
{
	if (c >= 1 && c <= 26)
		return clavier (c + 0x60) + 12;
	if (c >= 'A' && c <= 'Z')
		return clavier (c + 0x20) - 12;
	switch (c)
	{
		case 'q': return -3;
		case 'a': return -2;
		case 'w': return -1;
		case 's': return 0;
		case 'e': return 1;
		case 'd': return 2;
		case 'r': return 3;
		case 'f': return 4;
		case 'g': return 5;
		case 'y': return 6;
		case 'h': return 7;
		case 'u': return 8;
		case 'j': return 9;
		case 'i': return 10;
		case 'k': return 11;
		case 'l': return 12;
		case 'p': return 13;
		case ';': return 14;
		case '[': return 15;
		case '\'': return 16;
		case '\\': return 17;
		case ']': return 18;
		default : return prev;
	}
}

char start_music [] = { 0, 0xC1, 0, 0, 0x91, 60, 90, 0, 252 };
char stop_music  [] = { 0, 0xC1, 0, 0, 0x81, 60, 90, 0, 252 };

creins ()
{
int status_cmf;
int it_par_sec;
struct instrument instr;
char note, attaque;
char c;
int champ;
int lin, col;
int val_champ[N_CHAMPS];
char buf[50];
int i;
char filename[200];
FILE *f;
int flag_play;
int octave;

	flag_play = 0;
	octave = 4;

	it_par_sec = 96;

	reg.x.bx = 1;
	reg.x.dx = FP_SEG(&status_cmf);
	reg.x.ax = FP_OFF(&status_cmf);
	int86 (CMFDRV, &reg, &reg);

	reg.x.bx = 8;
	int86 (CMFDRV, &reg, &reg);
	if (reg.x.ax)
	{
		fprintf (stderr, "Erreur initialisation driver\n");
		if (reg.x.ax == 1)
			fprintf (stderr, "\tMorceau en cours\n");
	}

	reg.x.bx = 4;
	reg.x.ax = 1193180 / it_par_sec;
	int86 (CMFDRV, &reg, &reg);

	reg.x.bx = 5;
	reg.x.ax = 0;
	int86 (CMFDRV, &reg, &reg);

	champ = 0;

/*	note = 60;
	attaque = 90;*/

	val_champ[0] = instr.prop_mod.mult_freq = 1;
	val_champ[1] = instr.prop_por.mult_freq = 1;

	val_champ[2] = instr.prop_mod.duree_dep_ton = 0;
	val_champ[3] = instr.prop_por.duree_dep_ton = 0;

	val_champ[4] = instr.prop_mod.sustain = 1;
	val_champ[5] = instr.prop_por.sustain = 1;

	val_champ[6] = instr.prop_mod.vibr_freq = 0;
	val_champ[7] = instr.prop_por.vibr_freq = 0;

	val_champ[8] = instr.prop_mod.vibr_ampl = 0;
	val_champ[9] = instr.prop_por.vibr_ampl = 0;

	val_champ[10] = instr.vol_mod.magn = 32;
	val_champ[11] = instr.vol_por.magn = 32;

	val_champ[12] = instr.vol_mod.dim_aigu = 0;
	val_champ[13] = instr.vol_por.dim_aigu = 0;

	val_champ[14] = instr.ad_mod.attack = 8;
	val_champ[15] = instr.ad_por.attack = 8;

	val_champ[16] = instr.ad_mod.decay = 8;
	val_champ[17] = instr.ad_por.decay = 8;

	val_champ[18] = instr.sr_mod.sustain = 8;
	val_champ[19] = instr.sr_por.sustain = 8;

	val_champ[20] = instr.sr_mod.release = 8;
	val_champ[21] = instr.sr_por.release = 8;

	val_champ[22] = instr.distor_mod = 0;
	val_champ[23] = instr.distor_por = 0;

	val_champ[24] = instr.parallele = 0;
	val_champ[25] = instr.dephasage = 0;

	val_champ[26] = note = 60;
	val_champ[27] = attaque = 90;

	val_champ[28] = it_par_sec = 96;

	clrscr();

	champ = 0;

	for (;;)
	{
		gotoxy (1,1);

		printf ("Instrument                  : modulateur   porteur \n\n");

#define COL 52
		gotoxy (COL, 3); printf ("Commandes :");
		gotoxy (COL, 5); printf (" fleches : changer de champ");
		gotoxy (COL, 6); printf (" : : modifier le champ");
		gotoxy (COL, 7); printf (" r : instrument aleatoire");
		gotoxy (COL, 8); printf (" s : sauvegarde");
		gotoxy (COL, 9); printf (" l : lecture");
		gotoxy (COL, 10); printf (" espace : jouer 1 note");
		gotoxy (COL, 11); printf (" p : jouer melodie");



/*
		printf ("Multiplicateur de frequence :  1 \t  1 \n");
		printf ("   Duree depend de tonalite :  0 \t  0 \n");
		printf ("                    Sustain :  1 \t  1 \n");
		printf ("          Vibrato frequence :  0 \t  0 \n");
		printf ("                  amplitude :  0 \t  0 \n");
		printf ("                     Volume : 32 \t 32 \n");
		printf ("           Diminution aigus :  0 \t  0 \n");
		printf ("             Vitesse Attack :  8 \t  8 \n");
		printf ("                     Decay  :  8 \t  8 \n");
		printf ("             Niveau Sustain :  8 \t  8 \n");
		printf ("            Vitesse Release :  8 \t  8 \n");
		printf ("                 Distorsion :  0 \t  0 \n");
		printf ("\n");
		printf ("     Connexion en parallele : 0 \n");
		printf ("                  Dephasage : 0 \n");
*/

		gotoxy (coord_champ[champ][1], coord_champ[champ][0]);

		c = getch();

		if (flag_play)
		{
			if (c == '\r')
			{
				flag_play = 0;
			}
			else if (c >= '0' && c <= '9')
			{
				octave = c - '0';
				{ int i; for (i=0; i<30000; i++); }
			}
			else
			{
				val_champ[26] = 12 * octave +
					(prev = clavier (c));
				goto play;
			}
		}

		else
		switch (c)
		{
			case 0:
				c = getch();
				switch (c)
				{
					case 0x4B: champ--;	break;
					case 0x4D: champ++;     break;
					case 0x48: champ -= 2;  break;
					case 0x50: champ += 2;  break;
					default: ;
				}
				break;
			case ':':
				scanf ("%d", &(val_champ[champ]));
				break;

			case 'r':
				for (i=0; i<N_CHAMPS; i++)
					/*((char *)&instr)[i] = random(0x100);*/
					val_champ[i] = random(0x100);
				break;

			case 's':
#define CPROMPT 3
#define LPROMPT 23
				gotoxy (CPROMPT, LPROMPT);
				printf (" Filename ? ");
				gets (filename);
				f = fopen (filename, "wb");
				if (f == NULL)
					perror (filename);
				else
				{
					fwrite (val_champ, sizeof(val_champ),
						1, f);
					fclose (f);
				}
				break;

			case 'l':
				gotoxy (CPROMPT, LPROMPT);
				printf (" Filename ? ");
				gets (filename);
				f = fopen (filename, "rb");
				if (f == NULL)
					perror (filename);
				else
				{
					fread (val_champ, sizeof(val_champ),
						1, f);
					fclose (f);
				}
				break;

			case 'p':
				flag_play = 1;
				break;

			case ' ':

			play:
				instr.prop_mod.mult_freq = val_champ[0];
				instr.prop_por.mult_freq = val_champ[1];

				instr.prop_mod.duree_dep_ton = val_champ[2];
				instr.prop_por.duree_dep_ton = val_champ[3];

				instr.prop_mod.sustain = val_champ[4];
				instr.prop_por.sustain = val_champ[5];

				instr.prop_mod.vibr_freq = val_champ[6];
				instr.prop_por.vibr_freq = val_champ[7];

				instr.prop_mod.vibr_ampl = val_champ[8];
				instr.prop_por.vibr_ampl = val_champ[9];

				instr.vol_mod.magn = 63 - val_champ[10];
				instr.vol_por.magn = 63 - val_champ[11];

				instr.vol_mod.dim_aigu = val_champ[12];
				instr.vol_por.dim_aigu = val_champ[13];

				instr.ad_mod.attack = val_champ[14];
				instr.ad_por.attack = val_champ[15];

				instr.ad_mod.decay = val_champ[16];
				instr.ad_por.decay = val_champ[17];

				instr.sr_mod.sustain = val_champ[18];
				instr.sr_por.sustain = val_champ[19];

				instr.sr_mod.release = val_champ[20];
				instr.sr_por.release = val_champ[21];

				instr.distor_mod = val_champ[22];
				instr.distor_por = val_champ[23];

				instr.parallele = val_champ[24];
				instr.dephasage = val_champ[25];

				note = val_champ[26];
				attaque = val_champ[27];
				it_par_sec = val_champ[28];
		gotoxy (1, 3);
		printf ("\rMultiplicateur de frequence :%3d \t%3d \n",
			instr.prop_mod.mult_freq,
			instr.prop_por.mult_freq);
		printf ("   Duree depend de tonalite :%3d \t%3d \n",
			instr.prop_mod.duree_dep_ton,
			instr.prop_por.duree_dep_ton);
		printf ("                    Sustain :%3d \t%3d \n",
			instr.prop_mod.sustain,
			instr.prop_por.sustain);
		printf ("          Vibrato frequence :%3d \t%3d \n",
			instr.prop_mod.vibr_freq,
			instr.prop_por.vibr_freq);
		printf ("                  amplitude :%3d \t%3d \n",
			instr.prop_mod.vibr_ampl,
			instr.prop_por.vibr_ampl);
		printf ("                     Volume :%3d \t%3d \n",
			63-instr.vol_mod.magn,
			63-instr.vol_por.magn);
		printf ("           Diminution aigus :%3d \t%3d \n",
			instr.vol_mod.dim_aigu,
			instr.vol_por.dim_aigu);
		printf ("             Vitesse Attack :%3d \t%3d \n",
			instr.ad_mod.attack,
			instr.ad_por.attack);
		printf ("                     Decay  :%3d \t%3d \n",
			instr.ad_mod.decay,
			instr.ad_por.decay);
		printf ("             Niveau Sustain :%3d \t%3d \n",
			instr.sr_mod.sustain,
			instr.sr_por.sustain);
		printf ("            Vitesse Release :%3d \t%3d \n",
			instr.sr_mod.release,
			instr.sr_por.release);
		printf ("                 Distorsion :%3d \t%3d \n",
			instr.distor_mod % 4,
			instr.distor_por % 4);
		printf ("\n");
		printf ("     Connexion en parallele :%3d \n",
			instr.parallele);
		printf ("                  Dephasage :%3d \n",
			instr.dephasage);

		printf ("\n");
		printf ("                       Note :%3d \n", note);
		printf ("                    Attaque :%3d \n", attaque);

		printf ("  Interruptions par seconde :%3d \n", it_par_sec);

				start_music[5] = note;
				start_music[6] = attaque;
				stop_music[5] = note;
				stop_music[6] = attaque;

		gotoxy (coord_champ[champ][1], coord_champ[champ][0]);

	reg.x.bx = 4;
	reg.x.ax = 1193180 / it_par_sec;
	int86 (CMFDRV, &reg, &reg);

	reg.x.bx = 2;
	reg.x.cx = 1;
	reg.x.dx = FP_SEG(&instr);
	reg.x.ax = FP_OFF(&instr);
	int86 (CMFDRV, &reg, &reg);

		reg.x.bx = 6;
		reg.x.dx = FP_SEG(start_music);
		reg.x.ax = FP_OFF(start_music);
		int86 (CMFDRV, &reg, &reg);
		/*if (reg.x.ax)
		{
			fprintf (stderr, "Erreur musique 0x%X\n", reg.x.ax);
			if (reg.x.ax == 1)
				fprintf (stderr, "\tMorceau en cours\n");
		}*/

		/* sleep (1); */
		{ int i; for (i=0; i<30000; i++); }

		while (!status_cmf && !kbhit());

		reg.x.bx = 6;
		reg.x.dx = FP_SEG(stop_music);
		reg.x.ax = FP_OFF(stop_music);
		int86 (CMFDRV, &reg, &reg);
		/*if (reg.x.ax)
		{
			fprintf (stderr, "Erreur musique 0x%X\n", reg.x.ax);
			if (reg.x.ax == 1)
				fprintf (stderr, "\tMorceau en cours\n");
		}*/

		while (!status_cmf && !kbhit());

		reg.x.bx = 7;
		int86 (CMFDRV, &reg, &reg);
		/*if (reg.x.ax)
		{
			fprintf (stderr, "Erreur stop\n");
			if (reg.x.ax == 1)
				fprintf (stderr, "\tAucun morceau en cours\n");
			else
				fprintf (stderr, "\t0x%X\n", reg.x.ax);
		}*/


				break;
			case 'q':

		reg.x.bx = 8;
		int86 (CMFDRV, &reg, &reg);
		if (reg.x.ax)
		{
			fprintf (stderr, "Erreur reset\n");
			if (reg.x.ax == 1)
				fprintf (stderr, "\tMorceau en cours\n");
			else
				fprintf (stderr, "\t0x%X\n", reg.x.ax);
		}

		reg.x.bx = 1;
		reg.x.dx = 0;
		reg.x.ax = 0;
		int86 (CMFDRV, &reg, &reg);

				return;

			default:
			play1:
				val_champ[26] = c - 0x40;
				goto play;
				;
		}
	}

/*
		printf ("Multiplicateur de frequence ? ");
		scanf ("%d", &x); instr.prop_mod.mult_freq = x;
		scanf ("%d", &x); instr.prop_por.mult_freq = x;

		printf ("   Duree depend de tonalite ? ");
		scanf ("%d", &x); instr.prop_mod.duree_dep_ton = x;
		scanf ("%d", &x); instr.prop_por.duree_dep_ton = x;

		printf ("                    Sustain ? ");
		scanf ("%d", &x); instr.prop_mod.sustain = x;
		scanf ("%d", &x); instr.prop_por.sustain = x;

		printf ("          Vibrato frequence ? ");
		scanf ("%d", &x); instr.prop_mod.vibr_freq = x;
		scanf ("%d", &x); instr.prop_por.vibr_freq = x;

		printf ("                  amplitude ? ");
		scanf ("%d", &x); instr.prop_mod.vibr_ampl = x;
		scanf ("%d", &x); instr.prop_por.vibr_ampl = x;

		printf ("                     Volume ? ");
		scanf ("%d", &x); instr.vol_mod.magn = 64-x;
		scanf ("%d", &x); instr.vol_por.magn = 64-x;

		printf ("           Diminution aigus ? ");
		scanf ("%d", &x); instr.vol_mod.dim_aigu = x;
		scanf ("%d", &x); instr.vol_por.dim_aigu = x;

		printf ("             Vitesse Attack ? ");
		scanf ("%d", &x); instr.ad_mod.attack = x;
		scanf ("%d", &x); instr.ad_por.attack = x;

		printf ("                     Decay  ? ");
		scanf ("%d", &x); instr.ad_mod.decay = x;
		scanf ("%d", &x); instr.ad_por.decay = x;

		printf ("             Niveau Sustain ? ");
		scanf ("%d", &x); instr.sr_mod.sustain = x;
		scanf ("%d", &x); instr.sr_por.sustain = x;

		printf ("            Vitesse Release ? ");
		scanf ("%d", &x); instr.sr_mod.release = x;
		scanf ("%d", &x); instr.sr_por.release = x;

		printf ("                 Distorsion  ");
		scanf ("%d", &x); instr.distor_mod = x;
		scanf ("%d", &x); instr.distor_por = x;

		printf ("Connexion en parallele ? ");
		scanf ("%d", &x); instr.parallele = x;
		scanf ("%d", &x); instr.dephasage = x;

		printf ("\n");
*/
}

main (int argc, char *argv[])
{
	creins ();
}

testcmf (int argc, char *argv[])
{
FILE *f;
struct
{
	char ident[4];
	char version;
	char sous_version;
	int offset_instr;
	int offset_music;
	int tics_par_qnote;
	int it_par_sec;
	int offset_titre;
	int offset_auteur;
	int offset_rem;
	char canal[16];
	int n_instr;
	int rythme;
} header;
struct instrument instr, tab_instr[50];
int status;
char c, c1;
int i;
long delai;
char message, canal;
long x;
int status_cmf;
char tab_music[20000];
char *p;
char tab_cmf[30000];

	printf ("Creation du fichier CMF %s :\n", argv[1]);
	f = fopen (argv[1], "wb");
	if (f == NULL)
	{
	bad_file:
		perror (argv[1]);
		/* return errno; */
	}
	memcpy (header.ident, "CTMF", 4);
	header.version = 1;
	header.sous_version = 1;
	printf ("Nombre de tics par 1/4 de note ? ");
	scanf ("%d", &header.tics_par_qnote);
	printf ("Nombre d'interruptions par seconde ? ");
	scanf ("%d", &header.it_par_sec);
	header.offset_titre = 0;
	header.offset_auteur = 0;
	header.offset_rem = 0;

	printf ("Canaux  : \n");
	for (i=0; i<16; i++)
	{
		printf ("\t%d ? ", i);
		scanf ("%d", &header.canal[i]);
	}

	printf ("\n");
	printf ("Nombre d'instruments ? ");
	scanf ("%d", &header.n_instr);

	printf ("Rythme ? ");
	scanf ("%d", &header.rythme);

	header.offset_instr = sizeof(header);
	header.offset_music = sizeof(header) + header.n_instr * sizeof(instr);

	printf ("Header:"); dump (&header, sizeof(header));

	memcpy (tab_cmf, &header, sizeof(header));

	status = fwrite (&header, sizeof(header), 1, f);
	/*if (status != 1) goto bad_file;*/

#ifdef OLD
	reg.x.bx = 1;
	reg.x.dx = FP_SEG(&status_cmf);
	reg.x.ax = FP_OFF(&status_cmf);
	int86 (CMFDRV, &reg, &reg);

	reg.x.bx = 8;
	int86 (CMFDRV, &reg, &reg);
	if (reg.x.ax)
	{
		fprintf (stderr, "Erreur initialisation driver\n");
		if (reg.x.ax == 1)
			fprintf (stderr, "\tMorceau en cours\n");
	}

	reg.x.bx = 4;
	reg.x.ax = 1193180 / header.it_par_sec;
	int86 (CMFDRV, &reg, &reg);

	reg.x.bx = 5;
	reg.x.ax = 0;
	int86 (CMFDRV, &reg, &reg);
#else
	if (_cmf_prepare_use() == FALSE) erreur();
	_cmf_set_transposeofs(0);
	_cmf_set_driverclock((WORD)header.it_par_sec);
#endif
	status = fseek (f, header.offset_instr, SEEK_SET);
	/*if (status) goto bad_file;*/
	/*
	for (i=0; i<header.n_instr; i++)
	{ */
	i=0;
	loop_instr:
	if (!(i<header.n_instr))
	  goto exit_instr;
		/* status = fread (&instr, sizeof(instr), 1, f);
		if (status != 1) goto bad_file;                  */
		printf ("\nInstrument %d                : modulateur \t porteur \n", i);
/*
		printf ("Multiplicateur de frequence : %d         \t %d\n",
			instr.prop_mod.mult_freq,
			instr.prop_por.mult_freq);
*/
		printf ("Multiplicateur de frequence ? ");
		scanf ("%d", &x); instr.prop_mod.mult_freq = x;
		scanf ("%d", &x); instr.prop_por.mult_freq = x;

		printf ("   Duree depend de tonalite ? ");
		scanf ("%d", &x); instr.prop_mod.duree_dep_ton = x;
		scanf ("%d", &x); instr.prop_por.duree_dep_ton = x;

		printf ("                    Sustain ? ");
		scanf ("%d", &x); instr.prop_mod.sustain = x;
		scanf ("%d", &x); instr.prop_por.sustain = x;

		printf ("          Vibrato frequence ? ");
		scanf ("%d", &x); instr.prop_mod.vibr_freq = x;
		scanf ("%d", &x); instr.prop_por.vibr_freq = x;

		printf ("                  amplitude ? ");
		scanf ("%d", &x); instr.prop_mod.vibr_ampl = x;
		scanf ("%d", &x); instr.prop_por.vibr_ampl = x;

		printf ("                     Volume ? ");
		scanf ("%d", &x); instr.vol_mod.magn = 64-x;
		scanf ("%d", &x); instr.vol_por.magn = 64-x;

		printf ("           Diminution aigus ? ");
		scanf ("%d", &x); instr.vol_mod.dim_aigu = x;
		scanf ("%d", &x); instr.vol_por.dim_aigu = x;

		printf ("             Vitesse Attack ? ");
		scanf ("%d", &x); instr.ad_mod.attack = x;
		scanf ("%d", &x); instr.ad_por.attack = x;

		printf ("                     Decay  ? ");
		scanf ("%d", &x); instr.ad_mod.decay = x;
		scanf ("%d", &x); instr.ad_por.decay = x;

		printf ("             Niveau Sustain ? ");
		scanf ("%d", &x); instr.sr_mod.sustain = x;
		scanf ("%d", &x); instr.sr_por.sustain = x;

		printf ("            Vitesse Release ? ");
		scanf ("%d", &x); instr.sr_mod.release = x;
		scanf ("%d", &x); instr.sr_por.release = x;

		printf ("                 Distorsion ? ");
		scanf ("%d", &x); instr.distor_mod = x;
		scanf ("%d", &x); instr.distor_por = x;

		printf ("Connexion en parallele ? ");
		scanf ("%d", &x); instr.parallele = x;
		printf ("Dephasage ? ");
		scanf ("%d", &x); instr.dephasage = x;

		status = fwrite (&instr, sizeof(instr), 1, f);
		/*if (status != 1) goto bad_file;*/

		memcpy (&(tab_instr[i]), &instr, sizeof(instr));
	/* } */
	i++;
	goto loop_instr;
	exit_instr:

	memcpy (tab_cmf + sizeof(header), tab_instr,
		sizeof(instr)*header.n_instr);

	printf ("Instruments:"); dump (tab_instr, sizeof(instr)*header.n_instr);

#ifdef OLD
	reg.x.bx = 2;
	reg.x.cx = header.n_instr;
	reg.x.dx = FP_SEG(tab_instr);
	reg.x.ax = FP_OFF(tab_instr);
	int86 (CMFDRV, &reg, &reg);
#else
	_cmf_set_singleinstruments (tab_instr, header.n_instr);
#endif
	printf ("\nMusique : \n");

	status = fseek (f, header.offset_music, SEEK_SET);
	/*if (status) goto bad_file;*/

	p = tab_music;

loop_music:

	/*
	delai = read_delta (f);
	if (feof(f))
	{
		printf ("\nFin des donnees.\n");
		fclose (f);
		return 0;
	}
	printf (" %9d -> ", delai);
	*/

	printf ("\tDelai ? ");
	scanf ("%ld", &delai);
	if (delai == -1)
	{
		printf ("\nFin des donnees.\n");
		fclose (f);

		*p++ = 252;
		*p++ = 0;

		printf ("Musique:"); dump (tab_music, p-tab_music);

		memcpy (tab_cmf + sizeof(header) + sizeof(instr)*header.n_instr,
			tab_music,
			p - tab_music);

		printf ("CMF:"); dump (tab_cmf, sizeof(header) +
					sizeof(instr)*header.n_instr +
					p-tab_music);
#ifndef OLD
		_cmf_play_song (tab_cmf);

		do
		{
			printf ("\r\t%X ", cmf_status_byte);
		}
		while ((cmf_status_byte != 0) && (kbhit() != 0));

		if (kbhit() != 0)
			if (_cmf_stop_song() != TRUE) erreur();

		if (_cmf_reset_driver() != TRUE) erreur();
#endif
		reg.x.bx = 6;
		reg.x.dx = FP_SEG(tab_music);
		reg.x.ax = FP_OFF(tab_music);
		int86 (CMFDRV, &reg, &reg);
		if (reg.x.ax)
		{
			fprintf (stderr, "Erreur musique 0x%X\n", reg.x.ax);
			if (reg.x.ax == 1)
				fprintf (stderr, "\tMorceau en cours\n");
		}
#ifdef OLD
		while (!kbhit());

		reg.x.bx = 7;
		int86 (CMFDRV, &reg, &reg);
		if (reg.x.ax)
		{
			fprintf (stderr, "Erreur stop\n");
			if (reg.x.ax == 1)
				fprintf (stderr, "\tAucun morceau en cours\n");
			else
				fprintf (stderr, "\t0x%X\n", reg.x.ax);
		}

		reg.x.bx = 8;
		int86 (CMFDRV, &reg, &reg);
		if (reg.x.ax)
		{
			fprintf (stderr, "Erreur reset\n");
			if (reg.x.ax == 1)
				fprintf (stderr, "\tMorceau en cours\n");
			else
				fprintf (stderr, "\t0x%X\n", reg.x.ax);
		}

		reg.x.bx = 1;
		reg.x.dx = 0;
		reg.x.ax = 0;
		int86 (CMFDRV, &reg, &reg);

#else
		do printf ("\r\t Status = %4X", cmf_status_byte);
		while ((cmf_status_byte != 0) && (kbhit() == 0));

		if (kbhit() != 0)
			if (_cmf_stop_song() != TRUE) erreur();

		if (_cmf_reset_driver() != TRUE) erreur();

		_cmf_terminate_use();
#endif
		return errno;
	}
	write_delta (delai, f);
	p += swrite_delta (delai, p);

	printf ("Message ? 0x");
	scanf ("%X", &x);
	c = x;

/*
	loop_message:
		c = getc (f);
		if (!(c & 0x80))
		{
			printf ("?%X ", c & 0xFF);
			goto loop_message;
		}
*/

	message = (c >> 4) & 0x0F;
	canal = c & 0x0F;

	if (message != 0xF)
		printf ("Canal %2d : ", canal);

	putc (c, f);
	*p++ = c;

	switch (message)
	{
		case 8:
			printf ("Note off");
		data_2:
			/* c = getc (f); */
			printf ("\tData ? ");
			scanf ("%d", &x);
			c = x;
			putc (x, f);
			*p++ = c;
			if (c & 0x80)
				fprintf (stderr, "*** Donnees MIDI incorrectes ***");
			/* printf (" %d", c); */
		data_1:
			/* c = getc (f); printf (" %d", c); */
			printf ("\tData ? ");
			scanf ("%d", &x);
			c = x;
			putc (x, f);
			*p++ = c;
			if (c & 0x80)
				fprintf (stderr, "*** Donnees MIDI incorrectes ***");
			break;
		case 9:
			printf ("Note on");
			goto data_2;
		case 0xA:
			printf ("Polyphonic after touch");
			goto data_2;
		case 0xB:
			printf ("Control change");
			goto data_2;
			c = getc (f);
			if (c & 0x80)
				printf ("*** Donnees MIDI incorrectes ***");
			c1 = getc (f);
			if (c1 & 0x80)
				printf ("*** Donnees MIDI incorrectes ***");
			printf ("%d ", c);
			printf ("%d ", c1);
			switch (c)
			{
				case 122:
					printf ("Local %d", c1);
					break;
				case 123:
					if (c1 == 0) printf
						("All notes off");
					break;
				case 124:
					if (c1 == 0)
						printf ("Omni off");
					break;
				case 125:
					if (c1 == 0)
						printf ("Omni on");
					break;
				case 126:
					printf ("Mono on %d", c1);
					break;
				case 127:
					if (c1 == 0)
						printf ("Poly on");
					break;
				default:
					;
			}
			break;
		case 0xC:
			printf ("Program change");
			goto data_1;
		case 0xD:
			printf ("After touch");
			goto data_1;
		case 0xE:
			printf ("Pitch bend");
			goto data_2;
			c = getc (f);
			if (c & 0x80)
				printf ("*** Donnees MIDI incorrectes");
			printf (" %d", c | (getc(f) << 7));
			break;
		case 0xF:
			switch (c)
			{
				/*case 242:
					printf ("Song position pointer");
					break;*/
#define def_message(n,t) case n: printf(t); break
				def_message(242,"Song position pointer");
				def_message(243,"Song select");
				def_message(246,"Tune request");
				def_message(248,"Timing clock");
				def_message(250,"Start");
				def_message(251,"Continue");
				def_message(252,"Stop");
				def_message(254,"Active sensing");
				def_message(255,"System reset");

				default:
					printf ("*** Message inconnu ***");
			}
			break;
		default:
			printf ("*** Erreur ***");
	}
	printf ("\n");

	goto loop_music;

}




