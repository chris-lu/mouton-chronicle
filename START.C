void inittab(void)
{
register cont;
float A;
A=M_PI/256;
for (cont=0;cont<256;cont++)
	{
	tab1[cont]=sin(A*cont*14)*32;
	tab2[cont]=cos(A*cont*8+2)*32;
	tab3[cont]=sin(A*cont*10+1)*32;
	}
}



void cpyh(unsigned short haut,short pos,bytef *page1,bytef *page2)
{
if (pos>=0)
{
asm{
push ds
lds si,page1
les di,page2
mov ax,haut
mov bx,320
mul bx
add si,ax
add di,ax
mov ax,pos
add di,ax
mov ax,320
sub ax,pos
mov bx,4
div bx
mov cx,ax
db 66h
  rep movsw
mov cx,dx
  rep movsb
pop ds
}
}
else
{
pos=-pos;
asm{
push ds
lds si,page1
les di,page2
mov ax,haut
mov bx,320
mul bx
add si,ax
add di,ax
mov ax,pos
add si,ax
mov ax,320
sub ax,pos
mov bx,4
div bx
mov cx,ax
db 66h
  rep movsw
mov cx,dx
  rep movsb
pop ds
}
}

}

void cpyv(unsigned short col,short pos,bytef *page1,bytef *page2)
{
if (pos>=0)
{
asm{
push ds
lds si,page1
les di,page2
mov ax,col
add di,ax
add si,ax
mov ax,pos
mov bx,320
mul bx
add di,ax
mov cx,200
sub cx,pos
fin:
mov ah,ds:[si]
mov es:[di],ah
add di,320
add si,320
dec cx
or cx,cx
jnz fin
pop ds
}
}
else
{
pos=-pos;
asm{
push ds
lds si,page1
les di,page2
mov ax,col
add di,ax
add si,ax
mov ax,pos
mov bx,320
mul bx
add si,ax
mov cx,200
sub cx,pos
fin2:
mov ah,ds:[si]
mov es:[di],ah
add di,320
add si,320
dec cx
or cx,cx
jnz fin2
pop ds
}
}

}


/*void calculeH (unsigned char *pagescr,unsigned char *pagedst)
{
register cont;
for (cont=1;cont<200;cont++)
	cpyh(cont,(nb1*tab1[(nb2+cont)&255]+nb1*tab2[(nb3+cont)&255])>>6,page,pageAux);                    //2x plus rapide!!!!!!!!!
	//CpyBlk4(0,cont,320,1,tab[(byte)(a+cont)],cont,Font,PageAux2);
nb2++;
}

void calculeV (void)
{
register cont;
for (cont=1;cont<320;cont++)
	cpyv(cont,(nb1*tab2[(nb2+cont)&255]+nb1*tab3[(nb3+cont)&255])>>6,PageAux,PageAux2);                    //2x plus rapide!!!!!!!!!
	//LineV(cont,200,cont,tab2[(b+cont)%360],Font,PageAux2);
nb3+=2;
//b%=360;
}
		*/
void start(void)
{
register cont;
unsigned char phase=0;
unsigned char cc=1;
unsigned char screen=0;
char *Fich[3]={"mouton/atlantys.pcx","mouton/presente.pcx","mouton/mouton.pcx"};
/*unsigned char far *PageAux;
unsigned char far *PageAux2;   */

if((Plan_2[0]=(unsigned char far*)farmalloc(64000))==NULL)
	exit(0);

if((Plan_2[1]=(unsigned char far*)farmalloc(64000))==NULL)
	exit(0);

LoadPCX(Fich[screen],Page,Pal[1]);
inittab();
for(cont=0;cont<768;cont++)
	Pal[0][cont]=0;
SetAllPala(Pal[0]);
while(kbhit())
	getch();
Clr(Plan_2[0]);
Clr(Plan_2[1]);
Play(2);
while(!kbhit())
{
cc++;
cc&=127;
if(!(cc&63))
	phase++;
if(phase<2)
	{
	nb1=(11-cc/15);
	}
	else if(phase>3)
	nb1=(11-(127-cc)/15);
if(phase==0)
	{
	Goto_Pal(Pal[0],Pal[1]);
	SetAllPala(Pal[0]);
	}
	else if(phase==5)
	{
	Dec_Pal(Pal[0]);
	SetAllPala(Pal[0]);
	}
	else if(phase==6)
		{
		if(!screen)
			Play(3);
		screen++;
		phase=0;
		LoadPCX(Fich[screen],Plan_2[1],Pal[1]);
		delay(1350);
		if(screen==3)
				break;
		}

Clr(Plan_2[0]);
if(!screen)
{
for (cont=1;cont<200;cont++)
	cpyh(cont,(nb1*tab1[(nb2+cont)&255]+nb1*tab2[(nb3+cont)&255])>>6,Page,Plan_2[0]);                    //2x plus rapide!!!!!!!!!
nb2++;

for (cont=1;cont<320;cont++)
	cpyv(cont,(nb1*tab2[(nb2+cont)&255]+nb1*tab3[(nb3+cont)&255])>>6,Plan_2[0],Plan_2[1]);                    //2x plus rapide!!!!!!!!!
nb3+=2;
}
Bal();
Draw(Plan_2[1],0);
}
if(!screen)
	Play(3);
Clr(Page);
Draw(Page,0);
farfree(Plan_2[0]);
farfree(Plan_2[1]);
}
