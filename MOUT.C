

void Test_Dom(void)
{
register cont;
long temp,temp2,temp3;

for(cont=0;cont<Nb_Joueurs;cont++)
{
temp=Mouton[cont].PosX-Bombe.PosX;
temp2=Mouton[cont].PosY+4-Bombe.PosY;
temp3=sqrt(temp2*temp2+temp*temp);
if((temp<25&&temp>-25)&&(temp2<25&&temp2>-25))
	if(temp3&&temp2&&temp)
	{
	Mouton[cont].DirX=2*temp/temp3;
	Mouton[cont].DirY=2*temp2/temp3;

}
}
}






void Move_U(char nb)
{
unsigned short cont=0;
Mouton[nb].Bloque=0;

if ((Plan_1[Mouton[nb].PosX/320][Y[201-Mouton[nb].PosY]+(int)Mouton[nb].PosX%320])&&(Mouton[nb].PosY<200))
  {
		  cont=0;
		  while((Plan_1[Mouton[nb].PosX/320][Y[201-Mouton[nb].PosY-cont]+(int)Mouton[nb].PosX%320]))
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
register cont;
short Xa,Ya;
/*for(cont=0;cont<8;cont++)
	Plan_1[Mouton[nb].PosX/320][Y[200-Mouton[nb].PosY-cont]+(int)Mouton[nb].PosX%320]=255;
*/
Xa=Mouton[nb].PosX;
Ya=Mouton[nb].PosY;
for(cont=0;cont<8;cont++)
	if((Xa>x)&&(Xa<(x+320))&&(Ya+cont>200-y)&&(Ya+cont<400-y))  //si dans l'écran...
		Page[(Y[400-y-Ya-cont]+(Xa-x))]=153;  //...afficher

}

void Remove_Mouton(char nb)
{
register cont;
for(cont=0;cont<8;cont++)
	Plan_1[Mouton[nb].PosX/320][Y[200-Mouton[nb].PosY-cont]+(int)Mouton[nb].PosX%320]=0;
}


void Move_Mouton(char nb)
{
unsigned short Xa=Mouton[nb].PosX;
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
//		if(!Bombe.Explosion)
		Mouton[nb].Can_Move=1;
		Mouton[nb].DirY=-Mouton[nb].DirY/5;
		Mouton[nb].DirX/=2;
		}
Mouton[nb].PosY+=Mouton[nb].DirY;

Mouton[nb].PosX+=Mouton[nb].DirX;
Move_U(nb);
if (Mouton[nb].Bloque)
Mouton[nb].PosX+=Mouton[nb].DirX;



}


void Put_Viseur(char nb)
{
short Xa,Ya;
Xa=Mouton[nb].PosX+CosT[Vise]*10*Mouton[nb].Sens;
Ya=Mouton[nb].PosY+SinT[Vise]*10+4;
if((Xa>x)&&(Xa<(x+320))&&(Ya>200-y)&&(Ya<400-y))  //si dans l'écran...
		Page[(Y[400-y-Ya]+(Xa-x))]=255;  //...afficher
}

