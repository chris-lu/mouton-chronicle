/*************************************************************/
// TITRE   : BIBLIOTHEQUE GRAPHIQUE VGA STANDARD (320x200x256)
// AUTEUR  : SIRACUSA WALTER & PATRICE / wiSdom
// FICHIER : VGA.H
/*************************************************************/

/* ----------------INCLUSION DES BIBLIOTHEQUES ------------- */

#ifndef vga_head
#define vga_head

#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <io.h>
#include <fcntl.h>
#include <sys\stat.h>

/* ---------------- DECLARATION DES CONSTANTES ------------- */

#define MAXX 320   //Largeur de l'ecran
#define MAXY 200   //Hauteur de l'ecran
#define OX 160     //Abscisse du centre de l'ecran
#define OY 100     //Ordonnee du centre de l'ecran

/* ------------------ DECLARATION DES MACROS --------------- */

#define VBL asm         \
				{               \
					mov dx,0x3da; \
				  truc:         \
					in al,dx;     \
					test al,8;    \
					jnz truc;        \
				}
//Role : Attend un retour vertical du balayage ecran (Vertical BLank)

#define HBL asm         \
				{               \
					mov dx,0x3da; \
				  Bid:       \
					in al,dx;     \
					test al,1;    \
					jnz Bid;        \
				}
//Role : Attend un retour horizontal du balayage ecran (Horizontal BLank)

/* ------------------REDEFINITION DES TYPES ---------------- */

typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned long dword;
typedef unsigned char far bytef;
typedef unsigned short far wordf;
typedef unsigned long far dwordf;

/* ----------------DECLARATION DES PROTOTYPES -------------- */

void ModeVGA(void);
//Role : initialise le mode VGA standard 320x200x256

void ModeTxt(void);
//Role : initialise le mode texte 80x25

byte LoadPCX(char *path,bytef *scr,byte *pal);
//Role   : charge une image PCX (compression RLE) , 320x200x256
//Entree : chemin, page, palette
//Sortie : 0 (echec) ou 1 (succes)

byte SavePCX(char *path,bytef *scr,byte *pal);
//Role   : sauve une image au format PCX (compression RLE) , 320x200x256
//Entree : chemin, page, palette
//Sortie : 0 (echec) ou 1 (succes)

void Ink(byte e,byte r,byte v,byte b);
//Role   : modifie une encre en interne
//Entree : encre, rouge, vert bleu


void GetAllPal(byte *pal);
//Role   : memorise toute la palette de couleurs internes
//Entree : palette

void Box(short x,short y,short l,short h,bytef *scr,byte e);
//Role   : trace un rectangle
//Entree : x, y, largeur, hauteur, page, encre

void FBox(short x,short y,word l,word h,bytef *scr,byte e);
//Role   : trace un rectangle plein
//Entree : x, y, largeur, hauteur, page, encre

void Line(short x1,short y1,short x2,short y2,bytef *scr,byte e);
//Role   : trace une ligne
//Entree : x debut, y debut, x fin, y fin, page, encre

void Circle(short x,short y,word r,bytef *scr,byte e);
//Role   : trace un cercle
//Entree : x centre, y centre, rayon, page, encre

void GetBlk1(short x,short y,word l,word h,bytef *scr,bytef *buf);
//Role   : memorise un bloc (octet par octet)
//Entree : x, y, largeur, hauteur, page, buffer

void GetBlk2(short x,short y,word l,word h,bytef *scr,bytef *buf);
//Role   : memorise un bloc (mot par mot)
//Entree : x, y, largeur multiple de 2, hauteur, page, buffer

void GetBlk4(short x,short y,word l,word h,bytef *scr,bytef *buf);
//Role   : memorise un bloc (long mot par long mot)
//Entree : x, y, largeur multiple de 4, hauteur, page, buffer

void PutBlk1(short x,short y,word l,word h,bytef *scr,bytef *buf);
//Role   : affiche un bloc (octet par octet)
//Entree : x, y, largeur, hauteur, page, buffer

void PutBlk2(short x,short y,word l,word h,bytef *scr,bytef *buf);
//Role   : affiche un bloc (mot par mot)
//Entree : x, y, largeur multiple de 2, hauteur, page, buffer

void PutBlk4(short x,short y,word l,word h,bytef *scr,bytef *buf);
//Role   : affiche un bloc (long mot par long mot)
//Entree : x, y, largeur multiple de 4, hauteur, page, buffer

void PutSpr(short x,short y,word l,word h,bytef *scr,bytef *buf);
//Role   : affiche un bloc en transparence (encre 0 = transparence)
//Entree : x, y, largeur, hauteur, page, buffer

void CpyBlk(short xs,short ys,word l,word h,short xd,short yd,bytef *src,bytef *dst);
//Role   : copie un bloc
//Entree : x source, y source, largeur, hauteur, x destination, y destination,
//         page source, page destination

void CpyBlk1(short xs,short ys,word l,word h,short xd,short yd,bytef *src,bytef *dst);
//Role   : copie un bloc (octet par octet)
//Entree : x source, y source, largeur, hauteur, x destination, y destination,
//         page source, page destination

void CpyBlk2(short xs,short ys,word l,word h,short xd,short yd,bytef *src,bytef *dst);
//Role   : copie un bloc (mot par mot)
//Entree : x source, y source, largeur multiple de 2, hauteur, x destination,
//         y destination, page source, page destination

void CpyBlk4(short xs,short ys,word l,word h,short xd,short yd,bytef *src,bytef *dst);
//Role   : copie un bloc (long mot par long mot)
//Entree : x source, y source, largeur multiple de 4, hauteur, x destination,
//         y destination, page source, page destination

void CpySpr(short xs,short ys,word l,word h,short xd,short yd,bytef *src,bytef *dst);
//Role   : copie un bloc en transparence (encre 0 = transparence)
//Entree : x source, y source, largeur, hauteur, x destination,y destination,
//         page source, page destination

void CpyBnd(short ys,word h,short yd,bytef *src,bytef *dst);
//Role   : copie une bande de 320 pixels
//Entree : y source, hauteur, y destination, page source, page destination


void read_buf_pcx(int handle,byte *tampon,word *comp);
void write_buf_pcx(int handle,byte *tampon,word *comp);
static char signe(short v1,short v2);
#endif //vga_head
