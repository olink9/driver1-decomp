/* FAKEMATCH: case 93 relies on a dead store (p = 0; p = &D_000C017C;) to steer the compiler - counted as fake until a natural form is found. */
/* func_00068C6C: mission script (DMS) interpreter step (no Driver 2 counterpart).
 * Notes from matching:
 *  - case 69: t->vx/t->vy/t->vz store order decides which register t gets.
 *  - cases 106/109: `lab = obj->label` written in BOTH arms of an if/else keeps the obj copy
 *    (addu v1,s6,zero) alive past cse-skip-blocks and puts it in the bgez delay slot.
 *  - case 93: `p = 0; p = &D_000C017C;` sets p twice so loop.c does not hoist the address
 *    (one set = hoisted and spilled into t3); probably not the original form. */
typedef struct { int vx, vy, vz, pad; } VEC;

typedef struct {
    unsigned char f0;
    char pad1[0xC8 - 1];
    VEC pos;            /* 0xC8 */
    char padD8[0xF8 - 0xD8];
    int fF8;            /* 0xF8 */
    char padFC[0x114 - 0xFC];
    int f114[6];        /* 0x114 */
    char pad12C[3];
    unsigned char f12F; /* 0x12F */
    char pad130[0x17C - 0x130];
    short f17C;         /* 0x17C */
    char pad17E[0x18C - 0x17E];
} CAR;

typedef struct {
    int *ptr;
    short s4;
    short s6;
    short s8;
    unsigned char a;
    unsigned char b;
    unsigned char c;
    char pad[3];
} SND;

typedef struct {
    int x, y, z;
    int dir;
    int g;
    int h;
    int i;
    short s[6];
    short s17C;
    short pad;
} CP;

typedef struct {
    int x, y, z;
    int dir;
    short s[6];
    short s1C;
    unsigned char b1E;
    unsigned char b1F;
} CD;

typedef struct {
    int f0, f4, f8, fC, f10, f14, f18, f1C, f20, f24;
} STAT;

typedef struct { int h, i, j, k; } Q4;

typedef struct {
    unsigned int a;
    unsigned int b;
    int c;
    int d;
    VEC *e;
    short f;
    int *g;
    Q4 q;
} ENT;

typedef struct {
    int *label[5];
    SND snd[3];         /* 0x14 */
    char pad44[0x50 - 0x44];
    int *f50;           /* 0x50 */
    char pad54[4];
    CP saved;           /* 0x58 */
    CP cur;             /* 0x84 */
    CD d[1];            /* 0xB0 */
    STAT st;            /* 0xD0 */
    CAR *fF8;           /* 0xF8 */
    int *fFC;           /* 0xFC */
    int f100;
    int f104;
    char pad108[0x114 - 0x108];
    int f114;
    int f118;
    int f11C;
    int f120;
    int *f124;
    int *f128;
    int f12C;
    ENT ent[6];         /* 0x130 */
    VEC tgt[6];         /* 0x238 */
    int f298;
    int f29C;
    int f2A0;
    char pad2A4[0x2B4 - 0x2A4];
    int *end;           /* 0x2B4 */
    int *pc;            /* 0x2B8 */
    char pad2BC[0x2D0 - 0x2BC];
    int f2D0;
    int f2D4;
    char pad2D8[0x2E4 - 0x2D8];
    unsigned char f2E4;
} OBJ;

typedef struct E {
    int st;
    int *nx;
    int *w2;
    int w3;
} E;
typedef struct { char pad[0xE]; short h; } H;

typedef struct {
    char pad[0x34];
    short f34;
    unsigned char f36;
    unsigned char f37;
    char pad38[0x74 - 0x38];
    int f74, f78, f7C;
} T17C;

typedef struct {
    char pad[0x34];
    short f34;
    unsigned char f36;
    unsigned char f37;
} T38;

typedef struct {
    int f0, f4, f8, fC;
    unsigned char f10, f11, f12;
} R20;

typedef struct {
    char pad[0x10];
    int *f10;
    unsigned char f14;
} X15;

typedef struct {
    char pad[0x10];
    unsigned char f10, f11;
    char pad12[80 - 0x12];
} M80;

extern long long D_00096F80;
extern long long D_0009709C;
extern long long D_00096878;
extern long long D_00097760;
extern long long D_000976F8;
extern long long D_00096FC8;
extern long long D_00097708;
extern long long D_00097738;
extern long long D_0009773C;
extern long long D_00096888;
extern long long D_000971D4;
extern long long D_00097118;
extern long long D_000967DC;
extern long long D_00096FBC;
extern long long D_00096898;
extern long long D_00096808;
extern int *D_0009778C;
extern E D_0008BD58[];
extern H D_000BE9C4;
extern H D_000BE9E0;
extern ENT D_000C7034[];
extern CAR D_0009B0A4[];
extern unsigned char D_000C024C[];
extern T17C D_000C017C;
extern T38 D_000C01B4;
extern R20 D_000C0210[];
extern X15 *D_0008B9F8[];
extern int D_0008B9E8[];
extern int D_000C01EC[];
extern OBJ D_000C6F04;
extern short D_000C4C9C[];
extern VEC D_000A7AAC;
extern VEC D_0008AE7C;
extern M80 D_00098900[];

extern int printf();
extern void func_00017AA0();
extern int func_0001638C();
extern CAR *func_00016094();
extern void func_00061A14();
extern unsigned char func_00061240();
extern void func_0006A60C();
extern void func_00068384();
extern void func_00068928();
extern void func_0006DD3C();
extern int *func_000687D4();

static inline void jump(OBJ *o, int v)
{
    if (v < 0)
        v = ~v;
    o->pc = o->label[v];
}

void func_00068C6C(OBJ *obj)

{
    int *q;
    int cont;

    cont = *(int *)&D_00096F80 != 3;
    do {
        q = obj->pc;
        switch (*q++) {
        case 0:
        default:
            break;
        case 1:
            cont = 0;
            *(int *)&D_0009709C = 1;
            obj->f2E4 = 0;
            break;
        case 2:
            {
                E *e = &D_0008BD58[q[0]];
                int *n;
                int v;
                if (e->st == 1) {
                    e->nx = q + 2;
                    break;
                }
                n = e->nx;
                v = q[1];
                if (n) {
                    *n = v;
                    if (e == &D_0008BD58[0x1F])
                        D_000BE9C4.h = v;
                    else if (e == &D_0008BD58[10])
                        D_000BE9E0.h = v;
                }
            }
            break;
        case 4:
            {
                SND *r = &obj->snd[q[0]];
                r->c = q[1];
                r->b = q[2];
                r->s6 = q[3];
                r->s8 = q[4];
                r->s4 = q[5];
                r->a = 3;
                r->ptr = q + 6;
            }
            break;
        case 7:
            if (obj->f104) {
                if (obj->f104 == 0x7FFF)
                    obj->f104 = 0;
                else
                    cont = 0;
            }
            break;
        case 8:
            obj->f2A0 = 0;
            obj->f29C = 0x8C;
            break;
        case 9:
            obj->f2A0 = q[0];
            obj->f29C = 12;
            break;
        case 10:
            *(int *)&D_00096878 = 1;
            break;
        case 11:
            *(int *)&D_00096878 = 0;
            break;
        case 12:
            (*D_0008BD58[q[0]].nx)++;
            break;
        case 14:
            {
                int i = q[0];
                int *n = D_0008BD58[i].nx;
                if (n == 0)
                    printf("DMS_COM_DECREASE: DMS variable %d not yet working\n", i);
                else
                    (*n)--;
            }
            break;
        case 16:
            {
                int *n = D_0008BD58[*q++].nx;
                if (n == 0)
                    printf("DMS_COM_ADD: DMS variable %d not yet working\n", *q);
                else
                    *n += *q;
            }
            break;
        case 17:
            {
                int *n = D_0008BD58[*q++].nx;
                if (n == 0)
                    printf("DMS_COM_SUB: DMS variable %d not yet working\n", *q);
                else
                    *n -= *q;
            }
            break;
        case 20:
            {
                int n;
                int r;
                int a, b, c;
                n = *q++;
                r = 0;
                while (n--) {
                    a = *q++;
                    b = *q++;
                    c = *q++;
                    func_00017AA0(0);
                    r = func_0001638C(a, b, c);
                    if (r == 0)
                        break;
                }
                obj->fF8 = (CAR *)r;
            }
            break;
        case 22:
            obj->f114 = q[0];
            obj->f124 = q + 2;
            break;
        case 24:
            if (obj->f118 == 0) {
                cont = 0;
                obj->f118 = q[0] * 30;
            } else {
                if (--obj->f118)
                    cont = 0;
            }
            break;
        case 25:
            {
                ENT *p;
                unsigned int v;
                for (p = D_000C7034; p < D_000C7034 + 6; p++) {
                    p->a = 0x2EE;
                    p->b = 0x5DC0;
                    p->c = 0x1C2;
                    p->d = 0;
                    p->e = 0;
                    p->f = 10;
                    p->g = 0;
                    p->q.h = 1000;
                    p->q.i = 0;
                    p->q.j = 0;
                    p->q.k = 0;
                }
                v = q[0];
                obj->ent[0].a = v;
                if (obj->ent[0].b < v)
                    obj->ent[0].b = v;
                obj->ent[0].e = &obj->tgt[0];
                obj->tgt[0].vx = q[1];
                obj->tgt[0].vy = 0x7FFFFFFF;
                obj->tgt[0].vz = q[2];
            }
            break;
        case 26:
            obj->ent[0].d |= 1;
            obj->f298++;
            break;
        case 27:
            obj->ent[0].d &= ~1;
            obj->f298--;
            break;
        case 28:
            obj->f29C &= ~8;
            break;
        case 29:
            obj->f29C |= 8;
            break;
        case 35:
            obj->f120 = q[0];
            break;
        case 36:
            obj->f11C |= 1;
            break;
        case 37:
            obj->f11C &= ~1;
            break;
        case 33:
            {
                SND *r = &obj->snd[q[0]];
                r->c = q[1];
                r->b = q[2];
                r->s6 = q[3];
                r->s8 = q[4];
                r->s4 = q[5];
                r->a = 5;
                r->ptr = q + 6;
            }
            break;
        case 38:
            if ((unsigned int)q[0] < 5)
                *(int *)&D_00097760 = q[0];
            break;
        case 50:
            *(int *)&D_000976F8 = 1;
            *(int *)&D_00096FC8 = 0;
            break;
        case 49:
            *(int *)&D_000976F8 = 0;
            *(int *)&D_00096FC8 = 0;
            break;
        case 53:
            {
                int t = (q[0] - 1) * 430 + 659;
                if (*(int *)&D_00097708 < t)
                    *(int *)&D_00097708 = t;
                *(int *)&D_00097738 = D_0009B0A4[0].pos.vx;
                *(int *)&D_0009773C = D_0009B0A4[0].pos.vz;
            }
            break;
        case 54:
            {
                int a = q[1], b = q[2], c = q[3], d = q[4], e = q[5];
                func_00017AA0(2);
                obj->fF8 = func_00016094(a, b, c, d, e);
            }
            break;
        case 55:
            obj->f11C |= 4;
            break;
        case 56:
            obj->f11C &= ~4;
            break;
        case 58:
            obj->f29C = 0;
            break;
        case 59:
            obj->f2A0 += q[0];
            break;
        case 60:
            obj->f2A0 -= q[0];
            break;
        case 69:
            {
                int i = q[0];
                ENT *e = &obj->ent[i];
                VEC *t;
                unsigned int v;
                Q4 *qq = &e->q;
                e->a = 0x2EE;
                e->b = 0x5DC0;
                e->c = 0x1C2;
                e->f = 10;
                qq->h = 1000;
                t = &obj->tgt[i];
                e->d = 0;
                e->e = 0;
                e->g = 0;
                qq->i = 0;
                qq->j = 0;
                qq->k = 0;
                v = q[1];
                e->a = v;
                if (e->b < v)
                    e->b = v;
                e->e = t;
                t->vx = q[2];
                t->vy = 0x7FFFFFFF;
                t->vz = q[3];
                e->d |= 1;
                obj->f298++;
            }
            break;
        case 70:
            {
                int i = *q++;
                short s = *q++;
                ENT *e = &obj->ent[i];
                e->f = s;
                e->g = q;
            }
            break;
        case 71:
            {
                ENT *e = &obj->ent[*q++];
                e->q.h = *q++;
                e->q.i = *q++;
                e->q.j = *q++;
                e->q.k = *q++;
            }
            break;
        case 74:
            D_000C024C[0] = q[0];
            break;
        case 77:
            func_00061A14(&D_000C017C);
            break;
        case 78:
            if (func_00061240(&D_000C017C))
                cont = 0;
            break;
        case 80:
            {
                int i = *q++;
                R20 *r = &D_000C0210[i];
                r->f0 = *q++;
                r->f4 = 0;
                r->f8 = *q++;
                r->f12 = *q++;
                r->f10 = *q++;
                r->f11 = *q++;
                r->fC = *q++;
            }
            break;
        case 82:
            {
                X15 *x = D_0008B9F8[q[0]];
                int b = q[4];
                x->f10 = q + 1;
                x->f14 = b;
            }
            break;
        case 83:
            {
                T38 *p = 0;
                int v = q[1];
                switch (q[0]) {
                case 0:
                    p = (T38 *)&D_000C017C;
                    break;
                case 1:
                    p = &D_000C01B4;
                    break;
                }
                p->f36 = v;
                p->f34 = 0;
                p->f37 = 0;
            }
            break;
        case 85:
            func_0006A60C(&D_000C6F04, q[0], q[1], q[2]);
            break;
        case 86:
            cont = 0;
            break;
        case 87:
            if (*(int *)&D_000971D4 == 0)
                cont = 0;
            break;
        case 88:
            {
                CP *cp = &obj->saved;
                CAR *c = &D_0009B0A4[0];
                int *src;
                short *dst;
                cp->x = c->pos.vx;
                cp->y = c->pos.vy;
                cp->z = c->pos.vz;
                cp->dir = c->fF8;
                cp->g = *(int *)&D_00097708;
                cp->s17C = c->f17C;
                for (src = c->f114, dst = cp->s; src < &D_0009B0A4[0].f114[6]; src++, dst++)
                    *dst = *src;
                cp->h = *(int *)&D_00097118;
                cp->i = obj->f2A0;
                *(char *)&D_000967DC = 1;
            }
            break;
        case 90:
            obj->f11C |= 0x10;
            break;
        case 91:
            obj->f11C &= ~0x10;
            break;
        case 92:
            obj->ent[q[0]].b = q[1];
            break;
        case 93:
            {
                T17C *p = 0;
                p = &D_000C017C;
                p->f74 = q[0];
                p->f78 = q[1];
                p->f7C = q[2];
            }

            break;
        case 65:
            {
                CAR *c = &D_0009B0A4[*(int *)&D_00096888];
                int v = q[0];
                c->f114[0] = v;
                c->f114[1] = v;
                c->f114[2] = v;
                c->f114[3] = v;
                c->f114[4] = v;
                c->f114[5] = v;
                c->f17C = v * 6;
                c->f12F = 1;
            }
            break;
        case 94:
            obj->f128 = q + 1;
            break;
        case 95:
            {
                short *d;
                for (d = D_000C4C9C; d < D_000C4C9C + 4; d++)
                    *d = *q++;
            }
            break;
        case 96:
            obj->ent[q[0]].d = q[1];
            break;
        case 97:
            {
                ENT *e = &obj->ent[q[0]];
                int v = q[1];
                if (v >= 0)
                    e->e = &D_0009B0A4[v].pos;
                else switch (v) {
                case -1:
                    e->e = &D_000A7AAC;
                    break;
                case -2:
                    e->e = &obj->fF8->pos;
                    break;
                }
            }
            break;
        case 98:
            D_0008AE7C.vx = q[0];
            D_0008AE7C.vy = q[1];
            D_0008AE7C.vz = q[2];
            break;
        case 99:
            {
                int a = obj->f100;
                int *b = obj->end;
                func_00068384(obj);
                cont = 0;
                obj->f100 = a;
                obj->end = b;
                obj->pc = D_0009778C;
                func_00068928(obj->pc, obj->end);
            }
            break;
        case 100:
            obj->pc = D_0009778C;
            cont = 0;
            break;
        case 101:
            {
                STAT *s = &obj->st;
                int score = obj->f2A0;
                int best = s->f18;
                int laps = obj->st.f0;
                unsigned char better = 0;
                if (obj->f2D0 == 2) {
                    if ((short)s->f24 < laps)
                        better = 1;
                    else if (laps == (short)s->f24 && best < score)
                        better = 1;
                } else if (obj->f29C & 0x80) {
                    if (score < best)
                        better = 1;
                } else if (best < score)
                    better = 1;
                if (better) {
                    s->f24 = laps;
                    s->f18 = score;
                }
            }
            break;
        case 102:
            if (obj->fF8 == 0) {
                *(char *)&D_00096FBC = 1;
                cont = 0;
            } else {
                *(char *)&D_00096FBC = 0;
            }
            break;
        case 103:
            {
                SND *r = &obj->snd[q[0]];
                r->s4 = q[1];
                r->b = q[2];
                r->a = 1;
            }
            break;
        case 104:
            obj->f50 = q;
            break;
        case 106:
            {
                int v = q[0];
                int **lab;
                if (v < 0) {
                    v = ~v;
                    lab = obj->label;
                } else
                    lab = obj->label;
                cont = 0;
                obj->pc = lab[v];
            }
            break;
        case 107:
            {
                int t;
                if (obj->st.f10 == 0x7FFFFFFF)
                    t = obj->f2A0;
                else
                    t = obj->f2A0 - obj->st.f8;
                obj->st.fC = t;
                obj->st.f8 = obj->f2A0;
                if (t < obj->st.f20)
                    obj->st.f20 = t;
                if (t < obj->st.f1C)
                    obj->st.f1C = t;
            }
            break;
        case 108:
            if (((unsigned char *)&D_00096898)[1] == 0 && *(int *)&D_00096F80 == 1) {
                if (*(int *)&D_00096808 != 0x12)
                    func_0006DD3C(*(int *)&D_00096808);
                else
                    *(int *)&D_00096F80 = 5;
            }
            if (*(int *)&D_00096F80 != 5)
                cont = 0;
            if (((unsigned char *)&D_00096898)[1])
                *(int *)&D_00096F80 = 5;
            break;
        case 109:
            {
                int *v[2];
                v[0] = D_0008BD58[q[0]].nx;
                v[1] = D_0008BD58[q[1]].nx;
                if (*v[0] == *v[1]) {
                    int w = q[2];
                    int **lab;
                    if (w < 0) {
                        w = ~w;
                        lab = obj->label;
                    } else
                        lab = obj->label;
                    cont = 0;
                    obj->pc = lab[w];
                }

            }
            break;
        case 111:
            D_000C01EC[0] = D_0008B9E8[q[0]];
            break;
        case 112:
            obj->fFC = q;
            break;
        case 113:
            {
                CD *d = &obj->d[q[0]];
                CAR *c = &D_0009B0A4[q[1]];
                d->x = c->pos.vx;
                d->y = c->pos.vy;
                d->z = c->pos.vz;
                d->dir = c->fF8;
                d->s[0] = c->f114[0];
                d->s[1] = c->f114[1];
                d->s[2] = c->f114[2];
                d->s[3] = c->f114[3];
                d->s[4] = c->f114[4];
                d->s[5] = c->f114[5];
                d->s1C = c->f17C;
                {
                    M80 *m = &D_00098900[c->f0];
                    d->b1E = m->f10;
                    d->b1F = m->f11;
                }
            }
            break;
        case 114:
            {
                CD *d = &obj->d[q[0]];
                CAR *c;
                int a, b, dir, x, z;
                func_00017AA0(2);
                a = d->b1E;
                b = d->b1F;
                dir = d->dir;
                x = d->x;
                z = d->z;
                func_00017AA0(2);
                c = func_00016094(a, b, dir, x, z);
                c->f114[0] = d->s[0];
                c->f114[1] = d->s[1];
                c->f114[2] = d->s[2];
                c->f114[3] = d->s[3];
                c->f114[4] = d->s[4];
                c->f114[5] = d->s[5];
                c->f17C = d->s1C;
                c->f12F = 1;
            }
            break;
        }
        if (cont)
            obj->pc = func_000687D4(obj->pc);
    } while (obj->f2D4 && cont && obj->pc < obj->end);
}
