#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <alloc.h>
#include "mouse.c"
#include "vga.h"
#include "vga.c"
#include "vga_c.c"
#include "textemen.c"
#include <time.h>

unsigned short x,y,b;
unsigned char temp;


void FlamesAsm(bytef *src,bytef* dst)   //brule l'ecran(assembleur)
{
asm{
	push ds                              //sauvegarde ds
	les di,dst                           //|mémorise les adresses des pages
	lds si,src                           //|
	mov ax,51200
	add di,ax
	add si,ax
	mov cx,12480                         //ignore le ligne du bas
loop1:
	xor ax,ax                            //|remet a zero ax et bx
	xor bx,bx                            //|
	mov al,[si]                          //addition des différents points
	mov bl,[si+1]                        //point de droite
	add ax,bx
	mov bl,[si-1]                        //point de gauche
	add ax,bx
	mov bl,[si+320]                      //point du dessous
	add ax,bx
	shr ax,2                             //division par 4
	or al,al                             //al = 0? sinon décrémenter(effet de palette)
	jz suite
	dec al
suite:
	stosb                                //ecrit la valeur de déstination dans dst
	inc si                               //point suivant
	dec cx                               //décrémentation du compteur
	or cx,cx                            //cx = 0? sinon on recommence
	jnz loop1
	pop ds                               //rétablit ds
	}


}













void main(void)
{
register cont;
Page=farmalloc(320*200*sizeof(char));
Pal=farmalloc(768*sizeof(char));
for(cont=0;cont<97;cont++)
	Lettre[cont]=(unsigned char far *)farmalloc(144);



