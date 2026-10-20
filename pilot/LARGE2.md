# Run 2 - the ten largest functions (read pilot/AGENT.md first; everything there applies)

Your batch file is `pilot/batches/<id>.tsv` (b123..b132): ONE function of 3-6.5 KB, never attempted before.
Follow AGENT.md's loop with these changes:

- Up to **15 attempts**. A function this size is many blocks: get the control flow right first (every branch target
  in the diff lines up), then the data types, then registers and scheduling. Score often.
- Start from the Driver 2 prototype and REDRIVER2 C that `tools/fnasm.py` prints when there is a counterpart, and
  copy D2's debug-symbol locals literally. Many of the lessons at the end of AGENT.md came from functions like these.
- Split work with `static inline` helpers where the target shows a repeated block, as the lessons describe.
- On `MATCH 100.0` write `src/pilot/SYMBOL.c`. Otherwise keep the best attempt as `src/pilot/SYMBOL.wip.c`
  if it scored above 80%. Do NOT run linkcheck.py (shared build folder) - the coordinator link-checks.
- Append to `pilot/results.tsv`: `<UTC>\t<sym>\t<size>\t<best>\t<attempts>\t<matched|gave_up>\trun2-large`.
- Tags as in AGENT.md (FAKEMATCH / HANDWRITTEN ASM), real C preferred.

Scratch files under `build/scratch/<batch id>/`, never in `src/`.
Final reply: `<batch>: <symbol> <best score> <matched|gave up>`, what is left if not matched, each systematic lesson in
one line, and any library function identified with certainty.

If splat marked the function "Handwritten" (check `grep -B2 "nonmatching SYMBOL," asm/2C8C.s`) and it has no normal
prologue / uses add/addi / clobbers unsaved s-regs, it is genuine asm: `pilot/scripts/asmconv.py` converts the listing
into a matching file-scope `__asm__` block in one go (it matched a 1.5 KB one as-is). Tag it `/* HANDWRITTEN ASM */`.
