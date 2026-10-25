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
