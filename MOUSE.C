/*************************************************************/
// TITRE   : BIBLIOTHEQUE DE GESTION DE LA SOURIS
// FICHIER : MOUSE.C
/*************************************************************/

#include "mouse.h"

void InitMouse(void)
{
	asm
	{
	mov ax,0
	int 33h
	}
}

void MouseON(void)
{
	asm
	{
	mov ax,1
	int 33h
	}
}

void MouseOFF(void)
{
	asm
	{
	mov ax,2
	int 33h
	}
}

/*void MouseBout(void)
{
	asm
	{
	mov bx,0
	int 33h
	}
}       */
unsigned short GetMouse(unsigned short *x,unsigned short *y)
{
	//register short tmp;
	asm
	{
	push ds
	mov ax,3
	int 33h
	shr cx,1
	les di,x
	mov es:[di],cx
	les di,y
	mov es:[di],dx
	pop ds
	mov ax,bx              //renvoi toujour le registre AX
	}
	//return(tmp);
}

void SetMouse(unsigned short x,unsigned short y)
{
  asm{
  push ds
  mov ax,4
  mov cx,x
  shl cx,1
  mov dx,y
  int 33h
  pop ds
  }
  /*	union REGS regs;

	regs.x.ax=4;
	regs.x.cx=x<<1;
	regs.x.dx=y;

	int86(0x33,&regs,&regs);  */
}

void ZoneMouse(unsigned short x1,unsigned short y1,unsigned short x2,unsigned short y2)
{
	asm{
	push ds
	mov ax,7
	mov cx,x1
	shl cx,1
	mov dx,x2
	shl dx,1
	int 33h
	mov ax,8
	mov cx,y1
	mov dx,y2
	int 33h
	pop ds
	}

	/*union REGS regs;

	regs.x.ax=7;
	regs.x.cx=x1<<1;
	regs.x.dx=x2<<1;

	int86(0x33,&regs,&regs);

	regs.x.ax=8;
	regs.x.cx=y1;
	regs.x.dx=y2;

	int86(0x33,&regs,&regs);*/
}
