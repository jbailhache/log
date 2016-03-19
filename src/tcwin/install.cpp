/* Windows API source generated from resource file install.ctl */
 
#include <owl.h>
#include <window.h>
#include <string.h>

#include <stdlib.h>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dir.h>

#include <dde.h>

#include <time.h>

HANDLE hInst;
HWND hWndInstall;

#define NCTL 60
HWND hCtlObjInstall[NCTL];

static char szAppNameInstall[] = "Installation";

#define COMMAND0 100

#define hCtlIdInstall(id) (hCtlObjInstall[(id)-COMMAND0])

#define WidInstall (226)
#define HeiInstall (184)

#define WidInitInstall 320
#define HeiInitInstall 190

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

struct resource_item ResInstall[NCTL];

InitResInstall ()
{

	/*
	LTEXT "Installer à partir de", -1, 8, 14, 64, 8, WS_CHILD | WS_VISIBLE | WS_GROUP
	LTEXT ("STATIC") "Installer à partir de" ID= -1 X= 8 Y= 14 W= 64 H= 8 STYLE= WS_CHILD | WS_VISIBLE | WS_GROUP
	*/
	strcpy (ResInstall[1].name, "LTEXT");
	strcpy (ResInstall[1].ctlclass, "STATIC");
	strcpy (ResInstall[1].caption, "Installer à partir de");
	ResInstall[1].param_int[1] =  8;
	ResInstall[1].param_int[2] =  14;
	ResInstall[1].param_int[3] =  64;
	ResInstall[1].param_int[4] =  8;
	ResInstall[1].param_int[5] =  WS_CHILD | WS_VISIBLE | WS_GROUP;

	/*
	EDITTEXT ID_PROVENANCE, 81, 10, 135, 13, ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP
	EDITTEXT ("EDIT") "" ID=ID_PROVENANCE X= 81 Y= 10 W= 135 H= 13 STYLE= ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL
	*/
	strcpy (ResInstall[2].name, "EDITTEXT");
	strcpy (ResInstall[2].ctlclass, "EDIT");
	strcpy (ResInstall[2].caption, "");
	ResInstall[2].param_int[1] =  81;
	ResInstall[2].param_int[2] =  10;
	ResInstall[2].param_int[3] =  135;
	ResInstall[2].param_int[4] =  13;
	ResInstall[2].param_int[5] =  ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL;

	/*
	LTEXT "Copier dans", -1, 30, 30, 43, 8, WS_CHILD | WS_VISIBLE | WS_GROUP
	LTEXT ("STATIC") "Copier dans" ID= -1 X= 30 Y= 30 W= 43 H= 8 STYLE= WS_CHILD | WS_VISIBLE | WS_GROUP
	*/
	strcpy (ResInstall[3].name, "LTEXT");
	strcpy (ResInstall[3].ctlclass, "STATIC");
	strcpy (ResInstall[3].caption, "Copier dans");
	ResInstall[3].param_int[1] =  30;
	ResInstall[3].param_int[2] =  30;
	ResInstall[3].param_int[3] =  43;
	ResInstall[3].param_int[4] =  8;
	ResInstall[3].param_int[5] =  WS_CHILD | WS_VISIBLE | WS_GROUP;

	/*
	EDITTEXT ID_DESTINATION, 81, 28, 134, 14, ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP
	EDITTEXT ("EDIT") "" ID=ID_DESTINATION X= 81 Y= 28 W= 134 H= 14 STYLE= ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL
	*/
	strcpy (ResInstall[4].name, "EDITTEXT");
	strcpy (ResInstall[4].ctlclass, "EDIT");
	strcpy (ResInstall[4].caption, "");
	ResInstall[4].param_int[1] =  81;
	ResInstall[4].param_int[2] =  28;
	ResInstall[4].param_int[3] =  134;
	ResInstall[4].param_int[4] =  14;
	ResInstall[4].param_int[5] =  ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL;

	/*
	LTEXT "Groupe de programmes", -1, 1, 48, 82, 11, WS_CHILD | WS_VISIBLE | WS_GROUP
	LTEXT ("STATIC") "Groupe de programmes" ID= -1 X= 1 Y= 48 W= 82 H= 11 STYLE= WS_CHILD | WS_VISIBLE | WS_GROUP
	*/
	strcpy (ResInstall[5].name, "LTEXT");
	strcpy (ResInstall[5].ctlclass, "STATIC");
	strcpy (ResInstall[5].caption, "Groupe de programmes");
	ResInstall[5].param_int[1] =  1;
	ResInstall[5].param_int[2] =  48;
	ResInstall[5].param_int[3] =  82;
	ResInstall[5].param_int[4] =  11;
	ResInstall[5].param_int[5] =  WS_CHILD | WS_VISIBLE | WS_GROUP;

	/*
	EDITTEXT ID_GROUPE, 85, 47, 132, 14, ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP
	EDITTEXT ("EDIT") "" ID=ID_GROUPE X= 85 Y= 47 W= 132 H= 14 STYLE= ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL
	*/
	strcpy (ResInstall[6].name, "EDITTEXT");
	strcpy (ResInstall[6].ctlclass, "EDIT");
	strcpy (ResInstall[6].caption, "");
	ResInstall[6].param_int[1] =  85;
	ResInstall[6].param_int[2] =  47;
	ResInstall[6].param_int[3] =  132;
	ResInstall[6].param_int[4] =  14;
	ResInstall[6].param_int[5] =  ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL;

	/*
	LTEXT "Nom du programme", -1, 5, 67, 67, 8, WS_CHILD | WS_VISIBLE | WS_GROUP
	LTEXT ("STATIC") "Nom du programme" ID= -1 X= 5 Y= 67 W= 67 H= 8 STYLE= WS_CHILD | WS_VISIBLE | WS_GROUP
	*/
	strcpy (ResInstall[7].name, "LTEXT");
	strcpy (ResInstall[7].ctlclass, "STATIC");
	strcpy (ResInstall[7].caption, "Nom du programme");
	ResInstall[7].param_int[1] =  5;
	ResInstall[7].param_int[2] =  67;
	ResInstall[7].param_int[3] =  67;
	ResInstall[7].param_int[4] =  12;
	ResInstall[7].param_int[5] =  WS_CHILD | WS_VISIBLE | WS_GROUP;

	/*
	EDITTEXT ID_NOM, 85, 66, 129, 14, ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP
	EDITTEXT ("EDIT") "" ID=ID_NOM X= 85 Y= 66 W= 129 H= 14 STYLE= ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL
	*/
	strcpy (ResInstall[8].name, "EDITTEXT");
	strcpy (ResInstall[8].ctlclass, "EDIT");
	strcpy (ResInstall[8].caption, "");
	ResInstall[8].param_int[1] =  85;
	ResInstall[8].param_int[2] =  66;
	ResInstall[8].param_int[3] =  129;
	ResInstall[8].param_int[4] =  14;
	ResInstall[8].param_int[5] =  ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL;

	/*
	PUSHBUTTON "&Installer", ID_INSTALLER, 11, 87, 51, 14, WS_CHILD | WS_VISIBLE | WS_TABSTOP
	PUSHBUTTON ("BUTTON") "&Installer" ID= ID_INSTALLER X= 11 Y= 87 W= 51 H= 14 STYLE= WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON
	*/
	strcpy (ResInstall[9].name, "PUSHBUTTON");
	strcpy (ResInstall[9].ctlclass, "BUTTON");
	strcpy (ResInstall[9].caption, "Installer");
	ResInstall[9].param_int[1] =  11;
	ResInstall[9].param_int[2] =  87;
	ResInstall[9].param_int[3] =  51;
	ResInstall[9].param_int[4] =  14;
	ResInstall[9].param_int[5] =  WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON;

	/*
	PUSHBUTTON "&Annuler", ID_ANNULER, 75, 87, 43, 14, WS_CHILD | WS_VISIBLE | WS_TABSTOP
	PUSHBUTTON ("BUTTON") "&Annuler" ID= ID_ANNULER X= 75 Y= 87 W= 43 H= 14 STYLE= WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON
	*/
	strcpy (ResInstall[10].name, "PUSHBUTTON");
	strcpy (ResInstall[10].ctlclass, "BUTTON");
	strcpy (ResInstall[10].caption, "Quitter");
	ResInstall[10].param_int[1] =  75;
	ResInstall[10].param_int[2] =  87;
	ResInstall[10].param_int[3] =  43;
	ResInstall[10].param_int[4] =  14;
	ResInstall[10].param_int[5] =  WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON;

	/*
	LISTBOX ID_MESSAGES, 14, 112, 200, 60, LBS_NOTIFY | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL
	LISTBOX () "" ID=ID_MESSAGES X= 14 Y= 112 W= 200 H= 60 STYLE= LBS_NOTIFY | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL
	*/
	strcpy (ResInstall[11].name, "LISTBOX");
	strcpy (ResInstall[11].ctlclass, "LISTBOX");
	strcpy (ResInstall[11].caption, "");
	ResInstall[11].param_int[1] =  14;
	ResInstall[11].param_int[2] =  112;
	ResInstall[11].param_int[3] =  200;
	ResInstall[11].param_int[4] =  60;
	ResInstall[11].param_int[5] =  LBS_NOTIFY | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL;

#define NCTLInstall 11


	return 0;
}

#define ID_PROVENANCE        (COMMAND0 + 2)
#define ID_DESTINATION       (COMMAND0 + 4)
#define ID_GROUPE            (COMMAND0 + 6)
#define ID_NOM               (COMMAND0 + 8)
#define  ID_INSTALLER        (COMMAND0 + 9)
#define  ID_ANNULER          (COMMAND0 + 10)
#define ID_MESSAGES          (COMMAND0 + 11)

InitWndInstall ()
{
/* Add code here */
	SendMessage (hCtlIdInstall(ID_PROVENANCE), WM_SETTEXT, 0,
		(LPARAM) (LPCSTR) "A:\\SUPERZAM.EXE");
	SendMessage (hCtlIdInstall(ID_DESTINATION), WM_SETTEXT, 0,
		(LPARAM) (LPCSTR) "C:\\SUPERZAM\\SUPERZAM.EXE");
	SendMessage (hCtlIdInstall(ID_GROUPE), WM_SETTEXT, 0,
		(LPARAM) (LPCSTR) "Superzam");
	SendMessage (hCtlIdInstall(ID_NOM), WM_SETTEXT, 0,
	(LPARAM) (LPCSTR) "Superzam");

	return 0;
}

Message (char *buf)
{
MSG msg;
	SendMessage (hCtlIdInstall(ID_MESSAGES), LB_ADDSTRING, 0,
		(LPARAM) (LPCSTR) buf);

	while (PeekMessage (&msg, NULL, 0, 0, PM_REMOVE))
    {
	/* GetMessage (&msg, NULL, 0, 0); */

	TranslateMessage (&msg);
/*
	msg.hwnd = hCtlIdInstall(ID_MESSAGES);
	msg.message = LB_ADDSTRING;
	msg.wParam = 0;
	msg.lParam = (LPARAM) (LPCSTR) buf;
	time ((time_t *)&msg.time);
	GetCursorPos (&msg.pt);
*/
	DispatchMessage (&msg);
	}
/*
	SendMessage (hWndInstall, WM_PAINT, 0, 0);
*/
}

#define BLOCKSIZE 256

TestDir (char *path)
{
char dir[200];
int l, i;
int fh;
int status;
	l = strlen (path);
	for (i=l-1; i>=0 && path[i] != '\\'; i--);
	if (i == -1)
		return 0;
	strncpy (dir, path, i);
	dir[i] = 0;
	TestDir (dir);
	/* creer le repertoire dir s'il n'existe pas */
	status = mkdir (dir);
}

ExecuteDDE (char *s)
{
HGLOBAL hCommand;
void FAR *pCommand;
int size;
	size = strlen(s) + 1;
	hCommand = GlobalAlloc (GMEM_DDESHARE, size);
	pCommand = GlobalLock (hCommand);
	_fmemcpy (pCommand, s, size);

	SendMessage (-1, WM_DDE_EXECUTE, hWndInstall, MAKELPARAM (0, hCommand));

	GlobalUnlock (hCommand);
    GlobalFree (hCommand);
}



Install ()
{
int fh_in, fh_out;
char filename_in[200], filename_out[200];
char buf[1000];
int nr, nw;
ATOM aApp, aTopic;
char groupe[120], nom[120];
MSG msg;

	SendMessage (hCtlIdInstall(ID_PROVENANCE), WM_GETTEXT,
		sizeof(filename_in), (LPARAM) (LPSTR) filename_in);

	fh_in = open (filename_in, O_BINARY | O_RDONLY);
	if (fh_in < 0)
	{
		sprintf (buf, "Je ne peux pas accéder au fichier %s\n%s",
			filename_in, sys_errlist[errno]);
		MessageBox (hWndInstall, buf, "Problème", MB_OK);
		return errno;
    }

	SendMessage (hCtlIdInstall(ID_DESTINATION), WM_GETTEXT,
		sizeof(filename_out), (LPARAM) (LPSTR) filename_out);
 
	TestDir (filename_out);

	fh_out = open (filename_out, O_BINARY | O_CREAT | O_TRUNC,
		S_IREAD | S_IWRITE);
	if (fh_out < 0)
	{
		sprintf (buf, "Je ne peux pas créer le fichier %s\n%s",
			filename_out, sys_errlist[errno]);
		MessageBox (hWndInstall, buf, "Problème", MB_OK);
		return errno;
	}

	sprintf (buf, "Copie de %s vers %s en cours...",
		filename_in, filename_out);

	Message (buf);

	for (;;)
	{
		nr = read (fh_in, buf, BLOCKSIZE);
		if (nr <= 0)
			break;
		nw = write (fh_out, buf, nr);
		if (nw != nr)
		{
			sprintf (buf, "Je ne peux pas copier %s dans %s\n%s",
				filename_in, filename_out, sys_errlist[errno]);
			MessageBox (hWndInstall, buf, "Problème", MB_OK);
		return errno;
	}
    }
	
	sprintf (buf, "Copie de %s vers %s terminée",
		filename_in, filename_out);
	Message (buf);
	/*SendMessage (hCtlIdInstall(ID_MESSAGES), LB_ADDSTRING, 0,
		(LPARAM) (LPCSTR) buf);*/

	aApp = GlobalAddAtom ("PROGMAN");
    aTopic = GlobalAddAtom ("PROGMAN");
	SendMessage (-1, WM_DDE_INITIATE, hWndInstall, MAKELONG (aApp, aTopic));
/*
	send ("[CreateGroup(\"Test Group\")]");
	send ("[AddItem(c:\\d1\\test, Test)]");
	send ("[ShowGroup(\"Test Group\", 1)]");
*/
	sprintf (buf, "Installation en cours...",
		filename_in, filename_out);
    Message (buf);
	/*SendMessage (hCtlIdInstall(ID_MESSAGES), LB_ADDSTRING, 0,
		(LPARAM) (LPCSTR) buf);*/

	SendMessage (hCtlIdInstall(ID_GROUPE), WM_GETTEXT,
		sizeof(groupe), (LPARAM) (LPSTR) groupe);
	sprintf (buf, "[CreateGroup(\"%s\")]", groupe);
	ExecuteDDE (buf);

	SendMessage (hCtlIdInstall(ID_NOM), WM_GETTEXT,
		sizeof(nom), (LPARAM) (LPSTR) nom);
	sprintf (buf, "[ReplaceItem(\"%s\")]", nom);
	ExecuteDDE (buf);

	sprintf (buf, "[AddItem(%s, \"%s\")]", filename_out, nom);
	ExecuteDDE (buf);

	sprintf (buf, "[ShowGroup(\"%s\", 1)]", groupe);
	ExecuteDDE (buf);

	SendMessage (-1, WM_DDE_TERMINATE, hWndInstall, 0);

	sprintf (buf, "Installation terminée.",
		filename_in, filename_out);
	Message (buf);
	/*SendMessage (hCtlIdInstall(ID_MESSAGES), LB_ADDSTRING, 0,
		(LPARAM) (LPCSTR) buf);*/

	return 0;
}

	/*
	LTEXT "Installer à partir de", -1, 8, 14, 64, 8, WS_CHILD | WS_VISIBLE | WS_GROUP
	LTEXT ("STATIC") "Installer à partir de" ID= -1 X= 8 Y= 14 W= 64 H= 8 STYLE= WS_CHILD | WS_VISIBLE | WS_GROUP
	*/

void HandleCtlInstall1 ()
{
/* Add code here */

}

	/*
	EDITTEXT ID_PROVENANCE, 81, 10, 135, 13, ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP
	EDITTEXT ("EDIT") "" ID=ID_PROVENANCE X= 81 Y= 10 W= 135 H= 13 STYLE= ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL
	*/

void HandleCtlInstall2 ()
{
/* Add code here */

}

	/*
	LTEXT "Copier dans", -1, 30, 30, 43, 8, WS_CHILD | WS_VISIBLE | WS_GROUP
	LTEXT ("STATIC") "Copier dans" ID= -1 X= 30 Y= 30 W= 43 H= 8 STYLE= WS_CHILD | WS_VISIBLE | WS_GROUP
	*/

void HandleCtlInstall3 ()
{
/* Add code here */

}

	/*
	EDITTEXT ID_DESTINATION, 81, 28, 134, 14, ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP
	EDITTEXT ("EDIT") "" ID=ID_DESTINATION X= 81 Y= 28 W= 134 H= 14 STYLE= ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL
	*/

void HandleCtlInstall4 ()
{
/* Add code here */

}

	/*
	LTEXT "Groupe de programmes", -1, 1, 48, 82, 11, WS_CHILD | WS_VISIBLE | WS_GROUP
	LTEXT ("STATIC") "Groupe de programmes" ID= -1 X= 1 Y= 48 W= 82 H= 11 STYLE= WS_CHILD | WS_VISIBLE | WS_GROUP
	*/

void HandleCtlInstall5 ()
{
/* Add code here */

}

	/*
	EDITTEXT ID_GROUPE, 85, 47, 132, 14, ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP
	EDITTEXT ("EDIT") "" ID=ID_GROUPE X= 85 Y= 47 W= 132 H= 14 STYLE= ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL
	*/

void HandleCtlInstall6 ()
{
/* Add code here */

}

	/*
	LTEXT "Nom du programme", -1, 5, 67, 67, 8, WS_CHILD | WS_VISIBLE | WS_GROUP
	LTEXT ("STATIC") "Nom du programme" ID= -1 X= 5 Y= 67 W= 67 H= 8 STYLE= WS_CHILD | WS_VISIBLE | WS_GROUP
	*/

void HandleCtlInstall7 ()
{
/* Add code here */

}

	/*
	EDITTEXT ID_NOM, 85, 66, 129, 14, ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP
	EDITTEXT ("EDIT") "" ID=ID_NOM X= 85 Y= 66 W= 129 H= 14 STYLE= ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL
	*/

void HandleCtlInstall8 ()
{
/* Add code here */

}

	/*
	PUSHBUTTON "&Installer", ID_INSTALLER, 11, 87, 51, 14, WS_CHILD | WS_VISIBLE | WS_TABSTOP
	PUSHBUTTON ("BUTTON") "&Installer" ID= ID_INSTALLER X= 11 Y= 87 W= 51 H= 14 STYLE= WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON
	*/

void HandleCtlInstall9 ()
{
/* Add code here */
	Install ();
}

	/*
	PUSHBUTTON "&Annuler", ID_ANNULER, 75, 87, 43, 14, WS_CHILD | WS_VISIBLE | WS_TABSTOP
	PUSHBUTTON ("BUTTON") "&Annuler" ID= ID_ANNULER X= 75 Y= 87 W= 43 H= 14 STYLE= WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON
	*/

void HandleCtlInstall10 ()
{
/* Add code here */
	PostQuitMessage (0);
}

	/*
	LISTBOX ID_MESSAGES, 14, 112, 200, 60, LBS_NOTIFY | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL
	LISTBOX () "" ID=ID_MESSAGES X= 14 Y= 112 W= 200 H= 60 STYLE= LBS_NOTIFY | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL
	*/

void HandleCtlInstall11 ()
{
/* Add code here */

}

	/*

	 () "" ID=000 X=000 Y=000 W=000 H=000 STYLE=000
	*/

void HandleCtlInstall12 ()
{
/* Add code here */

}

LRESULT CALLBACK WndProcInstall (HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
int nCtl;
	switch (Message)
	{
		case WM_CREATE:
		/* create control objects hCtlObjInstall[i] with CreateWindow */
			for (nCtl=1; nCtl<=NCTLInstall; nCtl++)
			{
				hCtlObjInstall[nCtl] = CreateWindow (ResInstall[nCtl].ctlclass, ResInstall[nCtl].caption,
					ResInstall[nCtl].param_int[P_STYLE],
					ResInstall[nCtl].param_int[P_X]*2, ResInstall[nCtl].param_int[P_Y]*3/2, ResInstall[nCtl].param_int[P_W]*2, ResInstall[nCtl].param_int[P_H]*3/2,
					hWnd, COMMAND0+nCtl, hInst, NULL);
			}
			InitWndInstall ();
			break;
		case WM_CLOSE:
			for (nCtl=1; nCtl<=NCTLInstall; nCtl++)
				DestroyWindow (hCtlObjInstall[nCtl]);
			return DefWindowProc(hWnd, Message, wParam, lParam);
			break;
		case WM_SIZE:
			for (nCtl=1; nCtl<=NCTLInstall; nCtl++)
			{
				MoveWindow (hCtlObjInstall[nCtl],
					ResInstall[nCtl].param_int[P_X]*LOWORD(lParam)/(WidInstall),
					ResInstall[nCtl].param_int[P_Y]*HIWORD(lParam)/(HeiInstall),
					ResInstall[nCtl].param_int[P_W]*LOWORD(lParam)/(WidInstall),
					ResInstall[nCtl].param_int[P_H]*HIWORD(lParam)/(HeiInstall),
					TRUE);
			}
			break;
		case WM_COMMAND:
			switch (wParam)
			{
				/* case COMMAND0+i:
					HandleCtlInstalli();
					break; */
				case COMMAND0+1:
					HandleCtlInstall1();
					break;
				case COMMAND0+2:
					HandleCtlInstall2();
					break;
				case COMMAND0+3:
					HandleCtlInstall3();
					break;
				case COMMAND0+4:
					HandleCtlInstall4();
					break;
				case COMMAND0+5:
					HandleCtlInstall5();
					break;
				case COMMAND0+6:
					HandleCtlInstall6();
					break;
				case COMMAND0+7:
					HandleCtlInstall7();
					break;
				case COMMAND0+8:
					HandleCtlInstall8();
					break;
				case COMMAND0+9:
					HandleCtlInstall9();
					break;
				case COMMAND0+10:
					HandleCtlInstall10();
					break;
				case COMMAND0+11:
					HandleCtlInstall11();
					break;
				case COMMAND0+12:
					HandleCtlInstall12();
					break;
			}
			break;
		default:
			return DefWindowProc(hWnd, Message, wParam, lParam);
	}
	return NULL;
}

class TAppliWindowInstall : public TWindow
{
public:
	/* declare control objects PT... */
	/* PTButton Button1; */
    PTWindowsObject pWndCtlObj[NCTL];
	HWND hWndCtlObj[NCTL];
 
	TAppliWindowInstall (PTWindowsObject parent, LPSTR title) :
		TWindow (parent, title)
	{
		EnableKBHandler ();
	/* create control objects PT... with new T... */ 
		/* Button1 = new TButton (this, 101, "&Test 1", 50, 80, 40, 20, FALSE); */
	}

	LPSTR GetClassName ()
	{
	return szAppNameInstall;
	}

	virtual BOOL Create ()
	{
	BOOL status;
		status = TWindow::Create ();
		hWndInstall = HWindow;
	/* create objects hWndCtlObj[i] with CreateWindow */ 
		return status;
    }
	/* void HandleWndCtli (RTMessage Msg) = [ID_FIRST + COMMAND0 + i] { ... } */
};

TAppliWindowInstall *WndInstall;

class TAppliInstall : public TApplication
{
public:
	TAppliInstall (LPSTR AName, HINSTANCE hInstance, HINSTANCE hPrevInstance,
		LPSTR lpCmdLine, int nCmdShow) :
		TApplication (AName, hInstance, hPrevInstance, lpCmdLine, nCmdShow)
	{
	}
	virtual void InitMainWindow ()
	{
		WndInstall = new TAppliWindowInstall (NULL, Name);
		MainWindow = WndInstall;
		WndInstall->Attr.W = (WidInstall)*2;
		WndInstall->Attr.H = (HeiInstall)*3/2+20;
    }
};

int PASCAL WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
WNDCLASS wndclass;
MSG msg;

	InitResInstall ();
	if (!hPrevInstance)
	{
		wndclass.style = CS_HREDRAW | CS_VREDRAW;
		wndclass.lpfnWndProc = WndProcInstall;
		wndclass.cbClsExtra = 0;
		wndclass.cbWndExtra = 0;
		wndclass.hInstance = hInstance;
		wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
		wndclass.hbrBackground  = GetStockObject(WHITE_BRUSH);
		wndclass.lpszMenuName = NULL;
		wndclass.lpszClassName  = szAppNameInstall;

		if (!RegisterClass(&wndclass))
			return FALSE;
	}

#ifdef OWLInstall
	hInst = hInstance;
	TAppliInstall Appli (szAppNameInstall, hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	Appli.Run ();
	return Appli.Status;
#else
	hWndInstall = CreateWindow (szAppNameInstall, "Installation",
		WS_OVERLAPPEDWINDOW /*| WS_VSCROLL | WS_HSCROLL*/,
		CW_USEDEFAULT, CW_USEDEFAULT, (WidInitInstall)*2, (HeiInitInstall)*3/2+20,
		NULL, NULL, hInstance, NULL);

	if (!hWndInstall)
		return FALSE;

	ShowWindow (hWndInstall, nCmdShow);
	UpdateWindow (hWndInstall);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage (&msg);
		DispatchMessage (&msg);
	}

	return(msg.wParam);
#endif
}
