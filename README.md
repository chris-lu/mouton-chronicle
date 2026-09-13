# Mouton Chronicles

A 2-D turn-based artillery game ("Worms" with sheep) written in 1999–2001 for
16-bit MS-DOS in Borland C++, as the computer-science project of the
Baccalauréat 1999/2000 session.

* Up to 12 teams of 8 sheep, 6 weapons (Bazooka, Kamikaze, Super Missile,
  Super Kamikaze, Colère Divine, MediKit), procedurally generated terrain,
  parallax backgrounds, day/night skies, storms, palette effects.
* VGA mode 13h (320×200, 256 colours), keyboard + mouse, music played from
  CD-audio tracks through MSCDEX.
* Last source version: **0.13 beta** (June 2001). Last shipped version:
  the Bac 2000 CD (July 2000).

Authors (Atlantys team): Christophe Dri (GnoMaster), Jonathan Jolivalt (dJo),
Aymeric Baudin (Bart57). Music by the same team (FastTracker 2 modules).
The VGA library (`src/vga.c`, `src/vga_c.c`, `src/vga.h`) is the *wiSdom*
library by Walter Siracusa, modified.

## Layout

| Folder | Content |
|---|---|
| `src/` | The game: one `.c` file per module. `mouton.c` is the single translation unit and `#include`s the others; `mouton.h` holds all globals and prototypes. |
| `tools/` | Separate DOS programs: `credits.c` (scrolling credits), `greeting.c` / `credits2.c` (text-mode intro), `editpal.c` / `rotpal.c` (palette tools), `lignecnt.c` (line counter). They include the shared modules from `../src/`. |
| `data/` | Runtime data: `Mouton.cfg`, `Equipes.cfg`, `Lang.fr` / `Lang.en`, sprites, fonts, intro screens and one folder per level (`Neige`, `Montagne`, `Jungle`, `Lune`, `Plage`, `Frissons`; `Nouveau` is the template for a new level). |
| `build/dos/` | Borland C++ IDE project files (`mouton.ide`, `mouon.ide`). No makefile yet. |
| `docs/` | HTML manual and project web pages (`html/`), level file format (`level-format.txt`), `faq.txt`, `controls.txt`, the two licence texts of 2000, and `cleanup-plan.md` (the review that led to this layout). |
| `assets/` | Art sources, not shipped: Paint Shop Pro, BMP, PSD and PCX work files, screenshots, GIF animation frames. |
| `music/` | `xm/` the FastTracker 2 modules (the source); `mp3/` the renders used as CD audio tracks 2–9 (2004 renders). |
| `release/` | **Not versioned** (kept on disk, in `.gitignore`). Historical packaging: `2000-07-18-cd/` (the CD as shipped: EXE, data, manual), `2001-06-06-last-build/` (last MOUTON.EXE, credits.exe, GREETING.EXE), `autorun/` (C++ Builder CD launcher), `installshield/`. The last MOUTON.EXE, credits.exe and GREETING.EXE also exist in the 2001-08-23 commit. |
| `third_party/wdosx/` | **Not versioned** (kept on disk, in `.gitignore`). WDOSX 0.95 DOS extender, not used by the game. |

## Running the last build

The game is a real-mode DOS program; use DOSBox or DOSBox-X.

1. Create a run folder and copy `MOUTON.EXE` into it. It is in the
   untracked `release/2001-06-06-last-build/` folder on the original disk, or
   in the history: `git show a2a21d9:MOUTON.EXE > MOUTON.EXE`.
2. Copy `data/` into that folder **under the name `Mouton/`**. The paths are
   hard-coded in `src/option.c`, `src/graph.c`, `src/start.c` and
   `src/texte.c` as `Mouton/...`.
3. The music comes from CD audio. Build a CD image whose audio tracks 2–9 are
   the files in `music/mp3/` (track 1 is a data track), mount it in DOSBox
   with `imgmount d image.cue -t iso -fs iso`, and make sure MSCDEX is loaded
   (DOSBox does this for `imgmount`). Level files select the track with
   `-TRACK = n`; the menu plays track 3.
4. Run `MOUTON.EXE` from the run folder. `GREETING.EXE` and `credits.exe`
   are the intro and credits programs shown around the game on the CD.

`docs/controls.txt` lists the keys. Playing without a CD has not been tested
in this revival yet.

## Building

Two toolchains are possible.

**Open Watcom 2.0 (free, works today).** Install Open Watcom v2
(https://github.com/open-watcom/open-watcom-v2/releases, `open-watcom-2_0-c-win-x64.exe`,
which is a plain zip: extract `binnt64`, `h`, `lib286` to `C:\WATCOM`), then:

```
cd build\watcom
setenv.cmd
wmake            (game and tools into out\)
wmake run        (copies the EXE and data\ as Mouton\ into run\, starts DOSBox-X)
```

The makefile builds a 16-bit DOS, large-model, 386 executable in one translation
unit (`src/mouton.c` includes the other modules). `src/compat.h` provides the
Borland-only functions (`farmalloc`, `setvect`, `random`, `outportb`, ...).
Three Borland assumptions had to be reproduced: `char` is signed (`-j`), the
inline asm reads globals through DS while Open Watcom lets DS float (each asm
block now loads DS from SS), and floats used as array indexes are cast.
`tools/editpal.c` is an unfinished work file and is not built by default.

**Borland C++ 4.5 (original).** The IDE project is `build/dos/mouton.ide`;
compile `src/mouton.c` alone, large model. Not tested in this revival.

Both builds run in DOSBox-X (`build/watcom/dosbox.conf` has the settings used).

## Licence

The 2000 CD came with two notices, kept in `docs/`: `licence-2000-bac.txt`
(the confidential Bac version) and `licence-2000-free.txt` (free to copy and
distribute unmodified). No open-source licence has been chosen yet.
