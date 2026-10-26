#!/usr/bin/env python3
"""Driver 1 PC port: unpack the game disc into the directory the PC build reads (pcdata/).

    python3 port/pc/pc_extract.py DISC.bin OUT_DIR          (or DISC.cue)

DISC is a raw MODE2/2352 image of SLUS-008.42 (one data track, e.g. from a PBP via psxtract/pop-fe, or
a bin/cue rip). Writes the whole ISO9660 tree to OUT_DIR (upper-case names, ';1' dropped) and
OUT_DIR/DISC.LBA, the index port/pc/pc_cd.c serves the game's sector reads from:

    <first LBA> <sectors> <bytes> <sector size> <path>

Ordinary files (Mode 2 Form 1) are stored as their 2048-byte user data, i.e. the plain file.
Interleaved Form 2 files (XA audio *.XA, FMV *.STR: XA attribute 0x2000 set) are stored as raw
2352-byte sectors (sync+header+subheader+data+EDC), because their audio/video lives outside the
2048-byte view; pc_cd.c then reads them with sector size 2352.
"""
import os, struct, sys

def main():
    src, out = sys.argv[1], sys.argv[2]
    if src.lower().endswith(".cue"):
        for l in open(src):
            if l.strip().upper().startswith("FILE"):
                name = l.split('"')[1] if '"' in l else l.split()[1]
                src = os.path.join(os.path.dirname(src), name)
                break
    f = open(src, "rb")
    size = os.path.getsize(src)
    if size % 2352:
        sys.exit(f"{src}: size {size} is not a multiple of 2352 - need a raw MODE2/2352 image")

    def raw(lba):
        f.seek(lba * 2352)
        return f.read(2352)

    def user(lba):
        return raw(lba)[24:24 + 2048]

    pvd = user(16)
    if pvd[1:6] != b"CD001":
        sys.exit("no ISO9660 volume descriptor at sector 16")
    index = []

    def walk(lba, nbytes, rel):
        buf = b"".join(user(lba + i) for i in range((nbytes + 2047) // 2048))
        i = 0
        while i < len(buf):
            n = buf[i]
            if n == 0:                       # records never cross a sector: skip to the next one
                i = (i // 2048 + 1) * 2048
                continue
            r = buf[i:i + n]
            elba, esz, flags, nl = struct.unpack_from("<I", r, 2)[0], struct.unpack_from("<I", r, 10)[0], r[25], r[32]
            name = r[33:33 + nl]
            su = r[33 + nl + ((nl + 1) & 1):]
            xa = struct.unpack(">H", su[4:6])[0] if len(su) >= 8 and su[6:8] == b"XA" else 0
            i += n
            if name in (b"\0", b"\1"):
                continue
            name = name.decode("ascii").split(";")[0].upper()
            path = rel + name
            if flags & 2:
                os.makedirs(os.path.join(out, path), exist_ok=True)
                walk(elba, esz, path + "/")
                continue
            nsec = (esz + 2047) // 2048
            form2 = bool(xa & 0x2000)            # XA attribute: interleaved / Form 2 file
            with open(os.path.join(out, path), "wb") as o:
                if form2:
                    for s in range(nsec):
                        o.write(raw(elba + s))
                else:
                    left = esz
                    for s in range(nsec):
                        d = user(elba + s)
                        o.write(d[:min(2048, left)])
                        left -= 2048
            index.append((elba, nsec, esz, 2352 if form2 else 2048, path))

    os.makedirs(out, exist_ok=True)
    root = pvd[156:156 + 34]
    walk(struct.unpack_from("<I", root, 2)[0], struct.unpack_from("<I", root, 10)[0], "")
    index.sort()
    with open(os.path.join(out, "DISC.LBA"), "w") as o:
        o.write(f"# {os.path.basename(src)}: {size // 2352} sectors. lba sectors bytes sectorsize path\n")
        for e in index:
            o.write("%d %d %d %d %s\n" % e)
    print(f"{len(index)} files -> {out}")

if __name__ == "__main__":
    main()
