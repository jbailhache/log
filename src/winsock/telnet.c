
#include "windows.h"
#include "winsock.h"
 
#define FD_READ         0x01
#define FD_WRITE        0x02
#define FD_OOB          0x04
#define FD_ACCEPT       0x08
#define FD_CONNECT      0x10
#define FD_CLOSE        0x20

#define WM_REC (WM_USER+1)

HANDLE hInstanceTelnet;
HWND hwndTelnet;

char bigbuf[5000];
   
printWSA (WSADATA *WSAdata)
{
	printf ("WSA data :\n");
	printf ("\tversion %x\n", WSAdata->wVersion);
	printf ("\thighest %x\n", WSAdata->wHighVersion);
	printf ("\tdescription <%s>\n", WSAdata->szDescription);
	printf ("\tstatus <%s>\n", WSAdata->szSystemStatus);
	printf ("\tmax sockets %d\n", WSAdata->iMaxSockets);
	printf ("\tmax datagram size %d\n", WSAdata->iMaxUdpDg);
}

int adrnum (char *buf)
{
int i, l;
	l = strlen (buf);
	for (i=0; i<l; i++)
	{
		if (buf[i] != '.' && !(buf[i]>='0' && buf[i]<='9'))
			return 0;
	}
	return 1;
}

format (char *in, char *out)
{
int l, i, j;
	l = strlen (in);
	i = 0;
	j = 0;
	for (;;)
	{
		if (i >= l)
		{
			out[j] = 0;
			break;
	}
		if (i == l-1)
		{
			if (in[i] == '\\')
				out[j] = 0;
			else
			{
				out[j] = in[i];
				out[j+1] = 13;
				out[j+2] = 10;
				out[j+3] = 0;
	    }
	    break;
	}
		if (in[i] == '\\' && i<=l-3)
		{
			out[j++] = (in[i+1] - 0x40) | ((in[i+2] - 0x40) << 4);
			i += 3;
		}
		else
			out[j++] = in[i++];
	}
}

SOCKET s;

telnet ()
{
int x;
int status;
WORD version;
WSADATA WSAdata;
char buf[300 /*5000*/];
LPHOSTENT host;
LPSTR adr;
/* SOCKET s; */
struct sockaddr FAR *addr;
/*struct sockaddr*/ SOCKADDR_IN sa;
int port;
LPPROTOENT proto;
int i;
char buf1[400];
struct in_addr inetadr;
MSG msg;

	version = 0x101;
    status = WSAStartup (version, &WSAdata);
	if (status)
	{
		printf ("WSAStartup failed %d\n", status);
		return status;
    }

    printWSA (&WSAdata);

	s = socket (PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (s == INVALID_SOCKET)
	{
		printf ("Cannot create socket\n");
	}
	printf ("Socket created\n");

input_host:
    printf ("Host ? ");
	gets (buf);
	if (adrnum (buf))
	{
		inetadr = inet_addr ((char FAR *)buf);
		host = gethostbyaddr (&inetadr, 4, AF_INET);
    }
    else
		host = gethostbyname ((char FAR *)buf /*"www.yahoo.com"*/);
	if (!host)
	{
		printf ("Could not get IP address\n");
	goto input_host;
	}
	else
	{
		adr = inet_ntoa (*(LPIN_ADDR)*(host->h_addr_list));
		printf ("address : <%Fs>\n", adr);
	}

	printf ("Port ? ");
	gets (buf);
	port = htons (atoi (buf));

	sa.sin_family = AF_INET;
	sa.sin_port = port;

    /*
	printf ("Server address : \n");
	for (i=0; i<4; i++)
	{
		printf ("\tpart %d ? ", i+1);
		gets (buf1);
	buf[i] = atoi (buf1);
    }
	*/
	for (i=0; i<4; i++)
	buf[i] = host->h_addr_list[0][i];
	memcpy (&sa.sin_addr, buf, 4);
	
	/* _fmemcpy (&sa.sin_addr, *(host->h_addr_list), 4); */ 

	status = connect (s, (LPSOCKADDR)&sa, sizeof(sa));

	if (status)
	{
		printf ("Connect failed %d ", status);
		status = WSAGetLastError ();
		printf ("%d\n", status);
		return status;
	}
	printf ("Connected\n");

	WSAAsyncSelect (s, hwndTelnet, WM_REC, FD_READ);
    /*
					while(GetMessage(&msg, NULL, 0, 0))
					{
						TranslateMessage(&msg);
						DispatchMessage(&msg);
					}
	*/                     
	for (;;)
	{
		if (PeekMessage (&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			GetMessage (&msg, NULL, 0, 0);
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else if (kbhit ())
		{
					gets (buf1);

					if (!strcmp (buf1, "\\quit"))
					{
						closesocket (s);

						/* gets (buf); */
						status = WSACleanup ();
						if (status)
						{
							printf ("WSACleanup failed %d %d\n", status,
								WSAGetLastError());
							return status;
						}

						PostQuitMessage (0);
						return;
					}

		    format (buf1, buf);

					i = strlen (buf);
					/*
					buf[i] = 13;
					buf[i+1] = 10;
					buf[i+2] = 0;
					*/

					status = send (s, (char FAR *)buf, /*sizeof(buf)*/ strlen(buf), 0);
					/* printf ("%d bytes sent\n", status); */


	}

    }
	return;
	/*
	strcpy (buf, "test client->server");
	printf ("Data to send ? ");
	*/
loop1:

    printf ("-");
loop:
	status = recv (s, (char FAR *)bigbuf, sizeof(bigbuf), 0);
    if (status > 0 && status < sizeof(bigbuf))
	{
		bigbuf[status] = 0;
		/* printf ("Data received : <%s>\n", bigbuf); */
		printf ("%s", bigbuf);
		/* goto loop; */
	}
	else
	{
		/* printf ("recv failed %d\n", status); */
		printf ("\n");
	}

	printf ("?");
	gets (buf);

	if (!strcmp (buf, "quit"))
		goto end;

	i = strlen (buf);
	buf[i] = 13;
	buf[i+1] = 10;
	buf[i+2] = 0;

	status = send (s, (char FAR *)buf, /*sizeof(buf)*/ strlen(buf), 0);
	/* printf ("%d bytes sent\n", status); */

	goto loop1;

    /*
    printf ("Data received : \n");
	for (i=0; i<200; i++)
	{
		status = recv (s, (char FAR *)buf, 1, 0);
	printf ("%c", buf[0]);
	}
	*/
end:
    closesocket (s);

	/* gets (buf); */
	status = WSACleanup ();
	if (status)
	{
		printf ("WSACleanup failed %d\n");
		return status;
	}
	return 0;
}

#define szAppName "Telnet"

long FAR PASCAL _export WndProc(HWND hwnd, UINT iMessage, UINT wParam,
			LONG lParam)
	{
    int status, i;
    char buf[300];
		switch (iMessage)
			{
				case WM_REC:
					status = recv (s, (char FAR *)bigbuf, sizeof(bigbuf), 0);
					if (status > 0)
					{
						if (status >= sizeof(bigbuf))
							bigbuf[sizeof(bigbuf)-1] = 0;
			else
							bigbuf[status] = 0;
						/* printf ("Data received : <%s>\n", bigbuf); */
						printf ("%s", bigbuf);
						/* goto loop; */
					}
					break;

				case WM_KEYDOWN:
					gets (buf);

					if (!strcmp (buf, "quit"))
					{
						closesocket (s);

						/* gets (buf); */
						status = WSACleanup ();
						if (status)
						{
							printf ("WSACleanup failed %d\n");
							return status;
						}

						PostQuitMessage (0);
		    }

					i = strlen (buf);
					buf[i] = 13;
					buf[i+1] = 10;
					buf[i+2] = 0;

					status = send (s, (char FAR *)buf, /*sizeof(buf)*/ strlen(buf), 0);
					/* printf ("%d bytes sent\n", status); */

					break;

				default: ;
			}
		
		return(DefWindowProc(hwnd, iMessage, wParam, lParam));
	}


int PASCAL WinMain (HANDLE hInstance, HANDLE hPrevInstance,
	LPSTR lpszCmdLine, int nCmdShow)
{
MSG msg;
WNDCLASS wndclass;

	_InitEasyWin ();
	hInstanceTelnet = hInstance;

		if (!hPrevInstance)
			{
				wndclass.style                                  = CS_HREDRAW | CS_VREDRAW;
				wndclass.lpfnWndProc            = WndProc;
				wndclass.cbClsExtra                     = 0;
				wndclass.cbWndExtra                     = 0;
				wndclass.hInstance                      = hInstance;
				wndclass.hIcon                                  = LoadIcon(NULL, IDI_APPLICATION) /*(hInstance, szAppName)*/;
				wndclass.hCursor                                = LoadCursor(NULL, IDC_ARROW);
				wndclass.hbrBackground  = GetStockObject(WHITE_BRUSH);
				wndclass.lpszMenuName           = szAppName;
				wndclass.lpszClassName  = szAppName;

				if (!RegisterClass(&wndclass))
					return FALSE;
			}

		hwndTelnet = CreateWindow
					(
						szAppName, 
						"Telnet",
						WS_OVERLAPPEDWINDOW     | WS_VSCROLL | WS_HSCROLL,
						CW_USEDEFAULT, 
						CW_USEDEFAULT,
						CW_USEDEFAULT, 
						CW_USEDEFAULT,
						NULL, 
						NULL, 
						hInstance, 
						NULL
					);

		if (!hwndTelnet)
			return FALSE;

		/* ShowWindow(hwndTelnet, nCmdShow); */
		UpdateWindow(hwndTelnet);

		telnet ();

		/*if( StartWinsock())
			{*/
		/*
				while(GetMessage(&msg, NULL, 0, 0))
					{
						TranslateMessage(&msg);
						DispatchMessage(&msg);
					}
		*/
		/*      }
		WSACleanup(); */
		return(msg.wParam);

}
