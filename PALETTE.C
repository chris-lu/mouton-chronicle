/***************************************************************
Palette :
0-32:premier plan
32-64:etoiles
64-160:2nd plan
*/

void Mk_Snow(unsigned char far *Palette)
{
register cont;
for (cont=0;cont<32;cont++)
{
Palette[cont*3]=cont*7/4;
Palette[cont*3+1]=cont*7/4;
Palette[cont*3+2]=cont*2;
}
}

void Mk_Stars(unsigned char far *Palette)
{
register cont;
/*for (cont=32;cont<48;cont++)
{
Palette[cont*3]=(cont+16)/2;
Palette[cont*3+1]=(cont+16)/2;
Palette[cont*3+2]=(cont+16)/2;
} */
for(cont=0;cont<16;cont++)
{
Palette[(32+cont)*3]=(cont)*3;
Palette[(32+cont)*3+1]=(cont)*3;
Palette[(32+cont)*3+2]=(cont)*3;
Palette[(64-cont)*3]=(cont)*3;
Palette[(64-cont)*3+1]=(cont)*3;
Palette[(64-cont)*3+2]=(cont)*3;
}
Palette[48*3]=48;
Palette[48*3+1]=48;
Palette[48*3+2]=48;

}

void Rot_Pal(unsigned short deb,unsigned short nb, unsigned char *Pal1,unsigned char *Pal2)
{
char r,v,b;
nb*=3;
deb*=3;
asm{
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


void CpyPal(unsigned char *Pal1,unsigned char *Pal2)
{
register cont;
for(cont=0;cont<768;cont++)
Pal2[cont]=Pal1[cont];
}


void Create_Pal(unsigned char src,unsigned char nb,unsigned char* Pal1,unsigned char dst,unsigned char *Pal2)
{
register cont;
for (cont=0;cont<nb;cont++)
{
Pal2[(cont+dst)*3]=Pal1[(cont+src)*3];
Pal2[(cont+dst)*3+1]=Pal1[(cont+src)*3+1];
Pal2[(cont+dst)*3+2]= Pal1[(cont+src)*3+2];
}
}

void Inc_Scr(unsigned char nb,unsigned char far *Scr)
{
unsigned long cont;
for(cont=0;cont<64000;cont++)
	if(Scr[cont])
	Scr[cont]+=nb;
}


