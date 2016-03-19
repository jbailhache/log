/****************************************************************************
 *                                                                          *
 * File    : main.c                                                         *
 *                                                                          *
 * Purpose : Generic Pocket PC application.                                 *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/

#define UNICODE
#include <windows.h>
#include <windowsx.h>
#include <aygshell.h>
#include "main.h"

#define NELEMS(a)  (sizeof(a) / sizeof((a)[0]))

/** Prototypes **************************************************************/

static LRESULT WINAPI MainWndProc(HWND, UINT, WPARAM, LPARAM);
static BOOL Main_OnCreate(HWND, CREATESTRUCT *);
static void Main_OnActivate(HWND, UINT, HWND, BOOL);
static void Main_OnSettingChange(HWND, UINT, PCTSTR);
static void Main_OnPaint(HWND);
static void Main_OnCommand(HWND, int, HWND, UINT);
static void Main_OnDestroy(HWND);
static LRESULT WINAPI AboutDlgProc(HWND, UINT, WPARAM, LPARAM);

/** Global variables ********************************************************/

static HANDLE ghInstance;
static HWND ghwndMB;
static SHACTIVATEINFO gsai = { sizeof(SHACTIVATEINFO) };

HWND ghwnd;
HWND hEdit1, hEdit2, hButton1;
int gotinput;
#define INSIZE 1000
wchar_t inbuf[INSIZE+1];
#define OUTSIZE 10000
wchar_t outbuf[OUTSIZE+1];
int outptr = 0;
wchar_t *inptr = inbuf;
int insize = sizeof(inbuf);

void printb (char *s);
void printw (wchar_t *s);

void trace (char *s)
{
	int i;
	wchar_t buf[1000];
	for (i=0; s[i]; i++)
		buf[i] = s[i];
	buf[i] = 0;
	MessageBox (ghwnd, buf, L"Trace", MB_OK);
	/*
	printb ("trace: ");
	printb (s);
	printb ("\n");
	*/
}

void message (char *s)
{
	int i;
	wchar_t buf[1000];
	for (i=0; s[i]; i++)
		buf[i] = s[i];
	buf[i] = 0;
	MessageBox (ghwnd, buf, L"Message", MB_OK);
}

void handle_message (void)
{
	int status;
	MSG msg;

	status = PeekMessage (&msg, NULL, 0, 0, PM_REMOVE);
	if (status)
	{
		TranslateMessage (&msg);
		DispatchMessage (&msg);
	}
}

int input (wchar_t *s)
{
	int status;
	MSG msg;

	SendMessage (hEdit1, WM_SETTEXT, 0, (long)s);
	gotinput = 0;
	while (!gotinput)
	{
		status = GetMessage(&msg, NULL, 0, 0);    
		if (!status)
			return msg.wParam;
        	TranslateMessage(&msg);
       	 DispatchMessage(&msg);
	}
	
	return 0;
}

int readline (wchar_t *s, int n)
{
	int status; 
	MSG msg;

	inptr = s;
	insize = n;
	gotinput = 0;
	
	while (!gotinput)
	{
		status = GetMessage(&msg, NULL, 0, 0);    
		if (!status)
			return msg.wParam;
        	TranslateMessage(&msg);
       	 DispatchMessage(&msg);
	}
	printw (inbuf);
	printw (L"\r\n");
	return 0;
}

void printw (wchar_t *s)
{
int i, l, d;
	// MessageBox (ghwnd, s, L"print", MB_OK);
	for (l=0; s[l]; l++);
	if (outptr+l >= OUTSIZE)
	{
		d = outptr + l - OUTSIZE;
		for (i=d; i<=outptr; i++)
			outbuf[i-d] = outbuf[i];
		outptr -= d;
	}
	for (i=0; s[i]; i++)
		outbuf[outptr+i] = s[i];
	outbuf[outptr+i] = 0;
	outptr += i;
	SendMessage (hEdit1, WM_SETTEXT, 0, (long)outbuf);
}

void readb (char *s, int n)
{
	int n1;
	int i;

	SendMessage (hEdit1, WM_SETTEXT, 0, (long)outbuf);
	n = SendMessage (hEdit1, EM_GETLINECOUNT, 0, 0);
	SendMessage (hEdit1, EM_LINESCROLL, 0, n);

	if (INSIZE<n)
		n1 = sizeof(inbuf);
	else
		n1 = n;
	readline (inbuf, n1);
	for (i=0; inbuf[i] && i<n; i++)
		s[i] = inbuf[i];
	s[i] = 0;
}

void printb (char *s)
{
int i, j, l, d, n;
	// MessageBox (ghwnd, s, L"print", MB_OK);
	for (l=0; s[l]; l++);
	if (outptr+l >= OUTSIZE)
	{
		d = outptr + l - OUTSIZE;
		for (i=d; i<=outptr; i++)
			outbuf[i-d] = outbuf[i];
		outptr -= d;
	}
	for (i=0, j=0; s[i]; i++, j++)
      {
		if (s[i] == '\n')
			outbuf[outptr+j++] = '\r';
		outbuf[outptr+j] = s[i];
	}
	outbuf[outptr+j] = 0;
	outptr += j;

	// SendMessage (hEdit1, EM_SETSEL, 99999, 99999);
	// SendMessage (hEdit1, EM_REPLACESEL, 1, (long)s);

	// SendMessage (hEdit1, WM_SETTEXT, 0, (long)outbuf);
	// n = SendMessage (hEdit1, EM_GETLINECOUNT, 0, 0);
	// SendMessage (hEdit1, EM_LINESCROLL, 0, n);

	}

void init(void)
{
	gotinput = 0;
	outptr = 0;
}

int progmain1 (void)
{
	wchar_t buf[1000];
	input (L"Tapez quelque chose");
	swprintf (buf, L"Vous avez tapé: <<%s>>", inbuf);
	input (buf);
	return 0;
}

int progmain (void)
{
	int i;
	wchar_t buf[1000];
	char inb[1000], outb[1000];

	printb ("Tapez quelque chose");
	readb (inb, 999);
       sprintf (outb, "Vous avez tapé : <%s>", inb);
	printb (outb);

	readline (buf, sizeof(buf));
	for (i=0; i<12; i++)
	{
		printw (buf);
		printw (L"\r\n");
	}
       readline (buf, sizeof(buf));
	return 0;
}

/****************************************************************************
 *                                                                          *
 * Function: WinMain                                                        *
 *                                                                          *
 * Purpose : Initialize the application.  Register a window class,          *
 *           create and display the main window and enter the               *
 *           message loop.                                                  *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/

int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpszCmdLine, int nCmdShow)
{
    HWND hwnd;
    MSG msg;

    /* Always check first if program is already running */
    hwnd = FindWindow(L"mslpiaClass", NULL);
    if (hwnd)
    {
        /*
         * Set focus to the foremost child window. The "|0x01" is used to
         * bring any owned windows to the foreground and activate them.
         */
        SetForegroundWindow((HWND)((ULONG)hwnd|0x00000001));
        return 0;
    }

    if (!hPrevInstance)
    {
        WNDCLASS wc;

        wc.lpszClassName = L"mslpiaClass";
        wc.lpfnWndProc = MainWndProc;
        wc.style = CS_VREDRAW|CS_HREDRAW;
        wc.hInstance = hInstance;
        wc.hIcon = NULL;
        wc.hCursor = NULL;
        wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
        wc.lpszMenuName = NULL;
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;

        if (!RegisterClass(&wc))
            return 1;
    }

    ghInstance = hInstance;

    hwnd = CreateWindowEx(
        0,
        L"mslpiaClass",
        L"mslpia Program",
        WS_VISIBLE|WS_SYSMENU,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        NULL,
        NULL,
        hInstance,
        NULL
    );
    if (!hwnd) return 1;

    /*
     * When CW_USEDEFAULT is used to create the main window, the height of the
     * menu bar is not considered. We must manually reserve space for the menu.
     */
    if (ghwndMB)
    {
        RECT rcWin, rcMB;

        GetWindowRect(hwnd, &rcWin);
        GetWindowRect(ghwndMB, &rcMB);
        rcWin.bottom -= (rcMB.bottom - rcMB.top);
        MoveWindow(hwnd, rcWin.left, rcWin.top, rcWin.right - rcWin.left, rcWin.bottom - rcWin.top, FALSE);
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);
/*
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
*/
	// return progmain();
	mslpia_main();
	return 0;
	}

/****************************************************************************
 *                                                                          *
 * Function: MainWndProc                                                    *
 *                                                                          *
 * Purpose : Process application messages.                                  *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/

static LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        HANDLE_MSG(hwnd, WM_CREATE, Main_OnCreate);
        HANDLE_MSG(hwnd, WM_ACTIVATE, Main_OnActivate);
        HANDLE_MSG(hwnd, WM_SETTINGCHANGE, Main_OnSettingChange);
        HANDLE_MSG(hwnd, WM_PAINT, Main_OnPaint);
        HANDLE_MSG(hwnd, WM_COMMAND, Main_OnCommand);
        HANDLE_MSG(hwnd, WM_DESTROY, Main_OnDestroy);
        /* TODO: enter more messages here */
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
}

/****************************************************************************
 *                                                                          *
 * Function: Main_OnCreate                                                  *
 *                                                                          *
 * Purpose : Process a WM_CREATE message.                                   *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/

static BOOL Main_OnCreate(HWND hwnd, CREATESTRUCT *pcs)
{
    SHMENUBARINFO mbi;

    memset(&mbi, 0, sizeof(mbi));
    mbi.cbSize = sizeof(mbi);
    mbi.hwndParent = hwnd;
    mbi.nToolBarId = IDR_MNU_MAIN;
    mbi.hInstRes = ghInstance;
    mbi.nBmpId = 0;
    mbi.cBmpImages = 0;
    if (!SHCreateMenuBar(&mbi))  /* create the menu bar */
        return FALSE;

    ghwndMB = mbi.hwndMB;

 hEdit1 = CreateWindow (L"EDIT", L"", 
		WS_VISIBLE|WS_CHILD|WS_BORDER|WS_VSCROLL|WS_HSCROLL|
		ES_MULTILINE|ES_WANTRETURN|ES_AUTOHSCROLL|ES_AUTOVSCROLL,
		// 10, 10, 300, 60,
		10, 1, 300, 80,
		hwnd, NULL, ghInstance, NULL);

 hEdit2 = CreateWindow (L"EDIT", L"",
		WS_CHILD | WS_VISIBLE | ES_LEFT | WS_BORDER,
		// 10, 80, 240, 20,
		10, 83, 240, 18,
		hwnd, NULL, ghInstance, NULL);

 hButton1 = CreateWindow (L"BUTTON", L"OK", 
		WS_CHILD | WS_VISIBLE | ES_LEFT | WS_BORDER,
		// 260, 80, 40, 20,
		260, 83, 40, 18,
		hwnd, NULL, ghInstance, NULL);

    return TRUE;
}

/****************************************************************************
 *                                                                          *
 * Function: Main_OnActivate                                                *
 *                                                                          *
 * Purpose : Process a WM_ACTIVATE message.                                 *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/

static void Main_OnActivate(HWND hwnd, UINT state, HWND hwndActDeact, BOOL fMinimized)
{
    /* Notify the shell of our activate message */
    SHHandleWMActivate(hwnd, MAKEWPARAM(state,fMinimized), (LPARAM)hwndActDeact, &gsai, 0);
}

/****************************************************************************
 *                                                                          *
 * Function: Main_OnSettingChange                                           *
 *                                                                          *
 * Purpose : Process a WM_SETTINGCHANGE message.                            *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/

static void Main_OnSettingChange(HWND hwnd, UINT uiAction, PCTSTR pszSection)
{
    switch (uiAction)
    {
        case SPI_SETSIPINFO:
        {
            /* Adjust window size depending on SIP panel */
            SHHandleWMSettingChange(hwnd, -1, 0, &gsai);
            break;
        }
    }
}

/****************************************************************************
 *                                                                          *
 * Function: Main_OnPaint                                                   *
 *                                                                          *
 * Purpose : Process a WM_PAINT message.                                    *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/

static void Main_OnPaint(HWND hwnd)
{
    PAINTSTRUCT ps;
    RECT rc;

    BeginPaint(hwnd, &ps);
    GetClientRect(hwnd, &rc);
    // DrawText(ps.hdc, L"Hello, Windows CE!", -1, &rc, DT_CENTER|DT_VCENTER);
    EndPaint(hwnd, &ps);
}

/****************************************************************************
 *                                                                          *
 * Function: Main_OnCommand                                                 *
 *                                                                          *
 * Purpose : Process a WM_COMMAND message.                                  *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/

static void Main_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{

      wchar_t buf[1000];
	int i;
	if (hwndCtl == hButton1)
		{
			/*
			SendMessage (hEdit1, WM_GETTEXT, 999, (long)buf);
			for (i=0; buf[i]; i++)
					buf[i]++;
			SendMessage (hEdit1, WM_SETTEXT, 0, (long)buf);
			*/
			/* MessageBox (hwnd, L"Clic !", L"Message", MB_OK); */
			// SendMessage (hEdit2, WM_GETTEXT, sizeof(inbuf), (long)inbuf);
			SendMessage (hEdit2, WM_GETTEXT, insize, (long)inptr);
			gotinput = 1;
			// MessageBox (hwnd, inbuf, L"Lecture", MB_OK);
			return;
		}

    switch (id)
    {
        case IDM_ABOUT:
            DialogBox(ghInstance, MAKEINTRESOURCE(DLG_ABOUT), hwnd, (DLGPROC)AboutDlgProc);
            break;

        case IDOK:
            SendMessage(hwnd, WM_CLOSE, 0, 0);
            break;

        default:
            FORWARD_WM_COMMAND(hwnd, id, hwndCtl, codeNotify, DefWindowProc);
            break;

        /* TODO: Enter more commands here */
    }
}

/****************************************************************************
 *                                                                          *
 * Function: Main_OnDestroy                                                 *
 *                                                                          *
 * Purpose : Process a WM_DESTROY message.                                  *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/

static void Main_OnDestroy(HWND hwnd)
{
    DestroyWindow(ghwndMB);
    PostQuitMessage(0);
}

/****************************************************************************
 *                                                                          *
 * Function: AboutDlgProc                                                   *
 *                                                                          *
 * Purpose : Process messages for the About dialog.  The dialog is          *
             shown when the user selects "About" in the "Help" menu.        *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/

static LRESULT CALLBACK AboutDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_INITDIALOG:
        {
            SHINITDLGINFO shidi;

            /*
             * Create a Done button and size the dialog.
             */
            shidi.dwMask = SHIDIM_FLAGS;
            shidi.dwFlags = SHIDIF_DONEBUTTON|SHIDIF_SIPDOWN|SHIDIF_SIZEDLGFULLSCREEN;
            shidi.hDlg = hDlg;
            SHInitDialog(&shidi);
            return TRUE;
        }

        case WM_COMMAND:
            switch (wParam)
            {
                case IDOK:
                    /*
                     * OK was clicked, close the dialog.
                     */
                    EndDialog(hDlg, TRUE);
			PostQuitMessage(0);
			exit(0);
                    return TRUE;
            }
            break;
    }

    return FALSE;
}
