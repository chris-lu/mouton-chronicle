void Ecrire_Jeu_Cfg(void)
{
if ((Fichier_Cfg=fopen("Mouton/Mouton.cfg","w"))!=NULL)
{
fprintf(Fichier_Cfg,JEUCFG,4,4,40,0);
Options.Nb_Joueurs=4;
Options.Nb_Plans=4;
Options.Pts_2_Vie=66;
TX=4*320;
fclose(Fichier_Cfg);
}
else exit(1);
}

void Lire_Jeu_Cfg(void)
{
register cont;
char temp[256];
unsigned char ch;
if ((Fichier_Cfg=fopen("Mouton/Mouton.cfg","r"))!=NULL)
{
while (strcmp(temp,"END"))
{
fscanf(Fichier_Cfg,"%s",&temp);
if(!strcmp(temp,"END"))
	continue;
while((fgetc(Fichier_Cfg))!='=');
if (!strcmp(temp,"JOUEURS"))
	{
	fscanf(Fichier_Cfg,"%d",&ch);
	Options.Nb_Joueurs=ch;
	}
else if (!strcmp(temp,"PLANS"))
	{
	fscanf(Fichier_Cfg,"%d",&ch);
	Options.Nb_Plans=ch;
	}
else if (!strcmp(temp,"POINTS_2_VIE"))
	{
	fscanf(Fichier_Cfg,"%d",&ch);
	Options.Pts_2_Vie=ch;
	}
else if (!strcmp(temp,"CURNIV"))
	{
	fscanf(Fichier_Cfg,"%d",&ch);
	Niveau.Cur_Level=ch;
	}
else if (!strcmp(temp,"NIVEAUX"))
	{
	fscanf(Fichier_Cfg,"%d",&ch);
	Niveau.Nb_Level=ch;
	for (cont=0;cont<Niveau.Nb_Level;cont++)
	{
	while(fgetc(Fichier_Cfg)!='#');
	fscanf(Fichier_Cfg,"%s",&Niveau.Level[cont]);
	}
	}

while(fgetc(Fichier_Cfg)!='-');
}

TX=Options.Nb_Plans*320;
fclose(Fichier_Cfg);
printf("mouton.cfg charge");
}
else Ecrire_Jeu_Cfg();
}

void Ecrire_Level_Cfg(char lev)
{
char temp[255];
sprintf(temp,"Mouton/%s/%s.cfg",Niveau.Level[lev],Niveau.Level[lev]);
if ((Fichier_Cfg=fopen(temp,"w"))!=NULL)
{
fprintf(Fichier_Cfg,LEVCFG,2,0,2,Back[0],Back[1]);
}
}

void Lire_Level_Cfg(char lev)
{
register cont;
char temp[256];
unsigned char ch;

sprintf(temp,"Mouton/%s/%s.cfg",Niveau.Level[lev],Niveau.Level[lev]);
if ((Fichier_Cfg=fopen(temp,"r"))!=NULL)
{
while (strcmp(temp,"END"))
{
fscanf(Fichier_Cfg,"%s",&temp);
if(!strcmp(temp,"END"))
	continue;
while((fgetc(Fichier_Cfg))!='=');
if (!strcmp(temp,"CIEL"))
	{
	fscanf(Fichier_Cfg,"%d",&ch);
	Options.Ciel=ch;
	if(Options.Ciel)
	{
	while(fgetc(Fichier_Cfg)!='#');
	fscanf(Fichier_Cfg,"%d ",&Ciel_Coul.R_Start);
	fscanf(Fichier_Cfg,"%d ",&Ciel_Coul.V_Start);
	fscanf(Fichier_Cfg,"%d ",&Ciel_Coul.B_Start);

	fscanf(Fichier_Cfg,"%f ",&Ciel_Coul.R);
	fscanf(Fichier_Cfg,"%f ",&Ciel_Coul.V);
	fscanf(Fichier_Cfg,"%f ",&Ciel_Coul.B);
	}

	}
if (!strcmp(temp,"BACK"))
	{
	fscanf(Fichier_Cfg,"%d",&ch);
	for (cont=0;cont<ch;cont++)
	{
	while(fgetc(Fichier_Cfg)!='#');
	fscanf(Fichier_Cfg,"%s",&Back[cont]);
	}
	}
while(fgetc(Fichier_Cfg)!='-');
}
printf("%s.cfg charge",Niveau.Level[lev]);
}
else Ecrire_Level_Cfg(lev);
}




