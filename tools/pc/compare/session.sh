#!/bin/bash
# Live-play lockstep: run sessions from sessions.txt on both sides (headless, silent) and diff them.
#   tools/pc/compare/session.sh BUILD_DIR OUT_DIR [NAME ...]      (no NAME: every session)
# Same environment as run.sh (D1_DATA, EMU_DIR, D1_CUE). With D1_FUNCS=1 the PC side also writes funcs.txt (a
# -finstrument-functions build: function coverage, see coverage.py). Sessions run one after another; the PC side
# of each takes its menu time + ~2.3 vblanks per game frame in real time.
# D1_REF=<dir>: the PC side only, against a reference pack's <dir>/<name>/emu.trace (no emulator; how Windows runs
# it, see port/README.md). PYTHON: the interpreter (default python3).
wpwd() { pwd -W 2> /dev/null || pwd; }   # (Windows: C:/... - the native program can't open /c/...)
here=$(cd "$(dirname "$0")" && wpwd)
build=$(cd "$1" && wpwd)
mkdir -p "$2"
out=$(cd "$2" && wpwd)
shift 2
data=${D1_DATA:-$HOME/dev/driver1-work/pcdata}
emu=${EMU_DIR:-$HOME/dev/driver1-work/emu}
cue=${D1_CUE:-$HOME/dev/driver1-work/driver1.cue}
py=${PYTHON:-python3}
exe="$build/driver1"
[ -f "$build/driver1.exe" ] && exe="$build/driver1.exe"
inbuild() {   # in the Linux build container where there is one
    if command -v distrobox > /dev/null; then distrobox enter build -- bash -lc "$1"; else bash -c "$1"; fi
}
[ -n "$D1_REF" ] || inbuild "gcc -O2 -Wall -o '$emu/lrhost' '$here/lrhost.c' -ldl" || exit 1
rc=0
while IFS='|' read -r name pcpad emupad padf until emuextra pcenv; do
    case "$name" in ''|'#'*) continue;; esac
    if [ $# -gt 0 ] && ! printf '%s\n' "$@" | grep -qx "$name"; then continue; fi
    case "$padf" in drive:*) IFS=: read -r _ len seed <<< "$padf"; padf=$("$py" "$here/padgen.py" "$len" "$seed");; esac
    pcenv=${pcenv//@HERE@/$here}
    emuextra=${emuextra//@HERE@/$here}
    d="$out/$name"
    mkdir -p "$d"
    printf '%s\n' "$padf" > "$d/padf.txt"
    echo "== $name"
    funcs=
    [ -n "$D1_FUNCS" ] && funcs="D1_FUNCS='$d/funcs.txt'"
    run_pc() {
        local nice="nice -n 10" vdrv=offscreen
        # Windows: no offscreen GL there, and MSYS nice drops the Windows environment
        [ -f "$build/driver1.exe" ] && nice= && vdrv=windows
        inbuild "cd '$d' && ALSOFT_DRIVERS=null SDL_AUDIODRIVER=dummy SDL_VIDEODRIVER=$vdrv \
            D1_PAD='$pcpad' D1_PADF='$padf' D1_TRACE_UNTIL=$until D1_QUIT=60000 $pcenv $funcs \
            D1_TRACE='$here/trace.spec:pc.trace' D1_DATA='$data' $nice '$exe'" < /dev/null > "$d/pc.log" 2>&1
    }
    run_emu() {
        if [ -n "$D1_REF" ]; then cp "$D1_REF/$name/emu.trace" "$d/emu.trace"; : > "$d/emu.log"; return; fi
        inbuild "cd '$d' && nice -n 10 '$emu/lrhost' '$emu/swanstation_libretro.so' '$emu/system' '$cue' \
            --frames 200000 --pad '$emupad' --padf '$padf' --until-frame $until $emuextra \
            --trace '$here/trace.spec' emu.trace" < /dev/null > "$d/emu.log" 2>&1
    }
    case "$emuextra" in
    *@R@*)  # the emulator takes the PC's vblank-based pick (lrhost --pursuit-r): PC first
        run_pc
        r=$("$py" -c "import sys,struct; sys.path.insert(0,'$here'); import lockstep; t=lockstep.Trace('$d/pc.trace'); \
r=t.by_label['smalldata']; print(struct.unpack_from('<i', t.rec(0)[2], r.off + 0x96914 - r.addr)[0])")
        emuextra=${emuextra//@R@/$r}
        run_emu & emu_pid=$!;;
    *)  run_emu & emu_pid=$!
        run_pc;;
    esac
    wait $emu_pid
    grep -h "lrhost: [0-9]* vblanks\|D1_TRACE_UNTIL" "$d/emu.log" "$d/pc.log"
    "$py" "$here/lockstep.py" diff "$d/pc.trace" "$d/emu.trace" --pc-exe "$exe" > "$d/diff.txt" || rc=1
    cat "$d/diff.txt"
done < "$here/sessions.txt"
exit $rc
