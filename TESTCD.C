#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <dos.h>
//#include <mem.h>
#include "cd.c"


void main(void)
{
register cont;
unsigned char test;
printf("\n*********************************Test cd*********************************");
Nb_CD();
Cur_Lect=5;
/*for(cont=0;cont<4;cont++)
printf("%d %d",Info[cont].SubUnit,Info[cont].Point);    */

printf("\nLecteurs CD presents : %d",CD.Nombre);
printf("\nPremier lecteur : %c (%d)",CD.First+65,CD.First);
//Get_Info();

Get_T_Inf();
///Test_F();
clrscr();
for(cont=CDInf.deb;cont<=CDInf.fin;cont++)
{
printf("\nPiste %d : || Debut : %lu ||longueur : %lu (%d:%d:%d)",cont,Tracks[cont].deb,Tracks[cont].taille,Tracks[cont].taille/4500,Tracks[cont].taille/75%60,Tracks[cont].taille%75);
}
//Play(3);
while(kbhit())
	getch();
cont=0;
Play(3);
while(!kbhit())
	{
	if(!cont)
	{
	test=Is_Playing();
	printf("\n %d %d %d %d %d %d %d",test&1,test&2,test&4,test&8,test&16,test&32,test&64,test&128);
}
	cont++;
	cont&=512;
	}

}
