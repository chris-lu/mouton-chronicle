#define ERROR	{ ModeTxt();	printf("error %X XMS",nb);	getch();	exit(0);	}
#define LEVCFG "-DECORE = %d \n-CIEL = %d\n# 0 0 0 0.000000 0.000000 0.000000 \n-BACK = %d\n#%s\n#%s\n-END"
#define JEUCFG "-JOUEURS = %d \n-PLANS = %d \n-POINTS_2_VIE = %d \n-CURNIV = %d \n-END"
#include <stdio.h>
#include <conio.h>
#include <math.h>
#include <mouse.c>
#include <alloc.h>
#include <time.h>
#include "string.h"
#include "sheep.h"
#include "variable.c"
#include "Rtime.c"
#include "vga_c.c"
#include "vga.c"
#include "palette.c"
#include "plans.c"
#include "mout.c"
#include "clavier.c"
#include "graph.c"
#include "option.c"


//**********************************************************************************************
//Initalisation de la memoire : Allocation
//**********************************************************************************************

void Init_Mem(void)
{
register cont;
//Allocation mémoire pour le premier plan : 4 Pages de 64 Ko
for(cont=0;cont<Options.Nb_Plans;cont++)
	if((Plan_1[cont]=(unsigned char far *)farmalloc(64000))==NULL)
	{
	printf("\npas assez de memoire conv.!");
	exit(1);
	}

//Allocation mémoire pour le second plan : 2 Pages de 64 Ko
for(cont=0;cont<2;cont++)
	if((Plan_2[cont]=(unsigned char far *)farmalloc(64000))==NULL)
	{
	printf("\npas assez de memoire conv.!");
	exit(1);
	}
//Allocation mémoire pour la page de travail: 1 Page de 64 Ko
if((Page=(unsigned char far *)farmalloc(64000))==NULL)
	{
	printf("\npas assez de memoire conv.!");
	exit(1);
	}
//Allocation mémoire pour la page de travail: 1 Page de 64 Ko
	if((Mouton=farcalloc(Options.Nb_Joueurs,136))==NULL)
	{
	printf("\npas assez de memoire conv.!");
	exit(1);
	}
for(cont=0;cont<50;cont++)
	if((Mout_Spr[cont]=(unsigned char far *)farmalloc(64))==NULL)
	{
	printf("\npas assez de memoire conv.!");
	exit(1);
	}
//Allocation mémoire pour les pallettes: 3 tableaux de 768 octets chacun
for(cont=0;cont<3;cont++)
Pal[cont]=(unsigned char far *)farmalloc(768);

Bonhomme=(unsigned char far *)farmalloc(74*83);
Sapin=(unsigned char far *)farmalloc(160*58);
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
































//**********************************************************************************************
//													PROGRAME PRINCIPALE
//**********************************************************************************************
void main(void)
{
unsigned long images=0;                   //nombre d'images enregistrés
unsigned short cont;                      //compteur
clrscr();                                 //efface l'écran
Lire_Jeu_Cfg();
Lire_Level_Cfg(Niveau.Cur_Level);
//Init_XMS();
//Init_Pages();
printf("Mouton Chronicles Version 0.05 Alpha\n\n    *Si ce programme fait planter windows(ce qui ne chagera pas vos habitudes)\n     ou redemarrer votre pc : vous n'avez pas assez de memoire convetionnelle\n     (Mais normallement, Y'a pas de problemes)\n    *Si l'ecran est splite en 2 ou 4, les drivers de la souris ne sont pas \n     installes ou pas reconnus!\n\n\t\t\t\t\t\t\t");
getch();                                  //Lit une touche
setvect(0x1c,Vide);                       //enleve l'effet de clignottement
Init_Mem();                               //Initalise la memoire
Init_Tab();                               //Initlise le tableau
Init_Graph();
free(Bonhomme);
free(Sapin);
ModeVGA();                                 //Initaialise le mode VGA 320*200*256
//getch();
Pal[0][765]=63;
Pal[0][766]=63;
Pal[0][767]=63;
Mouton[0].DirY=0;    //Attraction terrestre 9.81/2
Mouton[1].DirY=0;    //Attraction terrestre 9.81/2

SetAllPala(Pal[0]);                        //active la pallette...
CpyPal(Pal[0],Pal[1]);                     //puis la duplique
InitMouse();                               //initialise la souris...
ZoneMouse(0,50,TX-320,200);                   //et definie sa zone de déplacement
for(cont=0;cont<Options.Nb_Joueurs;cont++)
	{
	Put_Mouton(20+((TX-40)/Options.Nb_Joueurs)*cont,cont);
	Mouton[cont].Mort=0;
	Mouton[cont].Pts_Vie=Options.Pts_2_Vie;
	}
Install_Clav();                            //modifie l'interruption clavier
DebTime();                                 //début du chronometre
Survivants=Options.Nb_Joueurs;
while(!Scan_Code[1])                       //tant que la touche [Esc] n'est pas enfoncée....
{
while(Mouton[Cur_Joueur].Mort&&Survivants>1)
{
Cur_Joueur++;
Cur_Joueur%=Options.Nb_Joueurs;
}
	if(!(Palrot&7))                            //rotation de pallette toutes les 8 images
	{
	PalNb=!PalNb;                           //change la pallette active   (
	Rot_Pal(1,15,Pal[PalNb],Pal[!PalNb]);  //Rotation des couleurs
	SetAllPala(Pal[PalNb]);                 //activation de la nouvelle pallette
	}
images++;                                  //incrémente le nb d'images
Palrot++;                                  //    "      le conteur pour la pallette
GetMouse(&x,&y);                           //lit les coordonnés de la souris
Clr(Page);                                 //efface la page de travail
if(Options.Ciel)
Draw_Deg(Page,208);
else
Draw_Back(Page);                           //affiche les étoiles

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
	Wait=200;
	Test_Dom();
	Init_Explose(Bombe.PosX,Bombe.PosY,CurBomb,25);//creer l'explosion
	Cur_Joueur++;
	Cur_Joueur%=Options.Nb_Joueurs;
	CurBomb++;                               //change le numero de la bombe
	CurBomb&=3;
	}
	else                               //sinon
	if((Bombe.PosX>x)&&(Bombe.PosX<(x+320))&&(Bombe.PosY>200-y)&&(Bombe.PosY<400-y))  //si la bombe est dans l'ecran,...
		Page[(Y[400-y-Bombe.PosY]+(Bombe.PosX-x))]=255;                                //la dessiner
}
}
else Wait--;
for(cont=0;cont<4;cont++)  //calcul des explosion des bombes(3bombe max sur l'écrans
	{
	if(Explose[cont].Used)                   //Bombe utilisée?
		Bomb_Ex(cont);                        //coninuer l'explosion
	else if(LastExB[cont])
		End_Bomb(cont);                       //sinon memorise les eclats dans le premier plan
	}

for(cont=0;cont<Options.Nb_Joueurs;cont++)
{
if(!Mouton[cont].Mort)
{
Move_Mouton(cont);
Draw_Mouton(cont);
}
}
CpyPlan(x,y,Plan_1,Page);                   //Copie le premier plan  //les plan doivent etre déssineé dans l'ordre décoissant pour une superposition
Print_Barre(5,5,100,10,Puissance/6.5*100,88,0,1);
if(Vent>0)
{
Print_Barre(210,7,50,6,0,66,0,0);
Print_Barre(265,7,50,6,Vent/0.02*100,99,0,1);
}
else
{
Print_Barre(210,7,50,6,-Vent/0.02*100,99,0,0);
Print_Barre(265,7,50,6,0,66,0,1);
}

Put_Viseur(Cur_Joueur);
Bal();                                    //attente du Balayage vertical
Draw(Page);                                 //Affiche la page de travail finale
for(cont=0;cont<Options.Nb_Joueurs;cont++)
if((Mouton[cont].PosX<0)||(Mouton[cont].PosX>TX)||(Mouton[cont].PosY<0))
	{
	Mouton[cont].Mort=1;
	Survivants--;
	}
if((Survivants<2)&&!Wait)
	Scan_Code[1]=1;
}
Cur_Joueur=Gagnant();
FinTime();                                  //arrete le chronometre
Remove_Clav();
ModeTxt();
printf("Variables Terrain : A:%d  B:%d  C:%d  D:%d",A,B,C,D);
printf("\nMemoire Libre :%lu octets\n",coreleft());
printf("\n\nTemps : %.3f sec.",temps);         //Affichage des performances
printf("\nImages : %lu images",images);
printf("\nImages/Sec. : ");
textcolor(9);
cprintf("%.2f Images/Sec.",images/temps);
if(Cur_Joueur>-1)
	printf("\nLe gagnant est le joueur %d avec %d points de vie!",Cur_Joueur+1,Mouton[Cur_Joueur].Pts_Vie);
  else printf("\nIl n'y a pas de gangant!!!!");

}





