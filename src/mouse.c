/*************************************************************/
// TITRE   : BIBLIOTHEQUE DE GESTION DE LA SOURIS
// FICHIER : MOUSE.C
/*************************************************************/

#include "compat.h"
#include "mouse.h"

void InitMouse(void)
{
	_asm {
	push ds
	mov ax,0
	int 33h
	pop ds
	}
}

void ReInitMouse(void)
{
	_asm {
	push ds
	mov ax,21h
	int 33h
	pop ds
	}
}

void ActiveMouse(void)
{
	_asm {
	push ds
	mov ax,20h
	int 33h
	pop ds
	}
}

void MouseON(void)
{
	_asm {
	mov ax,1
	int 33h
	}
}

void MouseOFF(void)
{
	_asm {
	push ds
	mov ax,2
	int 33h
	pop ds
	}
}

/*void MouseBout(void)
{
	_asm {
	mov bx,0
	int 33h
	}
}       */
unsigned short GetMouse(unsigned short *px,unsigned short *py)
{
  //	register short tmp;
	_asm {
	push ds
	mov ax,3
	int 33h
	shr cx,1
	les di,px
	mov es:[di],cx
	les di,py
	mov es:[di],dx
	pop ds
	mov ax,bx              //renvoi toujour le registre AX
	}
  //	return(tmp);
}

void SetMouse(unsigned short mx,unsigned short my)
{
  _asm {
  push ds
  mov ax,4
  mov cx,mx
  shl cx,1
  mov dx,my
  int 33h
  pop ds
  }
  /*	union REGS regs;

	regs.mx.ax=4;
	regs.mx.cx=mx<<1;
	regs.mx.dx=my;

	int86(0x33,&regs,&regs);  */
}

void ZoneMouse(unsigned short x1,unsigned short y1,unsigned short x2,unsigned short y2)
{
	_asm {
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
