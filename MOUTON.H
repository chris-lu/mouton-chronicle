//#define ERROR	{ ModeTxt();	printf("error %X XMS",nb);	getch();	exit(0);	}
//#define LEVCFG "-DECORE = %d \n-CIEL = %d\n# 0 0 0 0.000000 0.000000 0.000000 \n-BACK = %d\n#%s\n#%s\n-END"
#define JEUCFG "-VITALITE = %d\n-JOUEURS = %d \n-PLANS = %d \n-SUIVRE = %d\n-LECTCD = %d\n-MELANGE = %d\n-CURNIV = %d \n-LANG = %d\n#%s\n-NIVEAUX = %d\n"
//#define GRAV  0.049    // 9.81/200
#define VERSION "0.13 Beta"

#include <stdio.h>
#include <conio.h>
#include <math.h>
#include <malloc.h>
#include <time.h>
#include <string.h>
//#include "mouse.h"
#include "vga_c.c"
#include "vga.c"

#define NBDEB 100

unsigned short Y[200];			//Tableau de position des abscisses sur l'écran : Pos=320*Y
float SinT[256];				//Tableau de valeurs sinus...
float CosT[256];
char A,B,C,D;				//Variables aleatoires pour la génération des terrain
char CurBomb=0;				//Bombe sur l'écran (pour les explosions)
char LastExB[4]={0,0,0,0};		//Explosion précédente
char PalNb=0;				//Pallette utilisé : accélère les rotation de pallete
unsigned char Palrot=0;			//Conteur pour les rotation de Pal.
char Paroles[70][50];			//Paroles des mouton
unsigned short Par[4][3];		//Tableau de valeurs pour les paroles (mouton/parole)
unsigned char Cur_Par;

struct{					//structure des options
	unsigned char Ciel;		//Type de ciel
	char Pts_2_Vie;			//points de vie
	char Fallow;			//Suivie du monton
	unsigned char Nb_Equipes;	//Nombre d'equipes
	unsigned char Nb_Joueurs;	//Nombre de joueurs par equipe
	unsigned char Nb_J_Tot;		//nombre total de joueurs
	unsigned char Nb_Plans;		//largeur du terrain
	unsigned char Melange;		//Melanger les equipes?
}Options;

struct{					//structure des statistiques
	unsigned char Deb;
	unsigned char Longueur;
}Rotation[4];


struct{					//structure des statistiques
	unsigned char Meilleur;
	unsigned long Precis;
	unsigned short Bourrin;
	short Inutile;
	unsigned char Mouton[4];
}Stats;


struct{					//Structure pour la position de etoiles
	unsigned short  X[200];		//Coordonnés : X
	unsigned char Y[200];		//    "      : Y
	unsigned char Coul[200];	//couleur ...
}Stars; 

struct{					//Sctructure pour le degradé du ciel.
	unsigned char R_Start;		//Valeur initiales
	unsigned char V_Start;
	unsigned char B_Start;
	float R;				//valeur de degrade
	float V;
	float B;
}Ciel_Coul;

struct{					//structure pour les 2 trous (petits et grands)
	unsigned char Taille;
	unsigned char *Spr;
}Trou[2];

struct{					//Structure pour les explsoions
	unsigned short Used;		//Structure Utilisée ?
	float X[NBDEB];			//Coordonnés de chaque eclat : X
	float Y[NBDEB];			//     "       "     "       : Y
	float DirX[NBDEB];		//Directions de  chaque eclats
	float DirY[NBDEB];		//
	unsigned char Coul[NBDEB];	//Couleur de l'eclat
}Explose[4];				//4 Structures explosions

struct{					//Stucture pour la bombe
	char Used;				//la bombe est utilise???
	char Explosion;   	    	//La bombe vient d'explose ?
	float DirX;				//Direction de la bombe
	float DirY;				//     "     "  "  "
	float PosX;				//Coordonnés de la bombe
	float PosY;				//      "      "  "   "
}Bombe;               


struct {					//Structures des mouton
	short Pts_Vie;			//points de vie du monton
	unsigned char Equipe;		//equipe du monton
	char Frags;				//nombre de tue
	short Degats;			//degats que le mouton a inflige
	char Nom[17];			//nom du mouton
	char Cur_Spr;			//Sprite actuele
	char Mouv;				//permet la limite de defilement des sprites
	char Mort;				//Mouton est mort?
	char Bloque;			//peut plus bouger
	char Sens;				//Sens du mouton	(gauche au droite)
	unsigned short Bouge;		//temps en l'air
	float PosX;				//Position
	float PosY;
	float DirX;				//direction
	float DirY;
}*Mouton;

struct{					//Structure NIveau
	char Level[20][9];		//Nom des niveau
	unsigned char Nb_Level;		//nombre de niveaux total
	unsigned char Cur_Level;	//niveau actutel
}Niveau;

char Eq_Name[12][17];			//nom des equipes

Stock[12][6];

struct{					//structure pour les decors 
	unsigned short X;	
	unsigned short Y;
	unsigned short Lg;
	unsigned short Ht;
}Deco_Inf[5];

char Back[3][13]={"Back1.pcx","Back2.pcx","Backstar.pcx"};//Fichiers pour le second plan
unsigned char far *Plan_1[4];		//Page pour le premiers plan : 3 parties
unsigned char far *Plan_2[2];		//Page pour le second plan : 2 parties
unsigned char far *Deco_Spr[5];	//Sprite des decors
unsigned char far *Page;		//Page de tavail
unsigned char far PalTemp[24];      //Pallettes temporaire
unsigned char far Team_Coul[49];	//Couleurs des equipes
unsigned char far *Pal[3];		//Pallettes : 3 différantes pour tout travail sur les pallettes
unsigned char far *Lettre[96];	//Lettre
unsigned char far TrSpr1[24*24];	//Sprite trou
unsigned char far TrSpr2[46*46];	//Sprite trou
unsigned char far Lune[24*24];      //Sprite des astres : lune, soleil, terre
unsigned char far *Mout_Spr[8*8];	//sprite des mouton...

void far interrupt (*oldit_9)(void);//Sauvegarde de l'ancienne interruption clavier
void far interrupt Lire_scan(void);	//Interruption clavier : reprogrammé pour les scancodes
//void far interrupt Vide(void);	//Interruption vide
unsigned short Lx1,Ly1,Lx2,Ly2,x,y; //Position de la souris

char far Scan_Code[128];		//Tableau des scancodes

//short test=0;
unsigned char Pv[8]={0,16,32,64,72,136,152,216};//palletes differantes

short Vise=128;				//position du curseur
float Puissance;				//Puissance du tir
char Tir=0;					//vient de tirer
float Vent=0;				//Vent
float Add_Vent=0;				//Modif du vent sur les objets
char Cur_Joueur=0;			//Joueur actuel
unsigned short Wait=0;			//Attente
//unsigned char ok;				//??
unsigned char Nb_Spr;			//Nombre de sprites decor
unsigned char Deb_Tour=1;		//Afficher les noms et pts de vie
//int far XMS_Page[2];
char Last_Key;				//Derniere touche enfoncé
//char Survivants;

 unsigned short TX;			//longueur du terrain en pixels

//FILE *Fichier_Cfg;

unsigned char Rnd1[256];	//tableau de valeurs aleatoires
unsigned char Rnd2[256];
unsigned char Rnd3[256];
unsigned char Rnd4[256];
char Rnd5[256];
char Rnd6[256];
//largeur des lettres	(evite un trop grand espacement..)
char far Let_Lg1[96]={4,2,4,10,7,12,10,2,4,4,0,8,3,5,2,6,8,6,7,7,8,8,8,8,7,8,2,3,7,6,7,7,12,10,8,8,8,7,7,9,9,3,6,9,7,11,10,9,8,9,8,7,8,10,10,12,10,9,8,5,6,5,7,7,2,7,7,6,7,7,5,8,8,2,4,7,3,11,7,7,8,7,6,5,5,7,7,9,7,7,7,7,8,1,8,6};
char far Let_Lg[96]={2,1,3,5,5,4,4,2,2,2,3,3,2,3,1,4,3,3,3,3,3,3,3,3,3,3,1,1,2,3,2,3,4,4,4,4,4,4,4,5,4,3,4,4,4,4,5,4,4,5,4,4,4,4,4,4,4,4,4,2,4,2,2,3,4,3,3,3,3,3,3,3,3,1,2,3,2,5,3,3,3,3,2,2,2,3,3,5,3,3,3,3,5,3,4};
															  //.											  //A                       //N                          //Z        //a             //i                           //x
//texte du menu...
//char Txt[15][25]={"MOUTON CHRONICLES"," Jouer ","Options","Equipes","Resulat"," Sortie","Suite","Retour","OK","{","}","Vitalite :","Nombre de joueurs :","Taille du terrain :","Niveau :"};
char Txt[26][30]={"MOUTON CHRONICLES","","","","","","","","","","","","","",""};

struct		//structure pour les menu
{
unsigned short X;
unsigned char Y;
unsigned short Lg;
unsigned char Ht;
}Obj[26];

short CoulMul[13]={0,0,4,0,4,-1,4,-1,-1,-1,-1,-1};     //couleurs du menu : ici->bleu

unsigned char RndVar;	//compteur pour les tableau de variables aleatoires
char Cur_Obj;		//Objet acuel dans le menu
char Winner;		//gagnant

char Liste[96];		//Liste de l'ordre de jeu des mouton
char ListePos[96];	//Liste de la position des mouton

char Flash[2];		//Tableau pour les flash
unsigned char Arme;	//arme actuelle
//unsigned char TypeArme[2]={0,1};
void (*Proc[6])(void);	//pointeur vers fonction pour les armes



unsigned char nb1=0;	//nombre utilise pour les effets de chaleur, etc
unsigned char nb2=0;
unsigned char nb3=40;
char tab1[256];		//pareil
char tab2[256];
char tab3[256];
unsigned short Tremble=0;//intensite des tremblements de l'ecran
unsigned char Suicide;	//compteur pour l'animation suicide
char Rebonds;		//nombre de rebonds

unsigned short EclPos;	//position d'un eclair
float Gravite=0.049;	//facteur de gravite (gavite/200 : 9.81/200)
char TeamListe[12];	//Liste des equipes

unsigned short Pos[16*4]={90,20,0,0,   145,60,54,14,  145,80,54,14,   145,100,54,14, 145,120,54,14, 145,140,54,14,   250,155,54,14  ,30,155,54,14  ,150,155,54,14,   300,50,14,14,  300,135 ,14,14,  10,68,250,14, 10,86,280,14, 10,104,280,14,10,122,280,14,10,50,280,14};
char CURJ;			//Joueur actuel par rapport a son ordre de jeu
char far *Pointeur;      //pointeur pour le clavier;
char Fallow;		//suivre le mouton
unsigned short Chal;	//compeur pour les effets de chaleur
unsigned short Ecl;	//compteurs pour les eclairs
char ArmName[6][15]={"Bazooka","Kamikaze","Super Missile","Super Kamikaze","Colere Divine","MediKit"};	//Nom des armes

unsigned char Track;	//piste audio actuelle

char CURT;			//equipe actuelle par rapport a l'ordre de jeu du monton

unsigned char Panoplie[3][6]={1,1,0,0,0,0,1,1,1,1,0,0,1,1,1,1,1,1};	//disponibilite des armes predefinie

unsigned char Espace;	//espaces entre 2 elements du decors
unsigned char NbRot;
void *LetType[2]={Let_Lg,Let_Lg1};
unsigned char Let;
char lang[5]={"fr"};

unsigned short Light=0;
unsigned char Orage=1;
unsigned char FadeOut;
unsigned char FinPart;
unsigned char NoExit=0;




FILE *Fichier_Cfg;

//armes.c
void Init_Bomb(void);
unsigned char Test_Line(void);
void ArmBombe(void);
void ArmBombe2(void);
void ArmSuicide(void);
void ArmSuicide2(void);
void Eclairs(void);
void MediKit(void);



//Mouton.c
void Put_Foy(void);
char On_Obj(char Deb);
void Mk_Liste(void);
void Rnd_Liste(void);
void Init_Long(void);
void Drawy(unsigned char *src);
void Mem_Error(void) ;
void Init_Mem1(void);
void Init_Mem(void);
void Init_Tab(void);
void Draw_Paroles(void);
void Draw_Scores (void);
void jouer(void);
void Load_Graph_Menu(void);
void FlamesAsm(unsigned char *src,unsigned char* dst);
void Put_Cur(void);
void BurnScreen(void);
void Draw_Obj(unsigned short X,unsigned short Y,char *txt,char Nb,unsigned char Coul);
void Draw_Obj_Nb(unsigned short X,unsigned short Y,unsigned short N,char Nb,unsigned char coul);
void OptionsScreen(void);
void TeamScreen(void);
char SelectScreen(void);
void StatsScreen(void);







//Texte.c
void Txt_Down(void);
void Load_Txt(void);
void Print_Let(unsigned short x,byte y,byte lh,byte ht,byte *scr,byte Nb,byte Coul);
void Print_Txt(unsigned short x,unsigned char y,char * txt,unsigned char *Page,unsigned char Coul);
void Print_Nb(unsigned short x,unsigned char y,unsigned short nb,unsigned char *Page,unsigned char Coul);








//Pallette.c
void Mk_Snow(unsigned char far *Palette);
void Mk_Blood(unsigned char far *Palette);
void Mk_Sky(unsigned char far *Palette);
void Mk_Stars(unsigned char far *Palette);
void Rot_Pal(unsigned short deb,unsigned short nb, unsigned char *Pal1,unsigned char *Pal2);
void CpyPal(unsigned char *Pal1,unsigned char *Pal2);
void Inc_Pal(unsigned char * Pal);
void Dec_Pal(unsigned char * Pal);
void Goto_Pal(unsigned char *Pal2,unsigned char *Pal1);
void Create_Pal(unsigned char src,unsigned char nb,unsigned char* Pal1,unsigned char dst,unsigned char *Pal2);
void Init_Pal(unsigned char *p);
void Inc_Scr(unsigned char nb,unsigned char far *Scr);






//Plans.c
void Gen_Back(void);
void Draw_Back(unsigned char * Page);
void Draw_Deg(unsigned char * Page,unsigned char coul);
void CpyPlan(unsigned short x,unsigned short y,unsigned char **Plan,unsigned char *Page);
void Init_Explose(unsigned short debx,unsigned short deby,unsigned char coul);
void End_Bomb(char nb);
void Bomb_Ex(char nb);
void Print_Barre(unsigned short x,unsigned short y,short longueur,short haut,char pourcent,unsigned char coul1,unsigned char coul2,char sens);
void Look_Obj(short sx,short sy,unsigned short *dx,unsigned short *dy);


//Moutgest.c
void Message(char type,char modif,char Jou);
void Test_Dom(unsigned char MaxDom);
void Move_U(char nb);
void Put_Mouton(unsigned short x,char nb);
void Draw_Mouton(char nb);
void Remove_Mouton(char nb);
void Move_Mouton(char nb);
void Put_Viseur(char nb);
char Gagnant(void);
void Mk_Stats(void);
void Install_Clav(void);
void Remove_Clav(void);
void far interrupt Lire_scan(void);
void Lire_Keys(char nb);


//graph.c
void Put_Spr(unsigned short x,unsigned short y,unsigned short l,unsigned short h,unsigned char *buf);
void Init_Ter(void);
void Put_trou(unsigned short x,unsigned short y,unsigned short l,unsigned short h,unsigned char *buf);
void Load_Ter(void);
void Init_Graph(void);

//Option.c
void Ecrire_Jeu_Cfg(void);
void Lire_Jeu_Cfg(void);
void Lire_Level_Cfg(char lev);
void Ecrire_Joueur_Cfg(void);
void Lire_Joueur_Cfg(void);
void Lire_Lang(void);


//Start.c
void inittab(void);
void cpyh(unsigned short haut,short pos,unsigned char *page1,unsigned char *page2);
void cpyv(unsigned short col,short pos,unsigned char *page1,unsigned char *page2);
void start(void);


void SetMouse(unsigned short x,unsigned short y);

