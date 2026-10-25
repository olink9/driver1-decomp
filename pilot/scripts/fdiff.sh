#!/bin/bash
# fdiff.sh OBJ FUNC NEXTFUNC : diff mnemonics of a function between obj and asm/2C8C.s
cd "$(dirname "$0")/../.."
T=$(mktemp -d); trap 'rm -rf "$T"' EXIT
distrobox enter build -- mips-linux-gnu-objdump -d -r --no-show-raw-insn "$1" </dev/null | awk -v f="<$2>:" '$2==f{p=1;next} p&&/^[0-9a-f]+ </{exit} p' | grep -v "R_MIPS" | awk -F'\t' 'NF>=2{print $2" "$3}' | sed 's/ *$//' > $T/got.txt
awk -v a="glabel $2" -v b="glabel $3" '$0==a{p=1;next} $0==b{exit} p && /\/\* [0-9A-F]+ [0-9A-F]+ [0-9A-F]+ \*\//' asm/2C8C.s | sed 's/.*\*\/ *//' | awk '{print $1" "$2}' > $T/want.txt
cd $T; diff <(awk "{print \$1}" want.txt) <(awk "{print \$1}" got.txt | sed -e "s/^li$/addiu/" -e "s/^move$/addu/" -e "s/^negu$/subu/") | head -${4:-30}
