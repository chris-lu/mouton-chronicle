


void Test_Dom(void)
{
register cont;
long temp,temp2,temp3;
short dommages=0;

for(cont=0;cont<Options.Nb_Joueurs;cont++)
{
if(!Mouton[cont].Mort)
{
temp=Mouton[cont].PosX-Bombe.PosX+4;
temp2=Mouton[cont].PosY-Bombe.PosY+4;
temp3=sqrt(temp2*temp2+temp*temp);
if((temp<25&&temp>-25)&&(temp2<25&&temp2>-25))
	{
	{
	dommages=24-temp3;
	if((Mouton[cont].Pts_Vie-=dommages)<0)
		{
		Mouton[cont].Mort=1;
		Init_Explose(Mouton[cont].PosX,Mouton[cont].PosY+4,CurBomb,64);//creer l'explosion
		CurBomb++;                               //change le numero de la bombe
		CurBomb%=3;
		}
	if(temp)
	Mouton[cont].DirX=(dommages/2)*(temp/abs(temp));
	if(temp2)
	Mouton[cont].DirY=(dommages/5)*(temp2/abs(temp2));
	if((Mouton[cont].DirY>6)||(Mouton[cont].DirY<-6))
		Mouton[cont].DirY=0;
	}
	}
}
}
}






void Move_U(char nb)
{
unsigned short cont=0;
Mouton[nb].Bloque=0;

if ((Plan_1[(Mouton[nb].PosX+4)/320][Y[201-Mouton[nb].PosY]+(int)(Mouton[nb].PosX+4)%320])&&(Mouton[nb].PosY<200))
  {
		  cont=0;
		  while((Plan_1[(Mouton[nb].PosX+4)/320][Y[201-Mouton[nb].PosY-cont]+(int)(Mouton[nb].PosX+4)%320]))
				  cont++;
		  if (cont<4)
					{
					Mouton[nb].Bloque=0;
					Mouton[nb].PosY+=cont;
					}
				else
				Mouton[nb].Bloque=1;

		}
}

void Put_Mouton(unsigned short x,char nb)
{
unsigned char cont,temp;
Mouton[nb].PosX=x;
temp=x/320;
x%=320;
cont=1;
while(!(Plan_1[temp][Y[cont]+x]))
	cont++;
Mouton[nb].Can_Move=1;
Mouton[nb].PosY=200-cont;
}

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
if(!Mouton[nb].Can_Move||(nb!=Cur_Joueur))
if(Mouton[nb].DirY>1)
	Mouton[nb].Cur_Spr=2;
	else if (Mouton[nb].DirY>.3)
	Mouton[nb].Cur_Spr=1;
	else if (Mouton[nb].DirY>-.3)
	Mouton[nb].Cur_Spr=0;
	else if (Mouton[nb].DirY>-1)
	Mouton[nb].Cur_Spr=7;
		else
	Mouton[nb].Cur_Spr=6;





if((Xa>x)&&(Xa<(x+320))&&(Ya>200-y)&&(Ya<400-y))  //si dans l'écran...
	{
	if(Mouton[nb].Sens==-1)
	Put(Xa-x,400-y-Ya-6,8,8,Page,Mout_Spr[0+Mouton[nb].Cur_Spr]);
	else
	Put(Xa-x,400-y-Ya-6,8,8,Page,Mout_Spr[8+Mouton[nb].Cur_Spr]);
	}

 }

void Remove_Mouton(char nb)
{
register cont;
for(cont=0;cont<8;cont++)
	Plan_1[Mouton[nb].PosX/320][Y[200-Mouton[nb].PosY-cont]+(int)Mouton[nb].PosX%320]=0;
}


void Move_Mouton(char nb)
{
unsigned short Xa=Mouton[nb].PosX+4;
unsigned short Ya=Mouton[nb].PosY-1;


if (!(Plan_1[Xa/320][Y[200-Ya]+(Xa%320)])||(Mouton[nb].PosY-1>200))
	{
	Mouton[nb].Can_Move=0;
	Mouton[nb].DirY-=0.049;    //Attraction terrestre 9.81/2
	Add_Vent+=Vent/5;         //Masse du mouton : 5
	Mouton[nb].DirX=0.4*Mouton[nb].Sens+Add_Vent;


	//Mouton[nb].X[cont]-=Mouton[nb].DirX[cont];    //rebondir
  //	Mouton[nb].PosY+=Mouton[nb].DirY;
/*	//Mouton[nb].DirX[cont]=-Mouton[nb].DirX[cont]/2;
	if (Mouton[nb].DirY>0.005)
		Mouton[nb].DirY[cont]=-Mouton[nb].DirY[cont]/10);  */
	}
	 else
		{
		Add_Vent=0;
		Mouton[nb].Can_Move=1;
		Mouton[nb].DirY=-Mouton[nb].DirY/4;
		Mouton[nb].DirX/=2;
		}
Mouton[nb].PosY+=Mouton[nb].DirY;
Mouton[nb].PosX+=Mouton[nb].DirX;
Move_U(nb);
/*if (Mouton[nb].Bloque)
Mouton[nb].PosX+=Mouton[nb].DirX;    */



}


void Put_Viseur(char nb)
{
short Xa,Ya;
Xa=Mouton[nb].PosX+CosT[Vise]*10*Mouton[nb].Sens+4;
Ya=Mouton[nb].PosY+SinT[Vise]*10+3;
if((Xa>x)&&(Xa<(x+320))&&(Ya>200-y)&&(Ya<400-y))  //si dans l'écran...
		Page[(Y[400-y-Ya]+(Xa-x))]=48;  //...afficher
}

