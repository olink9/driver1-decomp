#!/usr/bin/env python3
"""Pair Driver 1 functions with their Driver 2 counterparts by code similarity.

    tools/d2match.py D2_FN_DIR D2_NAMES_TSV > pilot/d2map.tsv

D2_FN_DIR is a splat per-function split of the US Driver 2 exe (SLUS_011.61);
D2_NAMES_TSV names its functions (aligned from the Driver 2 debug symbol dump in
the archived SoapyMan/REDRIVER2 repo, symbols/spanish_dump.h). Each function
becomes a bag of 3-grams over its instructions with registers stripped
(D2MATCH=ops, the default: opcodes only, 4-grams; D2MATCH=args keeps immediates), compared with weighted Jaccard; pairs are
taken greedily, best first. Score >= 0.6 is a strong hint, 0.4-0.6 a weak one.
"""
import collections, os, re, sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
MODE = os.environ.get("D2MATCH", "ops")
INS = re.compile(r"/\* \w+ [0-9A-F]{8} [0-9A-F]{8} \*/\s+(\S+)\s*(.*)")


def tokens(path):
    out = []
    for line in open(path, errors="replace"):
        m = INS.search(line)
        if not m:
            continue
        op, args = m.group(1), m.group(2)
        if MODE == "ops":                                       # opcodes only: survives struct changes
            out.append(op)
            continue
        args = re.sub(r"\$\w+", "r", args)                      # registers
        args = re.sub(r"\.L[0-9A-F]+|func_[0-9A-F]+|D_[0-9A-F]+|jtbl_[0-9A-F]+", "L", args)
        args = re.sub(r"%(hi|lo|gp_rel)\(L\)", "S", args)
        out.append(op + " " + args)
    return out


def shingles(t):
    k = 4 if MODE == "ops" else 3
    c = collections.Counter(tuple(t[i:i + k]) for i in range(len(t) - k + 1))
    return c


def load(d, names=None):
    out = {}
    for f in os.listdir(d):
        if not f.endswith(".s"):
            continue
        sym = f[:-2]
        t = tokens(os.path.join(d, f))
        if len(t) >= 4:
            out[sym] = (len(t), shingles(t))
    return out


def sim(a, b):
    inter = sum((a & b).values())
    union = sum((a | b).values())
    return inter / union if union else 0.0


def main():
    d2dir, names_tsv = sys.argv[1], sys.argv[2]
    names = {}
    for line in open(names_tsv).read().splitlines()[1:]:
        addr, name = line.split("\t")[:2]
        names["func_%08X" % int(addr, 16)] = name
    d1 = load(os.path.join(ROOT, "asm_fn/nonmatchings/main"))
    d2 = {k: v for k, v in load(d2dir).items() if k in names}
    cands = []
    for k1, (n1, s1) in d1.items():
        for k2, (n2, s2) in d2.items():
            if 0.6 <= n1 / n2 <= 1.67:
                sc = sim(s1, s2)
                if sc >= 0.25:
                    cands.append((sc, k1, k2))
    cands.sort(reverse=True)
    used1, used2 = set(), set()
    print("d1_sym\td1_instrs\td2_name\td2_us_sym\tscore")
    for sc, k1, k2 in cands:
        if sc < 0.25 or k1 in used1 or k2 in used2:
            continue
        used1.add(k1); used2.add(k2)
        print(f"{k1}\t{d1[k1][0]}\t{names[k2]}\t{k2}\t{sc:.3f}")


if __name__ == "__main__":
    main()
