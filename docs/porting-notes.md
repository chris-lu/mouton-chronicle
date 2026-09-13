# Porting notes: from Borland C++ 4.5 to Open Watcom 2.0

Written on 2026-09-13, after the game was made to build and run again with
Open Watcom. This is the technical record: what differed between the two
compilers, what was changed in the sources, which latent bugs surfaced, and
how the hardest problem was tracked down. Read it before touching the inline
assembly or the build flags.

## 1. Summary

* Target unchanged: 16-bit real-mode DOS, large memory model, VGA mode 13h.
* Build: `build/watcom/makefile`, driven by `scripts/build.cmd`; run and test
  in DOSBox-X (`scripts/run.cmd`, `scripts/smoke-test.ps1`).
* `src/compat.h` maps the Borland-only library calls to Open Watcom.
* Three Borland behaviours had to be reproduced: signed `char`, DS always on
  the data segment, and a `random()` macro that accepts a float.
* Six latent bugs in the 2001 code were fixed; they only worked under Borland
  by luck of memory layout.
* Verified in DOSBox-X: intro, main menu, Options and Teams screens, and a
  round on the Frissons level. Not verified: keyboard control during play,
  CD audio, the Borland build itself.

## 2. Compiler options

`CFLAGS = -q -ml -3 -bt=dos -fpi -s -ox -zq -w2 -j`

| Option | Why |
|---|---|
| `-ml` | Large model: far code and far data, as the 2001 link map shows. `unsigned char *Video = (unsigned char*)0xA0000000` only works with far data pointers. |
| `-3` | 386 code. Required: the blitters write `db 66h` before `rep movsw`/`rep stosw` to move dwords, and Open Watcom refuses that prefix with `-0`/`-2`. |
| `-bt=dos` | DOS target and libraries (`lib286\dos\clibl.lib`). |
| `-fpi` | Inline x87 with emulation. `-fpi87` and `-fpc` also work (tested); DOSBox always has an FPU. |
| `-s` | No stack-overflow checks (the original had none; the checks would also run inside the keyboard interrupt handler). |
| `-j` | **`char` is signed**, as in Borland. Without it every sprite and menu text is misplaced (see §3.1). |
| `-zq -w2` | Quiet, warning level 2. |

Options deliberately **not** used:

* `-zp1` (byte packing). The sheep structure is 46 bytes with either packing,
  and `-zp1` also repacks the C library's own structures (`FILE`), which
  breaks `fopen` at run time. The MSCDEX request blocks in `src/cd.c` happen to
  be laid out identically either way.
* `-zdp` (DS pegged to DGROUP). It would have fixed the inline asm (§3.2) but
  the C library is built for a floating DS and returns with DS changed, so
  pegged callers then read their globals from the wrong segment: the program
  silently died in the config reader.

Link: `wlink system dos option stack=16384`. The 2001 build had a tiny default
stack; 16 KB leaves room for the screens' local arrays.

## 3. Differences between the compilers, and the fixes

### 3.1 `char` signedness

Borland: `char` is signed. Open Watcom: unsigned by default. The code stores
−1 in `char` everywhere: the sheep direction `Sens`, the `Rnd5`/`Rnd6` jitter
tables that shake the menu text, `On_Obj()` returning −1, `Signe` in the
missile code, `CurT` going below zero. With unsigned `char` the intro drew torn
vertical strips and the menu text landed off screen. Fix: `-j`.

### 3.2 DS: pegged under Borland, floating under Open Watcom

This one cost the most. Borland keeps DS on the data segment for the whole
program; every function can assume `DS == DGROUP` on entry and must restore it.
Open Watcom 16-bit code lets DS float: a function may load DS with another
segment (a far pointer's segment, the far-data segment of a `far` global) and
return without restoring it, because its *own* generated code always accesses
DGROUP through **SS** (the stack is in DGROUP). That is a valid convention, and
the C library follows it.

The inline assembly of this game, however, reads globals through DS as Borland
guaranteed: `les di,Video` in `Drawy()` and `Draw()`, `mov bx,TX` in
`Look_Obj()`, `xms_driver` in `src/xms.c`, `Cur_Pos` in the text-mode tools.
After a call such as `Print_Txt()` (which leaves DS on the far-data segment of
`Let_Lg1`), `Drawy()` loaded a garbage far pointer and copied 63 KB to a random
address. Which address depended on what DS held, hence on code and data layout
and on timing: a textbook heisenbug (see §6).

Fix applied to every asm block that names a global (11 blocks):

```
push ds
push ss
pop ds          ; DS = DGROUP: Open Watcom lets DS float, globals are read via DS
...
pop ds
```

Not applied to interrupt handlers (`Lire_scan`): their prologue already loads
DS with DGROUP, and SS is the interrupted program's stack there.

### 3.3 Symbol binding inside `_asm`

Open Watcom's inline assembler resolves names inconsistently when a parameter
or local has the same name as a global: in a test, `les di,y` bound the
two-byte global instead of the pointer parameter (and refused to assemble),
while `les di,x` was accepted. Parameters that shadowed globals were renamed
in `GetMouse`, `SetMouse`, `Put`, `Print_Let`, `Draw_Deg`, `Draw_White`,
`Inc_Pal`, `Dec_Pal`, `SetAllPala`, `Loupe`, `Move`. Rule for the future: no
parameter or local may share a name with a global in a function that contains
asm.

### 3.4 Labels

Asm labels are visible across the whole translation unit. `Look_Obj()` used
`suite:`/`fin:` like other blocks and the later `jmp suite` failed with the
misleading error "Expression must be integral" (reported on the wrong line:
Open Watcom miscounts lines across asm blocks). Labels in that function were
renamed `lo_*`. Rule: labels unique per file.

### 3.5 Borland's `random()` accepts a float

`random(3.14*200)` in `Init_Explose`. Borland's macro is
`(int)(((long)rand()*(num))/(RAND_MAX+1))`, which works with a float argument;
`compat.h` reproduces that macro exactly rather than using `%`.

### 3.6 Floats as array subscripts

Borland silently converted `Plan_1[(Bombe.PosX+cont*Signe)/320]`-style
indexes; Open Watcom rejects them ("Expression must be integral"). Explicit
casts were added in `armes.c`, `moutgest.c`, `credits.c`. Semantics unchanged
(truncation toward zero).

### 3.7 Small things

* `asm { }` becomes `_asm { }`; `if (Mod) _asm {...} else _asm {...}` needs
  braces around each block.
* `<alloc.h>` and `<mem.h>` do not exist: `compat.h` and `<string.h>`.
* `M_PI` is not in Open Watcom's `math.h`: defined in `compat.h`.
* The Borland pseudo-register `_AL` (`xms_installed`) became an asm store.
* `farmalloc/farfree/farcalloc`, `setvect/getvect`, `outportb/inportb`,
  `randomize`, `coreleft`, `clrscr`, `textcolor`, `gotoxy`, `_setcursortype`:
  macros or small BIOS-based functions in `compat.h`. `delay`, `kbhit`,
  `getch`, `cprintf`, `int86`, `MK_FP` exist in Open Watcom.
* `fcloseall()` exists in Open Watcom (`_fcloseall` does not).
* `Stock[12][6];` (implicit `int`) got its type.
* `vga.c` now includes `vga.h` itself (a tool included it before `vga_c.c`).
* `credits.c` had a stray `*/` inside an asm block that Borland tolerated.
* Structure packing: Open Watcom's default gives the same 46-byte sheep
  structure that `farcalloc(96,46)` hard-codes. Do not add `-zp1` (§2).

## 4. Latent bugs fixed

These are bugs of the 2001 code that Borland's memory layout hid:

1. `start.c`: the intro loop loaded `Fich[screen]` *before* testing
   `screen == 3`, reading a fourth element of a three-element array and
   passing stack garbage as a file name to the PCX loader, which copied it
   into a 45-byte global buffer.
2. `option.c`: `fscanf("%d", &x)` into `unsigned char` variables and structure
   fields (two bytes written into one). Reads now go through an `int`.
3. `cd.c`: with no CD present the track scan wrote `Tracks[-1]`.
4. `plans.c`: `Draw_Deg` pushed ES then DS but popped ES then DS, swapping the
   two registers on exit (used in play, for the sky gradient).
5. Parameters shadowing globals in asm functions (§3.3).
6. The `*/` in `credits.c` (§3.7).
7. `Lire_scan` (INT 9 keyboard handler, `clavier.c`) stored scancodes into
   `Scan_Code[]` through the global `Pointeur` read as `les di,Pointeur`, which
   needs DS=DGROUP. With DS floating, an interrupt during play loaded a garbage
   pointer and wrote scancodes to random memory. It corrupted live data in
   proportion to how much the player moved (more key events), so the game
   "crashed while just moving". Fixed by addressing `Scan_Code` with immediate
   `seg`/`offset` (DS/SS-independent). This is the same DS-floating family as
   §3.2 but in an interrupt, where DS is truly arbitrary.

## 5. Rules for the inline assembly in this code base

1. A block that names a global must start with `push ds / push ss / pop ds`
   and end with `pop ds`. Locals and parameters are fine (they are on the
   stack, addressed through SS/BP).
2. No parameter or local named like a global in a function with asm.
3. Labels unique per file.
4. Balanced `push`/`pop`, in the right order (a script in the history of this
   file audited that; `Draw_Deg` was the only offender).
5. Registers may be clobbered freely: Open Watcom saves what it needs around
   `_asm` blocks (verified with a test that kept register variables live
   across a block clobbering SI, DI, BX, DX).
6. Interrupt handlers (`__interrupt __far`): Open Watcom does NOT load DS in
   the prologue, and an interrupt fires with an arbitrary DS. A handler that
   touches a global must not read it through DS. Load the global's far
   address as linker-relocated immediates instead
   (`mov di,offset V / mov ax,seg V / mov es,ax`), which needs no segment
   register set up. `push ss; pop ds` is NOT safe here: SS is the interrupted
   code's stack and may not be DGROUP if the interrupt lands during a DOS or
   BIOS call. See `Lire_scan` in `src/clavier.c`.
7. `db 66h` before `rep movsw` is still accepted with `-3`; `rep movsd` would
   be the clean spelling.

## 6. How the DS bug was found

Kept here because the same approach will serve the refactoring.

* **Symptom**: at full speed the menu crashed within two frames; slowed down
  by any per-frame file write it ran for minutes. The DOSBox-X log showed
  "Illegal Unhandled Interrupt Called 6" storms, "Write ... to rom at fe03c",
  a triple fault, then "DOS fatal memory error: Corrupt MCB chain". Clicking
  Options crashed the same way for the author.
* **Not the cause** (each tested in isolation): the dynamic CPU core
  (`core=normal` crashed too), `-fpi` emulation (`-fpi87` crashed too), the
  optimiser (`-od` crashed too), structure packing, heap overruns (the
  allocation pattern was replayed in a test program), Watcom's handling of
  variables named in asm (tested), register preservation around asm (tested).
* **Instrumentation** that did not disturb timing: a debug-only probe that
  walks the DOS memory-control-block chain the way DOSBox does (start from
  INT 21h/52h, follow `M` blocks to `Z`) and writes to a log only when it
  breaks; and `_dos_allocmem(0xFFFF)`, an impossible request that forces DOS
  to walk the whole chain so that DOSBox flags corruption at that exact
  point. `_fheapchk()` is unusable in this runtime (reports a bad node even
  in a trivial program).
* **Elimination**: a `SKIP` bit mask compiled into the menu loop to drop one
  call at a time. Dropping `Put_Cur` or `Drawy` "fixed" it, which was
  misleading: they only changed the code layout and thus which segment DS
  happened to hold.
* **Disassembly**: `wdis` is broken in this Open Watcom build (exits with no
  output). The linker map (`wlink option map`) plus the Python `capstone`
  module reading the EXE gave the machine code of any function. Reading
  `Print_Txt` showed `mov ds,ax` with the far-data segment and no restore,
  and reading `Drawy` showed `les di,[Video]` through DS.
* **Confirmation**: a 30-line test program: a C function that walks a `far`
  array (leaves DS elsewhere) followed by an asm block doing `les di,Video`;
  it read `0000:0000` instead of `A000:0000` by default and the right value
  with `-zdp`.
* **Automation**: DOSBox-X was driven from PowerShell with a generated
  `.conf` (its `[autoexec]` mounts the run folder; `-c` arguments lose their
  quotes through PowerShell), screenshots taken with `PrintWindow(hwnd, hdc,
  2)` (GDI screen copies come back black), keys through `SendKeys`, and the
  emulator's stderr redirected to a file. `scripts/smoke-test.ps1` is the
  cleaned-up version.

## 7. Next steps

* Split the unity build into real modules, one `.h` per `.c`, `extern`
  globals defined once, compiled separately and linked. The makefile is ready
  for it (one rule per object). Run the smoke test after each step.
* Verify keyboard control in play (the INT 9 hook in `clavier.c`), then
  weapons and the end-of-round screens.
* CD audio through a mounted `.cue` image (docs/build-and-run.md §6).
* Repair or drop `tools/editpal.c`.
* The tool timer handlers `Coul` in `tools/greeting.c` and `tools/credits2.c`
  call `Dec_Scr(Page)` from an interrupt and read the global `Page` with a
  floating DS: same latent bug as `Lire_scan`, not yet fixed (the game does not
  use them).
* Optional: replace the `db 66h` prefixes by `rep movsd`/`rep stosd`.
