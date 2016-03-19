
#include <owl.h>

#include "mainwin.h"

class TFenetreFourmi : public TWindow
{
public:
	TFenetreFourmi (LPSTR Titre) : TWindow (NULL, Titre) {}
	virtual void Paint (HDC np, PAINTSTRUCT& info)
	{
    /*
	int i;
		for (i=10; i<50; i++)
			SetPixel (np, i, i, RGB(0,0,0));
	*/
#define T t
	int x, y, dir, t;
		x = 250;
		y = 60;
		dir = 1;
		while (x > 0 && x < 500 && y > 0 && y < 120)
		{
			if (GetPixel (np, x, y) != RGB(0,0,0))
			{
            	SetPixel (np, x, y, RGB(0,0,0));
				dir = (dir + 1) % 4;
				t = 1;			
			}
			else			
			{
				SetPixel (np, x, y, RGB(/*0,0,0*/255,255,255));
				dir = (dir + 3) % 4;
				t = 1;
			}
			switch (dir)
				{
					case 0: x += T; break;
					case 1: y += T; break;
					case 2: x -= T; break;
                    case 3: y -= T; break;
				}

        }
			
	}
};

MAINWIN (TFenetreFourmi, "Fourmi")
