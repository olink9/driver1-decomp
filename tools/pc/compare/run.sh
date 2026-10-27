#!/bin/bash
# Lockstep comparison, both sides + diff (port/README.md "Lockstep comparison"). Run from the host (not inside
# distrobox); it enters the `build` box for the two runs. Headless and silent.
#
#   tools/pc/compare/run.sh BUILD_DIR OUT_DIR [PC_VBLANKS] [EMU_VBLANKS]
#
# BUILD_DIR  a PC build (cmake -S port -B BUILD_DIR && make -C BUILD_DIR) - it contains driver1
# OUT_DIR    where pc.trace / emu.trace and the logs go (traces are ~19 KB per game frame: ~200 MB for 4 demos)
# Environment: D1_DATA (pcdata, default ~/dev/driver1-work/pcdata), EMU_DIR (default ~/dev/driver1-work/emu:
# lrhost, swanstation_libretro.so, system/scph5501.bin), D1_CUE (default ~/dev/driver1-work/driver1.cue).
# Defaults: 30000 PC vblanks (= 8m20s real time: the PC build runs at 60 Hz; the attract demos start at ~4500) and
# 62000 emulator vblanks (~5 min; the PS1 shows the intro FMVs first, demo 1 starts at ~18300).
set -e
here=$(cd "$(dirname "$0")" && pwd)
build=$(cd "$1" && pwd)
mkdir -p "$2"
out=$(cd "$2" && pwd)
pcv=${3:-30000}
emv=${4:-62000}
data=${D1_DATA:-$HOME/dev/driver1-work/pcdata}
emu=${EMU_DIR:-$HOME/dev/driver1-work/emu}
cue=${D1_CUE:-$HOME/dev/driver1-work/driver1.cue}

distrobox enter build -- bash -lc "gcc -O2 -Wall -o '$emu/lrhost' '$here/lrhost.c' -ldl"
distrobox enter build -- bash -lc "cd '$out' && nice -n 10 '$emu/lrhost' '$emu/swanstation_libretro.so' '$emu/system' \
    '$cue' --frames $emv --trace '$here/trace.spec' emu.trace" > "$out/emu.log" 2>&1 &
emu_pid=$!
distrobox enter build -- bash -lc "cd '$out' && ALSOFT_DRIVERS=null SDL_AUDIODRIVER=dummy SDL_VIDEODRIVER=offscreen \
    D1_PAD= D1_QUIT=$pcv D1_TRACE='$here/trace.spec:pc.trace' D1_DATA='$data' nice -n 10 '$build/driver1'" \
    > "$out/pc.log" 2>&1
wait $emu_pid
tail -1 "$out/emu.log"
python3 "$here/lockstep.py" diff "$out/pc.trace" "$out/emu.trace" --pc-exe "$build/driver1"
