//*****VESA=Video Electronic Standards Association
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <alloc.h>
#include <dos.h>
#define bytef unsigned char far
#define byte unsigned char
#define wordf unsigned short far
#define word unsigned short


struct
{
bytef sign[4];
bytef vers[2];
bytef *cons;
bytef *rien;
wordf *liste;
bytef dic[238];
}VesaInfo;

struct
{
word Flag;
byte Flag1;
byte Flag2;
word Gran;
word Taille;
word adr1;
word adr2;
void far *rout;
word octlin;
word resx;
word resy;
byte charl;
byte charh;
byte nbplan;
byte bpp;
byte nbblocks;
byte nbmodmem;
byte tailbl;
}ModeInfo;



bytef *pbuf=(&VesaInfo.sign[0]);





void Get_Vesa_Info(void)
{
asm{
push ax
push ds
mov ah,4fh
mov al,00h
les di,pbuf
int 10h
pop ax
pop ds
}
}

void main(void)
{
register word cont;
Get_Vesa_Info();
for(cont=0;cont<4;cont++)
	printf("%c",VesaInfo.sign[cont]);
printf("\nVersion %d.%d",VesaInfo.vers[1],VesaInfo.vers[0]);
printf("\nFabriquant : %s",VesaInfo.cons);
cont=0;
while(cont!=0xFFFF&&cont<50)
{
printf("\n%d",VesaInfo.liste[cont]);
}

getch();
}



