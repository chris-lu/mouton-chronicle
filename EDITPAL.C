#include <stdio.h>
#include <conio.h>
#include "vga.c"
#include "vga_c.c"
#include "mouse.c"

bytef *pal;
unsigned char Page[64000];

void inv_pal(bytef *pal)
{

byte pt[768];
int cont;
for (cont=0;cont<256;cont++)
	{
	pt[cont*3]=pal[768-cont*3];
	pt[cont*3+1]=pal[768-cont*3+1];
	pt[cont*3+2]=pal[768-cont*3+2];
	}
for (cont=0;cont<768;cont++)
	pal[cont]=pt[cont];
SetAllPal(pal);
}

void DrawScreen(void)
{
Clr(Video);
for (i=0;i<255;i++)
	for (j=9;j<189;j++)
	Video[(j<<8)+(j<<6)+i+54]=i;
gotoxy(1,6);
printf("Taux:");
gotoxy(1,8);
printf("Rouge:");
gotoxy(1,11);
printf("Vert:");
gotoxy(1,14);
printf("Bleu:");
gotoxy(1,25);
printf("GnoMaster");
}


void main (void)
{
register int i,j;
unsigned short x,y;
char *nom;
char c=0;
char Fin=0;
unsigned char Coul;
printf("Charger une image?(O/N)");
scanf("%c",&c);
pal=calloc(768,sizeof(bytef));
if ((c=='O')||(c=='o'))
	{
	printf("Entrez le nom du fichier:");
	scanf("%s",nom);
	if (LoadPal(nom,pal)==0)
		{
		c=0;
		printf("\n\nFichier inexistant : Chargement de la palette standard!\n");
		textcolor(7+128);
		cprintf("Appuyer sur une touche pour continuer . . .");
		sound(840);
		delay(200);
		nosound();
		getch();
		}
	}
ModeVGA();
if ((c=='O')||(c=='o'))
	SetAllPal(pal);
else
	GetAllPal(pal);
for (i=0;i<255;i++)
	for (j=9;j<189;j++)
	Video[(j<<8)+(j<<6)+i+54]=i;
ZoneMouse(54,9,309,188);
MouseON();
gotoxy(1,6);
printf("Taux:");
gotoxy(1,8);
printf("Rouge:");
gotoxy(1,11);
printf("Vert:");
gotoxy(1,14);
printf("Bleu:");
gotoxy(1,25);
printf("GnoMaster");
while(!Fin)
{
while(kbhit())
	c=getch();
switch(c);
	{
	case '1'	:	gotoxy(1,1);
					printf("Couleur a modifier :");
					scanf("%d",Coul);
					break;
GetMouse(&x,&y);
gotoxy(1,9);
printf("%2d",pal[(x-54)*3]);
gotoxy(1,12);
printf("%2d",pal[(x-54)*3+1]);
gotoxy(1,15);
printf("%2d",pal[(x-54)*3+2]);
gotoxy(1,1);
printf("                Couleur:%3d",x-54);
Bal();
FBox(5,130,44,44,Video,(x-54));
}
ModeTxt();
exit(0);
}
