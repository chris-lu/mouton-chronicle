#include "mouton.h"			//Header du jeu...
#include "cd.c"				//Gestion du lecteur cd
#include "start.c"			//ecrans de demarrages
//#include "Rtime.c"
#include "mouse.c"			//gestion de la souris
#include "clavier.c"			//gestion du clavier
#include "option.c"			//Lecture des fichiers d'options du jeu
#include "graph.c"
#include "palette.c"			//Gestion de la pallette des couleurs
#include "plans.c"         //Gestion et affichage des differants plans
#include "moutgest.c"      //Gestion generale des moutons
#include "texte.c"         //Gestion des texte....

void Mem_Error(void)
{
//printf("\npas assez de memoire conv.!");
exit(1);
}

//**********************************************************************************************
//Initalisation de la memoire : Allocation n°1
//**********************************************************************************************
void Init_Mem(void)
{
register cont;
if((Page=(unsigned char far *)farmalloc(64000))==NULL)	//Allocation mémoire pour la page de travail: 1 Page de 64 Ko
	Mem_Error();
for(cont=0;cont<96;cont++)
	if((Lettre[cont]=(unsigned char far *)farmalloc(144))==NULL)
		Mem_Error();
for(cont=0;cont<50;cont++)
	if((Mout_Spr[cont]=(unsigned char far *)farmalloc(64))==NULL)
		Mem_Error();
for(cont=0;cont<3;cont++)
	Pal[cont]=(unsigned char far *)farmalloc(768);		//Allocation mémoire pour les pallettes: 3 tableaux de 768 octets chacun

}

//**********************************************************************************************
//Initalisation de la memoire : Allocation n°2
//**********************************************************************************************
/*void Init_Mem1(void)
{
register cont;
			//Allocation mémoire pour le premier plan : 4 Pages de 64 Ko
for(cont=0;cont<Options.Nb_Plans;cont++)
	if((Plan_1[cont]=(unsigned char far *)farmalloc(64000))==NULL)
		Mem_Error();

}     */

//**********************************************************************************************
//Initalisation de la memoire : Allocation n°3
//**********************************************************************************************
/*void Init_Mem2(void)
{
register cont;
			//Allocation mémoire pour le second plan : 2 Pages de 64 Ko
for(cont=0;cont<2;cont++)
	if((Plan_2[cont]=(unsigned char far *)farmalloc(64000))==NULL)
		Mem_Error();
}

  */





//**********************************************************************************************
//Initalisation du missile (direction,puissance,etc..)
//**********************************************************************************************
void Init_Bomb(void)
{
Bombe.Used=1;
Bombe.Explosion=0;
Bombe.DirX=CosT[Vise]*Puissance*Mouton[CURJ].Sens;
Bombe.DirY=SinT[Vise]*Puissance;
Mouton[CURJ].DirX-=Bombe.DirX/4;
Mouton[CURJ].DirY-=Bombe.DirY/4;

Bombe.PosY=Mouton[CURJ].PosY+2;
Bombe.PosX=Mouton[CURJ].PosX+4;
}

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
//Tester la colision du missile et du terrain...
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

//**********************************************************************************************
//Arme n°1 et n°3 : le bazooka et super missile.....
//**********************************************************************************************
void ArmBombe(void)
{
register cont;
if(!Bombe.Explosion)              //la bombe n'explose pas?
	{
	Bombe.DirY-=Gravite;           //Attraction(9.81 /200)
	Bombe.DirX+=Vent;
	}
if ((Bombe.PosY>0)&&(Bombe.PosY+abs(Bombe.DirY)<200)&&(Bombe.PosX>0)&&(Bombe.PosX<TX))    //si la bombe est dans le terrain
	{
	if(Test_Line())    //si la bombe touche le sol
		{
		Tremble=5;      //Faire trembler l'ecran....
		Vent=(float)(random(100)-50)/2500;          //ganger la direcrtion du vent
		Put_trou(Bombe.PosX-(Trou[0].Taille>>1),200-Bombe.PosY-(Trou[0].Taille>>1),Trou[0].Taille,Trou[0].Taille,Trou[0].Spr);    //afficher un trou
		Test_Dom(25);//tester les dommages sur les mouton....
		Init_Explose(Bombe.PosX,Bombe.PosY,16);//creer l'explosion
		if (Rebonds<6)   //Encore des rebond???
			{
			Bombe.DirX=(float)random(300)/100-1.5; //changer la direction du missile
			Bombe.DirY=random(500)/100;
			Rebonds++;
			}
		else
			{                                //Sinon....
			Bombe.Explosion=1;               //Le missile vient d'exploser
			Wait=250;                        //fin du tour....
			Bombe.Used=0;                    //bobe inutilise
			Rebonds=0;                       //Plus de rebond
			SetMouse(x,y);
			}

		}
	}
else if (Bombe.PosY+abs(Bombe.DirY)<200)  //si la bombe sort...
	{
	Vent=(float)(random(100)-50)/2500;
	Bombe.Used=0;                                             //Bobe inutilise
	Wait=150;                                                 //fin du tour
	Rebonds=0;                                                //......
	SetMouse(x,y);
	}
else                                                         //Si la bombe est au dessus de l'ecran
	{                                                         //continuer le deplacement
	Bombe.PosX+=Bombe.DirX;
	Bombe.PosY+=Bombe.DirY;
	}
if((Bombe.PosX>x)&&(Bombe.PosX<(x+320))&&(Bombe.PosY>200-y)&&(Bombe.PosY<400-y))  //si la bombe est dans l'ecran,...
	Page[(Y[400-y-Bombe.PosY]+(Bombe.PosX-x))]=255;                                //la dessiner
}


//**********************************************************************************************
//Arme n°2 : Le Kamikaze.....
//**********************************************************************************************
void ArmSuicide(void)
{
Mouton[CURJ].Mort=1;               //Deces du mouton...
Init_Explose(Mouton[CURJ].PosX,Mouton[CURJ].PosY+4,64);//creer l'explosion
Init_Explose(Mouton[CURJ].PosX,Mouton[CURJ].PosY+4,16);//creer l'explosion
Flash[0]=10;     //Faire flasher l'ecran
Put_trou(Bombe.PosX-(Trou[1].Taille>>1),200-Bombe.PosY-(Trou[1].Taille>>1),Trou[1].Taille,Trou[1].Taille,Trou[1].Spr);    //et un piti trou de plus!
Vent=(float)(random(100)-50)/2500;     //Le cent change
Wait=175;                              //Fin du tour
Test_Dom(50);                          //tester les dommages..
Message(11,0,CURJ);                    //affiche un message categorie 11 (suicides en tout genres)
Bombe.Used=0;
SetMouse(x,y);
}

//**********************************************************************************************
//Arme n°4 : le super suicide.....
//**********************************************************************************************
void ArmSuicide2(void)
{
short TmpX,TmpY;
register cont;
if(Suicide)         //Le suicide a-t-il commencé????
	{
	if(Suicide&1)    //Vire au blanc toutes les 2 images
		{
		Goto_Pal(Pal[PalNb],Pal[2]);
		SetAllPala(Pal[PalNb]);
		}
	Bombe.PosX=Mouton[CURJ].PosX;       //met l'explosion au niveau du joueur
	Bombe.PosY=Mouton[CURJ].PosY;
	Tremble=(random(175-Suicide))>>4;   //faire trembler l'ecran de + en +
	if(Suicide==1)                      //derniere phase du suicide?
		{
		Mouton[CURJ].Mort=1;             //tuer la bestiole
		for(cont=0;cont<20;cont++)      //25 explosions dans l'écran
			{
			TmpX=(random(150))-75;       //aire d'effet : 150x90 pixels
			TmpY=(random(90))-45;
			if((Bombe.PosX+TmpX>0)&&(Bombe.PosX+TmpX<TX)&&(Bombe.PosY+TmpY>0)&&(Bombe.PosY+TmpY<200))   //l'explosion est dans le terrain?
				{
				Bombe.PosX+=TmpX;  //decaler l'explosion.
				Bombe.PosY+=TmpY;
				Put_trou(Bombe.PosX-(Trou[1].Taille>>1),200-Bombe.PosY-(Trou[1].Taille>>1),Trou[1].Taille,Trou[1].Taille,Trou[1].Spr);    //Un trou
				Test_Dom(50);  		//50 points de dommages par explosion!!!
				Bombe.PosX-=TmpX;
				Bombe.PosY-=TmpY;
				}
			else cont--;  //si dehor de l'ecran, recalculer une nouvelle position...
			}
		Message(11,0,CURJ);        //Message categorie 11 : suicides
//		CpyPal(Pal[!PalNb],Pal[PalNb]);
		Init_Explose(Mouton[CURJ].PosX,Mouton[CURJ].PosY+4,64);//creer l'explosion
		Vent=(float)(random(100)-50)/2500;   //change la direction du vent
		Wait=400;                         //Fin du tour
		Bombe.Used=0;
		SetMouse(x,y);
		Chal=350;                         // effet de chaleur (fonction sinus...)
		}
	Suicide--;
	}
else
	Suicide=170;                            //debut du suicide : 170 images....
}

//**********************************************************************************************
//Arme n°5 : Colere divine.....
//**********************************************************************************************
void Eclairs(void)
{
register short cont,cont2;
if(Ecl)         //debut de l'animation?
	{
	if(Ecl>350)  //phase 1 : tremblement
		{
		if(Ecl>380)   //phase 2 eclair sortant du mouton...
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
	else if(Ecl<250&&Ecl>200)  //phase 3 : fin de tremblement
		Tremble=random(2);
								//phase 4 : pitite pause...
	else if(Ecl<200)           //phase finale : destruction totale !!!!
		{
		EclPos=random(Options.Nb_Plans*320); //position du haut de l'eclair
		for(cont=400;cont>0;cont--)          //affiche l'eclair en totalite
			{
			EclPos+=(random(3)-1);            //faire vibrer l'eclaire
			if((EclPos>x)&&(EclPos<(x+320))&&(cont>200-y)&&(cont<400-y))  //si dans l'écran...
				Page[(Y[400-y-cont-1]+(EclPos-x))]=60+random(4);  //...afficher
			if(cont<200)                                         //si atteind l'ecran (x<200)
				{
				if(Plan_1[EclPos/320][Y[200-cont]+(EclPos%320)])     //Si touche le sol..
					{
					Bombe.PosX=EclPos;                                //donner les nouvelles coordonnes de l'explsion..
					Bombe.PosY=cont;
					if((Bombe.PosX>0)&&(Bombe.PosX<TX)&&(Bombe.PosY>0)&&(Bombe.PosY<200))
						Test_Dom(16);         //tester les domages
					if(!(Ecl&31))            //flash l'ecran toutes les 32 images
						Flash[0]=2;
					Tremble=random(3);       //fait trembler l'ecran
					break;
					//cont=250;
					}
				}
			}
		}
	if(Ecl==1)                     //fin de l'attaque...
		{
		Wait=200;
		Bombe.Used=0;
		}
	Ecl--;
	}
else Ecl=580;                        //debut de l'attaque valable qu'une fois...
}

//**********************************************************************************************
//Medikit....
//**********************************************************************************************
void MediKit(void)
{
Mouton[CURJ].Pts_Vie=Options.Pts_2_Vie-1; //restaure les points de vie
Flash[0]=2;                               //fait flasher l'ecran.......
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


//**********************************************************************************************
//Affiche les phrases dites par les mouton.....
//**********************************************************************************************
void Draw_Paroles(void)
{
char Txt[155];
register cont,cont2;
while((!Par[0][2])&&(Par[1][2]+Par[2][2]+Par[3][2])) //la phrase 1 est devenue obsolete???
	Txt_Down();    //l'enlever et deplacer les autres phrases
for(cont=0;cont<4;cont++)  // Affiche les paroles
	{
	if(Par[cont][2])
		{
		Par[cont][2]--;      //temps limite...
		sprintf(Txt,"%s : %s",Mouton[Par[cont][0]].Nom,Paroles[Par[cont][1]]);
		Print_Txt(120,cont*7+5,Txt,Page,244+Mouton[Par[cont][0]].Equipe);  //afficher le texte..
		}
	}
}











//**********************************************************************************************
//Affiche les scores et le joueur actuel.......
//**********************************************************************************************
void Draw_Scores (void)
{
register cont;
if(Deb_Tour)     //affichage actif?
	{
	for(cont=0;cont<Options.Nb_J_Tot;cont++) //pour chaque mouton...
		if(!Mouton[Liste[cont]].Mort)       //...s'il n'est pas mort....
			{
			if((Mouton[Liste[cont]].PosX>x)&&(Mouton[Liste[cont]].PosX<(x+312))&&(Mouton[Liste[cont]].PosY>200-y)&&(Mouton[Liste[cont]].PosY<376-y))  //...et si il est dans l'écran...
				{
				if(Cur_Joueur!=cont)    //affiche le score des joueurs autres que le joueur actuel...
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






//**********************************************************************************************
//Procedure principale de jeu...
//**********************************************************************************************
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
Let=0;

Proc[0]=*ArmBombe;            //Tableau de pointeurs vers fonctions
Proc[1]=*ArmSuicide;
Proc[2]=*ArmBombe;
Proc[3]=*ArmSuicide2;
Proc[4]=*Eclairs;
Proc[5]=*MediKit;

Trou[0].Spr=TrSpr1;          //memorise les caracteristiques des trous (petits et gros...)
Trou[1].Spr=TrSpr2;
Trou[0].Taille=24;
Trou[1].Taille=46;

for(cont=0;cont<768;cont++)      //Pallette de couleur toute blache pour les flash..
	Pal[2][cont]=63;

NbRot=0;
Lire_Level_Cfg(Niveau.Cur_Level);     //Lire les options des niveaux...
for(cont=0;cont<Options.Nb_Plans;cont++)
	if((Plan_1[cont]=(unsigned char far*)farmalloc(64000))==NULL) //memoire pour le premier plan
		Mem_Error();

Load_Ter();                 //creer un nouveau terrain...
for(cont=0;cont<2;cont++)          //Memoire pour le desieme plan
	if((Plan_2[cont]=(unsigned char far*)farmalloc(64000))==NULL)
		Mem_Error();

Init_Graph();        //Memorise et initaialise les graphismes du jeu.

Pal[0][765]=63;      //Couleur du missile...
Pal[0][766]=63;
Pal[0][767]=63;
Load_Txt();          //Charger la police d'ecriture
SetAllPala(Pal[0]);                        //active la pallette...
CpyPal(Pal[0],Pal[1]);                     //puis la duplique
InitMouse();                               //initialise la souris...
ZoneMouse(0,50,TX-320,200);                //et definie sa zone de déplacement
SetMouse(1,1);
for(cont=0;cont<Options.Nb_J_Tot;cont++)  //reinitialise les variables
	{
	Put_Mouton(10+((TX-10)/(Options.Nb_J_Tot+1))*(cont+1),ListePos[cont]); //positionner les moutons sur le terrain..
	Mouton[Liste[cont]].DirX=0;
	Mouton[Liste[cont]].DirY=0;
	Mouton[Liste[cont]].Mort=0;
	Mouton[Liste[cont]].Frags=0;
	Mouton[Liste[cont]].Degats=0;
	Mouton[Liste[cont]].Sens=(random(2)<<1)-1;
	Mouton[Liste[cont]].Pts_Vie=Options.Pts_2_Vie;
	}

for(cont=0;cont<12;cont++)
	for(cont2=0;cont2<6;cont2++)
		Stock[cont][cont2]=1;     //Definie le type d'armement disponible...
Install_Clav();               		//modifie l'interruption clavier
//DebTime();                              //début du chronometre
Stats.Bourrin=64000;          		//Mise a zero des statistiques
Stats.Precis=0;
Suicide=0;Ecl=0;Chal=0;
Cur_Joueur=random(Options.Nb_J_Tot);     //Initialise le premier joueurs....
CURJ=Liste[Cur_Joueur];
CURT=Mouton[CURJ].Equipe;
if(Mouton[CURJ].Pts_Vie>5)               //et l'armement disponible..
	cont2=0;
else if (Mouton[CURJ].Pts_Vie>1)
	cont2=1;
else
	cont2=2;
for(cont=0;cont<6;cont++)                	//copie du type d'armement par defaur dans le stock du mouton
	Stock[CURT][cont]=Panoplie[cont2][cont];
Arme=0;
Play(Track);
FadeOut=64;
FinPart=0;                           	//Lire une piste audio
Rebonds=0;
Light=1;
fcloseall();
while((!FinPart)||FadeOut)                    	//tant que la touche [Esc] n'est pas enfoncée....
	{
	if(Flash[0])					//L'ecran doit flasher?
		{
		SetAllPala(Pal[2]);			//L'ecran devient blanc pendant Flash[0] images....
		Flash[0]--;
		if(!Flash[0])
			Flash[1]=1;
		}
	else if(Flash[1])
		{
		SetAllPala(Pal[PalNb]);			//puis redevient normal....
		Flash[1]=0;
		}

	if(!(Palrot&7)&&!Suicide&&!Chal)              //rotation de pallette toutes les 8 images
		{
		PalNb=!PalNb;                           //change la pallette active   (
		for(cont=0;cont<NbRot;cont++)
			Rot_Pal(Rotation[cont].Deb,Rotation[cont].Longueur,Pal[PalNb],Pal[!PalNb]);
		Rot_Pal(1,14,Pal[PalNb],Pal[!PalNb]);   //Rotation des couleurs
		SetAllPala(Pal[PalNb]);                 //activation de la nouvelle pallette
		}

	if(Tremble)						     //fait decroitre le tremblement de l'image
		Tremble--;
	//images++;                                  //incrémente le nb d'images
	Palrot++;                                  //    "      le conteur pour la pallette
	Clr(Page);                                 //efface la page de travail
	if(Options.Ciel)
		{
		if(!Light&&Orage)
			{
			Light=random(500);
			Draw_White(Page);
			}
		else
			{
			Draw_Deg(Page,216);
			Light--;
			}
		}			 //creer un degrader pour le ciel (jour)
	else
		Draw_Back(Page);                     //sinon affiche les étoiles (nuit)
	if(Mouton[CURJ].Mort&&!Wait&&!Rebonds&!Ecl)       //Si le joueur meur, AU SUIVANT de jouer! (attend la fin des rebonds,etc...)
		Wait=100;
	else if (Wait&&(Rebonds||Ecl||Suicide))                     //Si le joueurs meur...Qu'il termine sont tour...
		Wait=100;
	if(Wait==1)							//fin de l'attente....
		{
		Lire_scan();					//lire les touches du clavier...
		Deb_Tour=1;						//affiche les noms des moutons.
		Last_Key=Scan_Code[0];				//memorise la derniere touche activé.
		Cur_Joueur++;					//change de joueur
		Cur_Joueur%=Options.Nb_J_Tot;			//	"	"
		CURJ=Liste[Cur_Joueur];				//	"	"
		Wait--;
		cont=255;						//trouver un mouton vivant....
		while(Mouton[CURJ].Mort&&cont)
			{
			Cur_Joueur++;
			Cur_Joueur%=Options.Nb_J_Tot;
			CURJ=Liste[Cur_Joueur];
			cont--;
			}
		CURT=Mouton[CURJ].Equipe;			//memorise l'equipe du mouton..(ce procede permet de gagne un peur de place en memoire...)
		Arme=0;						//arme du mouton : le bazooka
		if(Mouton[CURJ].Pts_Vie>5)			//initialise les armes du mouton en fonction de ses points de vie
			cont2=0;
		else if (Mouton[CURJ].Pts_Vie>1)
			cont2=1;
		else
			cont2=2;
		for(cont=0;cont<6;cont++)
			Stock[CURT][cont]=Panoplie[cont2][cont];
		Look_Obj(Mouton[CURJ].PosX,Mouton[CURJ].PosY,&x,&y);	//regarder le missile.
		SetMouse(x,y);
		}
	if(!Wait)                                  //placé ici pour reduire le tremblement....(trop long et chiant a expliquer...)
		Lire_Keys(CURJ);                               //teste les touches presses
	if(Bombe.Used&&!Ecl)                    //placé ici pour eviters les tremblements(pareil qu'au dessus : probleme de synchronisation des differants sprite et de l'ecran, bla bla bla,etc...).
		{
		Look_Obj(Bombe.PosX,Bombe.PosY,&x,&y);	//regarder le missile.
		Fallow=0;						//ne pas suivre le mouton...
		}
	else			//suit le mouton tant que la souris n'est pas deplacee.(permet la suivie du mouton en vol...)
		{				//rq: la suivie du mouton debute lors de la pression de certaines touche (voir "clavier.c")
		GetMouse(&Lx1,&Ly1);
		x=Lx1;
		y=Ly1;
		if(((Ly1!=Ly2)||(Lx1!=Lx2))&&Fallow&&Options.Fallow)	//les coordonnes de la souris ont changees???
			{
			Fallow=0;							//arreter la suivie
			Look_Obj(Mouton[CURJ].PosX+4,Mouton[CURJ].PosY+4,&x,&y);
			SetMouse(x,y);
			}
		else if(Fallow&&Options.Fallow)				//sinon continuer...
			{
			Look_Obj(Mouton[CURJ].PosX+4,Mouton[CURJ].PosY+4,&x,&y);
						}
		}                           //lit les coordonnés de la souris
	CpyPlan(x/3,(y>>2)+150,Plan_2,Page);      //affiche le second plan
	if (Bombe.Used)
		(*Proc[Arme])();			//execute la fonction des armes...
	if(!Wait)
		{
		if (Arme==0||Arme==2)
			Put_Viseur(CURJ);			//et place eventuellement un viseur.
		}
	else
		{
		Wait--;
		Mouton[CURJ].Cur_Spr=0;           // Ecite que le mouton se fige a la fin du tour
		}
	for(cont=0;cont<Options.Nb_J_Tot;cont++)
		{
		if(((Mouton[Liste[cont]].PosX<0)||(Mouton[Liste[cont]].PosX>TX-8)||(Mouton[Liste[cont]].PosY<0))&&!Mouton[Liste[cont]].Mort) //teste si le mouton est hors de l'ecran
			{
			Mouton[Liste[cont]].Mort=1;	//le mouton est mort..
			Message(13,0,Liste[cont]);	//afficher un message
			if(cont!=Cur_Joueur)
				Mouton[Liste[cont]].Frags++;	//ajouter cette mort au frags du joueur actuel.	
			}
		else if(!Mouton[Liste[cont]].Mort)	//si le mouton n'est pas mort...
			{
			Draw_Mouton(Liste[cont]);	//afficher ce mouton...
			Move_Mouton(Liste[cont]);	//..puis effectuer le deplacement...
			}
		}
	if(Deb_Tour)					//Debut du tour???
		Draw_Scores();				// afficher les scores
	CpyPlan(x,y,Plan_1,Page);                   //Copie le premier plan  //les plan doivent etre déssineé dans l'ordre décoissant pour une superposition
	for(cont=0;cont<4;cont++)  //calcul des explosion des bombes(3bombe max sur l'écrans
		{
		if(Explose[cont].Used)                   //Bombe utilisée?
			Bomb_Ex(cont);				//deplacer les particules
		else if(LastExB[cont])
			{
			Bomb_Ex(cont);				//si c'est la fin du deplacement, figer les particules
			End_Bomb(cont);
			}
		}
	Print_Barre(5,5,110,10,Puissance/6.5*100,88,0,1);		//afficher la barre de puissance
	if(Vent>0)								//et les barres du vents
		{
		Print_Barre(5,17,53,6,0,66,0,0);
		Print_Barre(62,17,53,6,Vent/0.02*100,99,0,1);
		}
	else
		{
		Print_Barre(5,17,53,6,-Vent/0.02*100,99,0,0);
		Print_Barre(62,17,53,6,0,66,0,1);
		}
	Draw_Paroles();							//afficher les paroles...
	Print_Txt(2,192,ArmName[Arme],Page,8);			//afficher le nom de l'arme
	if(Chal)								//effet de chaleur...
		{
		Goto_Pal(Pal[PalNb],Pal[!PalNb]);		//passage du blanc a la pallette de culeurs normale...
		SetAllPala(Pal[PalNb]);                 //activation de la nouvelle pallette
		Chal--;
		nb1=Chal>>4;					//amplitude des "vagues" de chaleur...
		Bal();
		for (cont=1;cont<200;cont++)			//afficher l'effet
		cpyh(cont,(nb1*tab3[(nb2+cont)&255])>>5,Page,Video);              //(procedure en assembleur...)
		nb2++;
		}
	else{
		Bal();                                    //attente du Balayage vertical
		Draw(Page,Tremble*320);			//Affiche la page de travail finale avec tremblement...
		}
	if(Gagnant()&&!Wait&&!Rebonds&&!Ecl)	//Teste les survivant...
		FinPart=1;				//et quitte s'il y a un gagnant...
if(Scan_Code[1])
	{FinPart=1;}
if(FinPart)
	{
	Dec_Pal(Pal[PalNb]);
	Dec_Pal(Pal[!PalNb]);
	SetAllPala(Pal[PalNb]);
	FadeOut--;
	}
}
//FinTime();                                  //arrete le chronometre

Remove_Clav();
Mk_Stats();
Winner=Gagnant();						//equipe gagnante


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
getch();
if(Cur_Joueur>0)	//ne set plus mais on sait jamais.......
	{
	printf("\nL'Equipe %s est vainqueur!!",Eq_Name[Cur_Joueur-1]);
	Mk_Stats();
	printf("\n\nLe meilleur mouton : %s (%d Frags)\nLe plus precis : %s (%d Frags)\nLe plus violant : %s (%d Frags)\nLe plus...INUTILE : %s ( %d Frags)",Mouton[Stats.Mouton[0]].Nom,Mouton[Stats.Mouton[0]].Frags,Mouton[Stats.Mouton[1]].Nom,Mouton[Stats.Mouton[1]].Frags,Mouton[Stats.Mouton[2]].Nom,Mouton[Stats.Mouton[2]].Frags,Mouton[Stats.Mouton[3]].Nom,Mouton[Stats.Mouton[3]].Frags);
	}
else printf("\nIl n'y a pas de gagnant!!!!");
for(cont=0;cont<4;cont++)			*/



for(cont=0;cont<Options.Nb_Plans;cont++)		//libere la memoire...
	farfree(Plan_1[cont]);
for(cont=0;cont<2;cont++)
	farfree(Plan_2[cont]);
Scan_Code[1]=0;
}


/*void Clry(bytef *dst)
{
asm	{
	push ds
	les di,dst
	xor ax,ax
	mov cx,25600
	rep stosw
	pop ds
	}
}    */


/*void FlamesAsm2(bytef *src,bytef* dst)   //brule l'ecran(assembleur) anciene procedure ne traitant que le bas de l'ecran...
{
asm	{
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

//**********************************************************************************************
//afficher les points "chauds" pour le feu
//**********************************************************************************************
void Put_Foy(void)
{
register cont,i;
unsigned short Co;
for(cont=0;cont<320;cont++)
	{
	Page[63360+cont]=(Rnd3[RndVar++]>>3)+(Rnd2[RndVar]>>3)+15;	//affiche une ligne de couleurs aleatoires pour renforecer l'effet
	}
for(cont=0;cont<3;cont++)
	{
	Co=63040+Rnd2[RndVar++]+30;	//l'affichage de rectangles se fait en bas de l'ecran
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

//**********************************************************************************************
//teste si le curseur de la souris se trouve sur un texte...
//**********************************************************************************************
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

//**********************************************************************************************
//Creer la liste des joueurs
//**********************************************************************************************
void Mk_Liste(void)
{
register cont,cont2,cont3=0,cont4;
for(cont2=0;cont2<Options.Nb_Joueurs;cont2++)
{
	cont4=0;
	for(cont=0;cont<12;cont++)
		{
		if(TeamListe[cont])
			{
			Liste[cont3]=cont*8+cont2;		//liste de l'ordre de jeu des mouton : permet de aux equipes de jouer chacun leur tour...(alterne chaque equipe)
			ListePos[cont2+cont4*Options.Nb_Joueurs]=cont*8+cont2;  //liste de la position des mouton : permet le placement par equipe (chaque mouton est mis avec les sien...)
			cont3++;
			cont4++;
			}
		}
	}
}

//**********************************************************************************************
//melanger la liste des joueurs
//**********************************************************************************************
void Rnd_Liste(void)
{
register  cont;
register unsigned char nb1,nb2,temp;

for(cont=0;cont<1000;cont++)	//interbertis deux joueurs pris au hasard...
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
for(cont=0;cont<16;cont++)	//initialise la position des objets dans le menu (les texte quoi...)
	{
	Obj[cont].X=Pos[cont<<2];
	Obj[cont].Y=Pos[(cont<<2)+1];
	Obj[cont].Lg=Pos[(cont<<2)+2];
	Obj[cont].Ht=Pos[(cont<<2)+3];
	}
/*Obj[0].X=90;	//ne sert plus pour l'instant...
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

void Drawy(bytef *src)	//affiche la page d'ecran s*mis a part les deux derniere ligne (ça fait plus beau : elimine les deffauts..)...
{
asm	{
	push ds
	les di,Video	//charge l'addresse video
	lds si,src	//...de la page...
	mov cx,15840	//compteur
	db 66h		//transfert permis par groupe de 4 octets
	rep movsw		//copier....
	pop ds
	}
}

//**********************************************************************************************
//Chrger les graphismes du menus...
//**********************************************************************************************
void Load_Graph_Menu(void)
{
register cont;
Clr(Video);
Clr(Page);
Load_Txt_Menu();
Init_Pal(Pal[2]);
SetAllPala(Pal[2]);
ReInitMouse();
ActiveMouse();
//MouseOFF();
ZoneMouse(1,1,317,196);
}


void FlamesAsm(bytef *src,bytef* dst)   //brule l'ecran(assembleur)
{
asm	{
	push ds                              //sauvegarde ds
	les di,dst                           //|mémorise les adresses des pages
	lds si,src                           //|
	mov ax,320					 //decalage d'une ligne (fait monter l'ecran...)
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

//**********************************************************************************************
//afficher le cursuer de la souris dans le menu...
//**********************************************************************************************
void Put_Cur(void)
{
Page[y*320+x]=(Rnd2[RndVar]>>3)+40;
Page[y*320+x+1]=(Rnd3[RndVar]>>3)+40;
}

//**********************************************************************************************
//fait disparaitres les textes du menu....
//**********************************************************************************************
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

//**********************************************************************************************
//Dessiner les texte du menu...)
//**********************************************************************************************
void Draw_Obj(unsigned short X,unsigned short Y,char *txt,char Nb,unsigned char Coul)
{
if(Cur_Obj==Nb)
	{
	Print_Txt(X+Rnd5[RndVar++],Y+Rnd6[RndVar&112]-1,txt,Page,Coul-16);
	Print_Txt(X+(Rnd5[RndVar++]>>1),Y+(Rnd6[RndVar&45]>>1),txt,Page,Coul+16);
	}
else
	{
	Print_Txt(X+Rnd6[RndVar--],Y+Rnd5[RndVar]-1,txt,Page,Coul-16);
	Print_Txt(X,Y,txt,Page,Coul);
	}
}

//**********************************************************************************************
//Dessiner le nombres du menu...)
//**********************************************************************************************
void Draw_Obj_Nb(unsigned short X,unsigned short Y,unsigned short N,char Nb,unsigned char coul)
{
if(Cur_Obj==Nb)
	{
	Print_Nb(X+Rnd5[RndVar++],Y+Rnd6[RndVar&112]-1,N,Page,coul-16);
	Print_Nb(X+(Rnd5[RndVar++]>>1),Y+(Rnd6[RndVar&45]>>1),N,Page,coul+16);
	}
else
	{
	Print_Nb(X+Rnd6[RndVar--],Y+Rnd5[RndVar]-1,N,Page,coul-16);
	Print_Nb(X,Y,N,Page,coul);
	}
}

//**********************************************************************************************
//Ecran d'options... : si tu arrive a optimiser la place du code, ça serrait tres utile (j'ai pas encore eu le temps de chercher...)
//**********************************************************************************************
void OptionsScreen(void)
{
register cont;
unsigned short Temp[10];
unsigned char b;
char JustPush=0;

for(cont=0;cont<4;cont++)	//position des objets
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

Temp[0]=Options.Pts_2_Vie;	//initialise les variables et leurs limites
Temp[1]=Options.Nb_Joueurs;
Temp[2]=Options.Nb_Plans;
Temp[3]=Niveau.Cur_Level+1;
Temp[4]=99;				///(limites)
Temp[5]=8;
Temp[6]=4;
Temp[7]=Niveau.Nb_Level;


while(GetMouse(&x,&y));		//attende la relache du bouton de la souris
while(!((Cur_Obj==8)&&b))	//tant que l'on ne quitte pas....
	{
	b=GetMouse(&x,&y);	//recuperer les coordonnes de la souris
	if(!b)
		JustPush=0;		//eviter l'icrementation excessive des options (attend la relache du bouton de la souris sans bloquer l'ecran graphique......)
	Cur_Obj=On_Obj(11);	//objet en surbrillance...
	Draw_Obj(Obj[2].X,Obj[2].Y-50,Txt[2],Cur_Obj,32);	//afficher les texte...
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
		if((Cur_Obj>15)&&(Cur_Obj<20))	//modifie les options....
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



//**********************************************************************************************
//Ecran des equipes : une majorite des commandes sont identiques a la procedure precedente je vais donc eviter de les annoter....
//**********************************************************************************************
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
			case 10 :	if(Debu<4)	//deplace la positon de la fenetre (des noms...)
					Debu++;
					break;
			case 9  :	if(Debu>0)
					Debu--;
					break;
			case 6  :	CurT++;			//change d'equipe..
					CurT%=12;
					break;
			case 7  :	CurT--;
					if(CurT<0)
					CurT=11;
					break;
			case 11 :
			case 12 :
			case 13 :
			case 14 :	Edit=1;	//editer les noms des mouton...
					CurLet=0;
					DebTxt=Mouton[CurT*8+Cur_Obj-11+Debu].Nom; //poiteur vers le tableau des noms (string)
					DebTxt[0]='\0';
					break;
			case 15 :	Edit=1;	//editer le nom de l'equipe...
					CurLet=0;
					DebTxt=Eq_Name[CurT];
					DebTxt[0]='\0';
					break;

			}
		if((Cur_Obj>5)&&(Cur_Obj<11))		//Reafficher les noms...
			{
			sprintf(temp[0],"Equipe %d",CurT+1);
			sprintf(temp[1],"Nom : %s",Eq_Name[CurT]);
			for(cont=0;cont<4;cont++)
				sprintf(temp[cont+2],"Mouton %d : %s",cont+1+Debu,Mouton[CurT*8+cont+Debu].Nom);
		 	}
		}
	if(!Edit)	//mode edition des noms???
		Put_Cur();		//non : afficher le curseurs de la souris
	else			//sinon, prendre la saisie au clavier....
		{
		if(kbhit())
			{
			ch=getch();
			switch(ch)
				{
				case 13 :	Edit=0;break;		//touche entree
				case 8 :	if(CurLet)		//retour...
							CurLet--;
						DebTxt[CurLet]='\0';
						break;
				default : 	if(CurLet<17)
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

//**********************************************************************************************
//Ecran de selectiondes equipes... //idem.......
//**********************************************************************************************
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
			case 10 :	if(Debut<6)
					Debut++;
					break;
			case 9  :	if(Debut>0)
					Debut--;
					break;
			case 16 :
			case 17 :
			case 18 :
			case 19 :
			case 20 :
			case 21 :	TeamListe[Cur_Obj-16+Debut]=!TeamListe[Cur_Obj-16+Debut];
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


//**********************************************************************************************
//Ecran de statistiques...
//**********************************************************************************************
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
	sprintf(temp[3],"Le plus violent : %s",Mouton[Stats.Mouton[2]].Nom);
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









//**********************************************************************************************
//											MAIN
//**********************************************************************************************
void main(void)
{
//unsigned long images=0;                   //nombre d'images enregistrés
unsigned short cont;                      //compteur
unsigned short b;
char fin=0;
clrscr();                                 //efface l'écran
randomize();

for(cont=0;cont<256;cont++)			//initialise le tableau avec les valeurs aleatoires
	{
	Rnd1[cont]=random(255);
	Rnd2[cont]=random(255);
	Rnd3[cont]=random(255);
	Rnd4[cont]=random(255);
	Rnd5[cont]=(random(255)-127)>>6;
	Rnd6[cont]=(random(255)-127)>>6;
	}
//printf("\nMemoire Libre :%lu octets\n",coreleft());
//Init_XMS();		//initialise la memire XMS...
//Init_Pages();
Lire_Jeu_Cfg();		//lire configuration du jeu
Nb_CD();			//Lit les info du cd (premier lecteur...)
Cur_Lect=Lect+CD.First;	//selection du lecteur
Get_T_Inf();		//prendre les info des pistes du cd (longueur, nombre, debut....)
printf("\nRecherche de CD Audio dans le lecteur %c: .....\n",Cur_Lect+65);
printf("\nMouton Chronicles Version %s Par :\n\tBaudin A. (Bart 57)\n\tJolivalt J.   (dJo)\n\tDri C.  (GnoMaster)\n\n\nSite officiel : www.ifrance.com/MChronicles",VERSION);
getch();                                  //Lit une touche
Init_Tab();                               //Initlise le tableau
Init_Mem();                               //Initalise la memoire
Mouton=farcalloc(96,46);			//memoire pour les strucuress mouton...
Lire_Joueur_Cfg();				//lire le noms des mouton, des equipes et leurs couleurs
Lire_Lang();					//memorise les phrases
ModeVGA();						//mode vga
delay(2000);					//attend que l'ecran ai finit de changer de resolution...
start();						//ecrans de demarages
Load_Graph_Menu();				//charger les graphismes
//setvect(0x1c,Vide);                       //enleve l'effet de clignottement
/*ModeVGA();
Init_Pal(Pal[2]);
SetAllPala(Pal[2]);*/
Init_Long();					//positon des objets
Let=1;
fcloseall();
while(!fin)
	{
	//Clry(Page);
	Put_Foy();
	b=GetMouse(&x,&y);
	Cur_Obj=On_Obj(0);
	Draw_Obj(Obj[0].X,Obj[0].Y,Txt[0],Cur_Obj,32);
	for(cont=1;cont<6;cont++)
		Draw_Obj(Obj[cont].X,Obj[cont].Y,Txt[cont],cont,32);
	if((Cur_Obj!=-1)&&b)		//tant que l'on ne quitte pas....
		{
		if(Is_Playing()&2)
			Play(3);
		switch(Cur_Obj)		//attendre la commande
			{
			case 1 :	BurnScreen();
					if(SelectScreen())
						{
						Mk_Liste();
						if(Options.Melange)
								Rnd_Liste();
						BurnScreen();
						for(cont=0;cont<4;cont++)
							LastExB[cont]=0;
						Bombe.Used=0;
						Vent=0;
						jouer();
						Play(3);
						Let=1;
						Load_Graph_Menu();
						StatsScreen();
						}
					break;
			case 2 :	BurnScreen();
					OptionsScreen();
//					if(Is_Playing()&2)	//fini de jouer la musique???
  //						Play(3);		//recommencer...
					break;
			case 3 :	BurnScreen();
					TeamScreen();
					break;
			case 4 :	BurnScreen();
					StatsScreen();
					break;
			case 5 :	BurnScreen();
					fin=1;
					break;
			}
		}
	Put_Cur();
	FlamesAsm(Page,Page);
	Bal();
	Drawy(Page);
	}
Ecrire_Jeu_Cfg();		//sauvegardes les fichier configurations..
Ecrire_Joueur_Cfg();
Stop();
ModeTxt();			//mode texte...
}




//2684 lignes de code !!! 65.5 Ko!!!
//  ->voir LigneCnt.exe
