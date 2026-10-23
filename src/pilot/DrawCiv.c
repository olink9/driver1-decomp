/* DrawCiv: Driver 2 counterpart DrawCiv (MOTION_C.C) - draw a pedestrian: shadow, interpolated
 * bone vertices into scratchpad, one POLY_FT4 sprite per bone, then the head model via func_0002E230.
 * The sprite cos lookups are written rcossin_tbl[i*2 + 1]: the target labels them D_0008EAAA (same
 * address); only this single-table form lets loop.c hoist the table base (rematerialised in $t9). */
#include "gte.h"

typedef struct { short vx, vy, vz, pad; } SVECTOR;
typedef struct { int vx, vy, vz, pad; } VECTOR;
typedef struct { unsigned char r, g, b, cd; } CVECTOR;
typedef struct { short m[3][3]; short pad; int t[3]; } MATRIX;
typedef struct { char *primptr; unsigned int *ot; } DB;

typedef struct {
    unsigned short uv0, uv1, uv2, uv3;
    unsigned short tpage, clut;
    unsigned char id;
} TEX;

typedef struct {
    unsigned char v0, v1, v2, v3;
    void **model;
    TEX *tex;
    unsigned char type;
} BONE;

typedef struct {
    unsigned char nframes;
    unsigned char pad1;
    unsigned char nverts;
    unsigned char pad3;
    short offs[1];
} MOTION;

typedef struct {
    VECTOR pos;          /* 0x00 */
    char pad10[8];
    SVECTOR dir;         /* 0x18 */
    char pad20[0x18];
    BONE **bones;        /* 0x38 */
    int flags;           /* 0x3C */
    short f40;           /* 0x40 */
    char pad42[6];
    unsigned char frame; /* 0x48 */
    char pad49[3];
    MOTION *motion;      /* 0x4C */
    char pad50[5];
    unsigned char pal;   /* 0x55 */
} PED;

typedef struct {
    unsigned int tag;
    unsigned int rgbc;
    unsigned int xy0;
    unsigned int uvc0;
    unsigned int xy1;
    unsigned int uvt1;
    unsigned int xy2;
    unsigned int uv2;
    unsigned int xy3;
    unsigned int uv3;
} PFT4;

typedef struct { unsigned addr:24; unsigned len:8; } P_TAG;
#define setaddr(_p, _addr) (((P_TAG *)(_p))->addr = (unsigned long)(_addr))
#define getaddr(_p) (unsigned long)(((P_TAG *)(_p))->addr)
#define addPrim(ot, p) setaddr(p, getaddr(ot)), setaddr(ot, p)

extern VECTOR camera_position;
extern short D_0008EAAA[];
extern short rcossin_tbl[];
extern long long gNight;
extern CVECTOR D_00096E48[];
extern MATRIX inv_camera_matrix;
extern MATRIX matrixtable[];
extern long long D_000970E4, D_00096A48;
extern long long current;
#define current (*(DB **)&current)
extern long long combointensity;
extern long long D_00096C4A;
extern int D_000BE9FC[][30][5];
extern long long D_000970B0, D_000970B4, D_000970B8, D_000970BC;
extern long long D_000971C0;

extern void ShowGroundLight();
extern void func_0004EAC0();
extern void MulMatrix0();
extern int ratan2();
extern void func_0002E230();

#define gte_ldv3c(r0) __asm__ volatile ( \
    "lwc2  $0, 0( %0 );" "lwc2  $1, 4( %0 );" "lwc2  $2, 8( %0 );" \
    "lwc2  $3, 12( %0 );" "lwc2  $4, 16( %0 );" "lwc2  $5, 20( %0 )" : : "r"(r0))
#define gte_rtpt() __asm__ volatile ("nop;" "nop;" "rtpt")
#define gte_rtv0() __asm__ volatile ("nop;" "nop;" "mvmva 1, 0, 0, 3, 0")
#define gte_stsxy3c(r0) __asm__ volatile ( \
    "swc2  $12, 0( %0 );" "swc2  $13, 4( %0 );" "swc2  $14, 8( %0 )" : : "r"(r0) : "memory")
#define gte_stsz3c(r0) __asm__ volatile ( \
    "swc2  $17, 0( %0 );" "swc2  $18, 4( %0 );" "swc2  $19, 8( %0 )" : : "r"(r0) : "memory")

#define FIX(x) (((x) + 0x800) >> 12)

#define RotYXZ(R, M) { \
    int cx, sx, cy, sy, cz, sz, stmp, tt; \
    cx = D_0008EAAA[((R)->vx & 0xfff) * 2]; \
    sx = D_0008EAAA[((R)->vx & 0xfff) * 2 - 1]; \
    sy = D_0008EAAA[((R)->vy & 0xfff) * 2 - 1]; \
    cy = D_0008EAAA[((R)->vy & 0xfff) * 2]; \
    cz = D_0008EAAA[((R)->vz & 0xfff) * 2]; \
    sz = D_0008EAAA[((R)->vz & 0xfff) * 2 - 1]; \
    stmp = FIX(cy * sz); \
    (M)->m[0][0] = FIX(cy * cz); \
    (M)->m[0][1] = FIX(-cx * stmp) + FIX(sx * sy); \
    stmp = FIX(cy * sx); \
    tt = cx * sy; \
    stmp = FIX(stmp * sz); \
    (M)->m[0][2] = FIX(tt) + stmp; \
    (M)->m[1][0] = sz; \
    (M)->m[1][1] = FIX(cx * cz); \
    (M)->m[1][2] = -FIX(cz * sx); \
    (M)->m[2][0] = -FIX(sy * cz); \
    (M)->m[2][1] = FIX((M)->m[0][2] * sz) - FIX((M)->m[0][0] * (M)->m[1][2]); \
    (M)->m[2][2] = FIX((M)->m[0][0] * (M)->m[1][1]) - FIX((M)->m[0][1] * sz); \
}

void DrawCiv(PED *p, int idx)
{
    char pad[0x20];
    MATRIX m;
    MATRIX hm;
    VECTOR pos;
    VECTOR v;
    CVECTOR cv;
    VECTOR hv;
    SVECTOR rot;
    SVECTOR sv;
    BONE **bl;
    BONE *head;
    long *sxy;
    long *sxy2;
    long *zb;
    SVECTOR *lerp;
    SVECTOR *out;
    int j;
    BONE *b;
    MOTION *mo;
    int *v1p, *v2p;
    int count, i;
    int size;
    int phase;

    sxy = (long *)0x1F800010;
    sxy2 = (long *)0x1F800010;
    zb = (long *)0x1F800210;
    head = 0;
    lerp = (SVECTOR *)0x1F800090;
    j = 0;
    bl = p->bones;
    out = (SVECTOR *)0x1F800090;

    pos.vx = p->pos.vx - camera_position.vx;
    pos.vy = p->pos.vy - camera_position.vy;
    pos.vz = p->pos.vz - camera_position.vz;
    gte_SetRotMatrix(&inv_camera_matrix);
    v.vy = pos.vy - 30;
    v.vx = pos.vx;
    v.vz = pos.vz;
    if (*(int *)&gNight == 0 && !(p->flags & 0x1000)) {
        VECTOR *pv = &v;
        CVECTOR *pcv = &cv;
        cv = D_00096E48[0];
        phase = (p->frame & 0xF) * 2;
        if (phase >= 16)
            phase = 0x70 - phase;
        else
            phase += 0x50;
        ShowGroundLight(pv, pcv, phase);
    }
    func_0004EAC0(&pos);
    gte_SetTransVec0(&pos);

    if (p->dir.vx == 0 && p->dir.vz == 0) {
        if (p->dir.vy & 0x3F) {
            m.m[1][2] = 0;
            m.m[2][1] = 0;
            m.m[1][0] = 0;
            m.m[0][1] = 0;
            m.m[1][1] = 0x1000;
            m.m[0][0] = m.m[2][2] = D_0008EAAA[(p->dir.vy & 0xfff) * 2];
            hm.m[0][2] = D_0008EAAA[(p->dir.vy & 0xfff) * 2 - 1]; /* sic: also writes the head matrix */
            m.m[0][2] = D_0008EAAA[(p->dir.vy & 0xfff) * 2 - 1];
            m.m[2][0] = -m.m[0][2];
            MulMatrix0(&inv_camera_matrix, &m, &m);
        } else {
            MulMatrix0(&inv_camera_matrix, &matrixtable[(p->dir.vy & 0xFC0) >> 6], &m);
        }
    } else {
        RotYXZ(&p->dir, &m);
        MulMatrix0(&inv_camera_matrix, &m, &m);
    }
    gte_SetRotMatrix(&m);

    mo = p->motion;
    v2p = (int *)((char *)mo + *(short *)((p->frame & ~1) + (int)mo + 4));
    count = mo->nverts;
    if (p->frame & 1) {
        if (p->frame < (mo->nframes - 1) * 2)
            v1p = (int *)((char *)mo + *(short *)(((p->frame + 1) & ~1) + (int)mo + 4));
        else
            v1p = (int *)((char *)mo + mo->offs[0]);
        for (i = count; i > 0;) {
            int w1, w2;
            i--;
            w1 = *v1p++;
            w2 = *v2p++;
            out->vx = ((w1 & 0x7FF) + (w2 & 0x7FF) - 0x800) >> 1;
            out->vy = (((w1 >> 11) & 0x3FF) + ((w2 >> 11) & 0x3FF) - 0x400) >> 1;
            out->vz = ((int)((unsigned)w1 >> 21) + (int)((unsigned)w2 >> 21) - 0x800) >> 1;
            out++;
        }
    } else {
        for (i = count; i > 0;) {
            int w;
            i--;
            w = *v2p++;
            out->vx = (w & 0x7FF) - 0x400;
            out->vy = ((w >> 11) & 0x3FF) - 0x200;
            out->vz = (int)((unsigned)w >> 21) - 0x400;
            out++;
        }
    }

    gte_ldv3c(&lerp[j]);
    gte_rtpt();
    b = *bl++;
    gte_stsz(zb);
    size = zb[0] + (*(int *)&D_000970E4 >> 2);
    size = (*(int *)&D_000970E4 << 12) / size;
    if (p->flags & 0x4000)
        size = (size * 5) >> 2;

    for (i = 0; b != 0; i++, b = *bl++) {
        TEX *tex = b->tex;
        PFT4 *prim;
        unsigned int tpage, clut;
        int x0, y0, x1, y1, dx, dy, ox, oy;
        short ang;
        int zsum, s, c, sn, ex, ey, w, zofs, t1, t2;
        int pal;
        int i0, i1;

        if (j < count) {
            gte_stsxy3c(&sxy[j]);
            gte_stsz3c(&zb[j]);
        }
        if (j < count - 3) {
            j += 3;
            gte_ldv3c(&lerp[j]);
            gte_rtpt();
        }
        i0 = b->v0;
        i1 = b->v1;
        if (zb[0] >= *(int *)&D_00096A48) {
            if (i == 0 || i == 3 || i == 8 || i == 11)
                continue;
        }
        if (b->model == 0 || zb[0] > *(int *)&D_00096A48 >> 2) {
            tpage = tex->tpage << 16;
            pal = p->pal;
            prim = (PFT4 *)current->primptr;
            if (b->type == 0x19) {
                pal >>= 4;
                if (pal)
                    clut = D_000BE9FC[1][tex->id][pal - 1] << 16;
                else
                    clut = tex->clut << 16;
            } else if (b->type) {
                pal &= 0xF;
                if (pal)
                    clut = D_000BE9FC[1][tex->id][pal - 1] << 16;
                else
                    clut = tex->clut << 16;
            } else {
                clut = tex->clut << 16;
            }
            x0 = sxy2[i0];
            x1 = sxy2[i1];
            y0 = x0 & 0xFFFF0000;
            x0 <<= 16;
            y1 = x1 & 0xFFFF0000;
            x1 <<= 16;
            if (zb[i0] < 50)
                return;
            dx = x0 - x1;
            dy = y0 - y1;
            zsum = zb[i0] + zb[i1];
            ang = -ratan2(dx, dy);
            ox = dx >> (b->v3 & 0xF);
            dx >>= b->v3 >> 4;
            oy = dy >> (b->v3 & 0xF);
            dy >>= b->v3 >> 4;
            prim->rgbc = *(int *)&combointensity;
            s = size;
            if (i == 6) {
                int o = ((*(short *)&D_00096C4A + p->f40 + 0x800) & 0xFFF) >> 7;
                prim->uvc0 = (tex->uv0 | clut) + o;
                prim->uvt1 = (tex->uv1 | tpage) + o;
                prim->uv2 = tex->uv2 + o;
                prim->uv3 = tex->uv3 + o;
                ang = o;
            } else {
                if (i == 7)
                    s += rcossin_tbl[((p->dir.vy + *(short *)&D_00096C4A) * 2 & 0xFFF) * 2 + 1] >> 6;
                prim->uvc0 = tex->uv0 | clut;
                prim->uvt1 = tex->uv1 | tpage;
                prim->uv2 = tex->uv2;
                prim->uv3 = tex->uv3;
            }
            c = rcossin_tbl[(ang & 0xFFF) * 2 + 1] * 3;
            sn = rcossin_tbl[(ang & 0xFFF) * 2];
            t1 = s * c;
            t2 = s * sn;
            w = b->v2;
            zofs = w >> 5;
            w &= 0x3F;
            ex = (t1 * w) >> 9;
            ey = (t2 * w) >> 8;
            ((P_TAG *)prim)->len = 9;
            ((unsigned char *)prim)[7] = 0x2C;
            prim->xy0 = ((y0 + ey + oy) & 0xFFFF0000) + ((x0 + ex + ox) >> 16);
            prim->xy1 = ((y0 - ey + oy) & 0xFFFF0000) + ((x0 - ex + ox) >> 16);
            prim->xy2 = ((y1 + ey - dy) & 0xFFFF0000) + ((x1 + ex - dx) >> 16);
            prim->xy3 = ((y1 - ey - dy) & 0xFFFF0000) + ((x1 - ex - dx) >> 16);
            addPrim(current->ot + ((zsum >> 4) + zofs), prim);
            current->primptr += sizeof(PFT4);
        } else
            head = b;
    }

    if (head) {
        void *model = *head->model;
        SVECTOR *a = &lerp[head->v1];
        SVECTOR *q = &lerp[head->v0];

        switch (idx & 3) {
        case 0: model = *(void **)&D_000970B0; break;
        case 1: model = *(void **)&D_000970B4; break;
        case 2: model = *(void **)&D_000970B8; break;
        case 3: model = *(void **)&D_000970BC; break;
        }
        if (model == 0)
            model = *(void **)&D_000970B4;

        if (p->dir.vx == 0 && p->dir.vz == 0) {
            if (p->dir.vy & 0x3F) {
                hm.m[1][2] = 0;
                hm.m[2][1] = 0;
                hm.m[1][0] = 0;
                hm.m[0][1] = 0;
                hm.m[1][1] = 0x1000;
                hm.m[0][0] = hm.m[2][2] = D_0008EAAA[(p->dir.vy & 0xfff) * 2];
                hm.m[0][2] = D_0008EAAA[(p->dir.vy & 0xfff) * 2 - 1];
                hm.m[2][0] = -hm.m[0][2];
                gte_SetRotMatrix(&hm);
            } else {
                gte_SetRotMatrix(&matrixtable[(p->dir.vy & 0xFC0) >> 6]);
            }
        } else {
            RotYXZ(&p->dir, &hm);
            gte_SetRotMatrix(&hm);
        }
        gte_ldv0(a);
        gte_rtv0();
        gte_stlvnl(&hv);
        gte_SetRotMatrix(&inv_camera_matrix);
        sv.vx = hv.vx + (p->pos.vx - camera_position.vx);
        sv.vy = hv.vy + (p->pos.vy - camera_position.vy);
        sv.vz = hv.vz + (p->pos.vz - camera_position.vz);
        gte_ldv0(&sv);
        gte_rtv0();
        gte_stlvnl(&v);
        rot.vx = p->dir.vx - ratan2(a->vz - q->vz, a->vy - q->vy);
        rot.vy = p->f40;
        rot.vz = p->dir.vz - ratan2(a->vx - q->vx, a->vy - q->vy);
        RotYXZ(&rot, &hm);
        gte_SetRotMatrix(&hm);
        *(unsigned char *)&D_000971C0 = 0;
        func_0002E230(model, &hm, &v, 0);
    }
}
