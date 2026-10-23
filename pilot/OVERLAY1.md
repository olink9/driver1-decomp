# Overlay pass 1 - FRONTEND.BIN / MCARD.BIN (read pilot/AGENT.md first, including its "Overlays" section)

Your batch file is `pilot/batches-ov/<id>.tsv` (`fr-NNN` = frontend, `mc-NNN` = mcard; columns size, addr, symbol).
Write matches to `src/pilot/frontend/SYMBOL.c` or `src/pilot/mcard/SYMBOL.c`; near-misses as `SYMBOL.wip.c` with a
header saying what is left. Score with `tools/fnscore.py SYMBOL file --diff` (it finds the overlay from the symbol).

- Overlays are **-G0**: every global is absolute; declare main's globals with their natural types (`extern int`,
  `short`, `char`, struct) - main's `extern long long` gp trick does NOT apply here. Use main's real names where main
  has them (symbol_addrs.txt; src/pilot/*.c shows types and struct layouts). Driver 2's frontend (FEMAIN.C, REDRIVER2)
  may help with structure, not code.
- Tiny functions (8-40 B, menu callbacks): write them quickly, one file each; batches of 40 should take minutes.
  Look for families (same shape, different constant) and do them in one go.
- Up to 8 attempts per symbol below 300 B, 12 above. Do NOT run linkcheck (the coordinator does). No permuter and no
  sweeps wider than -j2: **the user asked to keep the machine quiet** (fan noise).
- Append one line per symbol to pilot/results.tsv (time, symbol, size, best, attempts, status) with note `ov1`.
- Systematic lessons (things the next worker should know) go in your final reply, one line each.

Final reply: `<batch>: matched N / wip M / gave up K`, one line per wip/give-up with the remaining cause, then lessons.

## Lessons from fr-001..004 (160/160 matched)
- `lui/li/sb|sh|sw %lo(D)` = `extern char|short|int D; D = N;`. Two-offset stores off one base = `extern char D[]; D[0x14]=n; D[0x24]=n;` in asm order.
- A store via a SEPARATE `addiu +0x10` then `sb 0xC(v0)` = a pointer local: `T *p = D; p += 1; p->r = 255;` (index forms fold the offset, ~80%).
- `jal X; v0 = 0` in the slot = `int f(void){ g(); return 0; }`. A trailing `addiu v0,-1` in the jr slot is `return -1` only if v0 is not just the last stored value.
- Generating C from fnasm output with a small script works well for families of identical shapes.
- (fr-005/6) Table flag setters `D_tbl[D_idx].f = 1` = 16-byte struct, `char f` at +4; `if (i != -1) D_tbl[i].f = 0;` with `int i = D_idx;`. Toggles: `D_x ^= 1;` with `int D_x`. C89: declarations only at block start.
- (fr-010..012) `f2(D,0,0)` with `extern char D[]` (array extern = lui/addiu address). Keep a call result across a later call: `int r = g(); f2(...); return r;`. Address-constant tables: `extern int D[]; D[2] = (int)sym;`.
- (fr-013..015) Two pointers over one struct array: `E *q = p; while (q->f) { ...; q++; }` with the real element size (add trailing fields). A store in the first jal's delay slot = the global assigned before the first call. `D += 100; if (D > 0) D = 0;` gives blez. Unused locals that only size the frame count as FAKEMATCH - tag them.
