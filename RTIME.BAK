float far temps;
void far interrupt (*oldit)(void);
void far interrupt Time(void);


void far interrupt Time(void)
{
temps+=0.0549255;
outportb(0x20,0x20);
}

void DebTime(void)
{
temps=0;
oldit=getvect(0x1c);
setvect(0x1c,Time);
}

void FinTime(void)
{
setvect(0x1c,oldit);
}
