/* Windows API source generated from resource file c:\sb\d1\vtest\vl1.ctl */

#include <owl.h>
#include <window.h>
#include <string.h>
#include <ctype.h>

#include "mmsystem.h"

HANDLE hInst;
HWND hWndVL;

HWND hWndGbl;

#define NCTL 60
HWND hCtlObjVL[NCTL];

int FocusVL;

static char szAppNameVL[] = "AppliSyntWav";
static char WndCaptionVL[] = "SyntWav";

#define COMMAND0 100

#define hCtlIdVL(id) (hCtlObjVL[(id)-COMMAND0])

#define WidVL 345 /*(250)*/
#define HeiVL 195 /*(149)*/

#define P_ID 0
#define P_X 1
#define P_Y 2
#define P_W 3
#define P_H 4
#define P_STYLE 5

struct resource_item
{
	char name[15];
	char ctlclass[15];
	char caption[80];
	long param_int[8];
	int id, X, Y, W, H, style;
	};

struct resource_item ResVL[NCTL];

InitResVL ()
{
int i;
	/*
	EDITTEXT ID_AMPL, 194, 5, 35, 136, ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP
	EDITTEXT ("EDIT") "" ID=ID_AMPL X= 194 Y= 5 W= 35 H= 136 STYLE= ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL
	*/
	strcpy (ResVL[1].name, "EDITTEXT");
	strcpy (ResVL[1].ctlclass, "EDIT");
	strcpy (ResVL[1].caption, "");
	ResVL[1].param_int[1] =  205;
	ResVL[1].param_int[2] =  20; /*5;*/
	ResVL[1].param_int[3] =  30; /*35;*/
	ResVL[1].param_int[4] =  150; /*136;*/
	ResVL[1].param_int[5] =  ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL
		| ES_MULTILINE | ES_WANTRETURN;

	/*
	LTEXT "", -1, 170, 9, 16, 8, WS_CHILD | WS_VISIBLE | WS_GROUP
	LTEXT ("STATIC") "" ID= -1 X= 170 Y= 9 W= 16 H= 8 STYLE= WS_CHILD | WS_VISIBLE | WS_GROUP
	*/
	strcpy (ResVL[2].name, "LTEXT");
	strcpy (ResVL[2].ctlclass, "STATIC");
	strcpy (ResVL[2].caption, "Harmoniques");
	ResVL[2].param_int[1] =  150;
	ResVL[2].param_int[2] =  9;
	ResVL[2].param_int[3] =  45;
	ResVL[2].param_int[4] =  8;
	ResVL[2].param_int[5] =  WS_CHILD | WS_VISIBLE | WS_GROUP;

	/*
	PUSHBUTTON "Draw", ID_DRAW, 11, 129, 24, 14, WS_CHILD | WS_VISIBLE | WS_TABSTOP
	PUSHBUTTON ("BUTTON") "Draw" ID= ID_DRAW X= 11 Y= 129 W= 24 H= 14 STYLE= WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON
	*/
	strcpy (ResVL[3].name, "PUSHBUTTON");
	strcpy (ResVL[3].ctlclass, "BUTTON");
/*
	strcpy (ResVL[3].caption, "GÈnÈrer");
	ResVL[3].param_int[1] =  11;
	ResVL[3].param_int[2] =  129;
	ResVL[3].param_int[3] =  35;
	ResVL[3].param_int[4] =  14;
*/
	strcpy (ResVL[3].caption, "SynthÈtiser");
	ResVL[3].param_int[1] =  205;
	ResVL[3].param_int[2] =  175;
	ResVL[3].param_int[3] =  65;
	ResVL[3].param_int[4] =  14;
	ResVL[3].param_int[5] =  WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON;

	strcpy (ResVL[4].name, "");
	strcpy (ResVL[4].ctlclass, "BUTTON");
	strcpy (ResVL[4].caption, "Son compilÈ");
	ResVL[4].param_int[1] = 53;
	ResVL[4].param_int[2] = 160 /*130*/;
	ResVL[4].param_int[3] = 60;
	ResVL[4].param_int[4] = 12;
	ResVL[4].param_int[5] = WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTOCHECKBOX;

	strcpy (ResVL[5].name, "");
	strcpy (ResVL[5].ctlclass, "BUTTON");
	strcpy (ResVL[5].caption, "Graphe cosonique");
	ResVL[5].param_int[1] = 115;
	ResVL[5].param_int[2] = 175 /*130*/;
	ResVL[5].param_int[3] = 70;
	ResVL[5].param_int[4] = 12;
	ResVL[5].param_int[5] = WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTOCHECKBOX;

	strcpy (ResVL[6].name, "");
	strcpy (ResVL[6].ctlclass, "BUTTON");
	strcpy (ResVL[6].caption, "Graphe chronologique");
	ResVL[6].param_int[1] = 115;
	ResVL[6].param_int[2] = 160 /*112*/;
	ResVL[6].param_int[3] = 85;
	ResVL[6].param_int[4] = 12;
	ResVL[6].param_int[5] = WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTOCHECKBOX;

    i=7;
	strcpy (ResVL[i].name, "EDITTEXT");
	strcpy (ResVL[i].ctlclass, "EDIT");
	strcpy (ResVL[i].caption, "");
	ResVL[i].param_int[1] =  240;
	ResVL[i].param_int[2] =  20;
	ResVL[i].param_int[3] =  30;
	ResVL[i].param_int[4] =  150;
	ResVL[i].param_int[5] =  ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL
		| ES_MULTILINE | ES_WANTRETURN;

#define ID_AMPL_SIN (COMMAND0+8)
	i=8;
	strcpy (ResVL[i].name, "EDITTEXT");
	strcpy (ResVL[i].ctlclass, "EDIT");
	strcpy (ResVL[i].caption, "");
	ResVL[i].param_int[1] =  275;
	ResVL[i].param_int[2] =  20;
	ResVL[i].param_int[3] =  30;
	ResVL[i].param_int[4] =  150;
	ResVL[i].param_int[5] =  ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL
		| ES_MULTILINE | ES_WANTRETURN;

#define ID_AMPL_COS (COMMAND0+9)
	i=9;
	strcpy (ResVL[i].name, "EDITTEXT");
	strcpy (ResVL[i].ctlclass, "EDIT");
	strcpy (ResVL[i].caption, "");
	ResVL[i].param_int[1] =  310;
	ResVL[i].param_int[2] =  20;
	ResVL[i].param_int[3] =  30;
	ResVL[i].param_int[4] =  150;
	ResVL[i].param_int[5] =  ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL
		| ES_MULTILINE | ES_WANTRETURN;

    i = 10;
	strcpy (ResVL[i].name, "LTEXT");
	strcpy (ResVL[i].ctlclass, "STATIC");
	strcpy (ResVL[i].caption, "Amplitude");
	ResVL[i].param_int[1] =  205;
	ResVL[i].param_int[2] =  5;
	ResVL[i].param_int[3] =  34;
	ResVL[i].param_int[4] =  8;
	ResVL[i].param_int[5] =  WS_CHILD | WS_VISIBLE | WS_GROUP;

	i = 11;
	strcpy (ResVL[i].name, "LTEXT");
	strcpy (ResVL[i].ctlclass, "STATIC");
	strcpy (ResVL[i].caption, "Phase");
	ResVL[i].param_int[1] =  240;
	ResVL[i].param_int[2] =  5;
	ResVL[i].param_int[3] =  34;
	ResVL[i].param_int[4] =  8;
	ResVL[i].param_int[5] =  WS_CHILD | WS_VISIBLE | WS_GROUP;

	i = 12;
	strcpy (ResVL[i].name, "LTEXT");
	strcpy (ResVL[i].ctlclass, "STATIC");
	strcpy (ResVL[i].caption, "Sinus");
	ResVL[i].param_int[1] =  275;
	ResVL[i].param_int[2] =  5;
	ResVL[i].param_int[3] =  34;
	ResVL[i].param_int[4] =  8;
	ResVL[i].param_int[5] =  WS_CHILD | WS_VISIBLE | WS_GROUP;

	i = 13;
	strcpy (ResVL[i].name, "LTEXT");
	strcpy (ResVL[i].ctlclass, "STATIC");
	strcpy (ResVL[i].caption, "Cosinus");
	ResVL[i].param_int[1] =  310;
	ResVL[i].param_int[2] =  5;
	ResVL[i].param_int[3] =  34;
	ResVL[i].param_int[4] =  8;
	ResVL[i].param_int[5] =  WS_CHILD | WS_VISIBLE | WS_GROUP;

#define ID_SYNT_SIN_COS (COMMAND0+14)
	i = 14;
	strcpy (ResVL[i].name, "PUSHBUTTON");
	strcpy (ResVL[i].ctlclass, "BUTTON");
	strcpy (ResVL[i].caption, "SynthÈtiser");
	ResVL[i].param_int[1] =  275;
	ResVL[i].param_int[2] =  175;
	ResVL[i].param_int[3] =  65;
	ResVL[i].param_int[4] =  14;
	ResVL[i].param_int[5] =  WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON;
 
#define NCTLVL 14


	return 0;
}

#define ID_AMPL              (COMMAND0 + 1)
#define  ID_DRAW             (COMMAND0 + 3)

#define ID_PHASE	(COMMAND0 + 7)

int InitWndVL (HWND);

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

#ifndef DOS
#include <alloc.h>
#endif

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

#define cal(f) /* (((void (far *)())(f))()) */

#ifdef DOS
#define FP_SEG(fp) (*(int *)(((char *)&(fp))+2))
#endif

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

#ifdef DOS
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
#else
/*#define allocmem _dos_allocmem*/
#define _dos_allocmem allocmem

int allocmem (unsigned size, unsigned *segment)
{
#if 1
int status, CF;
	_AH = 0x48;
	_BX = size;
	geninterrupt (0x21);
    *segment = _AX;
	CF = _FLAGS & 1;
	if (CF == 0)
		return 0;
    return -1;
#else
union
{
void far *p;
struct
{
	unsigned ofs;
	unsigned seg;
} s;
} u; 
	u.p = farmalloc ((size+3)*16);
	if (u.p != NULL)
	{
		/* *segment = (unsigned)p; */
        *segment = u.s.seg + u.s.ofs/16 + 1;
		return 0;
	}
    else
    	return -1;
#endif
}

#endif
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

int init_son (void);
int clavier (char);
int vcreat (int, char **, char *);
int vtest (char *);

#if 0

#define kbhit kbhit_win

int kbhit (void)
{
MSG msg;
		GetMessage(&msg, NULL, 0, 0);

		if (!KBHandler (&msg, hWndVL, NCTLVL, ResVL, hCtlObjVL, &FocusVL))
		{
			if (msg.message == WM_CHAR)
			{

			}
			else
            {
				TranslateMessage (&msg);
				DispatchMessage (&msg);
            }
		}
}

#endif

#define WAVE


#define PI 3.1415926

#define f sound_fn

#define fonction(fx) \
	float f (float x) {float y; y=0.0; {fx;} return y; } \
	char def_f[] = #fx;

#define cofonction(fx) \
	float sound_cofn (float x) {float y; y=0.0; {fx;} return y; } 

/* fonction (y = sin(x)) */

/*
fonction (int n;
	for (n=1; n<=18; n++)
		y += sin (n * x) / n;)
*/

#define MH 40

float phi[MH];
float ampl[MH];
float phase[MH];
float ampl_sin[MH], ampl_cos[MH];
int nh;

int type_synthese;

calcul_ampl_sin_cos ()
{
int i;
	for (i=1; i<=nh; i++)
	{
		ampl_sin[i] = ampl[i] * cos (phase[i]);
        ampl_cos[i] = ampl[i] * sin (phase[i]);
    }
}

calcul_ampl_phase ()
{
int i;
	for (i=1; i<=nh; i++)
	{
		ampl[i] = sqrt (ampl_sin[i]*ampl_sin[i] + ampl_cos[i]*ampl_cos[i]);
		phase[i] = asin (ampl_cos[i] / ampl[i]);
		if (ampl_sin[i] < 0)
			phase[i] = PI - phase[i];
    }
}

aff_table (float *t, int n, float fact, HWND hWnd)
{
char buf[1000];
char *p;
int i;
	p = buf;
	for (i=0; i<n; i++)
	{
    	p += sprintf (p, "%f\r\n", t[i]*fact);
	}
	*p = 0;
    SendMessage (hWnd, WM_SETTEXT, 0, (LPARAM) (LPCSTR) buf); 
}

aff_ampl_sin_cos ()
{
	aff_table (ampl_sin+1, nh, 1.0, hCtlIdVL(ID_AMPL_SIN));
	aff_table (ampl_cos+1, nh, 1.0, hCtlIdVL(ID_AMPL_COS));
}

aff_ampl_phase ()
{
	aff_table (ampl+1, nh, 1.0, hCtlIdVL(ID_AMPL));
	aff_table (phase+1, nh, 2.0 / PI, hCtlIdVL(ID_PHASE));
}

/*
fonction (int n;
	for (n=1; n<=20; n++)
		y += sin (n * x + phi[n]) / n;)
*/


#if 1

float sound_fn (float x) {float y; y=0.0; {

/*fonction (*/ int n;
	switch (type_synthese)
	{
    	case 'AP':
			for (n=1; n<=nh; n++)
				y += sin ((n * x) + phase[n]) * ampl[n] /* / n */ ;
			break;
		case 'SC':
			for (n=1; n<=nh; n++)
				y += sin (n * x) * ampl_sin[n] + cos (n * x) * ampl_cos[n];
			break;
	}
/*)*/
;} return y; }
/*	char def_f[] = #fx; */

cofonction (int n;
	switch (type_synthese)
	{
		case 'AP':
			for (n=1; n<=nh; n++)
				y += cos ((n * x) + phase[n]) * ampl[n];
			break;
		case 'SC':
			for (n=1; n<=nh; n++)
				y += cos (n * x) * ampl_sin[n] - sin (n * x) * ampl_cos[n];
			break;
    }
)

#else
fonction (y = ampl[x])
#endif


char **gbl_argv;

#define MAX_SAMPLES 1000

int n_samples = MAX_SAMPLES;
 
int sound[MAX_SAMPLES+1];
int cosound[MAX_SAMPLES+1];

int flag_sound = 0;
int flag_cosound = 0;
int flag_chron = 0;

#ifndef DOS
int LARGEUR, HAUTEUR;
#endif

void main_dos (int argc, char *argv[])
{
  VOCPTR voc_buffer = 0;
  char   ch;
  float periode;
  int note;
  int t;

  gbl_argv = argv;
  init_table ();

  clrscr();

#ifdef DOS
  if (_voc_init_driver() == FALSE) erreur();
#endif

loop:
  gotoxy(1,1); printf("\n");
  if (!init_son ()) goto end;
  switch (type_synthese)
  {
	case 'AP':
		calcul_ampl_sin_cos ();
        aff_ampl_sin_cos ();
		break;
	case 'SC':
		calcul_ampl_phase ();
        aff_ampl_phase ();
        break;
  }
/* Charge le fichier DEMO.VOC dans la mÇmoire */
/*  voc_buffer = _voc_get_buffer("demo.voc"); */

	if (flag_sound)
	{
	HDC hDC;
	hDC = GetDC (hWndVL);

	for (t=0; t<n_samples; t++)
    {
		sound[t] = 0x80 + (int)(atoi(argv[2]) * sound_fn (2.0 * PI *
			(float)t / n_samples));
        if (flag_cosound)
		cosound[t] = 0x80 + (int)(atoi(argv[2]) * sound_cofn (2.0 * PI *
			(float)t / n_samples));
		{
		int x, y;
 		COLORREF cr;

		y = HAUTEUR - ((long)(sound[t]) * (long)HAUTEUR / 0x100);
		if (flag_chron)
        {
			x = (long)t * (long)LARGEUR / n_samples;
			cr = SetPixel (hDC, x, y, RGB(0,0,0));
        }
		if (flag_cosound)
        {
			x = (long)(cosound[t]) * (long)LARGEUR / 0x100;
			cr = SetPixel (hDC, x, y, RGB(0,0,0));
		}

		}
	}
	ReleaseDC (hWndVL, hDC);
	}

  voc_buffer = create_voc ((float)atoi(argv[1]), atoi(argv[2]),
	atoi(argv[3]), atoi(argv[4]), atoi(argv[5]), 1);
loop1:
  trace ("loop1", 0);
  vocfile_headerlength = 0;
  trace ("%p", voc_buffer);
/* Le fichier VOC n'a pas pu àtre chargÇ */
  if (voc_buffer == NULL) erreur();
/*  dump (voc_buffer, 120); */

#ifndef DOS
  {
  int i;
  for (i=0; i<0x40; i++)
  {
	if ((i % 0x10) == 0)
		printf ("\n%3X : ", i);
	printf ("%02X ", (unsigned char)(voc_buffer[i]));
  }
  }
	/* goto musique; */
	sndplaysound (voc_buffer,
		SND_ASYNC | SND_NODEFAULT | SND_MEMORY | SND_LOOP);
	return;
	sndplaysound ("c:\\windows\\accord.wav", SND_ASYNC);

  {
	FILE *f;
	int i;
    int c;
		f = fopen ("c:\\sbprogs\\wav\\verre.wav", "rb");
        i = 0;
		for (;;)
		{
			c = fgetc (f);
			if (feof(f) || c == EOF)
				break;
			voc_buffer[i++] = c;
        }
		fclose (f);
  {
  int i;
  for (i=0; i<0x40; i++)
  {
	if ((i % 0x10) == 0)
		printf ("\n%3X : ", i);
	printf ("%02X ", (unsigned char)(voc_buffer[i]));
  }
  }
		sndplaysound (voc_buffer,
			SND_ASYNC | SND_NODEFAULT | SND_MEMORY);
  }
  return;
musique:
  {
  LPSTR  WavResPtr;
  HANDLE WavHandle;
  HANDLE WavResInfo;
  int i;

  WavResInfo = FindResource(hInst,"Musique","WAVE");
  if (!(WavResInfo>0))
	 exit(1); // Erreur, ressource non trouvÇe
  WavHandle = LoadResource(hInst,WavResInfo);
  if (!(WavHandle>0))
     exit(2); // Erreur, Handle non trouvÇ
  WavResPtr = LockResource(WavHandle);

  if (WavResPtr != NULL) {
	 /* WavResPtr[5] = 0x97; */
	 sndplaysound(WavResPtr,
		SND_ASYNC | SND_NODEFAULT | SND_MEMORY);

  for (i=0; i<0x40; i++)
  {
	if ((i % 0x10) == 0)
		printf ("\n%3X : ", i);
	printf ("%02X ", WavResPtr[i]);
  }
  }
  }


	return;
#endif

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

play_note (char ch, char **argv)
{
int note;
float periode;
char *voc_buffer;
  voc_buffer = NULL;
  note = clavier (ch);
  trace ("%d", note);
  periode = (float) atoi(argv[1]) / pow (2.0, (float) note / 12.0);
  printf (" Touche '%c' 0x%02X, note %d, periode %f \n",
	ch, ch, note, periode);
  voc_buffer = create_voc (periode, atoi(argv[2]), atoi(argv[3]),
				atoi(argv[4]), atoi(argv[5]), 0);
  sndplaysound (voc_buffer,
		SND_ASYNC | SND_NODEFAULT | SND_MEMORY | SND_LOOP);

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
#if 0

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
#ifdef DOS
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
#else
char buf[1000];
char buf_phase[1000];
char buf_sin[1000];
char buf_cos[1000];
char buf1[30];
char *p, *p_phase, *p_sin, *p_cos;
int i, j;
float phase1;
	SendMessage (hCtlIdVL(ID_AMPL), WM_GETTEXT, sizeof(buf),
		(LPARAM) (LPSTR) buf);
	SendMessage (hCtlIdVL(ID_PHASE), WM_GETTEXT, sizeof(buf_phase),
		(LPARAM) (LPSTR) buf_phase);
	SendMessage (hCtlIdVL(ID_AMPL_SIN), WM_GETTEXT, sizeof(buf_sin),
		(LPARAM) (LPSTR) buf_sin);
	SendMessage (hCtlIdVL(ID_AMPL_COS), WM_GETTEXT, sizeof(buf_cos),
		(LPARAM) (LPSTR) buf_cos);

	p = buf;
	p_phase = buf_phase;
	p_sin = buf_sin;
	p_cos = buf_cos;

	for (i=1; ;i++)
	{
		if (*p == 't' || *p_sin == 't')
        	break;
		for (j=0; *p && *p != '\r'; )
		{
			buf1[j++] = *p++;
		}
        buf1[j] = 0;
		p += 2;
		if (*buf1)
			sscanf (buf1, "%f", &(ampl[i]));

		for (j=0; *p_phase && *p_phase != '\r'; )
		{
			buf1[j++] = *p_phase++;
		}
        buf1[j] = 0;
		p_phase += 2;
		if (*buf1)
			sscanf (buf1, "%f", &(phase1));
		phase[i] = 2 * PI * phase1 / 4.0;

		for (j=0; *p_sin && *p_sin != '\r'; )
		{
			buf1[j++] = *p_sin++;
		}
        buf1[j] = 0;
		p_sin += 2;
		if (*buf1)
			sscanf (buf1, "%f", &(ampl_sin[i]));

		for (j=0; *p_cos && *p_cos != '\r'; )
		{
			buf1[j++] = *p_cos++;
		}
        buf1[j] = 0;
		p_cos += 2;
		if (*buf1)
			sscanf (buf1, "%f", &(ampl_cos[i]));

	}
	nh = i-1;
	    
#endif

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
	/* restorecrtmode();*/
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

#ifdef DOS
#include <graphics.h>

#define LARGEUR 600
#define HAUTEUR 200
#else

/* int LARGEUR, HAUTEUR; */

#endif

void dessin (void);

int flag_init = 1;

struct wave_header
{
	char tag_riff[4];
	long len_riff;
	char tag_wave[4];
	char tag_fmt[4];
	long len_fmt;
    
	WORD formatTag;
	WORD nChannels;
	DWORD nSamplesPerSec;
	DWORD nAvgBytesPerSec;
	WORD nBlockAlign;

	WORD nBitsPerSample;

	char tag_data[4];
	long len_data;
};

init_wave_header (struct wave_header *h)
{
	memcpy (h->tag_riff, "RIFF", 4);
	h->len_riff = 0x24;
	memcpy (h->tag_wave, "WAVE", 4);
	memcpy (h->tag_fmt, "fmt ", 4);
	h->len_fmt = 0x10;
	h->formatTag = 1;
	h->nChannels = 1;
#define NSPS 0x2B11 /* 0x5622 */
	h->nSamplesPerSec = NSPS;
	h->nAvgBytesPerSec = NSPS;
	h->nBlockAlign = 1;
	h->nBitsPerSample = 8;
	memcpy (h->tag_data, "data", 4);
	h->len_data = 0;

}

char *create_voc (float periode_float, int a, int duree, int np, int flag_harmon,
	int flag_create)
{
char c;
int n, i;
int t;
char *p;
static unsigned char buf_son[4000];
int pn;
int driver, mode;
int status;
char buf[200];
/* static int flag_init = 1; */
int periode;
#ifndef DOS
HDC hDC;
struct wave_header header;
int len;
#endif
int flag_first;

	trace ("create_voc", 0);

	flag_first = 1;

	periode = (int) (periode_float + 0.5);

	trace ("", 0);

    if (flag_init == 0)
	flag_create = 0;
    flag_init = 0;

	trace ("", 0);

    if (flag_create)
    {
	trace ("", 0);
#ifdef DOS
	driver = HERCMONO;
	mode = HERCMONOHI;
	initgraph (&driver, &mode, "C:\D1");
	status = graphresult ();
	if (status != grOk)
	{
		printf ("Erreur initgraph %d 0x%X\n", status, status);
		exit (status);
	}
#else
	hDC = GetDC (hWndVL);
	/* SetPixel (hDC, 20, 8, RGB(0,0,0)); */

	/*
	LARGEUR = GetDeviceCaps (hDC, HORZRES) - 1;
	HAUTEUR = GetDeviceCaps (hDC, VERTRES) - 1;
	*/
    /*
	LARGEUR = 80;
	HAUTEUR = 30;
	*/

	HGDIOBJ hPen;
	hPen = GetStockObject (BLACK_PEN);
	SelectObject (hDC, hPen);
	/* Ellipse (hDC, 1, 1, 199, 119); */
	/* SetMapMode (hDC, MM_ISOTROPIC); */

	/*
	ReleaseDC (hWndVL, hDC);

	hDC = GetDC (hWndVL);
	*/
#endif
    }

	trace ("", 0);
	ptr = 0;
	p = buffer;

#ifdef WAVE
/*
	WORD formatTag;
	WORD nChannels;
	DWORD nSamplesPerSec
	DWORD nAvgBytesPerSec;
	DWORD nBlockAlign;

	WORD nBitsPerSample;
*/
/*	p += sprintf (p, "RIFF\0\0\0\0WAVEfmt \x10\0\0\0 */
	init_wave_header (&header);
	memcpy (p, &header, sizeof(header));
	p += sizeof(header);
#endif

#ifdef VOC
	if (duree != -1)
	p += sprintf (p, "%c%c%c%c%c%c", 6, 2, 0, 0,
		duree&0xFF, duree>>8);
 
	trace ("%p", p);

	p += sprintf (p, "%c%c%c%c%c%c", 0x01,
			(int)(periode*np+2)&0xFF,
			(int)(periode*np+2)>>8, 0, 0x83, 0);
#endif

	trace ("%d", periode);
	for (t=0; t<periode; t++)
	{
		/* printf ("t = %d\n", t); */
		trace ("%d", t);
	    if (flag_harmon)
		{
        	if (!flag_sound)
 				buf_son[t] = 0x80 + (int)(a * sound_fn (2.0 * PI *
							(float)t / periode));
 			else
				buf_son[t] = sound [(long)t * (long)n_samples / periode];
        }
	    else
		buf_son[t] = ampl[t-1];
 		if (flag_create)
		{
#ifdef DOS
		putpixel ((long)t * (long)LARGEUR / periode,
		 HAUTEUR - ((long)(buf_son[t]) * (long)HAUTEUR / 0x100),
			1);
#else
/*
		SetPixel (hDC, (long)t * (long)LARGEUR / periode,
		 HAUTEUR - ((long)(buf_son[t]) * (long)HAUTEUR / 0x100),
			RGB(0,0,0));
*/
		if (!flag_sound)
		{
		int x, y;
        static int x1, y1;
		COLORREF cr;
		int z;
		/* dessin(); */

		/*
		HDC hDC;
 		hDC = GetDC (hWndVL);
        */
		/* SetPixel (hDC, 20, 8, RGB(0,0,0)); */

		x = (long)t * (long)LARGEUR / periode;
		y = HAUTEUR - ((long)(buf_son[t]) * (long)HAUTEUR / 0x100);
		cr = SetPixel (hDC, x, y, RGB(0,0,0));

		if (flag_first)
		{
			MoveTo (hDC, x, y);
			flag_first = 0;
		}
		else
		{
			MoveTo (hDC, x1, y1);
			LineTo (hDC, x, y);
        }

		x1 = x;
		y1 = y;

		z = 0;

		/*ReleaseDC (hWndVL, hDC);*/
        
        }
		
#endif
	    }
	}

#define PI 3.1415926
	if (flag_create)
	{
	int h;
	float s;
	int t;
#ifdef DOS
		gets (buf);
		restorecrtmode ();
#else
		ReleaseDC (hWndVL, hDC);
#endif

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

#ifdef VOC
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
#endif

#ifdef WAVE
	len = p - buffer - sizeof(struct wave_header);
	((struct wave_header *)buffer)->len_riff += len;
	((struct wave_header *)buffer)->len_data = len;
#endif




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
	status = allocmem ((size+15L)/16, (unsigned int *)&segment);
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

int prev = 0;

int clavier1 (char c);

int clavier2 (char c)
{
	if (c >= 1 && c <= 26)
		return clavier1 (c + 0x60) + 12;
	if (c >= 'A' && c <= 'Z')
		return clavier1 (c + 0x20) - 12;
	switch (c)
	{
    /*
		case 'q': return -3;
		case 'a': return -2;
		case 'w': return -1;
	*/
		case 'a': return -3;
		case 'q': return -2;
		case 'z': return -1;

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
/*
		case ';': return 14;
		case '[': return 15;
		case '\'': return 16;
		case '\\': return 17;
		case ']': return 18;
*/
		case 'm': return 14;
        case ')': return 15;
		case '˘': return 16;
		case '$': return 17;
		case '*': return 18;

		default : return prev;
	}
}

int clavier1 (char c)
{
	prev = clavier2 (c);
	return prev;
}

int clavier (char c)
{
	return clavier1(c) - 5;
	/*return prev-5;*/
}


class TAppliWindowVL : public TWindow
{
public:
	/* declare control objects PT... */
	/* PTButton Button1; */
    PTWindowsObject pWndCtlObj[NCTL];
	HWND hWndCtlObj[NCTL];
 
	TAppliWindowVL (PTWindowsObject parent, LPSTR title) :
		TWindow (parent, title)
	{
		EnableKBHandler ();
        /* create control objects PT... with new T... */ 
		/* Button1 = new TButton (this, 101, "&Test 1", 50, 80, 40, 20, FALSE); */
	}

	LPSTR GetClassName ()
	{
    	return szAppNameVL;
	}

	virtual BOOL Create ()
	{
	BOOL status;
		status = TWindow::Create ();
		hWndVL = HWindow;
        /* create objects hWndCtlObj[i] with CreateWindow */ 
 		return status;
    }
	/* void HandleWndCtli (RTMessage Msg) = [ID_FIRST + COMMAND0 + i] { ... } */
};

TAppliWindowVL *WndVL;

class TAppliVL : public TApplication
{
public:
	TAppliVL (LPSTR AName, HINSTANCE hInstance, HINSTANCE hPrevInstance,
		LPSTR lpCmdLine, int nCmdShow) :
		TApplication (AName, hInstance, hPrevInstance, lpCmdLine, nCmdShow)
	{
	}
	virtual void InitMainWindow ()
	{
		WndVL = new TAppliWindowVL (NULL, Name);
		MainWindow = WndVL;
		WndVL->Attr.W = (WidVL)*2;
		WndVL->Attr.H = (HeiVL)*3/2+20;
    }
};


	/*
	EDITTEXT ID_AMPL, 194, 5, 35, 136, ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP
	EDITTEXT ("EDIT") "" ID=ID_AMPL X= 194 Y= 5 W= 35 H= 136 STYLE= ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL
	*/
void HandleCtlVLID_AMPL (TMessage *pmsg)
{
/* Add code here */

}

	/*
	LTEXT "", -1, 170, 9, 16, 8, WS_CHILD | WS_VISIBLE | WS_GROUP
	LTEXT ("STATIC") "" ID= -1 X= 170 Y= 9 W= 16 H= 8 STYLE= WS_CHILD | WS_VISIBLE | WS_GROUP
	*/
void HandleCtlVL2 (TMessage *pmsg)
{
/* Add code here */

}

	/*
	PUSHBUTTON "Draw", ID_DRAW, 11, 129, 24, 14, WS_CHILD | WS_VISIBLE | WS_TABSTOP
	PUSHBUTTON ("BUTTON") "Draw" ID= ID_DRAW X= 11 Y= 129 W= 24 H= 14 STYLE= WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON
	*/
void dessin ()
{
HDC hDC;
HPEN hPen;
	MessageBox (hWndVL, "Dessin (7)", "Message", MB_OK);
	hDC = GetDC (hWndVL);

	SetPixel (hDC, 20, 8, RGB(0,0,0));

	hPen = GetStockObject (BLACK_PEN);
	SelectObject (hDC, hPen);
	Ellipse (hDC, 40, 1, 70, 12 /*199, 119*/);

    ReleaseDC (hWndVL, hDC);
}

void synthetiser (void);

void HandleCtlVLID_DRAW (TMessage *pmsg)
{
	type_synthese = 'AP';
    synthetiser ();
}

void HandleCtlVLID_SYNT_SIN_COS (TMessage *pmsg)
{
	type_synthese = 'SC';
	synthetiser ();
}

void synthetiser (void)
{
/* Add code here */

	/*dessin ();*/
	
/*
char *create_voc (float periode, int a, int duree, int np, int flag_harmon,
	int flag_create);
*/

static char *argv1[10];

	hWndGbl = hWndVL;

	argv1[0] = "";
	argv1[1] = "40";
	argv1[2] = "1";
	argv1[3] = "30";
	argv1[4] = "1" /*"300"*/;
	argv1[5] = "1";
	argv1[6] = "1";

	flag_init = 1;

	main_dos (7, argv1);

}

	/*

	 () "" ID=000 X=000 Y=000 W=000 H=000 STYLE=000
	*/
void HandleCtlVL4 (TMessage *pmsg)
{
/* Add code here */
int status;
	status = SendMessage (hCtlIdVL(COMMAND0+4), BM_GETCHECK, 0, 0);
	flag_sound = status;
}

void HandleCtlVL5 (TMessage *pmsg)
{
	flag_cosound = SendMessage (hCtlIdVL(COMMAND0+5), BM_GETCHECK, 0, 0);
}

void HandleCtlVL6 (TMessage *pmsg)
{
	flag_chron = SendMessage (hCtlIdVL (COMMAND0+6), BM_GETCHECK, 0, 0);
}

LRESULT CALLBACK WndProcVL (HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
int nCtl;
struct TMessage msg;
	msg.Receiver = hWnd;
	msg.Message = Message;
	msg.WParam = wParam;
	msg.LParam = lParam;
	msg.Result = 0;
	switch (Message)
	{
		case WM_CREATE:
			WndVL->HWindow = hWndVL;
        	/* create control objects hCtlObjVL[i] with CreateWindow */
			for (nCtl=1; nCtl<=NCTLVL; nCtl++)
			{
				hCtlObjVL[nCtl] = CreateWindow (ResVL[nCtl].ctlclass, ResVL[nCtl].caption,
					ResVL[nCtl].param_int[P_STYLE],
					ResVL[nCtl].param_int[P_X]*2, ResVL[nCtl].param_int[P_Y]*3/2, ResVL[nCtl].param_int[P_W]*2, ResVL[nCtl].param_int[P_H]*3/2,
					hWnd, COMMAND0+nCtl, hInst, NULL);
			}
			FocusVL = 1;
			SetFocus (hCtlObjVL[FocusVL]);
			InitWndVL (hWnd);
			break;
		case WM_CLOSE:
			for (nCtl=1; nCtl<=NCTLVL; nCtl++)
				DestroyWindow (hCtlObjVL[nCtl]);
			sndplaysound ("c:\\windows\\carillon.wav", SND_ASYNC);
			return DefWindowProc(hWnd, Message, wParam, lParam);
		case WM_SIZE:
			for (nCtl=1; nCtl<=NCTLVL; nCtl++)
			{
				LARGEUR = LOWORD(lParam) * 3 / 5; /* 3/4 */
				HAUTEUR = HIWORD(lParam) * 4 / 5;
				MoveWindow (hCtlObjVL[nCtl],
					ResVL[nCtl].param_int[P_X]*LOWORD(lParam)/(WidVL),
					ResVL[nCtl].param_int[P_Y]*HIWORD(lParam)/(HeiVL),
					ResVL[nCtl].param_int[P_W]*LOWORD(lParam)/(WidVL),
					ResVL[nCtl].param_int[P_H]*HIWORD(lParam)/(HeiVL),
					TRUE);
			}
 			break;
		case WM_COMMAND:
			switch (wParam)
			{
				/* case COMMAND0+i:
					HandleCtlVLi();
					break; */
				case COMMAND0+1:
					HandleCtlVLID_AMPL (&msg);
					break;
				case COMMAND0+2:
					HandleCtlVL2 (&msg);
					break;
				case COMMAND0+3:
					HandleCtlVLID_DRAW (&msg);
					break;
				case COMMAND0+4:
					HandleCtlVL4 (&msg);
					break;
				case COMMAND0+5:
					HandleCtlVL5 (&msg);
					break;
				case COMMAND0+6:
					HandleCtlVL6 (&msg);
					break;
				case ID_SYNT_SIN_COS:
					HandleCtlVLID_SYNT_SIN_COS (&msg);
                    break;
 			}
			break;
		default:
			return DefWindowProc(hWnd, Message, wParam, lParam);
	}
	return NULL;
}

InitWndVL (HWND hWnd)
{
/* Add code here */
	SendMessage (hCtlIdVL(ID_AMPL), WM_SETTEXT, 0,
		(LPARAM) (LPCSTR) "t");
    flag_chron = 1;
	SendMessage (hCtlIdVL(COMMAND0+5), BM_SETCHECK, 1, 0);
	flag_cosound = 1;
	SendMessage (hCtlIdVL(COMMAND0+6), BM_SETCHECK, 1, 0);
	return 0;
}

char CtlKey (char *s)
{
int i, l;
	l = strlen (s);
	for (i=0; i<l-1; i++)
	{
		if (s[i] == '&')
        	return s[i+1];
	}
	return 0;

}

int KBHandler (MSG *pmsg, HWND hWnd, int NbrCtl, struct resource_item *Res, HWND *hCtlObj, int *pFocus)
{
int PrevFocus, nCtl;
HWND hw;
		if (pmsg->message == WM_CHAR)
		{
			if (pmsg->wParam == '\t')
			{
			  hw = GetFocus ();
			  for (nCtl=1; nCtl<=NbrCtl; nCtl++)
			  {
				if (hw == hCtlObj[nCtl])
					*pFocus = nCtl;
			  }        
              PrevFocus = *pFocus;
			  do
              {
				if (GetKeyState (VK_SHIFT) >= 0)
                {
					if (*pFocus < NbrCtl)
						(*pFocus)++;
					else
						*pFocus = 1;
				}
				else
				{
					if (*pFocus > 1)
						(*pFocus)--;
					else
						*pFocus = NbrCtl;
				}
			  }
			  while (!strcmp (Res[*pFocus].name, "LTEXT") &&
					 (*pFocus != PrevFocus));
			  SetFocus (hCtlObj[*pFocus]);
 			  return 1;
			}
        }
 		if (pmsg->message == WM_SYSCHAR)
		{
				for (nCtl=1; nCtl<=NbrCtl; nCtl++)
				{
					if (toupper(CtlKey (Res[nCtl].caption)) == toupper(pmsg->wParam))
					{
						*pFocus = nCtl;
						SetFocus (hCtlObj[*pFocus]);
						if (!strcmp (Res[nCtl].name, "PUSHBUTTON"))
							SendMessage (hWnd, WM_COMMAND, COMMAND0+nCtl, 0);
 						return 1;
					} 
                }
 		}
		return 0;
}	

int PASCAL WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
WNDCLASS wndclass;
MSG msg;

	_InitEasyWin ();
	hInst = hInstance;

	InitResVL ();
	if (!hPrevInstance)
	{
		wndclass.style = CS_HREDRAW | CS_VREDRAW;
		wndclass.lpfnWndProc = WndProcVL;
		wndclass.cbClsExtra = 0;
		wndclass.cbWndExtra = 0;
		wndclass.hInstance = hInstance;
		wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
		wndclass.hbrBackground  = GetStockObject(WHITE_BRUSH);
		wndclass.lpszMenuName = NULL;
		wndclass.lpszClassName  = szAppNameVL;

		if (!RegisterClass(&wndclass))
			return FALSE;
	}

#ifdef OWLVL
	hInst = hInstance;
	TAppliVL Appli (szAppNameVL, hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	Appli.Run ();
	return Appli.Status;
#else
	WndVL = new TAppliWindowVL (NULL, WndCaptionVL);
	hWndVL = CreateWindow (szAppNameVL, WndCaptionVL,
		WS_OVERLAPPEDWINDOW | WS_VSCROLL | WS_HSCROLL,
		CW_USEDEFAULT, CW_USEDEFAULT, (WidVL)*2, (HeiVL)*3/2+20,
		NULL, NULL, hInstance, NULL);

	if (!hWndVL)
		return FALSE;

	ShowWindow (hWndVL, nCmdShow);
	UpdateWindow (hWndVL);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!KBHandler (&msg, hWndVL, NCTLVL, ResVL, hCtlObjVL, &FocusVL))
		{
		  if (msg.message == WM_CHAR &&
				/* FocusVL != ID_AMPL-COMMAND0 && */
				GetFocus() != hCtlIdVL(ID_AMPL) &&
				GetFocus() != hCtlIdVL(ID_PHASE) &&
				GetFocus() != hCtlIdVL(ID_AMPL_SIN) &&
				GetFocus() != hCtlIdVL(ID_AMPL_COS) &&
				msg.wParam != ' ')
		  {
			play_note (msg.wParam, gbl_argv);
		  }
		  else
          {
			TranslateMessage (&msg);
			DispatchMessage (&msg);
          }
		}
	}

	return(msg.wParam);
#endif
}
