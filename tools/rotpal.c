#include "../src/compat.h"
#include <stdio.h>
#include <conio.h>
#include "../src/vga_c.c"
#include "../src/vga.c"

unsigned char Page[64000];
unsigned char Pal[768];
void Rot_Pal(unsigned short deb,unsigned short nb, unsigned char *Pal1,unsigned char *Pal2)
{
char r,v,b;
nb*=3;
deb*=3;
_asm {
push ds
mov dx,nb
lds si,Pal1
les di,Pal2
mov cx,0
mov ax,deb
add si,ax
add di,ax
mov ah,ds:[si]
mov r,ah
mov ah,ds:[si+1]
mov v,ah
mov ah,ds:[si+2]
mov b,ah


boucle:
mov ah,ds:[si+3]
mov es:[di],ah
inc si
inc di
inc cx
cmp cx,dx
jne boucle
mov ah,r
mov es:[di],ah
mov ah,v
mov es:[di+1],ah
mov ah,b
mov es:[di+2],ah

pop ds
}
}



void main(void)
{
unsigned char cont=0;
ModeVGA();
LoadPCX("test.pcx",Page,Pal);
SetAllPala(Pal);
while(!kbhit())
{
cont++;
if(!(cont&7))
	{
	Rot_Pal(52,10,Pal,Pal);
	SetAllPala(Pal);
	}
Bal();
Draw(Page,0);
}
exit(0);
}
