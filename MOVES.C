void Put_Mouton(unsigned short x)
{
unsigned char cont,temp;
Mouton[0].PosX=x;
temp=x/320;
x%=320;
while(!Plan_1[temp][Y[cont]+x])
	cont++;
Mouton[0].PosY=200-cont;
}


