#include <vga.h>
bytef *Video=(bytef*)MK_FP(0xa000,0);     // Adresse Video
char co=20;
char dr=3;

void far interrupt Coul(void)
{
co+=dr;
if (co>60||co<20)
	dr=-dr;
outportb(0x3c8,4);
outportb(0x3c9,co);
outportb(0x3c9,co/4);
outportb(0x3c9,co/4);
}

void Sign(void)
{
setvect(0x1c,Coul);
textcolor(4);
cprintf("GnoMaster");
textcolor(7);
}

void ModeVGA(void)
{
  union REGS regs;

  regs.x.ax=0x0013;
  int86(0x10,&regs,&regs);
}

void ModeTxt(void)
{
  union REGS regs;

  regs.x.ax=0x0003;
  int86(0x10,&regs,&regs);
}




void Bal(void)                    //retour de balayage
{
asm{
mov dx,0x3DA;
wait1:
in al,dx;
test al,8;
jz wait1;
}
}

void Put(short x,byte y,byte l,byte h,byte *scr,byte *spr)
{
unsigned int t,l2,h2;
h2=(int)h;
l2=(int)l;

t=(y<<8)+(y<<6);
asm{
  push ds
  les di,scr
  lds si,spr
  mov cx,t
  add cx,x
  add di,cx
  mov ax,di
  mov bx,h2
psprh:
  mov cx,l2
psprl:
  mov dl,ds:[si]
  or dl,dl
  jz psprl2
  mov es:[di],dl
psprl2:
  inc si
  inc di
  dec cx
  or cx,cx
  jnz psprl

  add ax,320
  mov di,ax
  dec bx
  or bx,bx
  jnz psprh
  pop ds
	}
}

void Draw(bytef *src)
{
asm{
  push ds
  les di,Video
  lds si,src
  mov cx,16000
db 66h
  rep movsw
  pop ds
}
}

void Cpy(bytef *src,bytef *dst)
{
asm{
  push ds
  les di,dst
  lds si,src
  mov cx,16000
db 66h
  rep movsw
  pop ds
}
}

void Clr(bytef *dst)
{
asm{
  push ds
  les di,dst
  xor ax,ax
  mov cx,32000
  rep stosw
  pop ds
}
}

