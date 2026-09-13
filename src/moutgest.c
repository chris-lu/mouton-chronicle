//**********************************************************************************************
//Initialise un message...
//**********************************************************************************************
void Message(char type,char modif,char Jou)
{
/*
Messages :
0 : Quand on tire a cote 
1 : Quand on s'écrase lamentablement au sol 
2 : Quand on se fait toucher 
3 : Quand on touche 
4 : Quand on se fait toucher par un coequipier 
5 : Quand on touche un coequipier 
6 : Quand on se tire dessus 
7 : Quand on se fait tuer 
8 : Quand on tue 
9 : Quand on se fait tuer par un coequipier 
10: Quand on tue un coequiper 
11: Quand on se suicide
12: Quand on tombe 
13: Quand on sort de la zone d'hostilites 
*/
if(!Ecl)	//message si il n'y a pas de colere divine en cour (car les messages defilent trop vite, on ne peut les voir donc c'est inutile....)
	{
	if(Mouton[CURJ].Mort)
	  modif=0;
	type+=modif;	//type de message + modificateur : le modif est si la phrase est dite par un adversaire ou par soit meme...
	if(modif)
		Par[Cur_Par][0]=CURJ;	//joueur parlant...
	else
		Par[Cur_Par][0]=Jou;
	Par[Cur_Par][1]=random(5)+5*type;	//message aleatoire
	Par[Cur_Par++][2]=500;			//temps du message : 500 images.
	if(Cur_Par>3)				//trop de phrase???
		{
		Txt_Down();				//en enlever une.....
		Cur_Par=3;
		}
	}
}


//**********************************************************************************************
//Teste les dommages sur les moutons
//**********************************************************************************************
void Test_Dom(unsigned char MaxDom)
{
register cont;
long temp,temp2,temp3;
unsigned short stat1,stat2;
unsigned long stat3,stat4;
unsigned char touch=0;
short dommages;
char rnd;

for(cont=0;cont<Options.Nb_J_Tot;cont++)
	{
	if(!Mouton[Liste[cont]].Mort)//si le mouton n'est pas mort...
		{
		temp=Mouton[Liste[cont]].PosX-Bombe.PosX+4;	//distance sur l'axe X 
		temp2=Mouton[Liste[cont]].PosY-Bombe.PosY+4;	//distance sur l'axe Y
		temp3=sqrt(temp2*temp2+temp*temp);			//Distance reele
		if(temp3<=(MaxDom))	//dans la zone d'effet : essayer avec temp3<MaxDOm....
			{
			rnd=random(4)&1;		//Modif pour les messages : 0 ou 1
			touch=1;			//touche quelqu'un...
			dommages=MaxDom-temp3;
			if(dommages<1)		//dommages minimums : 1
				dommages=1;
			Mouton[CURJ].Degats+=dommages;	//aujoute les dommages au parlmares du mouton (plus violant pour les stats...)
			if(Arme!=4)
				{
				stat1=Mouton[CURJ].PosX-Bombe.PosX+4;	//Position de la bombe par rapport au joueur....
				stat2=Mouton[CURJ].PosY-Bombe.PosY+4;
				stat4=sqrt(stat2*stat2+stat1*stat1);
				stat3=(unsigned long)stat4*dommages/MaxDom;	//Statistiques : plus precis (plus les degats sont important et plus le tir est loin, plus il y a de points...)
				if (stat3>Stats.Precis)
					{
					Stats.Precis=stat3;
					Stats.Mouton[1]=CURJ;
					}
				}
			if((Mouton[Liste[cont]].Pts_Vie-=dommages)<1)	//Un mouton de tué?
				{
				if(Cur_Joueur!=cont)
					Mouton[CURJ].Frags++;			//ajouter un frag au mouton s'il ne se suicide pas
				else
					Mouton[CURJ].Frags--;			//sinon lui retirer un frag
				Mouton[Liste[cont]].Mort=1;			//Le mouton est mort
				if(cont==Cur_Joueur)				//Suicide.....
					Message(11,0,Liste[cont]);
				else if(Mouton[Liste[cont]].Equipe==Mouton[CURJ].Equipe)	//un mort dans la meme equipe...
					Message(9,rnd,Liste[cont]);
				 else										//sinon un mort normal...
					Message(7,rnd,Liste[cont]);
				Init_Explose(Mouton[Liste[cont]].PosX,Mouton[Liste[cont]].PosY+4,64);//creer l'explosion
				}
			else
				{
				if(cont==Cur_Joueur)	//Le mouton se tire dessus???
					Message(6,0,Liste[cont]);
				else if(Mouton[Liste[cont]].Equipe==Mouton[CURJ].Equipe)	//ou tire sur un coequipier??
					Message(4,rnd,Liste[cont]);
				else								//Sinon : c'est un adversaire normal...
					Message(2,rnd,Liste[cont]);
				}
			Mouton[Liste[cont]].Bouge++;		//le mouton ne touche plus le sol pendant un instant...(pour etre propulse...)
			if(temp)			//le mouton est projeté....
				Mouton[Liste[cont]].DirX+=(dommages/12)*(temp/abs(temp));
			if(temp2)
				Mouton[Liste[cont]].DirY+=(dommages/5)*(temp2/abs(temp2));
			if(Mouton[Liste[cont]].DirX>5)		//limite la propultion du mouton
				Mouton[Liste[cont]].DirX=5;
			else 	if(Mouton[Liste[cont]].DirX<-5)
				Mouton[Liste[cont]].DirX=-5;
			if(Mouton[Liste[cont]].DirY>5)
				Mouton[Liste[cont]].DirY=5;
			else 	if(Mouton[Liste[cont]].DirY<-5)
				Mouton[Liste[cont]].DirY=-5;
			}
		}
	}
if(!touch&&Arme==0)		//si ne touche pas avec le bazooka...
	{
	Message(0,0,CURJ);	// message 0
	}
}





//**********************************************************************************************
//place le mouton sur le terrain (eviter qu'il se balade dans le decor...) et teste s'il est bloqué...
//**********************************************************************************************
void Move_U(char nb)
{
register unsigned short cont=0;
Mouton[nb].Bloque=0;
if ((Plan_1[(int)((Mouton[nb].PosX+4)/320)][Y[(int)(201-Mouton[nb].PosY)]+(int)(Mouton[nb].PosX+4)%320])&&(Mouton[nb].PosY<200))
	{
	while(((Plan_1[(int)((Mouton[nb].PosX+4)/320)][Y[(int)(201-Mouton[nb].PosY-cont)]+(int)(Mouton[nb].PosX+4)%320]))&&cont<5)
		cont++;
	if (cont<4)
		Mouton[nb].PosY+=cont;
	else
		Mouton[nb].Bloque=1;
	}
}

//**********************************************************************************************
//Initialise la positon initiale du mouton....
//**********************************************************************************************
void Put_Mouton(unsigned short x,char nb)
{
register unsigned char cont,temp;
Mouton[nb].PosX=x;
x+=4;
temp=x/320;
x%=320;
cont=1;
while(!(Plan_1[temp][Y[cont]+x]))
	cont++;
Mouton[nb].Bouge=0;
Mouton[nb].PosY=204-cont;
Mouton[nb].DirX=0;
Mouton[nb].DirY=0;

}

//**********************************************************************************************
//Affiche un mouton....
//**********************************************************************************************
void Draw_Mouton(char nb)
{
register short cont;
short Xa,Ya;
/*for(cont=0;cont<8;cont++)
	Plan_1[Mouton[nb].PosX/320][Y[200-Mouton[nb].PosY-cont]+(int)Mouton[nb].PosX%320]=255;
  */
Xa=Mouton[nb].PosX;
Ya=Mouton[nb].PosY;
/*for(cont=0;cont<8;cont++)
	if((Xa>x)&&(Xa<(x+320))&&(Ya+cont>200-y)&&(Ya+cont<400-y))  //si dans l'écran...
		Page[(Y[400-y-Ya-cont]+(Xa-x))]=153;  //...afficher
 */
if((Mouton[nb].Bouge>6)||(nb!=CURJ))           //le mouton se deplace dans les aires???	//ENLEVER ||nb!=CURJ ||et c'est tout rigolo!!!
	{
	if(Mouton[nb].DirY>1)			//sprite en fonction de la vitesse de deplacement...
		Mouton[nb].Cur_Spr=2;
	else if (Mouton[nb].DirY>.3)
		Mouton[nb].Cur_Spr=1;
	else if (Mouton[nb].DirY>-.3)
		Mouton[nb].Cur_Spr=0;
	else if (Mouton[nb].DirY>-1)
		Mouton[nb].Cur_Spr=7;
	else
		Mouton[nb].Cur_Spr=6;
	}
else if(Mouton[nb].Bouge)	//si le mouton se deplace normalement....
	{
	Mouton[nb].Mouv++;
	Mouton[nb].Mouv&=15;
	Mouton[nb].Cur_Spr=Mouton[nb].Mouv>>1;
	}
if((Xa>x)&&(Xa<(x+312))&&(Ya>200-y)&&(Ya<392-y))  //si dans l'écran...
	{
	if(Mouton[nb].Sens==-1)	//afficher le mouton en fonction de son sens....
		Put(Xa-x,400-y-Ya-6,8,8,Page,Mout_Spr[0+Mouton[nb].Cur_Spr]);
	else
		Put(Xa-x,400-y-Ya-6,8,8,Page,Mout_Spr[8+Mouton[nb].Cur_Spr]);
	}
}



/*void Remove_Mouton(char nb)
{
register cont;
for(cont=0;cont<8;cont++)
	Plan_1[Mouton[nb].PosX/320][Y[200-Mouton[nb].PosY-cont]+(int)Mouton[nb].PosX%320]=0;
}*/

void Test_Tombe(char nb)
{
	if(Mouton[nb].DirY<-3)	//si il vas trop vite en redescendant, il se fait mal et perd son tour..
		{
		if(nb==CURJ)
			{
			Puissance=0;
			Tir=0;
			Wait=200;
			}
		if((Mouton[nb].Pts_Vie+=Mouton[nb].DirY)<1)	//il peut aussi mourrir...
			{
			Mouton[nb].Frags--;
			Mouton[nb].Mort=1;
			Init_Explose(Mouton[nb].PosX,Mouton[nb].PosY+4,64);//creer l'explosion
			Message(1,0,nb);		//message 1 : il s'ecrase...
			}
		else
			Message(12,0,nb);		//il se fait mal et gemit..
		}
	if(!Mouton[nb].Bloque)	//si il retombe....
		{
		Add_Vent=0;
		Mouton[nb].Bouge=0;
		Mouton[nb].DirY=-Mouton[nb].DirY/3;
		Mouton[nb].DirX/=2;
		}
}







//**********************************************************************************************
//Deplace le mouton....
//**********************************************************************************************
void Move_Mouton(char nb)
{
unsigned short Xa=Mouton[nb].PosX+4;	//position du mouton....
unsigned short Ya=Mouton[nb].PosY-1;

if (!(Plan_1[Xa/320][Y[200-Ya]+(Xa%320)])||(Mouton[nb].PosY-1>200))	//mouton dans les aires??
	{
	//Mouton[nb].Can_Move=0;
	Mouton[nb].DirY-=Gravite;    //Attraction terrestre 9.81/2
	Add_Vent=Vent/4;         //poussé par le vent...
	if(!Mouton[nb].Bouge)	//evite que le mouton s'arrete au moindre denivelé....
		{
		Mouton[nb].DirX=0.4*Mouton[nb].Sens+Add_Vent;
		Mouton[nb].Bouge++;
		}
	else				//le faire bouger dans les aires
		{
		Mouton[nb].DirX+=Add_Vent;
		Mouton[nb].Bouge++;
		}
	}
else
	{
	Test_Tombe(nb);
	}
Mouton[nb].PosY+=Mouton[nb].DirY;
Mouton[nb].PosX+=Mouton[nb].DirX;
Move_U(nb);		//replace le mouton correctement sur le terrain....
if (Mouton[nb].Bloque)	//et s'il se prend un mur
	{
	Mouton[nb].PosY=Ya+1;
	Mouton[nb].PosX=Xa-4-Mouton[nb].DirX*2;     //evite de se coincer dans un mur....
	Test_Tombe(nb);
	Mouton[nb].DirY=0;
	Mouton[nb].DirX=0;			//ajouter si DirX>3 : degats....
	Move_U(nb);		//replace le mouton correctement sur le terrain....
	}
}


//**********************************************************************************************
//Afficher le viseur...
//**********************************************************************************************
void Put_Viseur(char nb)
{
unsigned short Xa,Ya;
Xa=Mouton[nb].PosX+CosT[Vise]*20*Mouton[nb].Sens+4;
Ya=Mouton[nb].PosY+SinT[Vise]*20+2;
if((Xa>x+1)&&(Xa<(x+319))&&(Ya>201-y)&&(Ya<399-y))  //si dans l'écran...
	{
	Ya=(Y[400-y-Ya]+(Xa-x));
	Page[Ya]=244+Mouton[nb].Equipe;  //...afficher
	Page[Ya+1]=244+Mouton[nb].Equipe;  //...afficher
	Page[Ya-1]=244+Mouton[nb].Equipe;  //...afficher
	Page[Ya+320]=244+Mouton[nb].Equipe;  //...afficher
	Page[Ya-320]=244+Mouton[nb].Equipe;  //...afficher
	}
}

//**********************************************************************************************
//Equipe gagnante..
//**********************************************************************************************
char Gagnant(void)
{
register char cont=0;
register char last=-1;
	while((cont<(Options.Nb_J_Tot-1))&&Mouton[Liste[cont]].Mort)
	cont++;
	if((cont==(Options.Nb_J_Tot-1))&&Mouton[Liste[cont]].Mort)
		{
		return(-1);
		}
	last=Mouton[Liste[cont]].Equipe;
	for (cont=0;cont<Options.Nb_J_Tot;cont++)
		if(!Mouton[Liste[cont]].Mort)
			if(Mouton[Liste[cont]].Equipe!=last)
				return(0);
	return(last+1);

}

//**********************************************************************************************
//Calcule les statistiques...
//**********************************************************************************************
void Mk_Stats(void)
{
register cont;
long Total;
Stats.Bourrin=0;
Stats.Meilleur=0;
Stats.Inutile=125;
for(cont=0;cont<Options.Nb_J_Tot;cont++)
	{
	if (Mouton[Liste[cont]].Frags>Stats.Meilleur)
		{
		Stats.Mouton[0]=Liste[cont];
		Stats.Meilleur=Mouton[Liste[cont]].Frags;
		}
	if (Mouton[Liste[cont]].Degats>Stats.Bourrin)
		{
		Stats.Mouton[2]=Liste[cont];
		Stats.Bourrin=Mouton[Liste[cont]].Degats;
		}
	Total=Mouton[Liste[cont]].Frags*Options.Pts_2_Vie+Mouton[Liste[cont]].Degats;
	if (Total<Stats.Inutile)
		{
		Stats.Mouton[3]=Liste[cont];
		Stats.Inutile=Total;
		}
	}
}

