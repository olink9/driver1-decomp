#!/bin/sh
# Score every src/pilot/*.c (inside distrobox build): one MATCH line per file.
cd "$(dirname "$0")/.."
for f in ${@:-src/pilot/*.c}; do
  s=$(basename "$f" .c); s=${s%.wip}
  printf "%s\t%s\n" "$s" "$(.venv/bin/python tools/fnscore.py "$s" "$f" $FNSCORE_ARGS 2>&1 | head -1)"
done
