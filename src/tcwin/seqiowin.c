
#include <windows.h>

char class_name[] = "test1";

char buf_in [1000];
int head_in = 0;
int tail_in = 0;
int count_in = 0;

#if 0
int getc_in ()
{
int c;
MSG msg;
  for (;;)
  {
	if (count_in > 0)
	{
    	count_in--;
		c = buf_in[head_in++];
		if (head_in >= sizeof(buf_in))
			head_in = 0;
        return c;
	}
	else
	{
		c = 0;
		/* WaitMessage (); */
		while (GetMessage (&msg, NULL, 0, 0))
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
  }
}
#else
int getc_in ()
{
MSG msg;
	while (GetMessage (&msg, NULL, 0, 0))
	{
		if (msg.message == WM_CHAR)
		{
        	return msg.wParam;
		}
		else
		{
			TranslateMessage (&msg);
            DispatchMessage (&msg);
        }

	}
}

#endif


gets_in (char *s)
{
int c;
	for (;;)
	{
		c = getc_in ();
		if (c == '\n' || c == '\r')
		{
			*s = 0;
			return;
		}
		*s++ = c;
	}
}


LRESULT CALLBACK WndProc (HWND hWnd, UINT Message,
	WPARAM wParam, LPARAM lParam)
{
static HDC hDC;
static PAINTSTRUCT ps;
static RECT taille_fenetre;
static char buf [600];
static int i;
/* char buf_out [200]; */
int x;
int status;

	switch (Message)
	{
		case WM_PAINT:
			hDC = BeginPaint (hWnd, &ps);
			GetClientRect (hWnd, &taille_fenetre);
			DrawText (hDC, "Bonjour !\nTapez un nombre : ",
				-1, &taille_fenetre, 0);
#ifdef V1
            EndPaint (hWnd, &ps);
#endif
			x = 0;
            i = 0;
#ifdef TEST1
			break;

		case WM_CHAR:
			if (wParam != '\n' && wParam != '\r')
				buf[i++] = wParam;
#else
			gets_in (buf);
#endif
#ifdef TEST1
			else
			{
			buf[i] = 0;
			i = 0;
#endif
            x = atoi (buf);
			sprintf (buf, "\n\nLe carre du nombre est : %d     ", x*x);
#ifdef V1
			hDC = BeginPaint (hWnd, &ps);
			GetClientRect (hWnd, &taille_fenetre);
#endif
			status = DrawText (hDC, buf, -1, &taille_fenetre, 0);
			EndPaint (hWnd, &ps);
#ifdef TEST1
			}
#endif
			break;

#ifndef TEST1
		case WM_CHAR:
    		count_in++;
			buf_in[tail_in++] = wParam;
			if (tail_in >= sizeof(buf_in))
				tail_in = 0;
			break;
#endif

		case WM_DESTROY:
			PostQuitMessage (0);
			break;

		default:
			return DefWindowProc (hWnd, Message, wParam, lParam);
	}
	return 0L;
}

int PASCAL WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpszCmdLine, int cmdShow)
{
HWND hWnd;
MSG msg;
WNDCLASS wndclass;

	if (!hPrevInstance)
	{
		wndclass.lpszClassName = class_name;
		wndclass.hInstance = hInstance;
		wndclass.lpfnWndProc = WndProc;
		wndclass.hCursor = LoadCursor (NULL, IDC_ARROW);
		wndclass.hIcon = LoadIcon (NULL, IDI_APPLICATION);
		wndclass.lpszMenuName = NULL;
		wndclass.hbrBackground = GetStockObject (WHITE_BRUSH);
		wndclass.style = CS_HREDRAW | CS_VREDRAW /*NULL*/;
		wndclass.cbClsExtra = 0;
		wndclass.cbWndExtra = 0;
		RegisterClass (&wndclass);
	}

	hWnd = CreateWindow (class_name, "Test",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0,
		CW_USEDEFAULT, 0,
		NULL, NULL, hInstance, NULL);

	ShowWindow (hWnd, cmdShow);
	UpdateWindow (hWnd); /* facultatif */

	while (GetMessage (&msg, NULL, 0, 0))
	{
		TranslateMessage (&msg);
		DispatchMessage (&msg);
	}
	return msg.wParam;
}


