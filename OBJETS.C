#include <string.h>

typedef struct p
{
unsigned short nb_Liens;
unsigned short *Lien;
}Pt;

typedef struct Obj3D
{
unsigned short nb_Pts;
unsigned short nb_Face;

short *Xi;
short *Yi;
short *Zi;

short *X;
short *Y;
short *Z;

byte *Pt;

};



//***************************************************************************
//***************************************************************************


void Init_Obj(unsigned short nb,Obj3D *Ob);
void Init_Carre(unsigned short lar,unsigned short ht,unsigned short prof,Obj3D *Car);
void Init_CoordI(Obj3D *Ob);
void Aff_Obj_Pts(Obj3D *Ob,bytef *Scr);
void Rot_X(byte a,Obj3D *Ob);
void Rot_Rel_X(byte a,Obj3D *Ob);
void Rot_Y(byte a,Obj3D *Ob);
void Rot_Rel_X(byte a,Obj3D *Ob);
void Rot_Z(byte a,Obj3D *Ob);


//***************************************************************************
//***************************************************************************












float tabS[256];
float tabC[256];





void Init_Obj(unsigned short nb,Obj3D *Ob)
{

Ob->nb_Pts=nb;
Ob->nb_Face=nb;
Ob->Xi=(short*)malloc(Ob->nb_Pts*sizeof(short));
Ob->Yi=(short*)malloc(Ob->nb_Pts*sizeof(short));
Ob->Zi=(short*)malloc(Ob->nb_Pts*sizeof(short));
Ob->X=(short*)malloc(Ob->nb_Pts*sizeof(short));
Ob->Y=(short*)malloc(Ob->nb_Pts*sizeof(short));
Ob->Z=(short*)malloc(Ob->nb_Pts*sizeof(short));
}

void Init_Carre(unsigned short lar,unsigned short ht,unsigned short prof,Obj3D *Car)
{
int i,j,k;
short xt,yt,zt,cont=0;
for (i=0;i<2;i++)
	{
	xt=(i*lar)-(lar/2);
	for (j=0;j<2;j++)
		{
		yt=(j*ht)-(ht/2);
		for (k=0;k<2;k++)
			{
			zt=((k*prof)-(prof/2));
			Car->Xi[cont]=xt;
			Car->Yi[cont]=yt;
			Car->Zi[cont]=zt;
			cont++;
		}
}
}
}

void Init_CoordI(Obj3D *Ob)
{
register cont;
for (cont=0;cont<(Ob->nb_Pts);cont++);
{
Ob->X[cont]=Ob->Xi[cont];
Ob->Y[cont]=Ob->Yi[cont];
Ob->Z[cont]=Ob->Zi[cont];
}
}

void Lire_Obj(char* name,Obj3D *Ob)
{
FILE *fi;
int nb;
int let,cont;
char caract=0;
char *txt;
if ((fi=fopen("test.ob","r"))!=NULL)
{
while ((let=fgetc(fi))!= '@');
while ((let=fgetc(fi))!= '=')
	{
	txt[caract]=let;
	caract++;
	}
txt[caract]='\0';
if (stricmp(txt,"Nb_pts")==0)
	{
		fscanf(fi,"%d",&nb);
		Init_Obj(nb,Ob);
	}
if (stricmp(txt,"Pts")==0&&Ob->nb_Pts)
	{
	while ((let=fgetc(fi))!= '{');
	for (cont=0;cont<Ob->nb_Pts;cont++)
		{
		fscanf(fi,"%d",&nb);
		Ob->Xi[cont]=nb;
		let=fgetc(fi);
		fscanf(fi,"%d",&nb);
		Ob->Yi[cont]=nb;
		let=fgetc(fi);
		fscanf(fi,"%d",&nb);
		Ob->Zi[cont]=nb;
		let=fgetc(fi);
		}
		}
}
}








void Aff_Obj_Pts(Obj3D *Ob,bytef *Scr)
{
register cont;
for (cont=0;cont<(Ob->nb_Pts);cont++)
{
Scr[((Ob->Y[cont]+100)*320+(Ob->X[cont])+160)]=15;
}
}


void Rot_X(byte a,Obj3D *Ob)
{
register cont;
for(cont=0;cont<(Ob->nb_Pts);cont++)
{
Ob->X[cont]=Ob->Xi[cont];
Ob->Y[cont]=Ob->Yi[cont]*tabC[a]+Ob->Zi[cont]*tabS[a];
Ob->Z[cont]=-Ob->Yi[cont]*tabS[a]+Ob->Zi[cont]*tabC[a];
}
}

void Rot_Rel_X(short a,Obj3D *Ob)
{
register cont;
for(cont=0;cont<(Ob->nb_Pts);cont++)
{
Ob->X[cont]=Ob->X[cont];
Ob->Y[cont]=Ob->Y[cont]*tabC[a]+Ob->Z[cont]*tabS[a];
Ob->Z[cont]=-Ob->Y[cont]*tabS[a]+Ob->Z[cont]*tabC[a];
}
}


void Rot_Y(byte a,Obj3D *Ob)
{
register cont;
for(cont=0;cont<(Ob->nb_Pts);cont++)
{
Ob->X[cont]=Ob->Xi[cont]*tabC[a]+Ob->Zi[cont]*tabS[a];
Ob->Y[cont]=Ob->Yi[cont];
Ob->Z[cont]=-Ob->Xi[cont]*tabS[a]+Ob->Zi[cont]*tabC[a];
}
}


void Rot_Rel_Y(byte a,Obj3D *Ob)
{
register cont;
for(cont=0;cont<(Ob->nb_Pts);cont++)
{
Ob->X[cont]=(Ob->X[cont]*tabC[a])+(Ob->Z[cont]*tabS[a]);
Ob->Y[cont]=Ob->Y[cont];
Ob->Z[cont]=-(Ob->X[cont]*tabS[a])+(Ob->Z[cont]*tabC[a]);
}
}

void Rot_Z(short a,Obj3D *Ob)
{
register cont;
float Sn=sin(a),Cn=cos(a);
for(cont=0;cont<(Ob->nb_Pts);cont++)
{
Ob->X[cont]=Ob->Xi[cont]*Cn-Ob->Yi[cont]*Sn;
Ob->Y[cont]=Ob->Xi[cont]*Sn+Ob->Yi[cont]*Cn;
Ob->Z[cont]=Ob->Zi[cont];
}
}

