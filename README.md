# Driver (PS1) — matching decompilation

A work-in-progress matching decompilation of **Driver: You Are the Wheelman** (PlayStation, 1999),
US Rev 1, `SLUS_008.42` (sha1 `94b44a84de2f9fa2dcaf120db2e7b3d74bc70d3d`). The goal is C that compiles back
to the identical executable, and then a native PC port on PsyCross, the way REDRIVER2 runs Driver 2.

No game data is in this repository. You supply your own disc.

## Status (2026-10-14)
- `make` relinks a **byte-identical** `SLUS_008.42` (split with splat 0.50.1).
- 799 game functions (398 KB) are queued in `pilot/queue.tsv`. PsyQ libraries start at `__main` 0x738F0,
  and the known ones are named from signatures (`tools/psyqsig.py`).
- Compiler: **GCC 2.7.2-cdk -O2 -G8** + maspsx (aspsx 2.79) + a small maspsx patch. See `pilot/AGENT.md`.
- Pilot: 67 functions exact, 2 in progress (`src/pilot/`). Includes a 1,124-byte function.
- Overlays (`DRIVER/FRONTEND.BIN`, `MCARD.BIN`, `CRED.BIN`, `IVIEW.BIN`, `DT?.BIN`, `NFMV/NFMV.EXE`)
  are not split yet.

## Layout
| | |
|---|---|
| `slus_008.42.yaml` | splat config for the baseline build (asm → `asm/`) |
| `fn.yaml` | same split, one asm file per function (`asm_fn/nonmatchings/main/`), the scoring target |
| `src/pilot/*.c` | matched functions, one file each (`.wip.c` = best attempt so far) |
| `include/common.h` | PsyQ SDK types/macros (PsyCross headers, real PSX layout) |
| `include/gte.h` | PsyQ inline GTE macros |
| `tools/fnscore.py` | compile one candidate, score one function (exact = zero differing instructions) |
| `tools/fnasm.py` | target asm + Driver 2 prototype + REDRIVER2 source for one function |
| `tools/verify.py` | re-score everything, promote/demote, write `pilot/verified.tsv` |
| `tools/nextbatch.py` | next N untried functions |
| `tools/d2match.py` | pair Driver 1 functions with Driver 2 functions by code similarity |
| `tools/psyqsig.py` | name PsyQ library objects from signatures |

## Setup
```sh
distrobox enter build -- tools/setup.sh      # PSX GCCs, maspsx (+patch), splat
# put the exe at orig/SLUS_008.42, then:
distrobox enter build -- sh -c '.venv/bin/python -m splat split fn.yaml && .venv/bin/python -m splat split slus_008.42.yaml && make'
```
For the Driver 2 hints, `tools/fnasm.py` reads `~/dev/redriver2-ref` (OpenDriver2/REDRIVER2),
`~/dev/redriver2-old/symbols/spanish_dump.h` (SoapyMan/REDRIVER2, the archived repo) and `pilot/d2map.tsv`.
To rebuild `pilot/d2map.tsv`, split the US Driver 2 executable (SLUS_011.61) the same way.

## Credits
- **REDRIVER2** (OpenDriver2, MIT): Driver 2 reverse-engineered to C. Its code and the Driver 2 debug
  symbol dump in its archived repository are used as hints and starting points. Any code derived from it
  is MIT-licensed and must keep this attribution.
- **PsyCross** (OpenDriver2, MIT): the PsyQ SDK headers in `include/psyq/` (LICENSE-PsyCross), and the
  planned PC layer.
- **splat**, **spimdisasm**, **maspsx** (mkst), **objdiff**, and **decompals/old-gcc** for the compilers.
- PsyQ signatures: lab313ru/psx_psyq_signatures.
