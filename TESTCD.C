#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include "cd.c"


void main(void)
{
printf("\n*********************************Test cd*********************************");
Nb_CD();
printf("\nLecteurs CD presents : %d",CD.Nombre);
printf("\nPremier lecteur : %c",CD.First+65);
}