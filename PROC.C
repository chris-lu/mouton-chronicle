void Put_Foy(void);
char On_Obj(char Deb);
void Mk_Liste(void);
void Init_Bomb(void);
void Rnd_Liste(void);
void Init_Long(void);
void Drawy(unsigned char *src);
void Mem_Error(void) ;
void Init_Mem1(void);
void Init_Mem(void);
void Init_Bomb(void);
void Init_Tab(void);
unsigned char Test_Line(void);
void ArmBombe(void);
void ArmBombe2(void);
void ArmSuicide(void);
void ArmSuicide2(void);
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








extern void Txt_Down(void);
void Print_Let(unsigned short x,unsigned char y,unsigned char *scr,unsigned char Nb,unsigned char Coul);
void Load_Txt(void);
void Print_Txt(unsigned short x,unsigned char y,char * txt,unsigned char *Page,unsigned char Coul);
void Print_Nb(unsigned short x,unsigned char y,unsigned short nb,unsigned char *Page,unsigned char Coul);
void Print_Let_Menu(unsigned short x,unsigned char y,unsigned char *scr,unsigned char Nb,unsigned char Coul);
void Load_Txt_Menu(void);
void Print_Txt_Menu(unsigned short x,unsigned char y,char * txt,unsigned char *Page,unsigned char Coul);
void Print_Nb_Menu(unsigned short x,unsigned char y,unsigned short nb,unsigned char *Page,unsigned char Coul);









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







void Gen_Back(void);
void Draw_Back(unsigned char * Page);
void Draw_Deg(unsigned char * Page,unsigned char coul);
void CpyPlan(unsigned short x,unsigned short y,unsigned char **Plan,unsigned char *Page);
void Init_Explose(unsigned short debx,unsigned short deby,unsigned char coul);
void End_Bomb(char nb);
void Bomb_Ex(char nb);
void Print_Barre(unsigned short x,unsigned short y,short longueur,short haut,char pourcent,unsigned char coul1,unsigned char coul2,char sens);
void Look_Obj(unsigned short sx,unsigned short sy,unsigned short *dx,unsigned short *dy);


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


void Put_Spr(unsigned short x,unsigned short y,unsigned short l,unsigned short h,unsigned char *buf);
void Init_Ter(void);
void Put_trou(unsigned short x,unsigned short y,unsigned short l,unsigned short h,unsigned char *buf);
void Load_Ter(void);
void Init_Graph(void);


void Ecrire_Jeu_Cfg(void);
void Lire_Jeu_Cfg(void);
void Lire_Level_Cfg(char lev);
void Ecrire_Joueur_Cfg(void);
void Lire_Joueur_Cfg(void);
void Lire_Lang(void);


void inittab(void);
void cpyh(unsigned short haut,short pos,unsigned char *page1,unsigned char *page2);
void cpyv(unsigned short col,short pos,unsigned char *page1,unsigned char *page2);
void start(void);