# PORT1 - Driver 1 PC port on PsyCross, phase 1: compile, link, boot

Goal of phase 1: `driver1` (PC, 32-bit x86, PsyCross) compiles, links, and boots to the first rendered frame
from an extracted copy of the disc. Later phases: drive in game, replay-driven lockstep vs an emulator, beta.

## Layout (commit 6334668)
- `port/psycross/` vendored PsyCross (MIT; 2 GCC 13 fixes). `port/CMakeLists.txt` builds psycross + every
  `src/*.c`, `src/{frontend,mcard,cred,iview}/*.c` + `port/pc/*.c` into one exe. NFMV is not in it yet.
- `port/pc/pc_compat.h` is force-included into every game file: defines `PSX_PC`, empties INCLUDE_ASM.
- `include/gte.h` uses PsyCross `psx/inline_c.h` (software GTE: gte_* macros call CTC2/MTC2/doCOP2) on PC.
- **32-bit on purpose**: structs keep PS1 layout, so disc data and int<->pointer casts work. Do NOT
  "fix" pointer-size issues for 64-bit now.

Build (in the `build` distrobox; never run it outside):
```
distrobox enter build -- bash -lc 'cmake -S ~/dev/driver1/port -B ~/dev/driver1-work/pcbuild/<you> && make -k -C ~/dev/driver1-work/pcbuild/<you> -j2'
```
Use your OWN build dir (`<you>` = A, B or C), -j2 (fan noise). Reference trees: ~/dev/redriver2-ref
(REDRIVER2 = Driver 2 on PsyCross, MIT: how D2 solved every one of these problems - read it first for your
area), ~/dev/psycross-ref. **Search the web for PS1 specifics** (psx-spx for GTE/CD/GPU, PsyQ docs,
OpenDriver2/REDRIVER2 issues) instead of guessing.

## Hard rules
1. **The PS1 build must stay byte-identical.** Every PC change in shared files sits behind
   `#ifdef PSX_PC` / `#ifndef PSX_PC` (or lives in port/pc/). Do NOT run `make` yourself (the PS1 build
   dir is shared); the coordinator runs `make && make overlays` and checks every sha1 after you report.
   `#ifdef` blocks must not change anything the PS1 compiler sees.
2. Work ALONE: do not spawn agents or subagents. Max -j2. No permuter, no linkcheck runs.
3. Edit only the files your section owns. If you need a change elsewhere, write it under "Requests" in
   your report instead.
4. Do not commit; the coordinator verifies and commits. Do not touch git state (no stash/checkout/reset).
5. Prefer behaviour-preserving translations; when unsure what the original does, read the asm in
   asm_fn/ (or `tools/fnasm.py`) and the D2 equivalent. Mark guesses with `/* PC-PORT: unverified ... */`.
6. Scratch files go in ~/dev/driver1-work/pcbuild/<you>-scratch/, never in src/ or port/.

## Sections

### A - make every game file compile (owner of src/**/*.c except system.c, spool.c, main.c)
The first PC build (log ~/dev/driver1-work/pcbuild/build1.log) leaves 47 compile errors + assembler errors
in: draw, cars, objanim, job_fx, frontend/ladview, shadow, mission, debris, cred/cred, plus files whose
local MIPS inline asm only fails at assembly (`rtpt`, `lwc2`, `swc2`, `break 1024`, `.set noreorder`).
- Local `#define gte_* __asm__ ...` blocks: wrap in `#ifndef PSX_PC` so PsyCross inline_c.h versions apply;
  where a file uses a macro PsyCross lacks, write the C equivalent (CTC2/MTC2/MFC2/doCOP2) under PSX_PC.
- FAKEMATCH asm hints (empty `__asm__`, `__asm__` alias names, register asm vars): PC gets the clean C.
- `break 1024` (asserts/divide traps): PC equivalent (e.g. a logging abort helper declared in port/pc/pc_compat.h -
  ask the coordinator; or a local `#ifdef PSX_PC` no-op/printf).
- Conflicting prototypes (mission.c PrintString/PrintDigit, main.c is C's).
- Then audit hard-coded addresses across your files: 0x1F80xxxx (scratchpad - PsyCross/REDRIVER2 approach:
  a static 1 KB buffer; see REDRIVER2 `_scratchpad`/`getScratchAddr`) and 0x80xxxxxx / 0x00xxxxxx absolute
  pointers. List each with what it is; fix scratchpad uses via a macro you propose for pc_compat.h.
Done = every file you own compiles to a .o with zero errors; report the list of addresses and what you did.

### B - rewrite the hand-written asm and the 11 near-miss functions in C (owner of port/pc/asm_*.c, port/pc/wips_pc.c)
On PC these are missing (INCLUDE_ASM is empty, src/*.s are MIPS):
- src/unpack_asm.s (2), tile_asm.s (20), gte_asm.s (8), mdraw_asm.s (25), rle_asm.s (1), carpoly_asm.s (2):
  58 routines, mostly GTE/primitive code. REDRIVER2 did the same for D2 in Game/ASM/rndrasm.c, d2mapasm.c,
  compres.c, rnc_2.c - many D1 routines have a D2 twin there. Translate each into C using PsyCross GTE
  (inline_c.h macros / gte_* calls), one file per module: port/pc/asm_unpack.c etc. Keep the original
  symbol names and calling convention (args a0-a3, return v0) as C prototypes that match the callers in src/.
- The 11 INCLUDE_ASM wips (main func_00020AE4/00062DB0/00064360; frontend 001C2114/001CD7C0; mcard
  000E7508/000E91B8/000E96F8; cred 001E06B8/001E0F1C; nfmv main - skip nfmv): the non-matching C drafts in
  src/pilot/** and src/*.wip.c are functionally meant to be right - check each against its asm and put the
  PC version in port/pc/wips_pc.c.
Done = all 68 compile in port/pc; per routine a one-line note (source: D2 twin / own translation; confidence).

### C - platform layer: entry point, disc files, CD streaming, overlays, link (owner of src/system.c, src/spool.c, src/main.c, port/pc/pc_main.c, port/pc/pc_*.c other than B's, port/CMakeLists.txt)
- Extract the disc: ~/dev/driver1-work/driver1.bin (SLUS-008.42 image unpacked from his PBP; check its
  sector format) -> ~/dev/driver1-work/pcdata/ (full ISO9660 tree). Write the tool/steps into port/README.md.
- Entry: PC main() that calls PsyX init (window, GPU, SPU, pads) then the game's own main, as REDRIVER2 does.
- File loading: Loadfile/LoadfileSeg/loadsectors/sector_ready and the CdSearchFile/CdRead use, plus spool.c's
  CdControl-driven streaming -> REDRIVER2's `USE_PC_FILESYSTEM` approach (fopen against pcdata/, spool
  callbacks run synchronously). Or PsyCross's own libcd image support if simpler - decide and say why.
- Overlays: loading FRONTEND/MCARD/CRED/IVIEW becomes a no-op (all linked in); FMV (NFMV.EXE) -> a stub
  that returns immediately for now.
- Link: after A/B finish, the remaining undefined/duplicate symbols are yours: PsyQ calls PsyCross lacks
  (stub them in port/pc/pc_psyq_stubs.c with a log line), duplicate D_/func_ names across overlays
  (CRED and IVIEW share 0x1E0000), linker-script symbols. Start with `-Wl,--unresolved-symbols=ignore-all`
  off and collect the list early so the coordinator can see the shape.
Done = disc extracted; files load on PC; you report the full undefined-symbol list (grouped) and
what's left before first boot.

## Report format
Files changed, what each change is, what's verified (compiles / make sha1 OK), open questions, Requests.
