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

# 3. Put the program next to the 32-bit libraries it needs (SDL2, OpenAL and the libraries they use, all open
#    source). ./driver1.sh uses each one only if your system lacks it (a system's own copies always win, so a
#    newer graphics driver never meets an older bundled library). Never bundled: the C library and the
#    OpenGL/GPU driver libraries, which must be your system's own.
rm -rf play/lib && mkdir -p play/lib
cp build-pc/driver1 play/driver1
skip='^(libc|libm|libdl|libpthread|librt|libresolv|ld-linux|libGL|libGLX|libGLdispatch|libEGL|libOpenGL|libdrm|libgbm|libvulkan)\.'
# SDL2 opens its video and audio backends at run time, so ldd on the program alone misses them.
extra=$(ls /usr/lib/i386-linux-gnu/{libX11.so.6,libXext.so.6,libXcursor.so.1,libXi.so.6,libXfixes.so.3,libXrandr.so.2,libXss.so.1,libxkbcommon.so.0,libwayland-client.so.0,libwayland-cursor.so.0,libwayland-egl.so.1,libdecor-0.so.0,libpulse.so.0,libasound.so.2,libdbus-1.so.3} 2>/dev/null || true)
for f in build-pc/driver1 $extra; do
  ldd "$f" 2>/dev/null | awk '/=> \//{print $3}'
  case "$f" in /*) echo "$f";; esac
done | sort -u | while read -r lib; do
  base=$(basename "$lib")
  [[ "$base" =~ $skip ]] && continue
  cp -L "$lib" "play/lib/$base"
done
say "built: play/driver1, with $(ls play/lib | wc -l) fallback libraries in play/lib"
