unsigned char far *Page;
//char far Let_Lg1[97]={4,2,4,10,7,12,10,2,4,4,0,8,3,5,2,6,8,6,7,7,8,8,8,8,7,8,2,3,7,6,7,7,12,10,8,8,8,7,7,9,9,3,6,9,7,11,10,9,8,9,8,7,8,10,10,12,10,9,8,5,6,5,7,7,2,7,7,6,7,7,5,8,8,2,4,7,3,11,7,7,8,7,6,5,5,7,7,9,7,7,7,7,4,1,4,6};

/*void Inc_Scr(unsigned char nb,unsigned char far *Scr)
{
unsigned long cont;
for(cont=0;cont<64000;cont++)
	if(Scr[cont])
		Scr[cont]+=nb;
}
		  */
/*void Create_Pal(unsigned char src,unsigned char nb,unsigned char* Pal1,unsigned char dst,unsigned char *Pal2)
{
register cont;
for (cont=0;cont<nb;cont++)
	{
	Pal2[(cont+dst)*3]=Pal1[(cont+src)*3];
	Pal2[(cont+dst)*3+1]=Pal1[(cont+src)*3+1];
	Pal2[(cont+dst)*3+2]= Pal1[(cont+src)*3+2];
	}
}

	  */
void Print_Let_Menu(unsigned short x,unsigned char y,unsigned char *scr,unsigned char Nb,unsigned char Coul)
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
  mov bx,12
psprh:
  mov cx,12
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



void Load_Txt_Menu(void)
{
register i,j;
LoadPCX("Mouton/fontm.pcx",Page,Pal[2]);       //charger les sprites de fond
for(j=0;j<4;j++)
	for(i=0;i<24;i++)
		GetBlk1((i*13)+1,(j*13)+1,12,12,Page,Lettre[j*24+i]);

}

void Print_Txt_Menu(unsigned short x,unsigned char y,char * txt,unsigned char *Page,unsigned char Coul)
{
register cont=0;
while(txt[cont]!='\0')
{
if((txt[cont]>31)&&(txt[cont]<127))
	{
	Print_Let_Menu(x,y,Page,txt[cont]-32,Coul);
	x+=Let_Lg1[txt[cont]-32]+2;

	}
cont++;
}
}

void Print_Nb_Menu(unsigned short x,unsigned char y,unsigned short nb,unsigned char *Page,unsigned char Coul)
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
Print_Let_Menu(x,y,Page,temp+16,Coul);
x+=Let_Lg1[temp+16]+2;
}
Lng--;
Exp/=10;
}
}

