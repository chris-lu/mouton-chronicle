/****************************************************************************/
// TITRE   : BIBLIOTHEQUE DE GESTION DE LA SOURIS
// FICHIER : MOUSE.H
/****************************************************************************/

#include "compat.h"
#include <dos.h>

void InitMouse(void);
//Role   : initialise la souris

void MouseON(void);
//Role   : affiche la souris

void MouseOFF(void);
//Role   : efface la souris

unsigned short GetMouse(unsigned short *x,unsigned short *y);
//Role   : renvoie l'etat des boutons et la position de la souris
//Entree : x, y
//Sortie : bouton : 0 (aucun), 1 (gauche), 2 (droite), 3 (gauche et droite)

void SetMouse(unsigned short x,unsigned short y);
//Role   : positionne la souris
//Entree : x, y

void ZoneMouse(unsigned short x1,unsigned short y1,unsigned short x2,unsigned short y2);
//Role   : Definit une zone rectangulaire dans laquelle la souris peut se
          
//Entree : xmin, ymin, xmax, ymax

