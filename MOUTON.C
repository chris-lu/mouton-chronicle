#define ERROR	{ ModeTxt();	printf("error %X XMS",nb);	getch();	exit(0);	}
#define LEVCFG "-DECORE = %d \n-CIEL = %d\n# 0 0 0 0.000000 0.000000 0.000000 \n-BACK = %d\n#%s\n#%s\n-END"
#define JEUCFG "-RIEN = \n-JOUEURS = %d \n-PLANS = %d \n-POINTS_2_VIE = %d \n-CURNIV = %d \n-END"
#include <stdio.h>
#include <conio.h>
#include <math.h>
#include <mouse.c>
#include <alloc.h>
#include <time.h>
#include "string.h"
#include "sheep.h"
#include "variable.c"
//#include "Rtime.c"
#include "vga_c.c"
#include "vga.c"
#include "texte.c"
#include "textemen.c"
#include "palette.c"
#include "plans.c"
#include "moutgest.c"
#include "clavier.c"
#include "graph.c"
#include "option.c"


//**********************************************************************************************
//Initalisation de la memoire : Allocation
//**********************************************************************************************
void Put_Foy(void)
{
register cont,i,j;
unsigned short Co;
for(cont=0;cont<320;cont++)
  {
  Page[63360+cont]=(Rnd4[temp++]>>3)+(Rnd2[temp]>>3)+15;
  }
for(cont=0;cont<3;cont++)
  {
  Co=63040+Rnd1[temp++]+30;
  for(i=0;i<10;i++)
	  {
	  Page[Co+i]=55;
	  Page[Co+320+i]=55;
	  }
  }
for(cont=0;cont<3;cont++)
  {
  Co=63040+Rnd2[temp++]+30;
  for(i=0;i<10;i++)
	  {
	  Page[Co+i]=60;
	  Page[Co+320+i]=60;
	  }
  }
for(cont=0;cont<5;cont++)
  {
  Co=63040+Rnd3[temp++]/2+96;
  for(i=0;i<10;i++)
	  {
	  Page[Co+i]=64;
	  Page[Co+320+i]=64;
	  }
  }
/*for(cont=0;cont<4;cont++)
  {
  Co=62400+Rnd[temp++]+30;
  for(j=0;j<4;j++)
	for(i=0;i<3;i++)
	  Page[Co+j*320+i]=40;
  }  */
}
char On_Obj(void)
{
register cont;
for(cont=0;cont<20;cont++)
	{
	if((x>Obj[cont].X)&&(x<Obj[cont].X+Obj[cont].Lg)&&(y>Obj[cont].Y)&&(y<Obj[cont].Y+Obj[cont].Ht))
		return(cont);
	}
return(-1);
}

void Init_Long(void)
{
register cont;
for(cont=0;cont<5;cont++)
	{
	Obj[cont].X=145;
	Obj[cont].Y=25*cont+32;
	Obj[cont].Lg=54;
	Obj[cont].Ht=14;
	}
}

void Drawy(bytef *src)
{
asm{
  push ds
  les di,Video
  lds si,src
  mov cx,15840
db 66h
  rep movsw
  pop ds
}
}


void Mem_Error(void)
{
printf("\npas assez de memoire conv.!");
exit(1);
}

void Init_Mem2(void)
{
register cont;
for(cont=0;cont<2;cont++)
	if((Plan_2[cont]=(unsigned char far *)farmalloc(64000))==NULL)
		Mem_Error();
}

void Init_Mem1(void)
{
register cont;
for(cont=0;cont<Options.Nb_Plans;cont++)
	if((Plan_1[cont]=(unsigned char far *)farmalloc(64000))==NULL)
		Mem_Error();

//Allocation mémoire pour les pallettes: 3 tableaux de 768 octets chacun
}




void Init_Mem(void)
{
register cont;
//Allocation mémoire pour le premier plan : 4 Pages de 64 Ko
//Allocation mémoire pour le second plan : 2 Pages de 64 Ko
//Allocation mémoire pour la page de travail: 1 Page de 64 Ko
if((Page=(unsigned char far *)farmalloc(64000))==NULL)
	Mem_Error();
for(cont=0;cont<96;cont++)
	if((Lettre[cont]=(unsigned char far *)farmalloc(144))==NULL)
		Mem_Error();
//Allocation mémoire pour la page de travail: 1 Page de 64 Ko
printf("\nMemoire Libre :%lu octets\n\n",coreleft());
/*	if((Mouton=farcalloc(Options.Nb_J_Tot,140))==NULL)
	Mem_Error();*/
for(cont=0;cont<50;cont++)
	if((Mout_Spr[cont]=(unsigned char far *)farmalloc(64))==NULL)
		Mem_Error();
for(cont=0;cont<3;cont++)
	Pal[cont]=(unsigned char far *)farmalloc(768);

}




void Init_Bomb(void)
{
Bombe.Used=1;
Bombe.Explosion=0;
Bombe.DirX=CosT[Vise]*Puissance*Mouton[Cur_Joueur].Sens;
Bombe.DirY=SinT[Vise]*Puissance;
Bombe.PosY=Mouton[Cur_Joueur].PosY+2;
Bombe.PosX=Mouton[Cur_Joueur].PosX+4;
}


//**********************************************************************************************
//Initalisation du tableau des position des Y
//**********************************************************************************************
void Init_Tab(void)
{
register short cont;
for (cont=0;cont<201;cont++)
	Y[cont]=cont*320;
for(cont=0;cont<256;cont++)
	SinT[cont]=sin(0.012271863*cont-1.57079);
for(cont=0;cont<256;cont++)
	CosT[cont]=cos(0.012271863*cont-1.57079);

	}


//**********************************************************************************************
//Créer un trou sur les pages
//**********************************************************************************************



unsigned char Test_Line(void)
{
float temp;
float lng;
float cont;
if(Bombe.DirX>0)
	{
	temp=(float)Bombe.DirY/Bombe.DirX;
	for(cont=0;cont<Bombe.DirX;cont++)
		{
		if(Plan_1[(Bombe.PosX+cont)/320][Y[200-(Bombe.PosY+cont*temp)]+(int)(Bombe.PosX+cont)%320])
			{
			Bombe.PosX+=cont;
			Bombe.PosY+=temp*cont;
			return(1);
			}
		}
	}
else if(Bombe.DirX<0)
	{
	temp=-(float)Bombe.DirY/Bombe.DirX;
	for(cont=0;cont<-Bombe.DirX;cont++)
		{
		if(Plan_1[(Bombe.PosX-cont)/320][Y[200-(Bombe.PosY+cont*temp)]+(int)(Bombe.PosX-cont)%320])
			{
			Bombe.PosX-=cont;
			Bombe.PosY+=temp*cont;
			return(1);
			}
		}
	}
Bombe.PosY+=Bombe.DirY;
Bombe.PosX+=Bombe.DirX;
return(0);
}




//**********************************************************************************************
//Déplacement de la bombe et test par rapport au sol
//**********************************************************************************************

void Move_Bomb(void)
{
register cont;

if(!Bombe.Explosion)              //la bombe n'explose pas?
	{
	Bombe.DirY-=0.049;                 //Attraction terrestre (9.81 /2)
	Bombe.DirX+=Vent;
	}
if ((Bombe.PosY>0)&&(Bombe.PosY<200)&&(Bombe.PosX>0)&&(Bombe.PosX<TX))    //si la bombe est dans le terrain
	{
//if(Plan_1[Bombe.PosX/320][Y[200-(Bombe.PosY-Bombe.DirY/2)]+((int)(Bombe.PosX-Bombe.DirX/2)%320)])    //si la bombe touche le sol
	if(Test_Line())    //si la bombe touche le sol
		{
		Vent=(float)(random(100)-50)/2500;
		Bombe.Explosion=1;                                       //explosion
		Bombe.Used=0;
		Put_trou(Bombe.PosX-12,200-Bombe.PosY-12,24,24,Trou);    //sprite sur 3 pages
		}
	}
//else if ((Bombe.PosX<0)||(Bombe.PosX>TX)||(Bombe.PosY<0))  //si la bombe sort...
else if (Bombe.PosY<200)  //si la bombe sort...
	{
	Vent=(float)(random(100)-50)/2500;
	Bombe.Used=0;                                             //...en creer une nouvelle
	}
else
	{
	Bombe.PosX+=Bombe.DirX;
	Bombe.PosY+=Bombe.DirY;
	}
}

/*void Init_XMS(void)
{
unsigned int version,revision,hma;
unsigned int maxblksize;
unsigned long memoire;
if(xms_installed())
	{
	printf("Memoire XMS installee");
	xms_init();
	xms_version(&version,&revision,&hma);
	xms_mem_info(&maxblksize,&memoire);
	printf("\nVersion : %.2f, Revision : %.2f, HMA %d",(float)version/100,(float)revision/100,hma);
	printf("\nMemoire libre : %d Mo",memoire/1024);
	/*if (memoire<2048)
		{
		clrscr();
		printf("Pas assez de memoire XMS");
		getch();
		exit(1);
		}
	printf("\n\n");
	}
 else
	{
	printf("\nMemoire XMS non disponible. . .");
	}
}

	  */
/*void Init_Pages(void)
{
register cont;
byte nb;
for(cont=0;cont<2;cont++)
{
if((nb=xms_allocate(&XMS_Page[cont],64))!=0)
	ERROR;
}
}


				*/
















void Draw_Paroles(void)
{
char Txt[255];
register cont,cont2;
while((!Par[0][2])&&(Par[1][2]+Par[2][2]+Par[3][2]))
	Txt_Down();
for(cont=0;cont<4;cont++)
	{
	if(Par[cont][2])
		{
		Par[cont][2]--;
		sprintf(Txt,"%s : %s",Mouton[Par[cont][0]].Nom,Paroles[Par[cont][1]]);
		Print_Txt(120,cont*7+5,Txt,Page,48+Mouton[Par[cont][0]].Equipe);
		}
	}
}












void Draw_Scores (void)
{
register cont;
if(Deb_Tour)
	{
	for(cont=0;cont<Options.Nb_J_Tot;cont++)
		if(!Mouton[cont].Mort)
		{
		if((Mouton[cont].PosX>x)&&(Mouton[cont].PosX<(x+305))&&(Mouton[cont].PosY>200-y)&&(Mouton[cont].PosY<385-y))  //si dans l'écran...
			{
			if(Cur_Joueur!=cont)
				{
				Print_Txt(Mouton[cont].PosX-x,400-Mouton[cont].PosY-y-18,Mouton[cont].Nom,Page,48+Mouton[cont].Equipe);
				Print_Nb(Mouton[cont].PosX-x,400-Mouton[cont].PosY-y-10,Mouton[cont].Pts_Vie,Page,48+Mouton[cont].Equipe);
				}
			else
				{
				Print_Txt(Mouton[cont].PosX-x,400-Mouton[cont].PosY-y-18,Mouton[cont].Nom,Page,1);
				Print_Nb(Mouton[cont].PosX-x,400-Mouton[cont].PosY-y-10,Mouton[cont].Pts_Vie,Page,1);
				}
			}
		}
	}
}







void jouer(void)
{
register cont;
Lire_Level_Cfg(Niveau.Cur_Level);
Init_Mem1();                              //Initalise la memoire en 2 temps...
Load_Ter();
Init_Mem2();
Init_Graph();
ModeVGA();                                 //Initaialise le mode VGA 320*200*256
//getch();
Pal[0][765]=63;
Pal[0][766]=63;
Pal[0][767]=63;
Mouton[0].DirY=0;    //Attraction terrestre 9.81/2
Mouton[1].DirY=0;    //Attraction terrestre 9.81/2
Load_Txt();
SetAllPala(Pal[0]);                        //active la pallette...
CpyPal(Pal[0],Pal[1]);                     //puis la duplique
InitMouse();                               //initialise la souris...
ZoneMouse(0,50,TX-320,200);                   //et definie sa zone de déplacement

for(cont=0;cont<Options.Nb_J_Tot;cont++)
	{
	Put_Mouton(20+((TX-40)/Options.Nb_J_Tot)*cont,cont);
	Mouton[cont].Mort=0;
	Mouton[cont].Pts_Vie=Options.Pts_2_Vie;
// Mouton[cont].Equipe=cont%Options.Nb_Equipes;
	}

Install_Clav();                            //modifie l'interruption clavier
//DebTime();                                 //début du chronometre
Survivants=Options.Nb_J_Tot;
Stats.Bourrin=64000;
while(!Scan_Code[1])                       //tant que la touche [Esc] n'est pas enfoncée....
{
	if(!(Palrot&7))                            //rotation de pallette toutes les 8 images
		{
		PalNb=!PalNb;                           //change la pallette active   (
		Rot_Pal(1,15,Pal[PalNb],Pal[!PalNb]);  //Rotation des couleurs
		SetAllPala(Pal[PalNb]);                 //activation de la nouvelle pallette
		}
	//images++;                                  //incrémente le nb d'images
	Palrot++;                                  //    "      le conteur pour la pallette
	GetMouse(&x,&y);                           //lit les coordonnés de la souris
	if(Bombe.Used)
		Look_Obj(Bombe.PosX,Bombe.PosY,&x,&y);
	Clr(Page);                                 //efface la page de travail
	if(Options.Ciel)
		Draw_Deg(Page,216);
	else
		Draw_Back(Page);                           //affiche les étoiles
	if(Mouton[Cur_Joueur].Mort&&!Wait)
		Wait=100;
	if(Wait==1)
		{
		Deb_Tour=1;
		Cur_Joueur++;
		Cur_Joueur%=Options.Nb_J_Tot;
		Wait--;
		cont=255;
		while(Mouton[Cur_Joueur].Mort&&cont)
			{
			Cur_Joueur++;
			Cur_Joueur%=Options.Nb_J_Tot;
			cont--;
			}
		 /*	x=Mouton[Cur_Joueur].PosX-160;
			y=200-(Mouton[Cur_Joueur].PosY-100);
			SetMouse(x,y);
			GetMouse(&x,&y);                           //lit les coordonnés de la souris   */
		}
	CpyPlan(x>>2,(y>>2)+150,Plan_2,Page);      //affiche le second plan
	if(!Wait)
		{
		Lire_Keys(Cur_Joueur);                               //Lits les touche
		if(Bombe.Used)
			{
			//Mouton[Cur_Joueur].Can_Move=0;
			Move_Bomb();                               //deplacement de la bombe et test
			if(Bombe.Explosion)                   //la bombe explose?
				{
				Wait=175;
				Test_Dom();
				SetMouse(x,y);
				Init_Explose(Bombe.PosX,Bombe.PosY,CurBomb,17);//creer l'explosion
				CurBomb++;                               //change le numero de la bombe
				CurBomb&=3;
				}
			else                               //sinon
				if((Bombe.PosX>x)&&(Bombe.PosX<(x+320))&&(Bombe.PosY>200-y)&&(Bombe.PosY<400-y))  //si la bombe est dans l'ecran,...
			Page[(Y[400-y-Bombe.PosY]+(Bombe.PosX-x))]=255;                                //la dessiner
			}
		Put_Viseur(Cur_Joueur);
		}
	else Wait--;

	for(cont=0;cont<4;cont++)  //calcul des explosion des bombes(3bombe max sur l'écrans
		{
		if(Explose[cont].Used)                   //Bombe utilisée?
			Bomb_Ex(cont);                        //coninuer l'explosion
		else if(LastExB[cont])
			End_Bomb(cont);                       //sinon memorise les eclats dans le premier plan
		}
		for(cont=0;cont<Options.Nb_J_Tot;cont++)
		{
		if((Mouton[cont].PosX<0)||(Mouton[cont].PosX>TX)||(Mouton[cont].PosY<0))
				Mouton[cont].Mort=1;
		else if(!Mouton[cont].Mort)
			{
			Move_Mouton(cont);
			Draw_Mouton(cont);
			}
		}
	if(Deb_Tour)
		Draw_Scores();

	CpyPlan(x,y,Plan_1,Page);                   //Copie le premier plan  //les plan doivent etre déssineé dans l'ordre décoissant pour une superposition
	Print_Barre(5,5,110,10,Puissance/6.5*100,88,0,1);
	if(Vent>0)
		{
		Print_Barre(5,17,53,6,0,66,0,0);
		Print_Barre(62,17,53,6,Vent/0.02*100,99,0,1);
		}
	else
		{
		Print_Barre(5,17,53,6,-Vent/0.02*100,99,0,0);
		Print_Barre(62,17,53,6,0,66,0,1);
		}
	//Print_Txt(2,192,"Mouton Chronicles",Page,1);
	Draw_Paroles();
	Bal();                                    //attente du Balayage vertical
	Draw(Page);                                 //Affiche la page de travail finale
	if(Gagnant()&&!Wait)
		Scan_Code[1]=1;
	}

Cur_Joueur=Gagnant();
//FinTime();                                  //arrete le chronometre
Remove_Clav();
ModeTxt();
printf("Variables Terrain : A:%d  B:%d  C:%d  D:%d",A,B,C,D);
printf("\nMemoire Libre :%lu octets\n\n",coreleft());
/*printf("\n\nTemps : %.3f sec.",temps);         //Affichage des performances
printf("\nImages : %lu images",images);
printf("\nImages/Sec. : ");
textcolor(9);
cprintf("%.2f Images/Sec.",images/temps);     */
if(Cur_Joueur>0)
	{
	printf("\nL'Equipe %s est vainqueur!!",Eq_Name[Cur_Joueur-1]);
	Mk_Stats();
	printf("\n\nLe meilleur mouton : %s (%d Frags)\nLe plus precis : %s (%d Frags)\nLe plus violant : %s (%d Frags)\nLe plus...INUTILE : %s ( %d Frags)",Mouton[Stats.Mouton[0]].Nom,Mouton[Stats.Mouton[0]].Frags,Mouton[Stats.Mouton[1]].Nom,Mouton[Stats.Mouton[1]].Frags,Mouton[Stats.Mouton[2]].Nom,Mouton[Stats.Mouton[2]].Frags,Mouton[Stats.Mouton[3]].Nom,Mouton[Stats.Mouton[3]].Frags);
	}
else printf("\nIl n'y a pas de gagnant!!!!");
for(cont=0;cont<Options.Nb_Plans;cont++)
  farfree(Plan_1[cont]);
for(cont=0;cont<2;cont++)
  farfree(Plan_2[cont]);
Scan_Code[1]=0;
}


void Clry(bytef *dst)
{
asm{
  push ds
  les di,dst
  xor ax,ax
  mov cx,25600
  rep stosw
  pop ds
}
}


void FlamesAsm2(bytef *src,bytef* dst)   //brule l'ecran(assembleur)
{
asm{
	push ds                              //sauvegarde ds
	les di,dst                           //|mémorise les adresses des pages
	lds si,src                           //|
	mov ax,50880
	add di,ax
	add ax,320
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
	mov bl,[si+320]                        //point de gauche
	add ax,bx
	mov bl,[si-320]                        //point de gauche
	add ax,bx
	mov bh,5
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





//**********************************************************************************************
//													PROGRAME PRINCIPALE
//**********************************************************************************************
void Load_Graph_Menu(void)
{
Clr(Page);
Load_Txt_Menu();
ModeVGA();
Init_Pal(Pal[2]);
SetAllPala(Pal[2]);
InitMouse();
MouseOFF();
ZoneMouse(0,0,320,198);
}


void main(void)
{
//unsigned long images=0;                   //nombre d'images enregistrés
unsigned short cont;                      //compteur
unsigned short b;
char CurObj,fin=0;
clrscr();                                 //efface l'écran
randomize();
for(cont=0;cont<256;cont++)
	{
	Rnd1[cont]=random(255);
	Rnd2[cont]=random(255);
	Rnd3[cont]=random(255);
	Rnd4[cont]=random(255);
}
Init_Tab();                               //Initlise le tableau
Init_Mem();                               //Initalise la memoire(2eme partie...)
Lire_Jeu_Cfg();
Mouton=farcalloc(Options.Nb_J_Tot,170);
Lire_Joueur_Cfg();

//Init_XMS();
//Init_Pages();
printf("Mouton Chronicles Version 0.08 Alpha Par :\n\tBaudin A. (Bart 57)\n\tJolivalt J.   (dJo)\n\tDri C.  (GnoMaster)\n\n\n    *Si ce programme fait planter windows(ce qui ne chagera pas vos habitudes)\n     ou redemarrer votre pc : vous n'avez pas assez de memoire convetionnelle\n     (Mais normallement, Y'a pas de problemes)\n    *Si l'ecran est splite en 2 ou 4, les drivers de la souris ne sont pas \n     installes ou pas reconnus!\n\n\t\t\t\t\t\t\t");
getch();                                  //Lit une touche
Load_Graph_Menu();
//setvect(0x1c,Vide);                       //enleve l'effet de clignottement
/*ModeVGA();
Init_Pal(Pal[2]);
SetAllPala(Pal[2]);*/
Init_Long();
while(!fin)
{
Clry(Page);
Put_Foy();
b=GetMouse(&x,&y);
CurObj=On_Obj();
for(cont=0;cont<5;cont++)
	if(cont==CurObj)
		{
		Print_Txt_Menu(Obj[cont].X,Obj[cont].Y,Txt[cont],Page,63);
		}
	else
		{
		Print_Txt_Menu(Obj[cont].X,Obj[cont].Y,Txt[cont],Page,15);
		}
if((CurObj!=-1)&&b)
	{
	if((CurObj==0))
		{
		jouer();
		Load_Graph_Menu();
      }
	if((CurObj==4))
	fin=1;
}
Page[y*320+x]=64;
FlamesAsm2(Page,Page);
Bal();
Drawy(Page);
}
ModeTxt();
}

