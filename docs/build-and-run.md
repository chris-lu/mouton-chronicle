# Building and running Mouton Chronicles

The game is a 16-bit MS-DOS program. It is compiled on Windows with Open Watcom
and run in the DOSBox-X emulator. Everything is driven by three scripts at the
root of the repository and one settings file.

## 1. Prerequisites

| What | Where | Notes |
|---|---|---|
| Windows 10/11 | | The scripts are `.cmd` and PowerShell; from Git Bash use `cmd //c scripts/build.cmd`. |
| Open Watcom 2.0 | https://github.com/open-watcom/open-watcom-v2/releases (tag *Current-build*, file `open-watcom-2_0-c-win-x64.exe`, about 128 MB) | Either run the installer, or notice that the `.exe` is a plain zip: with 7-Zip extract the folders `binnt64`, `h`, `lib286` (and `docs` if you want the PDF manuals) into `C:\WATCOM`. Nothing else is needed for a DOS build. |
| DOSBox-X | `winget install joncampbell123.DOSBox-X` or https://dosbox-x.com | Installs to `C:\DOSBox-X`. Plain DOSBox also works for playing, but the scripts assume DOSBox-X. |

## 2. Settings: the `.env` file

Copy `.env.example` to `.env` at the repository root and adjust it. `.env` is
ignored by git. The scripts read it (`KEY=VALUE`, `#` for comments).

| Key | Default | Meaning |
|---|---|---|
| `WATCOM` | `C:\WATCOM` | Open Watcom root (contains `binnt64\`, `h\`, `lib286\`). |
| `DOSBOX` | `C:\DOSBox-X\dosbox-x.exe` | Emulator executable. |
| `CDIMAGE` | empty | Optional `.cue` image with the music as CD-audio tracks, mounted as `D:` (see §6). |
| `DOSBOX_ARGS` | empty | Extra arguments appended to the DOSBox-X command line. |

If `WATCOM` or `DOSBOX` are already set in your Windows environment (an
official Open Watcom installation does that), `.env` still wins when the key
is present in the file.

## 3. Commands

| Command | What it does |
|---|---|
| `scripts\build.cmd` | Compiles the game and the tools with `wmake` into `build\watcom\out\` (`mouton.exe`, `credits.exe`, `greeting.exe`, `credits2.exe`, `rotpal.exe`, `lignecnt.exe`). Incremental. |
| `scripts\build.cmd clean` | Deletes `build\watcom\out\` and `build\watcom\run\`. |
| `scripts\build.cmd out\credits.exe` | Builds one target. |
| `scripts\run.cmd` | Builds, prepares `build\watcom\run\` and starts the game in DOSBox-X. |
| `scripts\run.cmd credits` | Same for a tool: `credits`, `greeting`, `credits2`, `rotpal`, `lignecnt`. |
| `scripts\run.cmd --no-build` | Skips the build step (combinable: `--no-build credits`). |
| `powershell -File scripts\smoke-test.ps1` | Unattended check, see §5. |
| `call scripts\env.cmd` | Only sets the environment (PATH, INCLUDE, `MC_ROOT`) in the current window, for using `wmake`, `wcc`, `wlink` by hand. |

`scripts\run.cmd` copies the executable and the `data\` folder into
`build\watcom\run\`, the data folder being renamed **`Mouton\`**: that name is
hard-coded in the game (`src/option.c`, `src/graph.c`, `src/start.c`,
`src/texte.c`), so any change made to `data\` is picked up at the next run.
The run folder is disposable and ignored by git.

In the game: the text banner waits for a key. Controls are listed in
`docs/controls.txt` (arrows to move and aim, Enter to jump, Space held down to
fire, `+`/`-` to change weapon, F1 names, F2 centre, Esc to leave a round).

## 4. What the build is

* `build\watcom\makefile` (Open Watcom `wmake`). One translation unit:
  `src\mouton.c` includes every other module, exactly as under Borland in 2001.
  The tools are one file each and include the shared modules from `..\src\`.
* Compiler options and why they matter: `-ml` large model, `-3` 386 code (the
  blitters use the `66h` operand-size prefix), `-bt=dos`, `-fpi` inline x87
  with emulation, `-s` no stack checks, `-ox`, **`-j` signed `char`** (the code
  stores −1 in `char`). The reasons are detailed in `docs/porting-notes.md`.
* Output: `out\*.obj`, `out\*.exe`. Link options: `system dos`, stack 16 KB.
* `src\compat.h` supplies the Borland-only functions when compiling with Open
  Watcom, and is inert under Borland.
* `tools\editpal.c` is an unfinished work file from 2000 (it never compiled)
  and is not part of the default target.

Manual equivalent, in a `cmd` window:

```
call scripts\env.cmd
cd build\watcom
wmake
mkdir run & copy out\mouton.exe run\MOUTON.EXE & xcopy /e /i /y ..\..\data run\Mouton
"%DOSBOX%" -conf dosbox.conf -c "mount c \"%CD%\run\"" -c c: -c MOUTON.EXE
```

## 5. Smoke test

`scripts\smoke-test.ps1` is a regression check for use after code changes: it
prepares the run folder, starts DOSBox-X with a generated configuration
(`build\watcom\run\smoke.conf`), presses Enter to leave the banner, takes a
screenshot of the emulator window every 5 seconds into `build\watcom\run\shots\`,
then scans the emulator log for invalid-opcode exceptions, DOS memory
corruption and triple faults. It prints `PASS` or `FAIL` and sets the exit code.

```
powershell -File scripts\smoke-test.ps1                  # 45 s, intro and menu
powershell -File scripts\smoke-test.ps1 -Seconds 90      # longer
powershell -File scripts\smoke-test.ps1 -ClickMenu       # also clicks Options, OK, Equipes
powershell -File scripts\smoke-test.ps1 -Program credits # a tool instead of the game
```

Keep the emulator window unobstructed while it runs: the key and mouse events
are sent to the foreground window (`-ClickMenu` uses DOSBox-X's seamless mouse
mode and expects the guest screen shown at 2× in the window). The Enter key
is occasionally not delivered; the test then reports that the game never
switched to graphics mode, and a rerun is enough. Look at the screenshots to
judge the display; the log check only catches crashes.

## 6. Music

The game does not play music files: it drives the CD-ROM drive through MSCDEX
and plays **audio tracks by number**. The 2000 CD had the game on track 1 and
the music on tracks 2 to 9, in the order of the files in `music\mp3\`
(2 Water, 3 Intro, 4 Neige, 5 Jyngle, 6 Space, 7 Plage, 8 Cimetière,
9 Crédits). Each level selects its track with `-TRACK = n` in its `.cfg`; the
menu plays track 3. Without a CD the game runs silently.

To hear the music in DOSBox-X, build a CD image with a data track followed by
the eight MP3 files as audio tracks and point `CDIMAGE` at its `.cue`; the run
script then mounts it as `D:`. A cue sheet looks like this (any small ISO file
works as track 1; `.mp3` audio tracks are supported by DOSBox-X):

```
FILE "data.iso" BINARY
  TRACK 01 MODE1/2048
    INDEX 01 00:00:00
FILE "2. Mouton Chronicles - Water.mp3" MP3
  TRACK 02 AUDIO
    INDEX 01 00:00:00
FILE "3. Mouton Chronicles - Intro.mp3" MP3
  TRACK 03 AUDIO
    INDEX 01 00:00:00
... one entry per file up to track 09
```

This has not been tried yet in this revival; the code path (`src/cd.c`) is
the original one.

## 7. Troubleshooting

| Symptom | Cause / fix |
|---|---|
| `[env] Open Watcom not found under "C:\WATCOM"` | Set `WATCOM` in `.env` to the folder that contains `binnt64\wcc.exe`. |
| `[run] DOSBox-X not found` | Set `DOSBOX` in `.env`. |
| DOSBox-X opens a "Select folder where to run emulation" dialog | The `-fastlaunch` option was not passed; use the scripts. |
| DOS prompt "Le montage de C:\ n'est pas recommandé ... [Y/N]" | The mount command lost its path (a quoting problem when launching by hand); answer N and use `scripts\run.cmd`, which quotes the path. |
| Window title becomes "Sélection DOSBox-X ..." | The window is in console selection mode after a click; press Esc in it. |
| "Could not initialize OpenGL, switching back to surface" in the log | Harmless; DOSBox-X falls back to plain rendering. |
| Nothing happens at the banner | The banner waits for a key; click the emulator window first so it has the keyboard focus. |
| Smoke test says "never switched to graphics mode" | The Enter key was not delivered to the emulator; rerun with the window in front. |

## 8. The original toolchain (Borland C++ 4.5)

`build\dos\mouton.ide` is the Borland IDE project used in 2001 (16-bit DOS
target, large model, `src\mouton.c` as the only compiled file). Borland C++
was never released for free, so this route only works if you have your own
installation; it has not been tried in this revival. The sources still compile
under Borland: `src\compat.h` only includes `<alloc.h>` there, and the asm
blocks now also load DS from SS, which is harmless under Borland.
