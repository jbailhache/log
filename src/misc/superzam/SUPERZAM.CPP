/* Windows API source generated from resource file superzam.ctl */
 
#include <owl.h>
#include <window.h>
#include <string.h>
#include <ctype.h>

#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <sys/stat.h>

#include <combobox.h>

#define OP_Ajouter 0
#define OP_Multiplier 1

#define sized(x) (x), (sizeof(x))

cmpstr (char *s1, char *s2)
{
int x, i1, i2, l1, l2, k;
	l1 = strlen (s1);
	l2 = strlen (s2);
	x = 0;
	for (i1=0; i1<l1; i1++)
	for (i2=0; i2<l2; i2++)
	{
		for (k=0; i1+k<l1 && i2+k<l2 && s1[i1+k]==s2[i2+k]; k++)
			x += k+1;
	}
	return x;
}

/*
#define KAT_LONGECO 15
#define DEM_LONGECO 120
#define RESP_LONGECO 20
*/
#define KAT_LONGECO 15
#define DEM_LONGECO 200
#define RESP_LONGECO 60

#define N_RESPONDOJ 3

#define N_SIMIL 500

struct registrero
{
	char Kateg[KAT_LONGECO+1];
	int Nombro;
	char Demando[DEM_LONGECO+1];
    char BonaResp;
    char Respondo[N_RESPONDOJ][RESP_LONGECO+1];
	char ext[30];
};

struct simil
{
	int sim;
	/* struct registrero r; */
	struct
	{
		char Kateg[KAT_LONGECO+1];
		int Nombro;
	} r;

};

HANDLE hInst;
HWND hWnd;

#define NCTL 60
HWND hCtlObj[NCTL];

int Focus;

static char szAppName[] = "Administrado de demandaro";

#define COMMAND0 100

#define hCtlId(id) (hCtlObj[(id)-COMMAND0])

#define Wid (300)
#define Hei (180)

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

struct resource_item Res[NCTL];

InitRes ()
{

	/*
	LTEXT "Kategorio", -1, 5, 23, 37, 10, WS_CHILD | WS_VISIBLE | WS_GROUP
	LTEXT ("STATIC") "Kategorio" ID= -1 X= 5 Y= 23 W= 37 H= 10 STYLE= WS_CHILD | WS_VISIBLE | WS_GROUP
	*/
	strcpy (Res[1].name, "LTEXT");
	strcpy (Res[1].ctlclass, "STATIC");
	strcpy (Res[1].caption, "Kategorio");
	Res[1].param_int[1] =  5;
	Res[1].param_int[2] =  23;
	Res[1].param_int[3] =  37;
	Res[1].param_int[4] =  10;
	Res[1].param_int[5] =  WS_CHILD | WS_VISIBLE | WS_GROUP;

	/*
	LTEXT "Diskstokejo", ID_SUMO, 7, 6, 38, 10, WS_CHILD | WS_VISIBLE | WS_GROUP
	LTEXT ("STATIC") "Diskstokejo" ID= ID_SUMO X= 7 Y= 6 W= 38 H= 10 STYLE= WS_CHILD | WS_VISIBLE | WS_GROUP
	*/
	strcpy (Res[2].name, "LTEXT");
	strcpy (Res[2].ctlclass, "STATIC");
	strcpy (Res[2].caption, "Diskstokejo");
	Res[2].param_int[1] =  7;
	Res[2].param_int[2] =  6;
	Res[2].param_int[3] =  38;
	Res[2].param_int[4] =  10;
	Res[2].param_int[5] =  WS_CHILD | WS_VISIBLE | WS_GROUP;

	/*
	EDITTEXT ID_KATEG, 43, 21, 47, 14, ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP
	EDITTEXT ("EDIT") "" ID=ID_KATEG X= 43 Y= 21 W= 47 H= 14 STYLE= ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL
	*/
	strcpy (Res[3].name, "EDITTEXT");
	strcpy (Res[3].ctlclass, "EDIT");
	strcpy (Res[3].caption, "");
	Res[3].param_int[1] =  43;
	Res[3].param_int[2] =  21;
	Res[3].param_int[3] =  47;
	Res[3].param_int[4] =  14;
	Res[3].param_int[5] =  ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL;

	/*
	LTEXT "Nombro", -1, 95, 24, 30, 9, WS_CHILD | WS_VISIBLE | WS_GROUP
	LTEXT ("STATIC") "Nombro" ID= -1 X= 95 Y= 24 W= 30 H= 9 STYLE= WS_CHILD | WS_VISIBLE | WS_GROUP
	*/
	strcpy (Res[4].name, "LTEXT");
	strcpy (Res[4].ctlclass, "STATIC");
	strcpy (Res[4].caption, "Nombro");
	Res[4].param_int[1] =  95;
	Res[4].param_int[2] =  24;
	Res[4].param_int[3] =  30;
	Res[4].param_int[4] =  9;
	Res[4].param_int[5] =  WS_CHILD | WS_VISIBLE | WS_GROUP;

	/*
	EDITTEXT ID_NOMBRO, 126, 20, 27, 15, ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP
	EDITTEXT ("EDIT") "" ID=ID_NOMBRO X= 126 Y= 20 W= 27 H= 15 STYLE= ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL
	*/
	strcpy (Res[5].name, "EDITTEXT");
	strcpy (Res[5].ctlclass, "EDIT");
	strcpy (Res[5].caption, "");
	Res[5].param_int[1] =  126;
	Res[5].param_int[2] =  20;
	Res[5].param_int[3] =  27;
	Res[5].param_int[4] =  15;
	Res[5].param_int[5] =  ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL;

	/*
	LTEXT "Nova demando", -1, 3, 43, 38, 17, WS_CHILD | WS_VISIBLE | WS_GROUP
	LTEXT ("STATIC") "Nova demando" ID= -1 X= 3 Y= 43 W= 38 H= 17 STYLE= WS_CHILD | WS_VISIBLE | WS_GROUP
	*/
	strcpy (Res[6].name, "LTEXT");
	strcpy (Res[6].ctlclass, "STATIC");
	strcpy (Res[6].caption, "Nova demando");
	Res[6].param_int[1] =  3;
	Res[6].param_int[2] =  43;
	Res[6].param_int[3] =  38;
	Res[6].param_int[4] =  17;
	Res[6].param_int[5] =  WS_CHILD | WS_VISIBLE | WS_GROUP;

	/*
	EDITTEXT ID_DEMANDO, 43, 40, 247, 17, ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP
	EDITTEXT ("EDIT") "" ID=ID_DEMANDO X= 43 Y= 40 W= 247 H= 17 STYLE= ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL
	*/
	strcpy (Res[7].name, "EDITTEXT");
	strcpy (Res[7].ctlclass, "EDIT");
	strcpy (Res[7].caption, "");
	Res[7].param_int[1] =  43;
	Res[7].param_int[2] =  40;
	Res[7].param_int[3] =  247;
	Res[7].param_int[4] =  17;
	Res[7].param_int[5] =  ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL;

	/*
	LTEXT "Respondoj", -1, 3, 64, 36, 11, WS_CHILD | WS_VISIBLE | WS_GROUP
	LTEXT ("STATIC") "Respondoj" ID= -1 X= 3 Y= 64 W= 36 H= 11 STYLE= WS_CHILD | WS_VISIBLE | WS_GROUP
	*/
	strcpy (Res[8].name, "LTEXT");
	strcpy (Res[8].ctlclass, "STATIC");
	strcpy (Res[8].caption, "Respondoj");
	Res[8].param_int[1] =  3;
	Res[8].param_int[2] =  64;
	Res[8].param_int[3] =  36;
	Res[8].param_int[4] =  11;
	Res[8].param_int[5] =  WS_CHILD | WS_VISIBLE | WS_GROUP;

	/*
	CONTROL "", ID_RESP1BONA, "BUTTON", BS_AUTORADIOBUTTON | WS_CHILD | WS_VISIBLE | WS_TABSTOP, 43, 65, 9, 12
	CONTROL ( "BUTTON") "" ID= ID_RESP1BONA X= 43 Y= 65 W= 9 H= 12 STYLE= BS_AUTORADIOBUTTON | WS_CHILD | WS_VISIBLE | WS_TABSTOP
	*/
	strcpy (Res[9].name, "CONTROL");
	strcpy (Res[9].ctlclass,  "BUTTON");
	strcpy (Res[9].caption, "");
	Res[9].param_int[1] =  43;
	Res[9].param_int[2] =  65;
	Res[9].param_int[3] =  9;
	Res[9].param_int[4] =  12;
	Res[9].param_int[5] =  BS_AUTORADIOBUTTON | WS_CHILD | WS_VISIBLE | WS_TABSTOP;

	/*
	CONTROL "", ID_RESP2BONA, "BUTTON", BS_AUTORADIOBUTTON | WS_CHILD | WS_VISIBLE | WS_TABSTOP, 129, 66, 8, 12
	CONTROL ( "BUTTON") "" ID= ID_RESP2BONA X= 129 Y= 66 W= 8 H= 12 STYLE= BS_AUTORADIOBUTTON | WS_CHILD | WS_VISIBLE | WS_TABSTOP
	*/
	strcpy (Res[10].name, "CONTROL");
	strcpy (Res[10].ctlclass,  "BUTTON");
	strcpy (Res[10].caption, "");
	Res[10].param_int[1] =  129;
	Res[10].param_int[2] =  66;
	Res[10].param_int[3] =  8;
	Res[10].param_int[4] =  12;
	Res[10].param_int[5] =  BS_AUTORADIOBUTTON | WS_CHILD | WS_VISIBLE | WS_TABSTOP;

	/*
	CONTROL "", ID_RESP3BONA, "BUTTON", BS_AUTORADIOBUTTON | WS_CHILD | WS_VISIBLE | WS_TABSTOP, 209, 64, 8, 16
	CONTROL ( "BUTTON") "" ID= ID_RESP3BONA X= 209 Y= 64 W= 8 H= 16 STYLE= BS_AUTORADIOBUTTON | WS_CHILD | WS_VISIBLE | WS_TABSTOP
	*/
	strcpy (Res[11].name, "CONTROL");
	strcpy (Res[11].ctlclass,  "BUTTON");
	strcpy (Res[11].caption, "");
	Res[11].param_int[1] =  209;
	Res[11].param_int[2] =  64;
	Res[11].param_int[3] =  8;
	Res[11].param_int[4] =  16;
	Res[11].param_int[5] =  BS_AUTORADIOBUTTON | WS_CHILD | WS_VISIBLE | WS_TABSTOP;

	/*
	EDITTEXT ID_RESPONDO1, 55, 64, 69, 16, ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP
	EDITTEXT ("EDIT") "" ID=ID_RESPONDO1 X= 55 Y= 64 W= 69 H= 16 STYLE= ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL
	*/
	strcpy (Res[12].name, "EDITTEXT");
	strcpy (Res[12].ctlclass, "EDIT");
	strcpy (Res[12].caption, "");
	Res[12].param_int[1] =  55;
	Res[12].param_int[2] =  64;
	Res[12].param_int[3] =  69;
	Res[12].param_int[4] =  16;
	Res[12].param_int[5] =  ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL;

	/*
	EDITTEXT ID_RESPONDO2, 141, 64, 64, 16, ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP
	EDITTEXT ("EDIT") "" ID=ID_RESPONDO2 X= 141 Y= 64 W= 64 H= 16 STYLE= ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL
	*/
	strcpy (Res[13].name, "EDITTEXT");
	strcpy (Res[13].ctlclass, "EDIT");
	strcpy (Res[13].caption, "");
	Res[13].param_int[1] =  141;
	Res[13].param_int[2] =  64;
	Res[13].param_int[3] =  64;
	Res[13].param_int[4] =  16;
	Res[13].param_int[5] =  ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL;

	/*
	EDITTEXT ID_RESPONDO3, 220, 64, 70, 16, ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP
	EDITTEXT ("EDIT") "" ID=ID_RESPONDO3 X= 220 Y= 64 W= 70 H= 16 STYLE= ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL
	*/
	strcpy (Res[14].name, "EDITTEXT");
	strcpy (Res[14].ctlclass, "EDIT");
	strcpy (Res[14].caption, "");
	Res[14].param_int[1] =  220;
	Res[14].param_int[2] =  64;
	Res[14].param_int[3] =  70;
	Res[14].param_int[4] =  16;
	Res[14].param_int[5] =  ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL;

	/*
	PUSHBUTTON "&Modifu", ID_REGISTRU, 188, 19, 33, 15, WS_CHILD | WS_VISIBLE | WS_TABSTOP
	PUSHBUTTON ("BUTTON") "&Modifu" ID= ID_REGISTRU X= 188 Y= 19 W= 33 H= 15 STYLE= WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON
	*/
	strcpy (Res[15].name, "PUSHBUTTON");
	strcpy (Res[15].ctlclass, "BUTTON");
	strcpy (Res[15].caption, "&Registru");
	Res[15].param_int[1] =  188;
	Res[15].param_int[2] =  19;
	Res[15].param_int[3] =  33;
	Res[15].param_int[4] =  15;
	Res[15].param_int[5] =  WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON;

	/*
	PUSHBUTTON "&Serchu", ID_SERCHU, 21, 88, 42, 13, WS_CHILD | WS_VISIBLE | WS_TABSTOP
	PUSHBUTTON ("BUTTON") "&Serchu" ID= ID_SERCHU X= 21 Y= 88 W= 42 H= 13 STYLE= WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON
	*/
	strcpy (Res[16].name, "PUSHBUTTON");
	strcpy (Res[16].ctlclass, "BUTTON");
	strcpy (Res[16].caption, "&Serchu");
	Res[16].param_int[1] =  21;
	Res[16].param_int[2] =  88;
	Res[16].param_int[3] =  42;
	Res[16].param_int[4] =  13;
	Res[16].param_int[5] =  WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON;

	/*
	PUSHBUTTON "&Forvishu", ID_FORVISHU, 260, 19, 33, 14, WS_CHILD | WS_VISIBLE | WS_TABSTOP
	PUSHBUTTON ("BUTTON") "&Forvishu" ID= ID_FORVISHU X= 260 Y= 19 W= 33 H= 14 STYLE= WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON
	*/
	strcpy (Res[17].name, "PUSHBUTTON");
	strcpy (Res[17].ctlclass, "BUTTON");
	strcpy (Res[17].caption, "&Forvishu");
	Res[17].param_int[1] =  260;
	Res[17].param_int[2] =  19;
	Res[17].param_int[3] =  33;
	Res[17].param_int[4] =  14;
	Res[17].param_int[5] =  WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON;

	/*
	PUSHBUTTON "Sekva&nta", ID_SEKVANTA, 139, 87, 38, 16, WS_CHILD | WS_VISIBLE | WS_TABSTOP
	PUSHBUTTON ("BUTTON") "Sekva&nta" ID= ID_SEKVANTA X= 139 Y= 87 W= 38 H= 16 STYLE= WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON
	*/
	strcpy (Res[18].name, "PUSHBUTTON");
	strcpy (Res[18].ctlclass, "BUTTON");
	strcpy (Res[18].caption, "Sekva&nta");
	Res[18].param_int[1] =  139;
	Res[18].param_int[2] =  87;
	Res[18].param_int[3] =  38;
	Res[18].param_int[4] =  16;
	Res[18].param_int[5] =  WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON;

	/*
	PUSHBUTTON "An&taùa", ID_ANTAUA, 78, 88, 46, 14, WS_CHILD | WS_VISIBLE | WS_TABSTOP
	PUSHBUTTON ("BUTTON") "An&taùa" ID= ID_ANTAUA X= 78 Y= 88 W= 46 H= 14 STYLE= WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON
	*/
	strcpy (Res[19].name, "PUSHBUTTON");
	strcpy (Res[19].ctlclass, "BUTTON");
	strcpy (Res[19].caption, "An&taùa");
	Res[19].param_int[1] =  78;
	Res[19].param_int[2] =  88;
	Res[19].param_int[3] =  46;
	Res[19].param_int[4] =  14;
	Res[19].param_int[5] =  WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON;

	/*
	LTEXT "Kategorio", -1, 7, 115, 37, 10, WS_CHILD | WS_VISIBLE | WS_GROUP
	LTEXT ("STATIC") "Kategorio" ID= -1 X= 7 Y= 115 W= 37 H= 10 STYLE= WS_CHILD | WS_VISIBLE | WS_GROUP
	*/
	strcpy (Res[20].name, "LTEXT");
	strcpy (Res[20].ctlclass, "STATIC");
	strcpy (Res[20].caption, "Kategorio");
	Res[20].param_int[1] =  7;
	Res[20].param_int[2] =  115;
	Res[20].param_int[3] =  37;
	Res[20].param_int[4] =  10;
	Res[20].param_int[5] =  WS_CHILD | WS_VISIBLE | WS_GROUP;

	/*
	EDITTEXT ID_TKATEG, 46, 113, 55, 14, ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP
	EDITTEXT ("EDIT") "" ID=ID_TKATEG X= 46 Y= 113 W= 55 H= 14 STYLE= ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL
	*/
	strcpy (Res[21].name, "EDITTEXT");
	strcpy (Res[21].ctlclass, "EDIT");
	strcpy (Res[21].caption, "");
	Res[21].param_int[1] =  46;
	Res[21].param_int[2] =  113;
	Res[21].param_int[3] =  55;
	Res[21].param_int[4] =  14;
	Res[21].param_int[5] =  ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL;

	/*
	LTEXT "Nombro", -1, 117, 116, 30, 9, WS_CHILD | WS_VISIBLE | WS_GROUP
	LTEXT ("STATIC") "Nombro" ID= -1 X= 117 Y= 116 W= 30 H= 9 STYLE= WS_CHILD | WS_VISIBLE | WS_GROUP
	*/
	strcpy (Res[22].name, "LTEXT");
	strcpy (Res[22].ctlclass, "STATIC");
	strcpy (Res[22].caption, "Nombro");
	Res[22].param_int[1] =  117;
	Res[22].param_int[2] =  116;
	Res[22].param_int[3] =  30;
	Res[22].param_int[4] =  9;
	Res[22].param_int[5] =  WS_CHILD | WS_VISIBLE | WS_GROUP;

	/*
	EDITTEXT ID_TNOMBRO, 150, 112, 28, 15, ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP
	EDITTEXT ("EDIT") "" ID=ID_TNOMBRO X= 150 Y= 112 W= 28 H= 15 STYLE= ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL
	*/
	strcpy (Res[23].name, "EDITTEXT");
	strcpy (Res[23].ctlclass, "EDIT");
	strcpy (Res[23].caption, "");
	Res[23].param_int[1] =  150;
	Res[23].param_int[2] =  112;
	Res[23].param_int[3] =  28;
	Res[23].param_int[4] =  15;
	Res[23].param_int[5] =  ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL;

	/*
	LTEXT "Trovita demando", -1, 5, 135, 38, 17, WS_CHILD | WS_VISIBLE | WS_GROUP
	LTEXT ("STATIC") "Trovita demando" ID= -1 X= 5 Y= 135 W= 38 H= 17 STYLE= WS_CHILD | WS_VISIBLE | WS_GROUP
	*/
	strcpy (Res[24].name, "LTEXT");
	strcpy (Res[24].ctlclass, "STATIC");
	strcpy (Res[24].caption, "Trovita demando");
	Res[24].param_int[1] =  5;
	Res[24].param_int[2] =  135;
	Res[24].param_int[3] =  38;
	Res[24].param_int[4] =  17;
	Res[24].param_int[5] =  WS_CHILD | WS_VISIBLE | WS_GROUP;

	/*
	EDITTEXT ID_TDEMANDO, 45, 132, 246, 17, ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP
	EDITTEXT ("EDIT") "" ID=ID_TDEMANDO X= 45 Y= 132 W= 246 H= 17 STYLE= ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL
	*/
	strcpy (Res[25].name, "EDITTEXT");
	strcpy (Res[25].ctlclass, "EDIT");
	strcpy (Res[25].caption, "");
	Res[25].param_int[1] =  45;
	Res[25].param_int[2] =  132;
	Res[25].param_int[3] =  246;
	Res[25].param_int[4] =  17;
	Res[25].param_int[5] =  ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL;

	/*
	LTEXT "Respondoj", -1, 5, 156, 36, 11, WS_CHILD | WS_VISIBLE | WS_GROUP
	LTEXT ("STATIC") "Respondoj" ID= -1 X= 5 Y= 156 W= 36 H= 11 STYLE= WS_CHILD | WS_VISIBLE | WS_GROUP
	*/
	strcpy (Res[26].name, "LTEXT");
	strcpy (Res[26].ctlclass, "STATIC");
	strcpy (Res[26].caption, "Respondoj");
	Res[26].param_int[1] =  5;
	Res[26].param_int[2] =  156;
	Res[26].param_int[3] =  36;
	Res[26].param_int[4] =  11;
	Res[26].param_int[5] =  WS_CHILD | WS_VISIBLE | WS_GROUP;

	/*
	CONTROL "", ID_TRESP1BONA, "BUTTON", BS_AUTORADIOBUTTON | WS_CHILD | WS_VISIBLE | WS_TABSTOP, 46, 157, 13, 12
	CONTROL ( "BUTTON") "" ID= ID_TRESP1BONA X= 46 Y= 157 W= 13 H= 12 STYLE= BS_AUTORADIOBUTTON | WS_CHILD | WS_VISIBLE | WS_TABSTOP
	*/
	strcpy (Res[27].name, "CONTROL");
	strcpy (Res[27].ctlclass,  "BUTTON");
	strcpy (Res[27].caption, "");
	Res[27].param_int[1] =  46;
	Res[27].param_int[2] =  157;
	Res[27].param_int[3] =  13;
	Res[27].param_int[4] =  12;
	Res[27].param_int[5] =  BS_AUTORADIOBUTTON | WS_CHILD | WS_VISIBLE | WS_TABSTOP;

	/*
	CONTROL "", ID_TRESP2BONA, "BUTTON", BS_AUTORADIOBUTTON | WS_CHILD | WS_VISIBLE | WS_TABSTOP, 130, 159, 12, 12
	CONTROL ( "BUTTON") "" ID= ID_TRESP2BONA X= 130 Y= 159 W= 12 H= 12 STYLE= BS_AUTORADIOBUTTON | WS_CHILD | WS_VISIBLE | WS_TABSTOP
	*/
	strcpy (Res[28].name, "CONTROL");
	strcpy (Res[28].ctlclass,  "BUTTON");
	strcpy (Res[28].caption, "");
	Res[28].param_int[1] =  130;
	Res[28].param_int[2] =  159;
	Res[28].param_int[3] =  12;
	Res[28].param_int[4] =  12;
	Res[28].param_int[5] =  BS_AUTORADIOBUTTON | WS_CHILD | WS_VISIBLE | WS_TABSTOP;

	/*
	CONTROL "", ID_TRESP3BONA, "BUTTON", BS_AUTORADIOBUTTON | WS_CHILD | WS_VISIBLE | WS_TABSTOP, 210, 158, 12, 16
	CONTROL ( "BUTTON") "" ID= ID_TRESP3BONA X= 210 Y= 158 W= 12 H= 16 STYLE= BS_AUTORADIOBUTTON | WS_CHILD | WS_VISIBLE | WS_TABSTOP
	*/
	strcpy (Res[29].name, "CONTROL");
	strcpy (Res[29].ctlclass,  "BUTTON");
	strcpy (Res[29].caption, "");
	Res[29].param_int[1] =  210;
	Res[29].param_int[2] =  158;
	Res[29].param_int[3] =  12;
	Res[29].param_int[4] =  16;
	Res[29].param_int[5] =  BS_AUTORADIOBUTTON | WS_CHILD | WS_VISIBLE | WS_TABSTOP;

	/*
	EDITTEXT ID_TRESPONDO1, 59, 156, 64, 16, ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP
	EDITTEXT ("EDIT") "" ID=ID_TRESPONDO1 X= 59 Y= 156 W= 64 H= 16 STYLE= ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL
	*/
	strcpy (Res[30].name, "EDITTEXT");
	strcpy (Res[30].ctlclass, "EDIT");
	strcpy (Res[30].caption, "");
	Res[30].param_int[1] =  59;
	Res[30].param_int[2] =  156;
	Res[30].param_int[3] =  64;
	Res[30].param_int[4] =  16;
	Res[30].param_int[5] =  ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL;

	/*
	EDITTEXT ID_TRESPONDO2, 142, 157, 64, 16, ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP
	EDITTEXT ("EDIT") "" ID=ID_TRESPONDO2 X= 142 Y= 157 W= 64 H= 16 STYLE= ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL
	*/
	strcpy (Res[31].name, "EDITTEXT");
	strcpy (Res[31].ctlclass, "EDIT");
	strcpy (Res[31].caption, "");
	Res[31].param_int[1] =  142;
	Res[31].param_int[2] =  157;
	Res[31].param_int[3] =  64;
	Res[31].param_int[4] =  16;
	Res[31].param_int[5] =  ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL;

	/*
	EDITTEXT ID_TRESPONDO3, 221, 157, 69, 17, ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP
	EDITTEXT ("EDIT") "" ID=ID_TRESPONDO3 X= 221 Y= 157 W= 69 H= 17 STYLE= ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL
	*/
	strcpy (Res[32].name, "EDITTEXT");
	strcpy (Res[32].ctlclass, "EDIT");
	strcpy (Res[32].caption, "");
	Res[32].param_int[1] =  221;
	Res[32].param_int[2] =  157;
	Res[32].param_int[3] =  69;
	Res[32].param_int[4] =  17;
	Res[32].param_int[5] =  ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL;

	/*
	LTEXT "ND   NR", -1, 213, 85, 29, 8, WS_CHILD | WS_VISIBLE | WS_GROUP
	LTEXT ("STATIC") "ND   NR" ID= -1 X= 213 Y= 85 W= 29 H= 8 STYLE= WS_CHILD | WS_VISIBLE | WS_GROUP
	*/
	strcpy (Res[33].name, "LTEXT");
	strcpy (Res[33].ctlclass, "STATIC");
	strcpy (Res[33].caption, "ND   NR");
	Res[33].param_int[1] =  213;
	Res[33].param_int[2] =  85;
	Res[33].param_int[3] =  29;
	Res[33].param_int[4] =  8;
	Res[33].param_int[5] =  WS_CHILD | WS_VISIBLE | WS_GROUP;

	/*
	LTEXT "TD", -1, 195, 96, 16, 8, WS_CHILD | WS_VISIBLE | WS_GROUP
	LTEXT ("STATIC") "TD" ID= -1 X= 195 Y= 96 W= 16 H= 8 STYLE= WS_CHILD | WS_VISIBLE | WS_GROUP
	*/
	strcpy (Res[34].name, "LTEXT");
	strcpy (Res[34].ctlclass, "STATIC");
	strcpy (Res[34].caption, "TD");
	Res[34].param_int[1] =  195;
	Res[34].param_int[2] =  96;
	Res[34].param_int[3] =  16;
	Res[34].param_int[4] =  8;
	Res[34].param_int[5] =  WS_CHILD | WS_VISIBLE | WS_GROUP;

	/*
	LTEXT "TR", -1, 194, 110, 16, 8, WS_CHILD | WS_VISIBLE | WS_GROUP
	LTEXT ("STATIC") "TR" ID= -1 X= 194 Y= 110 W= 16 H= 8 STYLE= WS_CHILD | WS_VISIBLE | WS_GROUP
	*/
	strcpy (Res[35].name, "LTEXT");
	strcpy (Res[35].ctlclass, "STATIC");
	strcpy (Res[35].caption, "TR");
	Res[35].param_int[1] =  194;
	Res[35].param_int[2] =  110;
	Res[35].param_int[3] =  16;
	Res[35].param_int[4] =  8;
	Res[35].param_int[5] =  WS_CHILD | WS_VISIBLE | WS_GROUP;

	/*
	PUSHBUTTON "&Legu", ID_LEGU, 160, 19, 24, 14, WS_CHILD | WS_VISIBLE | WS_TABSTOP
	PUSHBUTTON ("BUTTON") "&Legu" ID= ID_LEGU X= 160 Y= 19 W= 24 H= 14 STYLE= WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON
	*/
	strcpy (Res[36].name, "PUSHBUTTON");
	strcpy (Res[36].ctlclass, "BUTTON");
	strcpy (Res[36].caption, "&Legu");
	Res[36].param_int[1] =  160;
	Res[36].param_int[2] =  19;
	Res[36].param_int[3] =  24;
	Res[36].param_int[4] =  14;
	Res[36].param_int[5] =  WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON;

	/*
	EDITTEXT ID_SNDTD, 210, 94, 16, 12, ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP
	EDITTEXT ("EDIT") "" ID=ID_SNDTD X= 210 Y= 94 W= 16 H= 12 STYLE= ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL
	*/
	strcpy (Res[37].name, "EDITTEXT");
	strcpy (Res[37].ctlclass, "EDIT");
	strcpy (Res[37].caption, "");
	Res[37].param_int[1] =  210;
	Res[37].param_int[2] =  94;
	Res[37].param_int[3] =  16;
	Res[37].param_int[4] =  12;
	Res[37].param_int[5] =  ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL;

	/*
	EDITTEXT ID_SNRTD, 228, 94, 16, 12, ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP
	EDITTEXT ("EDIT") "" ID=ID_SNRTD X= 228 Y= 94 W= 16 H= 12 STYLE= ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL
	*/
	strcpy (Res[38].name, "EDITTEXT");
	strcpy (Res[38].ctlclass, "EDIT");
	strcpy (Res[38].caption, "");
	Res[38].param_int[1] =  228;
	Res[38].param_int[2] =  94;
	Res[38].param_int[3] =  16;
	Res[38].param_int[4] =  12;
	Res[38].param_int[5] =  ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL;

	/*
	EDITTEXT ID_SNDTR, 210, 107, 16, 12, ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP
	EDITTEXT ("EDIT") "" ID=ID_SNDTR X= 210 Y= 107 W= 16 H= 12 STYLE= ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL
	*/
	strcpy (Res[39].name, "EDITTEXT");
	strcpy (Res[39].ctlclass, "EDIT");
	strcpy (Res[39].caption, "");
	Res[39].param_int[1] =  210;
	Res[39].param_int[2] =  107;
	Res[39].param_int[3] =  16;
	Res[39].param_int[4] =  12;
	Res[39].param_int[5] =  ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL;

	/*
	EDITTEXT ID_SNRTR, 228, 107, 16, 12, ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP
	EDITTEXT ("EDIT") "" ID=ID_SNRTR X= 228 Y= 107 W= 16 H= 12 STYLE= ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL
	*/
	strcpy (Res[40].name, "EDITTEXT");
	strcpy (Res[40].ctlclass, "EDIT");
	strcpy (Res[40].caption, "");
	Res[40].param_int[1] =  228;
	Res[40].param_int[2] =  107;
	Res[40].param_int[3] =  16;
	Res[40].param_int[4] =  12;
	Res[40].param_int[5] =  ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL;

	/*
	PUSHBUTTON "&Helpu", ID_HELPU, 258, 4, 33, 13, WS_CHILD | WS_VISIBLE | WS_TABSTOP
	PUSHBUTTON ("BUTTON") "&Helpu" ID= ID_HELPU X= 258 Y= 4 W= 33 H= 13 STYLE= WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON
	*/
	strcpy (Res[41].name, "PUSHBUTTON");
	strcpy (Res[41].ctlclass, "BUTTON");
	strcpy (Res[41].caption, "&Helpu");
	Res[41].param_int[1] =  258;
	Res[41].param_int[2] =  4;
	Res[41].param_int[3] =  33;
	Res[41].param_int[4] =  13;
	Res[41].param_int[5] =  WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON;

	/*
	PUSHBUTTON "&Kreu liston", ID_KREULISTON, 211, 3, 38, 14, WS_CHILD | WS_VISIBLE | WS_TABSTOP
	PUSHBUTTON ("BUTTON") "&Kreu liston" ID= ID_KREULISTON X= 211 Y= 3 W= 38 H= 14 STYLE= WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON
	*/
	strcpy (Res[42].name, "PUSHBUTTON");
	strcpy (Res[42].ctlclass, "BUTTON");
	strcpy (Res[42].caption, "&Kreu liston");
	Res[42].param_int[1] =  211;
	Res[42].param_int[2] =  3;
	Res[42].param_int[3] =  38;
	Res[42].param_int[4] =  14;
	Res[42].param_int[5] =  WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON;

	/*
	PUSHBUTTON "Legu l&iston", ID_LEGULISTON, 168, 3, 40, 14, WS_CHILD | WS_VISIBLE | WS_TABSTOP
	PUSHBUTTON ("BUTTON") "Legu l&iston" ID= ID_LEGULISTON X= 168 Y= 3 W= 40 H= 14 STYLE= WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON
	*/
	strcpy (Res[43].name, "PUSHBUTTON");
	strcpy (Res[43].ctlclass, "BUTTON");
	strcpy (Res[43].caption, "Legu l&iston");
	Res[43].param_int[1] =  168;
	Res[43].param_int[2] =  3;
	Res[43].param_int[3] =  40;
	Res[43].param_int[4] =  14;
	Res[43].param_int[5] =  WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON;

	/*
	EDITTEXT ID_DISKSTOKEJO, 48, 2, 80, 15, ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP
	EDITTEXT ("EDIT") "" ID=ID_DISKSTOKEJO X= 48 Y= 2 W= 80 H= 15 STYLE= ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL
	*/
	strcpy (Res[44].name, "EDITTEXT");
	strcpy (Res[44].ctlclass, "EDIT");
	strcpy (Res[44].caption, "");
	Res[44].param_int[1] =  48;
	Res[44].param_int[2] =  2;
	Res[44].param_int[3] =  80;
	Res[44].param_int[4] =  15;
	Res[44].param_int[5] =  ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL;

	/*
	PUSHBUTTON "Shan&gu", ID_SHANGHU, 132, 3, 32, 14, WS_CHILD | WS_VISIBLE | WS_TABSTOP
	PUSHBUTTON ("BUTTON") "Shan&gu" ID= ID_SHANGHU X= 132 Y= 3 W= 32 H= 14 STYLE= WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON
	*/
	strcpy (Res[45].name, "PUSHBUTTON");
	strcpy (Res[45].ctlclass, "BUTTON");
	strcpy (Res[45].caption, "Shan&gu");
	Res[45].param_int[1] =  132;
	Res[45].param_int[2] =  3;
	Res[45].param_int[3] =  32;
	Res[45].param_int[4] =  14;
	Res[45].param_int[5] =  WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON;

	/*
	CONTROL "Am.fv.", AUTOFORVISHO, "BUTTON", BS_AUTOCHECKBOX | WS_CHILD | WS_VISIBLE | WS_TABSTOP, 223, 20, 30, 12
	CONTROL ( "BUTTON") "Am.fv." ID= AUTOFORVISHO X= 223 Y= 20 W= 30 H= 12 STYLE= BS_AUTOCHECKBOX | WS_CHILD | WS_VISIBLE | WS_TABSTOP
	*/
	strcpy (Res[46].name, "CONTROL");
	strcpy (Res[46].ctlclass,  "BUTTON");
	strcpy (Res[46].caption, "Am.fv.");
	Res[46].param_int[1] =  223;
	Res[46].param_int[2] =  20;
	Res[46].param_int[3] =  30;
	Res[46].param_int[4] =  12;
	Res[46].param_int[5] =  BS_AUTOCHECKBOX | WS_CHILD | WS_VISIBLE | WS_TABSTOP;

	/*
	PUSHBUTTON "List&o", 112, 254, 89, 24, 15, WS_CHILD | WS_VISIBLE | WS_TABSTOP
	PUSHBUTTON ("BUTTON") "List&o" ID= 112 X= 254 Y= 89 W= 24 H= 15 STYLE= WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON
	*/
	strcpy (Res[47].name, "PUSHBUTTON");
	strcpy (Res[47].ctlclass, "BUTTON");
	strcpy (Res[47].caption, "List&o");
	Res[47].param_int[1] =  254;
	Res[47].param_int[2] =  89;
	Res[47].param_int[3] =  24;
	Res[47].param_int[4] =  15;
	Res[47].param_int[5] =  WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON;
 
#define NCTL 47


	return 0;
}

#define  ID_SUMO             (COMMAND0 + 2)
#define ID_KATEG             (COMMAND0 + 3)
#define ID_NOMBRO            (COMMAND0 + 5)
#define ID_DEMANDO           (COMMAND0 + 7)
#define  ID_RESP1BONA        (COMMAND0 + 9)
#define  ID_RESP2BONA        (COMMAND0 + 10)
#define  ID_RESP3BONA        (COMMAND0 + 11)
#define ID_RESPONDO1         (COMMAND0 + 12)
#define ID_RESPONDO2         (COMMAND0 + 13)
#define ID_RESPONDO3         (COMMAND0 + 14)
#define  ID_REGISTRU         (COMMAND0 + 15)
#define  ID_SERCHU           (COMMAND0 + 16)
#define  ID_FORVISHU         (COMMAND0 + 17)
#define  ID_SEKVANTA         (COMMAND0 + 18)
#define  ID_ANTAUA           (COMMAND0 + 19)
#define ID_TKATEG            (COMMAND0 + 21)
#define ID_TNOMBRO           (COMMAND0 + 23)
#define ID_TDEMANDO          (COMMAND0 + 25)
#define  ID_TRESP1BONA       (COMMAND0 + 27)
#define  ID_TRESP2BONA       (COMMAND0 + 28)
#define  ID_TRESP3BONA       (COMMAND0 + 29)
#define ID_TRESPONDO1        (COMMAND0 + 30)
#define ID_TRESPONDO2        (COMMAND0 + 31)
#define ID_TRESPONDO3        (COMMAND0 + 32)
#define  ID_LEGU             (COMMAND0 + 36)
#define ID_SNDTD             (COMMAND0 + 37)
#define ID_SNRTD             (COMMAND0 + 38)
#define ID_SNDTR             (COMMAND0 + 39)
#define ID_SNRTR             (COMMAND0 + 40)
#define  ID_HELPU            (COMMAND0 + 41)
#define  ID_KREULISTON       (COMMAND0 + 42)
#define  ID_LEGULISTON       (COMMAND0 + 43)
#define ID_DISKSTOKEJO       (COMMAND0 + 44)
#define  ID_SHANGHU          (COMMAND0 + 45)
#define  ID_AUTOFORVISHO        (COMMAND0 + 46)
#define ID_LISTO			 (COMMAND0+47)

void InitWnd (void);


class TAppliWindow : public TWindow
{
public:
	/* declare control objects PT... */
	/* PTButton Button1; */
    PTWindowsObject pWndCtlObj[NCTL];
	HWND hWndCtlObj[NCTL];
 
	TAppliWindow (PTWindowsObject parent, LPSTR title) :
		TWindow (parent, title)
	{
		EnableKBHandler ();
        /* create control objects PT... with new T... */ 
		/* Button1 = new TButton (this, 101, "&Test 1", 50, 80, 40, 20, FALSE); */
	}

	LPSTR GetClassName ()
	{
    	return szAppName;
	}

	virtual BOOL Create ()
	{
	BOOL status;
#ifdef TRACE
    char mes[80];
		sprintf (mes, "HWindow = %d", HWindow);
		MessageBox (HWindow, mes, "Test", MB_OK);
#endif
		/* hWnd = HWindow; */
		status = TWindow::Create ();
#ifdef TRACE
		sprintf (mes, "HWindow = %d", HWindow);
		MessageBox (HWindow, mes, "Test", MB_OK);
#endif
		hWnd = HWindow;
        /* create objects hWndCtlObj[i] with CreateWindow */ 
 		return status;
	}

	void LeguSekvanta (int n);
	/* void LeguAntaua (); */

#if 0
	virtual void WMChar (RTMessage Msg)
	{
		switch (Msg.WParam)
		{
			case VK_NEXT:
				LeguSekvanta (1);
				break;
			case VK_PRIOR:
				LeguSekvanta (-1);
				break;
			default:
				/*TWindow::WMChar (Msg)*/;
        }
	}
#endif
	/* void HandleWndCtli (RTMessage Msg) = [ID_FIRST + COMMAND0 + i] { ... } */

	virtual void Registru (RTMessage Msg) = [ID_FIRST + ID_REGISTRU];
/*    virtual void Almetu (RTMessage Msg) = [ID_FIRST + ID_ALMETU]; */
	virtual void Serchu (RTMessage Msg) = [ID_FIRST + ID_SERCHU];
	virtual void Antaua (RTMessage Msg) = [ID_FIRST + ID_ANTAUA];
	virtual void Sekvanta (RTMessage Msg) = [ID_FIRST + ID_SEKVANTA];
	virtual void Legu (RTMessage Msg) = [ID_FIRST + ID_LEGU];
	virtual void Forvishu (RTMessage Msg) = [ID_FIRST + ID_FORVISHU];
	virtual void Helpu (RTMessage Msg) = [ID_FIRST + ID_HELPU];
	virtual void KreuListon (RTMessage Msg) = [ID_FIRST + ID_KREULISTON];
    virtual void LeguListon (RTMessage Msg) = [ID_FIRST + ID_LEGULISTON];
	virtual void Shanghu (RTMessage Msg) = [ID_FIRST + ID_SHANGHU];
	virtual void Listo (RTMessage Msg) = [ID_FIRST + ID_LISTO];

	int Malfermu ();
	void PleniguRegistreron ();
	void RegistruRegistreron ();
	void AlmetuRegistreron ();
	void Serchu1 ();
    void VidebliguDemandon ();
	void VidebliguTrovitanDemandon ();
	void NombruDemandojn ();
	void VidebliguNombron ();
	int SerchuRegistreron (int legu);
	void Analizu (char *);
    int LeguRegistreron (struct registrero *r);
/*
  PTEdit Edit_PremierNombre;
  int PremierNombre;
  PTEdit Edit_DeuxiemeNombre;
  int DeuxiemeNombre;
  PTCheckBox CheckBox_Hexa;
  int Hexa;
  int op;
  PTEdit Edit_Resultat;
  int Resultat;
*/
/*
	PTEdit Edit_Kateg, Edit_Demando, Edit_Respondo[3];
    PTGroupBox Group_RespBona;
	PTRadioButton Radio_RespBona[3];
*/
#ifdef STDIO
	FILE *f;
#else
	int fd;
#endif
	struct registrero r;
	struct simil simil[N_SIMIL];
	int vs;
    int nd;

};

TAppliWindow *Wnd;

#define TTrivialDialog TAppliWindow

	/*
	LTEXT "Kategorio", -1, 5, 23, 37, 10, WS_CHILD | WS_VISIBLE | WS_GROUP
	LTEXT ("STATIC") "Kategorio" ID= -1 X= 5 Y= 23 W= 37 H= 10 STYLE= WS_CHILD | WS_VISIBLE | WS_GROUP
	*/
void HandleCtl1 (TMessage *pmsg)
{
/* Add code here */

}

	/*
	LTEXT "Diskstokejo", ID_SUMO, 7, 6, 38, 10, WS_CHILD | WS_VISIBLE | WS_GROUP
	LTEXT ("STATIC") "Diskstokejo" ID= ID_SUMO X= 7 Y= 6 W= 38 H= 10 STYLE= WS_CHILD | WS_VISIBLE | WS_GROUP
	*/
void HandleCtlID_SUMO (TMessage *pmsg)
{
/* Add code here */

}

	/*
	EDITTEXT ID_KATEG, 43, 21, 47, 14, ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP
	EDITTEXT ("EDIT") "" ID=ID_KATEG X= 43 Y= 21 W= 47 H= 14 STYLE= ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL
	*/
void HandleCtlID_KATEG (TMessage *pmsg)
{
/* Add code here */

}

	/*
	LTEXT "Nombro", -1, 95, 24, 30, 9, WS_CHILD | WS_VISIBLE | WS_GROUP
	LTEXT ("STATIC") "Nombro" ID= -1 X= 95 Y= 24 W= 30 H= 9 STYLE= WS_CHILD | WS_VISIBLE | WS_GROUP
	*/
void HandleCtl4 (TMessage *pmsg)
{
/* Add code here */

}

	/*
	EDITTEXT ID_NOMBRO, 126, 20, 27, 15, ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP
	EDITTEXT ("EDIT") "" ID=ID_NOMBRO X= 126 Y= 20 W= 27 H= 15 STYLE= ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL
	*/
void HandleCtlID_NOMBRO (TMessage *pmsg)
{
/* Add code here */

}

	/*
	LTEXT "Nova demando", -1, 3, 43, 38, 17, WS_CHILD | WS_VISIBLE | WS_GROUP
	LTEXT ("STATIC") "Nova demando" ID= -1 X= 3 Y= 43 W= 38 H= 17 STYLE= WS_CHILD | WS_VISIBLE | WS_GROUP
	*/
void HandleCtl6 (TMessage *pmsg)
{
/* Add code here */

}

	/*
	EDITTEXT ID_DEMANDO, 43, 40, 247, 17, ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP
	EDITTEXT ("EDIT") "" ID=ID_DEMANDO X= 43 Y= 40 W= 247 H= 17 STYLE= ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL
	*/
void HandleCtlID_DEMANDO (TMessage *pmsg)
{
/* Add code here */

}

	/*
	LTEXT "Respondoj", -1, 3, 64, 36, 11, WS_CHILD | WS_VISIBLE | WS_GROUP
	LTEXT ("STATIC") "Respondoj" ID= -1 X= 3 Y= 64 W= 36 H= 11 STYLE= WS_CHILD | WS_VISIBLE | WS_GROUP
	*/
void HandleCtl8 (TMessage *pmsg)
{
/* Add code here */

}

	/*
	CONTROL "", ID_RESP1BONA, "BUTTON", BS_AUTORADIOBUTTON | WS_CHILD | WS_VISIBLE | WS_TABSTOP, 43, 65, 9, 12
	CONTROL ( "BUTTON") "" ID= ID_RESP1BONA X= 43 Y= 65 W= 9 H= 12 STYLE= BS_AUTORADIOBUTTON | WS_CHILD | WS_VISIBLE | WS_TABSTOP
	*/
void HandleCtlID_RESP1BONA (TMessage *pmsg)
{
/* Add code here */

}

	/*
	CONTROL "", ID_RESP2BONA, "BUTTON", BS_AUTORADIOBUTTON | WS_CHILD | WS_VISIBLE | WS_TABSTOP, 129, 66, 8, 12
	CONTROL ( "BUTTON") "" ID= ID_RESP2BONA X= 129 Y= 66 W= 8 H= 12 STYLE= BS_AUTORADIOBUTTON | WS_CHILD | WS_VISIBLE | WS_TABSTOP
	*/
void HandleCtlID_RESP2BONA (TMessage *pmsg)
{
/* Add code here */

}

	/*
	CONTROL "", ID_RESP3BONA, "BUTTON", BS_AUTORADIOBUTTON | WS_CHILD | WS_VISIBLE | WS_TABSTOP, 209, 64, 8, 16
	CONTROL ( "BUTTON") "" ID= ID_RESP3BONA X= 209 Y= 64 W= 8 H= 16 STYLE= BS_AUTORADIOBUTTON | WS_CHILD | WS_VISIBLE | WS_TABSTOP
	*/
void HandleCtlID_RESP3BONA (TMessage *pmsg)
{
/* Add code here */

}

	/*
	EDITTEXT ID_RESPONDO1, 55, 64, 69, 16, ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP
	EDITTEXT ("EDIT") "" ID=ID_RESPONDO1 X= 55 Y= 64 W= 69 H= 16 STYLE= ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL
	*/
void HandleCtlID_RESPONDO1 (TMessage *pmsg)
{
/* Add code here */

}

	/*
	EDITTEXT ID_RESPONDO2, 141, 64, 64, 16, ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP
	EDITTEXT ("EDIT") "" ID=ID_RESPONDO2 X= 141 Y= 64 W= 64 H= 16 STYLE= ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL
	*/
void HandleCtlID_RESPONDO2 (TMessage *pmsg)
{
/* Add code here */

}

	/*
	EDITTEXT ID_RESPONDO3, 220, 64, 70, 16, ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP
	EDITTEXT ("EDIT") "" ID=ID_RESPONDO3 X= 220 Y= 64 W= 70 H= 16 STYLE= ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL
	*/
void HandleCtlID_RESPONDO3 (TMessage *pmsg)
{
/* Add code here */

}

	/*
	PUSHBUTTON "&Registru", ID_REGISTRU, 188, 19, 33, 15, WS_CHILD | WS_VISIBLE | WS_TABSTOP
	PUSHBUTTON ("BUTTON") "&Registru" ID= ID_REGISTRU X= 188 Y= 19 W= 33 H= 15 STYLE= WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON
	*/
void HandleCtlID_REGISTRU (TMessage *pmsg)
{
/* Add code here */
	Wnd->Registru (*pmsg); 
}

	/*
	PUSHBUTTON "&Serchu", ID_SERCHU, 21, 88, 42, 13, WS_CHILD | WS_VISIBLE | WS_TABSTOP
	PUSHBUTTON ("BUTTON") "&Serchu" ID= ID_SERCHU X= 21 Y= 88 W= 42 H= 13 STYLE= WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON
	*/
void HandleCtlID_SERCHU (TMessage *pmsg)
{
/* Add code here */
	Wnd->Serchu (*pmsg);
}

	/*
	PUSHBUTTON "&Forvishu", ID_FORVISHU, 260, 19, 33, 14, WS_CHILD | WS_VISIBLE | WS_TABSTOP
	PUSHBUTTON ("BUTTON") "&Forvishu" ID= ID_FORVISHU X= 260 Y= 19 W= 33 H= 14 STYLE= WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON
	*/
void HandleCtlID_FORVISHU (TMessage *pmsg)
{
/* Add code here */
	Wnd->Forvishu (*pmsg);
}

	/*
	PUSHBUTTON "Sekva&nta", ID_SEKVANTA, 139, 87, 38, 16, WS_CHILD | WS_VISIBLE | WS_TABSTOP
	PUSHBUTTON ("BUTTON") "Sekva&nta" ID= ID_SEKVANTA X= 139 Y= 87 W= 38 H= 16 STYLE= WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON
	*/
void HandleCtlID_SEKVANTA (TMessage *pmsg)
{
/* Add code here */
	Wnd->Sekvanta (*pmsg);
}

	/*
	PUSHBUTTON "An&taùa", ID_ANTAUA, 78, 88, 46, 14, WS_CHILD | WS_VISIBLE | WS_TABSTOP
	PUSHBUTTON ("BUTTON") "An&taùa" ID= ID_ANTAUA X= 78 Y= 88 W= 46 H= 14 STYLE= WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON
	*/
void HandleCtlID_ANTAUA (TMessage *pmsg)
{
/* Add code here */
	Wnd->Antaua (*pmsg);
}

	/*
	LTEXT "Kategorio", -1, 7, 115, 37, 10, WS_CHILD | WS_VISIBLE | WS_GROUP
	LTEXT ("STATIC") "Kategorio" ID= -1 X= 7 Y= 115 W= 37 H= 10 STYLE= WS_CHILD | WS_VISIBLE | WS_GROUP
	*/
void HandleCtl20 (TMessage *pmsg)
{
/* Add code here */

}

	/*
	EDITTEXT ID_TKATEG, 46, 113, 55, 14, ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP
	EDITTEXT ("EDIT") "" ID=ID_TKATEG X= 46 Y= 113 W= 55 H= 14 STYLE= ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL
	*/
void HandleCtlID_TKATEG (TMessage *pmsg)
{
/* Add code here */

}

	/*
	LTEXT "Nombro", -1, 117, 116, 30, 9, WS_CHILD | WS_VISIBLE | WS_GROUP
	LTEXT ("STATIC") "Nombro" ID= -1 X= 117 Y= 116 W= 30 H= 9 STYLE= WS_CHILD | WS_VISIBLE | WS_GROUP
	*/
void HandleCtl22 (TMessage *pmsg)
{
/* Add code here */

}

	/*
	EDITTEXT ID_TNOMBRO, 150, 112, 28, 15, ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP
	EDITTEXT ("EDIT") "" ID=ID_TNOMBRO X= 150 Y= 112 W= 28 H= 15 STYLE= ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL
	*/
void HandleCtlID_TNOMBRO (TMessage *pmsg)
{
/* Add code here */

}

	/*
	LTEXT "Trovita demando", -1, 5, 135, 38, 17, WS_CHILD | WS_VISIBLE | WS_GROUP
	LTEXT ("STATIC") "Trovita demando" ID= -1 X= 5 Y= 135 W= 38 H= 17 STYLE= WS_CHILD | WS_VISIBLE | WS_GROUP
	*/
void HandleCtl24 (TMessage *pmsg)
{
/* Add code here */

}

	/*
	EDITTEXT ID_TDEMANDO, 45, 132, 246, 17, ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP
	EDITTEXT ("EDIT") "" ID=ID_TDEMANDO X= 45 Y= 132 W= 246 H= 17 STYLE= ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL
	*/
void HandleCtlID_TDEMANDO (TMessage *pmsg)
{
/* Add code here */

}

	/*
	LTEXT "Respondoj", -1, 5, 156, 36, 11, WS_CHILD | WS_VISIBLE | WS_GROUP
	LTEXT ("STATIC") "Respondoj" ID= -1 X= 5 Y= 156 W= 36 H= 11 STYLE= WS_CHILD | WS_VISIBLE | WS_GROUP
	*/
void HandleCtl26 (TMessage *pmsg)
{
/* Add code here */

}

	/*
	CONTROL "", ID_TRESP1BONA, "BUTTON", BS_AUTORADIOBUTTON | WS_CHILD | WS_VISIBLE | WS_TABSTOP, 46, 157, 13, 12
	CONTROL ( "BUTTON") "" ID= ID_TRESP1BONA X= 46 Y= 157 W= 13 H= 12 STYLE= BS_AUTORADIOBUTTON | WS_CHILD | WS_VISIBLE | WS_TABSTOP
	*/
void HandleCtlID_TRESP1BONA (TMessage *pmsg)
{
/* Add code here */

}

	/*
	CONTROL "", ID_TRESP2BONA, "BUTTON", BS_AUTORADIOBUTTON | WS_CHILD | WS_VISIBLE | WS_TABSTOP, 130, 159, 12, 12
	CONTROL ( "BUTTON") "" ID= ID_TRESP2BONA X= 130 Y= 159 W= 12 H= 12 STYLE= BS_AUTORADIOBUTTON | WS_CHILD | WS_VISIBLE | WS_TABSTOP
	*/
void HandleCtlID_TRESP2BONA (TMessage *pmsg)
{
/* Add code here */

}

	/*
	CONTROL "", ID_TRESP3BONA, "BUTTON", BS_AUTORADIOBUTTON | WS_CHILD | WS_VISIBLE | WS_TABSTOP, 210, 158, 12, 16
	CONTROL ( "BUTTON") "" ID= ID_TRESP3BONA X= 210 Y= 158 W= 12 H= 16 STYLE= BS_AUTORADIOBUTTON | WS_CHILD | WS_VISIBLE | WS_TABSTOP
	*/
void HandleCtlID_TRESP3BONA (TMessage *pmsg)
{
/* Add code here */

}

	/*
	EDITTEXT ID_TRESPONDO1, 59, 156, 64, 16, ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP
	EDITTEXT ("EDIT") "" ID=ID_TRESPONDO1 X= 59 Y= 156 W= 64 H= 16 STYLE= ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL
	*/
void HandleCtlID_TRESPONDO1 (TMessage *pmsg)
{
/* Add code here */

}

	/*
	EDITTEXT ID_TRESPONDO2, 142, 157, 64, 16, ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP
	EDITTEXT ("EDIT") "" ID=ID_TRESPONDO2 X= 142 Y= 157 W= 64 H= 16 STYLE= ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL
	*/
void HandleCtlID_TRESPONDO2 (TMessage *pmsg)
{
/* Add code here */

}

	/*
	EDITTEXT ID_TRESPONDO3, 221, 157, 69, 17, ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP
	EDITTEXT ("EDIT") "" ID=ID_TRESPONDO3 X= 221 Y= 157 W= 69 H= 17 STYLE= ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL
	*/
void HandleCtlID_TRESPONDO3 (TMessage *pmsg)
{
/* Add code here */

}

	/*
	LTEXT "ND   NR", -1, 213, 85, 29, 8, WS_CHILD | WS_VISIBLE | WS_GROUP
	LTEXT ("STATIC") "ND   NR" ID= -1 X= 213 Y= 85 W= 29 H= 8 STYLE= WS_CHILD | WS_VISIBLE | WS_GROUP
	*/
void HandleCtl33 (TMessage *pmsg)
{
/* Add code here */

}

	/*
	LTEXT "TD", -1, 195, 96, 16, 8, WS_CHILD | WS_VISIBLE | WS_GROUP
	LTEXT ("STATIC") "TD" ID= -1 X= 195 Y= 96 W= 16 H= 8 STYLE= WS_CHILD | WS_VISIBLE | WS_GROUP
	*/
void HandleCtl34 (TMessage *pmsg)
{
/* Add code here */

}

	/*
	LTEXT "TR", -1, 194, 110, 16, 8, WS_CHILD | WS_VISIBLE | WS_GROUP
	LTEXT ("STATIC") "TR" ID= -1 X= 194 Y= 110 W= 16 H= 8 STYLE= WS_CHILD | WS_VISIBLE | WS_GROUP
	*/
void HandleCtl35 (TMessage *pmsg)
{
/* Add code here */

}

	/*
	PUSHBUTTON "&Legu", ID_LEGU, 160, 19, 24, 14, WS_CHILD | WS_VISIBLE | WS_TABSTOP
	PUSHBUTTON ("BUTTON") "&Legu" ID= ID_LEGU X= 160 Y= 19 W= 24 H= 14 STYLE= WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON
	*/
void HandleCtlID_LEGU (TMessage *pmsg)
{
/* Add code here */
	Wnd->Legu (*pmsg);
}

	/*
	EDITTEXT ID_SNDTD, 210, 94, 16, 12, ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP
	EDITTEXT ("EDIT") "" ID=ID_SNDTD X= 210 Y= 94 W= 16 H= 12 STYLE= ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL
	*/
void HandleCtlID_SNDTD (TMessage *pmsg)
{
/* Add code here */

}

	/*
	EDITTEXT ID_SNRTD, 228, 94, 16, 12, ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP
	EDITTEXT ("EDIT") "" ID=ID_SNRTD X= 228 Y= 94 W= 16 H= 12 STYLE= ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL
	*/
void HandleCtlID_SNRTD (TMessage *pmsg)
{
/* Add code here */

}

	/*
	EDITTEXT ID_SNDTR, 210, 107, 16, 12, ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP
	EDITTEXT ("EDIT") "" ID=ID_SNDTR X= 210 Y= 107 W= 16 H= 12 STYLE= ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL
	*/
void HandleCtlID_SNDTR (TMessage *pmsg)
{
/* Add code here */

}

	/*
	EDITTEXT ID_SNRTR, 228, 107, 16, 12, ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP
	EDITTEXT ("EDIT") "" ID=ID_SNRTR X= 228 Y= 107 W= 16 H= 12 STYLE= ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL
	*/
void HandleCtlID_SNRTR (TMessage *pmsg)
{
/* Add code here */

}

	/*
	PUSHBUTTON "&Helpu", ID_HELPU, 258, 4, 33, 13, WS_CHILD | WS_VISIBLE | WS_TABSTOP
	PUSHBUTTON ("BUTTON") "&Helpu" ID= ID_HELPU X= 258 Y= 4 W= 33 H= 13 STYLE= WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON
	*/
void HandleCtlID_HELPU (TMessage *pmsg)
{
/* Add code here */
	Wnd->Helpu (*pmsg);
}

	/*
	PUSHBUTTON "&Kreu liston", ID_KREULISTON, 211, 3, 38, 14, WS_CHILD | WS_VISIBLE | WS_TABSTOP
	PUSHBUTTON ("BUTTON") "&Kreu liston" ID= ID_KREULISTON X= 211 Y= 3 W= 38 H= 14 STYLE= WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON
	*/
void HandleCtlID_KREULISTON (TMessage *pmsg)
{
/* Add code here */
	Wnd->KreuListon (*pmsg);
}

	/*
	PUSHBUTTON "Legu l&iston", ID_LEGULISTON, 168, 3, 40, 14, WS_CHILD | WS_VISIBLE | WS_TABSTOP
	PUSHBUTTON ("BUTTON") "Legu l&iston" ID= ID_LEGULISTON X= 168 Y= 3 W= 40 H= 14 STYLE= WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON
	*/
void HandleCtlID_LEGULISTON (TMessage *pmsg)
{
/* Add code here */
	Wnd->LeguListon (*pmsg);
}

	/*
	EDITTEXT ID_DISKSTOKEJO, 48, 2, 80, 15, ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP
	EDITTEXT ("EDIT") "" ID=ID_DISKSTOKEJO X= 48 Y= 2 W= 80 H= 15 STYLE= ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL
	*/
void HandleCtlID_DISKSTOKEJO (TMessage *pmsg)
{
/* Add code here */

}

	/*
	PUSHBUTTON "Shan&gu", ID_SHANGHU, 132, 3, 32, 14, WS_CHILD | WS_VISIBLE | WS_TABSTOP
	PUSHBUTTON ("BUTTON") "Shan&gu" ID= ID_SHANGHU X= 132 Y= 3 W= 32 H= 14 STYLE= WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON
	*/
void HandleCtlID_SHANGHU (TMessage *pmsg)
{
/* Add code here */
	Wnd->Shanghu (*pmsg);
}

	/*
	CONTROL "Am.fv.", ID_AUTOFORVISHO, "BUTTON", BS_AUTOCHECKBOX | WS_CHILD | WS_VISIBLE | WS_TABSTOP, 223, 20, 30, 12
	CONTROL ( "BUTTON") "Am.fv." ID= ID_AUTOFORVISHO X= 223 Y= 20 W= 30 H= 12 STYLE= BS_AUTOCHECKBOX | WS_CHILD | WS_VISIBLE | WS_TABSTOP
	*/
void HandleCtlID_AUTOFORVISHO (TMessage *pmsg)
{
/* Add code here */

}

	/*
	PUSHBUTTON "List&o", ID_LISTO, 254, 89, 24, 15, WS_CHILD | WS_VISIBLE | WS_TABSTOP
	PUSHBUTTON ("BUTTON") "List&o" ID= ID_LISTO X= 254 Y= 89 W= 24 H= 15 STYLE= WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON
	*/
void HandleCtlID_LISTO (TMessage *pmsg)
{
/* Add code here */
	Wnd->Listo (*pmsg);
}

LRESULT CALLBACK WndProc (HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
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
			Wnd->HWindow = hWnd;
        	/* create control objects hCtlObj[i] with CreateWindow */
			for (nCtl=1; nCtl<=NCTL; nCtl++)
			{
				hCtlObj[nCtl] = CreateWindow (Res[nCtl].ctlclass, Res[nCtl].caption,
					Res[nCtl].param_int[P_STYLE],
					Res[nCtl].param_int[P_X]*2, Res[nCtl].param_int[P_Y]*3/2, Res[nCtl].param_int[P_W]*2, Res[nCtl].param_int[P_H]*3/2,
					hWnd, COMMAND0+nCtl, hInst, NULL);
			}
			Focus = 1;
			SetFocus (hCtlObj[Focus]);
			InitWnd ();
			break;
		case WM_CLOSE:
			for (nCtl=1; nCtl<=NCTL; nCtl++)
				DestroyWindow (hCtlObj[nCtl]);
			return DefWindowProc(hWnd, Message, wParam, lParam);
		case WM_SIZE:
			for (nCtl=1; nCtl<=NCTL; nCtl++)
			{
				MoveWindow (hCtlObj[nCtl],
					Res[nCtl].param_int[P_X]*LOWORD(lParam)/(Wid),
					Res[nCtl].param_int[P_Y]*HIWORD(lParam)/(Hei),
					Res[nCtl].param_int[P_W]*LOWORD(lParam)/(Wid),
					Res[nCtl].param_int[P_H]*HIWORD(lParam)/(Hei),
					TRUE);
			}
 			break;
		case WM_COMMAND:
			switch (wParam)
			{
				/* case COMMAND0+i:
					HandleCtli();
					break; */

				case COMMAND0+1:
					HandleCtl1 (&msg);
					break;
				case COMMAND0+2:
					HandleCtlID_SUMO (&msg);
					break;
				case COMMAND0+3:
					HandleCtlID_KATEG (&msg);
					break;
				case COMMAND0+4:
					HandleCtl4 (&msg);
					break;
				case COMMAND0+5:
					HandleCtlID_NOMBRO (&msg);
					break;
				case COMMAND0+6:
					HandleCtl6 (&msg);
					break;
				case COMMAND0+7:
					HandleCtlID_DEMANDO (&msg);
					break;
				case COMMAND0+8:
					HandleCtl8 (&msg);
					break;
				case COMMAND0+9:
					HandleCtlID_RESP1BONA (&msg);
					break;
				case COMMAND0+10:
					HandleCtlID_RESP2BONA (&msg);
					break;
				case COMMAND0+11:
					HandleCtlID_RESP3BONA (&msg);
					break;
				case COMMAND0+12:
					HandleCtlID_RESPONDO1 (&msg);
					break;
				case COMMAND0+13:
					HandleCtlID_RESPONDO2 (&msg);
					break;
				case COMMAND0+14:
					HandleCtlID_RESPONDO3 (&msg);
					break;
				case COMMAND0+15:
					HandleCtlID_REGISTRU (&msg);
					break;
				case COMMAND0+16:
					HandleCtlID_SERCHU (&msg);
					break;
				case COMMAND0+17:
					HandleCtlID_FORVISHU (&msg);
					break;
				case COMMAND0+18:
					HandleCtlID_SEKVANTA (&msg);
					break;
				case COMMAND0+19:
					HandleCtlID_ANTAUA (&msg);
					break;
				case COMMAND0+20:
					HandleCtl20 (&msg);
					break;
				case COMMAND0+21:
					HandleCtlID_TKATEG (&msg);
					break;
				case COMMAND0+22:
					HandleCtl22 (&msg);
					break;
				case COMMAND0+23:
					HandleCtlID_TNOMBRO (&msg);
					break;
				case COMMAND0+24:
					HandleCtl24 (&msg);
					break;
				case COMMAND0+25:
					HandleCtlID_TDEMANDO (&msg);
					break;
				case COMMAND0+26:
					HandleCtl26 (&msg);
					break;
				case COMMAND0+27:
					HandleCtlID_TRESP1BONA (&msg);
					break;
				case COMMAND0+28:
					HandleCtlID_TRESP2BONA (&msg);
					break;
				case COMMAND0+29:
					HandleCtlID_TRESP3BONA (&msg);
					break;
				case COMMAND0+30:
					HandleCtlID_TRESPONDO1 (&msg);
					break;
				case COMMAND0+31:
					HandleCtlID_TRESPONDO2 (&msg);
					break;
				case COMMAND0+32:
					HandleCtlID_TRESPONDO3 (&msg);
					break;
				case COMMAND0+33:
					HandleCtl33 (&msg);
					break;
				case COMMAND0+34:
					HandleCtl34 (&msg);
					break;
				case COMMAND0+35:
					HandleCtl35 (&msg);
					break;
				case COMMAND0+36:
					HandleCtlID_LEGU (&msg);
					break;
				case COMMAND0+37:
					HandleCtlID_SNDTD (&msg);
					break;
				case COMMAND0+38:
					HandleCtlID_SNRTD (&msg);
					break;
				case COMMAND0+39:
					HandleCtlID_SNDTR (&msg);
					break;
				case COMMAND0+40:
					HandleCtlID_SNRTR (&msg);
					break;
				case COMMAND0+41:
					HandleCtlID_HELPU (&msg);
					break;
				case COMMAND0+42:
					HandleCtlID_KREULISTON (&msg);
					break;
				case COMMAND0+43:
					HandleCtlID_LEGULISTON (&msg);
					break;
				case COMMAND0+44:
					HandleCtlID_DISKSTOKEJO (&msg);
					break;
				case COMMAND0+45:
					HandleCtlID_SHANGHU (&msg);
					break;
				case COMMAND0+46:
					HandleCtlID_AUTOFORVISHO (&msg);
					break;
				case COMMAND0+47:
					HandleCtlID_LISTO (&msg);
					break;

			}
			break;
		default:
			return DefWindowProc(hWnd, Message, wParam, lParam);
	}
	return NULL;
}

void InitWnd (void)
{
/* Add code here */
	/* SetDlgItemText (HWindow, ID_DISKSTOKEJO, "C:\\TRIVIAL"); */
	/* SendMessage (hCtlObj[ID_DISKSTOKEJO-COMMAND0], WM_SETTEXT, 0, (LPARAM) (LPCSTR) "C:\\TRIV"); */ 
	SendMessage (hCtlId(ID_DISKSTOKEJO), WM_SETTEXT, 0, (LPARAM) (LPCSTR) "C:\\SUPERZAM");
	SendMessage (hCtlId(ID_SNDTD), WM_SETTEXT, 0, (LPARAM) (LPCSTR) "1");
	SendMessage (hCtlId(ID_SNDTR), WM_SETTEXT, 0, (LPARAM) (LPCSTR) "1");
	SendMessage (hCtlId(ID_SNRTD), WM_SETTEXT, 0, (LPARAM) (LPCSTR) "1");
	SendMessage (hCtlId(ID_SNRTR), WM_SETTEXT, 0, (LPARAM) (LPCSTR) "1");

	SendMessage (hCtlId(ID_AUTOFORVISHO), BM_SETCHECK, 1, 0);

	Wnd->Malfermu ();
/*
	MSG msg;
	while(GetMessage(&msg, NULL, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
*/
}

#define GetDlgItemText RedefGetDlgItemText

int GetDlgItemText (HWND hw, int id, LPSTR s, int n)
{
	SendMessage (hCtlId(id), WM_GETTEXT, n, (LPARAM)s);
}

#define SetDlgItemText RedefSetDlgItemText

int SetDlgItemText (HWND hw, int id, LPSTR s)
{
	SendMessage (hCtlId(id), WM_SETTEXT, 0, (LPARAM)s);
}

#define IsDlgButtonChecked RedefIsDlgButtonChecked

int IsDlgButtonChecked (HWND hw, int id)
{
	return SendMessage (hCtlId(id), BM_GETCHECK, 0, 0); 
}

#define CheckDlgButton RedefCheckDlgButton

int CheckDlgButton (HWND hw, int id, int state)
{
	return SendMessage (hCtlId(id), BM_SETCHECK, state, 0);
}

int TTrivialDialog::Malfermu ()
{
/* #define FILENAME "C:\\TRIVIAL.DAT" */
	char FILENAME[200];
	char diskstokejo[120];
	GetDlgItemText (HWindow, ID_DISKSTOKEJO, sized(diskstokejo));
    sprintf (FILENAME, "%s%s", diskstokejo, ".DAT"); 
#ifdef STDIO
	FILE *f1;
	f1 = fopen (FILENAME, "r+b");
	printf ("\nopen -> %d\n", f1);
	if (f1 != NULL)
	{
		fclose (f);
		f = f1;
        return 0;
	}
    else
	{
		f1 = fopen (FILENAME, "wb");
		fclose (f1);
		f1 = fopen (FILENAME, "r+b");
		if (f1 == NULL)
			/* perror ("fopen"); */
		{
		char teksto[200];
			sprintf (teksto, "Mi ne povas malfermi %s\n%s",
				FILENAME, sys_errlist[errno]);
			MessageBox (HWindow, teksto, "Problemo", MB_OK);
		}
        return errno;

	}
	
#else
	int fd1;
	fd1 = open (FILENAME, O_CREAT | O_RDWR | O_BINARY, S_IFREG | S_IREAD | S_IWRITE);
	if (fd1 != -1)
	{
		close (fd);
		fd = fd1;
        return 0;
	}
    else
	{
	char teksto[200];
		sprintf (teksto, "Mi ne povas malfermi %s\n%s",
			FILENAME, sys_errlist[errno]);
		MessageBox (HWindow, teksto, "Problemo", MB_OK);
        return errno;
    }
#endif
}

void TTrivialDialog::Shanghu (RTMessage Msg)
{
/*
#ifdef STDIO
	fclose (f);
#else
	close (fd);
#endif
*/
	Malfermu ();
}

/* #define HELPTEKSTO "Ne farita, pardonu..." */

#define HELPTEKSTO "\
Tajpu la kategorion de la registrota demando en la supra fako \"Kategorio\" \
kaj ghian tekston en la fako \"Nova demando\". Tajpu la respondojn kaj \
selektu la bonan klikante la maldekstran rondeton. \
Por registri la demandon kliku la butonon \"Registru\".\n\
Se vi volas vidi se similaj demandoj jam estas registritaj, kliku la \
butonon \"Serchu\". Jen aperas malsupre la pli similan demandon. \
Por vidi la aliajn en malpliighanta simileco ordo, kliku \"Sekvanta\".\n\
Por serchi demandojn enhavantaj vorton, tajpu la vorton en la fako\
\"Nova demando\" kaj kliku \"Serchu\".\n\
Se vi volas modifi demandon, tajpu en la supraj fakoj ghian kategorion kaj nombron kaj \
kliku \"Legu\". Modifu la tekston kaj kliku \"Registru\".\n\
Por krei liston de la registritaj demandoj kliku \"Kreu liston\". Ghi estas kreata \
en %s.LIS kaj ghin povas legi aliaj programoj."

void TTrivialDialog::Helpu (RTMessage Msg)
{
char teksto[2000];

char diskstokejo[120];
	GetDlgItemText (HWindow, ID_DISKSTOKEJO, sized(diskstokejo));

	sprintf (teksto, HELPTEKSTO, diskstokejo);
	MessageBox (HWindow, teksto, "Helpo", MB_OK);
}

void TTrivialDialog::Forvishu (RTMessage Msg)
{
int i;
	SetDlgItemText (HWindow, ID_KATEG, "");
	SetDlgItemText (HWindow, ID_NOMBRO, "");
	SetDlgItemText (HWindow, ID_DEMANDO, "");
    for (i=0; i<N_RESPONDOJ; i++)
		SetDlgItemText (HWindow, ID_RESPONDO1+i, "");

}

void TTrivialDialog::LeguSekvanta (int n)
{
#ifdef TRACE
	MessageBox (hWnd, "Sekvanta", "Test", MB_OK);
#endif
char buf[30];
int i;
TMessage Mes;
RTMessage Msg = Mes;

	SendMessage (hCtlId(ID_NOMBRO), WM_GETTEXT, sizeof(buf), (LPARAM) (LPSTR) buf);
	i = atoi(buf);
	i += n;
	sprintf (buf, "%d", i);
	SendMessage (hCtlId(ID_NOMBRO), WM_SETTEXT, 0, (LPARAM) (LPCSTR) buf);
	Legu (Msg);

}

/*
void TTrivialDialog::LeguAntaua ()
{

}
*/

traduku (char *src, char *dst)
{
int i, j;
	j = 0;
	for (i=0; src[i]; i++)
	{
		if (src[i] == '"')
			dst[j++] = src[i];
        dst[j++] = src[i];
	}
	dst[j] = 0;
}

void TTrivialDialog::KreuListon (RTMessage Msg)
{
int status;
struct registrero r1;
FILE *listo;
char buf[300];
int i;

/* #define LISTONOMO "C:\\TRIVIAL.LIS" */
char LISTONOMO[200];
char diskstokejo[120];
	GetDlgItemText (HWindow, ID_DISKSTOKEJO, sized(diskstokejo));
    sprintf (LISTONOMO, "%s%s", diskstokejo, ".LIS");

	listo = fopen (LISTONOMO, "w");
	if (listo == NULL)
	{
	char teksto[200];
		sprintf (teksto, "Mi ne povas malfermi %s\n%s",
			LISTONOMO, sys_errlist[errno]);
		MessageBox (HWindow, teksto, "Problemo", MB_OK);
		return;
	}
#ifdef STDIO
	status = fseek (f, 0, SEEK_SET);
#else
	status = lseek (fd, 0, SEEK_SET);
#endif

	for (;;)
	{
#ifdef STDIO
		status = fread (&r1, sizeof(r1), 1, f);
		if (feof(f))
#else
		status = ::read (fd, &r1, sizeof(r1));
        if (status == 0)
#endif
			break;
		traduku (r1.Kateg, buf);
		fprintf (listo, "\"%s\";%d;", buf, r1.Nombro);
		traduku (r1.Demando, buf);
		fprintf (listo, "\"%s\";%d", buf, r1.BonaResp);
		for (i=0; i<N_RESPONDOJ; i++)
		{
			traduku (r1.Respondo[i], buf);
			fprintf (listo, ";\"%s\"", buf);
		}
		fprintf (listo, "\n");
	}
    fclose (listo);
}

#define SEP ';'

char *AnalizuNbr (char *ptr, int *n)
{
char buf[30];
int i;
	/* for (i=0; ptr[i] && ptr[i]!=SEP; i++) */
    i=0;
	for (;;)
	{
		if (ptr[i] == ';')
			break;
		if (ptr[i] == 0)
        	break;
		buf[i] = ptr[i];
		i++;
	}
	buf[i] = 0;
	*n = atoi(buf);
	if (ptr[i] == ';')
		return ptr+i+1;
	return ptr+i;

}

char *AnalizuStr (char *ptr, char *str)
{
int i, j, flago;
	i = 0;
	j = 0;
    flago = 0;
	if (ptr[i] == '"')
	{
		flago = 1;
        i++;
    }
	for (;;)
	{
		if (!flago && ptr[i] == SEP)
			break;
		if (ptr[i] == 0)
			break;
		if (ptr[i] == '"')
		{
        	i++;
			if (ptr[i] == '"')
				str[j++] = ptr[i];
			else
			{
				for (; ptr[i] && ptr[i]!=SEP; i++);
				break;
            }
		}
		else
			str[j++] = ptr[i];
        i++;
	}
	str[j] = 0;
	if (ptr[i] == ';')
		return ptr+i+1;
	return ptr+i;

}

void TTrivialDialog::Analizu (char *lineo)
{
char *ptr;
int i;
int x;
	/* "Kateg";Nombro;"Demando";BonaResp;"Respondo1";"Respondo2";"Respondo3" */
    ptr = lineo;
	ptr = AnalizuStr (ptr, r.Kateg);
	ptr = AnalizuNbr (ptr, &r.Nombro);
	ptr = AnalizuStr (ptr, r.Demando);
	ptr = AnalizuNbr (ptr, &x);
	r.BonaResp = x;
	for (i=0; i<N_RESPONDOJ; i++)
	{
		ptr = AnalizuStr (ptr, r.Respondo[i]);
    }


}

void TTrivialDialog::LeguListon (RTMessage Msg)
{
FILE *listo;
char line[1000];
int status;

char LISTONOMO[200];
char diskstokejo[120];
	GetDlgItemText (HWindow, ID_DISKSTOKEJO, sized(diskstokejo));
    sprintf (LISTONOMO, "%s%s", diskstokejo, ".LIS");

	listo = fopen (LISTONOMO, "r");
	if (listo == NULL)
	{
	char teksto[200];
		sprintf (teksto, "Mi ne povas malfermi %s\n%s",
			LISTONOMO, sys_errlist[errno]);
		MessageBox (HWindow, teksto, "Problemo", MB_OK);
    }
    else
	{
		for (;;)
		{
			fgets (line, sizeof(line), listo);
			if (feof(listo))
				break;
			line[strlen(line)-1] = 0;
			Analizu (line);
            RegistruRegistreron ();
        }
		fclose (listo);
    }

}

void TTrivialDialog::NombruDemandojn ()
{
int status;
struct registrero r1;
	nd = 1;
#ifdef STDIO
	status = fseek (f, 0, SEEK_SET);
#else
	status = lseek (fd, 0, SEEK_SET);
#endif

	for (;;)
	{
#ifdef STDIO
		status = fread (&r1, sizeof(r1), 1, f);
		if (feof(f))
#else
		status = ::read (fd, &r1, sizeof(r1));
        if (status == 0)
#endif
			break;
		if (!strcmp (r.Kateg, r1.Kateg))
		{
			if (r1.Nombro >= nd)
            	nd = r1.Nombro + 1;
        }
    }
}

void TTrivialDialog::Legu (RTMessage Msg)
{
int status, i;
	PleniguRegistreron ();
	status = SerchuRegistreron (1);
	if (!status)
	{
    	
		SetDlgItemText (HWindow, ID_DEMANDO, "");
    	for (i=0; i<N_RESPONDOJ; i++)
			SetDlgItemText (HWindow, ID_RESPONDO1+i, "");

		/* MessageBox (HWindow, "Tio demando ne estas registrita.", "Legu", MB_OK); */

	}
#if 0
	SetFocus (HWindow /*hWnd*/);
#endif
#ifdef TRACE
	char mes[80];
	sprintf (mes, "HWindow = %d, hWnd = %d", HWindow, hWnd);
	MessageBox (HWindow, mes, "test", MB_OK);
#endif
}

int TTrivialDialog::SerchuRegistreron (int legu)
{
int status;
struct registrero r1;

	/* PleniguRegistreron (); */
#ifdef STDIO
	status = fseek (f, 0, SEEK_SET);
#else
	status = lseek (fd, 0, SEEK_SET);
#endif
	for (;;)
	{
#ifdef STDIO
		status = fread (&r1, sizeof(r1), 1, f);
		if (feof(f))
#else
		status = ::read (fd, &r1, sizeof(r1));
		if (status == 0)
#endif
			break;
		if (r.Nombro == r1.Nombro && !strcmp (r.Kateg, r1.Kateg))
		{
			if (legu)
            {
				memcpy (&r, &r1, sizeof(r));
				VidebliguDemandon ();
			}
			return 1;
 		}
	}
    return 0;
}

int TTrivialDialog::LeguRegistreron (struct registrero *r)
{
int status;
struct registrero r1;

	/* PleniguRegistreron (); */
#ifdef STDIO
	status = fseek (f, 0, SEEK_SET);
#else
	status = lseek (fd, 0, SEEK_SET);
#endif
	for (;;)
	{
#ifdef STDIO
		status = fread (&r1, sizeof(r1), 1, f);
		if (feof(f))
#else
		status = ::read (fd, &r1, sizeof(r1));
		if (status == 0)
#endif
			break;
		if (r->Nombro == r1.Nombro && !strcmp (r->Kateg, r1.Kateg))
		{
			/*if (legu)
            { */
				memcpy (r, &r1, sizeof(*r));
				/*VidebliguDemandon ();
			}   */
			return 1;
 		}
	}
    return 0;
}

void TTrivialDialog::PleniguRegistreron ()
{
char buf[120];
int i;
	GetDlgItemText (HWindow, ID_KATEG, sized(r.Kateg));
	GetDlgItemText (HWindow, ID_NOMBRO, sized(buf));
	r.Nombro = atoi (buf);
	GetDlgItemText (HWindow, ID_DEMANDO, sized(r.Demando));
	for (i=0; i<N_RESPONDOJ; i++)
    {
		GetDlgItemText (HWindow, ID_RESPONDO1+i, sized(r.Respondo[i]));
		if (IsDlgButtonChecked (HWindow, ID_RESP1BONA+i))
			r.BonaResp = i;
    }
}

void TTrivialDialog::VidebliguDemandon ()
{
char buf[120];
int i;
	/* GetDlgItemText (HWindow, ID_KATEG, sized(r.Kateg)); */
	SetDlgItemText (HWindow, ID_KATEG, r.Kateg);
	/*
	GetDlgItemText (HWindow, ID_NOMBRO, sized(buf));
	r.Nombro = atoi (buf);
	*/
	sprintf (buf, "%d", r.Nombro);
    SetDlgItemText (HWindow, ID_NOMBRO, buf);
	SetDlgItemText (HWindow, ID_DEMANDO, r.Demando);
	for (i=0; i<N_RESPONDOJ; i++)
    {
		SetDlgItemText (HWindow, ID_RESPONDO1+i, r.Respondo[i]);
		/*
		if (IsDlgButtonChecked (HWindow, ID_RESP1BONA+i))
			r.BonaResp = i;
		*/
        CheckDlgButton (HWindow, ID_RESP1BONA+i, FALSE);
	}
	CheckDlgButton (HWindow, ID_RESP1BONA+r.BonaResp, TRUE);

}

void TTrivialDialog::AlmetuRegistreron ()
{
int status;
#ifdef STDIO
	status = fseek (f, 0, SEEK_END);
	status = fwrite (&r, sizeof(r), 1, f);
	status = fflush (f);
#else
	status = lseek (fd, 0, SEEK_END);
	status = ::write (fd, &r, sizeof(r));
#endif
}

void TTrivialDialog::RegistruRegistreron ()
{
int status;
	if (SerchuRegistreron (0))
	{
#ifdef STDIO
		status = fseek (f, -sizeof(struct registrero), SEEK_CUR);
		status = fwrite (&r, sizeof(r), 1, f);
		status = fflush (f);
#else
		/* status = lseek (fd, -sizeof(struct registrero), SEEK_CUR); */
		/* status = lseek (fd, 0, SEEK_END); */
		status = tell (fd);
		status = lseek (fd, status-sizeof(struct registrero), SEEK_SET);
		status = ::write (fd, &r, sizeof(r));
#endif
	}
	else
    	AlmetuRegistreron ();
}

void TTrivialDialog::VidebliguNombron ()
{
char buf[120];
int n;
	GetDlgItemText (HWindow, ID_NOMBRO, sized(buf));
	n = atoi(buf);
	if (n == 0)
	{
		NombruDemandojn ();
		sprintf (buf, "%d", nd);
		r.Nombro = nd;
		SetDlgItemText (HWindow, ID_NOMBRO, buf);
    }
}

void TTrivialDialog::Registru (RTMessage Msg)
{
	PleniguRegistreron ();
    VidebliguNombron ();
	RegistruRegistreron ();
	if (SendMessage (hCtlId(ID_AUTOFORVISHO), BM_GETCHECK, 0, 0))
    {
		Forvishu (Msg);
		SetFocus (hCtlId(ID_KATEG));
    }
}

/*
void TTrivialDialog::Almetu (RTMessage Msg)
{
	SetDlgItemText (HWindow, ID_NOMBRO, "");
	Registru (Msg);
}
*/

void TTrivialDialog::Serchu1 ()
{
int status, i, j, sim;
struct registrero r1;
char buf[120];
int x;
#ifdef STDIO
	status = fseek (f, 0, SEEK_SET);
#else
	status = lseek (fd, 0, SEEK_SET);
#endif

    memset (simil, 0, sizeof(simil));
	for (;;)
	{
#ifdef STDIO
		status = fread (&r1, sizeof(r1), 1, f);
		if (feof(f))
#else
		status = ::read (fd, &r1, sizeof(r1));
        if (status == 0)
#endif
			break;
        sim = 0;
		/* if (IsDlgButtonChecked (HWindow, ID_SNDTD)) */
		GetDlgItemText (HWindow, ID_SNDTD, sized(buf));
		x = 1;
		sscanf (buf, "%d", &x);
        if (x != 0)
			sim += x * cmpstr (r.Demando, r1.Demando);
		/* if (IsDlgButtonChecked (HWindow, ID_SNDTR)) */
		GetDlgItemText (HWindow, ID_SNDTR, sized(buf));
		x = 1;
		sscanf (buf, "%d", &x);
        if (x != 0)
			for (i=0; i<N_RESPONDOJ; i++)
				sim += x * cmpstr (r.Demando, r1.Respondo[i]);
		/* if (IsDlgButtonChecked (HWindow, ID_SNRTD)) */
		GetDlgItemText (HWindow, ID_SNRTD, sized(buf));
		x = 1;
		sscanf (buf, "%d", &x);
        if (x != 0)
			for (i=0; i<N_RESPONDOJ; i++)
				sim += x * cmpstr (r.Respondo[i], r1.Demando);
		/* if (IsDlgButtonChecked (HWindow, ID_SNRTR)) */
		GetDlgItemText (HWindow, ID_SNRTR, sized(buf));
		x = 1;
		sscanf (buf, "%d", &x);
        if (x != 0)
			for (i=0; i<N_RESPONDOJ; i++)
			for (j=0; j<N_RESPONDOJ; j++)
				sim += x * cmpstr (r.Respondo[i], r1.Respondo[j]);
		for (i=0; i<N_SIMIL; i++)
		{
			if (sim >= simil[i].sim)
			{
					for (j=N_SIMIL-2; j>=i; j--)
					{
						memcpy (simil+j+1, simil+j, sizeof(simil[j]));
					}
					simil[i].sim = sim;
#ifdef SIMIL_MEM
					memcpy (&(simil[i].r), &r1, sizeof(struct registrero));
#else
					strcpy (simil[i].r.Kateg, r1.Kateg);
                    simil[i].r.Nombro = r1.Nombro;
#endif
					break;
			}

        }

    }

}

void TTrivialDialog::VidebliguTrovitanDemandon ()
{
char buf[120];
int i;
struct registrero r1;
int status;
	/* GetDlgItemText (HWindow, ID_KATEG, sized(r.Kateg)); */
	SetDlgItemText (HWindow, ID_TKATEG, simil[vs].r.Kateg);
	/*
	GetDlgItemText (HWindow, ID_NOMBRO, sized(buf));
	r.Nombro = atoi (buf);
	*/
	sprintf (buf, "%d", simil[vs].r.Nombro);
	SetDlgItemText (HWindow, ID_TNOMBRO, buf);
#ifndef SIMIL_MEM
	strcpy (r1.Kateg, simil[vs].r.Kateg);
    r1.Nombro = simil[vs].r.Nombro;
	status = LeguRegistreron (&r1);
	if (status)
    {
#endif

	SetDlgItemText (HWindow, ID_TDEMANDO,
#ifdef SIMIL_MEM
										simil[vs].r
#else
										r1
#endif
												.Demando);
	for (i=0; i<N_RESPONDOJ; i++)
    {
		SetDlgItemText (HWindow, ID_TRESPONDO1+i,
#ifdef SIMIL_MEM
												simil[vs].r
#else
												r1
#endif
													.Respondo[i]);
		/*
		if (IsDlgButtonChecked (HWindow, ID_RESP1BONA+i))
			r.BonaResp = i;
		*/
        CheckDlgButton (HWindow, ID_TRESP1BONA+i, FALSE);
	}
	CheckDlgButton (HWindow, ID_TRESP1BONA+
#ifdef SIMIL_MEM
										simil[vs].r
#else
										r1
#endif
												.BonaResp, TRUE);
#ifndef SIMIL_MEM
	}
#endif
}

void TTrivialDialog::Serchu (RTMessage Msg)
{
	PleniguRegistreron ();
	VidebliguNombron ();
	Serchu1 ();
	vs = 0;
    VidebliguTrovitanDemandon ();

}

void TTrivialDialog::Antaua (RTMessage Msg)
{
	if (vs > 0)
	{
		vs--;
		VidebliguTrovitanDemandon ();
	}
}

void TTrivialDialog::Sekvanta (RTMessage Msg)
{
	if (vs < N_SIMIL-1 && simil[vs].sim > 0)
	{
		vs++;
		VidebliguTrovitanDemandon ();
	}
}

#define ID_LISTOCB 101
#define ID_MONTRU 102
#define ID_FERMU 103

class TListoDialog : public TDialog
{
public:
	TAppliWindow *AppliWnd;
    PTComboBox Listo;  
	TListoDialog (PTWindowsObject parent, LPSTR name) :
		TDialog (parent, name)
	{
		AppliWnd = (TAppliWindow *)parent;
	}
	virtual void TListoDialog::WMInitDialog (RTMessage Msg)
	{
#ifdef SIMIL_MEM
		SendMessage (Listo->HWindow, CB_ADDSTRING, 0, (LPARAM) (LPCSTR) (AppliWnd->simil[0].r.Demando));
#endif 
 
	}
	virtual void Montru (RTMessage Msg) = [ID_FIRST + ID_MONTRU]
	{
	}
	virtual void Fermu (RTMessage Msg) = [ID_FIRST + ID_FERMU]
	{
		EndDialog (HWindow, 0);
    }

};

HWND hWndListo;

HWND hCtlObjListo[NCTL];

int FocusListo;

static char szAppNameListo[] = "Listo";

#define hCtlIdListo(id) (hCtlObjListo[(id)-COMMAND0])

#define WidListo (276)
#define HeiListo (169)

struct resource_item ResListo[NCTL];

InitResListo ()
{

	/*
	COMBOBOX ID_LIST, 11, 11, 255, 132, CBS_SIMPLE | WS_CHILD | WS_VISIBLE | WS_TABSTOP
	COMBOBOX () "" ID=ID_LIST X= 11 Y= 11 W= 255 H= 132 STYLE= CBS_SIMPLE | WS_CHILD | WS_VISIBLE | WS_TABSTOP
	*/
	strcpy (ResListo[1].name, "LISTBOX");
	strcpy (ResListo[1].ctlclass, "LISTBOX");
	strcpy (ResListo[1].caption, "");
	ResListo[1].param_int[1] =  11;
	ResListo[1].param_int[2] =  11;
	ResListo[1].param_int[3] =  255;
	ResListo[1].param_int[4] =  132;
/*	ResListo[1].param_int[5] =  CBS_SIMPLE | WS_CHILD | WS_VISIBLE | WS_TABSTOP; */
	ResListo[1].param_int[5] =  LBS_NOTIFY | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL;

	/*
	PUSHBUTTON "&Montru slipon", ID_MONTRU, 16, 144, 76, 14, WS_CHILD | WS_VISIBLE | WS_TABSTOP
	PUSHBUTTON ("BUTTON") "&Montru slipon" ID= ID_MONTRU X= 16 Y= 144 W= 76 H= 14 STYLE= WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON
	*/
	strcpy (ResListo[2].name, "PUSHBUTTON");
	strcpy (ResListo[2].ctlclass, "BUTTON");
	strcpy (ResListo[2].caption, "&Montru slipon");
	ResListo[2].param_int[1] =  16;
	ResListo[2].param_int[2] =  144;
	ResListo[2].param_int[3] =  76;
	ResListo[2].param_int[4] =  14;
	ResListo[2].param_int[5] =  WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON;

	/*
	PUSHBUTTON "&Fermu", ID_FERMU, 107, 144, 51, 14, WS_CHILD | WS_VISIBLE | WS_TABSTOP
	PUSHBUTTON ("BUTTON") "&Fermu" ID= ID_FERMU X= 107 Y= 144 W= 51 H= 14 STYLE= WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON
	*/
	strcpy (ResListo[3].name, "PUSHBUTTON");
	strcpy (ResListo[3].ctlclass, "BUTTON");
	strcpy (ResListo[3].caption, "&Fermu");
	ResListo[3].param_int[1] =  107;
	ResListo[3].param_int[2] =  144;
	ResListo[3].param_int[3] =  51;
	ResListo[3].param_int[4] =  14;
	ResListo[3].param_int[5] =  WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON;

#define NCTLListo 3


	return 0;
}


#define ID_LIST              (COMMAND0 + 1)
#define  ID_MONTRU           (COMMAND0 + 2)
#define  ID_FERMU            (COMMAND0 + 3)

#define HLISTO 

void InitWndListo (void);

MontruSlipon (struct registrero *r, HWND hw)
{
char buf[1000];
	sprintf (buf, " Kategorio %s  Nombro %d\n\n %s\n\n\n a) %s\n\n b) %s\n\n c) %s\n\nBona respondo: %c\n",
		r->Kateg, r->Nombro, r->Demando,
		r->Respondo[0], r->Respondo[1], r->Respondo[2],
		'a'+r->BonaResp);
	MessageBox (hw, buf, "Slipo", MB_OK);
}

class TAppliWindowListo : public TWindow
{
public:
	/* declare control objects PT... */
	/* PTButton Button1; */
	PTListBox Listo;
    PTWindowsObject pWndCtlObj[NCTL];
	HWND hWndCtlObj[NCTL];
 
	TAppliWindowListo (PTWindowsObject parent, LPSTR title) :
		TWindow (parent, title)
	{
		EnableKBHandler ();
        /* create control objects PT... with new T... */ 
		/* Button1 = new TButton (this, 101, "&Test 1", 50, 80, 40, 20, FALSE); */
/*			hCtlObjListo[1] = CreateWindow ("COMBOBOX", "",
				 CBS_SIMPLE | CBS_AUTOHSCROLL | WS_CHILD | WS_VISIBLE | WS_TABSTOP,
				 11*2,  11*3/2,  255*2,  132*3/2,
				hWnd, COMMAND0+1, hInst, NULL);
*/
#ifndef HLISTO  
/*
		Listo = new TComboBox (this, 101,
				 11*2,  11*3/2,  255*2,  132*3/2,
				 CBS_SIMPLE | CBS_AUTOHSCROLL | WS_CHILD | WS_VISIBLE | WS_TABSTOP,
				 1000);
*/

		Listo = new TListBox (this, 101,
				 11*2,  11*3/2,  255*2,  132*3/2);
		Listo->Attr.Style &= ~LBS_SORT;

#endif
	}

	LPSTR GetClassName ()
	{
    	return szAppNameListo;
	}

	virtual BOOL Create ()
	{
	BOOL status;
		status = TWindow::Create ();
		hWndListo = HWindow;
        /* create objects hWndCtlObj[i] with CreateWindow */
 		return status;
    }

	virtual void SetupWindow ()
	{
	int status;
	char mes[200];
	int i;
    char line[300];
		TWindow::SetupWindow ();
#if 0
#ifdef HLISTO
		status = SendMessage (/*Listo->HWindow*/ hCtlId(ID_LIST), CB_ADDSTRING, 0,
		(LPARAM) (LPCSTR) (/*Appli*/Wnd->simil[0].r.Demando));
		sprintf (mes, "%d %d %d", status, CB_ERR, CB_ERRSPACE);
		MessageBox (hWndListo, mes, "trace (w)", MB_OK);
#else
		Listo->AddString (Wnd->simil[0].r.Demando);
#endif
#endif
		for (i=0; i<N_SIMIL && Wnd->simil[i].sim>0; i++)
		{
#ifndef SIMIL_MEM
		struct registrero r1;
			memset (&r1, 0, sizeof(r1));
			strcpy (r1.Kateg, Wnd->simil[i].r.Kateg);
			r1.Nombro = Wnd->simil[i].r.Nombro;
            status = Wnd->LeguRegistreron (&r1);
#endif
			sprintf (line, "%s %d %s %ca)%s %cb)%s %cc)%s",
				Wnd->simil[i].r.Kateg,
				Wnd->simil[i].r.Nombro,
#ifdef SIMIL_MEM
				Wnd->simil[i].r.Demando,
				Wnd->simil[i].r.BonaResp==0?'*':' ',
				Wnd->simil[i].r.Respondo[0],
				Wnd->simil[i].r.BonaResp==1?'*':' ',
				Wnd->simil[i].r.Respondo[1],
				Wnd->simil[i].r.BonaResp==2?'*':' ',
				Wnd->simil[i].r.Respondo[2]
#else
 				r1.Demando,
				r1.BonaResp==0?'*':' ',
				r1.Respondo[0],
				r1.BonaResp==1?'*':' ',
				r1.Respondo[1],
				r1.BonaResp==2?'*':' ',
				r1.Respondo[2]

#endif
				);
#ifdef HLISTO
		status = SendMessage (/*Listo->HWindow*/ hCtlIdListo(ID_LIST),
			LB_ADDSTRING, 0, (LPARAM) (LPCSTR) line);

#else
			Listo->AddString (line);
#endif
        }

	}
	
	/* void HandleWndCtli (RTMessage Msg) = [ID_FIRST + COMMAND0 + i] { ... } */

	void Montru (RTMessage Msg) = [ID_FIRST + ID_MONTRU]
	{
#if 0
#ifdef HLISTO
	int status;
	char mes[200];
		status = SendMessage (/*Listo->HWindow*/ hCtlId(ID_LIST), CB_ADDSTRING, 0,
		(LPARAM) (LPCSTR) (/*Appli*/Wnd->simil[0].r.Demando));
		sprintf (mes, "%d %d %d", status, CB_ERR, CB_ERRSPACE);
		MessageBox (hWndListo, mes, "trace (w)", MB_OK);
#else
		Listo->AddString (Wnd->simil[0].r.Demando);
#endif
#endif
	int i;
	char buf[1000];
#ifdef HLISTO
		i = SendMessage (/*Listo->HWindow*/ hCtlIdListo(ID_LIST),
			LB_GETCURSEL, 0, 0);
#else
		i = Listo->GetSelIndex();
#endif

#ifdef SIMIL_MEM
		MontruSlipon (&(Wnd->simil[i].r), HWindow);
#else
		struct registrero r1;
        int status;
			memset (&r1, 0, sizeof(r1));
			strcpy (r1.Kateg, Wnd->simil[i].r.Kateg);
			r1.Nombro = Wnd->simil[i].r.Nombro;
            status = Wnd->LeguRegistreron (&r1);
			MontruSlipon (&r1, HWindow);
#endif

	}

	void List (RTMessage Msg) = [ID_FIRST + ID_LIST]
	{
		switch (Msg.LP.Hi)
		{
			case LBN_DBLCLK:
				Montru (Msg);
				break;
        }
	}

	void Fermu (RTMessage Msg) = [ID_FIRST + ID_FERMU]
	{
		DestroyWindow (hWndListo);
		this->TWindow::~TWindow();
        SendMessage (hWndListo, WM_QUIT, 0, 0);
    }

};

TAppliWindowListo *WndListo;

	/*
	COMBOBOX ID_LIST, 11, 11, 255, 132, CBS_SIMPLE | WS_CHILD | WS_VISIBLE | WS_TABSTOP
	COMBOBOX () "" ID=ID_LIST X= 11 Y= 11 W= 255 H= 132 STYLE= CBS_SIMPLE | WS_CHILD | WS_VISIBLE | WS_TABSTOP
	*/

	/*
	COMBOBOX ID_LIST, 11, 11, 255, 132, CBS_SIMPLE | WS_CHILD | WS_VISIBLE | WS_TABSTOP
	COMBOBOX () "" ID=ID_LIST X= 11 Y= 11 W= 255 H= 132 STYLE= CBS_SIMPLE | WS_CHILD | WS_VISIBLE | WS_TABSTOP
	*/
void HandleCtlListoID_LIST (TMessage *pmsg)
{
/* Add code here */
	WndListo->List (*pmsg);
}

	/*
	PUSHBUTTON "&Montru slipon", ID_MONTRU, 16, 144, 76, 14, WS_CHILD | WS_VISIBLE | WS_TABSTOP
	PUSHBUTTON ("BUTTON") "&Montru slipon" ID= ID_MONTRU X= 16 Y= 144 W= 76 H= 14 STYLE= WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON
	*/
void HandleCtlListoID_MONTRU (TMessage *pmsg)
{
/* Add code here */
	WndListo->Montru (*pmsg);

}

	/*
	PUSHBUTTON "&Fermu", ID_FERMU, 107, 144, 51, 14, WS_CHILD | WS_VISIBLE | WS_TABSTOP
	PUSHBUTTON ("BUTTON") "&Fermu" ID= ID_FERMU X= 107 Y= 144 W= 51 H= 14 STYLE= WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON
	*/
void HandleCtlListoID_FERMU (TMessage *pmsg)
{
/* Add code here */
	WndListo->Fermu (*pmsg);
}


	/*

	 () "" ID=000 X=000 Y=000 W=000 H=000 STYLE=000
	*/

void HandleCtlListo4 ()
{
/* Add code here */

}

LRESULT CALLBACK WndProcListo (HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
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
			WndListo->HWindow = hWndListo;
        	/* create control objects hCtlObjListo[i] with CreateWindow */
			for (nCtl=1; nCtl<=NCTLListo; nCtl++)
			{
				hCtlObjListo[nCtl] = CreateWindow (ResListo[nCtl].ctlclass, ResListo[nCtl].caption,
					ResListo[nCtl].param_int[P_STYLE],
					ResListo[nCtl].param_int[P_X]*2, ResListo[nCtl].param_int[P_Y]*3/2, ResListo[nCtl].param_int[P_W]*2, ResListo[nCtl].param_int[P_H]*3/2,
					hWnd, COMMAND0+nCtl, hInst, NULL);
			}
			FocusListo = 1;
			SetFocus (hCtlObjListo[FocusListo]);
#if 0
	/*
	COMBOBOX ID_LIST, 11, 11, 255, 132, CBS_SIMPLE | WS_CHILD | WS_VISIBLE | WS_TABSTOP
	COMBOBOX () "" ID=ID_LIST X= 11 Y= 11 W= 255 H= 132 STYLE= CBS_SIMPLE | WS_CHILD | WS_VISIBLE | WS_TABSTOP
	*/
#ifdef HLISTO
			hCtlObjListo[1] = CreateWindow ("COMBOBOX", "",
				 CBS_SIMPLE | CBS_AUTOHSCROLL | WS_CHILD | WS_VISIBLE | WS_TABSTOP,
				 11*2,  11*3/2,  255*2,  132*3/2,
				hWnd, COMMAND0+1, hInst, NULL);

#endif
	/*
	PUSHBUTTON "&Montru slipon", ID_MONTRU, 16, 144, 76, 14, WS_CHILD | WS_VISIBLE | WS_TABSTOP
	PUSHBUTTON ("BUTTON") "&Montru slipon" ID= ID_MONTRU X= 16 Y= 144 W= 76 H= 14 STYLE= WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON
	*/
			hCtlObjListo[2] = CreateWindow ("BUTTON", "&Montru slipon",
				 WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON,
				 16*2,  144*3/2,  76*2,  14*3/2,
				hWnd, COMMAND0+2, hInst, NULL);

	/*
	PUSHBUTTON "&Fermu", ID_FERMU, 107, 144, 51, 14, WS_CHILD | WS_VISIBLE | WS_TABSTOP
	PUSHBUTTON ("BUTTON") "&Fermu" ID= ID_FERMU X= 107 Y= 144 W= 51 H= 14 STYLE= WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON
	*/
			hCtlObjListo[3] = CreateWindow ("BUTTON", "&Fermu",
				 WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON,
				 107*2,  144*3/2,  51*2,  14*3/2,
				hWnd, COMMAND0+3, hInst, NULL);

	/*

	 () "" ID=000 X=000 Y=000 W=000 H=000 STYLE=000
	*/
/*			hCtlObjListo[4] = CreateWindow (, "",
				000,
				000*2, 000*3/2, 000*2, 000*3/2,
				hWnd, COMMAND0+4, hInst, NULL);
*/
#endif
			InitWndListo ();
 			break;
		case WM_CLOSE:
			for (nCtl=1; nCtl<=NCTLListo; nCtl++)
				DestroyWindow (hCtlObjListo[nCtl]);
			return DefWindowProc(hWnd, Message, wParam, lParam);
		case WM_SIZE:
			for (nCtl=1; nCtl<=NCTLListo; nCtl++)
			{
				MoveWindow (hCtlObjListo[nCtl],
					ResListo[nCtl].param_int[P_X]*LOWORD(lParam)/(WidListo),
					ResListo[nCtl].param_int[P_Y]*HIWORD(lParam)/(HeiListo),
					ResListo[nCtl].param_int[P_W]*LOWORD(lParam)/(WidListo),
					ResListo[nCtl].param_int[P_H]*HIWORD(lParam)/(HeiListo),
					TRUE);
			}
			break;
		case WM_COMMAND:
			switch (wParam)
			{
				/* case COMMAND0+i:
					HandleCtlListoi();
					break; */
				case COMMAND0+1:
					HandleCtlListoID_LIST (&msg);
					break;
				case COMMAND0+2:
					HandleCtlListoID_MONTRU (&msg);
					break;
				case COMMAND0+3:
					HandleCtlListoID_FERMU (&msg);
					break;

			}
			break;
		default:
			return(DefWindowProc(hWnd, Message, wParam, lParam));
	}
	return NULL;
}

void InitWndListo (void)
{
/* Add code here */
int status;
char mes[200];
#if 0
#ifdef HLISTO
		status = SendMessage (hCtlId(ID_LIST), WM_SETREDRAW, TRUE, 0);
		status = SendMessage (/*Listo->HWindow*/ hCtlId(ID_LIST), CB_ADDSTRING, 0,
		(LPARAM) (LPCSTR) (/*Appli*/Wnd->simil[0].r.Demando));
		sprintf (mes, "%d %d %d", status, CB_ERR, CB_ERRSPACE);
		MessageBox (hWndListo, mes, "trace", MB_OK);
#else
		WndListo->Listo->AddString ("testo"/*Wnd->simil[0].r.Demando*/);
#endif
#endif
		WndListo->SetupWindow ();
}

class TAppliListo : public TApplication
{
public:
	TAppliListo (LPSTR AName, HINSTANCE hInstance, HINSTANCE hPrevInstance,
		LPSTR lpCmdLine, int nCmdShow) :
		TApplication (AName, hInstance, hPrevInstance, lpCmdLine, nCmdShow)
	{
	}
	virtual void InitMainWindow ()
	{
		WndListo = new TAppliWindowListo (NULL, Name);
		MainWindow = WndListo;
		WndListo->Attr.W = 276*2;
		WndListo->Attr.H = 169*3/2+20;
    }
};

HINSTANCE ghPrevInstance;
LPSTR glpCmdLine;
int gnCmdShow;

int KBHandler (MSG *pmsg, HWND hWnd, int NbrCtl, struct resource_item *Res, HWND *hCtlObj, int *pFocus);

void TTrivialDialog::Listo (RTMessage Msg)
{
	/* GetApplication()->ExecDialog (new TListoDialog (this, "LISTO")); */
#if 0
	TAppliListo Appli (szAppNameListo, hInst, ghPrevInstance, glpCmdLine, gnCmdShow);
	Appli.Run ();
#else
MSG msg;
#define LWCAPTION "Listo"
	WndListo = new TAppliWindowListo (NULL, LWCAPTION);

	hWndListo = CreateWindow (szAppNameListo, LWCAPTION,
		WS_OVERLAPPEDWINDOW /*| WS_VSCROLL | WS_HSCROLL*/,
		CW_USEDEFAULT, CW_USEDEFAULT, (WidListo)*2, (HeiListo)*3/2+20,
		NULL, NULL, hInst, NULL);

	if (!hWndListo)
		return;

	ShowWindow (hWndListo, gnCmdShow);
	UpdateWindow (hWndListo);


	while (GetMessage(&msg, NULL, 0, 0))
	{
 		if (!KBHandler (&msg, hWndListo, NCTLListo, ResListo, hCtlObjListo, &FocusListo))
		{
 			TranslateMessage (&msg);
			DispatchMessage (&msg);
		}
	}

#endif
}

int SpecificHandler (MSG *pmsg)
{
	if (pmsg->message == WM_KEYDOWN)
	{
		switch (pmsg->wParam)
		{
			case VK_NEXT:
				Wnd->LeguSekvanta (1);
				return 1;
			case VK_PRIOR:
				Wnd->LeguSekvanta (-1);
				return 1;
			default:
            	return 0;
		}
	}
    return 0;
}

#if 0
class TAppliWindow : public TWindow
{
public:
	/* declare control objects PT... */
	/* PTButton Button1; */
    PTWindowsObject pWndCtlObj[NCTL];
	HWND hWndCtlObj[NCTL];
 
	TAppliWindow (PTWindowsObject parent, LPSTR title) :
		TWindow (parent, title)
	{
		EnableKBHandler ();
        /* create control objects PT... with new T... */ 
		/* Button1 = new TButton (this, 101, "&Test 1", 50, 80, 40, 20, FALSE); */
	}

	LPSTR GetClassName ()
	{
    	return szAppName;
	}

	virtual BOOL Create ()
	{
	BOOL status;
		status = TWindow::Create ();
		hWnd = HWindow;
        /* create objects hWndCtlObj[i] with CreateWindow */ 
 		return status;
    }
	/* void HandleWndCtli (RTMessage Msg) = [ID_FIRST + COMMAND0 + i] { ... } */
};

TAppliWindow *Wnd;
#endif

class TAppli : public TApplication
{
public:
	TAppli (LPSTR AName, HINSTANCE hInstance, HINSTANCE hPrevInstance,
		LPSTR lpCmdLine, int nCmdShow) :
		TApplication (AName, hInstance, hPrevInstance, lpCmdLine, nCmdShow)
	{
	}
	virtual void InitMainWindow ()
	{
		Wnd = new TAppliWindow (NULL, Name);
		MainWindow = Wnd;
		Wnd->Attr.W = (Wid)*2;
		Wnd->Attr.H = (Hei)*3/2+20;
    }
};

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

	InitRes ();

	ghPrevInstance = hPrevInstance;
	glpCmdLine = lpCmdLine;
	gnCmdShow = nCmdShow;

	if (!hPrevInstance)
	{
		wndclass.style = CS_HREDRAW | CS_VREDRAW;
		wndclass.lpfnWndProc = WndProc;
		wndclass.cbClsExtra = 0;
		wndclass.cbWndExtra = 0;
		wndclass.hInstance = hInstance;
		wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
		wndclass.hbrBackground  = GetStockObject(WHITE_BRUSH);
		wndclass.lpszMenuName = NULL;
		wndclass.lpszClassName  = szAppName;

		if (!RegisterClass(&wndclass))
			return FALSE;
	}

	InitResListo ();

	if (!hPrevInstance)
	{
		wndclass.style = CS_HREDRAW | CS_VREDRAW;
		wndclass.lpfnWndProc = WndProcListo;
		wndclass.cbClsExtra = 0;
		wndclass.cbWndExtra = 0;
		wndclass.hInstance = hInstance;
		wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
		wndclass.hbrBackground  = GetStockObject(WHITE_BRUSH);
		wndclass.lpszMenuName = NULL;
		wndclass.lpszClassName  = szAppNameListo;

		if (!RegisterClass(&wndclass))
			return FALSE;
	}

#if 0
	hInst = hInstance;
	TAppli Appli (szAppName, hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	Appli.Run ();
	return Appli.Status;
#else
#define MWCAPTION "Administrado de demandaro"
	Wnd = new TAppliWindow (NULL, MWCAPTION);
	hWnd = CreateWindow (szAppName, MWCAPTION,
		WS_OVERLAPPEDWINDOW /*| WS_VSCROLL | WS_HSCROLL*/,
		CW_USEDEFAULT, CW_USEDEFAULT, (Wid)*2, (Hei)*3/2+20,
		NULL, NULL, hInstance, NULL);
	if (!hWnd)
		return FALSE;
	/*Wnd->HWindow = hWnd;*/

	ShowWindow (hWnd, nCmdShow);
	UpdateWindow (hWnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
	  if (!SpecificHandler (&msg))
      {
		if (!KBHandler (&msg, hWnd, NCTL, Res, hCtlObj, &Focus))
		{
 			TranslateMessage (&msg);
			DispatchMessage (&msg);
		}
      }
	}

	return(msg.wParam);
#endif
}
