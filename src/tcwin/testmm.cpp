/* Windows API source generated from resource file testmm.ctl */

#include <owl.h>
#include <window.h>
#include <string.h>
#include <ctype.h>

#include <stdio.h>

#include <mmsystem.h>


HANDLE hInst;
HWND hWndTest;

#define NCTL 60
HWND hCtlObjTest[NCTL];

int FocusTest;

static char szAppNameTest[] = "AppliTest";
static char WndCaptionTest[] = "Test";

#define COMMAND0 100

#define hCtlIdTest(id) (hCtlObjTest[(id)-COMMAND0])

#define WidTest (216)
#define HeiTest (161)

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

struct resource_item ResTest[NCTL];

InitResTest ()
{

	/*
	EDITTEXT ID_TEXT, 8, 22, 116, 12, ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP
	EDITTEXT ("EDIT") "" ID=ID_TEXT X= 8 Y= 22 W= 116 H= 12 STYLE= ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL
	*/
	strcpy (ResTest[1].name, "EDITTEXT");
	strcpy (ResTest[1].ctlclass, "EDIT");
	strcpy (ResTest[1].caption, "");
	ResTest[1].param_int[1] =  8;
	ResTest[1].param_int[2] =  22;
	ResTest[1].param_int[3] =  116;
	ResTest[1].param_int[4] =  12;
	ResTest[1].param_int[5] =  ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL;

	/*
	LTEXT "Test", -1, 10, 6, 16, 8
	LTEXT ("STATIC") "Test" ID= -1 X= 10 Y= 6 W= 16 H= 8 STYLE=000
	*/
	strcpy (ResTest[2].name, "LTEXT");
	strcpy (ResTest[2].ctlclass, "STATIC");
	strcpy (ResTest[2].caption, "Test");
	ResTest[2].param_int[1] =  10;
	ResTest[2].param_int[2] =  6;
	ResTest[2].param_int[3] =  16;
	ResTest[2].param_int[4] =  8;
	ResTest[2].param_int[5] = WS_CHILD | WS_VISIBLE | WS_GROUP;

	/*
	PUSHBUTTON "&Play", ID_VALIDER, 21, 45, 32, 14
	PUSHBUTTON ("BUTTON") "&Play" ID= ID_VALIDER X= 21 Y= 45 W= 32 H= 14 STYLE=000 | BS_PUSHBUTTON
	*/
	strcpy (ResTest[3].name, "PUSHBUTTON");
	strcpy (ResTest[3].ctlclass, "BUTTON");
	strcpy (ResTest[3].caption, "&Play");
	ResTest[3].param_int[1] =  21;
	ResTest[3].param_int[2] =  45;
	ResTest[3].param_int[3] =  32;
	ResTest[3].param_int[4] =  14;
 	ResTest[3].param_int[5] =  WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON;


	/*
	LTEXT "Command", -1, 7, 73, 34, 8
	LTEXT ("STATIC") "Command" ID= -1 X= 7 Y= 73 W= 34 H= 8 STYLE=000
	*/
	strcpy (ResTest[4].name, "LTEXT");
	strcpy (ResTest[4].ctlclass, "STATIC");
	strcpy (ResTest[4].caption, "Command");
	ResTest[4].param_int[1] =  7;
	ResTest[4].param_int[2] =  73;
	ResTest[4].param_int[3] =  34;
	ResTest[4].param_int[4] =  8;
	ResTest[4].param_int[5] = WS_CHILD | WS_VISIBLE | WS_GROUP;

	/*
	EDITTEXT ID_COMMAND, 49, 70, 151, 12, ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP
	EDITTEXT ("EDIT") "" ID=ID_COMMAND X= 49 Y= 70 W= 151 H= 12 STYLE= ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL
	*/
	strcpy (ResTest[5].name, "EDITTEXT");
	strcpy (ResTest[5].ctlclass, "EDIT");
	strcpy (ResTest[5].caption, "");
	ResTest[5].param_int[1] =  49;
	ResTest[5].param_int[2] =  70;
	ResTest[5].param_int[3] =  151;
	ResTest[5].param_int[4] =  12;
	ResTest[5].param_int[5] =  ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL;

	/*
	LTEXT "Answer", -1, 8, 110, 30, 8
	LTEXT ("STATIC") "Answer" ID= -1 X= 8 Y= 110 W= 30 H= 8 STYLE=000
	*/
	strcpy (ResTest[6].name, "LTEXT");
	strcpy (ResTest[6].ctlclass, "STATIC");
	strcpy (ResTest[6].caption, "Answer");
	ResTest[6].param_int[1] =  8;
	ResTest[6].param_int[2] =  110;
	ResTest[6].param_int[3] =  30;
	ResTest[6].param_int[4] =  8;
	ResTest[6].param_int[5] = WS_CHILD | WS_VISIBLE | WS_GROUP;

	/*
	EDITTEXT ID_ANSWER, 50, 107, 144, 12, ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP
	EDITTEXT ("EDIT") "" ID=ID_ANSWER X= 50 Y= 107 W= 144 H= 12 STYLE= ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL
	*/
	strcpy (ResTest[7].name, "EDITTEXT");
	strcpy (ResTest[7].ctlclass, "EDIT");
	strcpy (ResTest[7].caption, "");
	ResTest[7].param_int[1] =  50;
	ResTest[7].param_int[2] =  107;
	ResTest[7].param_int[3] =  144;
	ResTest[7].param_int[4] =  12;
	ResTest[7].param_int[5] =  ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL;

	/*
	PUSHBUTTON "&Send", ID_SEND, 10, 89, 33, 14, WS_CHILD | WS_VISIBLE | WS_TABSTOP
	PUSHBUTTON ("BUTTON") "&Send" ID= ID_SEND X= 10 Y= 89 W= 33 H= 14 STYLE= WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON
	*/
	strcpy (ResTest[8].name, "PUSHBUTTON");
	strcpy (ResTest[8].ctlclass, "BUTTON");
	strcpy (ResTest[8].caption, "&Send");
	ResTest[8].param_int[1] =  10;
	ResTest[8].param_int[2] =  89;
	ResTest[8].param_int[3] =  33;
	ResTest[8].param_int[4] =  14;
	ResTest[8].param_int[5] =  WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON;



#define NCTLTest 8


	return 0;
}

#define ID_TEXT              (COMMAND0 + 1)
#define  ID_VALIDER          (COMMAND0 + 3)
#define ID_COMMAND           (COMMAND0 + 5)
#define ID_ANSWER            (COMMAND0 + 7)
#define  ID_SEND             (COMMAND0 + 8)

int InitWndTest ();

UINT wDeviceID;

testmm ()
{
MCI_OPEN_PARMS mciOpenParms;
MCI_ANIM_PLAY_PARMS mciPlayParms;
DWORD status;
char filename[200];
char ErrorText[300];
char buf[400];
	SendMessage (hCtlIdTest(ID_TEXT), WM_GETTEXT, sizeof(filename),
		(LPARAM) (LPSTR) filename);
	mciOpenParms.lpstrElementName = filename;
	status = mciSendCommand (NULL, MCI_OPEN, MCI_OPEN_ELEMENT,
		(DWORD) (LPVOID) &mciOpenParms);
	if (status)
	{
		mciGetErrorString (status, ErrorText, sizeof(ErrorText));
        sprintf (buf, "Impossible d'ouvrir le périphérique\n%s", ErrorText);
		MessageBox (hWndTest, buf, "Erreur", MB_OK); 
	}
	wDeviceID = mciOpenParms.wDeviceID;

	status = mciSendCommand (wDeviceID, MCI_PLAY, /*NULL*/ MCI_NOTIFY,
		(DWORD) (LPVOID) &mciPlayParms);
	if (status)
	{
		mciGetErrorString (status, ErrorText, sizeof(ErrorText));
        sprintf (buf, "Impossible d'interpréter le fichier\n%s", ErrorText);
		MessageBox (hWndTest, buf, "Erreur", MB_OK);
	}
}

class TAppliWindowTest : public TWindow
{
public:
	/* declare control objects PT... */
	/* PTButton Button1; */
    PTWindowsObject pWndCtlObj[NCTL];
	HWND hWndCtlObj[NCTL];
 
	TAppliWindowTest (PTWindowsObject parent, LPSTR title) :
		TWindow (parent, title)
	{
		EnableKBHandler ();
        /* create control objects PT... with new T... */ 
		/* Button1 = new TButton (this, 101, "&Test 1", 50, 80, 40, 20, FALSE); */
	}

	LPSTR GetClassName ()
	{
    	return szAppNameTest;
	}

	virtual BOOL Create ()
	{
	BOOL status;
		status = TWindow::Create ();
		hWndTest = HWindow;
        /* create objects hWndCtlObj[i] with CreateWindow */ 
 		return status;
    }
	/* void HandleWndCtli (RTMessage Msg) = [ID_FIRST + COMMAND0 + i] { ... } */
};

TAppliWindowTest *WndTest;

class TAppliTest : public TApplication
{
public:
	TAppliTest (LPSTR AName, HINSTANCE hInstance, HINSTANCE hPrevInstance,
		LPSTR lpCmdLine, int nCmdShow) :
		TApplication (AName, hInstance, hPrevInstance, lpCmdLine, nCmdShow)
	{
	}
	virtual void InitMainWindow ()
	{
		WndTest = new TAppliWindowTest (NULL, Name);
		MainWindow = WndTest;
		WndTest->Attr.W = (WidTest)*2;
		WndTest->Attr.H = (HeiTest)*3/2+20;
    }
};


	/*
	EDITTEXT ID_TEXT, 8, 22, 116, 12, ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP
	EDITTEXT ("EDIT") "" ID=ID_TEXT X= 8 Y= 22 W= 116 H= 12 STYLE= ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL
	*/
void HandleCtlTestID_TEXT (TMessage *pmsg)
{
/* Add code here */

}

	/*
	CONTROL "Test", -1, "STATIC", SS_LEFT | WS_CHILD | WS_VISIBLE | WS_GROUP, 10, 6, 16, 8
	CONTROL ( "STATIC") "Test" ID= -1 X= 10 Y= 6 W= 16 H= 8 STYLE= SS_LEFT | WS_CHILD | WS_VISIBLE | WS_GROUP
	*/
void HandleCtlTest2 (TMessage *pmsg)
{
/* Add code here */

}

	/*
	PUSHBUTTON "&Valider", ID_VALIDER, 21, 45, 32, 14, WS_CHILD | WS_VISIBLE | WS_TABSTOP
	PUSHBUTTON ("BUTTON") "&Valider" ID= ID_VALIDER X= 21 Y= 45 W= 32 H= 14 STYLE= WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON
	*/
void HandleCtlTestID_VALIDER (TMessage *pmsg)
{
/* Add code here */
	testmm ();

}

	/*
	CONTROL "Command", -1, "STATIC", SS_LEFT | WS_CHILD | WS_VISIBLE | WS_GROUP, 7, 73, 34, 8
	CONTROL ( "STATIC") "Command" ID= -1 X= 7 Y= 73 W= 34 H= 8 STYLE= SS_LEFT | WS_CHILD | WS_VISIBLE | WS_GROUP
	*/
void HandleCtlTest4 (TMessage *pmsg)
{
/* Add code here */

}

	/*
	EDITTEXT ID_COMMAND, 49, 70, 151, 12, ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP
	EDITTEXT ("EDIT") "" ID=ID_COMMAND X= 49 Y= 70 W= 151 H= 12 STYLE= ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL
	*/
void HandleCtlTestID_COMMAND (TMessage *pmsg)
{
/* Add code here */

}

	/*
	CONTROL "Answer", -1, "STATIC", SS_LEFT | WS_CHILD | WS_VISIBLE | WS_GROUP, 9, 91, 30, 8
	CONTROL ( "STATIC") "Answer" ID= -1 X= 9 Y= 91 W= 30 H= 8 STYLE= SS_LEFT | WS_CHILD | WS_VISIBLE | WS_GROUP
	*/
void HandleCtlTest6 (TMessage *pmsg)
{
/* Add code here */

}

	/*
	EDITTEXT ID_ANSWER, 47, 89, 144, 12, ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP
	EDITTEXT ("EDIT") "" ID=ID_ANSWER X= 47 Y= 89 W= 144 H= 12 STYLE= ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL
	*/
void HandleCtlTestID_ANSWER (TMessage *pmsg)
{
/* Add code here */

}


	/*
	PUSHBUTTON "&Send", ID_SEND, 10, 89, 33, 14, WS_CHILD | WS_VISIBLE | WS_TABSTOP
	PUSHBUTTON ("BUTTON") "&Send" ID= ID_SEND X= 10 Y= 89 W= 33 H= 14 STYLE= WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON
	*/
void HandleCtlTestID_SEND (TMessage *pmsg)
{
/* Add code here */
DWORD status;
char command[300], answer[400], ErrorText[300], buf[400];
	SendMessage (hCtlIdTest(ID_COMMAND), WM_GETTEXT, sizeof(command),
		(LPARAM) (LPSTR) command);
	status = mciSendString (command, answer, sizeof(answer), hWndTest);
	if (status)
	{
		mciGetErrorString (status, ErrorText, sizeof(ErrorText));
		sprintf (buf, "Impossible d'envoyer la commande\n%s", ErrorText);
		MessageBox (hWndTest, buf, "Erreur", MB_OK);
	}
	SendMessage (hCtlIdTest(ID_ANSWER), WM_SETTEXT, 0,
		(LPARAM) (LPCSTR) answer);

}

LRESULT CALLBACK WndProcTest (HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
int nCtl;
struct TMessage msg;
char buf[200];
	msg.Receiver = hWnd;
	msg.Message = Message;
	msg.WParam = wParam;
	msg.LParam = lParam;
	msg.Result = 0;
	switch (Message)
	{
		case WM_CREATE:
			WndTest->HWindow = hWndTest;
        	/* create control objects hCtlObjTest[i] with CreateWindow */
			for (nCtl=1; nCtl<=NCTLTest; nCtl++)
			{
				hCtlObjTest[nCtl] = CreateWindow (ResTest[nCtl].ctlclass, ResTest[nCtl].caption,
					ResTest[nCtl].param_int[P_STYLE],
					ResTest[nCtl].param_int[P_X]*2, ResTest[nCtl].param_int[P_Y]*3/2, ResTest[nCtl].param_int[P_W]*2, ResTest[nCtl].param_int[P_H]*3/2,
					hWnd, COMMAND0+nCtl, hInst, NULL);
			}
			FocusTest = 1;
			SetFocus (hCtlObjTest[FocusTest]);
			InitWndTest ();
			break;
		case WM_CLOSE:
			for (nCtl=1; nCtl<=NCTLTest; nCtl++)
				DestroyWindow (hCtlObjTest[nCtl]);
			return DefWindowProc(hWnd, Message, wParam, lParam);
		case WM_SIZE:
			for (nCtl=1; nCtl<=NCTLTest; nCtl++)
			{
				MoveWindow (hCtlObjTest[nCtl],
					ResTest[nCtl].param_int[P_X]*LOWORD(lParam)/(WidTest),
					ResTest[nCtl].param_int[P_Y]*HIWORD(lParam)/(HeiTest),
					ResTest[nCtl].param_int[P_W]*LOWORD(lParam)/(WidTest),
					ResTest[nCtl].param_int[P_H]*HIWORD(lParam)/(HeiTest),
					TRUE);
			}
 			break;
		case WM_COMMAND:
			switch (wParam)
			{
				/* case COMMAND0+i:
					HandleCtlTesti();
					break; */
				case COMMAND0+1:
					HandleCtlTestID_TEXT (&msg);
					break;
				case COMMAND0+2:
					HandleCtlTest2 (&msg);
					break;
				case COMMAND0+3:
					HandleCtlTestID_VALIDER (&msg);
					break;
				case COMMAND0+4:
					HandleCtlTest4 (&msg);
					break;
				case COMMAND0+5:
					HandleCtlTestID_COMMAND (&msg);
					break;
				case COMMAND0+6:
					HandleCtlTest6 (&msg);
					break;
				case COMMAND0+7:
					HandleCtlTestID_ANSWER (&msg);
					break;
				case COMMAND0+8:
					HandleCtlTestID_SEND (&msg);
					break;

 			}
			break;
		case MM_MCINOTIFY:
			sprintf (buf, "Notify %d", wParam);
			MessageBox (hWndTest, buf, "MCI", MB_OK);
			return TRUE;
		default:
			return DefWindowProc(hWnd, Message, wParam, lParam);
	}
	return NULL;
}

InitWndTest ()
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
	InitResTest ();
	if (!hPrevInstance)
	{
		wndclass.style = CS_HREDRAW | CS_VREDRAW;
		wndclass.lpfnWndProc = WndProcTest;
		wndclass.cbClsExtra = 0;
		wndclass.cbWndExtra = 0;
		wndclass.hInstance = hInstance;
		wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
		wndclass.hbrBackground  = GetStockObject(WHITE_BRUSH);
		wndclass.lpszMenuName = NULL;
		wndclass.lpszClassName  = szAppNameTest;

		if (!RegisterClass(&wndclass))
			return FALSE;
	}

#ifdef OWLTest
	TAppliTest Appli (szAppNameTest, hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	Appli.Run ();
	return Appli.Status;
#else
	WndTest = new TAppliWindowTest (NULL, WndCaptionTest);
	hWndTest = CreateWindow (szAppNameTest, WndCaptionTest,
		WS_OVERLAPPEDWINDOW | WS_VSCROLL | WS_HSCROLL,
		CW_USEDEFAULT, CW_USEDEFAULT, (WidTest)*2, (HeiTest)*3/2+20,
		NULL, NULL, hInstance, NULL);

	if (!hWndTest)
		return FALSE;

	ShowWindow (hWndTest, nCmdShow);
	UpdateWindow (hWndTest);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!KBHandler (&msg, hWndTest, NCTLTest, ResTest, hCtlObjTest, &FocusTest))
		{
			TranslateMessage (&msg);
			DispatchMessage (&msg);
		}
	}

	return(msg.wParam);
#endif
}
