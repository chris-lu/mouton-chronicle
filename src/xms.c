/*
  int xms_installed(void);
  void xms_init(void);

  unsigned int xms_getversion(void);
  unsigned int xms_getfreemem(void);

  char xms_allocate(int far *handle, unsigned int size);
  void xms_reallocate(int handle, unsigned int newsize);
  void xms_free(int far *handle);
  void xms_move(MOVEPARAMS far *params);
								  */



#include "compat.h"
#include <string.h>
//#pragma option -w- // Prevent "Function should return a value" warning
#include "xms.h"
void far *xms_driver = NULL;                /* Pointer to XMS driver */
void hma_on(void)
{
	 _asm {
	push ds
	push ss
	pop ds			//DS=DGROUP (Open Watcom laisse DS flotter)
		mov  ah, 0x03
		call [xms_driver]
	pop ds
		}
}

void hma_off(void)
{
	 _asm {
	push ds
	push ss
	pop ds			//DS=DGROUP (Open Watcom laisse DS flotter)
		mov  ah, 0x04
		call [xms_driver]
	pop ds
		}
}

void hma_free(void)
{
	 _asm {
	push ds
	push ss
	pop ds			//DS=DGROUP (Open Watcom laisse DS flotter)
		mov  ah, 0x02
		call [xms_driver]
	pop ds
		}
}


void hma_alloc(void)
{
	 _asm {
	push ds
	push ss
	pop ds			//DS=DGROUP (Open Watcom laisse DS flotter)
		mov  ah, 0x01
		mov dx,0xffff
		call [xms_driver]
	pop ds
		}
}

char xms_installed(void)  //verifie 'installation de la mémoire
  {
	 unsigned char al;
	 _asm {
		mov  ax, 0x4300
		int  0x2F
		mov  al,al
	 }
	 if (al == 0x80)
		return 1;
	 else
		return 0;
  }

void xms_version( int *v, int *rev, int *hma)
  {
	int ve,r,h;
	 _asm {
	push ds
	push ss
	pop ds			//DS=DGROUP (Open Watcom laisse DS flotter)
		mov  ax, 0x0000
		call [xms_driver]
		mov ve,ax
		mov r,bx
		mov h,dx
	pop ds
	 }
	 (*v)=ve;
	 *rev=r;
	 *hma=h;
		}


void xms_init(void)
  {
	 _asm {
	push ds
	push ss
	pop ds			//DS=DGROUP (Open Watcom laisse DS flotter)
		mov  ax, 0x4310
		int  0x2F
		mov  word ptr [xms_driver], bx
		mov  word ptr [xms_driver+2], es
	pop ds
	 }
  }


/*unsigned int xms_getversion()
  {
	 _asm {
	push ds
	push ss
	pop ds			//DS=DGROUP (Open Watcom laisse DS flotter)
		mov  ah, 0x00
		call [xms_driver]
	pop ds
	 }
  }  */


  void xms_mem_info( int *maxblk,long *frem)
  {
	 unsigned int fre=0,block=0;
	 _asm {
	push ds
	push ss
	pop ds			//DS=DGROUP (Open Watcom laisse DS flotter)
		mov  ah, 0x08
		call [xms_driver]
		mov  fre, dx
		mov  block, ax
	pop ds

	 }
  *frem=fre;
  *maxblk=block;
}

/*unsigned int xms_getfreemem()
  {
	 _asm {
	push ds
	push ss
	pop ds			//DS=DGROUP (Open Watcom laisse DS flotter)
		mov  ah, 0x08
		call [xms_driver]
		mov  ax, dx
	pop ds
	 }
  }   */


char xms_allocate(int far *handle,  int size)
  {
	char error=0;
	unsigned short h;
	 _asm {
	push ds
	push ss
	pop ds			//DS=DGROUP (Open Watcom laisse DS flotter)
		mov  ax, 0x0900
		mov  dx, size
		call [xms_driver]
		cmp ax,1
		jne err
		mov  h, dx
		jmp fin
		err:
		mov error,bl
		fin:
	pop ds
	 }
  if(!error)
  {
  *handle=h;
  }

  return(error);
  }


void xms_reallocate(int handle,  int newsize)
  {
	 _asm {
	push ds
	push ss
	pop ds			//DS=DGROUP (Open Watcom laisse DS flotter)
		mov  ah, 0x0F
		mov  bx, newsize
		mov  dx, handle
		call [xms_driver]
	pop ds
	 }
  }


void xms_free(int far *handle)
  {
	 _asm {
	push ds
	push ss
	pop ds			//DS=DGROUP (Open Watcom laisse DS flotter)
		mov  ah, 0x0A
		les  di, [handle]
		mov  dx, es:[di]
		call [xms_driver]
		mov  word ptr es:[di], 0
	pop ds
	 }
  }


void xms_move(MOVEPARAMS far *params)
  {
	 _asm {
		push ds
		push ss
		pop ds			//DS=DGROUP : Open Watcom laisse DS flotter, les globales (xms_driver) sont lues via DS
		push es
		mov  ax, ds
		mov  es, ax
		mov  ah, 0x0B
		lds  si, [params]
		call es:[xms_driver]        // necessary to work in huge model
		pop  es
		pop  ds
	 }
  }


void movepagemem(char far* page,int far * handle)
{
MOVEPARAMS  para;
para.length=64000;
para.sourcehandle=0;
para.sourceoffset=(long)page;
para.desthandle=*handle;
para.destoffset=0;
xms_move(&para);
}


void movememvid(int far * handle,char bank)
{
MOVEPARAMS  para;
para.length=65536;
para.sourcehandle=*handle;
para.sourceoffset=(long)bank*65536;
para.desthandle=0;
para.destoffset=(long)Video;
xms_move(&para);
}

void movemempage(int far * handle,unsigned char far* page)
{
MOVEPARAMS  para;
para.length=64000;
para.sourcehandle=*handle;
para.sourceoffset=0;
para.desthandle=0;
para.destoffset=(long)page;
xms_move(&para);
}









//#pragma option -w  // Re-enable "function should return a value" warning

