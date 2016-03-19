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
	EDITTEXT ID_W1, 20, 57, 55, 12, ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP
	EDITTEXT ("EDIT") "" ID=ID_W1 X= 20 Y= 57 W= 55 H= 12 STYLE= ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL
	*/
	strcpy (ResCodPil[8].name, "EDITTEXT");
	strcpy (ResCodPil[8].ctlclass, "EDIT");
	strcpy (ResCodPil[8].caption, "");
	ResCodPil[8].param_int[1] =  20;
	ResCodPil[8].param_int[2] =  57;
	ResCodPil[8].param_int[3] =  55;
	ResCodPil[8].param_int[4] =  12;
	ResCodPil[8].param_int[5] =  ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL;

	/*
	CONTROL "a", -1, "STATIC", SS_LEFT | WS_CHILD | WS_VISIBLE | WS_GROUP, 90, 5, 16, 8
	CONTROL ( "STATIC") "a" ID= -1 X= 90 Y= 5 W= 16 H= 8 STYLE= SS_LEFT | WS_CHILD | WS_VISIBLE | WS_GROUP
	*/
	strcpy (ResCodPil[9].name, "CONTROL");
	strcpy (ResCodPil[9].ctlclass,  "STATIC");
	strcpy (ResCodPil[9].caption, "a");
	ResCodPil[9].param_int[1] =  90;
	ResCodPil[9].param_int[2] =  5;
	ResCodPil[9].param_int[3] =  16;
	ResCodPil[9].param_int[4] =  8;
	ResCodPil[9].param_int[5] =  SS_LEFT | WS_CHILD | WS_VISIBLE | WS_GROUP;

	/*
	CONTROL "a'", -1, "STATIC", SS_LEFT | WS_CHILD | WS_VISIBLE | WS_GROUP, 137, 6, 16, 8
	CONTROL ( "STATIC") "a'" ID= -1 X= 137 Y= 6 W= 16 H= 8 STYLE= SS_LEFT | WS_CHILD | WS_VISIBLE | WS_GROUP
	*/
	strcpy (ResCodPil[10].name, "CONTROL");
	strcpy (ResCodPil[10].ctlclass,  "STATIC");
	strcpy (ResCodPil[10].caption, "a'");
	ResCodPil[10].param_int[1] =  137;
	ResCodPil[10].param_int[2] =  6;
	ResCodPil[10].param_int[3] =  16;
	ResCodPil[10].param_int[4] =  8;
	ResCodPil[10].param_int[5] =  SS_LEFT | WS_CHILD | WS_VISIBLE | WS_GROUP;

	/*
	EDITTEXT ID_A, 83, 16, 43, 110, ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP
	EDITTEXT ("EDIT") "" ID=ID_A X= 83 Y= 16 W= 43 H= 110 STYLE= ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL
	*/
	strcpy (ResCodPil[11].name, "EDITTEXT");
	strcpy (ResCodPil[11].ctlclass, "EDIT");
	strcpy (ResCodPil[11].caption, "");
	ResCodPil[11].param_int[1] =  83;
	ResCodPil[11].param_int[2] =  16;
	ResCodPil[11].param_int[3] =  43;
	ResCodPil[11].param_int[4] =  110;
	ResCodPil[11].param_int[5] =  ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL | ES_MULTILINE | ES_WANTRETURN;

	/*
	EDITTEXT ID_A1, 133, 16, 43, 110, ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP
	EDITTEXT ("EDIT") "" ID=ID_A1 X= 133 Y= 16 W= 43 H= 110 STYLE= ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL
	*/
	strcpy (ResCodPil[12].name, "EDITTEXT");
	strcpy (ResCodPil[12].ctlclass, "EDIT");
	strcpy (ResCodPil[12].caption, "");
	ResCodPil[12].param_int[1] =  133;
	ResCodPil[12].param_int[2] =  16;
	ResCodPil[12].param_int[3] =  43;
	ResCodPil[12].param_int[4] =  110;
	ResCodPil[12].param_int[5] =  ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL | ES_MULTILINE | ES_WANTRETURN;

	/*
	PUSHBUTTON "&Valider", ID_VALIDER, 18, 77, 46, 14, WS_CHILD | WS_VISIBLE | WS_TABSTOP
	PUSHBUTTON ("BUTTON") "&Valider" ID= ID_VALIDER X= 18 Y= 77 W= 46 H= 14 STYLE= WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON
	*/
	strcpy (ResCodPil[13].name, "PUSHBUTTON");
	strcpy (ResCodPil[13].ctlclass, "BUTTON");
	strcpy (ResCodPil[13].caption, "&Valider");
	ResCodPil[13].param_int[1] =  18;
	ResCodPil[13].param_int[2] =  77;
	ResCodPil[13].param_int[3] =  46;
	ResCodPil[13].param_int[4] =  14;
	ResCodPil[13].param_int[5] =  WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON;

	/*
	CONTROL "Clair", -1, "STATIC", SS_LEFT | WS_CHILD | WS_VISIBLE | WS_GROUP, 10, 132, 16, 8
	CONTROL ( "STATIC") "Clair" ID= -1 X= 10 Y= 132 W= 16 H= 8 STYLE= SS_LEFT | WS_CHILD | WS_VISIBLE | WS_GROUP
	*/
	strcpy (ResCodPil[14].name, "CONTROL");
	strcpy (ResCodPil[14].ctlclass,  "STATIC");
	strcpy (ResCodPil[14].caption, "Clair");
	ResCodPil[14].param_int[1] =  10;
	ResCodPil[14].param_int[2] =  132;
	ResCodPil[14].param_int[3] =  16;
	ResCodPil[14].param_int[4] =  8;
	ResCodPil[14].param_int[5] =  SS_LEFT | WS_CHILD | WS_VISIBLE | WS_GROUP;

	/*
	EDITTEXT ID_CLAIR, 34, 131, 185, 12, ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP
	EDITTEXT ("EDIT") "" ID=ID_CLAIR X= 34 Y= 131 W= 185 H= 12 STYLE= ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL
	*/
	strcpy (ResCodPil[15].name, "EDITTEXT");
	strcpy (ResCodPil[15].ctlclass, "EDIT");
	strcpy (ResCodPil[15].caption, "");
	ResCodPil[15].param_int[1] =  34;
	ResCodPil[15].param_int[2] =  131;
	ResCodPil[15].param_int[3] =  185;
	ResCodPil[15].param_int[4] =  12;
	ResCodPil[15].param_int[5] =  ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL;

	/*
	PUSHBUTTON "&Coder", ID_CODER, 15, 99, 24, 14, WS_CHILD | WS_VISIBLE | WS_TABSTOP
	PUSHBUTTON ("BUTTON") "&Coder" ID= ID_CODER X= 15 Y= 99 W= 24 H= 14 STYLE= WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON
	*/
	strcpy (ResCodPil[16].name, "PUSHBUTTON");
	strcpy (ResCodPil[16].ctlclass, "BUTTON");
	strcpy (ResCodPil[16].caption, "&Coder");
	ResCodPil[16].param_int[1] =  15;
	ResCodPil[16].param_int[2] =  99;
	ResCodPil[16].param_int[3] =  24;
	ResCodPil[16].param_int[4] =  14;
	ResCodPil[16].param_int[5] =  WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON;

	/*
	PUSHBUTTON "&Decoder", ID_DECODER, 47, 99, 31, 14, WS_CHILD | WS_VISIBLE | WS_TABSTOP
	PUSHBUTTON ("BUTTON") "&Decoder" ID= ID_DECODER X= 47 Y= 99 W= 31 H= 14 STYLE= WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON
	*/
	strcpy (ResCodPil[17].name, "PUSHBUTTON");
	strcpy (ResCodPil[17].ctlclass, "BUTTON");
	strcpy (ResCodPil[17].caption, "&Decoder");
	ResCodPil[17].param_int[1] =  47;
	ResCodPil[17].param_int[2] =  99;
	ResCodPil[17].param_int[3] =  31;
	ResCodPil[17].param_int[4] =  14;
	ResCodPil[17].param_int[5] =  WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON;

	/*
	CONTROL "Codé", -1, "STATIC", SS_LEFT | WS_CHILD | WS_VISIBLE | WS_GROUP, 8, 145, 16, 8
	CONTROL ( "STATIC") "Codé" ID= -1 X= 8 Y= 145 W= 16 H= 8 STYLE= SS_LEFT | WS_CHILD | WS_VISIBLE | WS_GROUP
	*/
	strcpy (ResCodPil[18].name, "CONTROL");
	strcpy (ResCodPil[18].ctlclass,  "STATIC");
	strcpy (ResCodPil[18].caption, "Codé");
	ResCodPil[18].param_int[1] =  8;
	ResCodPil[18].param_int[2] =  145;
	ResCodPil[18].param_int[3] =  16;
	ResCodPil[18].param_int[4] =  8;
	ResCodPil[18].param_int[5] =  SS_LEFT | WS_CHILD | WS_VISIBLE | WS_GROUP;

	/*
	EDITTEXT ID_CODE, 32, 149, 184, 12, ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP
	EDITTEXT ("EDIT") "" ID=ID_CODE X= 32 Y= 149 W= 184 H= 12 STYLE= ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL
	*/
	strcpy (ResCodPil[19].name, "EDITTEXT");
	strcpy (ResCodPil[19].ctlclass, "EDIT");
	strcpy (ResCodPil[19].caption, "");
	ResCodPil[19].param_int[1] =  32;
	ResCodPil[19].param_int[2] =  149;
	ResCodPil[19].param_int[3] =  184;
	ResCodPil[19].param_int[4] =  12;
	ResCodPil[19].param_int[5] =  ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL;

#define NCTLCodPil 19


	return 0;
}

#define ID_N                 (COMMAND0 + 5)
#define ID_M                 (COMMAND0 + 6)
#define ID_W                 (COMMAND0 + 7)
#define ID_W1                (COMMAND0 + 8)
#define ID_A                 (COMMAND0 + 11)
#define ID_A1                (COMMAND0 + 12)
#define  ID_VALIDER          (COMMAND0 + 13)
#define ID_CLAIR             (COMMAND0 + 15)
#define  ID_CODER            (COMMAND0 + 16)
#define  ID_DECODER          (COMMAND0 + 17)
#define ID_CODE              (COMMAND0 + 19)

int InitWndCodPil ();

#define MAX_A 200
#define MAX_TEXT 1000

int n;

long m, w, w1, a[MAX_A], a1[MAX_A];

long clr, cod;

char tclr[MAX_TEXT], tcod[MAX_TEXT];

class TAppliWindowCodPil : public TWindow
{
public:
	/* declare control objects PT... */
	/* PTButton Button1; */
    PTWindowsObject pWndCtlObj[NCTL];
	HWND hWndCtlObj[NCTL];
 
	TAppliWindowCodPil (PTWindowsObject parent, LPSTR title) :
		TWindow (parent, title)
	{
		EnableKBHandler ();
        /* create control objects PT... with new T... */ 
		/* Button1 = new TButton (this, 101, "&Test 1", 50, 80, 40, 20, FALSE); */
	}

	LPSTR GetClassName ()
	{
    	return szAppNameCodPil;
	}

	virtual BOOL Create ()
	{
	BOOL status;
		status = TWindow::Create ();
		hWndCodPil = HWindow;
        /* create objects hWndCtlObj[i] with CreateWindow */ 
 		return status;
    }
	/* void HandleWndCtli (RTMessage Msg) = [ID_FIRST + COMMAND0 + i] { ... } */
};

TAppliWindowCodPil *WndCodPil;

class TAppliCodPil : public TApplication
{
public:
	TAppliCodPil (LPSTR AName, HINSTANCE hInstance, HINSTANCE hPrevInstance,
		LPSTR lpCmdLine, int nCmdShow) :
		TApplication (AName, hInstance, hPrevInstance, lpCmdLine, nCmdShow)
	{
	}
	virtual void InitMainWindow ()
	{
		WndCodPil = new TAppliWindowCodPil (NULL, Name);
		MainWindow = WndCodPil;
		WndCodPil->Attr.W = (WidCodPil)*2;
		WndCodPil->Attr.H = (HeiCodPil)*3/2+20;
    }
};


	/*
	LTEXT "n", -1, 8, 9, 16, 8, WS_CHILD | WS_VISIBLE | WS_GROUP
	LTEXT ("STATIC") "n" ID= -1 X= 8 Y= 9 W= 16 H= 8 STYLE= WS_CHILD | WS_VISIBLE | WS_GROUP
	*/
void HandleCtlCodPil1 (TMessage *pmsg)
{
/* Add code here */

}

	/*
	LTEXT "w", -1, 6, 43, 16, 8, WS_CHILD | WS_VISIBLE | WS_GROUP
	LTEXT ("STATIC") "w" ID= -1 X= 6 Y= 43 W= 16 H= 8 STYLE= WS_CHILD | WS_VISIBLE | WS_GROUP
	*/
void HandleCtlCodPil2 (TMessage *pmsg)
{
/* Add code here */

}

	/*
	LTEXT "m", -1, 5, 30, 16, 8, WS_CHILD | WS_VISIBLE | WS_GROUP
	LTEXT ("STATIC") "m" ID= -1 X= 5 Y= 30 W= 16 H= 8 STYLE= WS_CHILD | WS_VISIBLE | WS_GROUP
	*/
void HandleCtlCodPil3 (TMessage *pmsg)
{
/* Add code here */

}

	/*
	LTEXT "1/w", -1, 6, 60, 16, 8, WS_CHILD | WS_VISIBLE | WS_GROUP
	LTEXT ("STATIC") "1/w" ID= -1 X= 6 Y= 60 W= 16 H= 8 STYLE= WS_CHILD | WS_VISIBLE | WS_GROUP
	*/
void HandleCtlCodPil4 (TMessage *pmsg)
{
/* Add code here */

}

	/*
	EDITTEXT ID_N, 26, 7, 48, 12, ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP
	EDITTEXT ("EDIT") "" ID=ID_N X= 26 Y= 7 W= 48 H= 12 STYLE= ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL
	*/
void HandleCtlCodPilID_N (TMessage *pmsg)
{
/* Add code here */

}

	/*
	EDITTEXT ID_M, 25, 23, 50, 12, ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP
	EDITTEXT ("EDIT") "" ID=ID_M X= 25 Y= 23 W= 50 H= 12 STYLE= ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL
	*/
void HandleCtlCodPilID_M (TMessage *pmsg)
{
/* Add code here */

}

	/*
	EDITTEXT ID_W, 21, 41, 55, 12, ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP
	EDITTEXT ("EDIT") "" ID=ID_W X= 21 Y= 41 W= 55 H= 12 STYLE= ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL
	*/
void HandleCtlCodPilID_W (TMessage *pmsg)
{
/* Add code here */

}

	/*
	EDITTEXT ID_W1, 20, 57, 55, 12, ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP
	EDITTEXT ("EDIT") "" ID=ID_W1 X= 20 Y= 57 W= 55 H= 12 STYLE= ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL
	*/
void HandleCtlCodPilID_W1 (TMessage *pmsg)
{
/* Add code here */

}

	/*
	CONTROL "a", -1, "STATIC", SS_LEFT | WS_CHILD | WS_VISIBLE | WS_GROUP, 90, 5, 16, 8
	CONTROL ( "STATIC") "a" ID= -1 X= 90 Y= 5 W= 16 H= 8 STYLE= SS_LEFT | WS_CHILD | WS_VISIBLE | WS_GROUP
	*/
void HandleCtlCodPil9 (TMessage *pmsg)
{
/* Add code here */

}

	/*
	CONTROL "a'", -1, "STATIC", SS_LEFT | WS_CHILD | WS_VISIBLE | WS_GROUP, 137, 6, 16, 8
	CONTROL ( "STATIC") "a'" ID= -1 X= 137 Y= 6 W= 16 H= 8 STYLE= SS_LEFT | WS_CHILD | WS_VISIBLE | WS_GROUP
	*/
void HandleCtlCodPil10 (TMessage *pmsg)
{
/* Add code here */

}

	/*
	EDITTEXT ID_A, 83, 16, 43, 110, ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP
	EDITTEXT ("EDIT") "" ID=ID_A X= 83 Y= 16 W= 43 H= 110 STYLE= ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL
	*/
void HandleCtlCodPilID_A (TMessage *pmsg)
{
/* Add code here */

}

	/*
	EDITTEXT ID_A1, 133, 16, 43, 110, ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP
	EDITTEXT ("EDIT") "" ID=ID_A1 X= 133 Y= 16 W= 43 H= 110 STYLE= ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL
	*/
void HandleCtlCodPilID_A1 (TMessage *pmsg)
{
/* Add code here */

}

long InvMod (long n, long m)
{
long i;
	for (i=0; i<m; i++)
	{
		if (((n*i) % m) == 1)
        	return i;
	}
    return -1;
}

ScanVector (int *n, long *a, char *s)
{
char buf[60];
int i;
	*n = 0;
    i = 0;
	for (;; s)
	{
		if (*s == '\r' || *s==0)
		{
			buf[i] = 0;
			*a++ = atoi (buf);
            if (i > 0)
				(*n)++;
			if (*s == 0)
				break;
			s += 2;
			i = 0;
		}
		else
        	buf[i++] = *s++;
    }
}

FormatVector (int n, long *a, char *s)
{
int i;

	for (i=0; i<n; i++)
	{
		s += sprintf (s, "%d\r\n", *a++);
    }
}

	/*
	PUSHBUTTON "&Valider", ID_VALIDER, 18, 77, 46, 14, WS_CHILD | WS_VISIBLE | WS_TABSTOP
	PUSHBUTTON ("BUTTON") "&Valider" ID= ID_VALIDER X= 18 Y= 77 W= 46 H= 14 STYLE= WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON
	*/
void HandleCtlCodPilID_VALIDER (TMessage *pmsg)
{
/* Add code here */
char buf[1000];
int i;
	SendMessage (hCtlIdCodPil(ID_N), WM_GETTEXT, sizeof(buf), (LPARAM) (LPSTR) buf);
	n = atoi (buf);
	SendMessage (hCtlIdCodPil(ID_M), WM_GETTEXT, sizeof(buf), (LPARAM) (LPSTR) buf);
	m = atoi (buf);
	SendMessage (hCtlIdCodPil(ID_W), WM_GETTEXT, sizeof(buf), (LPARAM) (LPSTR) buf);
	w = atoi (buf);
	w1 = InvMod (w, m);
	sprintf (buf, "%d", w1);
	SendMessage (hCtlIdCodPil(ID_W1), WM_SETTEXT, 0, (LPARAM) (LPCSTR) buf);

	SendMessage (hCtlIdCodPil(ID_A1), WM_GETTEXT, sizeof(buf), (LPARAM) (LPSTR) buf);
	ScanVector (&n, a1, buf);

	sprintf (buf, "%d", n);
    SendMessage (hCtlIdCodPil(ID_N), WM_SETTEXT, 0, (LPARAM) (LPCSTR) buf);

	for (i=0; i<n; i++)
	{
		/* a1[i] = (a[i] * w1) % m; */
        a[i] = (a1[i] * w) % m;
	} 

	FormatVector (n, a, buf);
	SendMessage (hCtlIdCodPil(ID_A), WM_SETTEXT, 0, (LPARAM) (LPCSTR) buf);

}

	/*
	CONTROL "Clair", -1, "STATIC", SS_LEFT | WS_CHILD | WS_VISIBLE | WS_GROUP, 10, 132, 16, 8
	CONTROL ( "STATIC") "Clair" ID= -1 X= 10 Y= 132 W= 16 H= 8 STYLE= SS_LEFT | WS_CHILD | WS_VISIBLE | WS_GROUP
	*/
void HandleCtlCodPil14 (TMessage *pmsg)
{
/* Add code here */

}

	/*
	EDITTEXT ID_CLAIR, 34, 131, 185, 12, ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP
	EDITTEXT ("EDIT") "" ID=ID_CLAIR X= 34 Y= 131 W= 185 H= 12 STYLE= ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL
	*/
void HandleCtlCodPilID_CLAIR (TMessage *pmsg)
{
/* Add code here */

}

	/*
	PUSHBUTTON "&Coder", ID_CODER, 15, 99, 24, 14, WS_CHILD | WS_VISIBLE | WS_TABSTOP
	PUSHBUTTON ("BUTTON") "&Coder" ID= ID_CODER X= 15 Y= 99 W= 24 H= 14 STYLE= WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON
	*/
void HandleCtlCodPilID_CODER (TMessage *pmsg)
{
/* Add code here */
char buf[1000];
int i;
	SendMessage (hCtlIdCodPil(ID_CLAIR), WM_GETTEXT, sizeof(buf),
		(LPARAM) (LPSTR) buf);
	sscanf (buf, "%X", &clr);
	cod = 0;
	for (i=0; i<n; i++)
		cod += ((clr>>i) & 1) * a[i];
    sprintf (buf, "%X", cod);
	SendMessage (hCtlIdCodPil(ID_CODE), WM_SETTEXT, 0,
    	(LPARAM) (LPCSTR) buf);

}

	/*
	PUSHBUTTON "&Decoder", ID_DECODER, 47, 99, 31, 14, WS_CHILD | WS_VISIBLE | WS_TABSTOP
	PUSHBUTTON ("BUTTON") "&Decoder" ID= ID_DECODER X= 47 Y= 99 W= 31 H= 14 STYLE= WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON
	*/
void HandleCtlCodPilID_DECODER (TMessage *pmsg)
{
/* Add code here */
char buf[1000];
int i;
long cod1;
	SendMessage (hCtlIdCodPil(ID_CODE), WM_GETTEXT, sizeof(buf),
		(LPARAM) (LPSTR) buf);
	sscanf (buf, "%X", &cod);
	cod1 = (cod * w1) % m;

	clr = 0;
	for (i=n-1; i>=0; i--)
	{
		if (cod1 >= a1[i])
		{
			clr += 1<<i;
            cod1 -= a1[i];
        }
    }

	sprintf (buf, "%X", clr);
	SendMessage (hCtlIdCodPil(ID_CLAIR), WM_SETTEXT, 0,
    	(LPARAM) (LPCSTR) buf);
}

	/*
	CONTROL "Codé", -1, "STATIC", SS_LEFT | WS_CHILD | WS_VISIBLE | WS_GROUP, 8, 145, 16, 8
	CONTROL ( "STATIC") "Codé" ID= -1 X= 8 Y= 145 W= 16 H= 8 STYLE= SS_LEFT | WS_CHILD | WS_VISIBLE | WS_GROUP
	*/
void HandleCtlCodPil18 (TMessage *pmsg)
{
/* Add code here */

}

	/*
	EDITTEXT ID_CODE, 32, 149, 184, 12, ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP
	EDITTEXT ("EDIT") "" ID=ID_CODE X= 32 Y= 149 W= 184 H= 12 STYLE= ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL
	*/
void HandleCtlCodPilID_CODE (TMessage *pmsg)
{
/* Add code here */

}

	/*

	 () "" ID=000 X=000 Y=000 W=000 H=000 STYLE=000
	*/
void HandleCtlCodPil20 (TMessage *pmsg)
{
/* Add code here */

}

LRESULT CALLBACK WndProcCodPil (HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
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
			WndCodPil->HWindow = hWndCodPil;
        	/* create control objects hCtlObjCodPil[i] with CreateWindow */
			for (nCtl=1; nCtl<=NCTLCodPil; nCtl++)
			{
				hCtlObjCodPil[nCtl] = CreateWindow (ResCodPil[nCtl].ctlclass, ResCodPil[nCtl].caption,
					ResCodPil[nCtl].param_int[P_STYLE],
					ResCodPil[nCtl].param_int[P_X]*2, ResCodPil[nCtl].param_int[P_Y]*3/2, ResCodPil[nCtl].param_int[P_W]*2, ResCodPil[nCtl].param_int[P_H]*3/2,
					hWnd, COMMAND0+nCtl, hInst, NULL);
			}
			FocusCodPil = 1;
			SetFocus (hCtlObjCodPil[FocusCodPil]);
			InitWndCodPil ();
			break;
		case WM_CLOSE:
			for (nCtl=1; nCtl<=NCTLCodPil; nCtl++)
				DestroyWindow (hCtlObjCodPil[nCtl]);
			return DefWindowProc(hWnd, Message, wParam, lParam);
		case WM_SIZE:
			for (nCtl=1; nCtl<=NCTLCodPil; nCtl++)
			{
				MoveWindow (hCtlObjCodPil[nCtl],
					ResCodPil[nCtl].param_int[P_X]*LOWORD(lParam)/(WidCodPil),
					ResCodPil[nCtl].param_int[P_Y]*HIWORD(lParam)/(HeiCodPil),
					ResCodPil[nCtl].param_int[P_W]*LOWORD(lParam)/(WidCodPil),
					ResCodPil[nCtl].param_int[P_H]*HIWORD(lParam)/(HeiCodPil),
					TRUE);
			}
 			break;
		case WM_COMMAND:
			switch (wParam)
			{
				/* case COMMAND0+i:
					HandleCtlCodPili();
					break; */
				case COMMAND0+1:
					HandleCtlCodPil1 (&msg);
					break;
				case COMMAND0+2:
					HandleCtlCodPil2 (&msg);
					break;
				case COMMAND0+3:
					HandleCtlCodPil3 (&msg);
					break;
				case COMMAND0+4:
					HandleCtlCodPil4 (&msg);
					break;
				case COMMAND0+5:
					HandleCtlCodPilID_N (&msg);
					break;
				case COMMAND0+6:
					HandleCtlCodPilID_M (&msg);
					break;
				case COMMAND0+7:
					HandleCtlCodPilID_W (&msg);
					break;
				case COMMAND0+8:
					HandleCtlCodPilID_W1 (&msg);
					break;
				case COMMAND0+9:
					HandleCtlCodPil9 (&msg);
					break;
				case COMMAND0+10:
					HandleCtlCodPil10 (&msg);
					break;
				case COMMAND0+11:
					HandleCtlCodPilID_A (&msg);
					break;
				case COMMAND0+12:
					HandleCtlCodPilID_A1 (&msg);
					break;
				case COMMAND0+13:
					HandleCtlCodPilID_VALIDER (&msg);
					break;
				case COMMAND0+14:
					HandleCtlCodPil14 (&msg);
					break;
				case COMMAND0+15:
					HandleCtlCodPilID_CLAIR (&msg);
					break;
				case COMMAND0+16:
					HandleCtlCodPilID_CODER (&msg);
					break;
				case COMMAND0+17:
					HandleCtlCodPilID_DECODER (&msg);
					break;
				case COMMAND0+18:
					HandleCtlCodPil18 (&msg);
					break;
				case COMMAND0+19:
					HandleCtlCodPilID_CODE (&msg);
					break;
				case COMMAND0+20:
					HandleCtlCodPil20 (&msg);
					break;
 			}
			break;
		default:
			return DefWindowProc(hWnd, Message, wParam, lParam);
	}
	return NULL;
}

InitWndCodPil ()
{
/* Add code here */

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
			  while (!(GetWindowLong (hCtlObj[*pFocus], GWL_STYLE) & WS_TABSTOP) &&
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

	hInst = hInstance;
	InitResCodPil ();
	if (!hPrevInstance)
	{
		wndclass.style = CS_HREDRAW | CS_VREDRAW;
		wndclass.lpfnWndProc = WndProcCodPil;
		wndclass.cbClsExtra = 0;
		wndclass.cbWndExtra = 0;
		wndclass.hInstance = hInstance;
		wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
		wndclass.hbrBackground  = GetStockObject(WHITE_BRUSH);
		wndclass.lpszMenuName = NULL;
		wndclass.lpszClassName  = szAppNameCodPil;

		if (!RegisterClass(&wndclass))
			return FALSE;
	}

#ifdef OWLCodPil
	TAppliCodPil Appli (szAppNameCodPil, hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	Appli.Run ();
	return Appli.Status;
#else
	WndCodPil = new TAppliWindowCodPil (NULL, WndCaptionCodPil);
	hWndCodPil = CreateWindow (szAppNameCodPil, WndCaptionCodPil,
		WS_OVERLAPPEDWINDOW | WS_VSCROLL | WS_HSCROLL,
		CW_USEDEFAULT, CW_USEDEFAULT, (WidCodPil)*2, (HeiCodPil)*3/2+20,
		NULL, NULL, hInstance, NULL);

	if (!hWndCodPil)
		return FALSE;

	ShowWindow (hWndCodPil, nCmdShow);
	UpdateWindow (hWndCodPil);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!KBHandler (&msg, hWndCodPil, NCTLCodPil, ResCodPil, hCtlObjCodPil, &FocusCodPil))
		{
			TranslateMessage (&msg);
			DispatchMessage (&msg);
		}
	}

	return(msg.wParam);
#endif
}
