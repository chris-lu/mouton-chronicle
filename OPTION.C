void Ecrire_Jeu_Cfg(void)
{
register cont;
if ((Fichier_Cfg=fopen("Mouton/Mouton.cfg","w"))!=NULL)
	{
	fprintf(Fichier_Cfg,JEUCFG,Options.Pts_2_Vie,Options.Nb_Joueurs,Options.Nb_Plans,Options.Fallow,Niveau.Cur_Level,Niveau.Nb_Level);
		for (cont=0;cont<Niveau.Nb_Level;cont++)
			{
			fprintf(Fichier_Cfg,"# %s\n",Niveau.Level[cont]);
			}
		fprintf(Fichier_Cfg,"-MENUCOUL = 1\n#");
		for (cont=0;cont<12;cont++)
			{
			fprintf(Fichier_Cfg,"%d ",CoulMul[cont]);
			}
	fprintf(Fichier_Cfg,"\n");
	fprintf(Fichier_Cfg,"-END");
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
		while(fgetc(Fichier_Cfg)!='-');
		fscanf(Fichier_Cfg,"%s",&temp);
		if(!strcmp(temp,"END"))
			continue;
		while((fgetc(Fichier_Cfg))!='=');
		fscanf(Fichier_Cfg,"%d",&ch);
		if (!strcmp(temp,"JOUEURS"))
			Options.Nb_Joueurs=ch;
		else if (!strcmp(temp,"MENUCOUL"))
			{
			while(fgetc(Fichier_Cfg)!='#');
			for(cont=0;cont<12;cont++)
				fscanf(Fichier_Cfg,"%d",&CoulMul[cont]);
			}
		else if (!strcmp(temp,"PLANS"))
			Options.Nb_Plans=ch;
		else if (!strcmp(temp,"SUIVRE"))
			Options.Fallow=ch;
		else if (!strcmp(temp,"VITALITE"))
			Options.Pts_2_Vie=ch;
		else if (!strcmp(temp,"CURNIV"))
			Niveau.Cur_Level=ch;
		else if (!strcmp(temp,"NIVEAUX"))
		 {
			Niveau.Nb_Level=ch;
			for (cont=0;cont<Niveau.Nb_Level;cont++)
				{
				while(fgetc(Fichier_Cfg)!='#');
				fscanf(Fichier_Cfg,"%s",&Niveau.Level[cont]);
				}
		}
//		Options.Nb_J_Tot=Options.Nb_Joueurs*Options.Nb_Equipes;
		}
	TX=Options.Nb_Plans*320;
	fclose(Fichier_Cfg);
//	printf("\nmouton.cfg charge");
	}
else exit(1);
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
		while(fgetc(Fichier_Cfg)!='-');
		fscanf(Fichier_Cfg,"%s",&temp);
		if(!strcmp(temp,"END"))
			continue;
		while((fgetc(Fichier_Cfg))!='=');
		fscanf(Fichier_Cfg,"%d",&ch);
		if (!strcmp(temp,"CIEL"))
			{
			Options.Ciel=ch;
			while(fgetc(Fichier_Cfg)!='#');
			fscanf(Fichier_Cfg,"%s",&Back[2]);
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
		else if (!strcmp(temp,"TRACK"))
      	Track=ch;
		else if (!strcmp(temp,"BACK"))
			for (cont=0;cont<ch;cont++)
				{
				while(fgetc(Fichier_Cfg)!='#');
				fscanf(Fichier_Cfg,"%s",&Back[cont]);
				}
		else if (!strcmp(temp,"SOL"))
			for (cont=0;cont<ch;cont++)
				{
				while(fgetc(Fichier_Cfg)!='#');
				fscanf(Fichier_Cfg,"%d",&PalTemp[cont*3]);
				fscanf(Fichier_Cfg,"%d",&PalTemp[cont*3+1]);
				fscanf(Fichier_Cfg,"%d",&PalTemp[cont*3+2]);
				}
		else if (!strcmp(temp,"DECORE"))
			{
			Nb_Spr=ch;
			for (cont=0;cont<Nb_Spr;cont++)
				{
				while(fgetc(Fichier_Cfg)!='#');
				fscanf(Fichier_Cfg,"%d",&Deco_Inf[cont].X);
				fscanf(Fichier_Cfg,"%d",&Deco_Inf[cont].Y);
				fscanf(Fichier_Cfg,"%d",&Deco_Inf[cont].Lg);
				fscanf(Fichier_Cfg,"%d",&Deco_Inf[cont].Ht);
				}
			}
		else if (!strcmp(temp,"GRAVITE"))
			{
			while(fgetc(Fichier_Cfg)!='#');
			fscanf(Fichier_Cfg,"%f",&Gravite);
			}
		}
//	printf("\n%s.cfg charge\n",Niveau.Level[lev]);
	}
else exit(1);
}





void Ecrire_Joueur_Cfg(void)
{
register cont,cont2;
if ((Fichier_Cfg=fopen("Mouton/Equipes.cfg","w"))!=NULL)
	{
	fprintf(Fichier_Cfg,"-EQUIPES = 12\n");
	for(cont=0;cont<12;cont++)
		{
		fprintf(Fichier_Cfg,"# %s,",Eq_Name[cont]);
		for(cont2=0;cont2<8;cont2++)
			fprintf(Fichier_Cfg,"%s, ",Mouton[cont*8+cont2].Nom);
		fprintf(Fichier_Cfg,"%d %d %d\n",Team_Coul[cont*3],Team_Coul[cont*3+1],Team_Coul[cont*3+2]);
		}
	fprintf(Fichier_Cfg,"-END\n");
	}
}



void Lire_Joueur_Cfg(void)
{
register cont,cont2,cont3;
char temp[256];
unsigned char ch,ch2;
char cha;
sprintf(temp,"Mouton/Equipes.cfg");
if ((Fichier_Cfg=fopen(temp,"r"))!=NULL)
	{
	while (strcmp(temp,"END"))
		{
		while(fgetc(Fichier_Cfg)!='-');
		fscanf(Fichier_Cfg,"%s",&temp);
		if(!strcmp(temp,"END"))
			continue;
		while((fgetc(Fichier_Cfg))!='=');
		if (!strcmp(temp,"EQUIPES"))
			{
			fscanf(Fichier_Cfg,"%d",&ch);
			for (cont=0;cont<12;cont++)
				{
				cont3=0;
				while(fgetc(Fichier_Cfg)!='#');
				while((cha=fgetc(Fichier_Cfg))==' ');     //efface les blancs au debut des noms
				fseek(Fichier_Cfg,-1,1);
				while((cha=fgetc(Fichier_Cfg))!=',')
				  Eq_Name[cont][cont3++]=cha;
				Eq_Name[cont][cont3]='\0';
			  //	fscanf(Fichier_Cfg,"%s",&Eq_Name[cont]);
			//	while((cha=fgetc(Fichier_Cfg))!=',');
				for(cont2=0;cont2<8;cont2++)
					{
					cont3=0;
					while((cha=fgetc(Fichier_Cfg))==' ');     //efface les blancs au debut des noms
					fseek(Fichier_Cfg,-1,1);
					while((cha=fgetc(Fichier_Cfg))!=',')
					  Mouton[cont*8+cont2].Nom[cont3++]=cha;
					Mouton[cont*8+cont2].Nom[cont3]='\0';
					Mouton[cont*8+cont2].Equipe=cont;
					}
				fscanf(Fichier_Cfg,"%d",&Team_Coul[cont*3]);
				fscanf(Fichier_Cfg,"%d",&Team_Coul[cont*3+1]);
				fscanf(Fichier_Cfg,"%d",&Team_Coul[cont*3+2]);
				}
			}
		}
	//Options.Nb_J_Tot=8*12;
	}
else exit(1);
}




void Lire_Lang(void)
{
register cont,cont2;
char var[255];
char cha1,cha2;
sprintf(var,"Mouton/Lang.fr");
if ((Fichier_Cfg=fopen(var,"r"))!=NULL)
	{
	while (strcmp(var,"END"))
		{
		while(fgetc(Fichier_Cfg)!='-');
		fscanf(Fichier_Cfg,"%s",&var);
		if(!strcmp(var,"END"))
			continue;
		while((fgetc(Fichier_Cfg))!='=');
		if (!strcmp(var,"T1"))
			{
			fscanf(Fichier_Cfg,"%d",&cha2);
			for (cont=0;cont<cha2;cont++)
				{
				cont2=0;
				while(fgetc(Fichier_Cfg)!='#');

				while((cha1=fgetc(Fichier_Cfg))!='\n')
				Paroles[cont][cont2++]=cha1;
			 //	Paroles[cont][cont2++]='\0';
				}
			}
		}
	}
}






