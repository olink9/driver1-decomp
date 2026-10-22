# Run 3 - the last 19 near-misses (read pilot/AGENT.md, pilot/RETRY2.md and pilot/RETRY3.md first; all apply)

Your batch file is `pilot/batches3/r4-<NN>.tsv`: ONE symbol. `src/pilot/<SYMBOL>.wip.c` has been attacked by two
or three Opus workers already. Its header says what was tried and the exact remaining cause - do not repeat it.

## New this run: decomp-permuter (randomised source mutations - the tool for register-allocation leftovers)

    distrobox enter build -- .venv/bin/python tools/permsetup.py SYMBOL [your.c]     # -> perm/SYMBOL/
    distrobox enter build -- timeout 1800 .venv/bin/python -u ~/dev/decomp-permuter/permuter.py perm/SYMBOL \
        -j4 --stop-on-zero > perm/SYMBOL.log 2>&1

- Run it **in the background** (Bash run_in_background) and keep working by hand while it runs. `-j4` exactly:
  four workers share this 16-core machine. Never run two permuters on one symbol.
- Better candidates appear as `perm/SYMBOL/output-<score>-<n>/source.c` (+ diff.txt). Lower = better, 0 = identical
  objdump. Base score is printed at the top of the log. Every ~5-10 min look at the best outputs: the diff of
  source.c vs base.c shows WHICH change helped - that is the real lesson; carry it into your wip by hand.
- You can restart it from an improved source (`permsetup.py SYMBOL better.c` wipes perm/SYMBOL). PERM macros
  (`PERM_GENERAL(a, b)`, `PERM_RANDOMIZE(...)`) restrict/guide it - see ~/dev/decomp-permuter/README.md.
- pycparser cannot parse `__asm__` or some GCC extensions: if permsetup's base.c fails to compile/parse, remove the
  offending construct for the permuter run (a do{}while(0) wrapper is fine) and note it.
- Permuter candidates are often ugly. A match is only accepted if the C is plausible source; any change whose only
  purpose is steering (dead store, `x = x`, pointless casts or temporaries with no reading, `if (1)`, asm hints)
  -> keep it but tag the file `/* FAKEMATCH */` with the reason. Never commit permuter output with garbage names.
- Verify every candidate with fnscore (`tools/fnscore.py SYMBOL file --diff`) - the permuter's compare ignores some
  reloc/rodata differences. fnscore 100.0 is the bar.

## Budget and result

- Up to ~60 minutes of wall time / as many attempts as fit. Never make the wip worse; save an improved wip with an
  updated header (score, what fixed what, what is left) even if it does not match.
- On a match: write `src/pilot/SYMBOL.c` (delete the .wip.c), append to pilot/results.tsv with note `run3-r4`.
  Do NOT run linkcheck (the coordinator does). Do not touch other symbols' files. Scratch only under perm/ or
  build/<your batch id>/.
- Kill your permuter before replying (`pkill -f "[p]ermuter.py perm/SYMBOL"` - the bracket keeps pkill from killing its own shell).

Final reply: `<batch> SYMBOL: matched | best NN.NN`, the precise remaining cause in one line, the permuter's useful
finding (if any) in one line, and each new systematic lesson in one line.
