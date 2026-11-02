#!/usr/bin/env bash
# Driver 1 PC port: set up from your own disc, then play.
#
#   ./driver1.sh setup <disc.cue|disc.bin|disc.chd>   once: unpack your disc, build the game
#   ./driver1.sh                                       play (fullscreen)
#   ./driver1.sh --window                              play in a window
#
# Optional: everything this does is also described step by step in port/README.md.
# Needs: Linux, python3, and podman or docker (or the build packages, see --no-container).
# Nothing from the game is downloaded or included: it all comes from the disc you point it at.
set -euo pipefail
ROOT="$(cd "$(dirname "$0")" && pwd)"
cd "$ROOT"
IMAGE=driver1-pc-build
SLUS_SHA1=94b44a84de2f9fa2dcaf120db2e7b3d74bc70d3d

die() { printf '\nError: %s\n' "$*" >&2; exit 1; }
say() { printf '%s\n' "$*"; }
sha1() { sha1sum "$1" | cut -d' ' -f1; }

usage() {
  sed -n '2,10p' "$0" | sed 's/^# \{0,1\}//'
  exit "${1:-0}"
}

setup() {
  local disc="" container=1 engine=""
  while [ $# -gt 0 ]; do
    case "$1" in
      --no-container) container=0 ;;
      --engine) engine="${2:-}"; shift ;;
      -h|--help) usage ;;
      -*) die "unknown option $1" ;;
      *) disc="$1" ;;
    esac
    shift
  done
  [ -n "$disc" ] || die "which disc? ./driver1.sh setup <path to your .cue, .bin or .chd>"
  [ -f "$disc" ] || die "$disc: no such file"
  command -v python3 >/dev/null || die "python3 is needed (it unpacks the disc)"

  # 1. The disc image: a raw bin/cue as is; a CHD converted with chdman first.
  case "${disc,,}" in
    *.cue|*.bin) ;;
    *.chd)
      command -v chdman >/dev/null || die "a .chd needs chdman to convert it (package: mame-tools, or mame)"
      say "Converting $disc to bin/cue ..."
      mkdir -p .disc && rm -f .disc/driver1.cue .disc/driver1.bin
      chdman extractcd -i "$disc" -o .disc/driver1.cue -ob .disc/driver1.bin > .disc/chdman.log 2>&1 \
        || die "chdman could not convert it, see .disc/chdman.log"
      disc=.disc/driver1.cue ;;
    *.pbp) die "a .pbp (PSP eboot) has to be converted to bin/cue first, e.g. with psxtract or pop-fe" ;;
    *.iso) die "a .iso only holds 2048-byte sectors, but the game's music and videos need the raw 2352-byte ones. Rip the disc as bin/cue (or convert to .chd)" ;;
    *) die "$disc: expected a .cue, .bin or .chd" ;;
  esac

  # 2. Unpack it and check it is the version this port is built for.
  say "Unpacking the disc into pcdata/ ..."
  rm -rf pcdata.new
  python3 port/pc/pc_extract.py "$disc" pcdata.new > /dev/null || die "unpacking failed (see the message above)"
  [ -f pcdata.new/SLUS_008.42 ] || { rm -rf pcdata.new; die "this is not Driver (US): no SLUS_008.42 on the disc"; }
  if [ "$(sha1 pcdata.new/SLUS_008.42)" != "$SLUS_SHA1" ]; then
    rm -rf pcdata.new
    die "this is a different version of Driver. The port needs the US release, Rev 1 (SLUS-008.42), whose SLUS_008.42 has sha1 $SLUS_SHA1"
  fi
  rm -rf pcdata && mv pcdata.new pcdata

  # 3. The original executable and code overlays, for the build to split the game's data out of.
  mkdir -p orig
  cp pcdata/SLUS_008.42 orig/SLUS_008.42
  local f
  for f in FRONTEND MCARD CRED IVIEW; do
    cp "pcdata/DRIVER/$f.BIN" "orig/$f.BIN"
    [ "$(sha1 "orig/$f.BIN")" = "$(cut -d' ' -f1 "${f,,}.sha1")" ] || die "orig/$f.BIN does not match ${f,,}.sha1"
  done

  # 4. Build, in a container so nothing has to be installed on your system.
  if [ "$container" = 1 ]; then
    if [ -z "$engine" ]; then
      if command -v podman >/dev/null; then engine=podman
      elif command -v docker >/dev/null; then engine=docker
      else die "podman or docker is needed to build (or install the build packages and use --no-container, see port/README.md)"; fi
    fi
    local tag
    tag="$IMAGE:$(sha1 port/container/Containerfile | cut -c1-12)"
    if ! "$engine" image inspect "$tag" > /dev/null 2>&1; then
      say "Preparing the build container (once, downloads Ubuntu and the build tools, ~1 GB) ..."
      "$engine" build -t "$tag" -f port/container/Containerfile port/container > .build-container.log 2>&1 \
        || die "the build container failed, see .build-container.log"
    fi
    say "Building the game ..."
    local user=()
    if [ "$engine" = podman ]; then user=(--userns=keep-id); else user=(-u "$(id -u):$(id -g)"); fi
    "$engine" run --rm "${user[@]}" -v "$ROOT:/src:z" -w /src "$tag" bash port/container/build.sh
  else
    say "Building the game on this system ..."
    [ -x .venv-pc/bin/python ] || {
      python3 -m venv .venv-pc
      .venv-pc/bin/pip install -q $(grep -oE '[A-Za-z0-9_-]+==[0-9.]+' port/container/Containerfile)
    }
    VENV="$ROOT/.venv-pc" bash port/container/build.sh
  fi
  say ""
  say "Done. Play with:  ./driver1.sh    (or ./driver1.sh --window)"
}

play() {
  local fullscreen=1
  while [ $# -gt 0 ]; do
    case "$1" in
      --window) fullscreen=0 ;;
      -h|--help) usage ;;
      *) die "unknown option $1 (setup goes first: ./driver1.sh setup <disc>)" ;;
    esac
    shift
  done
  [ -x play/driver1 ] && [ -d pcdata ] || die "not set up yet: ./driver1.sh setup <path to your disc image>"
  # Saves (card1.mcd), settings (driver1_mods.ini) and the log live in play/.
  cd play
  # play/lib holds 32-bit fallbacks (SDL2, OpenAL and what they use). Use one only where this system has
  # no 32-bit copy of its own: the system's libraries match its graphics driver, the bundled ones may not.
  rm -rf .libs && mkdir .libs
  local have so
  have=$(/sbin/ldconfig -p 2>/dev/null | grep -F '(libc6)' | awk '{print $1}')   # (libc6) = 32-bit entries
  for so in lib/*; do
    grep -qxF "$(basename "$so")" <<< "$have" || ln -s "../$so" ".libs/$(basename "$so")"
  done
  export LD_LIBRARY_PATH="$ROOT/play/.libs${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"
  local missing
  missing=$(ldd ./driver1 2>/dev/null | awk '/not found/{print $1}' | tr '\n' ' ')
  [ -z "$missing" ] || die "your system lacks these 32-bit libraries: $missing
Installing Steam usually brings them; otherwise install your distribution's 32-bit (i386/lib32) packages for them."
  export D1_DATA="$ROOT/pcdata"
  export D1_FULLSCREEN="${D1_FULLSCREEN:-$fullscreen}"
  say "Driver is running. The log is play/driver1.log."
  exec ./driver1 >> driver1.log 2>&1
}

case "${1:-}" in
  setup) shift; setup "$@" ;;
  -h|--help|help) usage ;;
  *) play "$@" ;;
esac
