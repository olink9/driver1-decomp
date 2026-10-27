#!/usr/bin/env python3
"""Validate a raw 128 KB PS1 memory card image (.mcd/.mcr) per psx-spx "Memory Card Data Format", independently of
port/pc/pc_mcard.c, and list its files.

usage: pc_mcard_check.py card1.mcd [--dump NAME OUT]   (exit status 1 if anything is wrong)

Checks: size, "MC" header + checksum, write-test frame, every directory / broken-sector frame checksum, allocation
states, block chains (51h first .. 52h middle .. 53h last, next pointers, no block in two files, sizes = blocks *
8 KB), each file's title frame ("SC", icon flag 11h..13h, block count); prints the Shift-JIS title as ASCII."""
import sys

FRAME, BLOCK, SIZE = 0x80, 0x2000, 0x20000


def xor(b):
    x = 0
    for c in b:
        x ^= c
    return x


def sjis_to_ascii(b):
    out, i = [], 0
    while i < len(b) and b[i]:
        c = b[i]
        if c in (0x81, 0x82) and i + 1 < len(b):
            d = b[i + 1]
            i += 2
            if c == 0x81 and d == 0x40: out.append(' ')
            elif c == 0x82 and 0x4F <= d <= 0x58: out.append(chr(d - 0x4F + 48))
            elif c == 0x82 and 0x60 <= d <= 0x79: out.append(chr(d - 0x60 + 65))
            elif c == 0x82 and 0x81 <= d <= 0x9A: out.append(chr(d - 0x81 + 97))
            else: out.append({0x46: ':', 0x5E: '/', 0x5B: '-', 0x7C: '-', 0x44: '.', 0x43: ',', 0x48: '?',
                              0x49: '!', 0x69: '(', 0x6A: ')', 0x95: '&', 0x94: '#', 0x93: '%', 0x5D: '-',
                              0x66: "'", 0x68: '"', 0x51: '_', 0x7B: '+', 0x81: '='}.get(d, '?') if c == 0x81 else '?')
        else:
            out.append(chr(c) if 32 <= c < 127 else '?')
            i += 1
    return ''.join(out)


def check(data):
    errs = []
    if len(data) != SIZE:
        return [f'size {len(data)} != {SIZE}'], []
    fr = lambda blk, f: data[blk * BLOCK + f * FRAME: blk * BLOCK + (f + 1) * FRAME]
    hdr = fr(0, 0)
    if hdr[:2] != b'MC':
        return ['header: no "MC" (unformatted card)'], []
    if xor(hdr[:0x7F]) != hdr[0x7F]: errs.append('header: bad checksum')
    if fr(0, 63) != hdr: print('  note: write-test frame 63 differs from the header frame (allowed)')
    dirs = {}
    for i in range(1, 16):
        d = fr(0, i)
        if xor(d[:0x7F]) != d[0x7F]: errs.append(f'dir {i}: bad checksum')
        st = int.from_bytes(d[0:4], 'little')
        if st not in (0x51, 0x52, 0x53, 0xA0, 0xA1, 0xA2, 0xA3): errs.append(f'dir {i}: bad state {st:#x}')
        dirs[i] = (st, int.from_bytes(d[4:8], 'little'), int.from_bytes(d[8:10], 'little'),
                   d[0x0A:0x1F].split(b'\0')[0].decode('ascii', 'replace'))
    for i in range(16, 36):
        d = fr(0, i)
        if xor(d[:0x7F]) != d[0x7F]: errs.append(f'broken-sector frame {i}: bad checksum')
        if int.from_bytes(d[0:4], 'little') != 0xFFFFFFFF: errs.append(f'broken-sector frame {i}: lists a sector')
    owner, files = {}, []
    for i in range(1, 16):
        st, size, nxt, name = dirs[i]
        if st != 0x51:
            continue
        chain, b = [i], i
        while dirs[b][2] != 0xFFFF:
            b = dirs[b][2] + 1
            if not 1 <= b <= 15 or b in chain:
                errs.append(f'{name}: broken chain at {b}'); break
            chain.append(b)
        for k, b in enumerate(chain):
            want = 0x51 if k == 0 else (0x53 if k == len(chain) - 1 else 0x52)
            if dirs[b][0] != want: errs.append(f'{name}: block {b} state {dirs[b][0]:#x}, want {want:#x}')
            if k and (dirs[b][1] or dirs[b][3]): errs.append(f'{name}: block {b} has size/name (only first may)')
            if b in owner: errs.append(f'block {b} in {owner[b]} and {name}')
            owner[b] = name
        if size != len(chain) * BLOCK: errs.append(f'{name}: size {size:#x} != {len(chain)} blocks')
        t = fr(i, 0)
        if t[:2] != b'SC': errs.append(f'{name}: title frame without "SC"')
        if t[2] not in (0x11, 0x12, 0x13): errs.append(f'{name}: icon flag {t[2]:#x}')
        files.append((name, chain, size, t[3], sjis_to_ascii(t[4:0x44]), b''.join(
            data[b * BLOCK:(b + 1) * BLOCK] for b in chain)))
    for i in range(1, 16):
        if dirs[i][0] in (0x52, 0x53) and i not in owner: errs.append(f'dir {i}: orphan {dirs[i][0]:#x}')
    return errs, files


def main():
    if len(sys.argv) < 2:
        print(__doc__); return 2
    data = open(sys.argv[1], 'rb').read()
    errs, files = check(data)
    used = sum(len(f[1]) for f in files)
    print(f'{sys.argv[1]}: {len(files)} file(s), {used}/15 blocks used')
    for name, chain, size, bc, title, _ in files:
        print(f'  {name:<20} blocks {",".join(map(str, chain)):<10} size {size:#07x} hdr-blocks {bc}  "{title}"')
    if '--dump' in sys.argv:
        k = sys.argv.index('--dump')
        for f in files:
            if f[0] == sys.argv[k + 1]:
                open(sys.argv[k + 2], 'wb').write(f[5])
    for e in errs:
        print('  ERROR:', e)
    print('OK' if not errs else f'{len(errs)} problem(s)')
    return 1 if errs else 0


if __name__ == '__main__':
    sys.exit(main())
