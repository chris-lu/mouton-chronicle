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
#include "clavier.c"

//couleurs 2eme plan : 96

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
unsigned short Ytemp[960];
register long cont;
unsigned short haut;
unsigned short temp;
unsigned char pos=100;

randomize();
A=random(2)+1;
B=random(12)+10;
C=random(13)+9;

for (cont=0;cont<960;cont++)
{
Ytemp[cont]=(random(5)-2)+((sin(cont*0.001*A+1))*cos((cont)*0.0009*B+1)+cos((cont)*C*0.002)*0.1);
if((pos+Ytemp[cont]>25)&&(pos+Ytemp[cont]<190))
pos+=Ytemp[cont];

temp=Y[200-pos]+(cont%320);
Plan_1[cont/320][temp]=31;
Plan_1[cont/320][temp+320]=30;
Plan_1[cont/320][temp+640]=29;
Plan_1[cont/320][temp+960]=28;
Plan_1[cont/320][temp+1280]=29;
Plan_1[cont/320][temp+1600]=30;
for(haut=pos-6;haut>0;haut--)
  Plan_1[cont/320][Y[200-haut]+(cont%320)]=31;
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
dirx=(float)(random(80)-40)/20;
diry=(float)(random(50)+10)/10;
posy=200;
posx=random(320)+320;
}

void Init_Tab(void)
{
register cont;
for (cont=0;cont<200;cont++)
	Y[cont]=cont*320;
}


void Put_trou(unsigned short x,short y,word l,word h,bytef *buf)
{
  register byte j;
  register word i;
  for(j=0;j<h;j++)
  {
		for(i=0;i<l;i++)
	 {
		if(*buf==1)
			if(x+i<960)              //inclue x<0  et x>960 car non signé!
			Plan_1[(x+i)/320][Y[y]+x+i]=0; //Transparence … l'encre 0
		buf++;
	 }
	 y++;
  }
}


void Move_Bomb(void)
{
if(!explose)
{
diry-=0.04;
posy+=diry;
posx+=dirx;
}
if ((posy>0)&&(posy<200)&&(posx>0)&&(posx<960))
{
if((Plan_1[posx/320][Y[200-posy]+posx])||posy<1)
	{
	explose=1;
	Put_trou(posx-14,200-posy-14,24,24,Trou);    //spr sur 3 pages
	}
}
else if ((posx<0)||(posx>960)||(posy<0))
	{
	Init_Bomb();
	}
}

void main(void)
{
unsigned long images=0;
unsigned short cont;
printf("Si ce progrmme fait planter windows(ce qui ne chagera pas vos habitudes)\nou redemarrer votre pc : vous n'avez pas assez de mememoire convetionnelle!\n(Mais normallement, Y'a pas de probèmes!)");
getch();
Init_Mem();
Init_Tab();
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

SetAllPala(Pal[0]);
CpyPal(Pal[0],Pal[1]);
InitMouse();
ZoneMouse(0,50,639,200);
Install_Clav();
DebTime();
//Init_Explose(200,180,0);
while(!Scan_Code[1])
{
Lire_Keys();
if(!(palrot%8))
	{
	tremble=!tremble;
	Rot_Pal(32,32,Pal[tremble],Pal[!tremble]);
	SetAllPala(Pal[tremble]);
	}
images++;
palrot++;
GetMouse(&x,&y);
Clr(Page);
Draw_Back(Page);
CpyPlan(x>>1,(y>>1)+100,Plan_2,Page);
Move_Bomb();                   //PB!!!
if(explose)
	{
	//SetMouse(x+random(10)-5,y+random(10)-5);
	//tremble=0;
	Init_Explose(posx,posy,CurBomb);
	CurBomb++;
	CurBomb%=3;
	Init_Bomb();
	}
	else
if((posx>x)&&(posx<(x+320))&&(posy>200-y)&&(posy<400-y))
	{
	Page[(Y[400-y-posy]+(posx-x))]=255;
	}
for(cont=0;cont<3;cont++)
	{
	if(Explose[cont].Used)
		Bomb_Ex(cont);
	else if(LastExB[cont])
		End_Bomb(cont);
	}
/*if(tremble)
	{
	tremble--;
	move=-move;
	SetMouse(x-(tremble/move),y+(tremble/move));
	}
 */
CpyPlan(x,y,Plan_1,Page);
Bal();
Draw(Page);
}
FinTime();
Remove_Clav();
ModeTxt();
printf("\nTemps : %.3f sec.",temps);           //Affichage des performances
printf("\nImages : %d images",images);
printf("\nImages/Sec. : ");
textcolor(9);
cprintf("%.2f Images/Sec.",images/temps);
printf("\n");
printf("A:%d  B:%d  C:%d",A,B,C);
printf("\nfree mem :%lu",coreleft());
}





