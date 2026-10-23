/* WIP 95.13: control flow, calls, jump table (case 1:18AC, 2:19C4+49C8, 3:19C4+5EE0, 4-6:19C4) and both loops match.
 * Left: in the mode!=0 loop loop.c hoists 0xFFFFFF (8 moves before it, threshold 4*life 58 >= 154 insns) and it wins an
 * s-reg (s3) over 5/0x29; the target keeps the masks local (lui a3/t1 in the body) so 5->fp, 0x29->s7, current->s6,
 * and done->s5 in both loops (mode 0 swaps done/0x29 as a knock-on). Needs one more moved invariant before rect 1's
 * mask or a shorter mask life; tried mask forms, while/do, i++ position, i>2, if(c)/if(z)/if(w) in the helper.
 * (wipB) -dL: 8 moves before the mask (jtbl hi+lo, &col, hi 88, hi 8C, hi current, 5, 0x29) -> threshold 4; the mask
 *   (life 58) is hoisted at 4*58=232 >= 154. A 9th move before insn 448, or a 0xFF000000-first order, would stop it.
 *   Hoisted-but-unallocated is ruled out: mask refs 5 beat 5/0x29 (3 refs) in global-alloc.
 * Rect helper = inline copy of func_001C2014 (its pad[32] gives the 0x40 spill slot); tbl part = func_001C1BD4 inlined. */
typedef struct { unsigned addr:24; unsigned len:8; } P_TAG;
typedef struct { unsigned int tag; unsigned char r0, g0, b0, code; short x0, y0, x1, y1, x2, y2, x3, y3; } PF4;
typedef struct { unsigned int *p; unsigned int **ot; } DB;
typedef struct { unsigned char r, g, b, cd; } CVEC;
typedef struct { short a, b; char c, d; short e; int f; int g; } E_001C1BD4;
extern int D_001CDE88, D_001CDE8C, D_001CDE5C;
extern DB *current;
extern void memset();
extern void SetDispMask();
extern void func_001C11E4();
extern void func_001C18AC();
extern void func_001C19C4();
extern void func_001C49C8();
extern void func_001C5EE0();
extern void func_000E7350();
extern void func_000E6F14();
extern void func_000E6F98();

static inline void func_001C2014(int x, int y, CVEC *c, int w, int h, int z)
{
    char pad[32];
    PF4 *p;
    x += D_001CDE88;
    y += D_001CDE8C;
    p = (PF4 *)current->p;
    ((unsigned char *)p)[3] = 5;
    p->y0 = y;
    p->y1 = y;
    ((unsigned char *)p)[7] = 0x29;
    p->x0 = x;
    p->x1 = x + w;
    p->x2 = x;
    p->y2 = y + h;
    p->x3 = x + w;
    p->y3 = y + h;
    p->r0 = c->r;
    p->g0 = c->g;
    p->b0 = c->b;
    ((P_TAG *)p)->addr = ((P_TAG *)current->ot[z])->addr;
    ((P_TAG *)current->ot[z])->addr = (unsigned)p;
    current->p += 6;
}

static inline void func_001C1BD4(E_001C1BD4 *p)
{
    E_001C1BD4 *q = p;
    func_000E7350();
    while (q->f != 0) {
        func_000E6F14(q->a, q->b, q->c, q->d, &q->g);
        func_000E6F98(q->f);
        q++;
    }
}

void func_001C2114(int unused, E_001C1BD4 *tbl, int mode)
{
    CVEC col;
    int h;
    int done;
    int i;

    memset(&col, 0, 4);
    done = 0;
    if (mode == 0) {
        h = 1;
        do {
            func_001C2014(0, 0, &col, 320, h, 1);
            func_001C2014(0, 512 - h, &col, 320, h, 1);
            h += 48;
            if (h >= 256) done = 1;
            func_001C11E4();
        } while (!done);
        SetDispMask(0);
    } else {
        h = 256;
        i = 0;
        SetDispMask(0);
        do {
            if (i == 2) SetDispMask(1);
            switch (D_001CDE5C) {
            case 1:
                func_001C18AC();
                break;
            case 2:
                func_001C19C4();
                func_001C49C8();
                break;
            case 3:
                func_001C19C4();
                func_001C5EE0();
                break;
            case 4:
            case 5:
            case 6:
                func_001C19C4();
                break;
            }
            func_001C2014(0, 0, &col, 320, h, 1);
            func_001C2014(0, 512 - h, &col, 320, h, 1);
            if (tbl) func_001C1BD4(tbl);
            if (i >= 3) h -= 48;
            i++;
            if (h <= 0) done = 1;
            func_001C11E4();
        } while (!done);
    }
}
