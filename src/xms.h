
  typedef struct
    {
      unsigned long length;
      unsigned int  sourcehandle;
      unsigned long sourceoffset;
      unsigned int  desthandle;
      unsigned long destoffset;
    } MOVEPARAMS;

  char xms_installed(void);
  void xms_init(void);
  void hma_alloc(void);
  void hma_on(void);
  void hma_off(void);
  void hma_free(void);
  void xms_version( int *v, int *rev, int *hma);
	void xms_mem_info( int *maxblk, long *frem);


  unsigned int xms_getversion(void);
  unsigned int xms_getfreemem(void);

 char xms_allocate(int far *handle,  int size);
  void xms_reallocate(int handle,  int newsize);
  void xms_free(int far *handle);


  void xms_move(MOVEPARAMS far *params);


