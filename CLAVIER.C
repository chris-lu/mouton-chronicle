void Install_Clav(void)
{
oldit_9=getvect(0x9);
setvect(0x9,Lire_scan);
}

void Remove_Clav(void)
{
setvect(0x9,oldit_9);
}

void far interrupt Lire_scan(void)
{
register char c;
c=inportb(0x60);
if (c<0)
	Scan_Code[Scan_Code[0]=(c&0x7f)]=0;
else
	Scan_Code[c]=1;

outportb(0x20,0x20);
}

void Lire_Keys(char nb)
{
register short cont,temp;
Lire_scan();
if(Mouton[nb].Can_Move)
	{
	if(Scan_Code[0]!=Last_Key)
		{
		Last_Key=Scan_Code[0];
		Deb_Tour=0;
		}
	if (Scan_Code[77])
		{
		Mouton[nb].Sens=1;
		Mouton[nb].Mouv++;
		Mouton[nb].Mouv&=15;
		Mouton[nb].Cur_Spr=Mouton[nb].Mouv>>1;
		Mouton[nb].PosX+=0.4;
		Move_U(nb);
		if (Mouton[nb].Bloque)
			Mouton[nb].PosX-=0.5;
		}
	else
		Mouton[nb].Cur_Spr=0;
	if (Scan_Code[75])
		{
		Mouton[nb].Sens=-1;
		Mouton[nb].Mouv++;
		Mouton[nb].Mouv&=15;
		Mouton[nb].Cur_Spr=Mouton[nb].Mouv>>1;
		Mouton[nb].PosX-=0.4;
		Move_U(nb);
		if (Mouton[nb].Bloque)
				Mouton[nb].PosX+=0.5;
		}
	if (Scan_Code[28])
		{
		Mouton[nb].Can_Move=0;
		Mouton[nb].PosY++;
		Mouton[nb].DirX=.6*Mouton[nb].Sens;               //72
		Mouton[nb].DirY=2;                                   //80
		}
	if (Scan_Code[80])
		{
		if(Vise>0)
		Vise-=1;
		}
	if (Scan_Code[72])
		{
		if(Vise<253)
			Vise+=1;
		}
	if (Scan_Code[57]&&(!Bombe.Used))
		{
		Last_Key=0;
		if(Puissance<6.5)
			{
			Puissance+=0.08;
			}
		else  Scan_Code[57]=0;
			Tir=1;
		}
	if(!Scan_Code[57]&&Tir)
		{
		Init_Bomb();
		Mouton[nb].Can_Move=0;
		Puissance=0;
		Tir=0;
		}
	if(Scan_Code[59])
		{
		Deb_Tour=!Deb_Tour;
		while(Scan_Code[59])
			Lire_scan();
		Last_Key=Scan_Code[0];
		}
	if(Scan_Code[60])
		{
		SetMouse(Mouton[nb].PosX-160,300-Mouton[nb].PosY);
		}
	}
}
