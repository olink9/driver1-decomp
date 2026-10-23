typedef struct { unsigned addr:24; unsigned len:8; } P_TAG;
typedef struct {
    P_TAG tag;
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
typedef struct {
    P_TAG tag;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    short x1, y1;
    short x2, y2;
    short x3, y3;
} POLY_F4;
typedef struct { char *primptr; P_TAG *ot; char *f8; int id; char pad[0x70]; } DB;
typedef struct { char name[20]; int attr; int size; void *next; int head; char system[4]; } DIRENTRY;
typedef struct {
    unsigned char u0, v0, u1, v1, u2, v2, u3, v3;
    unsigned short tpage;
    unsigned short clut;
    unsigned char idx, slot;
} OUT;

extern DB *current;
extern DB *D_00097524;
extern DB D_000BDFA4[];
extern POLY_FT4 D_000EF8D0[4];
extern POLY_F4 D_000EF970[2];
extern int D_000ED7D0;
extern int D_000EE920;
extern short D_0009752C;
extern short D_00097538;
extern char D_000E66FC[];
extern char D_000E6C44[], D_000E6C4C[], D_000E6C54[], D_000E6C6C[], D_000E6C88[], D_000E6CB4[], D_000E6CDC[];
extern char D_000E6CFC[], D_000E6D2C[];
extern void func_000EC280();
extern void func_00051374();
extern void func_000EC6BC();
extern int func_0007A7F4(); /* GetClut */
extern void func_000514E0();
extern void SetDispMask();
extern void func_0005C8F4();
extern void func_000E90D8();
extern void func_00077E48();
extern void func_00077ED8();
extern void func_00077E98();
extern void func_000E8DD4();
extern void VSync();
extern int func_00078E4C();
extern int func_00078BD8();

/* Byte-identical to the target: fnscore shows 99.88 only because splat ends the function before the final
   jr ra delay slot (that nop word is D_000ED528 in .data). */

/* boot-time memory card check: "checking cards" screen, then warns when a card has no free blocks */
void func_000EC89C(void)
{
    DIRENTRY dir[15];
    OUT tex;
    int count[2];
    int free[2];
    int r[2];
    int ok;
    int res;
    int frame;
    int n;
    int i;
    int q;
    int ret;
    int fr;
    POLY_FT4 *p;
    POLY_FT4 *p2;
    POLY_F4 *f;
    POLY_F4 *f1;

    ok = 0;
    res = 0;
    D_000ED7D0 = 1;
    func_000EC280(0);
    D_000BDFA4[0].primptr = D_000BDFA4[0].f8 = (char *)0xD8600;
    current = D_000BDFA4;
    D_000BDFA4[1].primptr = D_000BDFA4[1].f8 = (char *)0xF6600;
    D_000BDFA4[0].ot = (P_TAG *)0xD0200;
    D_000BDFA4[1].ot = (P_TAG *)0xD4400;
    D_00097524 = D_000BDFA4 + 1;
    func_00051374();
    func_000EC6BC(D_000E6C44, &tex);
    p = &D_000EF8D0[0];
    p2 = p + 2;
    ((P_TAG *)p)->len = 9;
    p->code = 0x2C;
    p->r0 = 0x80;
    p->g0 = 0x80;
    p->b0 = 0x80;
    p->code &= ~2;
    p->x0 = 0;
    p->y0 = 0;
    p->x1 = 0x100;
    p->y1 = 0;
    p->x2 = 0;
    p->y2 = tex.v2 - tex.v0 + 1;
    p->x3 = 0x100;
    p->y3 = tex.v2 - tex.v0 + 1;
    p->tpage = tex.tpage;
    p->clut = func_0007A7F4(0x3B0, 0x1FF);
    p->u0 = tex.u0;
    p->v0 = tex.v0;
    p->u1 = tex.u1;
    p->v1 = tex.v1;
    p->u2 = tex.u2;
    p->v2 = tex.v2;
    p->u3 = tex.u3;
    p->v3 = tex.v3;
    func_000EC6BC(D_000E6C4C, &tex);
    ((P_TAG *)p2)->len = 9;
    p2->code = 0x2C;
    p2->r0 = 0x80;
    p2->g0 = 0x80;
    p2->b0 = 0x80;
    p2->code &= ~2;
    p2->x0 = 0x100;
    p2->y0 = 0;
    p2->x1 = 0x200;
    p2->y1 = 0;
    p2->x2 = 0x100;
    p2->y2 = tex.v2 - tex.v0 + 1;
    p2->x3 = 0x200;
    p2->y3 = tex.v2 - tex.v0 + 1;
    D_000EF8D0[2].tpage = tex.tpage;
    p2->clut = func_0007A7F4(0x3A0, 0x1FF);
    p2->u0 = tex.u0;
    p2->v0 = tex.v0;
    p2->u1 = tex.u1;
    p2->v1 = tex.v1;
    p2->u2 = tex.u2;
    p2->v2 = tex.v2;
    p2->u3 = tex.u3;
    p2->v3 = tex.v3;
    f = &D_000EF970[0];
    f1 = &D_000EF970[1];
    ((P_TAG *)f)->len = 5;
    f->code = 0x28;
    f->r0 = 0;
    f->g0 = 0;
    f->b0 = 0;
    f->code &= ~2;
    f->x0 = 0;
    f->y0 = 0;
    f->x1 = 0x200;
    f->y1 = 0;
    f->x2 = 0;
    f->y2 = 0x19;
    f->x3 = 0x200;
    f->y3 = 0x19;
    ((P_TAG *)f1)->len = 5;
    f1->code = 0x28;
    f1->r0 = 0;
    f1->g0 = 0;
    f1->b0 = 0;
    f1->code &= ~2;
    f1->x0 = 0;
    f1->y0 = 0xD8;
    f1->x1 = 0x200;
    f1->y1 = 0xD8;
    f1->x2 = 0;
    f1->y2 = 0x100;
    f1->x3 = 0x200;
    f1->y3 = 0x100;
    frame = 0;
    do {
        func_000514E0();
        if (frame == 2)
            SetDispMask(1);
        ((P_TAG *)&D_000EF8D0[0])->addr = current->ot[12].addr;
        current->ot[12].addr = (unsigned)&D_000EF8D0[0];
        ((P_TAG *)&D_000EF8D0[2])->addr = current->ot[12].addr;
        current->ot[12].addr = (unsigned)&D_000EF8D0[2];
        func_0005C8F4(D_000E6C54, 0x78);
        frame++;
        func_000E90D8();
    } while (frame < 52);
    n = 999;
    func_00077E48();
    func_00077ED8(0);
    func_000E8DD4(0);
    r[0] = 0;
    r[1] = 0;
    do {
        VSync(0);
        if (func_00078E4C(0, &r[0], &r[1]))
            break;
    } while (--n != -1);
    free[0] = free[1] = 15;
    if ((D_000EE920 = func_00078BD8(0, D_000E66FC, dir, &count[0], 0, 15)) == 0) {
        for (i = 0; i < count[0]; i++) {
            q = dir[i].size / 8192;
            fr = free[0];
            if (dir[i].size % 8192)
                free[0] = fr - (q + 1);
            else
                free[0] = fr - q;
        }
        ok++;
    }
    func_00077ED8(0x10);
    func_000E8DD4(1);
    n = 999;
    r[0] = 0;
    r[1] = 0;
    do {
        VSync(0);
        if (func_00078E4C(0, &r[0], &r[1]))
            break;
    } while (--n != -1);
    D_000EE920 = -1;
    do {
        D_000EE920 = ret = func_00078BD8(0x10, D_000E66FC, dir, &count[1], 0, 15);
    } while (ret == -1);
    if (ret == 0) {
        for (i = 0; i < count[1]; i++) {
            q = dir[i].size / 8192;
            fr = free[1];
            if (dir[i].size % 8192)
                free[1] = fr - (q + 1);
            else
                free[1] = fr - q;
        }
        ok++;
    }
    if (ok == 1) {
        if (free[0] == 0 || free[1] == 0) {
            while (D_0009752C == 0) {
                func_000514E0();
                if (frame == 2)
                    SetDispMask(1);
                ((P_TAG *)&D_000EF8D0[current->id])->addr = current->ot[12].addr;
                current->ot[12].addr = (unsigned)&D_000EF8D0[current->id];
                ((P_TAG *)&D_000EF8D0[current->id + 2])->addr = current->ot[12].addr;
                current->ot[12].addr = (unsigned)&D_000EF8D0[current->id + 2];
                func_0005C8F4(D_000E6C6C, 0x46);
                func_0005C8F4(D_000E6C88, 0x64);
                func_0005C8F4(D_000E6CB4, 0x73);
                func_0005C8F4(D_000E6CDC, 0xC8);
                frame++;
                func_000E90D8();
            }
            res = 1;
        }
    } else if (ok == 2) {
        if (free[0] == 0 && free[1] == 0) {
            while (D_00097538 == 0) {
                func_000514E0();
                if (frame == 2)
                    SetDispMask(1);
                ((P_TAG *)&D_000EF8D0[current->id])->addr = current->ot[12].addr;
                current->ot[12].addr = (unsigned)&D_000EF8D0[current->id];
                ((P_TAG *)&D_000EF8D0[current->id + 2])->addr = current->ot[12].addr;
                current->ot[12].addr = (unsigned)&D_000EF8D0[current->id + 2];
                func_0005C8F4(D_000E6CFC, 0x46);
                func_0005C8F4(D_000E6D2C, 0x56);
                func_0005C8F4(D_000E6C88, 0x6E);
                func_0005C8F4(D_000E6CB4, 0x7D);
                func_0005C8F4(D_000E6CDC, 0xC8);
                frame++;
                func_000E90D8();
            }
            res = 1;
        }
    }
    if (res == 1) {
        for (i = 0; i < 0xE8; i += 8) {
            if (ok == 1) {
                ((P_TAG *)&D_000EF8D0[current->id])->addr = current->ot[12].addr;
                current->ot[12].addr = (unsigned)&D_000EF8D0[current->id];
                ((P_TAG *)&D_000EF8D0[current->id + 2])->addr = current->ot[12].addr;
                current->ot[12].addr = (unsigned)&D_000EF8D0[current->id + 2];
                ((P_TAG *)&D_000EF970[0])->addr = current->ot[0].addr;
                current->ot[0].addr = (unsigned)&D_000EF970[0];
                ((P_TAG *)&D_000EF970[1])->addr = current->ot[0].addr;
                current->ot[0].addr = (unsigned)&D_000EF970[1];
                func_0005C8F4(D_000E6C6C, 0x46);
            } else {
                ((P_TAG *)&D_000EF8D0[current->id])->addr = current->ot[12].addr;
                current->ot[12].addr = (unsigned)&D_000EF8D0[current->id];
                ((P_TAG *)&D_000EF8D0[current->id + 2])->addr = current->ot[12].addr;
                current->ot[12].addr = (unsigned)&D_000EF8D0[current->id + 2];
                ((P_TAG *)&D_000EF970[0])->addr = current->ot[0].addr;
                current->ot[0].addr = (unsigned)&D_000EF970[0];
                ((P_TAG *)&D_000EF970[1])->addr = current->ot[0].addr;
                current->ot[0].addr = (unsigned)&D_000EF970[1];
                func_0005C8F4(D_000E6C6C, 0x46);
                func_0005C8F4(D_000E6CFC, 0x56);
            }
            func_0005C8F4(D_000E6C88, 0x64);
            func_0005C8F4(D_000E6CB4, 0x73);
            func_0005C8F4(D_000E6CDC, 0xC8);
            func_000E90D8();
            D_000EF970[0].x0 = 0;
            D_000EF970[0].y0 = 0;
            D_000EF970[0].x1 = 0x200;
            D_000EF970[0].y1 = 0;
            D_000EF970[0].x2 = 0;
            D_000EF970[0].y2 = i + 0x19;
            D_000EF970[0].x3 = 0x200;
            D_000EF970[0].y3 = i + 0x19;
            (D_000EF970 + 1)->x0 = 0;
            (D_000EF970 + 1)->y0 = 0xD8 - i;
            (D_000EF970 + 1)->x1 = 0x200;
            (D_000EF970 + 1)->y1 = 0xD8 - i;
            (D_000EF970 + 1)->x2 = 0;
            (D_000EF970 + 1)->y2 = 0x100;
            (D_000EF970 + 1)->x3 = 0x200;
            (D_000EF970 + 1)->y3 = 0x100;
        }
    }
    SetDispMask(0);
    func_00077E98();
}

