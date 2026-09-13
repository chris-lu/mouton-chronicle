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
| `build/dos/` | Borland C++ IDE project files of 2001 (`mouton.ide`, `mouon.ide`). |
| `build/watcom/` | Open Watcom `makefile` and the DOSBox-X configuration. Output goes to `out/`, the run folder to `run/` (both ignored). |
| `scripts/` | `build.cmd`, `run.cmd`, `smoke-test.ps1`, `env.cmd`; settings in `.env` (see `.env.example`). |
| `docs/` | HTML manual and project web pages (`html/`), level file format (`level-format.txt`), `faq.txt`, `controls.txt`, the two licence texts of 2000, and `cleanup-plan.md` (the review that led to this layout). |
| `assets/` | Art sources, not shipped: Paint Shop Pro, BMP, PSD and PCX work files, screenshots, GIF animation frames. |
| `music/` | `xm/` the FastTracker 2 modules (the source); `mp3/` the renders used as CD audio tracks 2–9 (2004 renders). |
| `release/` | **Not versioned** (kept on disk, in `.gitignore`). Historical packaging: `2000-07-18-cd/` (the CD as shipped: EXE, data, manual), `2001-06-06-last-build/` (last MOUTON.EXE, credits.exe, GREETING.EXE), `autorun/` (C++ Builder CD launcher), `installshield/`. The last MOUTON.EXE, credits.exe and GREETING.EXE also exist in the 2001-08-23 commit. |
| `third_party/wdosx/` | **Not versioned** (kept on disk, in `.gitignore`). WDOSX 0.95 DOS extender, not used by the game. |

## Build and run

Windows, Open Watcom 2.0 and DOSBox-X. Copy `.env.example` to `.env`, set the
two paths, then:

```
scripts\build.cmd                         builds the game and the tools
scripts\run.cmd                           starts the game in DOSBox-X
scripts\run.cmd credits                   starts a tool (credits, greeting, credits2, rotpal, lignecnt)
powershell -File scripts\smoke-test.ps1  unattended run with screenshots and crash check
```

`docs/build-and-run.md` has the details (installation, settings, music,
troubleshooting) and `docs/porting-notes.md` explains what the Open Watcom
port changed and why. The Borland C++ 4.5 project of 2001 is in `build/dos/`.

## Licence

The 2000 CD came with two notices, kept in `docs/`: `licence-2000-bac.txt`
(the confidential Bac version) and `licence-2000-free.txt` (free to copy and
distribute unmodified). No open-source licence has been chosen yet.
