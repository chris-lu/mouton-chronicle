#include <dos.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include "rtime.c"

void main(void)
{
register short cont,cont2;
printf("Calcul . . .");
DebTime();
for(cont=0;cont<6400;cont++)
for(cont2=0;cont2<32000;cont2++);
FinTime();
printf("\nTemps C : %.2f",temps);
DebTime();
asm{
mov dx,6400
mov cx,32000
deb1:
dec dx
deb2:
dec cx
or cx,cx
jnz deb2
or dx,dx
jnz deb1
}
FinTime();
printf("\nTemps Asm : %.2f",temps);
}




