/*************************************************************/
// TITRE   : BIBLIOTHEQUE DE GESTION DE LA SOURIS
// FICHIER : MOUSE.C
/*************************************************************/

#include "mouse.h"

void InitMouse(void)
{
	union REGS regs;

	regs.x.ax=0;
	int86(0x33,&regs,&regs);
}

void MouseON(void)
{
	union REGS regs;

	regs.x.ax=1;
	int86 (0x33,&regs,&regs);
}

void MouseOFF(void)
{
	union REGS regs;

	regs.x.ax=2;
	int86 (0x33,&regs,&regs);
}

void MouseBout(void)
{
	union REGS regs;

	regs.x.bx=0;
	int86(0x33,&regs,&regs);
}
unsigned short GetMouse(unsigned short *x,unsigned short *y)
{
	union REGS regs;

	regs.x.ax=3;
	int86(0x33,&regs,&regs);

	*x=regs.x.cx>>1;
	*y=regs.x.dx;

	return(regs.x.bx);
}

void SetMouse(unsigned short x,unsigned short y)
{
	union REGS regs;

	regs.x.ax=4;
	regs.x.cx=x<<1;
	regs.x.dx=y;

	int86(0x33,&regs,&regs);
}

void ZoneMouse(unsigned short x1,unsigned short y1,unsigned short x2,unsigned short y2)
{
	union REGS regs;

	regs.x.ax=7;
	regs.x.cx=x1<<1;
	regs.x.dx=x2<<1;

	int86(0x33,&regs,&regs);

	regs.x.ax=8;
	regs.x.cx=y1;
	regs.x.dx=y2;

	int86(0x33,&regs,&regs);
}
