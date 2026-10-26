# Driver 1 PC port (PsyCross)

The decompiled game (`src/`) built as a native 32-bit Linux program on [PsyCross](https://github.com/OpenDriver2/PsyCross)
(vendored in `port/psycross/`, MIT), the PsyQ-on-SDL2/OpenGL layer REDRIVER2 uses. Status: it builds, links and
boots through the legal screen, the GT and Reflections splashes to the frontend title screen. No sound streaming
(XA), no FMV, no memory card yet.

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
`D1_SHOT=n1,n2,...` writes `gl_<n>.ppm` (the window) and `shot_<n>.ppm` (the displayed VRAM area) at those vblank
counts (the VRAM dump comes out red/blue-swapped next to the window grab - see the open issues in the
port notes); `D1_QUIT=n` exits after n vblanks. A crash prints a backtrace (`addr2line -e driver1 <addr>`).

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

### Frames
The game never calls `PsyX_EndScene` (REDRIVER2 does, from its game code), so `VSync` is wrapped
(`-Wl,--wrap=VSync`, `pc_main.c`): a drawn scene is presented there, and pending CD/MDEC work completes there.

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
| 77E1C..7932C (12) | MemCardInit/Start/Stop/Exist/Accept/ReadFile/WriteFile/GetDirentry/Sync/CreateFile/DeleteFile/Format | "Access Denied" strings, mcard.c use | pc_psyq.c: "no card" for now |
| 73AC4, 73AF8, 73B74, 73B94, 768E4, DecDCTvlcBuild | DecDCTReset, DecDCTin, DecDCTout, DecDCToutCallback, DecDCTvlc | MDEC, CRED.BIN portraits | pc_psyq.c stubs (callback chain kept) |
| 7EB24, 7EBE8, 7EB34, 7EC78, 7C744 | Exec, FlushCache, ExitCriticalSection, EnterCriticalSection, exit | BIOS A(43h,44h), syscalls, B(38h) | pc_psyq.c |
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
  decoded empty SPU RAM ("sample has no end flag"). sound.c LoadSoundBank calls SpuWrite under PSX_PC; spool.c SendSBK (the in-game spooled bank) needs the same.
- Audio: xmplay.c func_00073738 called `SilenceXM()` with the id left in a0 (same pattern as sound.c func_0004E67C);
  PC passes it (crashed in ReInitFrontend after the attract demo).
