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
   Claude-Session: https://claude.ai/code/session_0174xTejS3Fw92o4hCdLZBZH

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
### Batch 3 (camera denting bcollide objcoll gamesnd sound convert+sky loadview+system texture objanim overmap director pres civ_draw - all byte-identical, combined link of the 14 TUs OK)
- Two more merges, each one block afterwards: convert.c -> sky.c (D2 CONVERT line order runs through both, sky's gp .sbss
  spans ScaleCamera/Getlong, convert's only data D_0008AA38 sits right before sky's) and loadview.c -> system.c (.sbss
  0x974FC.. alternates system/loadview variables each side uses alone, 0x97524 is $gp in both; .sdata and .rodata
  interleave too). One shared $gp symbol alone is NOT enough: texture.c/objanim.c both $gp D_000975AC - kept apart.
- A <=8-byte initialised array stays in .data with `extern T x[];` first AND a size-less definition `T x[] = {..};`
  (director's icon lists, pres D_0008B704, system D_0008AB94); with an explicit size (`[2]`) it goes to .sdata anyway.
- Address form: a complete small type + abs= gives maspsx's `la` (lui/addiu in ONE register, D_00097654); an
  incomplete first declaration gives cc1's split lui/addiu (D_00096E28) - keep whichever the matched pilot had.
- Constants the code copies whole are file-scope consts in .rodata at the right point: `const VECTOR D_00010C84`
  (overmap), `const S9 D_00010920 = { "\\DRIVER\\" }` between loadview's and system's functions - closes .rodata holes.
- Literals of <= 8 bytes go to .sdata, data-pointed ones included: overmap's level-name table is defined after
  D_00096D60 so its names land behind it (moving a .data definition down keeps .data order, it is another section).
- Unreferenced strings: cc1 2.7 emits the literal of `if (0) printf("...")` with no code (tested), which explains
  sound.c's "InitSound\n".."---------------\n" (.rodata 0x106D0-0x10897) and the .sdata "Pause\n"/"%d" (sound),
  "%d"/"%x" (pres). Not done: which function each lived in is a guess; the .sdata ones are char arrays for now.
- Interior labels the original names (VECTOR .vy D_0008A1E0, camera_position.vz D_0008A8F8, RECT +2/+1 bytes,
  D_00097699/9A): define the whole object, keep the label `extern` (linkcheck PROVIDEs it).
- An 8-byte .sbss object 4 bytes into the file's .sbss needs sbss_align8=0 (sky D_000974E0, texture's RECTs).
- Left undefined (no owner evidence): D_000974B4 (objcoll|gamesnd), .sdata 0x96E4C-0x96E5B (after civ_draw, before
  felony), the "" words at 0x96CCC/0x96CE0. Placed by position only: camera_position + D_00096C48 (objcoll, could be
  debris), D_0008ABAC (objanim, could be texture), D_000975A8 (texture). .bss stays extern in all 14 files.
- Helpers (untracked): build/tu4-tools/own.py (labels of a range with users and $gp/abs per TU), gen.py/enc.py (C
  initialisers from asm/data, enc.py by member widths incl. .asciz), apply.py (insert definitions, drop the externs
  they replace, `extern long long` -> int + abs=).
### Batch 2 (pedest players pathfind felony job_fx misc_636 mission+overlay handlers pause fmvplay flyby scores ladder xaplay xmplay - all byte-identical, combined link of the 15 TUs OK)
- mission.c + overlay.c are ONE object, merged (91ad0d3): overlay's sprintf format (0x110C8) sits between mission's code
  strings and mission's jump tables (one object = all strings, then all jump tables), and overlay's D_0008C228 points at
  "Checkpoint" inside mission's data-string run. Merge needed only: TIMER renamed away, overlay's GS view of D_000C6F04 ->
  MISSION (t.f[2/6/7/8] = st.f0/f10/f14/f18), D_0008BD58 read through (MSG *), func_0004FFE8 K&R. .rodata one block.
- The .bss TAIL is laid out file by file (pathfind 0xC039C, felony 0xC4690 PURSUER[5], job_fx 0xC6CD8, mission
  0xC6EA8 + MISSION, flyby 0xC71EC, xaplay 0xC782C, xmplay 0xC7888-0xC7A10, then the library): plain tentative
  definitions in address order place as ".bss one block" - risk 5 does not apply there. Interior names other files use
  (D_000C4E38 = [1], D_000C241C in the cell map, D_000C7034 in MISSION) just stay extern. Tiny objects at the edge of a
  .bss block (D_000C6F00/01/02) must be the tail of the previous array: as chars they go to .sbss.
- A size-less definition alone (`void (*D_0008BA08[])(..) = {f};`) keeps a 4-byte initialised array in .data; the
  incomplete extern first only matters for how the code addresses it.
- Literal order, measured: one initialiser's literals come out in reverse element order (overlay's label tables,
  xaplay's MV1..MV5); each init-declarator's literals are emitted BEFORE its variable. So pause's `char *D_00096F20 =
  "Restart", *D_00096F24 = "Try It"` (pointers first, strings after them in .sdata) cannot be literals: pause's menu
  strings are named const arrays (<= 8 bytes land in .sdata, longer in .rodata, definition order = address order).
- A literal that precedes the first function's literals in .sdata although only a late function uses it ("+%d sec")
  comes from an inline function defined at the top: gcc expands an inline body - and outputs its constants - at its
  definition (overlay's ShowNum moved up, byte-identical).
- Incomplete-extern-first is NOT free for small arrays the code fills (pause D_000977B4/B8 char[4]): cc1 then keeps the
  address in an s-register (+0x28 bytes). Keep the complete type and put the name in abs=.
- A 1-byte variable at the end of a .sdata label block (flyby D_00096F8C, xmplay D_00096FA8) leaves the label's padding
  uncovered ("resumes at unaligned"); fill it with scalar chars - a char[3] is word-aligned and overruns the next file.
- linkcheck cannot place a .sdata block no code relocates against (pedest's candidate words 0x96E4C-0x96E5B): left in asm.
- mission's five message strings 0x10FAC-0x1100F stay an extern label: as literals they come out in the right order, but
  the object's .rodata then starts at 0x10FAC (4 mod 8) and the `.align 3` jump tables land 4 bytes early; the original
  has a zero word at 0x110D4 there - unexplained.
- Left extern (.bss, no clean per-file block): pedest's 0xBFA7C-0xC039C (the pedestrian array is cleared here but
  civ_draw.c reads D_000C0168 alone in the middle), players/handlers/pause/ladder/misc_636 (shared .comm only), scores.c's
  tables (0x868EC.., in main.c's early .data run).
- Helpers (untracked): build/tu4b/{own.py (labels + users + gp/abs per range), dat.py (C initialiser from the exe), gen.py
  (spec -> definitions, drops replaced externs), ll.py (extern long long -> int + abs=), lit.py (string label -> literal),
  cmpfn.sh (per-function compare that ignores relocation-form diffs)}.
### Loose ends (last `extern long long` data symbols, main's bss start)
- debris.c/draw.c: the last 29 data `extern long long` became int/short/pointer + abs= (build/tu4b/ll.py, then types
  by hand from the casts); the `__asm__` aliases stay. No data `extern long long` is left in src/*.c.
- slus_008.42.yaml now starts main's bss at 0x96FBC (rom 0x877BC). The file still runs to 0x87800 (2 KB zero padding),
  so the Makefile and linkcheck objcopy with `--pad-to=0x87800` (targets.py `pad_to`). splat writes asm/data/877BC.bss.s
  with labels for 0x96FBC-0x96FFF (.word 0 in .bss); linkcheck places each at its address and skips the ones a C
  object defines. SBSS_LO is gone. After a re-split delete the stale asm/data/87800.bss.s (Makefile finds every .s).
- handling.c's .sbss was in pieces because its byte variables were packed, not because of 0x97000: scalar char fillers
  after D_00097028/2C make it one block.
