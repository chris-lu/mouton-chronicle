#include "mouton.h"
#include "cd.c"
#include "start.c"
//#include "Rtime.c"
#include "graph.c"
#include "option.c"
#include "texte.c"
#include "textemen.c"
#include "palette.c"
#include "mouse.c"
#include "plans.c"
#include "moutgest.c"
#include "clavier.c"



//**********************************************************************************************
//Initalisation de la memoire : Allocation
//**********************************************************************************************
void Mem_Error(void)
{
//printf("\npas assez de memoire conv.!");
exit(1);
}

void Init_Mem(void)
{
register cont;
//Allocation mémoire pour la page de travail: 1 Page de 64 Ko
if((Page=(unsigned char far *)farmalloc(64000))==NULL)
	Mem_Error();
for(cont=0;cont<96;cont++)
	if((Lettre[cont]=(unsigned char far *)farmalloc(144))==NULL)
		Mem_Error();
for(cont=0;cont<50;cont++)
	if((Mout_Spr[cont]=(unsigned char far *)farmalloc(64))==NULL)
		Mem_Error();
//Allocation mémoire pour les pallettes: 3 tableaux de 768 octets chacun
for(cont=0;cont<3;cont++)
	Pal[cont]=(unsigned char far *)farmalloc(768);

}

void Init_Mem1(void)
{
register cont;
			//Allocation mémoire pour le premier plan : 4 Pages de 64 Ko
for(cont=0;cont<Options.Nb_Plans;cont++)
	if((Plan_1[cont]=(unsigned char far *)farmalloc(64000))==NULL)
		Mem_Error();

}

void Init_Mem2(void)
{
register cont;
			//Allocation mémoire pour le second plan : 2 Pages de 64 Ko
for(cont=0;cont<2;cont++)
	if((Plan_2[cont]=(unsigned char far *)farmalloc(64000))==NULL)
		Mem_Error();
}








void Init_Bomb(void)
{
Bombe.Used=1;
Bombe.Explosion=0;
Bombe.DirX=CosT[Vise]*Puissance*Mouton[CURJ].Sens;
Bombe.DirY=SinT[Vise]*Puissance;
Bombe.PosY=Mouton[CURJ].PosY+2;
Bombe.PosX=Mouton[CURJ].PosX+4;
}

/*void Init_Tir(void)
{
Bombe.Used=1;
Bombe.Explosion=0;
Bombe.DirX=CosT[Vise]*Mouton[Cur_Joueur].Sens;
Bombe.DirY=SinT[Vise];
Bombe.PosY=Mouton[Cur_Joueur].PosY+2;
Bombe.PosX=Mouton[Cur_Joueur].PosX+4;
}

  */
//**********************************************************************************************
//Initalisation du tableau des position des Y
//**********************************************************************************************
void Init_Tab(void)
{
register short cont;
for (cont=0;cont<200;cont++)
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
unsigned char cont;
char Signe;

if(Bombe.DirX>0)
	Signe=1;
 else if(Bombe.DirX<0)
	Signe=-1;
  else
	Signe=0;
if(Signe!=0)
	{
	temp=(float)Bombe.DirY/Bombe.DirX*Signe;
	for(cont=0;cont<Bombe.DirX*Signe;cont++)   //Teste pixel par pixel...
		{
		if(Plan_1[(Bombe.PosX+cont*Signe)/320][Y[200-(Bombe.PosY+cont*temp)]+(int)(Bombe.PosX+cont*Signe)%320])
			{
			Bombe.PosX+=cont*Signe;
			Bombe.PosY+=temp*cont;
			return(1);
			}
		}
	}
/*else if(Bombe.DirX<0)
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
	}      */
	else return(1);
Bombe.PosY+=Bombe.DirY;
Bombe.PosX+=Bombe.DirX;
return(0);
}




//**********************************************************************************************
//Déplacement de la bombe et test par rapport au sol
//**********************************************************************************************

/*void ArmBombe(void)
{
register cont;
if(!Bombe.Explosion)              //la bombe n'explose pas?
	{
	Bombe.DirY-=Gravite;                 //Attraction terrestre
	Bombe.DirX+=Vent;                    //Vent
	}
if ((Bombe.PosY>0)&&(Bombe.PosY+abs(Bombe.DirY)<200)&&(Bombe.PosX>0)&&(Bombe.PosX<TX))    //si la bombe est dans le terrain
	{
	if(Test_Line())    //si la bombe touche le sol
		{
		Tremble=5;
		Vent=(float)(random(100)-50)/2500;
		Bombe.Explosion=1;                                       //explosion
		Bombe.Used=0;
		Put_trou(Bombe.PosX-(Trou[0].Taille>>1),200-Bombe.PosY-(Trou[0].Taille>>1),Trou[0].Taille,Trou[0].Taille,Trou[0].Spr);    //sprite sur 3 pages
		Wait=175;
		Test_Dom(25);
		Init_Explose(Bombe.PosX,Bombe.PosY,16);//creer l'explosion
		SetMouse(x,y);
		}
	}
//else if ((Bombe.PosX<0)||(Bombe.PosX>TX)||(Bombe.PosY<0))  //si la bombe sort...
else if (Bombe.PosY+abs(Bombe.DirY)<200)  //si la bombe sort...
	{
	Vent=(float)(random(100)-50)/2500;
	Bombe.Used=0;                                             //...en creer une nouvelle
	Wait=150;
	SetMouse(x,y);
	}
else
	{
	Bombe.PosX+=Bombe.DirX;
	Bombe.PosY+=Bombe.DirY;
	}
if((Bombe.PosX>x)&&(Bombe.PosX<(x+320))&&(Bombe.PosY>200-y)&&(Bombe.PosY<400-y))  //si la bombe est dans l'ecran,...
			Page[(Y[400-y-Bombe.PosY]+(Bombe.PosX-x))]=255;                                //la dessiner
}
		 */
void ArmBombe(void)
{
register cont;
if(!Bombe.Explosion)              //la bombe n'explose pas?
	{
	Bombe.DirY-=Gravite;                 //Attraction terrestre (9.81 /2)
	Bombe.DirX+=Vent;
	}
if ((Bombe.PosY>0)&&(Bombe.PosY+abs(Bombe.DirY)<200)&&(Bombe.PosX>0)&&(Bombe.PosX<TX))    //si la bombe est dans le terrain
	{
	if(Test_Line())    //si la bombe touche le sol
		{
		Tremble=5;
		Vent=(float)(random(100)-50)/2500;
		Put_trou(Bombe.PosX-(Trou[0].Taille>>1),200-Bombe.PosY-(Trou[0].Taille>>1),Trou[0].Taille,Trou[0].Taille,Trou[0].Spr);    //sprite sur 3 pages
		Test_Dom(25);
		Init_Explose(Bombe.PosX,Bombe.PosY,16);//creer l'explosion
		if (Rebonds<6)
			{
			Bombe.DirX=(float)random(300)/100-1.5;
			Bombe.DirY=random(500)/100;
			Rebonds++;
			}
			else
			{
			Bombe.Explosion=1;
			Wait=250;
			Bombe.Used=0;
			Rebonds=0;
			SetMouse(x,y);
			}

		}
	}
else if (Bombe.PosY+abs(Bombe.DirY)<200)  //si la bombe sort...
	{
	Vent=(float)(random(100)-50)/2500;
	Bombe.Used=0;                                             //...en creer une nouvelle
	Wait=150;
	Rebonds=0;
	SetMouse(x,y);
	}
else
	{
	Bombe.PosX+=Bombe.DirX;
	Bombe.PosY+=Bombe.DirY;
	}
if((Bombe.PosX>x)&&(Bombe.PosX<(x+320))&&(Bombe.PosY>200-y)&&(Bombe.PosY<400-y))  //si la bombe est dans l'ecran,...
			Page[(Y[400-y-Bombe.PosY]+(Bombe.PosX-x))]=255;                                //la dessiner
}


void ArmSuicide(void)
{
Mouton[CURJ].Mort=1;
Init_Explose(Mouton[CURJ].PosX,Mouton[CURJ].PosY+4,64);//creer l'explosion
Init_Explose(Mouton[CURJ].PosX,Mouton[CURJ].PosY+4,16);//creer l'explosion
Flash[0]=10;
Put_trou(Bombe.PosX-(Trou[1].Taille>>1),200-Bombe.PosY-(Trou[1].Taille>>1),Trou[1].Taille,Trou[1].Taille,Trou[1].Spr);    //sprite sur 3 pages
Vent=(float)(random(100)-50)/2500;
Wait=175;
Test_Dom(50);
Message(11,0,CURJ);
Bombe.Used=0;
SetMouse(x,y);
}

void ArmSuicide2(void)
{
short TmpX,TmpY;
register cont;
if(Suicide)
	{
	if(Suicide&1)    //Vire au blanc toutes les 2 images
		{
		Goto_Pal(Pal[PalNb],Pal[2]);
		SetAllPala(Pal[PalNb]);
		}
	Bombe.PosX=Mouton[CURJ].PosX;
	Bombe.PosY=Mouton[CURJ].PosY;
	Tremble=(random(175-Suicide))>>4;
	if(Suicide==1)
		{
		Mouton[CURJ].Mort=1;
		for(cont=0;cont<20;cont++)      //explosions dans l'écran
			{
			TmpX=(random(140))-70;
			TmpY=(random(80))-40;
			if((Bombe.PosX+TmpX>0)&&(Bombe.PosX+TmpX<TX)&&(Bombe.PosY+TmpY>0)&&(Bombe.PosY+TmpY<200))
				{
				Bombe.PosX+=TmpX;
				Bombe.PosY+=TmpY;
//				Init_Explose(Bombe.PosX,Bombe.PosY+4,16);//creer l'explosion
				Put_trou(Bombe.PosX-(Trou[1].Taille>>1),200-Bombe.PosY-(Trou[1].Taille>>1),Trou[1].Taille,Trou[1].Taille,Trou[1].Spr);    //sprite sur 3 pages
				Test_Dom(50);
				Bombe.PosX-=TmpX;
				Bombe.PosY-=TmpY;
				}
			else cont--;
			}
			Message(11,0,CURJ);
//			CpyPal(Pal[!PalNb],Pal[PalNb]);
			Init_Explose(Mouton[CURJ].PosX,Mouton[CURJ].PosY+4,64);//creer l'explosion
			Vent=(float)(random(100)-50)/2500;
			Wait=400;
			Bombe.Used=0;
			SetMouse(x,y);
			Chal=350;
			}
	Suicide--;
	}
else
	Suicide=170;
}

void Eclairs(void)
{
register short cont,cont2;
if(Ecl)
	{
	if(Ecl>350)
		{
			if(Ecl>380)
			for(cont2=0;cont2<4;cont2++)
				{
				EclPos=Mouton[CURJ].PosX+3;
				for(cont=Mouton[CURJ].PosY+3;cont<400;cont++)
						{
						EclPos+=(random(3)-1);
						if((EclPos>x)&&(EclPos<(x+320))&&(cont>200-y)&&(cont<400-y))  //si dans l'écran...
								Page[(Y[400-y-cont-1]+(EclPos-x))]=60+random(4);  //...afficher
						}
				}
		Tremble=random(2);
		}
		else if(Ecl<250&&Ecl>200)
		Tremble=random(2);
		else if(Ecl<200)
		{
		EclPos=random(Options.Nb_Plans*320);;
		for(cont=400;cont>0;cont--)
			{
			EclPos+=(random(3)-1);
		if((EclPos>x)&&(EclPos<(x+320))&&(cont>200-y)&&(cont<400-y))  //si dans l'écran...
				Page[(Y[400-y-cont-1]+(EclPos-x))]=60+random(4);  //...afficher
			if(cont<200)
			{
			if(Plan_1[EclPos/320][Y[200-cont]+(EclPos%320)])
				{
				Bombe.PosX=EclPos;
				Bombe.PosY=cont;
				if((Bombe.PosX>0)&&(Bombe.PosX<TX)&&(Bombe.PosY>0)&&(Bombe.PosY<200))
					Test_Dom(16);
				if(!(Ecl&31))
					Flash[0]=2;
				Tremble=random(2);
				break;
	 //	cont=250;
				}
			 }
		}
	}
		if(Ecl==1)
		{
		Wait=200;
		Bombe.Used=0;
		}
	Ecl--;
	}
else Ecl=580;
}

void MediKit(void)
{
Mouton[CURJ].Pts_Vie=Options.Pts_2_Vie-1;
Flash[0]=2;
Wait=200;
Bombe.Used=0;
}


/*void Tirer(void)
{
register cont;
unsigned short TempX,TempY;
if ((TempY>0)&&(TempY<200)&&(TempX>0)&&(TempX<TX))    //si la bombe est dans le terrain
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
  */
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
char Txt[155];
register cont,cont2;
while((!Par[0][2])&&(Par[1][2]+Par[2][2]+Par[3][2]))
	Txt_Down();
for(cont=0;cont<4;cont++)
	{
	if(Par[cont][2])
		{
		Par[cont][2]--;
		sprintf(Txt,"%s : %s",Mouton[Par[cont][0]].Nom,Paroles[Par[cont][1]]);
		Print_Txt(120,cont*7+5,Txt,Page,244+Mouton[Par[cont][0]].Equipe);
		}
	}
}












void Draw_Scores (void)
{
register cont;
if(Deb_Tour)
	{
	for(cont=0;cont<Options.Nb_J_Tot;cont++)
		if(!Mouton[Liste[cont]].Mort)
		{
		if((Mouton[Liste[cont]].PosX>x)&&(Mouton[Liste[cont]].PosX<(x+312))&&(Mouton[Liste[cont]].PosY>200-y)&&(Mouton[Liste[cont]].PosY<376-y))  //si dans l'écran...
			{
			if(Cur_Joueur!=cont)
				{
				Print_Txt(Mouton[Liste[cont]].PosX-x,400-Mouton[Liste[cont]].PosY-y-18,Mouton[Liste[cont]].Nom,Page,244+Mouton[Liste[cont]].Equipe);
				Print_Nb(Mouton[Liste[cont]].PosX-x,400-Mouton[Liste[cont]].PosY-y-10,Mouton[Liste[cont]].Pts_Vie,Page,244+Mouton[Liste[cont]].Equipe);
				}
			else
				{
				Print_Txt(Mouton[Liste[cont]].PosX-x,400-Mouton[Liste[cont]].PosY-y-18,Mouton[Liste[cont]].Nom,Page,1);
				Print_Nb(Mouton[Liste[cont]].PosX-x,400-Mouton[Liste[cont]].PosY-y-10,Mouton[Liste[cont]].Pts_Vie,Page,1);
				}
			}
		}
	}
}







void jouer(void)
{
register cont,cont2;
unsigned short b;
//unsigned long images=0;
float A;
A=M_PI/256;
for (cont=0;cont<256;cont++)
	tab3[cont]=sin(A*cont*4+1)*32;

Clr(Video);
Proc[0]=*ArmBombe;
Proc[1]=*ArmSuicide;
Proc[2]=*ArmBombe;
Proc[3]=*ArmSuicide2;
Proc[4]=*Eclairs;
Proc[5]=*MediKit;

Trou[0].Spr=TrSpr1;
Trou[1].Spr=TrSpr2;
Trou[0].Taille=24;
Trou[1].Taille=46;
for(cont=0;cont<768;cont++)
	Pal[2][cont]=63;

Lire_Level_Cfg(Niveau.Cur_Level);
for(cont=0;cont<Options.Nb_Plans;cont++)
	if((Plan_1[cont]=(unsigned char far*)farmalloc(64000))==NULL)
		Mem_Error();
Load_Ter();
for(cont=0;cont<2;cont++)
	if((Plan_2[cont]=(unsigned char far*)farmalloc(64000))==NULL)
		Mem_Error();
Init_Graph();
Pal[0][765]=63;
Pal[0][766]=63;
Pal[0][767]=63;
Load_Txt();
SetAllPala(Pal[0]);                        //active la pallette...
CpyPal(Pal[0],Pal[1]);                     //puis la duplique
InitMouse();                               //initialise la souris...
ZoneMouse(0,50,TX-320,200);                   //et definie sa zone de déplacement

for(cont=0;cont<Options.Nb_J_Tot;cont++)
	{
	Put_Mouton(20+((TX-40)/Options.Nb_J_Tot)*cont,ListePos[cont]);
	Mouton[Liste[cont]].DirY=0;    //Attraction terrestre 9.81/2
	Mouton[Liste[cont]].DirY=0;    //Attraction terrestre 9.81/2
	Mouton[Liste[cont]].Mort=0;
	Mouton[Liste[cont]].Pts_Vie=Options.Pts_2_Vie;
	}
for(cont=0;cont<12;cont++)
	for(cont2=0;cont2<6;cont2++)
		Stock[cont][cont2]=1;
Install_Clav();                            //modifie l'interruption clavier
//DebTime();                                 //début du chronometre
Stats.Bourrin=64000;
Stats.Precis=0;
//Init_Suic();
Cur_Joueur=random(Options.Nb_J_Tot);
CURJ=Liste[Cur_Joueur];
//Wait=200;
Play(Track);
while(!Scan_Code[1])                       //tant que la touche [Esc] n'est pas enfoncée....
{
if(Flash[0])
	{
	SetAllPala(Pal[2]);
	Flash[0]--;
	if(!Flash[0])
		Flash[1]=1;
	}
else if(Flash[1])
	{
	SetAllPala(Pal[PalNb]);
	Flash[1]=0;
	}

if(!(Palrot&7)&&!Suicide&&!Chal)                            //rotation de pallette toutes les 8 images
	{
	PalNb=!PalNb;                           //change la pallette active   (
	Rot_Pal(1,14,Pal[PalNb],Pal[!PalNb]);  //Rotation des couleurs
	SetAllPala(Pal[PalNb]);                 //activation de la nouvelle pallette
	}

if(Tremble)
	Tremble--;

//	images++;                                  //incrémente le nb d'images
	Palrot++;                                  //    "      le conteur pour la pallette

	Clr(Page);                                 //efface la page de travail
	if(Options.Ciel)
		Draw_Deg(Page,216);
	else
		Draw_Back(Page);                           //affiche les étoiles
	if(Mouton[CURJ].Mort&&!Wait&&!Rebonds&!Ecl)       //Si le joueur meur, AU SUIVANT de jouer!
		Wait=100;
	else if (Wait&&Rebonds&&Ecl)                     //Si le joueurs meur...Qu'il termine sont tour...
		Wait=100;
	if(Wait==1)
		{
		Lire_scan();
		Deb_Tour=1;
		Last_Key=Scan_Code[0];
		Cur_Joueur++;
		Cur_Joueur%=Options.Nb_J_Tot;
		CURJ=Liste[Cur_Joueur];
		Wait--;
		cont=255;
		while(Mouton[CURJ].Mort&&cont)
			{
			Cur_Joueur++;
			Cur_Joueur%=Options.Nb_J_Tot;
			CURJ=Liste[Cur_Joueur];
			cont--;
			}
		 /*	x=Mouton[Cur_Joueur].PosX-160;
			y=200-(Mouton[Cur_Joueur].PosY-100);
			SetMouse(x,y);
			GetMouse(&x,&y);                           //lit les coordonnés de la souris   */
		CURT=Mouton[CURJ].Equipe;
		Arme=0;
		if(Mouton[CURJ].Pts_Vie>5)
			cont2=0;
			else if (Mouton[CURJ].Pts_Vie>1)
			cont2=1;
			else
			cont2=2;
	  for(cont=0;cont<6;cont++)
				Stock[CURT][cont]=Panoplie[cont2][cont];
		}
	if(!Wait)                                  //placé ici pour reduire le tremblement....
		Lire_Keys(CURJ);                               //Lits les touche

	if(Bombe.Used&&!Ecl)                    //placé ici pour eviters les tremblements
		{
		Look_Obj(Bombe.PosX,Bombe.PosY,&x,&y);
		Fallow=0;
		}
	  else
	 {
	 GetMouse(&Lx1,&Ly1);
	 x=Lx1;
	 y=Ly1;
	 if(((Ly1!=Ly2)||(Lx1!=Lx2))&&Fallow&&Options.Fallow)
		{
		Fallow=0;
		Look_Obj(Mouton[CURJ].PosX+4,Mouton[CURJ].PosY+4,&x,&y);
		SetMouse(x,y);
		}
	 else if(Fallow&&Options.Fallow)
		Look_Obj(Mouton[CURJ].PosX+4,Mouton[CURJ].PosY+4,&x,&y);
	 }                           //lit les coordonnés de la souris
	CpyPlan(x>>2,(y>>2)+150,Plan_2,Page);      //affiche le second plan
	if (Bombe.Used)
			(*Proc[Arme])();
	if(!Wait)
		{
		if (Arme==0||Arme==2)
			Put_Viseur(CURJ);
		}
	else
		{
		Wait--;
/*		if (Bombe.Used)       //si attente, continue le tir;
			(*Proc[Arme])();   */
		}
	for(cont=0;cont<Options.Nb_J_Tot;cont++)
		{
		if(((Mouton[Liste[cont]].PosX<0)||(Mouton[Liste[cont]].PosX>TX-8)||(Mouton[Liste[cont]].PosY<0))&&!Mouton[Liste[cont]].Mort)
				{
				Mouton[Liste[cont]].Mort=1;
				Message(13,0,Liste[cont]);
				if(cont!=Cur_Joueur)
					Mouton[Liste[cont]].Frags++;
				}
		else if(!Mouton[Liste[cont]].Mort)
			{
			Draw_Mouton(Liste[cont]);
			Move_Mouton(Liste[cont]);
			}
		}
	if(Deb_Tour)
		Draw_Scores();
//	Suicide();
	CpyPlan(x,y,Plan_1,Page);                   //Copie le premier plan  //les plan doivent etre déssineé dans l'ordre décoissant pour une superposition
	for(cont=0;cont<4;cont++)  //calcul des explosion des bombes(3bombe max sur l'écrans
		{
		if(Explose[cont].Used)                   //Bombe utilisée?
			Bomb_Ex(cont);
		else if(LastExB[cont])
			{
			Bomb_Ex(cont);
			End_Bomb(cont);
			}
		}
										 //sinon memorise les eclats dans le premier plan
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
	Draw_Paroles();
	Print_Txt(2,192,ArmName[Arme],Page,8);
	if(Chal)
		{
		Goto_Pal(Pal[PalNb],Pal[!PalNb]);
		SetAllPala(Pal[PalNb]);                 //activation de la nouvelle pallette
		Chal--;
		nb1=Chal>>4;
		for (cont=1;cont<200;cont++)
		cpyh(cont,(nb1*tab3[(nb2+cont)&255])>>5,Page,Video);                    //2x plus rapide!!!!!!!!!
		nb2++;
		}
	else{
	Bal();                                    //attente du Balayage vertical
	Draw(Page,Tremble*320);
	 }                                //Affiche la page de travail finale
	if(Gagnant()&&!Wait&&!Rebonds&&!Ecl)
		Scan_Code[1]=1;
	}
//Cur_Joueur=Gagnant();
//FinTime();                                  //arrete le chronometre
Remove_Clav();
Mk_Stats();
Winner=Gagnant();
//ModeTxt();
//printf("Variables Terrain : A:%d  B:%d  C:%d  D:%d",A,B,C,D);
/*printf("\nMemoire Libre :%lu octets\n\n",coreleft());
getch();
getch();
getch(); */

/*printf("\n\nTemps : %.3f sec.",temps);         //Affichage des performances
printf("\nImages : %lu images",images);
printf("\nImages/Sec. : ");
textcolor(9);
cprintf("%.2f Images/Sec.",(float)images/temps);
getch();
getch();      */
/*if(Cur_Joueur>0)
	{
	printf("\nL'Equipe %s est vainqueur!!",Eq_Name[Cur_Joueur-1]);
	Mk_Stats();
	printf("\n\nLe meilleur mouton : %s (%d Frags)\nLe plus precis : %s (%d Frags)\nLe plus violant : %s (%d Frags)\nLe plus...INUTILE : %s ( %d Frags)",Mouton[Stats.Mouton[0]].Nom,Mouton[Stats.Mouton[0]].Frags,Mouton[Stats.Mouton[1]].Nom,Mouton[Stats.Mouton[1]].Frags,Mouton[Stats.Mouton[2]].Nom,Mouton[Stats.Mouton[2]].Frags,Mouton[Stats.Mouton[3]].Nom,Mouton[Stats.Mouton[3]].Frags);
	}
else printf("\nIl n'y a pas de gagnant!!!!"); */
//for(cont=0;cont<4;cont++)
for(cont=0;cont<Options.Nb_Plans;cont++)
  farfree(Plan_1[cont]);
for(cont=0;cont<2;cont++)
  farfree(Plan_2[cont]);
Scan_Code[1]=0;
}


/*void Clry(bytef *dst)
{
asm{
  push ds
  les di,dst
  xor ax,ax
  mov cx,25600
  rep stosw
  pop ds
}
}    */


/*void FlamesAsm2(bytef *src,bytef* dst)   //brule l'ecran(assembleur)
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
	*/




//**********************************************************************************************
//													PROGRAME PRINCIPALE
//**********************************************************************************************
void Put_Foy(void)
{
register cont,i;
unsigned short Co;
for(cont=0;cont<320;cont++)
  {
  Page[63360+cont]=(Rnd3[RndVar++]>>3)+(Rnd2[RndVar]>>3)+15;
  }
for(cont=0;cont<3;cont++)
  {
  Co=63040+Rnd2[RndVar++]+30;
  for(i=0;i<10;i++)
	  {
	  Page[Co+i]=55;
	  Page[Co+320+i]=55;
	  }
  }
for(cont=0;cont<3;cont++)
  {
  Co=63040+Rnd3[RndVar++]+30;
  for(i=0;i<10;i++)
	  {
	  Page[Co+i]=60;
	  Page[Co+320+i]=60;
	  }
  }
for(cont=0;cont<5;cont++)
  {
  Co=63040+Rnd4[RndVar++]/2+96;
  for(i=0;i<10;i++)
	  {
	  Page[Co+i]=63;
	  Page[Co+320+i]=63;
	  }
  }
}

char On_Obj(char Deb)
{
register cont;
for(cont=6;cont<11;cont++)
	{
	if((x>Obj[cont].X)&&(x<(Obj[cont].X+Obj[cont].Lg))&&(y>Obj[cont].Y)&&(y<(Obj[cont].Y+Obj[cont].Ht)))
		return(cont);
	}
for(cont=Deb;cont<26;cont++)
	{
	if((x>Obj[cont].X)&&(x<(Obj[cont].X+Obj[cont].Lg))&&(y>Obj[cont].Y)&&(y<(Obj[cont].Y+Obj[cont].Ht)))
		return(cont);
	}
return(-1);
}

void Mk_Liste(void)
{
register cont,cont2,cont3=0;
for(cont2=0;cont2<Options.Nb_Joueurs;cont2++)
{
	for(cont=0;cont<12;cont++)
		if (TeamListe[cont])
		{
		Liste[cont3]=cont*8+cont2;
		cont3++;
		}
	 }
}

void Rnd_Liste(void)
{
register  cont;
register unsigned char nb1,nb2,temp;
for(cont=0;cont<96;cont++)
	ListePos[cont]=Liste[cont];
for(cont=0;cont<1000;cont++)
{
nb1=random(Options.Nb_J_Tot);
nb2=random(Options.Nb_J_Tot);
temp=ListePos[nb1];
ListePos[nb1]=ListePos[nb2];
ListePos[nb2]=temp;
}
}




void Init_Long(void)
{
register unsigned char cont;
for(cont=0;cont<16;cont++)
	{
	Obj[cont].X=Pos[cont<<2];
	Obj[cont].Y=Pos[(cont<<2)+1];
	Obj[cont].Lg=Pos[(cont<<2)+2];
	Obj[cont].Ht=Pos[(cont<<2)+3];
	}
/*Obj[0].X=90;
Obj[0].Y=20;
Obj[0].Lg=0;
Obj[0].Ht=0;

for(cont=1;cont<6;cont++)
	{
	Obj[cont].X=145;
	Obj[cont].Y=20*cont+40;
	Obj[cont].Lg=54;
	Obj[cont].Ht=14;
	}
Obj[6].X=250;
Obj[6].Y=155;
Obj[6].Lg=54;
Obj[6].Ht=14;
Obj[7].X=30;
Obj[7].Y=155;
Obj[7].Lg=54;
Obj[7].Ht=14;
Obj[8].X=150;
Obj[8].Y=155;
Obj[8].Lg=54;
Obj[8].Ht=14;

Obj[9].X=300;
Obj[9].Y=50;
Obj[9].Lg=14;
Obj[9].Ht=14;

Obj[10].X=300;
Obj[10].Y=135;
Obj[10].Lg=14;
Obj[10].Ht=14;

for(cont=0;cont<4;cont++)
	{
	Obj[cont+11].X=10;
	Obj[cont+11].Y=68+18*cont;
	Obj[cont+11].Lg=280;
	Obj[cont+11].Ht=14;
	}
Obj[15].X=10;
Obj[15].Y=50;
Obj[15].Lg=280;
Obj[15].Ht=14;
		 */
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

void Load_Graph_Menu(void)
{
register cont;
Clr(Video);
Clr(Page);
Load_Txt_Menu();
Init_Pal(Pal[2]);
SetAllPala(Pal[2]);
InitMouse();
MouseOFF();
ZoneMouse(1,1,317,196);
}


void FlamesAsm(bytef *src,bytef* dst)   //brule l'ecran(assembleur)
{
asm{
	push ds                              //sauvegarde ds
	les di,dst                           //|mémorise les adresses des pages
	lds si,src                           //|
	mov ax,320
	add si,ax
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

void Put_Cur(void)
{
Page[y*320+x]=(Rnd2[RndVar]>>3)+40;
Page[y*320+x+1]=(Rnd3[RndVar]>>3)+40;
}

void BurnScreen(void)
{
register cont;
for(cont=0;cont<40;cont++)
{
FlamesAsm(Page,Page);
Put_Foy();
Bal();
Drawy(Page);
}
}

void Draw_Obj(unsigned short X,unsigned short Y,char *txt,char Nb,unsigned char Coul)
{
if(Cur_Obj==Nb)
{
Print_Txt_Menu(X+Rnd5[RndVar++],Y+Rnd6[RndVar&112]-1,txt,Page,Coul-16);
Print_Txt_Menu(X+(Rnd5[RndVar++]>>1),Y+(Rnd6[RndVar&45]>>1),txt,Page,Coul+16);
}
else
{
Print_Txt_Menu(X+Rnd6[RndVar--],Y+Rnd5[RndVar]-1,txt,Page,Coul-16);
Print_Txt_Menu(X,Y,txt,Page,Coul);
}
}


void Draw_Obj_Nb(unsigned short X,unsigned short Y,unsigned short N,char Nb,unsigned char coul)
{
if(Cur_Obj==Nb)
{
Print_Nb_Menu(X+Rnd5[RndVar++],Y+Rnd6[RndVar&112]-1,N,Page,coul-16);
Print_Nb_Menu(X+(Rnd5[RndVar++]>>1),Y+(Rnd6[RndVar&45]>>1),N,Page,coul+16);
}
else
{
Print_Nb_Menu(X+Rnd6[RndVar--],Y+Rnd5[RndVar]-1,N,Page,coul-16);
Print_Nb_Menu(X,Y,N,Page,coul);
}
}

void OptionsScreen(void)
{
register cont;
unsigned short Temp[10];
unsigned char b;
char JustPush=0;

for(cont=0;cont<4;cont++)
	{
	Obj[cont+16].X=10;
	Obj[cont+16].Y=45+18*cont;
	Obj[cont+16].Lg=300;
	Obj[cont+16].Ht=14;
	Obj[cont+20].X=270;
	Obj[cont+20].Y=45+18*cont;
	Obj[cont+20].Lg=0;
	Obj[cont+20].Ht=0;
	}

Temp[0]=Options.Pts_2_Vie;
Temp[1]=Options.Nb_Joueurs;
Temp[2]=Options.Nb_Plans;
Temp[3]=Niveau.Cur_Level+1;
Temp[4]=50;
Temp[5]=8;
Temp[6]=4;
Temp[7]=Niveau.Nb_Level;


while(GetMouse(&x,&y));
while(!((Cur_Obj==8)&&b))
	{
	b=GetMouse(&x,&y);
	if(!b)
		JustPush=0;
	Cur_Obj=On_Obj(11);
	Draw_Obj(Obj[2].X,Obj[2].Y-50,Txt[2],Cur_Obj,32);
	for(cont=0;cont<3;cont++)
	{
	Draw_Obj(Obj[16+cont].X,Obj[16+cont].Y,Txt[cont+11],cont+16,32);
	Draw_Obj_Nb(Obj[20+cont].X,Obj[20+cont].Y,Temp[cont],cont+16,32);
	}
	Draw_Obj(Obj[19].X,Obj[19].Y,Txt[14],19,32);
	Draw_Obj(Obj[19].X+200,Obj[19].Y,Niveau.Level[Temp[3]-1],19,32);

	Draw_Obj(Obj[8].X,Obj[8].Y,Txt[8],8,32);
if(b&&!JustPush)
	  {
	  JustPush=1;
	  if((Cur_Obj>15)&&(Cur_Obj<20))
				{
					Temp[Cur_Obj-16]-=((b)&2)-1;
					if(Temp[Cur_Obj-16]>Temp[Cur_Obj-12])
						 Temp[Cur_Obj-16]=Temp[Cur_Obj-12];
					if(!Temp[Cur_Obj-16])
						Temp[Cur_Obj-16]=1;
				}
	  }

Put_Cur();
FlamesAsm(Page,Page);
Put_Foy();
Bal();
Drawy(Page);
}
Options.Pts_2_Vie=Temp[0];
Options.Nb_Joueurs=Temp[1];
Options.Nb_Plans=Temp[2];
Niveau.Cur_Level=Temp[3]-1;
TX=Options.Nb_Plans*320;
BurnScreen();
}




void TeamScreen(void)
{
register cont;
char CurT=0,CurLet=0;
char JustPush=0;
char Debu=0;
char Edit=0,ch;
char *DebTxt;
char temp[16][50];
unsigned short b;
sprintf(temp[0],"Equipe %d",CurT+1);
sprintf(temp[1],"Nom : %s",Eq_Name[CurT]);
for(cont=0;cont<4;cont++)
	sprintf(temp[cont+2],"Mouton %d : %s",cont+1,Mouton[CurT*8+cont].Nom);
while(GetMouse(&x,&y));
while(!((Cur_Obj==8)&&b))
	{
	if(!Edit)
		b=GetMouse(&x,&y);
	if(!b)
		JustPush=0;
	Cur_Obj=On_Obj(11);
	Draw_Obj(130,20,temp[0],Cur_Obj,32);
	Draw_Obj(Obj[15].X,Obj[15].Y,temp[1],15,32);
	for(cont=0;cont<4;cont++)
		Draw_Obj(Obj[cont+11].X,Obj[cont+11].Y,temp[cont+2],cont+11,32);
	for(cont=6;cont<11;cont++)
		Draw_Obj(Obj[cont].X,Obj[cont].Y,Txt[cont],cont,32);
	if(b&&!JustPush)
	  {
	  JustPush=1;
	  switch(Cur_Obj)
		  {
		case 10 :
					if(Debu<4)
					Debu++;
					break;
		case 9  :if(Debu>0)
					Debu--;
					break;
		case 6  :CurT++;
					CurT%=12;
					break;
		case 7  :CurT--;
					if(CurT<0)
					CurT=11;
					break;
		case 11 :
		case 12 :
		case 13 :
		case 14 :
					Edit=1;
					CurLet=0;
					DebTxt=Mouton[CurT*8+Cur_Obj-11+Debu].Nom;
					DebTxt[0]='\0';
					break;
		case 15 :
					Edit=1;
					CurLet=0;
					DebTxt=Eq_Name[CurT];
					DebTxt[0]='\0';
					break;

		  }
	  if((Cur_Obj>5)&&(Cur_Obj<11))
		  {
		  sprintf(temp[0],"Equipe %d",CurT+1);
		  sprintf(temp[1],"Nom : %s",Eq_Name[CurT]);
		  for(cont=0;cont<4;cont++)
				sprintf(temp[cont+2],"Mouton %d : %s",cont+1+Debu,Mouton[CurT*8+cont+Debu].Nom);
		 }
	}


	if(!Edit)
	  Put_Cur();
	else
	  {
	  if(kbhit())
		 {
		 ch=getch();
			switch(ch)
				{
				case 13 : Edit=0;break;
				case 8 :  if(CurLet)
							 CurLet--;
							 DebTxt[CurLet]='\0';
							 break;
				default : if(CurLet<17)
							 DebTxt[CurLet++]=ch;
							 DebTxt[CurLet]='\0';
							 break;
				}
	  }
	  sprintf(temp[0],"Equipe %d",CurT+1);
	  sprintf(temp[1],"Nom : %s",Eq_Name[CurT]);
	  for(cont=0;cont<4;cont++)
			sprintf(temp[cont+2],"Mouton %d : %s",cont+1+Debu,Mouton[CurT*8+cont+Debu].Nom);
	}
FlamesAsm(Page,Page);
Put_Foy();
Bal();
Drawy(Page);
}
BurnScreen();
}

char SelectScreen(void)
{
register cont;
char Debut=0;
char JustPush;
unsigned short b;

for(cont=0;cont<6;cont++)
	{
	Obj[cont+16].X=10;
	Obj[cont+16].Y=45+18*cont;
	Obj[cont+16].Lg=225;
	Obj[cont+16].Ht=14;
	}
b=0;

while(!(((Cur_Obj==8)||(Cur_Obj==7))&&b))
	{
	b=GetMouse(&x,&y);
	if(!b)
		JustPush=0;
	Cur_Obj=On_Obj(16);
	Draw_Obj(75,15,"Selectionnez les equipes",Cur_Obj,32);
	for (cont=0;cont<6;cont++)
		{
		if(TeamListe[cont+Debut])
					Draw_Obj(Obj[cont+16].X,Obj[cont+16].Y,Eq_Name[cont+Debut],Cur_Obj,32);
			else
					Draw_Obj(Obj[cont+16].X,Obj[cont+16].Y,Eq_Name[cont+Debut],-2,32);
		}
	for(cont=7;cont<11;cont++)
		Draw_Obj(Obj[cont].X,Obj[cont].Y,Txt[cont],cont,32);
	if(b&&!JustPush)
	  {
	  JustPush=1;
	  switch(Cur_Obj)
		  {
		case 10 :
					if(Debut<6)
					Debut++;
					break;
		case 9  :if(Debut>0)
					Debut--;
					break;
		case 16 :
		case 17 :
		case 18 :
		case 19 :
		case 20 :
		case 21 : TeamListe[Cur_Obj-16+Debut]=!TeamListe[Cur_Obj-16+Debut];
					 break;

		  }
		  }
	Put_Cur();
	FlamesAsm(Page,Page);
	Put_Foy();
	Bal();
	Drawy(Page);
	}

BurnScreen();
Options.Nb_Equipes=0;
for(cont=0;cont<12;cont++)
	if(TeamListe[cont])
		Options.Nb_Equipes++;
Options.Nb_J_Tot=Options.Nb_Joueurs*Options.Nb_Equipes;
if(Cur_Obj==7)
	return(0);
if(Options.Nb_Equipes<2)
	return(0);
return(1);

}


void StatsScreen(void)
{
char temp[5][50];
unsigned short b;
register cont;
//while(kbhit());

if(Winner>0)
{
sprintf(temp[0],"L'Equipe %s gagne !",Eq_Name[Winner-1]);
sprintf(temp[1],"Le meilleur mouton : %s",Mouton[Stats.Mouton[0]].Nom);
sprintf(temp[2],"Le plus precis : %s",Mouton[Stats.Mouton[1]].Nom);
sprintf(temp[3],"Le plus violant : %s",Mouton[Stats.Mouton[2]].Nom);
sprintf(temp[4],"Le plus . . . INUTILE : %s",Mouton[Stats.Mouton[3]].Nom);
}
else
{
temp[0][0]='\0';
temp[1][0]='\0';
sprintf(temp[2],"        Il n'y a aucun vainqueur!");
temp[3][0]='\0';
temp[4][0]='\0';
}

b=0;
while(!((Cur_Obj==8)&&b))
	{
	b=GetMouse(&x,&y);
	Cur_Obj=On_Obj(11);
	Draw_Obj(10,20,temp[0],Cur_Obj,32);
	for(cont=0;cont<4;cont++)
		Draw_Obj(10,50+cont*20,temp[cont+1],-2,32);
	Draw_Obj(Obj[8].X,Obj[8].Y,Txt[8],8,32);
	Put_Cur();
	FlamesAsm(Page,Page);
	Put_Foy();
	Bal();
	Drawy(Page);
	}
BurnScreen();
}


void main(void)
{
//unsigned long images=0;                   //nombre d'images enregistrés
unsigned short cont;                      //compteur
unsigned short b;
char fin=0;
clrscr();                                 //efface l'écran
randomize();
for(cont=0;cont<256;cont++)
	{
	Rnd1[cont]=random(255);
	Rnd2[cont]=random(255);
	Rnd3[cont]=random(255);
	Rnd4[cont]=random(255);
	Rnd5[cont]=(random(255)-127)>>6;
	Rnd6[cont]=(random(255)-127)>>6;

}
printf("\nMemoire Libre :%lu octets\n\n",coreleft());
//Init_XMS();
//Init_Pages();
printf("Mouton Chronicles Version %s Par :\n\tBaudin A. (Bart 57)\n\tJolivalt J.   (dJo)\n\tDri C.  (GnoMaster)\n\n\n",VERSION);
getch();                                  //Lit une touche
Init_Tab();                               //Initlise le tableau
Init_Mem();                               //Initalise la memoire(2eme partie...)
Lire_Jeu_Cfg();
Mouton=farcalloc(96,46);
Lire_Joueur_Cfg();
Lire_Lang();
ModeVGA();
Nb_CD();
Cur_Lect=CD.First;
Get_T_Inf();
delay(2000);
Play(1);
start();
Load_Graph_Menu();
//setvect(0x1c,Vide);                       //enleve l'effet de clignottement
/*ModeVGA();
Init_Pal(Pal[2]);
SetAllPala(Pal[2]);*/
Init_Long();
while(!fin)
{
//Clry(Page);
Put_Foy();
b=GetMouse(&x,&y);
Cur_Obj=On_Obj(0);
Draw_Obj(Obj[0].X,Obj[0].Y,Txt[0],Cur_Obj,32);
for(cont=1;cont<6;cont++)
		Draw_Obj(Obj[cont].X,Obj[cont].Y,Txt[cont],cont,32);

if((Cur_Obj!=-1)&&b)
	{
	switch(Cur_Obj)
	{
	case 1 : BurnScreen();
				if(SelectScreen())
				{
				Mk_Liste();
				Rnd_Liste();
				BurnScreen();
				for(cont=0;cont<4;cont++)
					LastExB[cont]=0;
				Bombe.Used=0;
				Vent=0;
				jouer();
            Play(1);
				Load_Graph_Menu();
				StatsScreen();
				}
				break;
	case 2 : BurnScreen();
				OptionsScreen();
				break;
	case 3 :	BurnScreen();
				TeamScreen();
				break;
	case 4 :	BurnScreen();
				StatsScreen();
				break;
	case 5 : BurnScreen();
				fin=1;
				break;
	}
}
Put_Cur();
FlamesAsm(Page,Page);
Bal();
Drawy(Page);
}
Ecrire_Jeu_Cfg();
Ecrire_Joueur_Cfg();
Stop();
ModeTxt();
}




//2684 lignes de code !!! 65.5 Ko!!!
//  ->voir LigneCnt.exe
