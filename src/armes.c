 
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
		if(Plan_1[(int)((Bombe.PosX+cont*Signe)/320)][Y[(int)(200-(Bombe.PosY+cont*temp))]+(int)(Bombe.PosX+cont*Signe)%320])
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
	Page[(unsigned)(Y[(int)(400-y-Bombe.PosY)]+(Bombe.PosX-x))]=255;                                //la dessiner
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
	Page[(unsigned)(Y[(int)(400-y-Bombe.PosY)]+(Bombe.PosX-x))]=255;                                //la dessiner
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
