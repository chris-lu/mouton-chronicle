void Txt_Down(void)
{
register cont;
	{
	Cur_Par--;
	for(cont=0;cont<3;cont++)
		{
		Par[cont][0]=Par[cont+1][0];
		Par[cont][1]=Par[cont+1][1];
		Par[cont][2]=Par[cont+1][2];
		}
	Par[3][0]=0;
	Par[3][1]=0;
	Par[3][2]=0;
	}
}

void Print_Let(unsigned short px,byte py,byte lg,byte ht,byte *scr,byte Nb,byte Coul)
{
unsigned int t;
void *Poit;
Poit=Lettre[Nb];
t=(py<<8)+(py<<6);

_asm {
  push ds
  les di,scr
  lds si,Poit
  mov cx,t
  add cx,px
  add di,cx
  mov ax,di
  mov bh,ht
psprh:
  mov ch,lg
psprl:
  mov dl,ds:[si]
  or dl,dl
  jz psprl2
  mov dl,Coul
  mov es:[di],dl
psprl2:
  inc si
  inc di
  dec ch
  or ch,ch
  jnz psprl

  add ax,320
  mov di,ax
  dec bh
  or bh,bh
  jnz psprh
  pop ds
	}
}



void Load_Txt(void)
{
register cont;
LoadPCX("Mouton/Font.pcx",Page,Pal[1]);
for(cont=0;cont<53;cont++)
	GetBlk1((cont*6)+1,1,5,6,Page,Lettre[cont]);
for(cont=0;cont<43;cont++)
	GetBlk1((cont*6)+1,8,5,6,Page,Lettre[cont+53]);

}

void Load_Txt_Menu(void)
{
register i,j;
LoadPCX("Mouton/fontm.pcx",Page,Pal[2]);       //charger les sprites de fond
for(j=0;j<4;j++)
	for(i=0;i<24;i++)
		GetBlk1((i*13)+1,(j*13)+1,12,12,Page,Lettre[j*24+i]);

}

void Print_Txt(unsigned short x,unsigned char y,char * txt,unsigned char *Page,unsigned char Coul)
{
register cont=0;
while(txt[cont]!='\0')
{
if((txt[cont]>31)&&(txt[cont]<127))
	{
	if(x>315)
	return;
	if(Let)
		{
		Print_Let(x,y,12,12,Page,txt[cont]-32,Coul);
		x+=Let_Lg1[txt[cont]-32]+2;
		}
	else
		{
		Print_Let(x,y,5,6,Page,txt[cont]-32,Coul);
		x+=Let_Lg[txt[cont]-32]+1;
		}
	}
cont++;
}
}

void Print_Nb(unsigned short x,unsigned char y,unsigned short nb,unsigned char *Page,unsigned char Coul)
{
unsigned char Lng=1;
unsigned short Exp=10;
unsigned char temp;
while(nb>=Exp)
	{
	Exp*=10;
	Lng++;
	}
Exp/=10;
while(Lng)
{
temp=(nb/Exp);
nb-=temp*Exp;
{
if(x>315)
	return;
if(Let)
	{
	Print_Let(x,y,12,12,Page,temp+16,Coul);
	x+=Let_Lg1[temp+16]+2;
	}
else
	{
	Print_Let(x,y,5,6,Page,temp+16,Coul);
	x+=Let_Lg[temp+16]+1;
	}

}
Lng--;
Exp/=10;
}
}

