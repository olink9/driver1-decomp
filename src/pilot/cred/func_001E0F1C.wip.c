/* (run4-ov) Tried: shade = 0x30; shade -= ty / -(ty-0x30) / 0x30 + -ty / ty = y; ty -= 0x80 (dead addiu never kept);
   x1/x3 as x + 0x50 (addiu from t3), chained x1 = x3 = 0x1AE, literal before x0 (94.51). With a literal the single-set
   0x1AE pseudo gets sched1's birthing boost (li + both stores hoisted next to y2/y3); the target's late li with the stores
   in source order suggests a value that is NOT single-set/birthing. */
/* wip 97.07: logic complete. Left: (1) target keeps a dead 'addiu v0,s1,-0x80' before the 0xB0-y shade calc (combine leftover; tried 0x30-(y-0x80), ty temp, clamp inline, separate ifs - none keep it), (2) 0x1AE (x1/x3) is a late v1 constant in the target; as a join-point local r it gets t4 early, as a literal the x1/x3 stores schedule too early (94.5). x/c/r locals at the if-join reproduce the t2/t3 constants (reorg copies the join insn into the bne delay slot). */
/* Credits: draw face picture n at height y (copy of the template POLY_FT4 D_001E2790[n]) */
typedef struct { unsigned addr:24; unsigned len:8; } P_TAG;
typedef struct { char *primptr; unsigned long *ot; } DB;
typedef struct {
    unsigned long tag;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    unsigned char u0, v0;
    unsigned short clut;
    short x1, y1;
    unsigned char u1, v1;
    unsigned short tpage;
    short x2, y2;
    unsigned char u2, v2;
    unsigned short pad1;
    short x3, y3;
    unsigned char u3, v3;
    unsigned short pad2;
} POLY_FT4;

extern DB *current;
extern POLY_FT4 D_001E2790[];
extern int D_001F3810[];
extern void func_001E0BE8(int x, int y, int face, int shade);
extern void func_001E0B0C(int x, int y);

int func_001E0F1C(int face, int y)
{
    POLY_FT4 *p;
    int ty;
    int shade;
    int x;
    int c;
    int r;

    if (y + 0x48 < 0x30)
        return 0;
    ty = y - 0x80;
    shade = 0x30 - ty;
    if (shade < 0)
        shade = 0;
    else if (shade > 0x28)
        shade = 0x28;
    if (D_001F3810[face] == 9)
        func_001E0BE8(0x15E, y, face, shade);

    x = 0x15E;
    r = 0x1AE;
    c = 0x80;
    p = (POLY_FT4 *)current->primptr;
    *p = D_001E2790[face];
    ((P_TAG *)p)->len = 9;
    p->code = 0x2E;
    p->y3 = p->y2 = y + 0x40;
    p->r0 = c;
    p->g0 = c;
    p->b0 = c;
    p->x0 = x;
    p->y0 = y;
    p->x1 = r;
    p->y1 = y;
    p->x2 = x;
    p->x3 = r;
    ((P_TAG *)p)->addr = ((P_TAG *)&current->ot[3])->addr;
    ((P_TAG *)&current->ot[3])->addr = (unsigned long)p;
    current->primptr += sizeof(POLY_FT4);
    func_001E0B0C(0x15E, y);
    return 1;
}
