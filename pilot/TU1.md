# TU migration, step 2: merge per-function files into real translation units (read pilot/TU-PLAN.md first)

Your batch names one or more files from the TU table in pilot/TU-PLAN.md (name, first..last function, address range).
For each file:

1. `distrobox enter build -- .venv/bin/python tools/mktu.py src/<file>.c <first> <last> --report` (see tools/mktu.py
   --help). It concatenates the src/pilot files of that range in address order, builds one prelude of includes,
   typedefs, structs and externs, and LISTS conflicts (it never resolves them).
2. Resolve every listed conflict by hand, inside the file: one spelling per type and per extern. Prefer the real
   type over `extern long long` where the build allows it; where a symbol must stay gp-relative in some functions and
   absolute in others, keep the matching spelling and note it (tools/gpsig.py --tus shows these). Keep each function
   body exactly as it was in its pilot file unless a conflict forces a change. HANDWRITTEN ASM functions do not go
   into a C TU (mktu flags them) - leave them as pilot files / the plan's .s modules; stop the TU before or after them.
3. Check: `distrobox enter build -- .venv/bin/python tools/linkcheck.py --tu src/<file>.c --workdir build/tu-<file>`
   must say byte-identical. Run it with YOUR OWN --workdir (other agents run linkcheck at the same time).
4. When it passes: `git rm` the pilot files it replaced (their content now lives in src/<file>.c; FAKEMATCH tags and
   header comments move along with the functions - keep them on the function they describe). Do not touch pilot
   files outside your range, and do not run the full `make` or `linkcheck --all` (the coordinator does, between batches).
5. Commit one file per commit: `TU <file>.c: N functions (0xSTART-0xEND), byte-identical`, ending exactly with
   Co-Authored-By: Claude Opus 5.5 <noreply@anthropic.com>
   
If a boundary looks wrong (a function at the edge needs types that clearly belong to the neighbour, or the TU
won't link as one block), say so in your reply instead of forcing it; shrink the TU to the part that works.
Rules: work ALONE (no sub-agents); nothing wider than -j2; `</dev/null` in shell loops around distrobox; no push.
Final reply per file: functions merged, conflicts found / how resolved, boundary doubts, commit hash; then lessons.

## Lessons (batch A: fmvplay, xaplay, pres, objanim, system, texture - all byte-identical, ~10 min)
- mktu also writes the conflict list into the file's header comment: edit only below the header (a whole-file replace hits it twice).
- Same global read with different widths across pilots: the target's load/store instructions decide the type; cast the odd access.
- "Different" pilot structs are often one record seen through partial shapes: lay out the offsets; D2 names (GARAGE_DOOR, CELL_OBJECT) usually fit.
- int[] -> char[]+cast can reschedule code at the same address: keep the type the scheduling-sensitive function used.
- A file including common.h clashes with unprototyped PsyQ externs elsewhere: until shared headers exist, declare the few GPU types locally.
- Commit with explicit paths (`git commit -- <paths>`) so another agent's staged files can't end up in your commit.
## Lessons (batch B: denting, bcollide, cars, camera, shadow - all byte-identical, ~10 min)
- mktu `#undef` trap (silent): a pilot section that #defines a macro gte.h already provides and #undefs it at the end removes the header's macro for all later sections -> implicit function calls (shows only as a size mismatch). Delete such duplicate define/undef pairs.
- Merged types can change codegen (incomplete int[] -> complete struct; struct-member view -> pointer cast). On a diff, spell the access the way the pilot did.
- Arity mismatches: give the definition the extra unused params its callers pass - same bytes every time.
## Lessons (Step 1 pilot: spool.c - 32 functions, its data moved in, byte-identical)
Answers to the plan's three questions: (1) data order falls out naturally: with the file's .data, .sdata, .sbss and
its one string all defined in C, linkcheck --tu reports each as ONE block (`.data one block at 0x89118`, `.sbss one
block at 0x971E0+0xFC`, ...). cc1 (2.7.2-cdk) emits .comm/.lcomm in order of each variable's FIRST declaration, so
define them in address order. (2) `extern long long` disappears, but not by itself: spool.c now has none - the file's
own small globals are real definitions, and every small global the original reached with %hi/%lo is named in a
per-file `/* maspsx: abs=... */` line instead. (3) Merging changes no codegen: 32 pilot bodies, exe identical.
Step 2 (merge, data stays external):
- Try the volatile spelling for every function before splitting a symbol: spoolcounter/D_000969EC/D_000972D4 were
  volatile in some pilots and plain in others; volatile everywhere matched (the plain pilots just didn't need it).
- A call with fewer args than the definition (UnpackRegion -> LoadRegionData()) can take the real args: same bytes.
  A K&R `f()` definition instead of `f(void)` accepts any call and changes nothing.
- You may already replace `extern long long X;` by `extern int X;` (its real type) plus `X` in the file's
  `/* maspsx: abs=X,Y */` line (tools/fnscore.py file_maspsx_env; several abs= lines add up). cc1 sees both as small,
  so codegen is the same; only maspsx's gp choice differed, and abs= covers it.
Step 4 (move definitions into the owning file) - the recipe that worked:
- Take the slice from dataown.json, then list EVERY address in it from undefined_syms_auto.txt (.sbss) or the data
  asm (.data/.sdata): symbols the file never uses (D_0009722C, D_00097298: used only by main.c/gamesnd.c) and unnamed
  gaps are the file's too. Define the gaps as plain unreferenced globals (`int D_00097214;`) so the block stays in
  one piece; linkcheck now lets labels without an original address ride along.
- Initialised data: copy the words out of asm/data/75B14.data.s into a C initialiser (an `int x = 0;` goes to .sdata
  with gcc 2.7). Nothing has to be removed from the asm or splat config: linkcheck --link/--tu drops the asm labels the
  C data covers (also a partial block, e.g. spoolpos_writing's 20-byte block) and `make` gets the same sha1.
- Run fnscore over every function of the file after the move (a loop over the `/* ---------------- NAME` headers) -
  it names the diff, linkcheck only the first size mismatch. What showed up and the per-file fix:
  * an array the original indexes with lui/addiu (not small to cc1) but that is <= 8 bytes: put an incomplete
    `extern short D_0009728C[];` before `short D_0009728C[4];` - cc1 decides small-data at the first declaration.
  * own variables the original stores with lui $at/%lo (D_0009725C, D_000972CC): add them to `abs=`.
  * `sym+offset` of an own variable is $gp in the original: `gp_allow_offset=1` (the empirical "never" rule was
    derived from pilots that defined other files' bss; for a file's own data, aspsx 2.79's real behaviour holds).
  * an 8-byte .sbss object at a 4-aligned address: `sbss_align8=0`.
- maspsx emits .sbss variables as LOCAL labels (no .globl), so other files still reach them through
  undefined_syms_auto.txt. Fine for byte-matching; a real final link needs them global (later step).
- tools/maspsx-abs-syms.patch adds MASPSX_ABS / MASPSX_SBSS_ALIGN8 (setup.sh applies it after the other two).
## Lessons (batch D: main glaunch pad motion_c map objcoll gamesnd - byte-identical, ~15 min)
- Type traps: a large pointer-sized extern (long long, B8) gives `la` (one register); an array type gives split %hi/%lo (other regs/slots). A `[]` global indexed by a constant keeps its base in a register; a struct field/cast folds into sym+off. When two functions in one TU need different spellings: a BLOCK-SCOPE extern in the earlier function, the file-scope declaration after it (gcc accepts that order).
- Forward declarations: return `int` where callers were compiled against `extern int f()`, and cast returned values.
- mktu bug: conflicting typedefs containing /* */ comments nest in the header comment - escape as `* /`.
- Fast diff: pilot/scripts/fdiff.sh (objdump of build/tu-<f>/obj/tu/<f>.o vs asm/2C8C.s, mnemonic by mnemonic).
- Boundaries moved by batch D: pad.c takes func_00035B50 (row 15); map.c extends to func_0003A270 (290, plan's map2.c); func_0003A45C (291) is probably the head of cars.c. main.c and glaunch.c are probably ONE file (shared string literals interleave in .rodata) - committed as two until Step 4 can prove it.
## Lessons (batch C: sound xmplay debris draw - byte-identical)
- Reusable untracked helpers in build/tu-tools/ (absolute paths, this machine only): unify.py (per-file cfg: maps pilot field names onto canonical types BY OFFSET, makes per-function views `TYPE_<func>` for shapes that don't fit, casts shared symbols, rebuilds the prelude; examples build/tu-debris/cfg.py, build/tu-draw/cfg.py), groups.py (offset table of each conflicting symbol across pilots), gtefix.py (fixes the gte.h #undef trap), protofix.py (calls through `((RET (*)())F)` where a new prototype with short/uchar params changed a call site), cmpall.sh (every function in a TU object vs its pilot compiled alone, ~5 s - finds ALL mismatches, linkcheck only the first).
- REDRIVER2 headers fit D1 records well (CHANNEL_DATA, XMCHANNEL/XMSONG/XMHEADER, SMOKE/DEBRIS/LEAF): map pilot shapes by offset; D1 differences are small (e.g. no kick2 in XMCHANNEL).
- A TU's .sdata/.sbss must be ONE contiguous block: if a file's named globals sit between its unnamed constants, stop the TU there until Step 4 moves the data (debris.c stops before DisplaySplashes; 5 functions left as pilots).
- Casting an extern to another type at the same address can still change register allocation - give the symbol its real record type or use field access.
- draw.c: functions 163-164 (func_0002BDB0, func_0002C29C) link byte-identical as their own block (build/tu-quad/quad.c) - not committed yet, no file name in the plan.
## Lessons (batch F: convert loadview overlay pause handling - byte-identical)
- Type views for incompatible pilot spellings in a big TU: `extern T sym__fn __asm__("sym");` + #define/#undef around that function's section (bodies unchanged). This is SCAFFOLDING for Step 3 (shared headers) - list every view in the file header so Step 3 can remove them. Renamed struct typedefs need views for every extern of that type too.
- Volatile split: if volatile-everywhere grows a function, use `*(volatile int *)&X` only in the readers that need it.
- mktu misses a static inline copy of a function the TU itself defines (redefinition error) - rename the copy `<fn>_inl`.
- Big TUs: `nm -S` sizes vs glabel addresses find the guilty function before fdiff.sh.
- handling.c stops before func_00020AE4 (main wip, 90.22) - 0x20AE4-0x212F0 is left for later.
## Lessons (batch E: overmap director pedest mission job_fx - byte-identical)
- After any size mismatch run fnscore over EVERY function of the TU (linkcheck names only the first).
- Map int-indexed pilot views (`((int *)&X)[k]`) onto real struct fields (add fields if needed) - casts change register allocation / fold offsets.
- Return types: pick what the CALLERS' bytes need (a narrower return makes callers sign-extend).
- Duplicate static inline copies of a function the file also defines -> one GNU89 `inline` definition (same bytes).
- Keep `#include "gte.h"` when rewriting a header by script. The #undef trap also hits macros the merged file defines itself.
