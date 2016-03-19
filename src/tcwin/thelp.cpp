/* Windows API source generated from resource file thelp.ctl */

#include <owl.h>
#include <window.h>
#include <string.h>
#include <ctype.h>



HANDLE hInst;
HWND hWndThelp;

#define NCTL 60
HWND hCtlObjThelp[NCTL];

int FocusThelp;

static char szAppNameThelp[] = "AppliThelp";
static char WndCaptionThelp[] = "Thelp";

#define COMMAND0 100

#define hCtlIdThelp(id) (hCtlObjThelp[(id)-COMMAND0])

#define WidThelp (142)
#define HeiThelp (92)


HMENU hMenu, hMenuFile;

CreateMenuThelp (HWND hWnd)
{
int status;
	hMenu = CreateMenu ();
	hMenuFile = CreatePopupMenu ();
	status = AppendMenu (hMenuFile, MF_ENABLED | MF_STRING, 201, "&Open");
	status = AppendMenu (hMenuFile, MF_ENABLED | MF_STRING, 202, "&Quit");
	status = AppendMenu (hMenu, MF_ENABLED | MF_POPUP, (UINT)hMenuFile, "&File");
	status = AppendMenu (hMenu, MF_ENABLED | MF_STRING, 203, "&Help");
	/* DrawMenuBar (hWndThelp); */
	status = SetMenu (hWnd, hMenu);
	if (!status)
	{
		MessageBox (hWndThelp, sys_errlist[errno], "Error", MB_OK);
	}
    /* DrawMenuBar (hWndThelp); */

}

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

struct resource_item ResThelp[NCTL];

InitResThelp ()
{

	/*
	EDITTEXT 101, 10, 9, 122, 13, ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP
	EDITTEXT ("EDIT") "" ID=101 X= 10 Y= 9 W= 122 H= 13 STYLE= ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL
	*/
	strcpy (ResThelp[1].name, "EDITTEXT");
	strcpy (ResThelp[1].ctlclass, "EDIT");
	strcpy (ResThelp[1].caption, "");
	ResThelp[1].param_int[1] =  10;
	ResThelp[1].param_int[2] =  29;
	ResThelp[1].param_int[3] =  122;
	ResThelp[1].param_int[4] =  13;
	ResThelp[1].param_int[5] =  ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL;

	/*
	PUSHBUTTON "Test", 102, 13, 33, 24, 14, WS_CHILD | WS_VISIBLE | WS_TABSTOP
	PUSHBUTTON ("BUTTON") "Test" ID= 102 X= 13 Y= 33 W= 24 H= 14 STYLE= WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON
	*/
	strcpy (ResThelp[2].name, "PUSHBUTTON");
	strcpy (ResThelp[2].ctlclass, "BUTTON");
	strcpy (ResThelp[2].caption, "Test");
	ResThelp[2].param_int[1] =  13;
	ResThelp[2].param_int[2] =  53;
	ResThelp[2].param_int[3] =  24;
	ResThelp[2].param_int[4] =  14;
	ResThelp[2].param_int[5] =  WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON;

#define NCTLThelp 2


	return 0;
}


int InitWndThelp ();
class TAppliWindowThelp : public TWindow
{
public:
	/* declare control objects PT... */
	/* PTButton Button1; */
    PTWindowsObject pWndCtlObj[NCTL];
	HWND hWndCtlObj[NCTL];
 
	TAppliWindowThelp (PTWindowsObject parent, LPSTR title) :
		TWindow (parent, title)
	{
		EnableKBHandler ();
        /* create control objects PT... with new T... */ 
		/* Button1 = new TButton (this, 101, "&Test 1", 50, 80, 40, 20, FALSE); */
	}

	LPSTR GetClassName ()
	{
    	return szAppNameThelp;
	}

	virtual BOOL Create ()
	{
	BOOL status;
		status = TWindow::Create ();
		hWndThelp = HWindow;
        /* create objects hWndCtlObj[i] with CreateWindow */ 
 		return status;
    }
	/* void HandleWndCtli (RTMessage Msg) = [ID_FIRST + COMMAND0 + i] { ... } */
};

TAppliWindowThelp *WndThelp;

class TAppliThelp : public TApplication
{
public:
	TAppliThelp (LPSTR AName, HINSTANCE hInstance, HINSTANCE hPrevInstance,
		LPSTR lpCmdLine, int nCmdShow) :
		TApplication (AName, hInstance, hPrevInstance, lpCmdLine, nCmdShow)
	{
	}
	virtual void InitMainWindow ()
	{
		WndThelp = new TAppliWindowThelp (NULL, Name);
		MainWindow = WndThelp;
		WndThelp->Attr.W = (WidThelp)*2;
		WndThelp->Attr.H = (HeiThelp)*3/2+20;
    }
};


	/*
	EDITTEXT 101, 10, 9, 122, 13, ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP
	EDITTEXT ("EDIT") "" ID=101 X= 10 Y= 9 W= 122 H= 13 STYLE= ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL
	*/
void HandleCtlThelp1 (TMessage *pmsg)
{
/* Add code here */

}

	/*
	PUSHBUTTON "Test", 102, 13, 33, 24, 14, WS_CHILD | WS_VISIBLE | WS_TABSTOP
	PUSHBUTTON ("BUTTON") "Test" ID= 102 X= 13 Y= 33 W= 24 H= 14 STYLE= WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON
	*/
void HandleCtlThelp2 (TMessage *pmsg)
{
/* Add code here */

}

	/*

	 () "" ID=000 X=000 Y=000 W=000 H=000 STYLE=000
	*/
void HandleCtlThelp3 (TMessage *pmsg)
{
/* Add code here */

}

LRESULT CALLBACK WndProcThelp (HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
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
			WndThelp->HWindow = hWndThelp;
        	/* create control objects hCtlObjThelp[i] with CreateWindow */
			for (nCtl=1; nCtl<=NCTLThelp; nCtl++)
			{
				hCtlObjThelp[nCtl] = CreateWindow (ResThelp[nCtl].ctlclass, ResThelp[nCtl].caption,
					ResThelp[nCtl].param_int[P_STYLE],
					ResThelp[nCtl].param_int[P_X]*2, ResThelp[nCtl].param_int[P_Y]*3/2, ResThelp[nCtl].param_int[P_W]*2, ResThelp[nCtl].param_int[P_H]*3/2,
					hWnd, COMMAND0+nCtl, hInst, NULL);
			}

			CreateMenuThelp (hWnd);

			FocusThelp = 1;
			SetFocus (hCtlObjThelp[FocusThelp]);
			InitWndThelp ();
			break;
		case WM_CLOSE:
			for (nCtl=1; nCtl<=NCTLThelp; nCtl++)
				DestroyWindow (hCtlObjThelp[nCtl]);
			SetMenu (hWndThelp, NULL);
			DestroyMenu (hMenu);
			return DefWindowProc(hWnd, Message, wParam, lParam);
		case WM_SIZE:
			for (nCtl=1; nCtl<=NCTLThelp; nCtl++)
			{
				MoveWindow (hCtlObjThelp[nCtl],
					ResThelp[nCtl].param_int[P_X]*LOWORD(lParam)/(WidThelp),
					ResThelp[nCtl].param_int[P_Y]*HIWORD(lParam)/(HeiThelp),
					ResThelp[nCtl].param_int[P_W]*LOWORD(lParam)/(WidThelp),
					ResThelp[nCtl].param_int[P_H]*HIWORD(lParam)/(HeiThelp),
					TRUE);
			}
			break;
		case WM_COMMAND:
			switch (wParam)
			{
				/* case COMMAND0+i:
					HandleCtlThelpi();
					break; */
				case COMMAND0+1:
					HandleCtlThelp1 (&msg);
					break;
				case COMMAND0+2:
					HandleCtlThelp2 (&msg);
					break;
				case COMMAND0+3:
					HandleCtlThelp3 (&msg);
					break;
				case 201:
					MessageBox (hWndThelp, "Open", "Menu", MB_OK);
					break;
				case 202:
					PostQuitMessage (0);
					break;
				case 203:
					/* MessageBox (hWndThelp, "Help", "Menu", MB_OK); */
					WinHelp (hWnd, "C:\\D1\\TCWIN\\THELP.HLP", HELP_CONTENTS, 0);
					break;
 			}
			break;
		default:
			return DefWindowProc(hWnd, Message, wParam, lParam);
	}
	return NULL;
}

InitWndThelp ()
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

	InitResThelp ();
	if (!hPrevInstance)
	{
		wndclass.style = CS_HREDRAW | CS_VREDRAW;
		wndclass.lpfnWndProc = WndProcThelp;
		wndclass.cbClsExtra = 0;
		wndclass.cbWndExtra = 0;
		wndclass.hInstance = hInstance;
		wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
		wndclass.hbrBackground  = GetStockObject(WHITE_BRUSH);
		wndclass.lpszMenuName = NULL;
		wndclass.lpszClassName  = szAppNameThelp;

		if (!RegisterClass(&wndclass))
			return FALSE;
	}

#ifdef OWLThelp
	hInst = hInstance;
	TAppliThelp Appli (szAppNameThelp, hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	Appli.Run ();
	return Appli.Status;
#else
	WndThelp = new TAppliWindowThelp (NULL, WndCaptionThelp);
	hWndThelp = CreateWindow (szAppNameThelp, WndCaptionThelp,
		WS_OVERLAPPEDWINDOW | WS_VSCROLL | WS_HSCROLL,
		CW_USEDEFAULT, CW_USEDEFAULT, (WidThelp)*2, (HeiThelp)*3/2+20,
		NULL, NULL, hInstance, NULL);

	if (!hWndThelp)
		return FALSE;

	ShowWindow (hWndThelp, nCmdShow);
	UpdateWindow (hWndThelp);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!KBHandler (&msg, hWndThelp, NCTLThelp, ResThelp, hCtlObjThelp, &FocusThelp))
		{
			TranslateMessage (&msg);
			DispatchMessage (&msg);
		}
	}

	return(msg.wParam);
#endif
}
