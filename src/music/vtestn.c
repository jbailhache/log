/*
 ********************************************************************
 * Module pour piloter la Carte sound-blaster avec  Borland C++ 3.0 *
 *             en utilisant le driver CT-VOICE.DRV.                 *
 ********************************************************************
 *                 (C) 1992 MICRO APPLICATION                       *
 *                    Auteur : Axel Stolz                           *
 ********************************************************************
*/

#include <conio.h>

#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <bios.h>
#include <fcntl.h>

#ifdef TRACE
#define trace(format,variable) \
{ \
char format1[1000]; \
	sprintf (format1, "%s(%d) %s : %s.\n", \
		__FILE__, __LINE__, #variable, format); \
	printf (format1, variable); \
}

#else
#define trace(format,variable)\
{ \
char format1[1000]; \
	sprintf (format1, "%s(%d) %s : %s.\n", \
		__FILE__, __LINE__, #variable, format); \
}
#endif

#define cal(f) (((void (far *)())(f))())

#define FP_SEG(fp) (*(int *)(((char *)&(fp))+2))

#define  FALSE  0
#define  TRUE   1

#define  DWORD  unsigned long
#define  WORD   unsigned int
#define  BYTE	unsigned char
#define  VOCPTR char far*



const char voctool_version[] = "v1.5"; /* NumÇro de version de VOCTool */
const BYTE voc_breakend      = 0;      /* Constante de fin de boucle */
const BYTE voc_breaknow      = 1;      /* Constante de fin de boucle */

   WORD   voc_status_word      = 0; /* Variable SB status      */
   WORD   voc_err_stat         = 0; /* Variable nß d'erreur driver */
   char   vocfile_header[]     =    /* Variable d'entàte Format CT */
              "Creative Voice File\0x1A\0x1A\0\0x0A\1\0x29\0x11\1";
   BYTE    voc_paused           = 0; /* Flag Pause Voice          */
   BYTE    vocdriver_installed  = 0; /* Flag Driver installÇ    */
   WORD    vocdriver_version    = 0; /* NumÇro de version du driver */
   BYTE    vocfile_headerlength = 0; /* Longueur de l'entàte du format CT   */
   VOCPTR  voc_ptr_to_driver    = 0; /* Pointeur sur le Driver  */

/*
 **************************************
 * Prototype pour les fonctions crÇes *
 **************************************
 */

   void      _print_voc_errmessage(void);
   VOCPTR    _voc_get_buffer(char *voicefile);
   WORD      _voc_getversion(void);
   void      _voc_setport(WORD portnumber);
   void      _voc_setirq(WORD irqnumber);
   WORD      _voc_init_driver(void);
   void      _voc_deinstall_driver(void);
   void      _voc_set_speaker(BYTE OnOff);
   void      _voc_output(char far *bufferaddress);
   void      _voc_output_loop(char far *bufferaddress);
   void      _voc_stop(void);
   void      _voc_pause(void);
   void      _voc_continue(void);
   void      _voc_breakloop(WORD breakmode);

int _dos_open (char *filename, int mode, int *filehandle)
{
   if ((*filehandle = open (filename, mode)) == -1)
	return -1;
   return 0;
}

_dos_read (int fh, char huge *buffer, unsigned int n, unsigned int *nread)
{
union REGS reg;
struct SREGS sreg;
	reg.h.ah = 0x3F;
	reg.x.bx = fh;
	reg.x.cx = n;
	reg.x.dx = FP_OFF(buffer);
	sreg.ds = FP_SEG(buffer);
	intdosx (&reg, &reg, &sreg);
	if (reg.x.flags & 1)
	{
		*nread = 0;
		return reg.x.ax;
	}
	*nread = reg.x.ax;
	return 0;
}

#define _dos_close close

/* #define _dos_allocmem allocmem */

int _dos_allocmem (unsigned size, unsigned *segment)
{
int status;
	status = allocmem (size, segment);
	if (status == -1) return 0;
	return -1;
}

#define _dos_freemem freemem

void _print_voc_errmessage(void)
/*
 * ENTRêE  : aucune
 * SORTIE  : aucune
 * FONCTION : Affiche Ö l'Çcran, sous forme texte, l'erreur SB venant de se
 *            produire sans modifier la valeur du status.
 */
{
   switch (voc_err_stat) {
     case 100 : printf(" Driver CT-VOICE.DRV non trouvÇ ");break;
     case 110 : printf(" Place mÇmoire insuffisante pour le driver ");break;
     case 120 : printf(" Driver incorrect ");break;

     case 200 : printf(" Fichier VOC non trouvÇ ");break;
     case 210 : printf(" Espace mÇmoire insuffisant pour le fichier VOC ");break;
     case 220 : printf(" Le fichier n'est pas au format VOC ");break;

     case 300 : printf(" Erreur d'implÇmentation en mÇmoire ");break;

     case 400 : printf(" Carte Sound Blaster non trouvÇe ");break;
     case 410 : printf(" Adresse de port incorrecte ");break;
     case 420 : printf(" Utilisation d'une interruption erronÇe ");break;

     case 500 : printf(" Pas de boucle en cours ");break;
     case 510 : printf(" Aucun Çchantillon en cours de diffusion ");break;
     case 520 : printf(" Pas d'Çchantillon interrompu ");break;
      }
}


VOCPTR _voc_get_buffer(char *voicefile)
/*

 * ENTRêE  : Nom de fichier sous forme chaåne de caractäre
 * SORTIE  : Pointeur sur le buffer de donnÇes VOC
 * FONCTION : Charge un fichier en mÇmoire, et en cas de chargement
              correct, retourne la valeur du pointeur sur les donnÇes
 */
{
       int        filehandle;
       long       filesize;
       WORD       blocksize;
       WORD       byte_read ;
       char huge *filepointer;
       VOCPTR     bufferaddress;
       BYTE       check;
       WORD       segment;


/* Le fichier n'a pas ÇtÇ trouvÇ */
   if (_dos_open(voicefile,O_RDONLY
#ifndef OLD
				    | O_BINARY
#endif
						,&filehandle) != 0)
      {
       voc_err_stat = 200;
       return(FALSE);
      }

   filesize = filelength(filehandle);
   blocksize = (filesize + 15L) /16;

/* Espace mÇmoire insuffisant pour le fichier VOC */
   check = _dos_allocmem(blocksize,&segment);
   if (check != 0)
      {
       voc_err_stat = 210;
       return(FALSE);
      }

   FP_SEG(bufferaddress) = segment;
   FP_OFF(bufferaddress) = 0;

   filepointer = (char huge*)bufferaddress;

/* Chargement du fichier Çchantillon */
   do
      {
       _dos_read(filehandle,filepointer,0x8000,&byte_read);
       filepointer += byte_read ;
      } while (byte_read == 0x8000);

#ifdef OLD
   _dos_close(filehandle);
#else
   close(filehandle);
#endif

/* Le Fichier n'est pas au format VOC */
   if ((bufferaddress[0] != 'C') || (bufferaddress[1] != 'r'))
      {
       voc_err_stat = 220;
       return(FALSE);
      }


/* Le chargement a ÇtÇ correctement effectuÇ */
   voc_err_stat = 0;

   vocfile_headerlength = (BYTE)bufferaddress[20];

   return(bufferaddress);
}

WORD _voc_getversion(void)
/*
 * ENTRêE  : aucune
 * SORTIE  : NumÇro de version du driver
 * FONCTION : Transmet le numÇro de version du driver
 */
{
   WORD    vdummy;
#ifdef ASM
   asm {
      mov       bx,0
      call      voc_ptr_to_driver
      mov       vdummy, ax
       }
#else
	_BX = 0;
	cal (voc_ptr_to_driver);
	vdummy = _AX;
#endif
   return(vdummy);
}

void _voc_setport(WORD portnumber)
/*
 * ENTRêE  : NumÇro de l'adresse du port
 * SORTIE  : aucune
 * FONCTION : Fixe l'adresse du port avant l'initialisation
 */
{
#ifdef ASM
   asm {
      mov    bx,1
      mov    ax,portnumber
      call   voc_ptr_to_driver
       }
#else
	_BX = 1;
	_AX = portnumber;
	cal (voc_ptr_to_driver);
#endif
}

void _voc_setirq(WORD irqnumber)
/*
 * ENTRêE  : NumÇro d'interruption
 * SORTIE  : aucune
 * FONCTION : Fixe le numÇro d'interruption avant l'initialisation
 */
{
#ifdef ASM
   asm {
      mov    bx,2
      mov    ax,irqnumber
      call   voc_ptr_to_driver
       }
#else
	_BX = 2;
	_AX = irqnumber;
	cal (voc_ptr_to_driver);
#endif
}

WORD _voc_init_driver(void)
/*
 * ENTRêE  : aucune
 * SORTIE  : NumÇro de message d'erreur suivant le rÇsultat de
              l'initialisation
 * FONCTION : Initialisation du logiciel Driver
 */

{
       int              filehandle;
       unsigned long    filesize;
       WORD             blocksize;
       char huge       *filepointer;
       WORD		byte_read;
       BYTE             check;
       WORD             segment;
       WORD	        vseg;
       WORD             vofs;
       WORD		vout;


   vocdriver_installed = FALSE;

/* Driver non trouvÇ */
   if (_dos_open("CT-VOICE.DRV",O_RDONLY,&filehandle) != 0)
      {
       voc_err_stat = 100;
       return(FALSE);
      }
   filesize = filelength(filehandle);
   blocksize = (filesize + 15L) /16;

/* Espace mÇmoire insuffisant pour le driver */
   check = _dos_allocmem(blocksize,&segment);
   if (check != 0)
      {
       voc_err_stat = 110;
       return(FALSE);
      }

   FP_SEG(voc_ptr_to_driver) = segment;
   FP_OFF(voc_ptr_to_driver) = 0;


   filepointer = (char huge*)voc_ptr_to_driver;

/* Chargement du driver */
   do
     {
      _dos_read(filehandle,filepointer,0x8000,&byte_read);
      filepointer += byte_read ;
     } while (byte_read == 0x8000) ;

   _dos_close(filehandle) ;


/* Driver incorrect, ne commence pas par "CT" */
   if ((voc_ptr_to_driver[3] != 'C') || (voc_ptr_to_driver[4] != 'T'))
      {
       voc_err_stat = 120;
       return(FALSE);
      }

/* Transmettre la version du driver */
   vocdriver_version = _voc_getversion();

/* DÇmarrage du Driver */
   vseg = FP_SEG(&voc_status_word);
   vofs = FP_OFF(&voc_status_word);

#ifdef ASM
   asm {
      mov       bx,3
      call      voc_ptr_to_driver
      mov       vout,ax
      mov       bx,5
      mov       es,vseg
      mov       di,vofs
      call      voc_ptr_to_driver
       }
#else
	_BX = 3;
	cal (voc_ptr_to_driver);
	vout = _AX;
	_BX = 5;
	_ES = vseg;
	_DI = vofs;
	cal (voc_ptr_to_driver);
#endif

/*  Carte Sound-Blaster non trouvÇe */
   if (vout == 1)
      {
       voc_err_stat = 400;
       return(FALSE);
      }

/* Utilisation d'une adresse de port incorrecte */
   if (vout == 2)
      {
       voc_err_stat = 410;
       return(FALSE);
      }

/* Utilisation d'un numÇro d'interruption incorrect */
   if (vout == 3)
      {
       voc_err_stat = 420;
       return(FALSE);
      }

   vocdriver_installed = TRUE;
   return(TRUE);
}


void _voc_deinstall_driver(void)
/*
 * ENTRêE  : aucune
 * SORTIE  : aucune
 * FONCTION : DÇsactivation du driver et libÇration de la mÇmoire
 */

{
   if (vocdriver_installed)
      {
#ifdef ASM
       asm {
          mov   bx,9
          call  voc_ptr_to_driver
	   }
#else
	_BX = 9;
	cal (voc_ptr_to_driver);
#endif
       _dos_freemem(FP_SEG(voc_ptr_to_driver));
      }
}


void _voc_set_speaker(BYTE on_off)
/*
 * ENTRêE  : TRUE pour l'activation du haut-parleur et FALSE pour la
 *           dÇsactivation
 * SORTIE  : aucune
 * FONCTION : Activation/DÇsactivation du haut-parleur de la carte SB
 */
{
#ifdef ASM
   asm {
      mov       bx,4
      mov       al,on_off
      call      voc_ptr_to_driver
       }
#else
	_BX = 4;
	_AL = on_off;
	cal (voc_ptr_to_driver);
#endif
}

void _voc_output(VOCPTR bufferaddress)
/*
 * ENTRêE  : Pointeur sur les donnÇes ÇchantillonnÇes
 * SORTIE  : Aucune
 * FONCTION  Diffusion d'un Çchantillon
 */
{
   WORD  vseg;
   WORD  vofs;

   _voc_set_speaker(TRUE);

   vseg = FP_SEG(bufferaddress);
   vofs = FP_OFF(bufferaddress) + vocfile_headerlength;
#ifdef ASM
   asm {
      mov       bx,6
      mov       es,vseg
      mov       di,vofs
      call      voc_ptr_to_driver
       }
#else
	_BX = 6;
	_ES = vseg;
	_DI = vofs;
	cal (voc_ptr_to_driver);
#endif
}


void _voc_output_loop(VOCPTR bufferaddress)
/*
 *    DiffÇrence avec _voc_output :
 *    Dans ce cas, le haut-parleur ne sera pas activÇ avant chaque dif-
 *    fusion d'un Çchantillon. Cela empàche le bruit  rÇsultant sur la
 *    plupart des cartes Sound Blaster lors de la diffusion d'une boucle.
 */

{
   WORD  vseg;
   WORD  vofs;

   vseg = FP_SEG(bufferaddress);
   vofs = FP_OFF(bufferaddress) + vocfile_headerlength;

#ifdef ASM
   asm {
      mov       bx,6
      mov       es,vseg
      mov       di,vofs
      call      voc_ptr_to_driver
       }
#else
	trace ("calling driver %p for output...", voc_ptr_to_driver);
	trace ("%p", vseg);
	trace ("%p", vofs);
	/* sleep (1); */
	/* { long i; for (i=0; i<100000; i++); } */
	while (voc_status_word);
	_BX = 6;
	_ES = vseg;
	_DI = vofs;
	cal (voc_ptr_to_driver);
	trace ("driver called", 0);
#endif
}

void _voc_stop(void)
/*
 * ENTRêE  : aucune
 * SORTIE  : aucune
 * FONCTION : Stoppe un Çchantillon
 */
{
#ifdef ASM
   asm {
      mov       bx,8
      call      voc_ptr_to_driver
       }
#else
	_BX = 8;
	cal (voc_ptr_to_driver);
#endif
}

void _voc_pause(void)
/*
 * ENTRêE  : aucune
 * SORTIE  : aucune
 * FONCTION : Interrompt un Çchantillon
 */
{
   WORD  pcheck;

   voc_paused = TRUE;

#ifdef ASM
   asm {
      mov       bx,10
      call      voc_ptr_to_driver
      mov       pcheck,ax
       }
#else
	_BX = 10;
	cal (voc_ptr_to_driver);
	pcheck = _AX;
#endif
   if (pcheck == 1)
      {
       voc_paused = FALSE;
       voc_err_stat = 510;
      }
}

void _voc_continue(void)
/*
 * ENTRêE  : aucune
 * SORTIE  : aucune
 * FONCTION : Reprend un Çchantillon interrompu
 */
{
   WORD  pcheck;

#ifdef ASM
   asm {
      mov       bx,11
      call      voc_ptr_to_driver
      mov       pcheck,ax
       }
#else
	_BX = 11;
	cal (voc_ptr_to_driver);
	pcheck = _AX;
#endif
   if (pcheck == 1)
      {
       voc_paused = FALSE;
       voc_err_stat = 520;
      }
}

void _voc_breakloop(WORD breakmode)
/*
 * ENTRêE  : Mode d'interruption
 * SORTIE  : aucune
 * FONCTION : Stoppe une boucle dans un Çchantillon
 */
{
#ifdef ASM
   asm {
      mov       bx,12
      mov       ax,breakmode
      call      voc_ptr_to_driver
      mov       breakmode,ax
       }
#else
	_BX = 12;
	_AX = breakmode;
	cal (voc_ptr_to_driver);
	breakmode = _AX;
#endif
   if (breakmode == 1) voc_err_stat = 500;
}



/*
 ****************************************************************************
 *  Programme de dÇmonstration pour l'UnitÇ VOCTOOL, (W) in Borland C++ 3.0 *    *
 ****************************************************************************
 *                     (C) 1992 MICRO APPLICATION                           *
 *                         Auteur : Axel Stolz                              *
 ****************************************************************************
 */
/*
#include <conio.h>
#include "voctool.h"
*/
#include <math.h>
void erreur()
/*
 * ENTRêE   : aucune, les donnÇes proviennent de la variable globale
            : voc_err_stat
 * SORTIE   : aucune
 * FONCTION : Affiche le type d'erreur venant de se produire avec le
 *            numÇro d'erreur. Termine le programme avec le niveau
 *            d'erreur correspondant au numÇro.
 */

{
   printf(" Erreur # %d, ",voc_err_stat);
   _print_voc_errmessage();
   exit(voc_err_stat);
}

char *create_voc (float periode, int a, int duree, int np, int flag_harmon,
	int flag_create);
void dump (char *buffer, int n);

#define TAILLE_TABLE_SIN 1000

float table_sin [TAILLE_TABLE_SIN+1];

#define PI 3.1415926

init_table ()
{
int i;
	for (i=0; i<=TAILLE_TABLE_SIN; i++)
		table_sin[i] = sin (i * 2.0 * PI / TAILLE_TABLE_SIN);
}

float mysin (float x)
{
float y;
int i;
	i = x * TAILLE_TABLE_SIN / (2.0 * PI);
	i = i % TAILLE_TABLE_SIN;
	if (i < 0)
		i += TAILLE_TABLE_SIN;
	y = table_sin [i];
	return y;
}

void main (int argc, char *argv[])
{
  VOCPTR voc_buffer = 0;
  char   ch;
  float periode;
  int note;

  init_table ();

  clrscr();

  if (_voc_init_driver() == FALSE) erreur();
loop:
  gotoxy(1,1); printf("\n");
  if (!init_son ()) goto end;
/* Charge le fichier DEMO.VOC dans la mÇmoire */
/*  voc_buffer = _voc_get_buffer("demo.voc"); */
  voc_buffer = create_voc ((float)atoi(argv[1]), atoi(argv[2]),
	atoi(argv[3]), atoi(argv[4]), atoi(argv[5]), 1);
loop1:
  trace ("loop1", 0);
  vocfile_headerlength = 0;
  trace ("%p", voc_buffer);
/* Le fichier VOC n'a pas pu àtre chargÇ */
  if (voc_buffer == NULL) erreur();
/*  dump (voc_buffer, 120); */

/* Boucle principale du programme */
/*
  printf("CT-Voice-Driver Version : ");
  printf("%d.%02d\n",vocdriver_version >> 8, vocdriver_version % 256);
  printf("Voulez-vous Çcouter le son (u)ne ou (p)lusieurs fois ?\n");
  printf("EntrÇe : ");
  ch = getche();
  printf("\n\n");
*/
  ch = 'p';
  switch (ch) {
     case 'u': printf("Frapper une touche pour interrompre");
               _voc_output(voc_buffer);
               do {} while ((voc_status_word != 0) && (kbhit() == 0));
               if (kbhit() != 0) _voc_stop();
               break;
     case 'p': /* printf(" Appuyer sur<ESC> pour quitter");
	       ch = ' ';*/
	       ch = 0;
	       do {
		   trace ("output %p", voc_buffer);
		   _voc_output_loop(voc_buffer);
		   trace ("done", 0);
		   do {
			trace ("%d", voc_status_word);
		   } while ((voc_status_word != 0) && (kbhit() == 0));
		   if (kbhit() != 0)
		   {
			ch = getch();
			trace ("0x%02X", ch);
		   }
		  } /*while (ch != (char)27);*/ while (!ch);
	       trace ("stop...", 0);
	       _voc_stop();
	       trace ("", 0);
               break;
     }
  trace ("0x%02X", ch);
  if (ch == '\n' || ch == '\r')
	goto loop;
  note = clavier (ch);
  trace ("%d", note);
  periode = (float) atoi(argv[1]) / pow (2.0, (float) note / 12.0);
  printf (" Touche '%c' 0x%02X, note %d, periode %f \n",
	ch, ch, note, periode);
  voc_buffer = create_voc (periode, atoi(argv[2]), atoi(argv[3]),
				atoi(argv[4]), atoi(argv[5]), 0);
  {
  int i;
	/* for (i=0; i<120; i++)
		printf ("%02X ", (unsigned char)(voc_buffer[i])); */
	printf ("\n");
  }
  goto loop1;

/* LibÇrer la mÇmoire occupÇe par le fichier VOC  */
end:
  _dos_freemem(FP_SEG(voc_buffer));

  _voc_deinstall_driver();
}


/*#include <graphics.h>*/
/*
#include <stdio.h>
#include <stdlib.h>
*/
#include <io.h>
/*
#include <fcntl.h>
#include <dos.h>
*/

#define FP_SEG(fp) (*(int *)(((char *)&(fp))+2))
#define FP_OFS(fp) ((int)(fp))

#define call(adr) (*(void (far *)())(adr))()

#define PI 3.1415926

#define f sound_fn

#define fonction(fx) \
	float f (float x) {float y; y=0.0; {fx;} return y; } \
	char def_f[] = #fx;

/* fonction (y = sin(x)) */

/*
fonction (int n;
	for (n=1; n<=18; n++)
		y += sin (n * x) / n;)
*/

float phi[50];
float ampl[50];
int nh;

/*
fonction (int n;
	for (n=1; n<=20; n++)
		y += sin (n * x + phi[n]) / n;)
*/


#if 1
fonction (int n;
	for (n=1; n<=nh; n++)
		y += sin (n * x) * ampl[n] /* / n */ ;)
#else
fonction (y = ampl[x])
#endif

/*
float f (float x)
{
	return sin(x);
}
*/

#define SIZE 0x4000
char buffer[SIZE];
int ptr;

void dump (char *buffer, int length)
{
int i;
	for (i=0; i<length; i++)
	{
		if ((i & 0xF) == 0)
		{
			printf ("\n %3X: ", i);
		}
		printf ("%02X ", buffer[i]&0xFF);
	}
	printf ("\n");
}

init_son ()
{
int i;
char buf[120];

	i = 1;
saisie:
	printf ("\tHarmonique %d ? ", i);
	gets (buf);
	if (*buf != 't')
	{
		if (*buf) sscanf (buf, "%f", &(ampl[i]));
		i++;
		goto saisie;
	}
	nh = i-1;
	return (nh>0);
}

main1 (int argc, char *argv[])
{
int status;
int t, i;
float x, y;
int driver, mode;
int len;
FILE *fvoc;
char buf[120];
	/* initgraph (HERCMONO, HERCMONOHI, ""); */
	/* initgraph (EGA, EGA64LO, ""); */
#if 0
	driver = EGA;
	mode = EGA64LO;
	driver = HERCMONO;
	mode = HERCMONOHI;
	initgraph (&driver, &mode, "");
	status = graphresult();
	if (status != grOk)
		printf ("Erreur initgraph %d 0x%X\n", status, status);

	for (i=1; i<50; i++)
	{
		phi[i] = /*0;*/ 2 * PI * (float)random(10000) / 10000.0;
	}

	for (t=0; t<1000; t++)
	{
	/*	x = sin(t);
		y = sin(t*1.5);
		putpixel (100+60*x, 80+40*y, 1);
	*/
		x = t * 2 * PI / 1000;
		y = f(x);
		putpixel (10+100*x, 180-100*y, 1);
	}
	getchar();
#endif

	i = 1;
saisie:
	printf ("\tHarmonique %d ? ");
	gets (buf);
	if (*buf)
	{
		sscanf (buf, "%f", &(ampl[i]));
		i++;
		goto saisie;
	}
	nh = i;

	printf ("Adresse: %p:%p\n", _DS, (char far *)buffer);
	len = vcreat (argc, argv, buffer);
	dump (buffer+0x1A, 250);
	vtest (buffer+0x1A);
	fvoc = fopen (argv[1], "wb");
	if (fvoc == NULL)
	{
		perror(argv[1]);
		return (errno);
	}
	fwrite (buffer, 1, len, fvoc);
	fclose (fvoc);
	/*restorecrtmode();*/
}
/*
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
*/

#define f voc_file
FILE *f;

flush ()
{
	fwrite (buffer, 1, ptr, f);
	ptr = 0;
}

put (char c)
{
	if (ptr >= SIZE) flush();
	buffer[ptr++] = c;
}

int vcreat (int argc, char *argv[], char *buffer)
{
char c;
int n, i;
int t, duree, periode;
float a, puls;
char *p;
char buf[120];

	ptr = 0;
	p = buffer;
	a = atoi(argv[2]);
	periode = atoi(argv[3]);
	duree = atoi(argv[4]);

	i = 1;
saisie:
	printf ("\tHarmonique %d ? ");
	gets (buf);
	if (*buf)
	{
		sscanf (buf, "%f", &(ampl[i]));
		i++;
		goto saisie;
	}
	nh = i;

	/*
	f = fopen (argv[1], "wb");
	if (f == NULL)
	{
		perror(argv[1]);
		return (errno);
	}
	*/
	p += sprintf (p, "Creative Voice File%c%c%c%c%c%c%c",
			0x1A, 0x1A, 0, 0x0A, 0x01, 0x29, 0x11);

	p += sprintf (p, "%c%c%c%c%c%c", 6, 2, 0, 0,
		duree&0xFF, duree>>8);

	p += sprintf (p, "%c%c%c%c%c%c", 0x01,
			(periode+2)&0xFF, (periode+2)>>8, 0, 0x83, 0);

	for (t=0; t<periode; t++)
	{
		c = 0x80 + (int)(a * sound_fn (2.0 * PI * (float)t / periode));
		/*putc (c, f);*/
		/* put (c); */
		*(p++) = c;
		if ((t & 0xF) == 0) printf("\r%5X", t);
	}

	/* flush(); */
	p += sprintf (p, "%c%c%c%c", 7, 0, 0, 0);
	/* putc (0, f); */
	*(p++) = 0;
	/* fclose (f); */
	return (p - buffer);
}

#include <graphics.h>

#define LARGEUR 600
#define HAUTEUR 200

char *create_voc (float periode_float, int a, int duree, int np, int flag_harmon,
	int flag_create)
{
char c;
int n, i;
int t;
char *p;
unsigned char buf_son[4000];
int pn;
int driver, mode;
int status;
char buf[200];
static int flag_init = 1;
int periode;

	trace ("create_voc", 0);

	periode = (int) (periode_float + 0.5);

	trace ("", 0);

    if (flag_init == 0)
	flag_create = 0;
    flag_init = 0;

	trace ("", 0);

    if (flag_create)
    {
	trace ("", 0);
	driver = HERCMONO;
	mode = HERCMONOHI;
	initgraph (&driver, &mode, "C:\D1");
	status = graphresult ();
	if (status != grOk)
	{
		printf ("Erreur initgraph %d 0x%X\n", status, status);
		exit (status);
	}
    }
	trace ("", 0);
	ptr = 0;
	p = buffer;


	if (duree != -1)
	p += sprintf (p, "%c%c%c%c%c%c", 6, 2, 0, 0,
		duree&0xFF, duree>>8);

	trace ("%p", p);
	p += sprintf (p, "%c%c%c%c%c%c", 0x01,
			(int)(periode*np+2)&0xFF,
			(int)(periode*np+2)>>8, 0, 0x83, 0);


	trace ("%d", periode);
	for (t=0; t<periode; t++)
	{
		/* printf ("t = %d\n", t); */
		trace ("%d", t);
	    if (flag_harmon)
		buf_son[t] = 0x80 + (int)(a * sound_fn (2.0 * PI *
							(float)t / periode));
	    else
		buf_son[t] = ampl[t-1];
	    if (flag_create)
	    {
		putpixel ((long)t * (long)LARGEUR / periode,
		 HAUTEUR - ((long)(buf_son[t]) * (long)HAUTEUR / 0x100),
			1);
	    }
	}

#define PI 3.1415926
	if (flag_create)
	{
	int h;
	float s;
	int t;
		gets (buf);
		restorecrtmode ();

		printf ("\n");
		for (h=1; h<20; h++)
		{
			s = 0;
			for (t=0; t<periode; t++)
			    s += (float)(buf_son[t] - 0x80) * sin (h *
				2.0 * PI * (float)t / periode);
			s = s * 2.0 / (float)periode;
			printf ("\tHarmonique %d : %f\n", h, s);


		}

	}

/*
	for (t=0; t<periode*np; t++)
	{
	    c = 0x80 + (int)(a * sound_fn (2.0 * PI * (float)t / periode));
	    *(p++) = c;
	}
*/
	for (pn=0; pn<np; pn++)
	{
		/* printf ("pn = %d\n", pn); */
		for (t=0; t<periode; t++)
			*(p++) = buf_son[t];
	}

	if (duree != -1)
	{
/*
	    p += sprintf (p, "%c%c%c%c%c%c", 6, 2, 0, 0,
		duree&0xFF, duree>>8);

	    p += sprintf (p, "%c%c%c%c", 0x02,
			(periode)&0xFF, (periode)>>8, 0);
	    for (t=0; t<periode; t++)
	    {
		c = 0x80 + (int)(a * sound_fn (2.0 * PI * (float)t / periode));
		*(p++) = c;
	    }
*/
	    p += sprintf (p, "%c%c%c%c", 7, 0, 0, 0);
	}

	*(p++) = 0;



	return buffer;
}

/*
	if (t < duree) goto loop;
	c = getc(f);
	if (feof(f))
	{
		fclose(f);
		return 0;
	}
	n = 40 + ((unsigned)c & 0xFF) - 0x80;
	if (n<0) n = 0;
	if (n > 76) n = 76;
	for (i=0; i<n; i++) putchar(' ');
	printf("%X\n",c);
	goto loop;
*/

#define VOC_DRV_FILE "C:\\SB\\DRV\\CT-VOICE.DRV"

char far *voc_drv = 0;
unsigned int voc_status;

union REGS reg;
struct SREGS sreg;

int filesize (char *filename)
{
FILE *f;
	f = fopen(filename,"rb");
	if (f == NULL)
	{
		perror(filename);
		return -1;
	}
	if (fseek (f, 0, SEEK_END))
	{
		perror(filename);
		return -1;
	}
	return ftell(f);
}

vtest (char *buffer)
{
int status, segment, fh, vseg, vofs;
unsigned int size, n;
/*	size = filesize(VOC_DRV_FILE);*/
	fh = open (VOC_DRV_FILE, O_RDONLY|O_BINARY);
	if (fh == -1)
	{
		perror(VOC_DRV_FILE);
		return errno;
	}
	size = filelength(fh);
	status = allocmem ((size+15L)/16, &segment);
	if (status != -1)
	{
		fprintf (stderr, "\nMemoire insuffisante.\n");
		return status;
	}
	FP_SEG(voc_drv) = segment;
	FP_OFS(voc_drv) = 0;
	n = 0;
	while (n < size)
	{
		reg.h.ah = 0x3F;
		reg.x.bx = fh;
		reg.x.cx = size;
		reg.x.dx = FP_OFS(voc_drv);
		sreg.ds = FP_SEG(voc_drv);
		intdosx (&reg, &reg, &sreg);
		if (reg.x.flags & 1)
		{
			errno = reg.x.ax;
			perror("READ");
			return errno;
		}
		n += reg.x.ax;
	}
	if (voc_drv[3] != 'C')
	{
		fprintf (stderr, "\nDriver incorrect.\n");
		return -1;
	}
	close(fh);
/*
	asm mov bx, 3;
	asm call voc_drv;
	asm mov status, ax;
*/
	_BX = 3;
	call (voc_drv);
	status = _AX;

	if (status)
	{
		fprintf (stderr, "\nErreur initialisation %d.\n", status);
		return status;
	}

	_BX = 5;
	_ES = FP_SEG (voc_status);
	_DI = FP_OFS (voc_status);
	call (voc_drv);

/* Haut-parleur en marche */
/*
	asm mov bx, 4;
	asm mov al, 1;
	asm call voc_drv;
*/
	_BX = 4;
	_AL = 1;
	call (voc_drv);

/* Jouer la musique */
	vseg = FP_SEG(buffer);
	vofs = FP_OFS(buffer);
/*
	asm mov bx, 6;
	asm mov es, vseg;
	asm mov di, vofs;
	asm call voc_drv;
*/
/*
	_BX = 6;
	_ES = vseg;
	_DI = vofs;
	call (voc_drv);
	while (voc_status && !kbhit());
*/
	while (!kbhit())
	{
		_BX = 6;
		_ES = FP_SEG(buffer);
		_DI = FP_OFS(buffer);
		call (voc_drv);
		while (voc_status && !kbhit());
	}
}


