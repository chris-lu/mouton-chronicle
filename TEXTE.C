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

void Print_Let(unsigned short x,byte y,byte *scr,byte Nb,byte Coul)
{
unsigned int t;
void *Poit;
Poit=Lettre[Nb];
t=(y<<8)+(y<<6);

asm{
  push ds
  les di,scr
  lds si,Poit
  mov cx,t
  add cx,x
  add di,cx
  mov ax,di
  mov bx,6
psprh:
  mov cx,5
psprl:
  mov dl,ds:[si]
  or dl,dl
  jz psprl2
  mov dl,Coul
  mov es:[di],dl
psprl2:
  inc si
  inc di
  dec cx
  or cx,cx
  jnz psprl

  add ax,320
  mov di,ax
  dec bx
  or bx,bx
  jnz psprh
  pop ds
	}
}



void Load_Txt(void)
{
register cont;
LoadPCX("Mouton/Font.pcx",Page,Pal[1]);       //charger les sprites de fond
for(cont=0;cont<53;cont++)
	GetBlk1((cont*6)+1,1,5,6,Page,Lettre[cont]);
for(cont=0;cont<43;cont++)
	GetBlk1((cont*6)+1,8,5,6,Page,Lettre[cont+53]);

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
	Print_Let(x,y,Page,txt[cont]-32,Coul);
	x+=Let_Lg[txt[cont]-32]+1;

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
Print_Let(x,y,Page,temp+16,Coul);
x+=Let_Lg[temp+16]+1;
}
Lng--;
Exp/=10;
}
}

