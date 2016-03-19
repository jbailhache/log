/* Windows API source generated from resource file codpil.ctl */

#include <owl.h>
#include <window.h>
#include <string.h>
#include <ctype.h>

#include <stdio.h>

HANDLE hInst;
HWND hWndCodPil;

#define NCTL 60
HWND hCtlObjCodPil[NCTL];

int FocusCodPil;

static char szAppNameCodPil[] = "AppliCodPil";
static char WndCaptionCodPil[] = "CodPil";

#define COMMAND0 100

#define hCtlIdCodPil(id) (hCtlObjCodPil[(id)-COMMAND0])

#define WidCodPil (228)
#define HeiCodPil (165)

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

struct resource_item ResCodPil[NCTL];

InitResCodPil ()
{

	/*
	LTEXT "n", -1, 8, 9, 16, 8, WS_CHILD | WS_VISIBLE | WS_GROUP
	LTEXT ("STATIC") "n" ID= -1 X= 8 Y= 9 W= 16 H= 8 STYLE= WS_CHILD | WS_VISIBLE | WS_GROUP
	*/
	strcpy (ResCodPil[1].name, "LTEXT");
	strcpy (ResCodPil[1].ctlclass, "STATIC");
	strcpy (ResCodPil[1].caption, "n");
	ResCodPil[1].param_int[1] =  8;
	ResCodPil[1].param_int[2] =  9;
	ResCodPil[1].param_int[3] =  16;
	ResCodPil[1].param_int[4] =  8;
	ResCodPil[1].param_int[5] =  WS_CHILD | WS_VISIBLE | WS_GROUP;

	/*
	LTEXT "w", -1, 6, 43, 16, 8, WS_CHILD | WS_VISIBLE | WS_GROUP
	LTEXT ("STATIC") "w" ID= -1 X= 6 Y= 43 W= 16 H= 8 STYLE= WS_CHILD | WS_VISIBLE | WS_GROUP
	*/
	strcpy (ResCodPil[2].name, "LTEXT");
	strcpy (ResCodPil[2].ctlclass, "STATIC");
	strcpy (ResCodPil[2].caption, "w");
	ResCodPil[2].param_int[1] =  6;
	ResCodPil[2].param_int[2] =  43;
	ResCodPil[2].param_int[3] =  16;
	ResCodPil[2].param_int[4] =  8;
	ResCodPil[2].param_int[5] =  WS_CHILD | WS_VISIBLE | WS_GROUP;

	/*
	LTEXT "m", -1, 5, 30, 16, 8, WS_CHILD | WS_VISIBLE | WS_GROUP
	LTEXT ("STATIC") "m" ID= -1 X= 5 Y= 30 W= 16 H= 8 STYLE= WS_CHILD | WS_VISIBLE | WS_GROUP
	*/
	strcpy (ResCodPil[3].name, "LTEXT");
	strcpy (ResCodPil[3].ctlclass, "STATIC");
	strcpy (ResCodPil[3].caption, "m");
	ResCodPil[3].param_int[1] =  5;
	ResCodPil[3].param_int[2] =  30;
	ResCodPil[3].param_int[3] =  16;
	ResCodPil[3].param_int[4] =  8;
	ResCodPil[3].param_int[5] =  WS_CHILD | WS_VISIBLE | WS_GROUP;

	/*
	LTEXT "1/w", -1, 6, 60, 16, 8, WS_CHILD | WS_VISIBLE | WS_GROUP
	LTEXT ("STATIC") "1/w" ID= -1 X= 6 Y= 60 W= 16 H= 8 STYLE= WS_CHILD | WS_VISIBLE | WS_GROUP
	*/
	strcpy (ResCodPil[4].name, "LTEXT");
	strcpy (ResCodPil[4].ctlclass, "STATIC");
	strcpy (ResCodPil[4].caption, "1/w");
	ResCodPil[4].param_int[1] =  6;
	ResCodPil[4].param_int[2] =  60;
	ResCodPil[4].param_int[3] =  16;
	ResCodPil[4].param_int[4] =  8;
	ResCodPil[4].param_int[5] =  WS_CHILD | WS_VISIBLE | WS_GROUP;

	/*
	EDITTEXT ID_N, 26, 7, 48, 12, ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP
	EDITTEXT ("EDIT") "" ID=ID_N X= 26 Y= 7 W= 48 H= 12 STYLE= ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL
	*/
	strcpy (ResCodPil[5].name, "EDITTEXT");
	strcpy (ResCodPil[5].ctlclass, "EDIT");
	strcpy (ResCodPil[5].caption, "");
	ResCodPil[5].param_int[1] =  26;
	ResCodPil[5].param_int[2] =  7;
	ResCodPil[5].param_int[3] =  48;
	ResCodPil[5].param_int[4] =  12;
	ResCodPil[5].param_int[5] =  ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL;

	/*
	EDITTEXT ID_M, 25, 23, 50, 12, ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP
	EDITTEXT ("EDIT") "" ID=ID_M X= 25 Y= 23 W= 50 H= 12 STYLE= ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL
	*/
	strcpy (ResCodPil[6].name, "EDITTEXT");
	strcpy (ResCodPil[6].ctlclass, "EDIT");
	strcpy (ResCodPil[6].caption, "");
	ResCodPil[6].param_int[1] =  25;
	ResCodPil[6].param_int[2] =  23;
	ResCodPil[6].param_int[3] =  50;
	ResCodPil[6].param_int[4] =  12;
	ResCodPil[6].param_int[5] =  ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL;

	/*
	EDITTEXT ID_W, 21, 41, 55, 12, ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP
	EDITTEXT ("EDIT") "" ID=ID_W X= 21 Y= 41 W= 55 H= 12 STYLE= ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL
	*/
	strcpy (ResCodPil[7].name, "EDITTEXT");
	strcpy (ResCodPil[7].ctlclass, "EDIT");
	strcpy (ResCodPil[7].caption, "");
	ResCodPil[7].param_int[1] =  21;
	ResCodPil[7].param_int[2] =  41;
	ResCodPil[7].param_int[3] =  55;
	ResCodPil[7].param_int[4] =  12;
	ResCodPil[7].param_int[5] =  ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL;

	/*
	EDITTEXT ID_W1, 