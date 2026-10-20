# Run 2 - functions with no file yet (read pilot/AGENT.md first; everything there applies)

Your batch file is `pilot/batches2/<id>.tsv` (`hw-NN` or `f-NN`). Follow AGENT.md's loop, with these changes:

- Up to **12 attempts** per function. Keep a `.wip.c` if the best attempt scored above 80%.
- `f-NN` batches: run 1 tried these and fell below 80%. Look at them with fresh eyes. Check the control flow and every
  branch target against the asm before touching registers. Look for hidden arguments in callees' prologues, and
  for the Driver 2 counterpart.
- `hw-NN` batches: splat marked these "Handwritten" because they contain GTE (cop2) instructions. Most are C with
  the PsyQ GTE macros in `include/gte.h` (add macros there, copying the exact register sequence). Some are genuine
  hand-written asm: no normal prologue, jumps into another function (`j .Lxxxx` to a label that function owns),
  registers no compiler would pick. Write those as a **file-scope** `__asm__` block with `.set noreorder`, tagged
  `/* HANDWRITTEN ASM */`. Leave out the nop after a `j`/`jal` (maspsx inserts one even under noreorder); a `jr` with
  a real instruction in its delay slot is fine. maspsx also adds a nop after a conditional branch (`bgtz` etc.) that has a real delay-slot
  instruction: hide the branch from it with a one-line gas macro (`.macro XBGTZ r,t; bgtz \\r,\\t; .endm`), as in
  `src/pilot/func_000268B8.c`. Matched examples: `src/pilot/func_0002680C.c`, `func_00030464.c`.
- Do NOT run linkcheck.py (shared build folder); the coordinator link-checks. Results line:
  `<UTC>\t<sym>\t<size>\t<best>\t<attempts>\t<matched|gave_up>\trun2-fresh`.

Scratch files under `build/scratch/<batch id>/`, never in `src/`.
Final reply: `<batch>: matched N / gave up M`, each systematic lesson in one line, any library function identified
with certainty, and which functions you judged to be genuine asm and why.
  hw-NN tip: pilot/scripts/asmconv.py converts a fnasm listing into the file-scope asm form (macro-wraps branches with real delay slots).
