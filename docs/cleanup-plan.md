# Mouton Chronicles — Folder review and proposed architecture

Reviewed on 2026-09-13. 806 files, ~110 MB. Original development 1998-11 → 2001-08 (Bac 1999/2000
computer-science project by Christophe Dri / GnoMaster, Jonathan Jolivalt / dJo, Aymeric Baudin / Bart57).

---

## 1. What the program actually is

* A 2-D turn-based artillery game ("Worms" style) with sheep. Up to 12 teams of 8 sheep,
  6 weapons (Bazooka, Kamikaze, Super Missile, Super Kamikaze, Colère Divine, MediKit), procedural
  terrain, day/night sky, storms, CD-audio music.
* Target: **16-bit real-mode DOS**, VGA mode 13h (320×200×256), Borland C++ (`asm { }` inline
  blocks, `far` pointers, `farmalloc`, `setvect`, `int86`, BIOS/DOS/MSCDEX interrupts).
* Version string in source: `0.13 Beta` (`Mouton.h`). The manual says `0.11b Beta`.
* Build model: **unity build**. `MOUTON.C` does `#include "xxx.c"` of every module; `Mouton.h`
  contains *all* global variables and all prototypes. There is exactly one translation unit.

### The canonical (latest) source set — 14 files

| File | Role | Last edit |
|---|---|---|
| `MOUTON.C` | main(), menu screens (options / teams / select / stats), game loop `jouer()`, fire effect | 2001-06-04 |
| `Mouton.h` | ALL globals + structs + prototypes; includes `vga_c.c` and `vga.c` | 2001-04-26 |
| `ARMES.C` | Weapons (missile physics, kamikaze, lightning, medikit) | 2000-07-18 |
| `MOUTGEST.C` | Sheep logic: damage, movement, falling, death, stats, speech messages | 2000-09-05 |
| `GRAPH.C` | Terrain generation, décor sprite placement, sprite/palette loading | 2000-04-01 |
| `PLANS.C` | Parallax planes, sky gradient, stars, explosion particles, UI bars, camera | 2000-10-15 |
| `PALETTE.C` | Palette build/rotate/fade helpers | 2000-04-09 |
| `TEXTE.C` | Bitmap font rendering (game + menu fonts) | 2000-04-10 |
| `OPTION.C` | Reads/writes `Mouton/Mouton.cfg`, `Equipes.cfg`, `<Level>/<Level>.cfg`, `Lang.xx` | 2000-12-13 |
| `START.C` | Intro screens (Atlantys / Presente / Mouton) with slide effects | 2000-09-05 |
| `CLAVIER.C` | INT 9 keyboard hook (scancode table) + key handling | 2000-10-11 |
| `MOUSE.C` / `MOUSE.H` | INT 33h mouse | 2000-05-02 |
| `CD.C` | MSCDEX (INT 2Fh) CD-audio play/stop/track info | 2000-11-02 |
| `Xms.c` / `Xms.h` | XMS memory driver wrapper (initialised in main but never actually used for pages) | 2001-06-06 |
| `VGA.C` / `VGA_C.C` / `VGA.H` | Third-party VGA library (Siracusa Walter, "wiSdom"), modified: mode switch, vsync, PCX loader, blits, sprites | 2001-06-06 / 2000-10-15 |

Latest binary: root `MOUTON.EXE` (2001-06-06, 113 748 bytes), built from the above with
`MOUTON.IDE` (Borland C++ 4.x/5.x IDE project). `MOUON.DSW` is a Turbo C++ / BC 3.1 DOS-IDE desktop
file pointing to `C:\PROGRA~2\PROGRA~1\EXE\SIEGE\` — the project was first called *Siège*.

### Runtime data — `Mouton/` (must sit next to the EXE, paths are hard-coded as `Mouton/...`)

* `Mouton.cfg` (game options, level list), `Equipes.cfg` (12 teams × 8 names + RGB),
  `Lang.fr` / `Lang.en` (menu strings + 70 speech lines; EN is unfinished), `config.txt` + `FAQ.txt`
  (level-file format doc), `temp.txt` (font width tables, scratch).
* Global art: `sprites.pcx` (sheep 8×8 sprites, 4 rows × 8), `font.pcx`, `FontM.pcx` (menu font),
  `FontC.pcx`, `atlantys.pcx` / `Presente.pcx` / `mouton.pcx` (intro), `screen0-5.pcx`,
  `BackStar/BackSun/BackEart/BackGore/BackVide.pcx` (sky sprites: moon/sun + crater sprites).
* Levels (one folder each, `<Name>/<Name>.cfg` + `Decor.pcx` + 2 background planes):
  `Neige`, `Montagne`, `Jungle`, `Lune`, `Plage`, `Frissons`. `Nouveau/` is the **template** for a new level.
* Music is **not** in the data folder: the game plays **CD audio tracks** by number (`-TRACK = n` in
  each level cfg, track 3 = menu). Track numbers match the file names in `Musiques/Wav/`
  (2 Water, 3 Intro, 4 Neige, 5 Jyngle, 6 Space, 7 Plage, 8 Cimetière, 9 Crédits).

---

## 2. Everything else, classified

### 2a. Separate programs (each has its own `main()`; keep as *tools*, not part of the game)

| Source | Binary | What it is | Verdict |
|---|---|---|---|
| `Credits.c` | `credits.exe` (2000-12-13) | Scrolling credits screen with magnifier/blur effects, CD audio | keep → `tools/credits` |
| `DEFIL.C` | `DEFIL.EXE`, `Defil1-4.exe` | Ancestor of `Credits.c` (9 lines diff) | delete (superseded) |
| `GREETING.C` | `GREETING.EXE` (2000-12-13) | Text-mode 80×25 "greeting" scroller shown before the game | keep → `tools/greeting` |
| `CREDITS2.C` | `CREDITS2.EXE` | Variant of GREETING with timed chars | keep or merge with greeting |
| `MENU.C` + `TEXTEMEN.C` | `MENU.EXE` | Early menu prototype (fire effect), now inside `MOUTON.C` | delete |
| `EDITPAL.C` | — | Palette viewer/editor tool | keep → `tools/` |
| `ROTPAL.C` | `ROTPAL.EXE` | Palette-rotation test on `test.pcx` | keep → `tools/` (experiment) |
| `LIGNECNT.C` | `LIGNECNT.EXE` | Counts lines of code in the sources (the "2684 lignes" comment) | keep → `tools/` |
| `TEST.C`, `TESTCD.C`, `MEM.C`, `VESA.C` | `*.EXE` | one-off experiments (CD driver, memory, VESA info) | archive or delete |
| `COULEUR.EXE` (1999-06) | — | no source found | archive |

### 2b. Obsolete / superseded source (safe to delete once history is in git)

* `SHEEP.C`, `SHEEP2.C`, `SHEEP.PIF`: the game before it was renamed Mouton (Jan 2000).
* `MOUTON2.C` == `MOUTON2.CPP` (identical, Aug 2000), `MOUTON.CPP` (May 2000): older snapshots of
  `MOUTON.C` where `armes.c` was still inlined. `MOUTON.CBA` is another copy.
* `MOUT.C` became `MOUTGEST.C`. `MOVES.C` (12 lines) is dead. `PLANSB.C` is a variant of `PLANS.C`.
* `VARIABLE.C`, `VARI.C`, `VARIABLE.H`: all merged into `Mouton.h`. `PROC.C` is a prototype list, merged too.
* `VGAASM.C`: older VGA lib. `vga_c.asm`: **compiler-generated** assembly listing of `VGA_C.C` (2270 lines), not hand-written.
* `OBJETS.C` (1999): 3-D wireframe object experiment, unrelated to the game.
* `SIGN.C`, `RTIME.C`, `CHECK.C`, `HMA.C` (0 bytes): tiny experiments / dead.
* All 36 `*.BAK`: Borland IDE auto-backups (previous save of the same file). Delete.
* `Mouton.dsp / .ncb / .opt / .plg / resource.h`: a Visual C++ 6 project (2000-09-20) pointing at
  `Mouton.c`. It can never compile this code (16-bit asm, far pointers). Delete.

### 2c. Build artifacts (delete, `.gitignore` them)

`*.OBJ`, `*.MAP`, `MOUTON.CSM`, `MOUTON.OBR`, `Copie de BCWDEF.*`, `SavA3A1.TMP`, `Final/*.tds`,
`Final/*.obj`, `Final/*.~bp`, `Final/*.~cp`, `Final/autorun.res`.
`*.PIF`, `*.pif`, `Eteindre.lnk` are Windows 9x shortcuts. Delete.
`DOS4GW.EXE` is the Watcom DOS extender, never used by this 16-bit Borland build. Delete.

### 2d. Numbered folders `1` … `11` — the pre-git history

Each contains one zip = a snapshot of the whole source tree at that date:

| Folder | Date | Milestone |
|---|---|---|
| 1 | 1998-12-08 | `SHEEP.EXE` only (+ 3 pcx) |
| 1.5 | 1998-12-11 | first sources: VGA lib, mouse, `SHEEP.C` 3.8 KB |
| 2 | 1998-12-15 | `SHEEP2.C` appears, palette |
| 3 | 2000-01-05 | keyboard, `MaxFrame.txt` (perf notes) |
| 4 | 2000-01-16 | CD audio, XMS, `MOUT.C`, `SHEEP.C` 16 KB |
| 5 | 2000-01-19 | `GRAPH.C`, `OPTION.C` |
| 6 | 2000-01-23 | **renamed `MOUTON.C`**, `MOUTGEST.C` |
| 7 | 2000-02-02 | first level cfg files (Neige, Jungle), `Equipes.cfg` |
| 7.5 | 2000-02-09 | `MENU.C`, data moved into `mouton/` subfolder |
| 8, 9 | 2000-02-09 / 02-19 | release zips (EXE + data only) |
| 10 | 2000-03-22 | `GREETING.C`, `LIGNECNT.C`, levels Lune/Montagne |
| 11 | 2000-04-26 | `Credits.c`, `EDITPAL.C`, levels Plage/Frissons/Nouveau |

Plus `Projet Mouton.rar` (2002-03-08, 747 KB), which could not be listed here (no unrar on this machine).
It is probably a 2002 snapshot of the sources. **Extract and diff against root before deleting anything.**

Recommendation: import these 13 snapshots as **chronological git commits** (one commit per zip,
author date = zip date), then the root tree as the final commit, then delete the numbered folders.

### 2e. Releases / packaging

* `dos/Mouton Chronicles/` is a **distribution package** (2000-07-18): `Mouton.exe` (98 838 B,
  older than root `MOUTON.EXE`), `credits.exe`, `Greeting.exe`, `TEST.EXE`, `Manuel/` (HTML manual),
  `Mouton/` data, `Mouton.ico`. `dos/Mouton Chronicles.zip` is the same thing re-zipped in 2018.
* `dos/BIN, DOC, SRC, *.TXT` is **WDOSX 0.95 SR1**, a third-party 32-bit DOS extender. Not used by
  the game (the `Raccourci vers Stubit.exe.pif` suggests you once tried to stub the EXE with it).
  Delete, or move to `third_party/` with a note.
* `Final/` is **not** the final game. It is a Borland C++ Builder **CD autorun launcher**
  (`Autorun.exe` + `Autorun.inf` + `Sigle.avi` logo + `start.wav`). The form source it references
  (`..\projets\test.cpp`) is missing. Keep `Autorun.cpp/.bpr/.inf`, `Sigle.avi`, `start.wav` as release material.
* `Mouton Chronicles.iwz`, `Mouton Chronicles BAC.iwz`: InstallShield Express projects for the CD installer.
* `Licence.txt` (Bac confidential version) / `Licence2.txt` (free-to-copy version).
* `Mouton.ico`, `start.wav` (root, same as in Final/).

### 2f. Documentation / website

* `Projet html/Manuel/` (full manual with images) and `Manuel Light/` (no images) + 2 zips of the same.
  `dos/Mouton Chronicles/Manuel/` is an older copy (has `pers.htm`, lacks `desinst.htm`).
* `Projet html/Projet/Lire.htm` + `Projet.htm`: the "read me first" page and website skeleton for the Bac jury.
* `Mouton/config.txt`, `Mouton/FAQ.txt`, `dos/.../Clavier.txt`: level format + controls, worth turning into markdown.
* `MaxFrame.txt`: perf notes (337 fps). `tmp.txt`: keyboard mashing. Delete.
* **`Nouveau Document texte.txt` contains what looks like a login/password (`slyke@wanna`). Delete it and
  never commit it.**

### 2g. Art work files (keep, but out of the runtime tree)

* Root `*.psp` (Paint Shop Pro), `*.bmp`, `Nuclear.psd`, `pochette.psp` (CD cover), `photo1-3.psp`,
  `font.psp`, `lune.psp`, `temp*.psp`: source art.
* Root `*.pcx`: working copies; the ones that matter are already in `Mouton/`. Root
  `Bg11/Bg12/Bg21/Bg22/p11..p22/plan1-4/snow/moutons*.pcx` are from the 1998–2000 prototypes.
* `Captures/`: screenshots for the manual/website (2 PNG re-exports from 2021).
* `anim/`, `Animation*.gif`: animated GIF frames for the website.
* `Musiques/`: FastTracker 2 `.XM` modules (+ `.rar` of the same) and 2004 MP3 renders. 84 MB.
  The XM files are the true source; the MP3s are the "CD audio" content the game expects.

---

## 3. Code observations that drive the architecture

1. **One translation unit, ~150 globals.** Every module reads/writes globals declared in `Mouton.h`
   (`Mouton[]`, `Options`, `Bombe`, `Explose[]`, `Plan_1[]`, `Page`, `Pal[]`, `x`, `y`, …).
   Nothing has a header of its own. This is the first thing to fix, and it can be done while still
   building with Borland: give each module a `.h`, compile modules separately, link.
2. **Hardware access is concentrated**, which is good news for a later port. Per-file count of
   `asm{}` / `setvect` / `int86` / port I/O:

   | File | hits | What |
   |---|---|---|
   | `VGA_C.C` | 20 | mode 13h, vsync, blits, sprite copies (all inline asm) |
   | `Xms.c` | 14 | XMS driver calls |
   | `VGA.C` | 5 | palette DAC writes, PCX loader |
   | `MOUSE.C` | 5 | INT 33h |
   | `CLAVIER.C` | 5 | INT 9 hook, port 0x60 / 0x20 |
   | `PALETTE.C` | 5 | DAC writes |
   | `MOUTON.C` | 5 | `FlamesAsm`, `Drawy` (fire effect + page flip) |
   | `START.C` | 4 | `cpyh`/`cpyv` slide blits |
   | `CD.C` | 3 | MSCDEX INT 2Fh |
   | `PLANS.C` | 3 | `Draw_Deg`, `Draw_White` fills |
   | `TEXTE.C` | 1 | font blit |

   Everything else (`ARMES.C`, `MOUTGEST.C`, `GRAPH.C`, `OPTION.C`, most of `MOUTON.C`) is portable
   C that only touches byte buffers.
3. **Frame model**: fixed 320×200 8-bit `Page` work buffer, 3 layers (`Plan_1[]` terrain up to
   4×64 KB, `Plan_2[2]` parallax backgrounds, generated sky/stars), palette tricks for effects
   (rotation, flash, fade, heat). Timing is "one frame per vsync", no delta time.
4. **Third-party code**: `VGA.H/VGA.C/VGA_C.C` (Siracusa Walter, wiSdom; needs attribution),
   `Xms.c` (classic XMS wrapper), WDOSX (unused).
5. **Data formats** are all text (`-KEY = n` + `#` lines) or PCX. Easy to keep as-is.
6. **Sloppy bits worth noting** for later: `fscanf("%d", &unsigned char)` everywhere in `OPTION.C`
   (stack smashing on a 32-bit build), `Init_Ter` reads `Deco_Inf[temp]` with `temp` uninitialised,
   `Lire_Keys` writes `Panoplie[0][cont]` for `cont<18` on a `[3][6]` array (works only because it
   is contiguous), `fcloseall()` in the game loop, `exit(1)` on any config error.

---

## 4. Proposed repository layout

```
mouton-chronicles/
├── README.md                  what it is, how to build, how to run (DOSBox)
├── HISTORY.md                 the timeline above + credits + licence text
├── LICENSE                    (decide: Licence2.txt says free to copy unmodified)
├── .gitignore                 *.obj *.exe *.map *.csm *.obr *.bak *.tds *.~* build/
│
├── src/                       the game (one .c + one .h per module)
│   ├── main.c                 main(), start-up, main menu loop           <- MOUTON.C (part)
│   ├── menu.c / menu.h        OptionsScreen/TeamScreen/SelectScreen/StatsScreen, fire effect <- MOUTON.C (part)
│   ├── game.c / game.h        jouer(), turn logic, HUD                   <- MOUTON.C (part)
│   ├── state.h                the shared structs (Options, Mouton, Bombe, Niveau, Stats…) <- Mouton.h
│   ├── weapons.c / .h         <- ARMES.C
│   ├── sheep.c / .h           <- MOUTGEST.C
│   ├── terrain.c / .h         <- GRAPH.C
│   ├── planes.c / .h          <- PLANS.C
│   ├── palette.c / .h         <- PALETTE.C
│   ├── text.c / .h            <- TEXTE.C
│   ├── config.c / .h          <- OPTION.C
│   ├── intro.c / .h           <- START.C
│   └── platform/              everything that touches hardware
│       ├── video.h            ModeVGA/ModeTxt/Bal/Draw/Cpy/Clr/Put/Blka/GetBlk1/CpySpr/SetAllPala/LoadPCX
│       ├── input.h            keyboard scancodes + mouse
│       ├── audio.h            Play/Stop/Is_Playing
│       ├── memory.h           XMS (probably drop)
│       └── dos/
│           ├── vga.c          <- VGA.C + VGA_C.C (+ VGA.H, keep author credit)
│           ├── keyboard.c     <- CLAVIER.C (interrupt part only; key *handling* moves to game.c)
│           ├── mouse.c        <- MOUSE.C
│           ├── cdaudio.c      <- CD.C
│           └── xms.c          <- Xms.c
│
├── tools/                     separate DOS programs, each its own folder + main
│   ├── credits/               <- Credits.c
│   ├── greeting/              <- GREETING.C, CREDITS2.C
│   ├── editpal/               <- EDITPAL.C, ROTPAL.C
│   └── lignecnt/              <- LIGNECNT.C
│
├── data/                      runtime data, copied next to the EXE at build time  <- Mouton/
│   ├── mouton.cfg  equipes.cfg  lang.fr  lang.en
│   ├── gfx/                   sprites.pcx font*.pcx back*.pcx intro screens
│   └── levels/  neige/ montagne/ jungle/ lune/ plage/ frissons/  + _template/ (<- Nouveau)
│
├── assets/                    source art, not shipped
│   ├── psp/  bmp/  psd/       root *.psp *.bmp *.psd
│   ├── pcx-work/              root working *.pcx
│   ├── captures/              <- Captures/
│   └── anim/                  <- anim/ + Animation*.gif
│
├── music/
│   ├── xm/                    *.XM (the source)
│   └── mp3/                   Musiques/Wav/*.mp3 (CD tracks 2–9)
│
├── docs/
│   ├── manual/                <- Projet html/Manuel (keep one copy, the newest)
│   ├── site/                  <- Projet html/Projet, Projet.htm
│   ├── level-format.md        <- Mouton/config.txt + FAQ.txt + Nouveau.cfg
│   └── controls.md            <- Clavier.txt
│
├── release/                   historical packaging, read-only
│   ├── 2000-07-18-cd/         <- dos/Mouton Chronicles (EXE + data + manual)
│   ├── autorun/               <- Final/ (cpp, bpr, inf, avi, wav; no .obj/.tds)
│   └── installshield/         *.iwz, Licence*.txt, Mouton.ico
│
├── third_party/
│   └── wdosx/                 <- dos/BIN, DOC, SRC (or just delete; unused)
│
└── build/
    ├── dos/                   Borland makefile or .IDE/.PRJ, dosbox.conf
    └── (later) CMakeLists.txt for the SDL port
```

Naming: the current identifiers are French (`Mouton`, `jouer`, `Lire_Keys`). Keep them for the
first pass. Renaming is churn with no functional benefit and the git diff should stay readable.
Only the *files* get English names above; French file names are fine too if you prefer.

---

## 5. Proposed module architecture (after cleanup, still DOS)

```
             +----------+
             |  main.c  |  start-up, config load, main menu loop
             +----+-----+
        +---------+----------+
   +----v---+ +---v----+ +---v----+
   | menu.c | | game.c | |intro.c |
   +----+---+ +---+----+ +---+----+
        |   +-----+----------+------------+----------+
        |   |     |          |            |          |
   +----v---v+ +--v------+ +-v-------+ +--v------+ +-v--------+
   | text.c  | |weapons.c| | sheep.c | |terrain.c| | planes.c |   gameplay / rendering into byte buffers
   +----+----+ +--+------+ +-+-------+ +--+------+ +-+--------+
        |         |          |            |          |
        +---------+----------+-----+------+----------+
                             +-----v-----+   +----------+
                             | palette.c |   | config.c |
                             +-----+-----+   +----------+
                                   |
                    +--------------v------------------+
                    |  platform/  video . input . audio|   the ONLY files allowed to contain asm / int86 / setvect
                    +--------------+------------------+
                              platform/dos/*.c   (later: platform/sdl/*.c)
```

Rules that make this hold:
* `state.h` declares the shared game structs; globals become `extern` there and are *defined*
  in exactly one `.c` (`main.c` or a `state.c`). No module includes another `.c`.
* Nothing above `platform/` may use `asm`, `far`, `setvect`, `int86`, `outportb`, `farmalloc`.
  Use `typedef unsigned char far *page_t;` in `video.h` so the `far` disappears in one place.
* `platform/video.h` exposes exactly the calls the game already uses (`Draw`, `Cpy`, `Clr`, `Put`,
  `Blka`, `GetBlk1`, `CpySpr`, `CpyBnd`, `SetAllPala`, `LoadPCX`, `Bal`). The inline-asm
  `FlamesAsm`, `Drawy`, `Draw_Deg`, `Draw_White`, `cpyh`, `cpyv` move into `platform/dos/vga.c`
  and get plain-C equivalents later.
* Key *reading* (`Lire_scan`, scancode table) stays in platform; key *meaning* (`Lire_Keys`:
  fire, move, jump, aim, weapon change) moves to `game.c`.

This gives you a project that still builds with Borland C++ under DOSBox, but where a port to
SDL2 / Raylib (320×200 palettised buffer to texture, 8-bit palette emulated, OGG/MP3 instead of
CD audio, XM playback via libxmp) is a matter of writing `platform/sdl/*.c` (roughly 600
lines) without touching gameplay.

---

## 6. Suggested order of work

1. **Preserve history first.** `git init`; script the import of the 13 zips (+ the 2002 rar) as
   dated commits; commit the current root as-is. Nothing is lost after this, so deletion becomes safe.
2. **Delete noise**: `*.BAK`, `*.OBJ`, `*.PIF`, `*.lnk`, `.CSM/.OBR/.CBA/.MAP`, VC6 files, `DOS4GW.EXE`,
   `Defil*.exe`, `MENU.EXE`, duplicate zips, `Copie de *`, `tmp.txt`, and the password text file.
3. **Move** files into the layout of §4 (no code edits yet). Verify the game still runs from
   `data/`: keep a build step that copies `data/` to `Mouton/` next to the EXE, or change the
   6 hard-coded `"Mouton/..."` paths to a single `DATA_DIR` define.
4. **Get a reproducible DOS build**: Borland C++ 3.1 or 5.02 in DOSBox-X, a `makefile`, and a
   `dosbox.conf` with a mounted CD image (or `imgmount` of a `.cue` built from the MP3s) so music works.
5. **Split the unity build**: one `.h` per module, `extern` globals, compile per file, link. Still
   Borland, still same EXE behaviour. Fix the `fscanf` into `unsigned char` while there.
6. **Carve out `platform/`** as in §5. Now the code is "clean" and a port becomes a separate, optional project.

## 7. Open questions for you

* Is `Projet Mouton.rar` (2002) newer than the root sources? Extract it and diff before step 2.
* Do you still have Borland C++ (3.1 / 4.5 / 5.02) install media? It decides step 4.
* `COULEUR.EXE`, `TEST.EXE` in the release: throw away, or keep as curiosities in `release/`?
* Keep WDOSX at all? Nothing references it.
