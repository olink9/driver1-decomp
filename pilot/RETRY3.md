# Run 2, final pass - the hardest near-misses (read pilot/AGENT.md and pilot/RETRY2.md first; both apply)

Your batch file is `pilot/batches2/r3-<id>.tsv`. Every symbol has a `src/pilot/<SYMBOL>.wip.c` that at least one Opus
worker already took as far as it could. Its header comment (and pilot/retry-notes.md) says what was tried and what
is left - do not repeat those attempts.

- Up to **15 attempts** per symbol. Diagnose before changing anything: `cc1 -dl -dg` (.lreg/.greg priorities and
  dispositions), `-dL` (loop.c), `--extra="-fno-schedule-insns ..."`, as AGENT.md describes. Name the exact cause.
- Use the tools: `pilot/scripts/hillclimb.py` for statement order, `pilot/scripts/fs.py` (own build dir - never run
  two sweeps of one symbol at once without it), `pilot/scripts/sw_gen.py` for switch trees.
- Read the newest lessons at the END of AGENT.md: many are from this run and were not known when the wip was written.
- `r3-cell`: six functions share one world-to-cell lookup (/1500, /60 and %60 - see retry-notes). Crack it in the
  smallest one first, then port the form to the others. `r3-twins`: two near-identical functions; one fix ports.
- Same rules as RETRY2.md: no linkcheck (the coordinator runs it), never make a wip worse, FAKEMATCH only as a last
  resort and tagged. Results line note: `run2-r3`.

Final reply: `<batch>: matched N / gave up M`, for each give-up the precise remaining cause in one line, and each new
systematic lesson in one line.
