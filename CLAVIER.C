void Install_Clav(void)
{
oldit_9=getvect(0x9);
setvect(0x9,Lire_scan);
}

void Remove_Clav(void)
{
setvect(0x9,oldit_9);
}

void far interrupt Lire_scan(void)
{
register char c;
c=inportb(0x60);


if (c<0)
	Scan_Code[Scan_Code[0]=(c&0x7f)]=0;
	else
	Scan_Code[c]=1;

outportb(0x20,0x20);
}

void Lire_Keys(void)
{
Lire_scan();
}
