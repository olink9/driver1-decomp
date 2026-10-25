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
   Claude-Session: https://claude.ai/code/session_0174xTejS3Fw92o4hCdLZBZH

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
## Lessons (batch H: roadbits tile models sky civ_draw + pres/pedest/draw extensions - byte-identical)
- Before merging a low-confidence row run `gpsig.py --range` across BOTH edges: a shared gp .sbss run decides edge functions without trial and error (it moved 4 functions into pres.c, 3 into pedest.c, func_00030198 into draw.c).
- lh vs lhu decides a short's signedness; two needs in one TU -> `__asm__("sym")` view, or `*(short *)&o->field` for a struct field.
- A 4-byte record type on a global the original reaches absolutely makes maspsx use $gp: list it in `/* maspsx: abs=... */`.
- K&R definition settles "a later call passes fewer args" at no byte cost.
- Boundaries: roadbits.c starts at 0x20DB4 (func_00020AE4, 90.22 wip, belongs on top of it once matched). convert.c + sky.c look like ONE D1 file (D2 split it) - kept as two TUs for now.
## Lessons (batch I: players handlers misc_636 flyby scores ladder pathfind felony quad + extensions - byte-identical)
- An edge function that initialises/reads a neighbour's .sbss through $gp belongs to that neighbour.
- Who owns a handler in a pointer table: grep asm/data for `.word func_X` and see whose table it sits in.
- A wip inside a range ends the TU there (linkcheck --tu allows no holes) - say so in the header. pathfind.c stops before func_00062DB0, felony.c before func_00064360; both can be extended once those match.
- D2 functions in file + line order: ~/dev/driver1-work/tu-plan/scripts/d2funcs.json (identified PATHFIND via popNode).
- New files found: pathfind.c, flyby.c (FB fly-by camera paths, took fmvplay.c's tail), scores.c, quad.c (163-164). felony + cop control are one D1 file.
## Lessons (batch G: clearmem civ_ai ai - byte-identical)
- .sbss starts at 0x96FBC (not 0x97000): the plan's first .sbss slice is shifted.
- In this region a gp-relative access marks OWNERSHIP (non-owners use %hi/%lo); ordering each file's first-use addresses proves a range can't be cut.
- Neighbouring arrays are often one array: check addr - stride (D_00098950 = D_00098900[1], D_0009B230 = car_data[1], D_000C4E38 = D_000C4690[1]).
- Big low-agreement TU recipe: per-section `#define NAME NAME__<func>` + `extern T sym__<func> __asm__("sym");` views build first try; then a canonical record from the majority field per offset absorbs most views (generator: build/tu-civ_ai/nsgen2.py + cfg2.py).
- The plan's ai.c is civ_ai.c (1-58, D2 CIV_AI) + ai.c (59-62, D2 AI StoreGameFlags...); cop logic lives in felony.c.
## Overlays (IVIEW CRED NFMV MCARD - all byte-identical; FRONTEND not started)
- Layout: overlay TUs live in `src/<target>/<file>.c` (src/iview/iview.c, src/cred/cred.c, src/nfmv/{main,movie,pad}.c,
  src/mcard/mcard.c). The Makefile links the COMMITTED ones (`git ls-files ':(glob)src/<target>/*.c'`) through
  `linkcheck --target <target> --link` (workdir build/tulink-<target>); main's `src/*.c` glob does not see them.
  Per-function pilots stay in src/pilot/<target>/. Check: `linkcheck --target T --tu src/T/F.c --workdir build/tu-ov-F`;
  whole target: `linkcheck --target T --all --tu $(git ls-files 'src/T/*.c')`. `make overlays` relinks main first
  (main_syms.ld depends on main's ELF): while others merge main, use `make -o build/main_syms.ld overlays`.
- mktu and linkcheck --tu already took `--target`; the only tool change: linkcheck ignores `<fn>.NON_MATCHING` labels.
- WIPS INSIDE A TU: `INCLUDE_ASM("asm_fn/nonmatchings/<target>", fn);` in the function's place (build/tu-tools/wipasm.py
  does it; the TU defines its own INCLUDE_ASM without include_asm.h's `.include labels.inc`, which clashes with the
  macro.inc the pipeline prepends). The C draft stays src/pilot/<target>/fn.wip.c; once it matches, paste it over the
  INCLUDE_ASM line (linkcheck --all skips pilots inside a passing TU). This keeps one file per original file instead of
  cutting a TU at every wip - main's handling.c/roadbits.c could take func_00020AE4 the same way.
- Boundary evidence without gp: (1) a string literal used by two functions puts them in ONE object (gcc 2.7 reuses an
  identical literal only inside one compilation unit) - decided MCARD (one file, shared literals from 0xE7B2C to
  0xEC89C); (2) .rodata/.sdata rising with function order; (3) call clusters: NFMV main.c calls movie.c and pad.c, never
  the reverse, and each owns its data (D_801268A8 / D_80126920) - split into three, medium confidence.
- C++ check for MCARD: no jalr at all (no vtables/function pointers), no new/delete -> C, func_ names kept.
- Conflicts were resolved with per-function `__asm__` views generated by build/tu-tools/views.py (after fixnest.py,
  which repairs mktu's nested /* */ in typedef conflict comments; views.py turns the (0x..) back into comments).
- Same traps as main: #undef of libgpu macros (cred), libgpu.h typedefs duplicated locally next to common.h (cred),
  callers needing a narrower return type (nfmv func_801019C8 -> short), differing static inline copies (mcard
  check_card x3, later ones renamed).
- tools/progress.py counts committed TUs now (see the FRONTEND section).
## Lessons (Step 3 part 1: include/structs.h; 17 TUs switched - byte-identical)
- One header, include/structs.h: CAR_DATA, CIV_SLOT, PURSUER (0x7A8), MODEL (D2 int offsets), CELL_OBJECT, DB,
  PLAYBACKCAMERA, TEXINF/TEXTURE_DETAILS(UV coords), MENU_ITEM, WHEEL, SXY, VECTOR_NOPAD and the gte base types
  (skipped when common.h is included first). OFS_ASSERT/SIZE_ASSERT = negative-array typedefs (works in cc1 2.7).
- cc1 2.7.2 has NO anonymous unions/structs in C: one field name per offset. Other spellings are renames in the
  bodies (pos.vx for x, where.t[k] for px/where[k], damage[k] for f114..) or a cast at the use.
- Free (same bytes, every case tried): renaming fields; scalar `extern T car_data;` -> `car_data[0].f`; int MODEL
  offsets cast at the use `(SVECTOR *)m->vertices`; char[]/int[] __asm__ views of a struct array -> `(char *)SYM`
  casts (felony); a FAKEMATCH's REGION view -> CAR_DATA where.t[] (objcoll); ushort -> short for values that are
  masked (camera angle); struct copy V16 -> VECTOR; 16-bit reads of byte pairs `*(unsigned short *)&t.coords.u0`.
- NOT free: glaunch func_00025C68's CARX view (a short read of the int at 0xF8, char mat[]) grew 12 bytes on field
  access - kept, documented. Workflow per TU: drop the local typedefs, sed the renames, compile errors find the
  rest, linkcheck --tu decides; on a size diff revert only that function's spelling to a local view.
- Field renames must be scoped by variable type (same field names on different records): rename by the object
  expression (`cp->`, `car_data[..].`) or by line range, never globally.
## Lessons (FRONTEND: 23 files, 362 functions, all byte-identical; evidence in each file's head comment)
- OBJECT PADDING is the strongest boundary signal in the overlays: every object's .text and .rodata start 8-aligned,
  so a file that ends at 4 mod 8 leaves ONE zero word. .text: a nop after the final `jr ra` + delay slot (10 in
  FRONTEND - every one was a cut, 3 of them inside ranges the call/data analysis had kept whole). .rodata: a zero
  word no code or .data points at, not followed by a jtbl (jtbls are 8-aligned themselves). linkcheck --tu reports
  the .text one as `+0x4 zero padding`. Main has no such padding (TU-PLAN finding 1), the overlays do: check MCARD/CRED.
- Strings pointed at by INITIALISED .data come first in their file's .rodata (the data is defined at the top), so the
  first .data-referenced string after a pad names the next file's data block (scan the raw .data words for rodata
  and .text addresses - splat leaves most tables as .byte/.short, so `.word func_X` greps miss them).
- Literal duplicates decide whole regions: "MCARD.BIN", DRFONTn, "On"/"Off", "Accept Changes", even "" (each file
  that needs an empty string has its own). Unlabelled strings inside a splat block count too - list every .asciz.
- Overlay pilots keep strings extern (D_ labels), so a TU has no rodata of its own and linkcheck says
  "no data of its own" - the build test then proves only .text. Turning code-only string labels (not pointed at by
  .data, block = one string + padding) into C literals makes it real: ladder.c = 3 literals + 2 jtbls as ONE
  .rodata block at the original address. Script: build/tu-tools/lits.py (re.sub with a lambda - a plain
  replacement string turns \P/\f/\n into control characters; linkcheck catches it as a .rodata diff).
- Menu screens follow one pattern (screen function + handler table + 2-3 tiny handlers per item); each screen with
  its table is its own object, called from the parent menu's handler. Calls between files are everywhere (the
  shared draw/flip/pad helpers in femain/fecore are called by every screen) - zero-crossing gaps need span <= 25.
- Same traps as before: libgpu typedef clash in a TU whose one section includes common.h (cut it out of the
  file or delete the local copies with the same layout), static inline copy of a function the TU defines (_inl).
## Lessons (Step 3 B: handling.c, debris.c onto structs.h - byte-identical)
- Interior symbols into car_data (`sym = car_data + K`): `CARS_AT(sym, K)[i].field` - gcc folds -K+offset and keeps the original reloc symbol. Don't rewrite them as car_data[i].field (changes the hi/lo immediates).
- A walk advancing a record pointer by a SUB-stride (`car + j*16`) must stay a byte-offset record pointer; a real array index changes allocation.
- Per-function compare of a TU against its pre-change object: pilot/scripts/cmpfn.sh FILE.c BASE.o (~5 s; cmpall.sh no longer works once the pilots are gone).
- car_data has at least 25 slots (handling.c loops 25, uses slot 23 as D_0009D438) - the header comment's "20" is wrong.
## Lessons (Step 3 A: civ_ai.c onto structs.h - 50 views -> 0, byte-identical)
- K&R definitions remove "unprototyped callers pass ints to a char parameter" views at no byte cost (even with a struct passed by value).
- To keep the FULL result of a function declared narrower: call through a cast function pointer with the full prototype (still a direct jal); a plain call adds andi.
- Narrow reads from wider fields and `&arr[i].field` for interior symbols are byte-free.
- Let the compiler drive renames: delete the local typedef, parse "no member named" errors, rename only those lines (~200 renames, no mistakes).
- Grep trap: `\bC\b` matches "AI.C" in comments - check one-letter type names by hand.
## Lessons (main leftovers: wips as INCLUDE_ASM, stranded functions, HANDWRITTEN .s modules)
- -G8: cc1 emits function bodies at the end but top-level asm() immediately - tools/fnscore.py place_include_asm() moves INCLUDE_ASM back before the next function's .ent (needed for a wip in the MIDDLE of a main TU).
- Build .s modules from the pilot's MASPSX output, not its __asm__ strings (maspsx adds the delay-slot nops the X-macros suppressed); a short module "passes" if the missing tail is zero - check for "+0x.. zero padding" in the report.
- Unreferenced .sdata words inside a file's slice: define them as plain globals at the right point. Definition placement in the source sets .sdata order (constants come with their function).
- Main's modules don't follow the overlays' 8-byte object-padding rule.
- 6 asm modules: src/{unpack,tile,gte,mdraw,rle,carpoly}_asm.s; the 13 zero words at 0x26760 are the unpacker's scratch, not padding.
## Lessons (Step 3 overlays: frontend 23 files, nfmv, cred onto headers - views -> 0)
- include/frontend.h (MENU_TEXT 16 B labels, MENU_TITLE, TEX, TEXINFO, TP_ENTRY/TP_SHEET, DB_ENV 0x80 with real PSX DRAWENV 0x5C - only when common.h is not included) and include/nfmv.h (DECENV, MOVIE, DRAWENV_PSX).
- Header size checks must use their own macro name per header (`__LINE__`-named asserts collide across headers).
- MENU_ITEM neighbour bytes are left/right/up/down (PSX pad bit order); frontend's up/down/left/right spelling was renamed.
- Free changes: char vs unsigned char (-funsigned-char), int vs void returns when ignored, signedness on stores. NOT free: signedness of a short passed as a call ARGUMENT (sll/sra); some casts change allocation - then retype the global for the whole file.
- `current->b->ot` = `current->ot[2]`; `(DB_ENV *)current` is free.
- Overlay checks compile against the WORKING-TREE structs.h: never leave a broken structs.h edit uncommitted while others build.
- mcard.c (50 views) and iview.c (23 views) not yet on headers.
## Lessons (Step 3: mcard.c, iview.c - views 50+23 -> 0; include/mcard.h)
- `(short)X == 0` on an unsigned short global folds back to lhu; only `*(short *)&X` keeps lh.
- Recipe that worked first time: script-delete all view scaffolding -> swap prelude for header includes -> let compile errors drive renames (scoped per function) -> per-function compare names the few changed functions.
- Overlay cmpfn: build/tu-s3mc/{cc.py,cmpfn.sh,fdiff.sh} (untracked; `env CCT=<target>`).
- DVECTOR should move into structs.h with the gte types (iview keeps a local one).
## Lessons (Step 3 C: mission draw tile sky texture civ_draw quad motion_c flyby players pause pedest + main/glaunch/overmap)
- New shared records: PEDESTRIAN (0x58), MISSION (0x2E8, D_000C6F04, + MS_* sub-records), PERCENTAGE_BAR, MATRIX2. TEXTURE_DETAILS is 14 bytes now (texture_number/page at 0xC/0xD) - overlays include structs.h too: re-check them with --target after any size change.
- Field renames, record-pointer parameter swaps, same-size struct copies, 8-bit bitfields -> plain chars: all byte-free.
- Interior symbols reveal records under other names (D_000C0104 = ped[19], D_000C7034 = mission.ent[0]); field arithmetic (xC = x28 - x0) reveals layout.
- motion_c.c's POLY_* shapes need a main-exe libgpu header (frontend.h has one for overlays only).
- Open: tile.c func_000287A4 reads MODEL+0x24 - D1's MODEL header may be larger than D2's.
- civ_ai.c still declares `extern B_t D_000C6F04` - switch to MISSION (one-liner).
