void Gen_Back(void)
{
 unsigned char cont;
for(cont=0;cont<200;cont++)
	{
	Stars.X[cont]=random(320);
	Stars.Y[cont]=random(190);
	Stars.Coul[cont]=random(32)+32;
	}
}

void Draw_Back(unsigned char * Page)
{
unsigned char cont;
for(cont=0;cont<200;cont++)
	{
	Page[Y[Stars.Y[cont]]+Stars.X[cont]]=Stars.Coul[cont];
	}
Put(270,20,24,24,Page,Lune);
//Put(258,25,24,24,Page,Trou);
}





void CpyPlan(unsigned short x,unsigned short y,unsigned char **Plan,unsigned char *Page)
{
register p,cont;
p=x/320;    //attention a l'arrondi!!!
x%=320;
if(x&&y)
{
/*Blka(x,320-x,0,Plan[p],Page);
Blka(0,x,320-x,Plan[p+1],Page);
}
else
Blka(0,320,0,Plan[p],Page); */
Blka(x,0,320-x,y,0,200-y,Plan[p],Page);
Blka(0,0,x,y,320-x,200-y,Plan[p+1],Page);
}
else if(!x&&y)
Blka(0,0,320,y,0,200-y,Plan[p],Page);
else
Blka(0,0,320,y,0,200-y,Plan[p],Page);

}

void Init_Explose(unsigned short debx,unsigned short deby,char nb)
{
register cont;
float a,vit;
Explose[nb].StartX=debx;
Explose[nb].StartY=deby;
Explose[nb].Used=255;
Explose[nb].Used=255;
LastExB[nb]=1;
for(cont=0;cont<100;cont++)
{
a=(float)(random(3.14*200))/100;
vit=(float)(random(80)+30)/50;
Explose[nb].X[cont]=0;
Explose[nb].Y[cont]=0;
Explose[nb].DirX[cont]=(cos(a))*vit;
Explose[nb].DirY[cont]=(sin(a))*vit;
Explose[nb].Coul[cont]=random(7)+25;
}
}

void End_Bomb(char nb)
{
register cont;
register short Xa,Ya;
LastExB[nb]=0;
for(cont=0;cont<100;cont++)
{
Xa=Explose[nb].X[cont]+Explose[nb].StartX;
Ya=Explose[nb].Y[cont]+Explose[nb].StartY;
   if((Ya<200)&&(Ya>0)&&(Xa>0)&&(Xa<960))
	Plan_1[Xa/320][Y[200-Ya]+(Xa%320)]=Explose[nb].Coul[cont];
}
}

void Bomb_Ex(char nb)
{
register cont;
register short Xa,Ya;
Explose[nb].Used--;
for(cont=0;cont<100;cont++)
	{
	Explose[nb].DirY[cont]-=0.04;
	Explose[nb].X[cont]+=Explose[nb].DirX[cont];
	Explose[nb].Y[cont]+=Explose[nb].DirY[cont];
	Xa=Explose[nb].X[cont]+Explose[nb].StartX;
	Ya=Explose[nb].Y[cont]+Explose[nb].StartY;
	if ( Plan_1[Xa/320][Y[200-Ya]+(Xa%320)])
		{
		Explose[nb].X[cont]-=Explose[nb].DirX[cont];
		Explose[nb].Y[cont]-=Explose[nb].DirY[cont];
		Explose[nb].DirX[cont]=-Explose[nb].DirX[cont]/2;
		Explose[nb].DirY[cont]=-Explose[nb].DirY[cont]/(Explose[nb].Coul[cont]/3);
		}

	if((Xa>x)&&(Xa<(x+320))&&(Ya>200-y)&&(Ya<400-y))
		Page[(Y[400-y-Ya-1]+(Xa-x))]=Explose[nb].Coul[cont];


	}
}


