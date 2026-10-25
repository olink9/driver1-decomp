# TU migration, step 4: each file owns its data (read pilot/TU-PLAN.md section 2-3 and ALL of pilot/TU1.md first)

Two files already did this and are the models: **src/spool.c** (Step 1 pilot: .data/.sdata/.sbss all defined in C,
`/* maspsx: abs=... gp_allow_offset=1 sbss_align8=0 */` lines) and **src/debris.c** (its named .sdata moved into C so
the TU could take DisplaySplashes..; definition placement sets .sdata order). Their file headers and the TU1.md
lessons "Step 1 pilot" and "main leftovers" are the recipe. Data ownership per file: ~/dev/driver1-work/tu-plan/
scripts/dataown.py / dataown.json (re-run it; boundaries moved since the plan - the TU files in src/ are the truth),
and tools/gpsig.py (a gp-relative access marks ownership in this exe).

For each file in your batch, in address order:
1. Find the file's slices of .data, .sdata, .rodata (strings/jump tables still referenced as extern labels), .sbss
   and .bss: every address in the slice, INCLUDING symbols only other files use and unnamed gaps (define those as
   plain variables so the block stays whole). Initialised words come from asm/data/*.data.s / *.rodata.s.
2. Define them in C in address order (with real types from include/structs.h etc.); turn the file's extern string
   labels into C literals where they are the file's own .rodata (build/tu-tools/lits.py; re.sub needs a lambda).
3. Remove `extern long long` from the file: symbols the original reaches with %hi/%lo go in the file's
   `/* maspsx: abs=... */` line; `gp_allow_offset=1` / `sbss_align8=0` / an incomplete extern declared first, as the
   spool.c lessons explain. Other files' externs of a symbol you now define must keep compiling (they stay extern).
4. Check: `linkcheck --tu src/<file>.c --workdir build/tu4-<file>` byte-identical AND every section reported as
   "one block" where possible; run pilot/scripts/cmpfn.sh against the pre-change object for per-function diffs.
   linkcheck drops the asm labels the C data now covers - nothing in asm/ or the splat config needs editing unless
   the report says so. .bss laid out by the linker as common (.comm) may not be placeable per file (TU-PLAN risk 5):
   leave .comm variables extern and list them.
5. One commit per file, explicit paths, message `TU4 <file>.c: owns its data (.data N, .sdata N, .sbss N, .rodata N)`,
   ending exactly with
   Co-Authored-By: Claude Opus 5.5 <noreply@anthropic.com>
   
Rules: work ALONE (no sub-agents); nothing wider than -j2; `</dev/null` in shell loops around distrobox; no push;
don't touch files outside your batch (other agents work on other files at the same time); don't run the full make /
linkcheck --all (the coordinator does). If a file's data cannot be made one block, stop at what works and say why.
Final reply per file: what moved (counts per section), abs/gp lines needed, what stayed extern and why, commits;
then lessons (append them to this file under "## Lessons" with your batch name, explicit-path commit).
### Batch 1 (clearmem civ_ai ai handling roadbits main+glaunch tile shadow quad draw models pad motion_c map cars - all byte-identical, combined link OK)
- main's real .sbss starts at 0x96FBC (civ_ai 0x96FBC-, handling -0x97037): splat calls 0x96FBC-0x96FFF .data zeros. linkcheck
  now places a C .sbss piece there like data (SBSS_LO, c39c090); a slice straddling 0x97000 is cut at its labels, and an
  unreferenced filler label rides with the piece before it (51b5176). Fixing slus_008.42.yaml (bss from 0x96FBC) would be cleaner.
- Section placement is cc1's size rule, not the original's: an 8-byte initialised array cc1 puts in .sdata needs
  `__attribute__((section(".data")))` (+ an incomplete extern first when the code reaches it with %hi/%lo: civ_ai, tile,
  models, pad); the other way round (a >8-byte run inside .sdata) maspsx cannot parse `.section .sdata`: define the words
  as scalars (shadow). char[] names in .sdata (`char X[] = "DLOGO";`) need the incomplete extern first too.
- Byte globals in .sbss sit 4 apart (D_00097050/54, D_000972DC/E0, D_00097028/2C): maspsx packs consecutive chars, so
  scalar `unsigned char` fillers keep the block whole (a char[3] gets .align 1). A per-file maspsx "sbss byte vars are
  4-aligned" option (psylink .comm alignment?) would replace them.
- Ownership by POSITION beats "who references it": draw.c's dispatch tables D_00089FFC/D_0008A1BC are motion_c's (every entry
  is a motion_c function, and they sit after pad's .data); pad's three byte patterns precede its pointer table D_00089F0C
  (they looked like models'); texture.c's three BLOCK100 tables sit inside handling's .data; system.c's level-name table
  and the score tables are main's. A $gp access by a file that does NOT own the slot = that file's tentative (.comm)
  declaration (mission.c on main's D_0009686C) - but an edge function touching ONLY its neighbour's $gp data belongs to
  the neighbour: func_000298B0/func_000298BC moved shadow.c -> tile.c.
- main.c + glaunch.c are ONE object, merged (df77ac3): MCARD.BIN/FRONTEND.BIN literals shared by both halves and their
  .sdata/.sbss interleave symbol by symbol through $gp. Merge took 6 fixes (two libgpu/libcd prototypes, one typedef,
  two extern spellings, one block-scope int[] view -> `__asm__` alias + #define in that function).
- Strings only .data points at: cc1 emits a pointer table's literals in REVERSE per table at the table's definition -
  main's 16 level/palette names come out exactly as the original 0x10054-0x10183 with two char *[8] tables. linkcheck
  infers such a unit's address from the next unit (78a4acf). main's .rodata still is 8 pieces: GLOAD.TIM + unreferenced
  "frame %d" precede main()'s strings in the original, follow them in this source order.
- lits.py on main: .data holds false-positive `.word D_00010000` (the value 0x10000), so D_00010000 must not count as
  a data-pointed string. Pilot sizes can be wrong (shadow D_00097178 [8] overlapped D_0009717C): the one-block report finds it.
- After the move cmpfn/fd.sh show only relocation-form diffs (`.sbss+off` vs the symbol) - linkcheck is the check; use the
  per-function diff only on a size failure (it found D_00097178 needing abs= and main's D_00097060+1 needing gp_allow_offset=1).
