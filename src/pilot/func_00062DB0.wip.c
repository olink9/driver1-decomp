/* run2 r2-041: 80.73 -> run2-r3 r3-15: 83.35. Pass loop is now a real `for (pass...)` (goto loop lost the loop-depth
   weighting): prologue now matches the target's shape (s5 = s4+2, both mult constants in t-regs, s7 saved but unused =
   the hoisted D_000C039C high part allocated s7 then spilled by reload and rematerialised as lui v0).
   Left: loop.c hoists the pass-2 `z0 + FIX(ns*180 - c*900)` chain out of the pass loop (target recomputes it inside
   `if (pass)` from ns/c), which also shifts every t-reg by one. Tried: user-var copies of ns/c (cse propagates them),
   `z = ...` user var (only the last add stays), x1 inline in the if-block (prologue regresses to the old shape), an
   invalid pass loop (jump into it: same regression). 1500 statement-order swaps (perm2.py) found nothing above 83.35. */
typedef struct {
    char pad0[0xC8];
    int x;          /* 0xC8 */
    char pad1[4];
    int z;          /* 0xD0 */
    char pad2[0xF8 - 0xD4];
    int ang;        /* 0xF8 */
    char pad3[0x108 - 0xFC];
    int len;        /* 0x108 */
    char pad4[396 - 0x10C];
} CAR;
typedef struct {
    unsigned char b0;
    unsigned char active;
    char pad[3];
    unsigned char id;
    char pad2[80 - 6];
} SLOT;
typedef struct {
    int pad[2];
    int x;
    int z;
} CAM;
typedef struct { unsigned char c[4]; } CELL;
typedef struct {
    int id;
    int pad[0x13];
    int x;      /* 0x50 */
    int z;      /* 0x54 */
} MAP;

extern long long D_00098950;
extern CAR D_0009B230[];
extern CAM D_0009E714;
extern short D_0008EAA8[];
extern CELL D_000C039C[];

static inline int add750(int v) { return v + 750; }
#define FIX(a) (((a) + 2048) >> 12)

void func_00062DB0(MAP *m)
{
    int i;
    short *tb;
    int pass;
    int d;
    int len;
    int x, z;
    int ns, c;
    int stepx, stepz;
    int x1;
    int z0;
    int idx;
    int bits;
    int pos[2];
    CAR *car;
    SLOT *s;

    for (i = 0; i < 22; i++) {
        car = &D_0009B230[(unsigned char)i];
        s = &((SLOT *)&D_00098950)[(unsigned char)i];
        if (s->active == 0)
            continue;
        if (s->id == m->id + 1)
            continue;
        x = car->x + D_0009E714.x;
        ns = -D_0008EAA8[(car->ang & 0xFFF) * 2];
        c = D_0008EAA8[(car->ang & 0xFFF) * 2 + 1];
        stepx = FIX(ns * 750);
        stepz = FIX(c * 750);
        z0 = add750(D_0009E714.z) - car->z;
        pos[1] = z0 + FIX(c * -900 - ns * 180);
        len = car->len * 20 + 450;
        pos[0] = x + FIX(ns * -900 - c * 180);
        x1 = x + FIX(c * 180 + ns * -900);
        for (pass = 0; pass < 2; pass++) {
            if (pass) {
                pos[0] = x1;
                pos[1] = z0 + FIX(ns * 180 - c * 900);
            }
            for (d = -900; d < len; d += 750) {
                x = (pos[0] + 0x7270E0) / 750 - m->x * 2 - 9936;
                z = (pos[1] + 0x7270E0) / 750 - m->z * 2 - 9936;
                if ((unsigned)x < 128 && (unsigned)z < 128) {
                    idx = (x >> 1) + ((z >> 1) << 6);
                    switch ((z & 1) << 1 | (x & 1)) {
                    case 0:
                    default:
                        bits = 0x22;
                        break;
                    case 1:
                        bits = 0x24;
                        break;
                    case 2:
                        bits = 0x28;
                        break;
                    case 3:
                        bits = 0x30;
                        break;
                    }
                    D_000C039C[idx].c[3] = bits | D_000C039C[idx].c[3];
                }
                pos[0] += stepx;
                pos[1] += stepz;
            }
        }
    }
}
