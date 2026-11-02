#!/usr/bin/env bash
# Builds the PC port from the repo plus the files ./driver1.sh setup copied from your disc into orig/.
# Run by driver1.sh inside the build container (port/container/Containerfile), or on the host with
# ./driver1.sh setup --no-container. Output: build-pc/driver1 and play/ (the program, its libraries).
set -euo pipefail
cd "$(dirname "$0")/../.."
VENV="${VENV:-/opt/venv}"
say() { printf '  %s\n' "$*"; }

# 1. Split the game's data out of the original executable and overlays (splat, as the decomp does).
#    The overlays resolve calls into the main executable through build/main_syms.txt. That list is
#    normally made by the PS1 build (tools/mainsyms.py); a copy ships in port/container/ so playing
#    doesn't need the PS1 toolchain.
mkdir -p build
[ -f build/main_syms.txt ] || cp port/container/main_syms.txt build/main_syms.txt
for y in slus_008.42 frontend mcard cred iview; do
  say "splitting $y"
  "$VENV/bin/python" -m splat split "$y.yaml" > "build/split-$y.log" 2>&1 \
    || { echo "splat failed on $y.yaml, see build/split-$y.log" >&2; exit 1; }
done

# 2. Compile the PC program (32-bit, SDL2 + OpenAL, PsyCross).
say "compiling (this takes a few minutes)"
# (the project's default build type: it is what the lockstep comparison and the play tests ran)
cmake -S port -B build-pc > build/cmake.log 2>&1 \
  || { echo "cmake failed, see build/cmake.log" >&2; exit 1; }
make -C build-pc -j"$(nproc)" > build/make.log 2>&1 \
  || { echo "compiling failed, see build/make.log" >&2; exit 1; }

# 3. Put the program next to the two 32-bit libraries most systems lack: SDL2 and OpenAL (both open source).
#    Everything else (C/C++ runtime, X11/Wayland, sound, OpenGL drivers) must be your system's own 32-bit
#    libraries: bundling Ubuntu's copies breaks a newer system's graphics driver. A system that runs Steam has them.
rm -rf play/lib && mkdir -p play/lib
cp build-pc/driver1 play/driver1
for so in libSDL2-2.0.so.0 libopenal.so.1; do
  cp -L "/usr/lib/i386-linux-gnu/$so" play/lib/ 2>/dev/null || cp -L "$(ldd build-pc/driver1 | awk -v n="$so" '$1==n{print $3}')" play/lib/
done
say "built: play/driver1 (+ SDL2 and OpenAL in play/lib)"
