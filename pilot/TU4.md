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
