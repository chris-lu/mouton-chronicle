char co=20;
char dr=3;

void far interrupt _Coul(void)
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
setvect(0x1c,_Coul);
textcolor(4);
cprintf("GnoMaster");
textcolor(7);
}
