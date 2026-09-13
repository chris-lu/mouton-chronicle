#include <stdio.h>
#include <conio.h>
#include <math.h>
#include <alloc.h>
#include <time.h>
//#include "rtime.c"
#include <string.h>
#include "../src/vga_c.c"
#include "../src/vga.c"
#include "../src/mouse.c"
#include "../src/cd.c"

#define NB 97

#define Rayon 28                         //26
#define Diametre 56

unsigned char far *Page;
unsigned char far *Image1;
unsigned char far *Image2;
unsigned char far *Pal;
//unsigned char far *Pal2;
byte *Block;
char CercleFin[Diametre];
char CercleDeb[Diametre];

byte *Y;
short *X;
unsigned short YP[200];

char Texte[97][45]={"           MOUTON CHRONICLES","            Version 0.13 Beta","","    Creation et developpement :","Atlantys Team","","    Idee Originale :","Dri Christophe (GnoMaster)","Jolivalt Jonathan (dJo)", "Baudin Aymeric(Bart57)","","    Programmation/Graphismes :"," GnoMaster","", "    Graphismes/Musiques :","dJo","","    Graphismes/Documentation" ,"Bart57","","    Testeurs :","GnoMaster","dJo","Bart57","Guinness","Wax Renaud","Del Frate Sylvain","","Remerciements Suplementaires : ","Mr. Chambon","Mr. Mariate","Bouchraki Khier","Marez Julien","Mercier Julien","Michel Geoffrey","Ney Jonathan","Signo Julien","Wilkosz Alexia",
"Zannoni Adeline","","Tous les habitants d'IRC :) ","","Etc...(la liste est encore longue)","","","","","","Aucun Mouton n'a ete blesse ou tue","au cour du devellopement de ce jeu.","","Toutes images pouvant faire croire a des","actes de violance envers l'une des","betes du jeu sont fausses!Il sagissait","seulement de figurants deguises en","moutons et non de vrais moutons. . .","","","","","",
"Nous rendons aussi homage aux","figurants blesses ou tue au cour du ","devellopement de ce jeu et present-","ons nos excuses a leurs familles.","","","","","","","","","","","","","","","","","","","","C'est FINI !!!","","","","","","","Enfin, jusqu'au prochain jeu....."};

unsigned char far *Lettre[96];	//Lettre
char far Let_Lg[96]={4,2,6,13,10,12,11,2,5,5,7,8,2,5,2,8,9,6,8,7,10,8,8,9,8,8,2,3,5,6,5,7,12,10,8,8,10,7,9,10,10,8,10,8,7,13,11,11,7,13,9,9,10,10,9,15,10,9,10,5,6,5,6,10,3,8,8,7,9,7,7,7,7,2,6,7,2,10,7,7,7,7,6,6,6,7,7,10,8,8,7,5,2,6,8};
/*
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

//***********************************************************************************
//incremente les couleurs d'une image
//***********************************************************************************
void Inc_Scr(unsigned char nb,unsigned char far *Scr)
{
unsigned long cont;
for(cont=0;cont<64000;cont++)
	if(Scr[cont])
		Scr[cont]+=nb;
}

			 */
void Init_Move(void)
{
register long cont;
for (cont=0;cont<2048;cont++)
{
X[cont]=160-Rayon+cos(cont*0.00307*20)*sin(cont*0.00307*8)*(160-Rayon);
Y[cont]=100-Rayon+sin(cont*0.00307*12+1)*cos(cont*0.00307*16+1)*(100-Rayon);
}
}




void cercle(void)
{
register cont,cont2;
for (cont=-Rayon;cont<Rayon;cont++)
	{
	CercleFin[cont+Rayon]=sqrt(Rayon*Rayon-cont*cont);
	CercleDeb[cont+Rayon]=-CercleFin[cont+Rayon]+Rayon;
	CercleFin[cont+Rayon]+=Rayon;
	}
}

void Loupe(short x,short y,byte *scr,byte *blk2)
{
register i,j;
float a,b,c;
long pos;
short b2;
pos=YP[y]+x;

for(j=0;j<Diametre;j++)
	for (i=CercleDeb[j];i<CercleFin[j];i++)
	{
	 a=i-Rayon;
	 a/=Rayon;

	 b=j-Rayon;
	 b/=Rayon;

	 c=sqrt(a*a+b*b);

	 c*=Rayon;

	 a*=c;
	 b*=c;

	 a+=Rayon;
	 b+=Rayon;

	 blk2[(j*Diametre)+i]=scr[pos+((int)b)*320+a];
	}
 }





void Print_Let(unsigned short x,byte y,byte lg,byte ht,byte *scr,byte Nb)
{
unsigned int t;
void *Poit;
Poit=Lettre[Nb];
t=(y<<8)+(y<<6);
asm{
  push ds
  les di,scr
  lds si,Poit
  mov cx,t
  add cx,x
  add di,cx
  mov ax,di
  mov cl,ht
lbl1:
  mov ch,lg
lbl2:
  xor dx,dx
  xor bx,bx
  mov dl,ds:[si]
  or dl,dl
  jz lbl3*/
  mov bl,es:[di]
  add dx,bx
  cmp dx,255
  jle sto2
sto1:
  mov dl,255
sto2:
  mov es:[di],dl

lbl3:
  inc si
  inc di
  dec ch
  or ch,ch
  jnz lbl2

  add ax,320
  mov di,ax
  dec cl
  or cl,cl
  jnz lbl1
  pop ds
	}
}




void Load_Txt(void)
{
register i,j;
LoadPCX("mouton/FontC.pcx",Page,Pal);       //charger les sprites de fond
for(j=0;j<5;j++)
	for(i=0;i<19;i++)
		GetBlk1((i*16),(j*16),16,16,Page,Lettre[j*19+i]);

}

void Print_Txt(unsigned short x,unsigned char y,char * txt,unsigned char *Page)
{
register cont=0;
while(txt[cont]!='\0')
{
if((txt[cont]>31)&&(txt[cont]<127))
	{
	if(x>304)
		return;
	if(y>183)
		goto suite;
	Print_Let(x,y,16,16,Page,txt[cont]-32);
	suite:
	x+=Let_Lg[txt[cont]-32]+2;
	}
cont++;
}
}
	  /*
void Print_Nb(unsigned short x,unsigned char y,unsigned short nb,unsigned char *Page)
{
unsigned char Lng=1;
unsigned short Exp=10;
unsigned char temp;
while(nb>=Exp)
	{
	Exp*=10;
	Lng++;
	}
Exp/=10;
while(Lng)
{
temp=(nb/Exp);
nb-=temp*Exp;
{
if(x>304)
	return;
	Print_Let(x,y,16,16,Page,temp+16);
	x+=Let_Lg[temp+16]+2;
}
Lng--;
Exp/=10;
}
}
			 */
 /*
void Blur(bytef *src,bytef* dst)   //brule l'ecran(assembleur)
{
asm	{
	push ds                              //sauvegarde ds
	les di,dst                           //|mémorise les adresses des pages
	lds si,src
	mov ax,320
	add si,ax                           //|
	mov cx,63680                         //ignore le ligne du bas
loop1:
	xor ax,ax                            //|remet a zero ax et bx
	xor bx,bx                            //|
	mov al,[si]                          //addition des différents points
	mov bl,[si+1]                        //point de droite
	add ax,bx
	mov bl,[si-1]                        //point de gauche
	add ax,bx
	mov bl,[si+320]                        //point de gauche
	adc ax,bx
	mov bh,4
	div bh                             //division par 4
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
			 */

void GotoAB(unsigned char *src,unsigned char *dst)
{
asm{
push ds
les di,dst
lds si,src
mov cx,64000
deb:
mov al,es:[di]
mov ah,ds:[si]
cmp ah,al
jb suite
cmp ah,al
je fin

inc al
mov es:[di],al
jmp fin
suite:
dec al
mov es:[di],al

fin:
inc si
inc di
dec cx
or cx,cx
jnz deb
pop ds
}
}





void main(void)
{
register cont,c,cur;
//short JustPush=0;
unsigned char Transit=0;
unsigned short x=0,y=0;
short texte[14][2];
unsigned char lasttxt=0,b=0;
unsigned char CurScreen=0;
char Name[50];

for(cont=0;cont<96;cont++)
	Lettre[cont]=(unsigned char far*)farmalloc(16*17*sizeof(char));
Page=(unsigned char far*)farmalloc(320*200*sizeof(char));
Image1=(unsigned char far*)farmalloc(320*200*sizeof(char));
Image2=(unsigned char far*)farmalloc(320*200*sizeof(char));
Pal=(unsigned char far*)farmalloc(768*sizeof(char));
Block=(byte*)calloc(Diametre*Diametre,sizeof(byte));

X=(short*)malloc(2048*sizeof(short));   //allocation mémoire de la page video
Y=(byte*)malloc(2048*sizeof(byte));   //allocation mémoire de la page video

for (cont=0;cont<200;cont++)
	YP[cont]=cont*320;
Clr(Page);
Load_Txt();
Init_Move();
ModeVGA();
cercle();
LoadPCX("mouton/screen0.pcx",Image1,Pal);       //charger les sprites de fond
LoadPCX("mouton/screen1.pcx",Image2,Pal);       //charger les sprites de fond
SetAllPala(Pal);
Clr(Page);
lasttxt=0;
for(cont=0;cont<50;cont++)
texte[cont][0]=-1;

lasttxt=0;
texte[0][0]=220;
texte[0][1]=lasttxt++;
ZoneMouse(0,0,320-Diametre,200-Diametre);
c=4;
CurScreen=2;
Transit=0;
Nb_CD();
Cur_Lect=CD.First;
Get_T_Inf();
Play(9);


while(!kbhit())
{
b=GetMouse(&x,&y);

Cpy(Image1,Page);
c++;
c&=2047;

if(!(c&1023))
	Transit=255;
if(Transit)
	{
	GotoAB(Image2,Image1);
	Transit--;
	if(!Transit)
		{
		sprintf(Name,"mouton/screen%d.pcx",CurScreen);
		LoadPCX(Name,Image2,Pal);       //charger les sprites de fond
		CurScreen++;
		CurScreen%=6;
		}
	}

Cpy(Image1,Page);

for(cont=0;cont<14;cont++)
{
if(c&1)
{
if(texte[cont][0]>=0)
	texte[cont][0]--;
if((texte[cont][0]==190)&&lasttxt<NB)
	{
	texte[(cont+1)%14][0]=220;
	texte[(cont+1)%14][1]=lasttxt++;
	}
}
Print_Txt(5,texte[cont][0],Texte[texte[cont][1]],Page);
}

if(b)
	{
	Loupe(x,y,Page,Block);
	Put(x,y,Diametre,Diametre,Page,Block);
	}
else
	{
	Loupe(X[c],Y[c],Page,Block);
	Put(X[c],Y[c],Diametre,Diametre,Page,Block);
	}
Bal();
Draw(Page,0);
Clr(Page);
}
Stop();
ModeTxt();
}
