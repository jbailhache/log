/* Project created with File / New / Console Wizard / Window (GUI) in Borland C++ Builder 4 */ 

#include <windows.h>
#pragma hdrstop
#include <condefs.h>


//---------------------------------------------------------------------------
#pragma argsused

#define WORD unsigned int

LONG FAR PASCAL WndProc (HWND hWnd, WORD message,
        WORD wParam, LONG lParam)
{
HDC hDC;
PAINTSTRUCT ps;
RECT taille_fenetre;

        switch (message)
        {
                case WM_PAINT:
                        hDC = BeginPaint (hWnd, &ps);
                        GetClientRect (hWnd, &taille_fenetre);
                        DrawText (hDC, "Bonjour !", -1,
                                &taille_fenetre,
                                DT_SINGLELINE | DT_CENTER | DT_VCENTER);
                        EndPaint (hWnd, &ps);
                        break;

                case WM_DESTROY:
                        PostQuitMessage (0);
                        break;

                default:
                        return DefWindowProc (hWnd, message, wParam, lParam);
        }
        return NULL;
}

WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
        LPSTR lpszCmdLine, int nCmdShow)
{
 static char szAppName[] = "Hello";
HWND hWnd;
MSG msg;
WNDCLASS wndclass;

        if (!hPrevInstance)
        {
                wndclass.style = CS_HREDRAW | CS_VREDRAW;
                wndclass.lpfnWndProc = WndProc;
                wndclass.cbClsExtra = 0;
                wndclass.cbWndExtra = 0;
                wndclass.hInstance = hInstance;
                wndclass.hIcon = LoadIcon (NULL, IDI_APPLICATION);
                wndclass.hCursor = LoadCursor (NULL, IDC_ARROW);
                wndclass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH);
                wndclass.lpszMenuName = NULL;
                wndclass.lpszClassName = szAppName;
                RegisterClass (&wndclass);
        }

        hWnd = CreateWindow (szAppName, "Bonjour",
                WS_OVERLAPPEDWINDOW,
                CW_USEDEFAULT, 0,
                CW_USEDEFAULT, 0,
                NULL, NULL, hInstance, NULL);

        ShowWindow (hWnd, nCmdShow);
        UpdateWindow (hWnd);

        while (GetMessage (&msg, NULL, 0, 0))
        {
                TranslateMessage (&msg);
                DispatchMessage (&msg);
        }

        return (msg.wParam);
        /*return 0;*/
}



