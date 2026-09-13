/*************************************************************/
// compat.h : couche de compatibilite Borland C++ 4.5 -> Open Watcom
//
// Le jeu a ete ecrit pour Borland C++ (DOS 16 bits, modele large).
// Ce fichier fournit les fonctions et mots-cles propres a Borland
// quand on compile avec Open Watcom (wcc -ml -bt=dos). Sous Borland,
// il se contente d'inclure <alloc.h>.
/*************************************************************/
#ifndef MOUTON_COMPAT_H
#define MOUTON_COMPAT_H

#ifdef __WATCOMC__

#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <dos.h>
#include <i86.h>
#include <conio.h>
#include <time.h>

/* memoire lointaine (far heap) */
#define farmalloc(n)      _fmalloc(n)
#define farcalloc(n,s)    _fcalloc((n),(s))
#define farfree(p)        _ffree(p)
#define coreleft()        ((unsigned long)_memavl())

/* vecteurs d'interruption et ports */
#define setvect(n,h)      _dos_setvect((n),(h))
#define getvect(n)        _dos_getvect(n)
#define outportb(p,v)     outp((p),(v))
#define inportb(p)        inp(p)

/* nombres aleatoires : meme definition que la macro random() de Borland
   (stdlib.h), qui accepte un argument reel, ex. random(3.14*200) */
#define random(n)         ((int)(((long)rand()*(n))/(RAND_MAX+1)))
#define randomize()       srand((unsigned)time(NULL))

/* conio Borland : seulement utilise pour les messages en mode texte */
#define clrscr()          ((void)0)
#define textcolor(c)      ((void)0)

/* curseur texte (BIOS int 10h), memes constantes que Borland */
#define _NOCURSOR         0
#define _SOLIDCURSOR      1
#define _NORMALCURSOR     2

static void gotoxy(int x, int y)
{
	union REGS r;
	r.h.ah = 2;
	r.h.bh = 0;
	r.h.dh = (unsigned char)(y - 1);
	r.h.dl = (unsigned char)(x - 1);
	int86(0x10, &r, &r);
}

static void _setcursortype(int type)
{
	union REGS r;
	r.h.ah = 1;
	if (type == _NOCURSOR)      { r.h.ch = 0x20; r.h.cl = 0x00; }
	else if (type == _SOLIDCURSOR) { r.h.ch = 0x00; r.h.cl = 0x07; }
	else                        { r.h.ch = 0x06; r.h.cl = 0x07; }
	int86(0x10, &r, &r);
}

#else /* Borland C++ */

#include <alloc.h>

#endif

/* Borland math.h definit M_PI, pas Open Watcom */
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#endif /* MOUTON_COMPAT_H */
