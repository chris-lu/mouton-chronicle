#include <conio.h>
#include <math.h>
#include <mouse.c>
#include <alloc.h>
#include <time.h>
#include "variable.c"
#include "Rtime.c"
#include "vga_c.c"
#include "vga.c"
#include "palette.c"
#include "plans.c"
//couleurs 2eme plan : 96


unsigned short Y[960];
char A,B,C;

char explose=0;
short dirx=12;
short diry=35;
unsigned short posy=150<<6;
unsigned short posx=100<<6;


void Init_Mem(void)
{
register cont;
for(cont=0;cont<3;cont++)
	if((Plan_1[cont]=(unsigned char far *)farmalloc(64000))==NULL)
	{
	printf("\npas assez de memoire conv.!");
	exit(1);
	}
for(cont=0;cont<2;cont++)
	if((Plan_2[cont]=(unsigned char far *)farmalloc(64000))==NULL)
	{
	printf("\npas assez de memoire conv.!");
	exit(1);
	}
/*if((Plan_3[0]=(unsigned char far *)farmalloc(64000))==NULL)
	{
	printf("\npas assez de memoire conv.!");
	exit(1);
	}*/
if((Page=(unsigned char far *)farmalloc(64000))==NULL)
	{
	printf("\npas assez de memoire conv.!");
	exit(1);
	}
for(cont=0;cont<3;cont++)
Pal[cont]=(unsigned char far *)farmalloc(768);
}


void Init_Ter(void)
{
register long cont;
unsigned short haut;
unsigned short temp;
unsigned char pos=100;
randomize();
A=random(3)+2;
B=random(12)+10;
C=random(13)+9;

for (cont=0;cont<960;cont++)
{
//Y[cont]=(sin(cont*0.001*A+1))*cos((cont)*0.0009*B+1)+cos((cont)*C*0.002)*0.1;
//Y[cont]=(random(5)-2)+((sin(cont*0.001*A+1))*cos((cont)*0.0009*B+1)+cos((cont)*C*0.002)*0.1);
//Y2[cont]=Y[cont];
//Y[cont]*=Y[cont];
/*Y[cont]*=89; */
Y[cont]=(random(5)-2)+((sin((cont/1000)*A+1))*cos((cont/1000)*B+1)+cos((cont/500)*C)/10);
if((pos+Y[cont]>25)&&(pos+Y[cont]<190))
pos+=Y[cont];

temp=(200-pos)*320+(cont%320);
Plan_1[cont/320][temp]=31;
Plan_1[cont/320][temp+320]=30;
Plan_1[cont/320][temp+640]=29;
Plan_1[cont/320][temp+960]=28;
Plan_1[cont/320][temp+1280]=29;
Plan_1[cont/320][temp+1600]=30;
for(haut=pos-6;haut>0;haut--)
  Plan_1[cont/320][(200-haut)*320+(cont%320)]=31;
}
}

void Memorise_Spr(void)
{
GetBlk2(25,0,24,24,Page,Trou);
}

void Get_Lune(void)
{
GetBlk2(0,0,24,24,Page,Lune);
}

void Init_Bomb(void)
{
explose=0;
dirx=(random(40)-20)/2;
diry=(random(40)+10)/2;
posy=200<<6;
posx=(random(320)+320)<<6;
}


void Put_trou(short x,short y,word l,word h,bytef *buf)
{
  register byte j;
  register word i;
  for(j=0;j<h;j++)
  {
		for(i=0;i<l;i++)
	 {
		if(*buf==1)
			Plan_1[(x+i)/320][(y*320)+x+i]=0; //Transparence … l'encre 0
		buf++;
	 }
	 y++;
  }
}


void Move_Bomb(void)
{
if(!explose)
{
diry-=1;
posy+=diry;
posx+=dirx;
}
if ((posy>0)&&(posy<12800)&&(posx>0)&&(posx<61440))
if(Plan_1[(int)(posx>>6)/320][(((int)(200-(posy>>6))*320)+(int)(posx>>6))]||((posy>>6)<1))
	{
	explose=1;
	Put_trou((posx>>6)-14,200-(posy>>6)-14,24,24,Trou);
	}
}

void main(void)
{
unsigned long images=0;
unsigned short cont;
unsigned short x,y;
printf("Si ce progrmme fait planter windows(ce qui ne chagera pas vos habitudes)\nou redemarrer votre pc : vous n'avez pas assez de mememoire convetionnelle!\n(Mais normallement, Y'a pas de probèmes!)");
getch();
Init_Mem();

for(cont=0;cont<3;cont++)
  Clr(Plan_1[cont]);

for(cont=0;cont<2;cont++)
	LoadPCX(fich[cont],Plan_2[cont],Pal[1]);
Create_Pal(0,96,Pal[1],64,Pal[0]);
LoadPCX("BackStar.pcx",Page,Pal[1]);
Create_Pal(0,31,Pal[1],160,Pal[0]);
Memorise_Spr();
Inc_Scr(160,Page);
Get_Lune();


Init_Ter();
Mk_Snow(Pal[0]);
Mk_Stars(Pal[0]);
Inc_Scr(64,Plan_2[0]);
Inc_Scr(64,Plan_2[1]);
Gen_Back();
ModeVGA();
getch();
Pal[0][765]=63;
Pal[0][766]=63;
Pal[0][767]=63;

SetAllPal(Pal[0]);
InitMouse();
ZoneMouse(0,50,639,200);
//DebTime();
//Put_trou(100,150,28,28,Plan_1[0],Trou);
while(!kbhit())
{
//images++;
GetMouse(&x,&y);
//Cpy(Plan_3[0],Page);
Clr(Page);
Draw_Back(Page);
CpyPlan(x>>2,(y>>1)+100,Plan_2,Page);
CpyPlan(x,y,Plan_1,Page);
Move_Bomb();
if(explose)
	Init_Bomb();
	else
if(((posx>>6)>x)&&((posx>>6)<(x+320))&&((posy>>6)>200-y)&&((posy>>6)<400-y))
	Page[(((int)(400-y-(posy>>6))*320)+(int)((posx>>6)-x))]=255;
//Bal();
Draw(Page);
}
//FinTime();
ModeTxt();
//printf("\nTemps : %.3f sec.",temps);           //Affichage des performances
printf("\nImages : %d images",images);
printf("\nImages/Sec. : ");
textcolor(9);
//cprintf("%.2f Images/Sec.",images/temps);
printf("\n");
printf("A:%d  B:%d  C:%d",A,B,C);
printf("free mem :%lu",coreleft());
}





