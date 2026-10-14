#!/bin/sh
# Fetch the toolchain (run inside distrobox `build`): PSX GCCs, maspsx, splat.
# Then put the exe at orig/SLUS_008.42 (sha1 94b44a84de2f9fa2dcaf120db2e7b3d74bc70d3d)
# and run: splat split fn.yaml && splat split slus_008.42.yaml && make
set -e
cd "$(dirname "$0")/.."
command -v mips-linux-gnu-as >/dev/null || sudo apt-get install -y binutils-mips-linux-gnu
[ -d .venv ] || python3 -m venv .venv
.venv/bin/pip install -q "splat64[mips]" spimdisasm pyelftools
[ -d tools/maspsx ] || { git clone --depth 1 https://github.com/mkst/maspsx tools/maspsx && git -C tools/maspsx apply ../maspsx-extern-gp.patch; }
for v in 2.7.2-cdk 2.7.2-psx 2.8.1-psx; do
  [ -x compilers/gcc-$v/cc1 ] && continue
  mkdir -p compilers/gcc-$v
  curl -sL https://github.com/decompals/old-gcc/releases/latest/download/gcc-$v.tar.gz | tar xz -C compilers/gcc-$v
done
