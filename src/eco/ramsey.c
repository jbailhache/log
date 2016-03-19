#include <windows.h>

LRESULT CALLBACK MainProc(HWND Dlg, UINT message,WPARAM wParam,LPARAM lParam);

HINSTANCE hInst;

int n;

#define XMAX 256
#define YMAX 256

#define XSCALE 0.1
#define YSCALE 0.1
#define ZSCALE 0.1
#define ZZSCALE 0.1

#define EXPTH 1.2

float tv[XMAX][YMAX];
float v1;
float tc[XMAX][YMAX];

float tv1[XMAX][YMAX];
float tc1[XMAX][YMAX];

float a;

#define ID_BTN 110

   int WINAPI WinMain (HINSTANCE hInstance, 
                        HINSTANCE hPrevInstance, 
                        PSTR szCmdLine, 
                        int iCmdShow) 
   {
   int x, y;

   a = 1;

   for (x=0; x<XMAX; x++)
   for (y=0; y<YMAX; y++)
   {
    tv[x][y] = (float)y * XSCALE;
    tc[x][y]= 0;
   }

   n = 0;
 
   hInst = hInstance;
      
   MessageBox (NULL, "Hello", "Hello Demo", MB_OK);

   WNDCLASSEX principale;
   principale.cbSize=sizeof(WNDCLASSEX);
   principale.style=CS_HREDRAW|CS_VREDRAW;
   principale.lpfnWndProc=MainProc;
   principale.cbClsExtra=0;
   principale.cbWndExtra=0;
   principale.hInstance=hInstance;
   principale.hIcon=LoadIcon(NULL,IDI_APPLICATION);
   principale.hCursor=LoadCursor(NULL,IDC_ARROW);
   /* principale.hbrBackground=reinterpret_cast<HBRUSH>(COLOR_WINDOW+1);*/
   principale.hbrBackground = COLOR_WINDOW+1;
   principale.lpszMenuName=NULL;
   principale.lpszClassName="std";
   principale.hIconSm=LoadIcon(NULL,IDI_APPLICATION);
   RegisterClassEx(&principale);

   HWND hWnd;
   hWnd=CreateWindowEx(
      WS_EX_CLIENTEDGE,
      "std",
      "Notre fenêtre",
      WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT,
      CW_USEDEFAULT,
      CW_USEDEFAULT,
      CW_USEDEFAULT,
      NULL,
      NULL,
      hInstance,
      NULL
      );

   ShowWindow(hWnd,SW_SHOW);

   MSG msg;
   while(GetMessage(&msg,NULL,0,0)==TRUE)
   {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
   }

      return (0);
   }


LRESULT CALLBACK MainProc(HWND hWnd, UINT mes, WPARAM wParam, LPARAM lParam)
{
   HDC hDC;
   PAINTSTRUCT paintst;
   static HWND btn;
   UINT id;
   RECT rect;
   char buf[100];

   rect.left = 0;
   rect.top = 0;
   rect.right = 255;
   rect.bottom = 255;

loop:
   switch (mes)
   {
   case WM_CREATE :
      btn = CreateWindow ("button", "Bouton", WS_CHILD | WS_VISIBLE,
                                     300, 100, 96, 24, hWnd, ID_BTN, hInst, NULL);
      
      return 0;

   case WM_PAINT:
      hDC=BeginPaint(hWnd,&paintst);

      int x, y;
      int z, zz;
      for (y=0; y<YMAX; y++)
      for (x=0; x<XMAX; x++)
      {
       z = (int)(tv[x][y] / ZSCALE);
       zz = (int)(tc[x][y] / ZZSCALE);

       SetPixel (hDC, x, YMAX-1-y, RGB(zz,0,0));
      }
      EndPaint(hWnd,&paintst);
      return 0;

   case WM_DESTROY:
      PostQuitMessage(0);
      return 0;

   case WM_COMMAND:
      id = LOWORD(wParam);
      switch (id)
      {
       case ID_BTN:
        /* MessageBox (NULL, "Hello", "Bouton cliqué", MB_OK); */
        n++;
        a *= EXPTH;        
	/*
        sprintf (buf, "n=%d", n);
        MessageBox (NULL, buf, "Test", MB_OK);
        */

        for (x=0; x<XMAX; x++)
        for (y=0; y<YMAX; y++)
        {
        float k, s, c, k1, s1, vmax, copt;
        int x1, y1;
         k = x * XSCALE;
         s = y * YSCALE;

         vmax = 0;
         copt = 0;

         for (c=0; c<k; c+=XSCALE)
         {
          k1 = k + sqrt(k) - c;
          s1 = s + a*c;
          x1 = (int)(k1/XSCALE);
          if (x1 >= XMAX)
           x1 = XMAX-1;
          y1 = (int)(s1/YSCALE);
          if (y1 >= YMAX)
           y1 = YMAX-1;
          v1 = tv[x1][y1];
          if (v1 > vmax)
          {
           vmax = v1;
           copt = c;
          } 
         }
         
         /*
	 sprintf (buf, "x=%d y=%d vmax=%f copt=%f",x,y,vmax,copt);
         MessageBox (NULL, buf, "Trace", MB_OK);
	 */

         tv1[x][y] = (int)vmax;
         tc1[x][y] = (int)copt;
      
        }

        memcpy (tv, tv1, XMAX*YMAX*sizeof(float));
        memcpy (tc, tc1, XMAX*YMAX*sizeof(float));

        InvalidateRect (hWnd, NULL, TRUE);
        /*
        mes = WM_PAINT;
        goto loop;
        PostMessage (hWnd, WM_PAINT, 0, 0);
        */
        break;
      }
      return 0;

   default:
      return DefWindowProc(hWnd, mes, wParam, lParam);
   }
}
               