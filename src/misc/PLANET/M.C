
#include <dos.h>

union REGS reg;


m_init ()
{

}

m_close ()
{
}

m_show ()
{
	reg.x.ax = 1;
	int86 (0x33, &reg, &reg);
}

m_hide ()
{
	reg.x.ax = 2;
	int86 (0x33, &reg, &reg);
}

int m_xpos ()
{
	reg.x.ax = 3;
	int86 (0x33, &reg, &reg);
	return reg.x.cx;
}

int m_ypos ()
{
	reg.x.ax = 3;
	int86 (0x33, &reg, &reg);
	return reg.x.dx;
}

int m_button ()
{
	reg.x.ax = 3;
	int86 (0x33, &reg, &reg);
	return reg.x.bx;
}


