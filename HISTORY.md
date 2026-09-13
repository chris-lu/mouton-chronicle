# History of this repository

Mouton Chronicles was developed between November 1999 and August 2001, before
the team used any version control. Backups were taken as zip files into
numbered folders (`1`, `1.5`, `2` … `11`), plus one RAR archive in 2002. On
2026-09-13 those archives were turned into the git history you are reading,
then the working folder was cleaned up and reorganized.

## Timeline of the commits

| Commit date | Origin | What it contains |
|---|---|---|
| 1999-12-08 | `1/Backup.zip` | First `SHEEP.EXE` and three PCX backgrounds |
| 1999-12-11 | `1.5/backup.zip` | First sources: `SHEEP.C`, wiSdom VGA library, mouse |
| 1999-12-15 | `2/backup.zip` | `SHEEP2.C`, palette module |
| 2000-01-05 | `3/backup.zip` | Keyboard handler, performance notes |
| 2000-01-16 | `4/Backup.zip` | CD audio (MSCDEX), XMS, `MOUT.C` |
| 2000-01-19 | `5/backup.zip` | Terrain generation (`GRAPH.C`), config files (`OPTION.C`) |
| 2000-01-23 | `6/Backup.zip` | Program renamed `MOUTON.C`, `MOUTGEST.C` |
| 2000-02-02 | `7/backup.zip` | First level files (Neige, Jungle), `Equipes.cfg` |
| 2000-02-09 | `7.5/backup.zip` | Menu prototype, data moved into `Mouton/` |
| 2000-02-09 | `8/mouton.zip` | Release: EXE + data |
| 2000-02-19 | `9/Mouton.zip` | Release: EXE + data |
| 2000-03-22 | `10/backup.zip` | `GREETING.C`, `LIGNECNT.C`, levels Lune, Montagne |
| 2000-04-26 | `11/Backup.zip` | Credits screen, palette editor, levels Plage, Frissons, Nouveau |
| 2001-08-23 | `Projet Mouton.rar` | Sources 0.13 beta, data, last builds (identical to the working folder) |
| 2001-08-24 | working folder | Everything else dated up to August 2001, at its original path |
| 2004-05-08 | working folder | MP3 renders, 2004 config edits, CD cover PSD |
| 2021-12-03 | working folder | Two PNG screenshots |
| 2026-09-13 | cleanup | Five commits: remove artifacts, remove superseded sources, reorganize, fix tool includes, add this documentation |

## How the dates were chosen

* Snapshot commits are dated with the modification time of the zip file, which
  is when the backup was made. Release zips (8, 9) were applied on top of the
  previous tree instead of replacing it.
* The three earliest zips are stamped December **1998**, but they contain
  files stamped mid-1999, the next backup is January 2000, and the project was
  a 1999/2000 school assignment. The PC clock was evidently one year behind
  until it was corrected around New Year 2000, so those commits are dated
  December **1999**. Some files inside later zips are stamped 2028; that is the
  same clock problem in the other direction and was ignored.
* Snapshot 1's zip file is dated 2000-02-22 although its content is from
  December 1999; the content date was used.
* The 2001 snapshot is dated by its newest file (2001-08-23), not by the RAR
  file (2002-03-08).

## Path remapping

Some zips stored the tree under a prefix that reflected the original PC:
`programmation/programme/exe/siege/` (the project was first called *Siège*,
and the Turbo C desktop file still points to `C:\PROGRA~2\PROGRA~1\EXE\SIEGE`),
`Programmation/Projet/`, or a lower-case `mouton/` data folder. These prefixes
were stripped so that every commit has the sources at the root and the data
under `Mouton/`, matching the final working folder.

## What was deliberately left out

* The numbered snapshot folders and the RAR themselves (their content is the
  history).
* Archives that only duplicated an extracted folder: `Manuel.zip`,
  `Manuel Light.zip`, `Captures/1.zip`, `dos/Mouton Chronicles.zip`, and the
  `.rar` copies of the `.XM` music files.
* A text file that contained a login and password.
* Binary, optional material, kept on disk only (see `.gitignore`): the CD
  package of 2000-07-18 (`release/2000-07-18-cd/`), the last builds, the C++
  Builder autorun launcher, the InstallShield projects, the WDOSX extender and
  the RAR of the 2000 MP3 renders. The last executables are still in the
  2001-08-23 commit (`git show a2a21d9:MOUTON.EXE`).

## Original file names

The cleanup lower-cased the source file names (`MOUTON.C` → `src/mouton.c`)
because the `#include` lines in the code already use lower case; on a
case-sensitive system the original names would not have compiled. All other
files kept their original names. Git records the moves as renames, so
`git log --follow src/mouton.c` walks back to `SHEEP.C` in 1999.
