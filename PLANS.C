//**********************************************************************************************
//Creation du 3eme plan
//**********************************************************************************************
void Gen_Back(void)
{
unsigned char cont;
for(cont=0;cont<200;cont++)
	{
	Stars.X[cont]=random(320);
	Stars.Y[cont]=random(190);
	Stars.Coul[cont]=random(16)+1;
	}
}



//**********************************************************************************************
//Affichage du 3eme plan
//**********************************************************************************************
void Draw_Back(unsigned char * Page)
{
unsigned char cont;
for(cont=0;cont<200;cont++)
	Page[Y[Stars.Y[cont]]+Stars.X[cont]]=Stars.Coul[cont];
Put(270,20,24,24,Page,Lune);
//Put(258,25,24,24,Page,Trou);
}

void Draw_Deg(unsigned char * Page,unsigned char coul)
{
unsigned char cont;
asm{
push es
push ds

les di,Page
mov ah,coul
mov bx,14h
loop1:
mov cx,0c80h
loop2:
mov es:[di],ah
inc di
dec cx
or cx,cx
jnz loop2
inc ah
dec bx
or bx,bx
jnz loop1

pop es
pop ds
}
Put(270,20,24,24,Page,Lune);
}


//**********************************************************************************************
//Copie un des plan dans la page selectionné
//**********************************************************************************************
void CpyPlan(unsigned short x,unsigned short y,unsigned char **Plan,unsigned char *Page)
{
register char p;
p=x/320;    //choix de la page selon l'ordonné
x%=320;     //ordonné comprise entre 0 et 320
if(x&&y)
	{
	Blka(x,0,320-x,y,0,200-y,Plan[p],Page); //copie d'un block avec transparence
	Blka(0,0,x,y,320-x,200-y,Plan[p+1],Page);
	}
else
	Blka(0,0,320,y,0,200-y,Plan[p],Page);
}



//**********************************************************************************************
//Initialise l'explosion d'une bombe
//**********************************************************************************************
void Init_Explose(unsigned short debx,unsigned short deby,char nb,unsigned char coul)
{
register cont;
float a,vit;
Explose[nb].StartX=debx;
Explose[nb].StartY=deby;
Explose[nb].Used=200;
LastExB[nb]=1;
for(cont=0;cont<100;cont++)
	{
	a=(float)(random(3.14*200))/150-0.785;        //3/4 de cerlce pour les debris
	vit=(float)(random(80)+35)/50;                //vitesse des debris
	Explose[nb].X[cont]=0;
	Explose[nb].Y[cont]=0;
	Explose[nb].DirX[cont]=(cos(a))*vit;
	Explose[nb].DirY[cont]=(sin(a))*vit;
	Explose[nb].Coul[cont]=random(8)+coul;
	}
}


void Look_Obj(unsigned short sx,unsigned short sy,unsigned short *dx,unsigned short *dy)
{
if(sx>TX-160)
	sx=TX-160;
else if((int)sx<160)
	sx=160;
if(sy>200)
	sy=200;
else if(	sy<100)
	sy=100;
*dx=sx-160;
*dy=300-sy;
}

//**********************************************************************************************
//"Incruste" les débris dans le premier plan
//**********************************************************************************************
void End_Bomb(char nb)
{
register cont;
register unsigned short Xa,Ya;
LastExB[nb]=0;
for(cont=0;cont<100;cont++)
	{
	Xa=Explose[nb].X[cont]+Explose[nb].StartX;  //pour simplifier la formule ci dussous
	Ya=Explose[nb].Y[cont]+Explose[nb].StartY;
	if((Ya<200)&&(Xa<TX)&&Ya>1)   //si le débris est dans le plan...
		Plan_1[Xa/320][Y[200-Ya]+(Xa%320)]=Explose[nb].Coul[cont];  //il faut l'afficher
	}
}




//**********************************************************************************************
//Calcul de l'explosion
//**********************************************************************************************
void Bomb_Ex(char nb)
{
register cont;
register unsigned short Xa,Ya;
Explose[nb].Used--;
for(cont=0;cont<100;cont++)
	{
	Explose[nb].DirY[cont]-=0.049;    //Attraction terrestre 9.81/2
	Explose[nb].X[cont]+=Explose[nb].DirX[cont];   //deplacement de chaque débris
	Explose[nb].Y[cont]+=Explose[nb].DirY[cont];
	Xa=Explose[nb].X[cont]+Explose[nb].StartX;     //simplifie les formules
	Ya=Explose[nb].Y[cont]+Explose[nb].StartY;
	if((Ya>400)||(Xa>=TX))                   //hors du plan?
		{
		continue;
		}
	else 	if (( Plan_1[Xa/320][Y[200-Ya]+(Xa%320)])&&(Ya<200)) //sinon ,si obstacle
		{
		Explose[nb].X[cont]-=Explose[nb].DirX[cont];    //rebondir
		Explose[nb].Y[cont]-=Explose[nb].DirY[cont];
		Explose[nb].DirX[cont]=-Explose[nb].DirX[cont]/2;
		Explose[nb].DirY[cont]=-Explose[nb].DirY[cont]/(Explose[nb].Coul[cont]/2);
		}
	if((Xa>x)&&(Xa<(x+320))&&(Ya>200-y)&&(Ya<400-y))  //si dans l'écran...
		Page[(Y[400-y-Ya-1]+(Xa-x))]=Explose[nb].Coul[cont];  //...afficher
	}
}


void Print_Barre(unsigned short x,unsigned short y,short longueur,short haut,char pourcent,unsigned char coul1,unsigned char coul2,char sens)
{
register i,j;
unsigned char nb;
nb=longueur*pourcent/100;
if(sens==1)
	{
	for(j=0;j<haut;j++)
		{
		for(i=0;i<nb;i++)
			Page[Y[j+y]+i+x]=coul1;
		}
	}
else
	{
	for(j=0;j<haut;j++)
		{
		for(i=0;i<nb;i++)
			Page[Y[j+y]+longueur-i-1+x]=coul1;
		}
	}
Box(x,y,longueur,haut,Page,coul2+30);
}



