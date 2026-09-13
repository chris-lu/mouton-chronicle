/*Palette :
0-16  	:Etoiles        *
16-24		:premier plan 	 *
24-32		:????????       *
32-64		:moutons      	 *
64-72		:sang           *
72-136	:2nd plan       *
136-152	:Lune       	 *
152-216	:Bonhomme   	 *
216-244  :Ciel           *
*/



//***********************************************************************************
//Creer la pallette de la neige
//***********************************************************************************
void Mk_Snow(unsigned char far *Palette)
{
register cont;
for (cont=Pv[1];cont<Pv[2];cont++)
	{
	Palette[cont*3]=cont*2;
	Palette[cont*3+1]=cont*2;
	Palette[cont*3+2]=cont*2;
	//Palette[cont*3+2]=cont*2;
	}
}


/*void Mk_Mout_Coul(unsigned char far *Palette)
{
register cont;
for (cont=48;cont<64;cont++)
	{
	Palette[cont*3]=random(63);
	Palette[cont*3+1]=random(63);
	Palette[cont*3+2]=random(63);
	}
}   */


//***********************************************************************************
//Creer la pallette du sang
//***********************************************************************************
void Mk_Blood(unsigned char far *Palette)
{
register cont,temp=31;
for (cont=Pv[3];cont<Pv[4];cont++)
	{
	Palette[cont*3]=temp;
	Palette[cont*3+1]=0;
	Palette[cont*3+2]=0;
	//Palette[cont*3+2]=cont*2;
	temp+=4;
	}
}



//***********************************************************************************
//Creer les couleurs du degrade du ciel
//***********************************************************************************
void Mk_Sky(unsigned char far *Palette)
{
register cont;
float temp;
temp=Ciel_Coul.R_Start;
for (cont=216;cont<244;cont++)
	{
	Palette[cont*3]=temp;
	temp+=Ciel_Coul.R;
	}
temp=Ciel_Coul.V_Start;
for (cont=216;cont<244;cont++)
	{
	Palette[cont*3+1]=temp;
	temp+=Ciel_Coul.V;
	}
temp=Ciel_Coul.B_Start;
for (cont=216;cont<244;cont++)
	{
	Palette[cont*3+2]=temp;
	temp+=Ciel_Coul.B;
	}
}


//***********************************************************************************
//Creer les couleurs des etoiles
//***********************************************************************************
void Mk_Stars(unsigned char far *Palette)
{
register cont;
/*for (cont=32;cont<48;cont++)
	{
	Palette[cont*3]=(cont+16)/2;
	Palette[cont*3+1]=(cont+16)/2;
	Palette[cont*3+2]=(cont+16)/2;
	} */
for(cont=0;cont<8;cont++)
	{
	Palette[(cont)*3]=(cont)*6;
	Palette[(cont)*3+1]=(cont)*6;
	Palette[(cont)*3+2]=(cont)*6;
	Palette[(16-cont)*3]=(cont)*6;
	Palette[(16-cont)*3+1]=(cont)*6;
	Palette[(16-cont)*3+2]=(cont)*6;
	}
Palette[8*3]=48;
Palette[8*3+1]=48;
Palette[8*3+2]=48;

}


//***********************************************************************************
//Rotation de la pallette des couleurs
//***********************************************************************************
void Rot_Pal(unsigned short deb,unsigned short nb, unsigned char *Pal1,unsigned char *Pal2)
{
char r,v,b;
nb*=3;
deb*=3;
asm{
	push ds
	mov dx,nb
	lds si,Pal1
	les di,Pal2
	mov cx,0
	mov ax,deb
	add si,ax
	add di,ax
	mov ah,ds:[si]
	mov r,ah
	mov ah,ds:[si+1]
	mov v,ah
	mov ah,ds:[si+2]
	mov b,ah
boucle:
	mov ah,ds:[si+3]
	mov es:[di],ah
	inc si
	inc di
	inc cx
	cmp cx,dx
	jne boucle
	mov ah,r
	mov es:[di],ah
	mov ah,v
	mov es:[di+1],ah
	mov ah,b
	mov es:[di+2],ah
	pop ds
	}
}


//***********************************************************************************
//Duplique une pallette de couleurs
//***********************************************************************************
void CpyPal(unsigned char *Pal1,unsigned char *Pal2)
{
asm{
	push ds
	lds si,Pal1
	les di,Pal2
	mov cx,192
	db 66h
	rep movsw
	pop ds
	}
}

//***********************************************************************************
//Incremente la pallette des couleur (vire au blanc)
//***********************************************************************************
void Inc_Pal(unsigned char * Pal)
{
asm{
	push ds
	lds si,Pal
	les di,Pal
	mov cx,768
	xor ax,ax
deb:
	mov al,ds:[si]
	cmp al,63
	jge suite
	inc al
	mov es:[di],al
suite:
	inc di
	inc si
	dec cx
	or cx,0
	jnz deb
	pop ds
	}
}

//***********************************************************************************
//decremente la pallette des couleurs (vire au noir)
//***********************************************************************************
void Dec_Pal(unsigned char * Pal)
{
asm{
	push ds
	lds si,Pal
	les di,Pal
	mov cx,768
	xor ax,ax
deb:
	mov al,ds:[si]
	or al,0
	jz suite
	dec al
	mov es:[di],al
suite:
	inc di
	inc si
	dec cx
	or cx,0
	jnz deb
	pop ds
	}
}

//***********************************************************************************
//transfert entre 2 pallette de couleurs
//***********************************************************************************
void Goto_Pal(unsigned char *Pal2,unsigned char *Pal1)
{
asm{
	push ds
	lds si,Pal1
	les di,Pal2
	mov cx,768
debut:
	mov al,ds:[si]
	mov ah,es:[di]
	cmp al,ah
	je suite
	jg suite1
	dec ah
	mov es:[di],ah
	jmp suite
suite1:
	inc ah
	mov es:[di],ah
suite:
	inc di
	inc si
	dec cx
	or cx,0
	jnz debut
	pop ds
	}
}

//***********************************************************************************
//deplace une partie d'une pallette des couleurs vers une autre
//***********************************************************************************
void Create_Pal(unsigned char src,unsigned char nb,unsigned char* Pal1,unsigned char dst,unsigned char *Pal2)
{
register cont;
for (cont=0;cont<nb;cont++)
	{
	Pal2[(cont+dst)*3]=Pal1[(cont+src)*3];
	Pal2[(cont+dst)*3+1]=Pal1[(cont+src)*3+1];
	Pal2[(cont+dst)*3+2]= Pal1[(cont+src)*3+2];
	}
}

//***********************************************************************************
//incremente les couleurs d'une image
//***********************************************************************************
void Inc_Scr(unsigned char nb,unsigned char far *Scr)
{
unsigned long cont;
for(cont=0;cont<64000;cont++)
	if(Scr[cont])
		Scr[cont]+=nb;
}

void Init_Pal(bytef *p)
{
register cont,cont2,cont3;

for (cont=0;cont<4;cont++)  // dégradé de rouge
	{
	for(cont2=0;cont2<16;cont2++)
		{
		for(cont3=0;cont3<3;cont3++)
			{
			if(CoulMul[(cont*3)+cont3]!=-1)
				p[(cont2+cont*16)*3+cont3]=cont2*CoulMul[(cont*3)+cont3];      //rouge
			else
				p[(cont2+cont*16)*3+cont3]=63;      //rouge
			}
		}
	}
}


