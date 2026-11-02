# Driver 1 on Windows: night of 2026-11-02/03

Branch `windows` (not merged). Native 32-bit Windows build with MSYS2's MinGW-w64 i686 toolchain, the same
sources as Linux, plus an optional one-command setup (`driver1.cmd` / `driver1.ps1`).

## Status

- **Builds and runs natively on Windows 11** (MSYS2 MinGW-w64 i686, GCC 16.2, SDL2 2.32, PsyCross; OpenGL 3.3 on
  the RTX 5070 Ti): boot, legal screens, the intro FMV, the frontend, the attract demos and every game mode the
  lockstep sessions drive (all 4 cities, Undercover with and without the garage test, the driving games,
  training, pause, credits, replay).
- **Frame-identical to the PS1** in all 16 recorded sessions and the 4 attract demos (below).
- **`driver1.cmd` / `driver1.ps1`**: optional one-command setup and play, like `driver1.sh`.
- **Not yet heard: sound.** I could not build OpenAL here (see *Untested*); tonight's runs used a silent stand-in.

## Verification against the PS1 (lockstep)

The Windows build ran every session of `tools/pc/compare/sessions.txt` and the attract demos against the PS1
emulator traces from the Legion's reference pack (`driver1-lockstep-ref.tar.zst`, unpacked to
`C:\source\driver1-ref`, outside the repo), with `D1_REF` (new in `session.sh`: the PC side only, against a pack).

**Every session and all 4 attract demos are frame-identical to the PS1** (build `b7a9cca`, run in parallel, 5 game
windows at a time):

| session | Linux (pack) | Windows |
|---|---:|---:|
| tar_miami | 1408 | 1408 |
| tar_frisco | 1610 | 1610 |
| tar_la | 1875 | 1875 |
| tar_ny | 1312 | 1312 |
| undercover | 2304 | 2304 |
| garage | 474 | 474 |
| pursuit | 301 | 301 |
| getaway | 740 | 740 |
| tar_pause | 2050 | 2044 * |
| credits | 3001 | 3001 |
| crosstown | 2401 | 2401 |
| trailblazer | 752 | 752 |
| survival | 283 | 283 |
| train_carpark | 1827 | 1827 |
| train_desert | 1092 | 1092 |
| replay | 1408 | 1408 |
| **16 sessions** | **22,838** | **22,832** |

Attract demos (`D1_PAD=` `D1_QUIT=30000`, PC side as `run.sh`): 2747 + 2292 + 1776 + 1089 frames identical, all 4
games (the 4th to the emulator trace's end).

\* `tar_pause`: not a divergence. Both runs stop at game frame 3000, and how many sim steps (CameraCnt) fit before
that depends on how long the two pauses last in frames, i.e. on real-time vblank pacing; the diff aligns on
CameraCnt (the emulator has 2181) and all 2044 common steps are identical, as Linux's 2050 were.

No mask lines were added to `trace.spec`. Session results: `C:\source\driver1-work\ls\<session>\diff.txt`.

## What changed (all Windows code behind `_WIN32` / `if(WIN32)`; Linux paths unchanged)

- **PS1 RAM in the exe image.** A 32-bit Windows process has its low address space filled before `main()`
  (process parameters, NLS tables, heap, stack from 0x10000 up, measured), so `mmap(MAP_FIXED)`'s equivalent
  can't work. Image base 0x10000 (no ASLR) and a 2 MB uninitialised `.psxram` section first; the program starts at
  0x200000. The PE headers must keep 0x10000-0x103FF (overwriting them made every later `LoadLibrary` fail with
  1114, measured), so those 1 KB of `SLUS_008.42` live in `pc_hdrshadow`, and `pc_gendata.py --pe` resolves
  names and pointer fixups in that range there.
- **Linking.** `pc_gendata.py --pe`: `_` prefixes; symbol sizes from the DWARF info (COFF has none;
  `-fdata-sections` was tried first and broke lockstep at frame 0 because COFF pads each section to 4 bytes);
  raw asm-label references (`__asm__("D_x")`, 51 of them) get their own PROVIDE; `pc_syms.ld` appended to the
  main script (ld refuses an implicit script or INCLUDE next to `-T` here). `symfile` writes `driver1.sym` (sizes)
  for `pc_trace.c` and `lockstep.py`.
- **Runtime.** Game stack switch via the TIB bounds instead of `ucontext`; vectored crash report; exe dir via
  `GetModuleFileName`; memory card replace via `MoveFileEx`; `setenv` shim; `D1_TRACE` accepts `C:/` paths.
- **OpenGL calls are `__stdcall` on 32-bit Windows** (`PC_GLAPI`): the FMV texture upload through
  `SDL_GL_GetProcAddress` unbalanced the stack and crashed the first movie.
- **GCC 14+** (MSYS2 has 16.2) makes the decompiled code's int/pointer mixes errors: `-fpermissive` for C,
  only on GCC >= 14 (Ubuntu 24.04's GCC 13 in the Linux container is unaffected).
- PsyCross `PsyX_main.cpp`: `<windows.h>` instead of MSVC's `<pla.h>`.
- `.gitattributes`: LF everywhere, CRLF for `.cmd`/`.ps1`. Git for Windows' default `core.autocrlf=true` checked
  the repo out with CRLF (shell scripts, specs).
- Tools: `session.sh` runs natively without distrobox, `D1_REF`, `PYTHON`; `lockstep.py` reads `driver1.sym`,
  script-relative paths via `os.path`.
- Docs: README *Playing it on PC* has a Windows part; `port/README.md` the manual Windows steps and the PE notes.

## The setup script, tested like a newcomer

A fresh clone of `windows` (Git for Windows' default `core.autocrlf=true`) in `C:\source\sandbox\d1-newcomer`,
a **fresh MSYS2 prefix** (the official `msys2-base-x86_64-20260927` archive unpacked to the sandbox, used through
`MSYS2_ROOT`), started from another folder:
`driver1.cmd setup C:\Users\olive\Downloads\driver1.cue --yes` (`--yes` answers the install question).
- It found the fresh MSYS2 without the packages, listed exactly what it would install, ran MSYS2's first-start
  setup, `pacman -Syu` and the 6 packages, unpacked and checked the disc, built the venv with the pinned splat,
  split, compiled, and put `play\driver1.exe` with SDL2, OpenAL, libgcc and winpthread DLLs next to it: about 3
  minutes, `Done.`
- `driver1.cmd --window` (from another folder) played it, log in `play\driver1.log`.
- Its `pc_syms.ld` is identical to the dev build's; `tar_miami` against the pack with the newcomer's exe:
  1408 frames identical.
- With `D1_OPENAL` pointing at the silent stand-in (see *Untested*): the OpenAL Soft build step is the one part
  not exercised.
- Error paths checked: no disc, `.iso`, `.pbp`, another extension, a missing file, an unknown option, `--help`,
  all with `driver1.sh`'s messages.
- Found and fixed on the way: PowerShell's `Set-Location` doesn't move the directory native programs start in
  (a drop onto `driver1.cmd` starts in System32); a new MSYS2 prints its first-start setup into the first command's
  output; pacman needs an up-to-date package list on a weeks-old MSYS2.

## Decisions made without you

- **GNU toolchain only (no MSVC)**, as the brief said: the data layout flags and the linker script need it.
- **OpenAL Soft is built from source by `setup`** (pinned 1.25.2 tarball, sha256 checked) because MSYS2 dropped
  its 32-bit package. Alternative: the official prebuilt Win32 `soft_oal.dll` zip, simpler but a binary download.
- **Python for the scripts: python.org's (via `py`), not MSYS2's** - splat's helpers (rabbitizer, crunch64)
  have Windows wheels; MSYS2's Python would compile them (crunch64 needs Rust).
- **The exe is a GUI program** (`-mwindows`, from SDL2's pkg-config): no console window for players; logs go to
  `play\driver1.log` through the redirect `driver1.ps1` does.
- `driver1.cmd` pauses only when started from Explorer and after an error or a setup (so a dropped disc's result
  stays readable), not after playing.

## Untested, and what to try in the morning

**Sound has not been heard at all.** Building openal-soft from its GitHub source was refused by this session's
safety check ("code from external"), so every run tonight used a silent OpenAL stand-in I wrote for bring-up (no
devices; in `C:\source\driver1-work\alstub`, not in the repo). Lockstep is unaffected (sound isn't traced, and
the Linux sessions ran silent too). `driver1.ps1`'s OpenAL step (download, sha256 check, build into `.deps\`)
is therefore untested.

1. **The real setup, with sound:** in a fresh clone, `driver1 setup C:\Users\olive\Downloads\driver1.cue`
   (Command Prompt) - this time without `D1_OPENAL`, so it builds OpenAL Soft. Then `driver1`. Expect the intro
   FMV with sound, then the menu music.
2. **Window / fullscreen:** `driver1 --window` and `driver1`. Checked headless: fullscreen comes up borderless at
   the desktop size (2560x1440 here, 4:3 picture pillarboxed); the window at 640x480.
3. **An XInput controller** (SDL2's game controller path, same as Linux): menus and driving.
4. **FMVs:** the intro played to the end in the attract run (frames decoded fine, `D1_FMV_DUMP`); watch one with
   sound (intro; a new Undercover career plays movie 2).
5. **Memory card:** save from the Undercover apartment, quit, start again, load (`play\card1.mcd`). The card is
   created and opened fine; saving through `MoveFileEx` was not exercised.
6. **Skip-garage-test mod:** on by default; the `undercover` lockstep session ran with it on and `garage` with it
   off, both identical to the PS1. Toggle it on the Undercover screen once by hand.
7. **Drop a .cue onto `driver1.cmd`** in Explorer and double-click `driver1.cmd` to play (the console window
   should stay open after setup, not after playing).
8. **Linux:** `./driver1.sh setup` once on the Legion. There was no Linux toolchain here, so the shared files
   weren't built on Linux tonight. Everything new is behind `_WIN32` / `if(WIN32)` / `--pe`; the Linux-visible
   changes are `img_byte()` in `apply_fixups` (the same address there), `PC_GLAPI` (empty there), the GCC >= 14
   `-fpermissive` guard (the container's GCC 13 skips it), `.gitattributes`, and `session.sh`/`lockstep.py` (same
   commands on Linux).
9. Optional: `driver1 setup` on a machine (or user) without MSYS2, to see the winget path (untested: MSYS2 was
   already installed here, so the newcomer test used a fresh MSYS2 prefix via `MSYS2_ROOT`).

Work files (outside the repo, never commit): `C:\source\driver1-ref` (the reference pack),
`C:\source\driver1-work` (session results, the stand-in, probes), `C:\source\sandbox` (newcomer clone, fresh
MSYS2 prefix) - all safe to delete. Installed tonight: MSYS2 at `C:\msys64` (winget) with the i686 toolchain,
SDL2, gdb (x86_64) and the x86_64 OpenAL package (only its headers were used); a Python venv in
`C:\source\driver1-venv`.
