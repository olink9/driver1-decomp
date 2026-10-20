# Run 2 - retrying near-misses (read pilot/AGENT.md first; everything there applies)

Your batch file is `pilot/batches2/<id>.tsv`: size, addr, symbol, D2 name, D2 score, best score from run 1.
Each symbol already has a near-miss `src/pilot/<SYMBOL>.wip.c` from run 1 (start from it; it is the best
attempt so far). Some have no wip any more: then start fresh as in AGENT.md.

For each symbol:
1. Read `pilot/AGENT.md` (all lessons) and `pilot/retry-notes.md` (systemic fixes already found - several
   near-misses are named there with their fix). Then `tools/fnasm.py SYMBOL` and score the wip with
   `tools/fnscore.py SYMBOL src/pilot/SYMBOL.wip.c --diff` to see what is still different.
2. Up to **12 attempts**. Use the diagnostics in AGENT.md (`--extra="-fno-schedule-insns ..."`, `cc1 -dl -dg`,
   `-dL`) instead of guessing when it is a register or scheduling difference.
3. On `MATCH 100.0`: write the result as `src/pilot/SYMBOL.c` and delete the `.wip.c`. Do NOT run
   linkcheck.py (it shares one build folder; parallel runs break each other) - the coordinator link-checks every
   new match, and a fnscore 100 that fails it goes back to being a wip. So make sure the scorer shows zero
   differing rows, including the `.rodata`/`.sdata` bytes and the function's length.
4. Otherwise keep the better of old and new as `src/pilot/SYMBOL.wip.c` (never make it worse).
5. Append to `pilot/results.tsv` as in AGENT.md, with a `run2` note: `<UTC>\t<sym>\t<size>\t<best>\t<attempts>\t<matched|gave_up>\trun2`.
6. Tags: empty-asm hints -> `/* FAKEMATCH: ... */` at the top; genuine asm -> `/* HANDWRITTEN ASM */`.
   Prefer real C; a FAKEMATCH is only a last resort.

Scratch files: put them under `build/scratch/<batch id>/`, never in `src/`.
Final reply: one line `<batch>: matched N / gave up M` plus each systematic lesson in one line (the coordinator
adds them to AGENT.md) and any library function you identified with certainty.
