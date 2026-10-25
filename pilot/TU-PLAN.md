# Driver 1 decomp: real translation units, shared headers, then the PsyCross port

Written 2026-10-25. This is research only and nothing under ~/dev/driver1 was changed.
Scripts and intermediate data are in `~/dev/driver1-work/tu-plan/scripts/`. They read `asm/2C8C.s`, `symbol_addrs.txt`, `pilot/*.tsv` and `src/pilot/*.c` and run with plain python3 in about a second each.
The full table of proposed files is in `tu-table.md`, next to this file.

## 0. Findings that shape the plan

1. **There is no alignment padding between objects in main.** 797 of the 799 game functions end directly against the next one (at most the `jr ra` delay-slot nop). The one exception is `func_00026194`, which has 13 extra nops (padding) before 0x26794. Padding can't find file boundaries here.
2. **D2 source-line order survives in D1 address order.** Inside every D2 file that has several anchors (SPOOL 28, XMPLAY 59, SOUND 32, DEBRIS 48, DIRECTOR 20), D1 addresses rise with D2 `line` numbers. D2 binary offsets do not follow line order, but D1 addresses do. So GCC emitted functions in source order, and the D2 file of a matched name is a reliable anchor.
3. **Small data is laid out per object, in text order.** I took symbols used by only one proposed file and sorted them by address. TU order runs backwards in only 10 of 235 adjacent pairs in .sdata, 9/155 in .data and 15/281 in .sbss. .bss gives 35/241, which is expected because it is `.comm` allocated by the linker. The first local .sdata address of each file rises strictly from tile.c (0x96938) to xmplay.c (0x96FA0). This independently confirms the file order. It also gives each file's .sdata/.sbss slice: file k owns from its first local symbol up to the next file's.
4. **gp-relative versus absolute addressing depends on the file, not the function.** 204 symbols are declared `extern long long` (absolute) in some pilot files and with their real type (gp-relative) in others. For 182 of them the split falls exactly on the proposed file boundaries. Only 22 are mixed inside one proposed file, and those mark either wrong boundaries or a second effect such as the maspsx `.comm` rule. So the `long long` trick is a per-file property. A real file needs one declaration plus a per-file addressing list, not 831 type lies (section 3).
5. **The 56 `HANDWRITTEN ASM` functions are original assembler modules, not compiler output.** All of them sit in five contiguous runs: 0x26140-0x270D4 (118-139), 0x2C53C-0x2C6DC (165-172), 0x30380-0x31734 (187-211), 0x35B5C (254) and 0x3DF94-0x3E244 (309-310). Each run is its own `.s` object in the original link.

Method, in `scripts/`:
- `funcs.py` parses the asm into functions, calls and references.
- `d2files.py` turns spanish_dump.h into a list of 1,182 functions with file and line.
- `align.py` attaches D2 files by exact name or through d2map.tsv, which anchors 229 of 799.
- `callcut.py` counts local call and data edges (span 25 or less) crossing each gap; 86 gaps have zero crossings.
- `dataown.py` assigns data ownership and checks ordering.
- `externs.py` scans the pilot `extern` declarations.
- `tus.py` holds the hand-made boundary list and is the place to edit it.

## 1. Proposed translation units for main (SLUS_008.42 game code 0x12498-0x738F0)

**49 files** cover all 799 functions: 17 high confidence (323 functions), 16 medium (238) and 16 low (238). The PsyQ libraries from `__main` at 0x738F0 stay as library objects; `pilot/psyq_objs.tsv` already names them.
The table below is condensed; `tu-table.md` has the full version with address ranges and data slices.

| # | file | functions (idx) | address | conf | main evidence |
|---|---|---|---|---|---|
| 1 | clearmem.c | 0 | 12498 | low | D2 SYSTEM ClearMem alone at the start of the exe |
| 2 | ai.c | 1-62 (62) | 1250C-1B380 | low | no anchors; cop/civ AI with state handlers reached by function pointer; probably 2-3 files (best inner cuts at 14 and 56) |
| 3 | handling.c | 63-85 | 1B380-212F0 | med | D2 HANDLING x9; WHEELFORCES (StepCars/StepOneCar) and DR2ROADS MapHeight sit between them, so D1 had one physics file that D2 split up |
| 4 | roadbits.c | 86-97 | 212F0-22168 | low | ROADS_GetRouteData and route helpers |
| 5 | main.c | 98-109 | 22168-24754 | high | LoadGameLevel, InitModelNames, main loop 0002360C, main |
| 6 | glaunch.c | 110-117 | 24754-26140 | med | ReInitFrontend, FRONTEND.BIN/GLOAD; the gp evidence (D_0009711C) suggests it may be part of main.c |
| 7 | (asm) tile_asm.s | 118-139 | 26140-270D4 | high (as asm) | 21 of 22 functions are HANDWRITTEN; 13 nops of padding at 26794 means at least two .s modules (118-119, then 120-139) |
| 8 | tile.c | 140-149 | 270D4-298B0 | low | C callers of the asm (140 is called by DrawMapPSX) |
| 9 | shadow.c | 150-162 | 298B0-2BDB0 | high | D2 SHADOW x6 in line order, tyre-track adders, CARSHAD init; zero cut at 163 |
| 10 | draw.c | 163-186 (minus asm 165-172) | 2BDB0-30380 | high | D2 DRAW x6 incl. DrawMapPSX; asm 165-172 is its own .s object, which makes 163-164 their own small C file |
| 11 | (asm) mdraw.s | 187-211 | 30380-317B0 | high (as asm) | 24 of 25 HANDWRITTEN |
| 12 | models.c | 212-215 | 317B0-32DEC | low | "Model %s was not found", texture names |
| 13 | spool.c | 216-247 | 32DEC-357B4 | high | 28 SPOOL anchors |
| 14 | pad.c | 248-252 | 357B4-35B50 | med | PadInitDirect |
| 15 | misc_253.c + asm 254 | 253-254 | 35B50-35BC8 | low | |
| 16 | motion_c.c | 255-275 | 35BC8-39450 | med | 21 NormalClip draw handlers incl. wjmDraw3 |
| 17 | map.c | 276-288 | 39450-3A100 | med | MAX_ROADS lump readers, CheckUnpackNewRegions |
| 18 | map2.c | 289-291 | 3A100-3A698 | low | |
| 19 | cars.c | 292-300 | 3A698-3C998 | high | DrawCar, DrawCarWheels, MangleWheelModels |
| 20 | camera.c (+ asm 309-310) | 301-310 | 3C998-3E540 | high | |
| 21 | denting.c | 311-319 | 3E540-3F2E0 | high | zero cut at 320 |
| 22 | bcollide.c | 320-325 | 3F2E0-40BAC | high | |
| 23 | debris.c | 326-380 (55) | 40BAC-4A9F4 | high | 30 anchors; D2 SKY DrawLensFlare/DisplaySun are inside, so in D1 they were in debris.c |
| 24 | objcoll.c | 381-385 | 4A9F4-4C1C8 | med | CheckScenaryCollisions-class function 383 |
| 25 | gamesnd.c | 386-398 | 4C1C8-4D3DC | med | |
| 26 | sound.c | 399-431 | 4D3DC-4E948 | high | |
| 27 | convert.c | 432-439 | 4E948-4ED34 | med | |
| 28 | sky.c | 440-449 | 4ED34-503F4 | low | ScaleCamera/Getlong (D2 CONVERT) sit inside the sky functions: either one file 432-449 or D1 moved them |
| 29 | loadview.c | 450-455 | 503F4-50E28 | med | |
| 30 | system.c | 456-466 | 50E28-516AC | high | |
| 31 | texture.c | 467-481 | 516AC-52C10 | high | |
| 32 | objanim.c | 482-491 | 52C10-539C8 | high | |
| 33 | overmap.c | 492-524 | 539C8-58814 | med | D2 OVERLAY DrawPercentageBar and TARGETS DrawStopZone are inside |
| 34 | director.c | 525-553 | 58814-5C68C | med | D2 REPLAYS cjpRecord and REPLAY.BLK are inside |
| 35 | pres.c | 554-562 | 5C68C-5D164 | high | |
| 36 | civ_draw.c | 563-572 | 5D164-5F0EC | low | DrawCiv (D2 MOTION_C) |
| 37 | pedest.c | 573-595 | 5F0EC-61214 | med | |
| 38 | players.c | 596-604 | 61214-61B9C | low | |
| 39 | felony.c | 605-631 | 61B9C-66224 | low | cop/felony cluster; inner cut unknown |
| 40 | job_fx.c | 632-635 | 66224-67838 | med | |
| 41 | misc_636.c | 636-643 | 67838-682C0 | low | |
| 42 | mission.c | 644-657 | 682C0-6ACC8 | med | MISSION.BLK and the DMS interpreter |
| 43 | overlay.c | 658-665 | 6ACC8-6BB24 | med | HUD and timers |
| 44 | handlers.c | 666-678 | 6BB24-6BDA4 | low | 13 tiny functions reached only by pointer table |
| 45 | pause.c | 679-703 | 6BDA4-6D56C | med | menu item handlers, MC launch, pause loop |
| 46 | fmvplay.c | 704-707 | 6D56C-6DD3C | high | |
| 47 | ladder.c | 708-726 | 6DD3C-6F2DC | low | mladder.psx, FB images; D2 GLAUNCH ladder analog |
| 48 | xaplay.c | 727-735 | 6F2DC-6F81C | high | |
| 49 | xmplay.c | 736-798 (63) | 6F81C-738F0 | high | 52 anchors |

Counting the asm modules separately gives about 46 C files and 5-6 `.s` files. The low-confidence rows decide the final count, which should land around 45-55 C files.

**Boundaries to check first.** These are the uncertain splits, in order of the damage a wrong one would do:
- ai.c (62 functions, no anchor), felony.c, players.c, handlers.c and misc_636.c: all guesses.
- handling.c/roadbits.c/main.c at idx 86 and 98.
- main.c/glaunch.c: the gp evidence says these may be one file.
- convert.c/sky.c at 440.
- The ownership of 163-164 and 186 around the draw.c asm.
- clearmem.c (0) and gamesnd.c's first 3 functions (386-388).

How to check each one:
- **(a) gp signature:** for every function, list which small globals it reaches through gp and which absolutely. A boundary where the pattern changes is real (finding 4).
- **(b) data order:** a proposed file's local .sdata/.sbss must form one contiguous block (`dataown.py`).
- **(c) the build:** compiling the merged file must put its rodata at the original addresses. A wrong boundary breaks this right away (Step 2).

**Overlays:** one file each for FRONTEND (~362 functions; D2 has one 40-function FEMAIN.C, but D1's frontend may hold several files, so run the same `callcut`/sdata analysis with `--target frontend`), CRED (13), IVIEW (23) and MCARD (35). D2's MEMCARD was C++ in 7 files (DISPLAY/SLOT/PAD/MAIN...); D1's 35-function MCARD is more likely 1-3 C files, so check the symbols for mangled names before deciding. NFMV.EXE is its own program and should mirror the PsyQ MOVIE sample layout.

## 2. Data ownership

- The function→data pass assigned 916 symbols to exactly one proposed file. These are the ones that can become `static` or file-scope definitions, and they are listed per file with ranges in `dataown.json`. Another 361 symbols are used by several files and belong in shared headers.
- **Most shared:** D_00096834 (25 files, .sdata), car_data (22, .bss 0x9B0A4), camera_position (20), current (19, .sbss), rcossin_tbl (18), modelpointers (18), CameraCnt (15), gNight, pauseflag, inv_camera_matrix and cameraview (11-12 each). The full list is in `scripts/topshared.json`.
- **Per-file data slices.** .sdata (0x96770-0x97000) and .sbss (0x97000-0x97848) can be cut into per-file blocks at each file's first local symbol; the table gives the start addresses. .data (0x85314-0x96770) is ordered per file in the same way.
- **.bss above 0x97848 is mostly `.comm`.** Its order is the linker's, not the file's. It stays in one generated `bss.s`/linker-script block during migration, and each file declares (not defines) those variables. Only `static` (.lcomm) arrays move into their files.

## 3. Shared headers

**Size of the conflict.** 1,248 matched C files hold 4,444 `extern` lines covering 1,521 symbols:
- 479 symbols have more than one type spelling, and 81 have more than three.
- There are 1,771 `typedef struct` definitions across 601 files, mostly anonymous local shapes (`E`, `O`, `S`, `Q`, `S84`...).
- 831 extern lines (236 symbols) use the `long long` gp trick.

Worst cases:

| symbol | decls | distinct types | notes |
|---|---|---|---|
| car_data | 90 | 32 | CAR[] / int[] / CAR_DATA[] / partial shapes. This is **the** struct, so build it first from D2 `_CAR_DATA` (REDRIVER2 dr2types.h) and check every field offset against the 90 files |
| D_00098950 | 41 | 20 | ENT/CIV/S80/SLOT: civ/ped entity array |
| D_000BD498 | 23 | 15 | sound channel array (CH/CHAN/S84) |
| D_000C6F04 | 20 | 13 | player array (PL / PlayerS / int[]): D2 `_PLAYER` |
| XMC | 30 | 12 | XMCHANNEL*; XM player structs come almost verbatim from D2 XMPLAY |
| D_0009B230 | 22 | 12 | second CAR-like array |
| current | 125 | 10 | 86 x `long long`, 31 x `DB *`: draw buffer (D2 `DB *current`) |
| car_cosmetics | 16 | 9 | COS/MODEL/...: D2 `CAR_COSMETICS` |
| camera_position | 65 | 8 | VECTOR vs int[] |
| modelpointers | 27 | 7 | MODEL*[] |

Other header families: MODEL, DB/DRAWENV, PLAYBACKCAMERA (replay camera, 4 definitions), menu item records (pause.c and FRONTEND), mcard slot records, cell/road/junction records (D2 `DRIVER2_JUNCTION` etc. will not fit as-is), and the spool/texture structs (TEXINF).

Headers to create:
- `include/driver.h`: base types plus the PsyQ includes.
- `include/structs.h`: CAR_DATA, CAR_COSMETICS, MODEL, PLAYER, CIV/ped entity, DB, PLAYBACKCAMERA, XM structs. Reconstruct each from the most complete pilot shape, take field names from D2 dr2types.h where the offsets agree, and put `_Static_assert`-style offset checks behind a `#ifndef PSX` (or a negative-array trick) so a layout drift fails to compile.
- One `include/<file>.h` per file for its globals and prototypes.

**The gp constraint.** With MASPSX_EXTERN_G=4, a 4-byte extern is addressed gp-relative. Finding 4 says the original choice belonged to the file. The likely mechanism is aspsx's: gp only for symbols the object defines in .sdata/.sbss or `.comm`'s, plus externs the headers sized at most -G. Recommended strategy, in order:
1. **Put the definitions in their owning files.** Many `long long` uses should then disappear, because the defining file now really owns the symbol, initialised or `.comm`. Measure this on one file (spool.c, which has 46 local .sbss symbols) before deciding anything else.
2. **Handle what remains with per-file lists, not type lies.** Extend `tools/maspsx-extern-gp.patch` with a per-file list of forced-absolute and forced-gp symbols, e.g. `MASPSX_ABS_SYMS=@build/gp/<file>.txt`, generated from the pilot `long long` declarations, which already say exactly which file wants which addressing. The headers then declare each global once with its real type. The same mechanism covers the 22 mixed-inside-a-file symbols if any survive correct boundaries.
3. **Fallback without patching maspsx:** declare the absolute-only globals in the header as incomplete arrays or `extern T sym[]`-style, with an access macro. This is what several PS1 decomps do for "size unknown → absolute". It is uglier and touches every use, so use it only if (2) fails.

Also keep `-G8` per file but allow a per-file `-G0`. It is cheap to test whether any original module was built with -G0, because a file with no gp access at all is the signature.

## 4. Migration plan (byte-identical at every step)

Invariant: after each step, `make` gives sha1 94b44a84 for main (and each overlay's sha1), and `tools/linkcheck.py --all` passes. One file per commit.

- **Step 0: infrastructure** (1 agent, about 1-2 h).
  - Today `make` does **not** link src/pilot at all; the Makefile excludes it, and only linkcheck does the real-build substitution. Add a `src/<file>.c` build path to the Makefile.
  - Add a linkcheck `--tu FILE` mode: compile one multi-function object, map its `.text` symbols onto the original asm block range, and place its .rodata/.sdata/.sbss as one block at the file's slice. This generalises the per-function placement code linkcheck already has.
  - Write `tools/mktu.py <file> <first> <last>`. It concatenates the pilot files in address order, collects their externs and typedefs into a file-local prelude, deduplicates identical ones, and lists the conflicts.
  - Write `tools/gpsig.py`, the per-function gp/absolute signature from finding 4, to confirm boundaries before any merge.
- **Step 1: pilot one high-confidence file end to end** (coordinator, ~1 h). spool.c, because it is self-contained and has 46 .sbss locals: merge, build, compare. This settles three questions: (1) does rodata/sdata ordering fall out naturally, (2) what happens to `long long` once the definitions move in, (3) do merged files change codegen at all (GCC 2.7 -O2 doesn't inline across functions unless asked, so this should be safe).
- **Step 2: merge files without moving data** (3 agents in parallel, each taking whole files). In each file, data stays in asm and the file uses file-local externs, conflicting ones unified into one spelling per file. Merge the high files first (17), then medium (16), then low (16), whose boundaries the build itself confirms or refutes. Measured rates: mechanical edits with verification run at about 25-30 functions/min (223 renames in ~8 min), and type unification is the slow part. Guess **10-25 min per file**, so about **49 files in 3-4 h of wall time with 3 agents**. Overlays (4-6 files) add about 1 h.
- **Step 3: shared headers** (1 agent for the core structs, then fan-out). First build `structs.h` with CAR_DATA, PLAYER, MODEL, DB, entity, cosmetics and XM (~2 h, with offset asserts). Then the fan-out: 3 agents swap file-local typedefs for the shared ones, one file per batch, with each file's build as the check (~2-3 h).
- **Step 4: move data definitions into the owning files** (3 agents, after Step 3). Order: .sdata/.data first (initialised, with exact addresses known), then .sbss/static .bss, then decide on `.comm`. Remove `long long`, then apply the per-file gp list from section 3. About 2-3 h.
- **Step 5: names and cleanup** (optional, can run in parallel). Rename D_ symbols that now have an owner, add prototypes to the per-file headers, and drop `extern` lines from the .c files.

Rough total: **about 12-16 agent-hours, about 5-7 wall hours with 3 agents**, so one long run or two evenings. The note in memory says estimates run long, and Steps 2 and 4 are the mechanical ones, so expect the low end. The wildcards are the 22 mixed-addressing symbols and the low-confidence boundaries; budget extra rework for ai.c and the felony/players area.

## 5. What the PsyCross port then needs (short)

- **56 HANDWRITTEN ASM routines, 5 groups.** Rewrite them in C, as REDRIVER2 did with `Game/ASM/rndrasm.c`, `d2mapasm.c`, `compres.c` and `rnc_2.c`:
  - 0x26140-0x270D4, 21 functions: unpacker (0x26194) plus tile/poly GTE helpers under DrawMapPSX. The D2 equivalents are in REDRIVER2's `rndrasm.c`/`d2mapasm.c`.
  - 0x2C53C-0x2C6DC, 8: small GTE/matrix helpers used by draw/cars/debris.
  - 0x30380-0x31734, 24: the model/primitive renderer (mdraw).
  - 0x35B5C, 1.
  - 0x3DF94/0x3E244, 2: camera-side GTE maths.

  The 45 FAKEMATCH files need a clean C version kept behind `#ifdef PSX`.
- **32-bit pointers.** Pointer-to-int casts like `(int)&` appear on 32 lines in 9 files (DrawMapPSX, DrawExplosion, 0003A698, 0004DDA8, 0003837C, four FRONTEND files). 51 files use hard-coded addresses (0x1F80xxxx scratchpad, 0x8xxxxxxx). Beyond those, the real exposure is every struct that stores pointers in 32-bit slots, and the ordering table/primitives. REDRIVER2 solved it with `psyx_compat.h` (`intptr`/`u_intptr`, `OTTYPE` widened to 64-bit, `P_LEN`). Adopt those types in the shared headers from Step 3 onward so the PC build needs no second pass, and keep fixed-size on-disc records (`int`/`short`) apart from runtime pointer fields.
- **CD and file loading.** Loadfile, LoadfileSeg, loadsectors, sector_ready and the CdSearchFile/CdRead use in 0x503F4, plus spool.c's CdControl-driven streaming. REDRIVER2 `C/system.c` uses `#if USE_PC_FILESYSTEM` (fopen against an extracted DRIVER/ tree), and spool callbacks run synchronously. D1 can follow the same pattern.
- **Overlays** (FRONTEND/MCARD/CRED/IVIEW). REDRIVER2 uses `LOAD_OVERLAY(...)` (glaunch.c), which is a no-op off-PSX, and links `Frontend/FEmain.c` statically. Do the same: all overlays become plain files in one PC executable. CRED and IVIEW share 0x1E0000 but never coexist, so check them for symbol clashes.
- **FMV.** D1 runs NFMV.EXE as a separate program. REDRIVER2 `C/fmvplay.c` calls an in-process `FMV_main()` (PsyCross MDEC/STR decoding) instead of loading `FMV\FMV.EXE`, so port NFMV's player logic into the game the same way. XA audio (xaplay.c) needs PsyCross's XA path or a WAV substitute, and XM music (xmplay.c) is SPU-only and can run on PsyCross's SPU emulation as in REDRIVER2.

## Files

- `PLAN.md`: this file.
- `tu-table.md`: the full 49-row table with idx, addresses, data slices and evidence.
- `scripts/`: the analysis scripts plus JSON/TSV outputs:
  - `main_aligned.tsv`: per function, its D2 file/line anchor.
  - `overview.tsv`: callers, callees and strings for every function.
  - `callcut.json`, `dataown.json`, `externs.json`, `topshared.json`.
  - `tus.py`: edit its boundary list and re-run `tus.py` then `dataown.py` to re-validate.
