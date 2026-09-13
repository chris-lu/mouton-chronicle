#include "../src/compat.h"
#include <conio.h>
#include <dos.h>

unsigned char far *Video=(unsigned char far*)(0xB8000000);
unsigned char far Page[8000];
unsigned short Cur_Char=0,Cur_Pos=0;
unsigned char Pal[192],Last[192];
void far interrupt (*Vide)(void);
unsigned char Txt[]={"\nInitializing ±&±&±&±&±&±&±&±&±&±&±&   Done. |\n\nEnterring I.S.N. ±&±&±&±&±&±&±&   Done. ||\n\n\n********************************\n*  S.H.E.E.P. Command system   *\n******************************** \n||\nCommand : \n-> &&\rExecute Secret_Project \n\n| Loading files& ±&±&±&±&±&±&±&±& Done. \n Files loaded. Lunching. |"};

void ModeVGA(void)
{
  _asm {
  push ds
  mov ax,13h
  int 10h
  pop ds}

}


void Move(unsigned char col,unsigned char row)
{
  _asm {
  push ds
  mov ah,02h
  mov bh,0
  mov dh,row
  mov dl,col
  int 10h
  pop ds
  }
}


void Mode(void)
{
  _asm {
  push ds
  mov ax,1112h
  mov bl,0
  int 10h
  pop ds
  }
}


void Draw8025(unsigned char far *Pg)
{
_asm {
  push ds
  push ss
  pop ds			//DS=DGROUP : Open Watcom laisse DS flotter, les globales (Video) sont lues via DS
  les di,Video
  lds si,Pg
  mov cx,1998
db 66h
  rep movsw
  pop ds
}
}

/*void Wait(unsigned long TIME)
{
_asm {
push ds
mov ah,86h
mov cx,0
mov dx,TIME
int 15h
pop ds
}
}    */


void Put_Char(char c,unsigned char BgCoul,unsigned char TxtCoul,unsigned char far* Pg,unsigned short TIME)
{
_asm {
push ds
push ss
pop ds			//DS=DGROUP : Open Watcom laisse DS flotter ; globales lues via DS : Cur_Pos
les di,Pg
mov ax,Cur_Pos
shl ax,1
add di,ax
mov al,c
mov es:[di],al
mov al,BgCoul
shl al,4
add al,TxtCoul
mov es:[di+1],al
pop ds
}
delay(TIME);
}

void Dec_Scr(unsigned char far * Pg)
{
_asm {
push ds
lds si,Pg
les di,Pg
mov cx,2000
mov bx,2
deb:
mov al,ds:[si+1]
cmp al,1
jle suite
dec al
mov es:[di+1],al
suite:
add si,bx
add di,bx
dec cx
or cx,0
jnz deb
pop ds
}
}


void Mk_Norm(unsigned char far * Pg)
{
_asm {
push ds
lds si,Pg
les di,Pg
mov cx,2000
mov bx,2
mov al,7
deb:
mov es:[di+1],al
add si,bx
add di,bx
dec cx
or cx,0
jnz deb
pop ds
}
}

void SetInk(unsigned char e,unsigned char r,unsigned char v,unsigned char b)
{
  outportb(0x3c8,e);
  outportb(0x3c9,r);
  outportb(0x3c9,v);
  outportb(0x3c9,b);
}

void GetInk(unsigned char e,unsigned char *r,unsigned char *v,unsigned char *b)
{
outportb(0x3c7,e);
*r=inportb(0x3c9);
*v=inportb(0x3c9);
*b=inportb(0x3c9);
}

void far interrupt Coul(void)
{
Dec_Scr(Page);
}

void main(void)
{
register cont;
Mode();
getch();
_setcursortype(1);
/*
Pal[0]=0;
Pal[1]=0;
Pal[2]=0;
for(cont=1;cont<8;cont++)
	{
	Pal[cont*3]=42+(cont-1)*3;
	Pal[cont*3+1]=42-(cont-1)*5;
	Pal[cont*3+2]=42-(cont-1)*5;
	}
Pal[(20)*3]=60;
Pal[(20)*3+1]=12;
Pal[(20)*3+2]=12;
for(cont=0;cont<=8;cont++)
	{
	Pal[(cont+56)*3]=63;
	Pal[(cont+56)*3+1]=cont*63/8;
	Pal[(cont+56)*3+2]=0;
	}
				*/
Pal[7*3]=0;
Pal[7*3+1]=40;
Pal[7*3+2]=0;

for(cont=0;cont<64;cont++)
	{
	GetInk(cont,&Last[cont*3],&Last[cont*3+1],&Last[cont*3+2]);
	SetInk(cont,Pal[cont*3],Pal[cont*3+1],Pal[cont*3+2]);
	}
//Vide=getvect(0x1c);
//setvect(0x1c,Coul);

while(Txt[Cur_Char]!=0)
{if(Txt[Cur_Char]=='\n')
	{	Cur_Pos=((short)(Cur_Pos/80)+1)*80-1;}
else if(Txt[Cur_Char]=='\r')
	{Cur_Pos--;Put_Char(' ',0,7,Page,70);Cur_Pos--;for(cont=0;cont<32000;cont++);}
else if(Txt[Cur_Char]=='|')
	{delay(1000);Cur_Pos--;}
else if(Txt[Cur_Char]=='&')
	{delay(400);Cur_Pos--;}
else if(Txt[Cur_Char]=='*')
	Put_Char(Txt[Cur_Char],0,7,Page,10);
else Put_Char(Txt[Cur_Char],0,7,Page,70);
Cur_Char++;Cur_Pos++;
gotoxy((Cur_Pos)%80,((Cur_Pos)/80)+1);
Draw8025(Page);
}
for(cont=0;cont<6000;cont++) Draw8025(Page);
//setvect(0x1c,Vide);
//for(cont=0;cont<64;cont++)	SetInk(cont,Last[cont*3],Last[cont*3+1],Last[cont*3+2]);
//Mk_Norm(Page);
//Draw8025(Page);
//gotoxy(1,(Cur_Pos/80)+2);
//_setcursortype(2);
clrscr();
getch();
getch();
}
