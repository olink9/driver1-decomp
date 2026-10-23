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
