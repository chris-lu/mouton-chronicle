/*
  int xms_installed(void);
  void xms_init(void);

  unsigned int xms_getversion(void);
  unsigned int xms_getfreemem(void);

  char xms_allocate(int far *handle, unsigned int size);
  void xms_reallocate(int handle, unsigned int newsize);
  void xms_free(int far *handle);
								  */
  typedef struct
	 {
		unsigned long length;         /* Length must be a multiple of two */
		unsigned int  sourcehandle;
		unsigned long sourceoffset;
		unsigned int  desthandle;
		unsigned long destoffset;
	 } MOVEPARAMS;

  void xms_move(MOVEPARAMS far *params);


#include <mem.h>
//#pragma option -w- // Prevent "Function should return a value" warning

static void far *xms_driver = NULL;                /* Pointer to XMS driver */

int xms_installed(void)  //verifie 'installation de la mémoire
  {
	 asm {
		mov  ax, 0x4300
		int  0x2F
	 }
	 if (_AL == 0x80)
		return 1;
	 else
		return 0;
  }

void xms_version(unsigned int *v,unsigned int *rev,unsigned int *hma)
  {
	int ve,r,h;
	 asm {
		mov  ax, 0x0000
		call [xms_driver]
		mov ve,ax
		mov r,bx
		mov h,dx
	 }
	 (*v)=ve;
	 *rev=r;
	 *hma=h;
		}


void xms_init(void)
  {
	 asm {
		mov  ax, 0x4310
		int  0x2F
		mov  word ptr [xms_driver], bx
		mov  word ptr [xms_driver+2], es
	 }
  }


/*unsigned int xms_getversion()
  {
	 asm {
		mov  ah, 0x00
		call [xms_driver]
	 }
  }  */


  void xms_mem_info(unsigned int *maxblk,unsigned long *frem)
  {
	 unsigned int fre=0,block=0;
	 asm {
		mov  ah, 0x08
		call [xms_driver]
		mov  fre, dx
		mov  block, ax

	 }
  *frem=fre;
  *maxblk=block;
}

/*unsigned int xms_getfreemem()
  {
	 asm {
		mov  ah, 0x08
		call [xms_driver]
		mov  ax, dx
	 }
  }   */


char xms_allocate(int far *handle, unsigned int size)
  {
	char error=0;
	unsigned short h;
	 asm {
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
	 }
  if(!error)
  {
  *handle=h;
  }

  return(error);
  }


void xms_reallocate(int handle, unsigned int newsize)
  {
	 asm {
		mov  ah, 0x0F
		mov  bx, newsize
		mov  dx, handle
		call [xms_driver]
	 }
  }


void xms_free(int far *handle)
  {
	 asm {
		mov  ah, 0x0A
		les  di, [handle]
		mov  dx, es:[di]
		call [xms_driver]
		mov  word ptr es:[di], 0
	 }
  }


void xms_move(MOVEPARAMS far *params)
  {
	 asm {
		push ds
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


void movepagemem(unsigned char far* page,int far * handle)
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

