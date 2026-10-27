# Driver 1 PC port (PsyCross)

The decompiled game (`src/`) built as a native 32-bit Linux program on [PsyCross](https://github.com/OpenDriver2/PsyCross)
(vendored in `port/psycross/`, MIT), the PsyQ-on-SDL2/OpenGL layer REDRIVER2 uses. Status: it builds, links and
boots through the splashes to the frontend, runs the attract-mode demos, and plays (Take A Ride in Miami, driven
headlessly through `D1_PAD`). FMVs play (NFMV in-process, see FMV). Memory card: slot 1 is a raw `.mcd` image (see Memory card).

## 1. Game data: unpack your disc

You need your own US disc (SLUS-008.42) as a raw `MODE2/2352` image (`.bin` + `.cue`; e.g. converted from a PBP with
psxtract or pop-fe, or ripped). Then:

```
python3 port/pc/pc_extract.py driver1.cue pcdata
```

It writes the whole ISO9660 tree (`pcdata/DRIVER/...`, `pcdata/SLUS_008.42`) plus `pcdata/DISC.LBA`, the index the
game's CD reads are served from (first sector, sector count, size, sector size, path per file). Plain files are
their 2048-byte data; the interleaved XA audio (`*.XA`) and FMV (`*.STR`) files are kept as raw 2352-byte sectors
for later. Check: `sha1sum pcdata/SLUS_008.42` must be `94b44a84de2f9fa2dcaf120db2e7b3d74bc70d3d`.

## 2. Build

In the `build` distrobox (32-bit SDL2/OpenAL dev packages, Python 3):

```
cmake -S port -B ../driver1-work/pcbuild/<dir> && make -C ../driver1-work/pcbuild/<dir> -j2
```

## 3. Run

```
D1_DATA=/path/to/pcdata ../driver1-work/pcbuild/<dir>/driver1     # or: driver1 /path/to/pcdata, or ./pcdata
```

Headless (no window, e.g. over ssh): `SDL_VIDEODRIVER=offscreen`. Debug aids (environment):
`D1_SHOT=n1,n2,...` writes `gl_<n>.ppm` (the window, grabbed as the next frame is presented) and `shot_<n>.ppm`
(the displayed VRAM area) at those vblank counts (the VRAM dump comes out red/blue-swapped next to the window grab);
`D1_QUIT=n` exits after n vblanks. Counts are real 60 Hz vblanks (PsyCross's counter), the same clock in the
frontend and in game. `D1_PAD=FROM[-TO]=BTN[+BTN..],...` replaces pad 1 with scripted presses (a lone FROM holds
6 vblanks; BTN: up down left right x o tri sq l1 l2 r1 r2 start select) and ignores real input - set it (even
empty) for any unattended run, or a connected gamepad leaks in. Take A Ride, Miami, day:
`D1_PAD=2500=right,2600=right,2700=right,2800=right,2900=right,3000=x,3200=x,4000-8900=x`.
Silent: `ALSOFT_DRIVERS=null SDL_AUDIODRIVER=dummy`. A crash prints a backtrace (`addr2line -e driver1 <addr>`).

## Mods

PC-only gameplay options (`port/pc/pc_mods.c`; the PS1 build never sees them). Settings live in
`driver1_mods.ini` in the working directory (`D1_MODS_INI=path` overrides it), one `key=value` per line; the game
rewrites the file when a setting is changed in a menu. A missing file means the defaults below.

| key | default | what |
|---|---|---|
| `skip_garage_test` | `1` | A new Undercover career skips the garage driving test ("the interview") |

**Skip the garage test.** Toggle it on the Undercover screen (main menu, first icon): below *New Game* and
*Load Saved Game* there is a third entry, **Skip Garage Test: On/Off** - move down to it with the d-pad and press
X; the change is saved at once. With it on, *New Game* plays what the original plays before the test (the intro
FMV) and then goes straight to the apartment (TV / "Save Game", "Get A New
Job" on the answering machine), exactly where passing the test leads. With it off the garage test runs as on the
PS1.

How it works: a new game is `func_00024754` case 0 (main.c): it sets up mission 0xAE in the IVIEW level (city 2,
`D_00096854` = interview mode) and runs it with `func_00024F1C`. Passing is `iview.c func_001E0290` state 2 setting
`D_000970A8 = 1`; `func_00024F1C` then shows the end menu (mode 1, `D_00096818 = 1`) and returns, and case 0 starts
the career ladder (`func_0006EACC` + `func_0006EB74`, `data\mladder.psx` from the top: Miami, apartment, first job)
only when `D_000970A8 == 1` - failing/"Exit" leaves it 0 and returns to the frontend. The mod replaces only the
`func_00024F1C()` call with those two stores, so every other assignment of the original path runs unchanged and the
ladder starts in the state a pass leaves (mission state is re-initialised by the ladder's first mission anyway).
The menu entry: `newgame.c` passes a 3-entry copy of the screen's MENU_ITEM table (`D_001CEA18` from FRONTEND.BIN,
re-copied on each entry since the overlay is reloaded) with the up/down ring extended, to the game's own menu
driver (`func_001C2644`/`func_001C26E4`); the label is drawn by a hook at the end of the label drawer
(`fecore.c func_001C1BD4` and its copy in `wips_pc.c func_001C2114`).

## Lockstep comparison

Proof that the PC build simulates what the PS1 game does: both run the attract-mode demos (the four disc replays,
missions 0xB4-0xB7; no input needed), both write the same set of game variables once per game frame, and
`tools/pc/compare/lockstep.py` compares them frame by frame and names the first word that differs.

```
tools/pc/compare/run.sh <pc build dir> <out dir> [pc vblanks=30000] [emulator vblanks=62000]
```

runs both sides (headless, silent, in the `build` distrobox, in parallel), then the diff. It needs
`~/dev/driver1-work/emu/` (or `EMU_DIR`): `swanstation_libretro.so` (the DuckStation libretro core; copied from the
RetroArch flatpak's cores), `system/scph5501.bin` (US BIOS, copied from the EmuDeck bios folder), and builds `lrhost`
there from `tools/pc/compare/lrhost.c`; the disc is `~/dev/driver1-work/driver1.cue` (`D1_CUE`). The PC side defaults
cover all four demos (they start at ~4400 vblanks and end at ~31000; the PC runs in real time, so ~9 min), the
emulator side runs ~10x faster than real time but plays the intro FMVs first (demo 1 at ~18300 vblanks). Traces are
~25 KB per game frame (~250 MB for the four demos). Output: per demo, `lockstep: N frames identical`, or
`FIRST DIVERGENCE at frame F` with every differing word (region, field or PS1 symbol, PS1 address, both values).

Pieces:
- `tools/pc/compare/trace.spec` - what is traced (label, PS1 address, PC symbol, size): frame counter, CameraCnt,
  the BIOS rand seed, camera position, Random2's table, car_data[20], felony.c's pursuer records, MISSION, the civ
  slots, and all of .sdata/.sbss (`*`: a PS1 range the PC assembles from PC_RAM and the C-owned objects inside it),
  plus `mask` lines: the words that legitimately differ, each with its reason (vblank clocks, saved stack pointers,
  a string literal's address, CD-streaming progress, the XM player's interrupt-driven state, a speech-queue timer
  that follows SPU playback). A new divergence is either a port bug or gets a mask line with its explanation.
- PC: `port/pc/pc_trace.c`, `D1_TRACE=<spec>:<out>`. `__wrap_VSync` passes its return address; the first VSync call
  per game frame from the in-game loop (main.c func_0002360C: the VSync(-1) wait right after ControlReplay) writes a
  record. Symbols come from the exe's own ELF symbol table (so statics like pc_psyq.c's rand seed work); `*` ranges
  read build/slus_008.42.map (`D1_TRACE_MAP` overrides).
- Emulator: `tools/pc/compare/lrhost.c`, a ~600-line headless libretro frontend (no window, no audio, software
  renderer, interpreter CPU, no memory cards; `--pad` takes the D1_PAD syntax, `--shot`, `--ramdump`,
  `--zerocheck`). A libretro core only returns between vblanks and a game frame takes >= 2, so the host patches the
  PS1 game in emulated RAM (not the disc) once SLUS_008.42 is loaded: `jal stub` at 0x24240, the same point as the
  PC hook. The 31-instruction stub copies the spec's regions into a buffer at 0x1E5100 and bumps a sequence word;
  the host collects the buffer after every vblank. 0x1E4E00-0x1ED800 is RAM nothing uses during boot, menus and
  the demos (measured with `--zerocheck`; only CRED.BIN, the credits overlay, would cover it), and the host
  reports if anything else writes there.
- `lockstep.py info|diff|show`: games are runs of the frame counter (0,1,2..), paired in order and aligned on that
  counter, never on vblanks. Pointers are compared by what they point at (`--pc-exe`: PC addresses of C-owned
  objects, functions and the scratchpad are mapped back to PS1 addresses; Driver stores physical addresses).
  `show TRACE GAME FRAME [label]` dumps a record; `--all` lists the first divergent frame per region.

Result (2026-10-27, commit fb2b13f + this tool): all four demos (0xB4/0xB5 in city 0, 0xB6/0xB7 in city 1) are
identical for their whole length - 2747 + 2292 + 1776 + 1674 = 8489 game frames, every traced byte outside the mask
lines; the demos also end on the same frame. Divergences found and explained on the way: felony.c func_00065424's
uninitialised stack VEC (vy/pad garbage in the pursuers' cur/prev, unread), pathfind's heap pointers to the sentinel
cell (a PC address - same object), and the timing/platform words in the mask list. Sensitivity check: pairing two
different demos diverges at frame 0 in 1051 words.

To find the code behind a divergence on the PC side, watch the word with gdb in the build box (break on
`PC_TraceVSync if D_00097534 == F-1`, then `watch` the PC address and print `bt` in its commands).

## How it fits together

### Memory: PS1 RAM lives at its PS1 addresses
- `pc_psxram` (the 2 MB main RAM, `PC_RAM(a)` in `pc_compat.h`) is a linker symbol at **address 0**;
  `port/pc/pc_main.c` maps pages 0x10000-0x1FFFFF there at startup (0-0xFFFF, the PS1 kernel area, stays unmapped:
  `vm.mmap_min_addr`). So `PC_RAM(a)` == the PS1 physical address, and any hard-coded address that slipped through
  still works.
- `SLUS_008.42` is loaded at 0x10000 at startup, and FRONTEND/MCARD/CRED/IVIEW.BIN at their addresses whenever the
  game loads them (`Loadfile` -> `PC_OverlayLoad`), as on the PS1. Their code bytes are dead weight (all code is
  linked in); their **data** is what matters: everything the C files don't own yet is used in place, with its PS1
  contents, at its PS1 address, and the pointers inside it (`0x001C00D8` ...) are valid PC pointers as they are.
- The words splat identified as pointers (`.word func_x` / `.word D_x`) are rewritten after each load to the PC
  symbol (`pc_fixups.s`): function-pointer tables and pointers to C-owned objects. Pointers splat did not
  recognise (e.g. halves printed as `.short`) still point at the PS1 copy - correct for asm-owned data, stale for
  C-owned data that changes at run time (none seen so far).
- The exe is non-PIE at 4 MB (`-Ttext-segment=0x400000`) and the game runs on a stack inside its `.bss`
  (`pc_main.c`, `ucontext`), so everything the game can link into an OT is below 16 MB.
- Primitives keep the **PS1 layout** (`USE_EXTENDED_PRIM_POINTERS=0`, `USE_PGXP=0` for game and PsyCross):
  4-byte P_TAG with a 24-bit next pointer. Because `pc_psxram` is at 0 and all game memory is below 16 MB, the
  24-bit value *is* the address and PsyCross's OT walker (`nextPrim`) needs no change.
- `pc_scratchpad` (the 1 KB scratchpad, `PC_SCRATCH`) is a plain array in `pc_main.c`.
- Game files are compiled with `-fno-toplevel-reorder -malign-data=abi -fno-zero-initialized-in-bss -fno-common`
  so each file's data keeps its PS1 order and alignment: the game reads across neighbours
  (`xmplay.c` reads `D_0008D218[-1]` = `D_0008D208[7]`).

### Linking: `port/pc/pc_gendata.py` (build step)
`pc_gendata.py fixups` writes `pc_fixups.s` (above). `pc_gendata.py syms` runs after compiling, reads every object
with `nm`, and writes `pc_syms.ld` (an implicit linker script) with one `PROVIDE` per symbol left undefined:
1. PsyQ functions the game calls by address -> the implementation (table `PSYQ_ALIAS`, see below);
2. a name inside a C-owned object -> owner + offset (`D_000C4C7C = D_000C4690 + 0x5EC`, from the PS1 addresses
   in both names);
3. data inside a PS1 image (main 0x10000-0xD0054 incl. .bss, the overlays' ranges) -> its PS1 address;
4. splat's short overlay call names (`func_1E0290`) -> the overlay function.
Every decision is listed in `<build>/pc_syms.txt`; names it can't resolve are left to the linker and listed as
MISSING (currently none).

### CD: `port/pc/pc_cd.c` (replaces PsyCross's LIBCD.C, which is never linked)
Served from `pcdata/` through `DISC.LBA`: every file keeps its original sector numbers, so the game's own sector
maths (CdSearchFile/CdPosToInt, `LoadfileSeg`, the level spooler) is unchanged; a read of LBA n comes from the file
covering n. Why not PsyCross's CD-image path or REDRIVER2's `USE_PC_FILESYSTEM`: Driver addresses levels and
streams by absolute sector (`loadsectors`, `spool.c`, `func_000503F4`), so a per-file fopen layer would mean
rewriting those, and PsyCross's image reader runs the CdlReadS stream on its own thread (racing the game's spin
loops and CdSearchFile). Here everything is synchronous and deterministic (needed for replay lockstep later):
- `CdRead` completes immediately; `CdReadSync` reports 0 sectors left.
- A `CdlReadS` stream runs on the game thread when the game would first see data: when a ready callback is
  installed while a stream is pending (`loadsectors`: ReadS, then callbacks), or at the next `VSync`/`CdSync`/
  `CdReadSync` (`UpdateSpool`: callbacks, then ReadS). It delivers CdlDataReady per sector (ready callback ->
  `CdGetSector` -> data callback) until the game pauses it or clears the ready callback; re-seeks from inside a
  callback continue the same loop.
- XA audio streams (`CdlModeRT`, xaplay.c: MV1-5/NMAJ/CUTS.XA) go to `port/pc/pc_xa.c`: it reads the raw sectors at
  the drive's speed (150/75 sectors per second of PsyCross's vblank clock - real time, so not part of the
  deterministic model), decodes the audio sectors matching the CdlSetfilter file/channel (XA-ADPCM, bit-exact with
  ffmpeg's adpcm_xa) into a streaming OpenAL source (`PsyX_SPUAL_XAQueue`), and delivers every other sector to the
  ready callback (924h mode: CdGetSector starts at the header), where xaplay.c's cbready finds the end marker.
Mods: replacing a file in `pcdata/` with one of the same sector count just works.

### FMV: NFMV in-process (`port/pc/pc_fmv.c`, `pc_mdec.c`)
fmvplay.c's Exec of NFMV.EXE runs src/nfmv (nfmv_main) in-process; STR sectors stream from pcdata at 150 sectors/s
(video into frames, XA audio through pc_xa.c), libpress is a software MDEC (BS v2/v3, psx-spx tables), and each 24-bit
frame is drawn as an RGBA texture quad from a shadow of the movie's VRAM writes (PsyCross copies presented frames back
into VRAM as 15-bit). `D1_FMV=0/1` (default: on, off when `D1_PAD` is set); test aids `D1_FMV_LIST=n,..` (play these
instead of the first movie) and `D1_FMV_DUMP=1` (fmv_<movie>_<frame>.ppm). Movies used: 0 intro, 2 new game, 33, 51-62
(ladder tune tables), and the mladder.psx ones (1-45, 65, 70-74; 70/71 play RENDER73; RENDER14 is referenced but not on
the disc). Verified: all 42 play to their end frame with audio; frames vs ffmpeg's MDEC decoder >= 46 dB PSNR.

### Memory card: `port/pc/pc_mcard.c`
Slot 1 is a standard raw 128 KB PS1 card image (`.mcd`/`.mcr`, psx-spx layout) - the same file DuckStation/PCSX use,
so saves move both ways (a symlink to an emulator's card works; writes go through to its target). Path: `D1_MCARD`, else
`card1.mcd` in the working directory; a missing file is created as a freshly formatted card (byte-identical to a
DuckStation-formatted one). Slot 2 is always empty. A file that isn't 128 KB is never touched (slot reads "no card");
one without the "MC" header is an unformatted card the game offers to format. libmcrd semantics follow the game's own
library (async Exist/Accept/Read/WriteFile + MemCardSync, McErr codes, new-card flag cleared by Accept); commands complete
at once. Every change is written immediately (temp file + rename). Check a card: `python3 port/pc/pc_mcard_check.py card1.mcd`.

### Frames
The game never calls `PsyX_EndScene` (REDRIVER2 does, from its game code), so `VSync` is wrapped
(`-Wl,--wrap=VSync`, `pc_main.c`): a drawn scene is presented there, and pending CD/MDEC work completes there.
Only on the game thread: PsyCross runs the PS1 VSyncCallback on its interrupt thread and Driver's callback (xmplay.c)
calls VSync(1) itself - that stays a plain counter read. The frontend ends a frame with VSync(0); the in-game loop
(main.c func_0002360C) draws with DrawOTag and then polls VSync(-1), so a poll presents too (and sleeps 0.2 ms
while the count stands still). Busy-waits on a flag the CD interrupt sets (spool.c) call `PC_WaitService()`.
Pads: PsyCross only updates the PadInitDirect buffers from `PsyX_UpdateInput()`, which its own code calls only in
_DEBUG builds, so the game's pad read (pad.c func_000357F0) calls `PC_PadRead()` first.

### PsyQ functions known only by address
The library is identified (`pilot/psyq_objs.tsv` names 67 by signature) but most calls are `func_0007xxxx`.
Identified for the port by BIOS call number (`li t2,0xA0/0xB0; li t1,n`), the function's own debug string
("ClearOTagR(%08x,%d)..."), a per-function match against psx_psyq_signatures, or arguments/callers:

| address | function | how | implementation |
|---|---|---|---|
| 739B4 / 77D6C | SetGeomOffset / SetGeomScreen | signature | PsyCross |
| 73F88, 740FC | ResetGraph, SetGraphDebug | debug strings | PsyCross |
| 74374, 74404, 744C4, 766A8 | ClearImage, LoadImage, MoveImage, StoreImage | debug strings | PsyCross |
| 7457C, 74684 | ClearOTagR, DrawOTag | debug strings | PsyCross |
| 74CAC | SetDrawArea | (DR_AREA *, RECT *), calls _get_cs/_get_ce | PsyCross |
| 74D2C | SetDrawStp | builds E6000000/E6000001, len 2 | pc_psyq.c |
| 776F4, 777E8 | ResetCallback, StopCallback | main(): Stop, Reset, ResetGraph (D2 order) | PsyCross |
| 776E4, 77D7C | rand, srand | BIOS A(2Fh), A(30h) | pc_psyq.c (PS1 LCG) |
| 79800 | SetVideoMode | signature | PsyCross |
| 79AE0, 79AF0 | GetSp, SetSp | signature | pc_psyq.c (no-op: the stack stays put) |
| 79DB0, 7B218, 7B228, 7E8EC, 7EC88, 7EC98 | memcpy, strncmp, strcmp, strcat, strlen, strncpy | BIOS A(2Ah,18h,17h,15h,1Bh,1Ah) | pc_psyq.c -> libc |
| 79F70 | CdSync | calls CdControl/VSync; `!= 2` polls | pc_cd.c |
| 7A12C, 7A53C, 7A4FC | CdReadyCallback, CdDataCallback, CdGetSector | spool.c use, CdGetSector signature | pc_cd.c |
| 7A27C, 7A580 | CdControlF, CdIntToPos | spool.c use (0x1B ReadS / 9 Pause) | pc_cd.c |
| 7E8FC | CdReadFile | calls CdSearchFile/CdRead; Loadfile's use | pc_cd.c |
| 7A7F4, 7D608 | GetClut, LoadClut2 | signature; 7D608 loads a 16x1 rect | PsyCross |
| 7D66C, 7D720 | SetDefDrawEnv, SetDefDispEnv | args, D2 twins | PsyCross |
| 7AEB4, 7AED4, 7AF94, 7B160, 7B198 | PadStartCom, PadGetState, PadInfoMode, PadSetActAlign, PadSetAct | pad.c use (7AEB4 right after PadInitDirect) | PsyCross |
| 7C7D4 | SpuSetReverbVoice | signature | PsyCross |
| 77E1C..7932C (12) | MemCardInit/Start/Stop/Exist/Accept/ReadFile/WriteFile/GetDirentry/Sync/CreateFile/DeleteFile/Format | "Access Denied" strings, mcard.c use | pc_mcard.c (raw .mcd image) |
| 73AC4, 73AF8, 73B74, 73B94, 768E4, DecDCTvlcBuild | DecDCTReset, DecDCTin, DecDCTout, DecDCToutCallback, DecDCTvlc | MDEC, CRED.BIN portraits | pc_mdec.c (software MDEC); pc_psyq.c forwards |
| 7EB24, 7EBE8, 7EB34, 7EC78, 7C744 | Exec, FlushCache, ExitCriticalSection, EnterCriticalSection, exit | BIOS A(43h,44h), syscalls, B(38h) | pc_psyq.c; Exec runs NFMV (pc_fmv.c) |
| SsSetSerialVol, SetLineF2, SetLineF4 | (named) | missing in PsyCross | pc_psyq.c |

### PsyCross
Source changes: (1) the two GCC 13 build fixes (PsyX_GPU.cpp prim_terminator cast, PsyX_main.cpp SDL_strcasecmp);
(2) `src/audio/PsyX_SPUAL.cpp` decodeSound stops at the output buffer size (a sample without an end flag overran
waveBuffer and crashed in attract mode); (2b) same file, Driver 1 port: integer SPU ADPCM decode, loop start at
the start of the LoopStart block (was one block late, and offset by a byte distance so whole samples looped), and the
XA stream output (`PsyX_SPUAL_XAQueue/XAStop/XASetVolume`); (3) `include/psx/libgpu.h` DRAWENV loses PsyCross's unused `drt` byte, so DRAWENV has the real
PsyQ layout (0x5C bytes; Driver's draw buffers put DISPENV at DRAWENV + 0x5C and address fields by offset).
Build notes (`port/CMakeLists.txt`): the PsyQ replacements are upper-case
`src/psx/*.C`; gcc compiles `.C` as C++ (their headers have the `extern "C"` blocks for it), with `-fpermissive
-Wno-narrowing`; LIBCD.C is superseded by `pc_cd.c`; LIBMCRD.C is bypassed (it indexes `memoryCards[2]` with
`chan = port << 4` and fseeks a NULL FILE when `0.MCD` is missing).

## Port notes
- Audio: the PsyQ function at 0x7A794 named `SpuRead` (psyq_objs signature) is SpuWrite (it calls `_spu_write`,
  func_0008213C: `_spu_t(2)`, `_spu_t(1 = write)`, `_spu_t(3)`); on PC the sound banks went nowhere and every voice
  decoded empty SPU RAM ("sample has no end flag"). sound.c LoadSoundBank calls SpuWrite under PSX_PC; spool.c SendSBK (the in-game spooled bank) does the same.
- Audio: xmplay.c func_00073738 called `SilenceXM()` with the id left in a0 (same pattern as sound.c func_0004E67C);
  PC passes it (crashed in ReInitFrontend after the attract demo).
- pathfind.c func_00063114 fills 0x1F6 A* heap slots from D_000C43B4 (declared [183]), running on into felony.c's
  D_000C4690[0] as the contiguous PS1 .bss allows; on PC it ran over random .bss (FindSeated's pointer -> attract
  crash). PC: one object D_000C43B4 spans both, felony.c only declares D_000C4690 (bound by pc_gendata rule 2).
- Leftover-a0 calls (agent E's scan, each checked against the asm): civ_ai.c func_00013B80 -> func_0004C8F8(a),
  handling.c func_00020780 -> func_000299B8(i), director.c func_00059638(a, b) passes both through to
  func_0005C318, mission.c func_0006A6C8 -> func_0004FFE8(p->flags). The rest of the scan's hits are prototypes,
  local inlines or rand(): no change.
- VSync from PsyCross's interrupt thread presented frames and pumped the CD stream concurrently with the game:
  in game the only VSync(>=0) calls came from there, so nothing showed and "[Psy-X] Failed to map the buffer"
  (glMapBuffer without a GL context on that thread) flooded the log. Fixed in pc_main.c (Frames above).
- spool.c func_00033AF8 / StartSpooling spin on D_000972D4 with nothing that services the CD: `PC_WaitService()`.
- NULL dereferences the PS1 survives (kernel RAM 0x0-0xF is the "garbage area", psx-spx): iview.c func_001E1008
  (D_001E4250 is never set), pause.c's first InitMenu (D_000977C0 NULL while leave() handlers run). PC points them
  at `pc_lowram` (pc_main.c); page 0 can't be mapped.
- include/frontend.h TEX (12 bytes) is filled by func_001C334C as a TEXINFO (14): on the PS1 the 2 bytes land in
  the stack slot's padding, on PC they hit the next local (mcard.c func_000E6D60's font pointer -> no menu text).
  PC: TEX gets 2 padding bytes.
- Function pointers splat printed as `.byte` runs (the frontend's menu tables, e.g. city select D_001CE048, and
  main's mission handler tables) got no fixup: pc_gendata.py now also fixes an aligned word of a .byte/.short run
  whose value is exactly a game function's entry address.
- The career ladder after a new game / loaded save (first exercised by the garage-skip mod; passing the test for real
  takes the same path) had three PC faults: (1) `modelpointers` slots no level has filled since boot are NULL and the
  apartment view's first `func_000148C4` -> `func_0001EEB4` reads `modelpointers[tile][1]` before InitSpooling (the
  PS1 reads its NULL garbage area): pc_main.c points still-NULL slots at `pc_lowram` at boot. (2) FRONTEND.BIN .data
  0x1CF308-0x1CF324 holds eight raw scratchpad pointers (ladview.c's D_001CF308..D_001CF324, 0x1F8000xx):
  PC_OverlayLoad rewrites them to `pc_scratchpad`. (3) ladconfirm.c (3 loops) and ladoptions.c loop on DrawSync +
  the buffer flip with no VSync, so nothing was presented ("Get A New Job" froze, D1_QUIT never fired):
  `PC_LadderFrame()` paces them like ladder.c's own loop (poll VSync(-1) until 2 vblanks passed).
