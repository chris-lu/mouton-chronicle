#define NBDEB 100

unsigned short Y[200];             //Tableau de position des abscisses sur l'écran : Pos=320*Y
float SinT[256];
float CosT[256];
char A,B,C,D;                        //Variables aleatoires pour la génération des terrain
char CurBomb=0;                    //Bombe sur l'écran
char LastExB[4]={0,0,0,0};           //Explosion précédente
char PalNb=0;                      //Pallette utilisé : accélère les rotation de pallete
unsigned char Palrot=0;            //Conteur pour les rotation de Pal.
char Paroles[70][50];
unsigned short Par[4][3];
unsigned char Cur_Par;

struct{
unsigned char Ciel;
char Pts_2_Vie;
char Fallow;
unsigned char Nb_Equipes;
unsigned char Nb_Joueurs;
unsigned char Nb_J_Tot;
unsigned char Nb_Plans;
}Options;


struct{
unsigned char Meilleur;
unsigned long Precis;
unsigned short Bourrin;
char Inutile;
unsigned char Mouton[4];
}Stats;


struct{
unsigned short  X[200];           //Coordonnés : X
unsigned char Y[200];             //    "      : Y
unsigned char Coul[200];
}Stars;           //Structure pour la position de etoiles


struct{
unsigned char R_Start;
unsigned char V_Start;
unsigned char B_Start;
float R;
float V;
float B;
}Ciel_Coul;

struct{
unsigned char Taille;
unsigned char *Spr;
}Trou[2];

struct{
unsigned short StartX;             //Coordonnés de l'explosion : X
unsigned short StartY;             //    "       "        "    : Y
unsigned short Used;                //Structure Utilisée ?
float X[NBDEB];                      //Coordonnés de chaque eclat : X
float Y[NBDEB];                      //     "       "     "       : Y
float DirX[NBDEB];                   //Directions de  chaque eclats
float DirY[NBDEB];                   //
unsigned char Coul[NBDEB];           //Couleur de l'eclat
}Explose[4];                       //3 Structures explosions

struct{
char Used;
char Explosion;       //La bombe explose ?
float DirX;           //Direction de la bombe
float DirY;           //     "     "  "  "
float PosX;           //Coordonnés de la bombe
float PosY;           //      "      "  "   "
}Bombe;               //Stucture pour la bombe


struct {
short Pts_Vie;
unsigned char Equipe;
char Frags;
short Degats;
char Nom[17];
char Cur_Spr;
char Mouv;
char Mort;
char Bloque;
char Sens;
unsigned short Bouge;
float PosX;
float PosY;
float DirX;
float DirY;
}*Mouton;

struct{
char Level[20][9];
unsigned char Nb_Level;
unsigned char Cur_Level;
}Niveau;

char Eq_Name[12][17];

Stock[12][6];

struct{
unsigned short X;
unsigned short Y;
unsigned short Lg;
unsigned short Ht;
}Deco_Inf[5];

char Back[3][13]={"Back1.pcx","Back2.pcx","Backstar.pcx"};     //Fichiers pour le second plan
unsigned char far *Plan_1[4];                 //Page pour le premiers plan : 3 parties
unsigned char far *Plan_2[2];                 //Page pour le second plan : 2 parties
unsigned char far *Deco_Spr[5];
unsigned char far *Page;                      //Page de tavail
unsigned char far PalTemp[24];                    //Pallettes : 3 différantes pour tout travail sur les pallettes
unsigned char far Team_Coul[49];                    //Pallettes : 3 différantes pour tout travail sur les pallettes
unsigned char far *Pal[3];                    //Pallettes : 3 différantes pour tout travail sur les pallettes
unsigned char far *Lettre[96];                //Lettre (5x5)
unsigned char far TrSpr1[24*24];                //Sprite trou
unsigned char far TrSpr2[46*46];                //Sprite trou
unsigned char far Lune[24*24];                //Sprite pour la lune
unsigned char far *Mout_Spr[8*8];

void far interrupt (*oldit_9)(void);          //Sauvegarde de l'ancienne interruption clavier
void far interrupt Lire_scan(void);           //Interruption clavier : reprogrammé pour les scancodes
//void far interrupt Vide(void);                //Interruption vide
unsigned short Lx1,Ly1,Lx2,Ly2,x,y;                           //Position de la souris

char far Scan_Code[128];                      //Tableau des scancodes

//short test=0;
unsigned char Pv[8]={0,16,32,64,72,136,152,216};

short Vise=128;
float Puissance;
char Tir=0;
float Vent=0;
float Add_Vent=0;
char Cur_Joueur=0;
unsigned short Wait=0;
unsigned char ok;
unsigned char Nb_Spr;
unsigned char Deb_Tour=1;
//int far XMS_Page[2];
char Last_Key;
//char Survivants;

 unsigned short TX;

//FILE *Fichier_Cfg;

unsigned char Rnd1[256];
unsigned char Rnd2[256];
unsigned char Rnd3[256];
unsigned char Rnd4[256];
char Rnd5[256];
char Rnd6[256];
char far Let_Lg1[96]={4,2,4,10,7,12,10,2,4,4,0,8,3,5,2,6,8,6,7,7,8,8,8,8,7,8,2,3,7,6,7,7,12,10,8,8,8,7,7,9,9,3,6,9,7,11,10,9,8,9,8,7,8,10,10,12,10,9,8,5,6,5,7,7,2,7,7,6,7,7,5,8,8,2,4,7,3,11,7,7,8,7,6,5,5,7,7,9,7,7,7,7,8,1,8,6};
char far Let_Lg[96]={2,1,3,5,5,4,4,2,2,2,3,3,2,3,1,4,3,3,3,3,3,3,3,3,3,3,1,1,2,3,2,3,4,4,4,4,4,4,4,5,4,3,4,4,4,4,5,4,4,5,4,4,4,4,4,4,4,4,4,2,4,2,2,3,4,3,3,3,3,3,3,3,3,1,2,3,2,5,3,3,3,3,2,2,2,3,3,5,3,3,3,3,5,3,4};
															  //.											  //A                       //N                          //Z        //a             //i                           //x
char Txt[15][25]={"MOUTON CHRONICLES"," Jouer ","Options","Equipes","Resulat"," Sortie","Suite","Retour","OK","{","}","Vitalite :","Nombre de joueurs :","Taille du terrain :","Niveau :"};

struct
{
unsigned short X;
unsigned char Y;
unsigned short Lg;
unsigned char Ht;
}Obj[26];

short CoulMul[13]={0,0,4,0,4,-1,4,-1,-1,-1,-1,-1};      //bleu

unsigned char RndVar;
char Cur_Obj;
char Winner;

char Liste[96];
char ListePos[96];

char Flash[2];
unsigned char Arme;
//unsigned char TypeArme[2]={0,1};
void (*Proc[6])(void);



unsigned char nb1=0;
unsigned char nb2=0;
unsigned char nb3=40;
char tab1[256];
char tab2[256];
char tab3[256];
unsigned short Tremble=0;
unsigned char Suicide;
char Rebonds;

unsigned short EclPos;
float Gravite=0.049;
char TeamListe[12];

unsigned short Pos[16*4]={90,20,0,0,   145,60,54,14,  145,80,54,14,   145,100,54,14, 145,120,54,14, 145,140,54,14,   250,155,54,14  ,30,155,54,14  ,150,155,54,14,   300,50,14,14,  300,135 ,14,14,  10,68,250,14, 10,86,280,14, 10,104,280,14,10,122,280,14,10,50,280,14};
char CURJ;
char *Pointeur;      //pointeur pour le clavier;
char Fallow;
char Redraw;
unsigned short Chal;
unsigned short Ecl;
char ArmName[6][15]={"Bazooka","Kamikaze","Super Missile","Super Kamikaze","Colere Divine","MediKit"};

unsigned char Track;

char CURT;

unsigned char Panoplie[3][6]={1,1,0,0,0,0,1,1,1,1,0,0,1,1,1,1,1,1};
