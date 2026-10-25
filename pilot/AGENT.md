# Matching loop - instructions for one agent

You are decompiling functions of **Driver** (PS1, 1999, US Rev 1, `SLUS_008.42`) into C that compiles to
**exactly** the original machine code. Working directory: `~/dev/driver1`. Everything runs through the
Ubuntu container: prefix every tool command with `distrobox enter build -- `.

## Your batch
You get a list of symbols from `pilot/queue.tsv` (size, address, symbol, Driver 2 name, similarity).
Work through them in order. For each one:

1. Read the target and its hints: `distrobox enter build -- .venv/bin/python tools/fnasm.py SYMBOL`.
   This prints the asm and the globals it references. When the function has a Driver 2 counterpart,
   it also prints that function's **debug-symbol prototype** (source file, parameter names and registers,
   locals, stack frame) and its **REDRIVER2 C source**. Driver 2 runs on the same engine, so start from
   that code, but Driver 1's version often differs (other constants, fields, extra or missing branches).
2. Write `src/pilot/<SYMBOL>.c`, defining `SYMBOL` with exactly that name (the symbol as
   queue.tsv and the asm spell it: a confirmed name from symbol_addrs.txt, or `func_XXXXXXXX`; don't
   rename it yourself, put a probable Driver 2 name in a comment). Declare every global and function you use with the exact
   name the asm uses (`D_000BE754`, `ClearMem`, `DrawPrim`, ...).
3. Score it: `distrobox enter build -- .venv/bin/python tools/fnscore.py SYMBOL src/pilot/SYMBOL.c --diff`.
   It prints `MATCH <percent>` and the differing rows (`--asm` prints both full listings). 100.0 means
   zero differing instructions. A string literal or jump table may come from your own `.rodata`/`.sdata`;
   the scorer checks its bytes against the original.
4. Stop at `MATCH 100.0`, or after **8 attempts**.
5. Append one line to `pilot/results.tsv`: `<UTC time>\t<symbol>\t<size>\t<best>\t<attempts>\t<matched|gave_up>`.
   On giving up, keep the best attempt as `src/pilot/<SYMBOL>.wip.c` if it scored above 80%; otherwise
   delete it.

## Compiler (confirmed 2026-10-14 on 70 functions)
**GCC 2.7.2-cdk** (`compilers/gcc-2.7.2-cdk`), `-O2 -G8`, then maspsx (`--aspsx-version=2.79
--expand-div -G8`, plus the extern patch below) and GNU as. These are fnscore's defaults; don't change them.
GCC 2.7.2-psx and 2.8.x each failed functions that cdk matches.

## Overlays: FRONTEND.BIN / MCARD.BIN (added 2026-10-23)
Same compiler, but **built -G0** (confirmed on 8 functions: 6 FRONTEND, 2 MCARD; -G8 fails them). fnscore,
linkcheck and permsetup pick this up by themselves: don't pass `-G`.
- Queue: `pilot/queue-frontend.tsv` / `pilot/queue-mcard.tsv` (`tools/nextbatch.py N --target frontend`).
- Write `src/pilot/frontend/<SYMBOL>.c` (MCARD: `src/pilot/mcard/`). `tools/fnasm.py SYMBOL` and
  `tools/fnscore.py SYMBOL src/pilot/frontend/SYMBOL.c --diff` find the overlay from the symbol name
  (or pass `--target frontend`). Log to the same `pilot/results.tsv`.
- **Every global is absolute** (`lui` + `%lo`), including main's `.sdata` variables that main itself reaches
  through `$gp`. Declare globals with their natural type (`extern int D_x;`, `extern short`, `extern char`, arrays
  `extern T D_x[]`); main's `extern long long` 8-byte trick is a -G8 artifact and does NOT apply here.
- References into main use main's spelling (`func_00073F88`, `SetDispMask`, `D_000967F4`); FRONTEND and MCARD call
  each other directly (`func_000E6D60`, `func_001C334C`), declare those as plain externs.
- A raw `(0xE6600 >> 16)` / `(0xE6600 & 0xFFFF)` in the asm is a plain number (a load address or buffer), not a symbol:
  write it as an integer constant, e.g. `(char *)0xE6600`.
- Proof: `tools/linkcheck.py --target frontend` (`--target mcard`) relinks the overlay; final arbiter as for main.
- ~200 FRONTEND functions are 8-40 B menu callbacks (`return 0;`, `D_x = 0;`, `f(); return 0;`): 1 attempt each.

## What the pilot learned
- **Globals come in three addressing forms; declare them to match:**
  - `%gp_rel(D_x)($gp)`: a small extern of ≤4 bytes: `extern int D_x;` / `extern char D_x;`.
  - `lui at / sw %lo(D_x)(at)` or `lui v0 / lw %lo(D_x)(v0)` (assembler macro form, no address reuse):
    an **8-byte** object. Declare `extern long long D_x;` and access it as `*(int *)&D_x`. This is
    empirical: maspsx is patched (`tools/maspsx-extern-gp.patch`, MASPSX_EXTERN_G=4) so that externs
    of 5–8 bytes are not gp-relative.
  - `lui v0 / addiu v0,%lo(D_x)` with the address kept in a register: a big object such as an array or
    struct. Declare `extern T D_x[];` or a struct type larger than 8 bytes.
- Use `abs(x)` (a GCC builtin) instead of an `ABS()` macro or a `?:` expression.
- Struct copies of n words show up as `lw/sw` countdown loops (`bgtz`): use a struct assignment.
- Switches: an explicit `case 0:` that shares the `default` label puts 0 into the jump table.
  Identical case bodies, written out once per case, are still emitted through a jump table.
- Register choice follows source variables: reusing a parameter (`volume = ...`) or doing `p += 4; X = p;`
  versus `X = p + 4` changes which register is used.
- Statement order matters even for independent stores (setRGB0, setRGB2, setRGB1, setRGB3 in ShowLoading).
- GTE code: the PsyQ inline macros are in `include/gte.h` (`gte_SetRotMatrix`, `gte_SetLightMatrix`,
  `gte_SetColorMatrix`). Add new macros there, copying the exact register sequence from the target.
  splat marks such functions "Handwritten", but they are C.
- `break` from inline asm: aspsx encodes `break n` as `n << 6`, so the target's `break 1` is
  `__asm__ volatile("break 1024")`.
- PsyQ types and macros: `#include "common.h"` (PsyCross headers configured for real PSX layouts:
  POLY_*, VECTOR, MATRIX, DRAWENV, setRGB0, setXYWH, ...). Small functions don't need it.
- Library functions not yet named appear as `func_0007xxxx`. If you identify one with certainty, note it
  in your summary (the coordinator adds it to symbol_addrs.txt). Don't edit it yourself.

- (run 1) Store through an unsized array (`extern int D_x[]; D_x[0] = v;`) hoists the `lui` before nearby gp stores.
- (run 1) Loops over a global pointer: index it inside the loop (`D_ptr[i].f`), don't cache it in a local.
- (run 1) Conditional stores: write the store in each branch (`if (c) D = 3; else D = 6;`) for the delay-slot form.
- (run 1) `andi 0xff` on a char global copy: use an `unsigned char` local.

- (run 1) Genuinely hand-written asm (e.g. GTE op in the `jr` delay slot): a file-level
  `__asm__(".set noreorder\n.globl SYM\nSYM: ... jr $31\n<op>\n.set reorder")` is acceptable ONLY when C can't express it.
- (run 1) An `a0` that looks unused may be passed through to a callee - try passing it.
- (run 1) `case 1: case 2: ... return 1;` sharing one label collapses to a compare; separate cases give a jump table.

- (run 1) A register loaded early and never stored is usually an argument to the next `jal`: declare the callee
  `extern void f();` and pass it. Copying fields through locals often fixes load order vs pointer arithmetic.
- (run 1) Probable Driver 2 names you spot: list them in your summary (coordinator keeps pilot/names-proposed.tsv).

- (run 1) File-scope `__asm__` functions MUST start with the comment `/* HANDWRITTEN ASM */` and are only for
  code with no C form (no prologue/`jr ra`, GTE op in a delay slot). They count separately, not as decompiled C.
- (run 1) Dead volatile read: `volatile int D_x;` + `__asm__ volatile("");` before the read keeps its position.
- (run 1) Unused `char pad[N];` local sets the stack frame size. Pad structs to the real stride instead of `a*132+off`.
- (run 1) In shell loops over files, `distrobox enter` eats stdin: use `for x in $(cat f)` not `while read`.

- (run 1) Switch with entry 0 = default: `case 0: default:` first (plain `default:` gives a -1 bias). Pointer-walking
  loops (`p++`) vs `D[i]` indexing is worth trying both. Returning a comparison (`return z <= b;`) gives `slt; xori 1`.

- (run 1) Register swaps: register allocation follows local declaration order - declare separately, reorder.
- (run 1) `bgez; addiu 0xF; sra 4` is signed `/ 16`, not `>> 4`.

- (run 1) Top-tested loop with `j` back: only `loop: if (!v) return; ...; goto loop;` reproduces it.
- (run 1) 9-byte struct copy as lw/lw/lb: `typedef struct { char c[9]; } S __attribute__((aligned(4)));`.

- (run 1) Target has a `nop` in a branch delay slot: an empty `__asm__ volatile("");` in the block stops GCC filling it.
- (run 1) GTE register moves without a handwritten function: `register int t4 __asm__("$12") = x;` + inline `ctc2`.

- (run 1) REDRIVER2's odd local forms (`pmsk = mask; pmsk &= 1 << t; if (pmsk)`) often ARE the original - keep them.
- (run 1) Struct arrays: model the real struct and index it (`p->ch[t]`) instead of manual `+ off` arithmetic.

- (run 1) `static inline` helpers (as in REDRIVER2) reproduce temporaries that a local pointer pushes into s-registers.

- (run 1) Handwritten asm fragments: maspsx adds a `nop` after j/jal/beq/bne/bgez/bgtz. Keep plain `.set noreorder` and
  wrap jumps in a gas macro (`.macro XJ t; j \t; .endm` then `XJ label`). Don't add `.set noat` after it.
- (run 1) Duplicated setup in two branches: write it twice in C; GCC cross-jumps the tails.

- (run 1) **Scratch files are shared between parallel agents** - name every temp file with your batch id
  (e.g. `b031_try.c`), never generic names like `b.c` or `test.c`.
- (run 1) Stack locals: the first-declared local gets the lowest address - reverse declaration order to match.
- (run 1) `unsigned char v = arg;` at the top keeps a separate callee-saved copy (`int` gets copy-propagated away).

- (run 1) Strides from shifts: `i*3 + (i*3<<5)` then `<<2` = 396 (0x18C); `x*3<<4 - x*3` = *45. Derive before trusting D2.
- (run 1) Reassociation (`x + (rand()&0x1ff) - 256` vs `x - 256 + ...`): put the intended sub-expression in its own temp.

- (run 1) Pointer globals in lui form: `extern long long D_x; #define current (*(DB **)&D_x)`.
- (run 1) Recover divisors from magic: find d with ceil(2^(32+s)/d) == M (0x57619F1/sra5 = /1500, 0x88888889/sra5 = /60).
- (run 1) PsyQ macros store fields in their own order - write the stores out by hand when the target order differs.

- (run 1) Plain `char` is UNSIGNED in this GCC; `sll 24; sra 2x` on a value means `signed char`.
- (run 1) A chain of `==1`, `==2` tests on one value may only match as a `switch`.

- (run 1) Old cpp rejects `0x12E-0x24` in array sizes - write the decimal. Gas macros must be one line with `;`.

- (run 1) Keep your BEST attempt: copy it to `<SYMBOL>.best.c` whenever the score improves, and save that as the .wip.c.
- (run 1) One symbol used in two addressing forms: `extern unsigned char Arr[] __asm__("D_x");` alias works.

- (r001, Opus) For pure register-priority swaps, declaration order rarely helps. What works: order of FIRST
  ASSIGNMENT, which variable a sum is stored into, new pseudos (a local pointer), parameter TYPE (`short` param gives
  temp-then-copy), an `int` return type with no return (keeps v0 live, stops epilogue delay-slot filling), and hidden
  extra call arguments. A small brute-force script over source variants (~1 variant/s) is fine for sweeping shapes.
- (b042) Functions called with fewer args than they take need a K&R definition.

- (b047) `andi 0xff`/`0xf0` at call sites: give callees real prototypes with `unsigned char` params (K&R `()` loses it).

- (b048) Small helpers that were already matched (e.g. ClearMem) get inlined: copy the matched body
  as a `static inline` into the caller's file.

- (b052) Delay slots tell the global's form: `long long` form is never moved into a delay slot (target shows `nop`);
  `T D_x[]` form is (target shows `lui` in the slot).

- (b053) **Empty `__asm__("")` / `__asm__("" : "=r"(p) : "0"(p))` hints emit no code but are not real source:**
  allowed as a last resort (same for a second `__asm__("D_x")` alias name used only to defeat CSE), but the file MUST start with `/* FAKEMATCH: ... */` (counted separately; a later pass
  looks for a real C form). Prefer real C.
- (b053) **addPrim: use the real PsyQ bitfield P_TAG** (`typedef struct { unsigned addr:24; unsigned len:8; } P_TAG;`
  and `((P_TAG*)p)->addr = ((P_TAG*)&ot[z])->addr; ((P_TAG*)&ot[z])->addr = (unsigned)p;`). Fixes the
  0xFFFFFF/0xFF000000 mask scheduling.
- (b053) Globals that are fields of a real struct (camera_angle.vx/.vy/.vz) must be declared as structs - aliasing
  rules decide whether loads move past stores.

- (b059) A local copied from an .sdata D_ via lui/addiu + lwl/lwr is an initialized local aggregate
  (`SVECTOR a = {0,0,10};`), not a struct copy of an extern.

- (b066) `D[x-1].f` gets folded into the offset; if the target keeps `addiu -1` before the multiply, compute the
  index in its own statement first.

- (b072) A `goto`-built loop is invisible to loop.c (nothing hoisted) - try it when the target recomputes invariants.

- (b077) GCC 2.7 MEM_IN_STRUCT alias rule: byte/short globals written through `*(T *)&D_x` let loads move above
  the store; declare them as 8-byte structs and write a field (`typedef struct { unsigned char v; char pad[7]; } B8;`).
- (b077) Two copies of the same code with different loop shapes can still be ONE `static inline` called twice.

- (b080) A small buffer whose address the target rebuilds after calls: declare it with its real small size
  (`extern char D_x[8];`). A `for(;;)` left only by `return` gets no invariant hoisting - use `break`.

- (b089) Operand order across a signed divide matters: the `bgez` of the division splits the block, so the operand
  written first is evaluated before it. Scalar gp loads drifting early: declare the global as a 4-byte struct.

- (b094) GCC 2.7 substitutes constant args into inline bodies; if the target keeps that constant in a register
  across a loop, copy the parameter into a local inside the inline (`int step = dstep;`).

- (b093) Diagnostic: `fnscore --extra="-fno-schedule-insns -fno-schedule-insns2"` tells whether an order diff comes from
  the scheduler or from RTL order. `x - (y*48 - 4096)` reassociation: wrap the inner part in a `static inline` sub().

- (b103) Leaf functions have no 0x10 outgoing-arg area: size `char pad[]` from sp+0. Copy Driver 2's debug-symbol
  locals literally (xmin/xmax..., reused tx/tz) - it often fixes allocation.

- (b104) Diagnosing register swaps: run cc1 with `-dl -dg` and compare per-pseudo "used N times across M insns" with
  GCC 2.7 global-alloc priority floor_log2(refs)*refs/live_length - shows which variable needs more/fewer refs.

- (b110) rcossin table: index ONE table (`rcossin_tbl[ang*2+1]`), don't declare D_0008EAAA separately - lets loop.c
  hoist the base. Spilled locals in the target = compute all products first, then compare.

- (b120) `abs()` is ONE insn (abssi2: bgez/move/subu) - no block split, loads move across it; `if (w<0) w=-w` is a real
  branch. `bgez x; nop; negu x` = abs in place. Use Driver 2's per-block locals from the debug symbols.

- (b122) loop.c hoists a load only if threshold*savings*lifetime >= insn_count; duplicate identical case bodies add up
  savings (cross-jumping re-merges them). Check with `cc1 -dL` (.loop dump). Combined bit tests: nest the ifs.

## Final check (coordinator)
`tools/linkcheck.py` links every matched C file into the real build and compares bytes with the original exe - the
real proof. fnscore 100 is necessary, linkcheck pass is what counts. Hand-written asm must also pass it (no stray nop).

## Rules
- Only write in `src/pilot/`, and only append to `pilot/results.tsv` (and `pilot/review.tsv` for near-misses whose
  only diff is a relocation label on byte-identical code: `<symbol>\t<score>\t<why>`). Never edit configs, tools,
  `orig/`, `asm*/` or `include/` (except new macros in `include/gte.h`).
- Don't run `make` or git; the coordinator commits.
- Be efficient: small functions should take 1–3 attempts. Move on rather than burn tokens.
- At the end, reply with one line (matched / gave up / total) plus anything systematic you learned.
- (r2-001) The game-mode byte D_00096898 is the 8-byte form: `lui reg; lbu (same reg)` = `*(unsigned char *)&D_00096898` with `extern long long D_00096898;`.
- (r2-003) A stack local used in a loop gets sp+N hoisted into an s-reg; moving it into a `static inline` helper (caller keeps a `char pad[]`) keeps `addiu a0,sp,N` before each call.
- (r2-003) `addu dst,tmp,base` operand order follows C: `&q[x]`/`ptr+x` put the pointer first; for index-first write `(T *)(x*size + (int)q)`.
- (r2-004) Register swaps between consecutive values are often global-alloc priority TIES (ties go to the lower pseudo): read "used N times across M insns" in -dl/-dg. Computing all sums first, then `x /= T;` in separate statements, changes pseudo order (0006D688).
- (r2-002) `lui v0; lw a1,%lo(D_x)(v0)` (high part in its own reg) = unsized array `extern T *D_x[]; D_x[0]`; the 8-byte form ties the high part to the destination.
- (r2-002) `mfhi t0` on a const divide = reload spilled HI; check the `.greg` dump ("Spilling reg 64") - reorder the divides so no other mult sits inside a high part's span.
- (r2-005) "x = a; if (c) x = b;" vs a real if/else: the if/else's j+barrier changes reorg's delay-slot filling and cse - try it when the target loads a constant fresh or steals an arg into a delay slot.
- (r2-005) A load kept below a store to another global = that store conflicts in GCC 2.7 (unsized `int[]`, volatile, or a loop note); two constant symbol addresses never conflict.
- (r2-006) Reusing one pointer across two loops lengthens its life and swaps allocation (a1/a2): give each loop its own pointer.
- (r2-006) Target re-tests a value already in a register (two bnez on the same reg): put the first `if` in a `static inline` - its return label stops CSE folding the second test.
- (r2-006) For a register swap, copy the form of an already matched sibling first (F3 vs F4 prim builders) before sweeping.
- (r2-007) One loop variable `i` reused for two loops = one pseudo with many refs that outranks the pointers: a separate variable per loop. Same for `if (i == 5) i = -1;` - use a separate result variable.
- (r2-007) 0xFF-terminated records under an outer counter are a nested loop: `for (...) do { c = ...; if (c != 0xFF) {...} } while (c != 0xFF);` (depth weighting raises the constant's priority).
- (r2-007) Identical div/shift math in both branches with a register mismatch: one `static inline` helper called from both.
- (r2-011) Loop pointer at base+field with negative offsets = `T *e = arr; ... e++`, not `arr[i]`. Table entries read by an inline helper: named locals in the caller.
- (r2-011) `slot++` then uses of slot = a separate pseudo; write `slot + 1` at each use. A late `move aN,sX` in the prologue = a parameter declared `unsigned char` - declare it `int`.
- (r2-009) A global loaded early into v0 and never used again is the return value: `x = D_x; ...; return x;` (no volatile/asm hint).
- (r2-009) Loop counter losing the priority race: `arr[k++] = v` in every branch instead of one `k++` at the bottom.
- (r2-008) `*(short *)(p+off) = 0` through a char* keeps later gp loads below the stores; model the object as a real struct (`p->f18 = 0`).
- (r2-008) Mixing a local alias and the global for the same array (`t = D` plus `D[i]`) is what makes the target recompute `t + i*4` twice with flipped operands - write it the same mixed way to match, or all through one local to merge.
- (r2-008) Store order to one struct is reshuffled by the scheduler, so the target's order isn't the source order; try the natural order (x/y before r0/g0/b0).
- (r2-013) Array address as `lui aN; addiu aN,aN,%lo` (same reg) = `extern long long D_x;` cast `(T *)&D_x`; split `lui vX; addiu aN,vX` = unsized `T D_x[]`.
- (r2-013) `sh; lhu` reload of a just-stored field = the field is `unsigned short`. `beqz x; bne x,2` to the same code = `switch` with `case 0: break;`.
- (r2-012) A constant computed after a call when expected before: pass it as an argument to the static inline (substitution changes RTL order). Real local struct sizes come from the slot gaps; a struct at the shared temp base belongs in its own inline.
- (r2-012) Pointer/counter swap with the pointer bumped in the delay slot: write a plain indexed `for` (`D[a][i]`) and let loop.c strength-reduce.
- (r2-010) Loop-counter init in the delay slot of an earlier if/else: a redundant `i = 23;` right after the if/else (legal C). A parameter reused as a temp keeps its a-reg; use a fresh local when the target has a t-reg.
- (r2-010) Missing `move aN,vX` before a call = the original called a wrapper (e.g. seed() -> push()), not the callee directly.
- (r2-015) Array lui/addiu before the index sll when the target does sll first: `T **pp = &D_x[i]; p = *pp; ... *pp = p + 3;`. An extra `addu tN,sp,zero` = a `volatile int *q = &t;`.
- (r2-016) Loop over a global array: start/current pointers from a local `base`, but the bound against the global (`p < &D_x[N]`) so loop.c hoists it and base dies early. Two copies of a counter stepping by 16 and 0x10000 = a plain `for (i...)` with `&D[i]` and a `short` use of i.
- (r2-014) camera_angle D_00096C48 is an 8-byte SVECTOR struct (`extern SVEC D_00096C48;`), not `int[]`. A return constant set mid-function (v0=1 early) can't come from `return 1;` - so far only a FAKEMATCH reproduces it.
- (r2-017) Switch cases sharing a tail, one jumping into the other: write the tail once with a backward goto (`case B: off = x; goto words;`). Conditional fall-through: `case 2: if (c) goto r12; case 0x16: ...; break; r12: return ...;`.
- (r2-017) Return value set in a branch delay slot (`bnez; addu v0,zero,zero`): early returns (`if (a || b) return 0; return expr;`) instead of one `&&`.
- (r2-018) A reload of a global with nothing aliasing in between = `volatile` (spool/callback state). `move aN,x; bgez x; negu aN,x` = `if (x < 0) a = -x; else a = x;`, not abs().
- (r2-020) Prologue loads interleaved as in `-fno-schedule-insns` output = a block boundary in the original; a tiny `static inline` (e.g. `sub2(a,b)`) adds one. Prim words via `*p++` (unsigned long *) keep gp loads below the stores; `prim->field` lets them hoist.
- (r2-020) Chained `&` operand order decides load order (`0x80008000 & q[0] & q[1] ...`) - sweep permutations.
- (r2-021) Constant loads swapped at the top: script every store order (the scheduler keeps ties in RTL order). Base needed before the index math: `b = D; r = &b[w];`. A pointer set in the guard's delay slot: initialise it in the `for (...)` init clause.
- (r2-021) First field of a small global gp-relative, the rest through a pointer: `short *z = &D_x; z[0] = ...; z[1] = ...;` with a plain small extern.
- (r2-019) Switch decision trees: brute-force extra empty `case N: break;` values (0, 1, 3, 0xFF0, 0x7FFF...) on test switches with cc1 - they emit no code but move balance_case_nodes' split; scripts in pilot/scripts/sw_gen.py, sw_brute3.py.
- (r2-019) Two stores sched2 swaps (gp store vs `lui at; sw`): sweep the source position of BOTH statements together.
- (r2-024) v0=0 in two consecutive bnez delay slots falling into `jr ra` = guards joined with one `||` (separate early returns add a `j`). `addiu aN,sp,X` hoisted before a switch = a local pointer passed as an extra argument in one case.
- (r2-023) A diff row that is only a branch/jump target offset = a REAL control-flow bug (wrong label/semantics) - check every branch target of a wip against the asm before tuning registers; two 94% wips had wrong logic.
- (r2-023) Split temporaries reused across distant uses into block-local variables (one long-lived pseudo shifts every t-reg).
- (r2-022) An op repeated in a bgez delay slot and after it: write the remainder `x % N` before the divide (CSE shares it). `- K` computed only on one path: put it inside the condition (`if (... && t - 0x4B1 >= 0x69FU)`). The distance approximation (abs, abs, `dz < dx ? dx + dz/2 : dz + dx/2`) as a `static inline`.
- (r2-025) A global loop bound is hoisted only if the loop's stores are struct-field stores through a pointer (`t->flags |= 4`), not raw `unsigned short *` stores. Increment order in the for clause sets giv init order in reverse - permute. Which loops share a counter variable: sweep the combinations.
- (r2-026) PsyQ prim builders: sweep the order of whole macro calls (setPolyG4, setRGBn, setXYWH, setSemiTrans), not single field stores. Dead stores to stack slots 8 B apart = plain VECTOR locals. Two compare arms jumping to one shared body = `if (c ? x < a : x < b)`.
- (r2-028) A shared `return 0` block placed early = `goto fail;` from each failure check with `fail: return 0;` inside the first guard (`if (a || b) { fail: return 0; }`). `T[i*32+j]` walked by its own pointer = a real 2D array `D_x[][32]`.
- (r2-027) Check the CALLEE's prologue for hidden arguments (a "stray" sum in a1 = a second argument). One `lui` in a delay slot serving both paths of `||` = unsized array form (`extern short D_x[]; D_x[0]`). A `short` param truncated at first use with s0 keeping the full copy = declare the parameter `short` in a prototype.
- (r2-030) A loop counter init placed after the hoisted invariants = a giv: write `320 + j * 128` inside the call. Keep `(&arr[j])->f` / `setRGB0(&arr[j], ...)` when the target shows the hoisted frame base + offset form. `dam = arr; dam += idx;` puts the base straight into the destination s-reg.
- (r2-029) A value recomputed after `if (t<0) t=-t;` = CSE skip-blocks (check `--extra=-fno-cse-skip-blocks`); fix with an if/else storing in both branches. A redundant `bltz` before `slti` in a clamp = a goto into the else branch. `V *cam = &D;` at the declaration hoists the lui; assign it later.
- (r2-032) `lw x; addiu x,-K; addu x,x,rand` = `pos - K` was its own sub-expression: `jit(pos[0]) + (r & 0x3f)` with `static inline int jit(int v){return v-32;}`. A store after a later volatile load with shifted regs: load both into block-local temps first, then store.
- (r2-031) Prim field stores no ordering fixes = PsyQ macros: rebuild the calls (setXYWH from x/w/h patterns, setPolyGT4 + setSemiTrans from code 0x3E) and sweep whole calls. A goto-built loop can fix a pointer register via loop-depth weighting.
- (r2-033) A byte read early and stored to a global after later stores = an `unsigned char c = p[k];` local stored last. In-place remainder after `/60`: `x -= xc*60;` as its own statement. Late param copies with `andi 0xffff/0xff` at each use = `unsigned short`/`unsigned char` parameters.
- (r2-034) `p[1]` / `*(p+1)` are MEM_IN_STRUCT and move above a gp scalar store; to keep the load below, read `*(int *)((char *)p + 4)`. A base copied into an s-reg = the original indexed the global directly (`D_x[0].f`), no `base` local. A compare constant loaded before a divide = the `+= K` was folded into the call argument.
- (r2-036) A compare repeated in each arm before shared divide code = the whole inner if/else written in each branch (cross-jumping merges the tails). A pointer copy used only after `p += hdr`: write `o->f` before the `p += hdr`.
- (r2-035) Base `lui/addiu` before the stride multiply = a real 2D extern `T D_x[][N]` indexed `D_x[a][i]`. A `sll 16; sra 16` on a lhu-loaded local in a subtraction = the source used the local (`e->y - (short)y`).
- (r2-037) `slt vX; move aN,vX` = the result is a `char` local. Out-of-range inner index on one row (`D[0][i + 63*64]`) when the base is `index*4 + K`. Giv `i*size + (base - size)` = a local `cb = car - 1;` before the loop and `&cb[i]` inside.
- (r2-040) `xor/sltu` where the target has beq/bne with constants in delay slots: a `static inline unsigned char side(...)` with early `return 0; return 1;`. Pointer biv in the for clause (`for (i...; i++, r++)`) with every field as `r->f`. Statement-order sweeps (720 variants) take about a minute in one distrobox call - use them early.
- (r2-039) A loop-invariant computed inside the loop = a user variable assigned after an earlier branch in the body (loop.c won't hoist "maybe never set" user vars). An invariant the target hoists but you don't ("savings 1 not desirable" in -dL): write the expression out instead of a variable.
- (r2-038) A guard testing D[0] then a rotated do-while = a plain `while` (loop rotation) - no asm hint. Chained `D_x = obj->f = src;` = one load, stores in source order. fnscore --diff lists rows only after "relaxed check stopped at row N": check --asm for swaps above N.
- (r2-042) A hand-written function = a file-scope `__asm__` block with `.set noreorder` (not asm inside a C body, which adds an epilogue). A hoisted bound copied in the target (`mflo; blez; ... move`): `if (n*n > 0) { i = 0; do {...} while (++i < n*n); }`. Sign-test clamp: `if (v & 0x80000000) goto lo;`.
- (coord) In file-scope HANDWRITTEN asm, leave out the nop after a j/jal: maspsx inserts one even under .set noreorder (a jr with a real delay-slot insn is fine).
- (r2-041) One lw/sw countdown copying 100 B = an explicit `for (i = 25; i > 0; i--) *d++ = *s++;` (struct assignment gives the unrolled movstr). Clamp `bltz` then `slti` = `MIN(10000, MAX(0, v))` in that order. Hoisted invariants follow their first use in the loop.
- (b124) A local initialised from a named rodata label (aligned lw/sw) = a struct copy from an extern (`translate = D_00010C84;`). `a = b = c = v` stores the LAST first - reverse the chain. One loop variable for all sequential loops when the target keeps them in one s-reg.
- (b123) A clamp storing a reloaded value: write the stored value differently (`x8 << 1` vs the compare's `x8*2`) so CSE doesn't reuse it. A default set in a delay slot before an if/else-if chain: write the default first (`d = -10; if ... d = -20; else if ...`).
- (r2-043) Param copied away at entry with a0 reused for a table pointer: declare the table pointer block-local in each branch. `q + K` reassociated: `static inline int sc(int v,int d){return v/d + K;}`. Struct walk with the param as biv: a fresh local `for (q = p; q < end; q++)`.
- (hw-04) In file-scope asm, a j/jal with a REAL delay-slot instruction also needs a gas macro wrapper (XJAL/XJ), or maspsx inserts a nop; plain `jr $ra; nop` is fine.
- (b125) A repeated test whose taken branch goes to a second identical test = jump threading blocked by a USER variable as the operand (`mode = obj->flag; if (mode == 1)`). A sparse switch (0,2,4,6) gets a jump table only with the other values listed as empty cases; entry order follows the order of the case bodies.
- (b127) splat "Handwritten" can be just an inlined gte_SetRotMatrix in real C - check the prologue first. Lists filled through a plain `int *lst` stop loop.c hoisting global loads; indexed `D_x[i*2+j]` lets them hoist. Spill-slot order follows declaration order. `(unsigned)x - 3 < 300` gives sltiu. Exit-test-then-`j loop` = the last exit is `goto next` to a label outside the if/else.
- (b128) Big switches: case bodies are emitted in SOURCE order - sort the cases by the target's code order, not by value (86 -> 93). Number cases from the jump table's .word entries. An 8-byte stack slot stored but never reloaded = a local array (`int *v[2];`).
- (b126) PARALLEL SWEEPS of one symbol race on build/fnscore/c_SYMBOL.o and give wrong scores: run them in sequence, or import fnscore and set `fnscore.B` to your own build dir (example: pilot/scripts/fs.py).
- (b126) Function addresses stored as data: `extern long long func_X;` + `(int)&func_X` gives the rematerialisable la form. A missing spill slot (all later stack offsets off by 4) = one missing local (`lim = 19 - start;`).
- (f-01) `flags & 2` kept as `sltu` (not srl;andi): `b = flags & 2; b = b != 0;`. Setters with early loads and late stores: locals read at the top. Unsolved: `bnez c; addiu c,1` (test the old value, step in the delay slot) - `c++ != 0` folds to `++c != 1`.
- (f-02) Before sweeping: check for an already matched SIBLING (same struct/callee) and port it, and for an already matched helper that is inlined (copy it in as static inline).
- (b131) One block-local `cp`/`st` per sequential loop (as D2's debug info); a function-wide one lands in an s-reg everywhere. An addressable scalar (`&depth`) gets its slot when its address is first used - declare later arrays in a block opened after that call. Two-way `beqz; beq x,1; j skip; body0; body1` = `switch` with cases 0 and 1. `lh` swaps use `int` temps (short gives lhu).
- (f-03) Statement-order problems: pilot/scripts/hillclimb.py (single swaps, ~0.3 s/try with its own build dir) found LongQuaternion2Matrix in ~160 tries. rcossin loads that must stay below gp stores: `*(short *)((int)rcossin_tbl + i*4)`. Globals re-read every pass with no hoisting = a goto-built loop.
- (b132) `if (A || B || C) {X} else Y` around an inlined loop: try the inverse `if (!A && !B && !C) Y; else ...`. `x - 5` then beqz/beq 1 = `switch (x - 5) { case 0: case 1: }`. A reset like `move = 0` belongs at the END of a block when its register is reused as a temp inside.
- (f-04) A delay-slot insn before a jal that "should" come after the call = it was scheduled right before the jal (as swapped it in). Sweep global declaration forms (unsized `T D_x[]` vs `long long`) TOGETHER with statement order. Sweep output: sort numerically on the score (plain sort puts 100.0 below 98).
- (f-05) A value left in a0 across a later jal = a call argument. Several RequestSpool calls in one function need `volatile int spoolcounter` (spoolcounter). pilot/scripts/permute.py permutes lines between `//PERM{` and `//PERM}` markers (several groups) using fs.py.
- (b129) A table base rebuilt with `lui t9/addiu t9` at each use = loop.c hoisted it then it was spilled: put all lookups on ONE table symbol so loop.c merges them. Args set before a branch = local pointers assigned first (`VECTOR *pv = &v;`). `and x,reg(-2)` vs your andi 0xfe = an explicit `& ~1`. Separate variables for products vs results.
- (r3-twins) Target `sll N; srl M` on a value whose shifted copy is already in a register: write the literal `(unsigned)(x << N) >> M` - casts/masks get CSE-folded into andi.
- (b130) A load kept below a store with no alias = cross-jumped duplicate stores: write the stores in both branches. An s-reg swap from a near-tie in priority: add real references (a store per branch). `count = c ? 4 : 12; for (i = count - 1; i != -1; i--)` gives `beqz count` + `addiu -1` in the slot. Sparse 0/1/default switch with a `<2` test: an extra `case 2:` on the default body.
- (r3-01) mfhi/mflo landing in v0 where the target has v1/t0 = reload spilled HI/LO (another mult inside the span) and retried into the first free GPR - check .greg 'Spilling reg 64/65'; the fix is which product wins HI/LO (refs/live length), not declaration order.
- (r3-02) A compare constant CSE'd into a long-lived variable holding it (`odir = 1` then `x == 1`): use a block-local narrower-typed variable (`unsigned char odir;`). Split `lui v0; addiu aN,v0` vs same-reg form depends on whether the address pseudo is local to one block (local = always tied).
- (r3-cell) World-to-cell lookup: `q = v / 60; r = v - q*60 (in place); qz = D_00097374 - qz;` (parity term reassigned INTO the quotient variable), then `(qx & 1) + ((qz & 1) << 1)`. Before any sweep, grep src/pilot for matched files using the same globals and copy their exact text.
- (r3-03) A register swap on the LAST use of a pointer = the dying pointer passes its preference to the load: one temp reused for all the field loads blocks it. In the -dS dump, priority 0x7f000001 = adjust_priority boost; no store order beats it (only a real dependence/volatile) - stop sweeping.
- (r3-04) A load kept below a store with no alias can be a sched2 ANTI-dependence (shared hard reg): fix the register choice by statement order (check `--extra=-fno-schedule-insns2`). `do { } while (0)` wrappers that only shift priorities are FAKEMATCH by our rule (b053) - tag them.
- (r3-05) Target `addiu aN,base,K` with 0(aN) loads in a loop where yours has K(aN): a guarded do-while over a local base pointer. A copy deleted by cse-skip-blocks after `if (v<0) v=~v;`: assign it in both arms. Dead stores used only to steer the compiler count as FAKEMATCH.
- (r3-07) ARRAY_REF loads (`D_x[i]`) are MEM_IN_STRUCT, so a gp scalar store drifts freely past them; loads that must stay on one side of such a store: `*(short *)((int)D_x + i*4)`. `current` loaded early among struct stores: declare current as an 8-byte struct field. Compare -dS dumps with -fno-schedule-insns to tell priority placement from alias dependence.
- (r3-08) `A = B = expr` ties the two copies on priority; write the expression twice (CSE'd) to break a pair swap. A store held late: put the new value in a fresh variable that dies in the next op (sched2 anti-dependence pins it). Clamp testing a copy: `v = MIN(hi, MAX(0, v + K));`.
- (r3-06) Verify every `return` constant against the asm (a branch to the epilogue with v0 already 1 = `return 1`). Check whether a matched sibling is inlined in a function's tail before working on it.
- (r3-09) An s-reg lost to a parameter with equal refs: look for the original reusing that variable later (call results add refs). Pointer stepping (c++, c->f) but the call argument kept indexed (&D_x[i].f) gives two pointers.
- (r3-10) A constant return GCC turns branch-free: put it behind a label other code jumps to (`ret0: return 0;`). A pointer computed before a branch but not loaded: assign it inside the condition (`((t = &x->f)->flags & 8)`).
- (r3-11) Two copies of one `i*size + base` sum (both operand orders) = the original wrote the address two ways; `*(int *)((int)D_x + i*sizeof(T) + off)` keeps CSE from merging. A lui in an earlier guard's delay slot used past a later guard = the assignment sat between two early returns.
- (r3-12) OR in the opposite operand order (`or v0,v1,v0`): split into user variables (`b = v & 1; v = ...; c = v | b;`). Global-alloc priority uses integer division: long-lived vars tie at 0 and go last in pseudo order.
- (r3-13) Biv bumped mid-body but givs at the end = two pointers over the same array. A pseudo kept in an s-reg without crossing a call = used in a loop branch whose identical arms get cross-jumped. `x = a; x -= b;` adds 2 refs to x.
- (r3-15) maspsx crashes on spaced operands in hand-written asm (`sltu $v0, $a2, $a0`): write them without spaces. A delay slot filled from the branch TARGET while an independent insn stays right before the branch = hand-written asm (GCC reorg never does that).
- (r3-14) Use pilot/scripts/permute_dep2.py (def-before-use filter) for order sweeps: plain permutes 'improve' wips with broken semantics. (Matched files are safe: byte-identical code means identical behaviour.) `bp = D_x;` inits are sunk to their use by update_equiv_regs - change refs, not position.
- (r3-16) A subu that sched1 won't put next to a jal: store it in a NEW single-set local (birthing boost). Reverse: args scheduled too late = single-set temps; use one shared multi-set variable. Check ';; N preferences:' in .greg before sweeping declarations.
- (r4-01) A pointer losing a -dS birthing boost (0x7f000001) is set twice (init + loop p++): copy it into a single-set local for its pre-loop use (`b = buf; call(b); for (p = b; ...)`) - global alloc ties both to one reg.
- (r4-03) A loop-invariant flag tested before AND inside a loop loses its reg to short temps (long live range): a copy at loop entry (`int wide = big;`) splits it - FAKEMATCH steering. Permuter found it in ~2000 iterations.
- (r4-03) Global-alloc priority here is floor_log2(refs)*refs/len compared as FLOAT (.1359 vs .1364 decides) - corrects (r3-12); read the `;; N regs to allocate:` order in .greg.
- (r4-04) Near-tie reg swap: move a variable's first set one statement earlier to lengthen its life and drop its priority (`ax = dx; a = ...; if (dx < 0) ax = -dx;` beat if/else abs). A delay slot holding the later independent subtraction = the copy was assigned before it.
- (r4-04) A permuter candidate that "improves" by reusing an unrelated variable as a load temp = the target loads into a separate short-lived reg: fix priorities, don't keep the temp.
- (r4-02) `lui vX; addiu aN,vX,%lo` with the pointer dying in its first block = the variable is reused in a later block (global-alloc keeps one reg for both ranges); labels/inline helpers don't do it. Unused C labels are deleted before flow - they can't split blocks.
- (r4-06) An inline helper's PARAMETER ORDER sets the regs of its inlined loop (ConvRow(src, dst, n) vs (dst, src)) - swap helper params when src/dst regs are swapped inside it. A param losing its a0 preference = a block-0 local got a0 after the param copy: find it in .greg dispositions first.
- (r4-10) A reg diff far from any call (e.g. mfhi into v0 vs t0) can be a WRONG CALLEE RETURN TYPE: `extern void f()` vs `int f()` changes v0 liveness after the call even when the result is discarded. Before sweeping, check each callee's type in matched files: `grep -rhn "func_X *(" src/pilot | grep -v "^.*{"`.
- (coord r4) Swept every remaining wip: flipping each `extern void` callee to int one at a time - only func_0001A30C changed (matched). Done; no need to repeat.
- (r4-05) Prologue `sw sN` too early: the save has an anti-dep on the param->sN copy and sched2 breaks ties by luid - copy the param into a user local AFTER the other entry setup. local-alloc DOUBLES the live length of REG_EQUIV pseudos (params, hoisted const addresses) -> lower global-alloc priority; a user-var copy of a param loses the doubling and steals the s-reg (compare .flow/.lreg "used N times across M insns").
- (r4-07) Shift + base computed before a jal and added after it = two user locals assigned before the call (`ofs = i << 6; base = (char *)tbl; call(); p = base + ofs;`). A gp-relative short that is a field of a small struct (tpage.w/h): declare `typedef struct { short v; } SHORT1;` - stays gp-relative, gets MEM_IN_STRUCT aliasing.
- (r4-07) hillclimb.py can move statements across call arguments and "improve" with broken semantics - keep call-argument setup pinned.
- (r4-11) A chain the target recomputes every pass inside `if (pass)` = a goto-built pass loop (`again: ... if (++pass < 2) goto again;`; loop.c ignores it). A pre-loop temp sharing a name with an inner-loop var becomes one long global pseudo - give it its own name. `bits | mem_byte` operand order needs `unsigned char bits`. Run permute_dep2 over pre-loop statements BEFORE register tuning (85.9 -> 98.4 in one sweep).
- (r4-12) min/max via `MAX(a->x + a->w, b->x + b->w)` (?: macro) gives b-before-a load order, not an if-clamp. A shared merge tail reached by goto can be the body written out in BOTH branches (cross-jumping re-merges; extra refs raise priorities). A branch with `v0 = 0` in its delay slot = its own mid-function `return 0` (nested ifs, not &&). `rg = D; rg += r;` stops CSE recording the address sum.
- (r4-12, tooling) permsetup wipes perm/SYMBOL including outputs - copy good outputs out before re-running it.
- (r4-14) Single-block function: local-alloc ranks by floor_log2(refs)*refs/(death-birth) - a long-lived constant losing its reg is usually the END of a chain; fix the first link (e.g. the order of two table loads), not the constant. Priority dump helper: pilot/scripts/pri.py.
- (r4-15) Hoisted loop invariants get pseudo numbers in order of FIRST USE in the loop body - move the first use to reorder them; break a .greg tie with one extra ref (operand order in `||`). A dead hoisted invariant in the target = an if with identical arms cross-jumped after global-alloc (reproducing it with dead arms is FAKEMATCH).
- (r4-16) r4-02 confirmed: a missing split `lui vX; addiu aN,vX` base = merge a second same-type pointer local from a later block into the earlier one, THEN re-sweep init order (360 orders without the merge could never work).
- (r4-08) Cell lookup: D_0009E714 as `int[]` with `t = D[3] + 750; cz = (t - z) / 1500;` (sibling form). If sched1 always issues one divide first, route the OTHER divide through `static inline int q60(int v){return v/60;}`. Compare register-normalised asm (pilot/scripts/shape.py) to separate schedule from allocation diffs.
- (r4-18) A value set in both if/else arms then masked/shifted after the join = write the whole tail in EACH arm with a block-local temp (cross-jumping re-merges; the extra block end also keeps later loads down). A loop pointer losing its s-reg by a small gap: duplicate `p++; continue;` on an early path (real C, +refs; floor_log2 jumps at 32). Removed two do{}while(0) FAKEMATCH wrappers this way.
- (r4-05b) A field store sched2 lifts too high: make nearby global loads MEM_IN_STRUCT (`typedef struct { unsigned char v; char pad[7]; } B8;` read as `.v`) - replaced a do{}while(0) wrapper. Before tuning priorities on a copied param, try D2's debug-symbol shape LITERALLY (params used directly, block locals, char widths).
- (r4-13) Port the matched sibling's exact giv form first (`D[i]` / `&D[k]` givs instead of explicit pointers fixed 3 reg pairs, 94.8 -> 99.8). Global-alloc length is measured AFTER sched1; a constant-pointer init is a sched1 leaf ordered only by luid among other leaves, so its length moves in steps. Helpers: pilot/scripts/lensweep.py, climb.py (def-use-safe hill climb).
- (r4-13, tooling) Permuter can't parse GTE __asm__: keep the macros as plain calls in base.c and prepend `cat hdr.h "$IN" | gcc -E -P` in perm/SYM/compile.sh (re-patch after every permsetup).
- (r4-19) `s *= s` ties the product into the sum's qty in local-alloc (lower priority) - use fresh single-set locals for products (`distx = dx*dx; distz = dz*dz;`). `bs + (speed + 580)` is reassociated by combine to `(bs+580)+speed`; keep `extra = speed + 580;` as its own statement.
- (coord, run 3) fnscore now accepts NEGATIVE reloc addends (`%hi(D_x-0x20)` vs a same-address splat label); before, the relaxed check stopped there and showed e.g. 99.98 for byte-identical code. All remaining wips rescored - none was only hitting this.
- (r4-09) A second constant divide issued only after the first quotient is complete = a block/loop boundary between them in the original (a loop note reproduces it). An `andi 0xffff` that combine folds in every C form survives only if the def is hidden - untried idea: a `static inline` helper with an `unsigned short` param/return (GCC 2.7 inlining keeps the conversion).
- (r4-06b) local-alloc: an insn ties its output to the first dying input UNLESS the output already has a qty - reassign (`ty = D - ty`) to keep load and result apart; `sel = D - ty` ties them. Params used only in block 0 (loops use separate locals `px = x - tx*60`) get their a0/a1 suggestion first. Read GCC's local-alloc source before sweeping: pilot/gccsrc/local-alloc-2.95.c (close to 2.7; block_alloc, combine_regs, QTY_CMP_PRI).
- (r4-13b) Fast pre-filter for s-reg swaps: run only cc1 and read the hard-reg outcome in .greg (~0.2 s/variant). Duplicated tails only work for plain C - asm volatile (GTE macros) paths are never cross-jumped. A repeated dead store that survives to sched1 lengthens all block-0 lives by one (FAKEMATCH).
- (r4-14b) Before sweeping, check a register target is REACHABLE: a constant with 3 refs can never beat a 4-ref ~9-insn temp in local-alloc - look for an extra ref or a set-twice (global) pseudo instead. Reusing an index for the value it loads (`idx = *p0`) reshuffled a whole single-block function. Permuter `[weight_overrides]` (perm_ins_block = 0 ...) in settings.toml stops do-while/if(1) wrappers.
- (r4-11b) loop.c move_movables lowers its threshold by 3 per moved insn: a target that hoists one invariant chain but recomputes its sibling inside the loop = a REAL loop with both chains in the body, the hoisted one first (no goto needed). A goto loop drops one loop-depth weighting level. `x = q - m->x*2` vs `x = q; x -= m->x*2;` changes which local group gets v0/v1. GCC 2.7.2.3 sources (loop.c, flow.c, global.c, local-alloc.c, jump.c) are in pilot/gccsrc/ - read them for exact heuristics.
- (r4-cell) Refs and live length move together: removing duplicated-arm statements cuts length as much as refs. To flip a global-alloc race add refs that don't extend the range (several refs in the same insns). In a shared inline, sched1 issues the divide with the longer dependent chain first regardless of source order. `andi 0xffff` on a computed index: casts, ushort locals/params/inline helpers, bitfields, explicit & all fold (combine knows nonzero bits) - unsolved.
- (r4-11c) Moving a variable's assignment much EARLIER lengthens its life and lowers its global-alloc priority so a hoisted const address (doubled length) wins. `(a - b) + K` is reassociated; an inline `add750(a) - b` keeps addiu-before-subu. An s-reg saved but never used next to mult code = reload spilled HI; the mfhi regs then come from reload, not local-alloc. A product as its own statement (`dx = c * 180;`) moves neighbouring loads.
- (run4) A constant that must win a local-alloc race against short temps may have had a SECOND set in the original; an empty asm copy (`__asm__("" : "=r"(w) : "0"(w))`) proves it (FAKEMATCH), then look for real C - cse removes re-assignments of an equal constant. Reassigning a variable (`q = D - q`, `x -= ...`) drops the birthing boost for its whole chain; equal priorities go to the HIGHER luid.
