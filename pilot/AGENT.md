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
2. Write `src/pilot/<SYMBOL>.c`, defining `SYMBOL` with exactly that name (keep the `func_XXXXXXXX`
   name; put the Driver 2 name in a comment). Declare every global and function you use with the exact
   name the asm uses (`D_000BE754`, `func_00012498`, `DrawPrim`, ...).
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

- (b048) Small helpers that were already matched (e.g. ClearMem func_00012498) get inlined: copy the matched body
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

- (b110) rcossin table: index ONE table (`D_0008EAA8[ang*2+1]`), don't declare D_0008EAAA separately - lets loop.c
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
