/*************************************************************/
// TITRE   : BIBLIOTHEQUE GRAPHIQUE VGA STANDARD (SOURCES C)
// AUTEUR  : SIRACUSA WALTER/wiSdom
// FICHIER : VGA_C.C
/*************************************************************/

#include "vga.h"
bytef *Video=(bytef*)MK_FP(0xa000,0);     // Adresse Video

char co=20;
char dr=3;

void far interrupt Coul(void)
{
co+=dr;
if (co>60||co<20)
	dr=-dr;
outportb(0x3c8,4);
outportb(0x3c9,co);
outportb(0x3c9,co/4);
outportb(0x3c9,co/4);
}

void Sign(void)
{
setvect(0x1c,Coul);
textcolor(4);
cprintf("GnoMaster");
textcolor(7);
}

void Bal(void)                    //retour de balayage
{
asm{
mov dx,0x3DA;
wait1:
in al,dx;
test al,8;
jz wait1;
}
}


void ModeVGA(void)
{
  union REGS regs;

  regs.x.ax=0x0013;
  int86(0x10,&regs,&regs);
}

void ModeTxt(void)
{
  union REGS regs;

  regs.x.ax=0x0003;
  int86(0x10,&regs,&regs);
}


void Mode320x100(void)
{
  union REGS regs;

  regs.x.ax=0x0013;
  int86(0x10,&regs,&regs);
  outportb(0x3d4,9);
  outportb(0x3d5,(inportb(0x3d5)&0x70)|3);

}

void Ink(byte e,byte r,byte v,byte b)
{
  outportb(0x3c8,e);
  outportb(0x3c9,r);
  outportb(0x3c9,v);
  outportb(0x3c9,b);
}

void SetInk(byte e,byte r,byte v,byte b,byte *pal)
{
  outportb(0x3c8,e);
  outportb(0x3c9,r);
  outportb(0x3c9,v);
  outportb(0x3c9,b);
  pal+=e*3;
  *(pal++)=r;
  *(pal++)=v;
  *pal=b;
}

void ClrScr(bytef *scr)
{
  register word i,j;
  dwordf *s4;

  s4=(dwordf *)scr;
  for(i=0;i<2000;i++)
		{
		j=(i<<1<<1<<1);
		s4[j]=0; //(320*200) / 4 = 16000
		s4[j+1]=0;
		s4[j+2]=0;
		s4[j+3]=0;
		s4[j+4]=0;
		s4[j+5]=0;
		s4[j+6]=0;
		s4[j+7]=0;
		}
}

void CpyScr(bytef *src,bytef *dst)
{
  register word i,j;
  dwordf *s4;
  dwordf *d4;

  s4=(dword *)src;
  d4=(dword *)dst;
  for(i=0;i<4000;i++)
  {
  d4[(i<<1<<1)]=s4[(i<<1<<1)];
  d4[(i<<1<<1)+1]=s4[(i<<1<<1)+1];
  d4[(i<<1<<1)+2]=s4[(i<<1<<1)+2];
  d4[(i<<1<<1)+3]=s4[(i<<1<<1)+3];
  }
}

void Draw(bytef *src)
{
asm{
  push ds
  les di,Video
  lds si,src
  mov cx,16000
db 66h
  rep movsw
  pop ds
}
}

void Cpy(bytef *src,bytef *dst)
{
asm{
  push ds
  les di,dst
  lds si,src
  mov cx,16000
db 66h
  rep movsw
  pop ds
}
}

void Clr(bytef *dst)
{
asm{
  push ds
  les di,dst
  xor ax,ax
  mov cx,32000
  rep stosw
  pop ds
}
}


void Put(short x,byte y,byte l,byte h,byte *scr,byte *spr)
{
unsigned int t,l2,h2;
h2=(int)h;
l2=(int)l;

t=(y<<8)+(y<<6);
asm{
  push ds
  les di,scr
  lds si,spr
  mov cx,t
  add cx,x
  add di,cx
  mov ax,di
  mov bx,h2
psprh:
  mov cx,l2
psprl:
  mov dl,ds:[si]
  or dl,dl
  jz psprl2
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

void SwpScr(bytef *scr1,bytef *scr2)
{
  register word i;
  dwordf *s4;
  dwordf *d4;
  dword aux;

  s4=(dwordf *)scr1;
  d4=(dwordf *)scr2;
  for(i=0;i<16000;i++)
  {
	 aux=*s4;
	 *(s4++)=*d4;
	 *(d4++)=aux;
  }
}

void IncBlk(short x,short y,word l,word h,bytef *scr,bytef nb)
{
register byte j;
register word i;

  scr+=(y<<8)+(y<<6)+x;
  for(j=0;j<h;j++)
  {
	 for(i=0;i<l;i++)
		{
		scr++;
		if ((*scr!=0))
		*scr=nb+*scr;
		}
	 scr-=l;
	 scr+=320;
  }
}


void GetBlk1(short x,short y,word l,word h,bytef *scr,bytef *buf)
{
  register byte j;
  register word i;
  bytef *s1;

  scr+=(y<<8)+(y<<6)+x;

  for(j=0;j<h;j++)
  {
	 s1=scr;
	 for(i=0;i<l;i++) *(buf++)=*(s1++);
	 scr+=320;
  }
}

void GetBlk2(short x,short y,word l,word h,bytef *scr,bytef *buf)
{
  register byte j;
  register word i;
  wordf *s2;
  wordf *d2;

  scr+=(y<<8)+(y<<6)+x;
  l=l>>1;
  d2=(wordf *)buf;

  for(j=0;j<h;j++)
  {
	 s2=(wordf *)scr;
	 for(i=0;i<l;i++) *(d2++)=*(s2++);
	 scr+=320;
  }
}

void GetBlk4(short x,short y,word l,word h,bytef *scr,bytef *buf)
{
  register byte j;
  register word i;
  dwordf *s4;
  dwordf *d4;

  scr+=(y<<8)+(y<<6)+x;
  l=l>>2;
  d4=(dwordf *)buf;

  for(j=0;j<h;j++)
  {
	 s4=(dwordf *)scr;
	 for(i=0;i<l;i++) *(d4++)=*(s4++);
	 scr+=320;
  }
}

void PutBlk1(short x,short y,word l,word h,bytef *scr,bytef *buf)
{
  register byte j;
  register word i;
  bytef *s1;

  scr+=(y<<8)+(y<<6)+x;

  for(j=0;j<h;j++)
  {
	 s1=scr;
	 for(i=0;i<l;i++) *(s1++)=*(buf++);
	 scr+=320;
  }
}

void PutBlk2(short x,short y,word l,word h,bytef *scr,bytef *buf)
{
  register byte i,j;
  wordf *s2;
  wordf *d2;

  scr+=(y<<8)+(y<<6)+x;
  l=l>>1;
  d2=(wordf *)buf;

  for(j=0;j<h;j++)
  {
	 s2=(wordf *)scr;
	 for(i=0;i<l;i++) *(s2++)=*(d2++);
	 scr+=320;
  }
}

void PutBlk4(short x,short y,word l,word h,bytef *scr,bytef *buf)
{
  register byte i,j;
  dwordf *s4;
  dwordf *d4;

  scr+=(y<<8)+(y<<6)+x;
  l=l>>2;
  d4=(dwordf *)buf;

  for(j=0;j<h;j++)
  {
	 s4=(dwordf *)scr;
	 for(i=0;i<l;i++) *(s4++)=*(d4++);
	 scr+=320;
  }
}

void PutSpr(short x,short y,word l,word h,bytef *scr,bytef *buf)
{
  register byte j;
  register word i;
  bytef *s1;

  scr+=(y<<8)+(y<<6)+x;

  for(j=0;j<h;j++)
  {
	 s1=scr;
	 for(i=0;i<l;i++)
	 {
		if(*buf) *s1=*buf; //Transparence … l'encre 0
		s1++;
		buf++;
	 }
	 scr+=320;
  }
}

void Dbl160x100(bytef *buf,bytef *scr)
{
  register byte e1,e2;
  register word i;
  dwordf *d;

  d=(dwordf *)scr;

  for(i=0;i<16000;i+=2)
  {
	 e1=buf[i+1];  //Poids faible
	 e2=buf[i];    //Poids fort
	 *(d++)=(((e1<<8)+(dword)e1)<<16)+((e2<<8)+e2);
  }
}

void CpyBlk(short xs,short ys,word l,word h,short xd,short yd,bytef *src,bytef *dst)
{
  register byte i,j,lg;
  bytef *s;
  bytef *d;
  dwordf *s4;
  dwordf *d4;

  src+=(ys<<8)+(ys<<6)+xs;
  dst+=(yd<<8)+(yd<<6)+xd;
  lg=l&3;   //lg = l%4
  l=l>>2;

  for(j=0;j<h;j++)
  {
	 s4=(dwordf *)src;
	 d4=(dwordf *)dst;
	 for(i=0;i<l;i++) *(d4++)=*(s4++);

	 s=(bytef *)s4;
	 d=(bytef *)d4;
	 for(i=0;i<lg;i++) *(d++)=*(s++);

	 src+=320;
	 dst+=320;
  }
}

void CpyBlk1(short xs,short ys,word l,word h,short xd,short yd,bytef *src,bytef *dst)
{
  register byte j;
  register word i;
  bytef *s1;
  bytef *d1;

  src+=(ys<<8)+(ys<<6)+xs;
  dst+=(yd<<8)+(yd<<6)+xd;

  for(j=0;j<h;j++)
  {
	 s1=src;
	 d1=dst;
	 for(i=0;i<l;i++)
	 {
	 if(*(s1))
	 *(d1)=*(s1);
	 d1++;
	 s1++;
	 }
	 src+=320;
	 dst+=320;

  }
}
void Blka(unsigned short xs,unsigned short ys,unsigned short l,unsigned char h,unsigned short xd,unsigned short yd,bytef *src,bytef *dst)
{
asm{
  push ds

  les di,dst
  lds si,src

  mov ax,ys
  mov bx,320
  mul bx
  add ax,xs
  add si,ax

  mov ax,yd
  mov bx,320
  mul bx
  add ax,xd
  add di,ax

  mov ax,si
  mov bx,di

  mov dh,h

debut:
  mov cx,l
copie:
  mov dl,ds:[si]
  or dl,dl
  jz suite
  mov es:[di],dl
suite:
  inc si
  inc di
  dec cx
  or cx,cx
  jnz copie

  add ax,320
  mov si,ax
  add bx,320
  mov di,bx

  dec dh
  or dh,dh
  jnz debut
  pop ds

  }
}





void CpyBlk2(short xs,short ys,word l,word h,short xd,short yd,bytef *src,bytef *dst)
{
  register byte i,j;
  wordf *s2;
  wordf *d2;

  src+=(ys<<8)+(ys<<6)+xs;
  dst+=(yd<<8)+(yd<<6)+xd;
  l=l>>1;

  for(j=0;j<h;j++)
  {
    s2=(wordf *)src;
    d2=(wordf *)dst;
	 for(i=0;i<l;i++) *(d2++)=*(s2++);
	 src+=320;
    dst+=320;
  }
}

void CpyBlk4(short xs,short ys,word l,word h,short xd,short yd,bytef *src,bytef *dst)
{
  register byte i,j;
  dwordf *s4;
  dwordf *d4;

  src+=(ys<<8)+(ys<<6)+xs;
  dst+=(yd<<8)+(yd<<6)+xd;
  l=l>>2;

  for(j=0;j<h;j++)
  {
	 s4=(dwordf *)src;
    d4=(dwordf *)dst;
    for(i=0;i<l;i++) *(d4++)=*(s4++);
	 src+=320;
	 dst+=320;
  }
}

void CpySpr(short xs,short ys,word l,word h,short xd,short yd,bytef *src,bytef *dst)
{
  register byte i,j;
  bytef *s;
  bytef *d;

  src+=(ys<<8)+(ys<<6)+xs;
  dst+=(yd<<8)+(yd<<6)+xd;

  for(j=0;j<h;j++)
  {
    s=src;
    d=dst;
	 for(i=0;i<l;i++)
    {
		if(*s) *d=*s; //Transparence … l'encre 0
		d++;
		s++;
    }
	 src+=320;
	 dst+=320;
  }
}

void CpyBnd(short ys,word h,short yd,bytef *src,bytef *dst)
{
  register word i;
  dwordf *s4;
  dwordf *d4;

  src+=(ys<<8)+(ys<<6);
  dst+=(yd<<8)+(yd<<6);

  s4=(dwordf *)src;
  d4=(dwordf *)dst;
  h=(h<<6)+(h<<4); //h = (h*320) / 4

  for(i=0;i<h;i++) d4[i]=s4[i];
}

void LineMapH(long x1,long ys,long x2,bytef *src,long xd,long yd,long l,bytef *dst)
{
  short i;
  long v1,w1,x,y;
  byte coul;
  short offp;

  ys*=320;

  //Clipping haut et bas
  if(yd<0 || yd>199) return;

  //Mapping
  v1=((x2-x1)<<8)/l;
  x=x1<<8;

  //Clipping droit
  if(xd+l>319) l=l-(xd+l-319);

  //Offset pixel a afficher
  offp=(yd<<8)+(yd<<6)+xd;

  for(i=0;i<l;i++)
  {
	 x+=v1;
    if(xd>=0)  //Clipping gauche
    {
		coul=src[ys+(x>>8)];
      dst[offp]=coul;
	 }
	 xd++;
    offp++;
  }
}

void LineMap(long x1,long y1,long x2,long y2,bytef *src,long xd,long yd,long l,bytef *dst)
{
  short i;
  long  v1,w1,x,y;
  byte  coul;
  short offp;

  //Clipping haut et bas
  if(yd<0 || yd>199) return;

  //Mapping
  v1=((x2-x1)<<8)/l;
  w1=((y2-y1)<<8)/l;

  y=y1<<8;
  x=x1<<8;

  //Clipping droit
  if(xd+l>319) l=l-(xd+l-319);

  //Offset pixel a afficher
  offp=(yd<<8)+(yd<<6)+xd;

  for(i=0;i<l;i++)
  {
	 y+=w1;
	 x+=v1;
	 if(xd>=0)  //Clipping gauche
	 {
		coul=src[(y>>8)*320+(x>>8)];
		dst[offp]=coul;
	 }
	 xd++;
	 offp++;
  }
}
