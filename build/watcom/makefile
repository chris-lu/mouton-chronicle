# Mouton Chronicles - Open Watcom build (16-bit DOS, large model)
#
# Normally driven by the scripts at the repository root:
#   scripts\build.cmd          -> out\mouton.exe and the tools
#   scripts\run.cmd            -> copies data\ next to the exe as Mouton\ and starts DOSBox-X
#   scripts\build.cmd clean
# Direct use: call scripts\env.cmd, cd build\watcom, wmake [target]

CC     = wcc
LINK   = wlink
SRC    = ..\..\src
TOOLS  = ..\..\tools
OUT    = out

# -ml large model  -3 386 code  -bt=dos  -fpi x87 with emulation  -s no stack checks
# -ox optimize     -zq quiet    -w2 warnings
# -j  'char' is signed, as in Borland C (the code stores -1 in char variables)
# -zdp DS pegged to DGROUP, as in Borland C: the inline asm reads globals
#      through DS (les di,Video ...); Open Watcom lets DS float by default
#      and only its own generated code uses SS for globals
CFLAGS = -q -ml -3 -bt=dos -fpi -s -ox -zq -w2 -j -i=$(SRC)
LFLAGS = system dos option quiet option stack=16384

GAME_SRC = $(SRC)\mouton.c $(SRC)\mouton.h $(SRC)\compat.h $(SRC)\armes.c &
	$(SRC)\moutgest.c $(SRC)\graph.c $(SRC)\plans.c $(SRC)\palette.c &
	$(SRC)\texte.c $(SRC)\option.c $(SRC)\start.c $(SRC)\clavier.c &
	$(SRC)\mouse.c $(SRC)\mouse.h $(SRC)\cd.c $(SRC)\xms.c $(SRC)\xms.h &
	$(SRC)\vga.c $(SRC)\vga_c.c $(SRC)\vga.h

# editpal.c is an unfinished work file (never compiled in 2000 either); build it
# explicitly with "wmake out\editpal.exe" once it is repaired.
all : $(OUT) $(OUT)\mouton.exe $(OUT)\credits.exe $(OUT)\greeting.exe &
	$(OUT)\credits2.exe $(OUT)\rotpal.exe $(OUT)\lignecnt.exe

$(OUT) :
	if not exist $(OUT) mkdir $(OUT)

# the game is one translation unit: mouton.c includes every other module
$(OUT)\mouton.obj : $(GAME_SRC)
	$(CC) $(CFLAGS) -fo=$^@ $(SRC)\mouton.c

$(OUT)\credits.obj : $(TOOLS)\credits.c $(SRC)\vga.c $(SRC)\vga_c.c $(SRC)\mouse.c $(SRC)\cd.c
	$(CC) $(CFLAGS) -fo=$^@ $(TOOLS)\credits.c

$(OUT)\greeting.obj : $(TOOLS)\greeting.c $(SRC)\compat.h
	$(CC) $(CFLAGS) -fo=$^@ $(TOOLS)\greeting.c

$(OUT)\credits2.obj : $(TOOLS)\credits2.c $(SRC)\compat.h
	$(CC) $(CFLAGS) -fo=$^@ $(TOOLS)\credits2.c

$(OUT)\editpal.obj : $(TOOLS)\editpal.c $(SRC)\compat.h $(SRC)\vga.c $(SRC)\vga_c.c $(SRC)\vga.h $(SRC)\mouse.c
	$(CC) $(CFLAGS) -fo=$^@ $(TOOLS)\editpal.c

$(OUT)\rotpal.obj : $(TOOLS)\rotpal.c $(SRC)\compat.h $(SRC)\vga.c $(SRC)\vga_c.c $(SRC)\vga.h
	$(CC) $(CFLAGS) -fo=$^@ $(TOOLS)\rotpal.c

$(OUT)\lignecnt.obj : $(TOOLS)\lignecnt.c $(SRC)\compat.h
	$(CC) $(CFLAGS) -fo=$^@ $(TOOLS)\lignecnt.c

$(OUT)\mouton.exe : $(OUT)\mouton.obj
	$(LINK) $(LFLAGS) name $^@ file $[@

$(OUT)\credits.exe : $(OUT)\credits.obj
	$(LINK) $(LFLAGS) name $^@ file $[@

$(OUT)\greeting.exe : $(OUT)\greeting.obj
	$(LINK) $(LFLAGS) name $^@ file $[@

$(OUT)\credits2.exe : $(OUT)\credits2.obj
	$(LINK) $(LFLAGS) name $^@ file $[@

$(OUT)\editpal.exe : $(OUT)\editpal.obj
	$(LINK) $(LFLAGS) name $^@ file $[@

$(OUT)\rotpal.exe : $(OUT)\rotpal.obj
	$(LINK) $(LFLAGS) name $^@ file $[@

$(OUT)\lignecnt.exe : $(OUT)\lignecnt.obj
	$(LINK) $(LFLAGS) name $^@ file $[@

clean : .SYMBOLIC
	if exist $(OUT) rmdir /s /q $(OUT)
	if exist run rmdir /s /q run
