/* NOTE b113: 99.99 - only diff is row 202 "addiu v0, s3, %lo(D_1F80000C)" (target reloc to a splat label) vs "addiu v0, s3, 0xc": byte-identical. */
/* func_0002EB94: draws a shadow-like textured tri/quad (+ flat underlay) per object in a list,
 * using camera x/z for the texture u/v offset. No Driver 2 counterpart. */
typedef struct { short vx, vy, vz, pad; } SVECTOR;
typedef struct { int vx, vy, vz; } VECTOR;
typedef struct { short m[3][3]; int t[3]; } MATRIX;
typedef struct { char *primptr; unsigned int *ot; } DB;

typedef struct {
    char pad0[0xD];
    unsigned char flags;          /* 0x0D */
    unsigned short model;         /* 0x0E */
} OBJ;

typedef struct {
    char pad0[0x10];
    SVECTOR *verts;               /* 0x10 */
    unsigned char *polys;         /* 0x14 */
} MODEL;

typedef struct { short vx, vy; } __attribute__((aligned(4))) DVECTOR;
typedef union { struct { unsigned char u, v; } b; unsigned short w; } UV;
typedef struct { int u, v; } UVBASE;
typedef struct { unsigned addr:24; unsigned len:8; } P_TAG;
#define setaddr(_p, _addr) (((P_TAG *)(_p))->addr = (unsigned long)(_addr))
#define getaddr(_p) (unsigned long)(((P_TAG *)(_p))->addr)
#define addPrim(ot, p) setaddr(p, getaddr(ot)), setaddr(ot, p)

typedef struct {
    unsigned int tag;
    unsigned char r0, g0, b0, code;
    DVECTOR xy0, xy1, xy2;
} PF3;
typedef struct {
    unsigned int tag;
    unsigned char r0, g0, b0, code;
    DVECTOR xy0, xy1, xy2, xy3;
} PF4;
typedef struct {
    unsigned int tag;
    unsigned char r0, g0, b0, code;
    DVECTOR xy0; unsigned int uv0clut; DVECTOR xy1; unsigned int uv1tpage; DVECTOR xy2; unsigned int uv2;
} PFT3;
typedef struct {
    unsigned int tag;
    unsigned char r0, g0, b0, code;
    DVECTOR xy0; unsigned int uv0clut; DVECTOR xy1; unsigned int uv1tpage; DVECTOR xy2; unsigned int uv2; DVECTOR xy3; unsigned int uv3;
} PFT4;

extern OBJ **D_0009719C;
extern MODEL *D_000A0E20[];
extern VECTOR D_0008A8F0;
extern MATRIX D_000A5E3C[];
extern long long D_000974B4;
extern unsigned short D_00087030[][32];
extern unsigned short D_00086F30[];
extern long long D_00097530;
#define current (*(DB **)&D_00097530)
extern int func_0002680C();
extern void func_00074D54();

#define gte_SetRotMatrix(r0) __asm__ volatile ( \
    "lw    $12, 0( %0 );" "lw    $13, 4( %0 );" "ctc2  $12, $0;" "ctc2  $13, $1;" \
    "lw    $12, 8( %0 );" "lw    $13, 12( %0 );" "lw    $14, 16( %0 );" \
    "ctc2  $12, $2;" "ctc2  $13, $3;" "ctc2  $14, $4" : : "r"(r0) : "$12", "$13", "$14")
#define gte_ldv3c(r0) __asm__ volatile ( \
    "lwc2  $0, 0( %0 );" "lwc2  $1, 4( %0 );" "lwc2  $2, 8( %0 );" \
    "lwc2  $3, 12( %0 );" "lwc2  $4, 16( %0 );" "lwc2  $5, 20( %0 )" : : "r"(r0))
#define gte_ldv0(r0) __asm__ volatile ("lwc2  $0, 0( %0 );" "lwc2  $1, 4( %0 )" : : "r"(r0))
#define gte_rtpt() __asm__ volatile ("nop;" "nop;" "rtpt")
#define gte_rtps() __asm__ volatile ("nop;" "nop;" "rtps")
#define gte_avsz3() __asm__ volatile ("nop;" "nop;" "avsz3")
#define gte_avsz4() __asm__ volatile ("nop;" "nop;" "avsz4")
#define gte_stotz(r0) __asm__ volatile ("swc2  $7, 0( %0 )" : : "r"(r0) : "memory")
#define gte_stsxy2(r0) __asm__ volatile ("swc2  $14, 0( %0 )" : : "r"(r0) : "memory")
#define gte_stsxy3c(r0) __asm__ volatile ( \
    "swc2  $12, 0( %0 );" "swc2  $13, 4( %0 );" "swc2  $14, 8( %0 )" : : "r"(r0) : "memory")

void func_0002EB94(int count)
{
    int z;
    OBJ **list;
    unsigned int clut;
    unsigned int tpage;
    int i;
    OBJ *obj;
    MODEL *model;
    SVECTOR *verts;
    unsigned char *polys;
    unsigned char type;
    DVECTOR *sxy;
    UV *uv;
    UVBASE *uvbase;

    uv = (UV *)0x1F800040;
    uvbase = (UVBASE *)0x1F800060;
    sxy = (DVECTOR *)0x1F800000;
    list = D_0009719C;
    clut = 0;
    tpage = 0;

    for (i = 0; i < count; i++) {
        obj = list[i];
        model = D_000A0E20[obj->model];
        polys = model->polys;
        verts = model->verts;
        if (i == 0) {
            if (obj->flags & 0x20)
                uvbase->u = (D_0008A8F0.vx >> 7) & 0x3F;
            else
                uvbase->u = (D_0008A8F0.vz >> 7) & 0x3F;
            uvbase->v = (D_0008A8F0.vy >> 5) & 0x3F;
            z = func_0002680C(obj);
            gte_SetRotMatrix(&D_000A5E3C[obj->flags]);
            clut = D_00087030[*(int *)&D_000974B4][0] << 16;
            tpage = (D_00086F30[*(int *)&D_000974B4] | 0x20) << 16;
        }
        gte_ldv3c(verts);
        gte_rtpt();
        uv[0].b.u = uvbase->u + (verts[0].vx + 0x465) / 64;
        uv[1].b.u = uvbase->u + (verts[1].vx + 0x465) / 64;
        uv[2].b.u = uvbase->u + (verts[2].vx + 0x465) / 64;
        uv[0].b.v = uvbase->v + (verts[0].vy + 800) / 32;
        uv[1].b.v = uvbase->v + (verts[1].vy + 800) / 32;
        uv[2].b.v = uvbase->v + (verts[2].vy + 800) / 32;
        type = *polys & 7;
        gte_stsxy3c(sxy);
        if (type & 1) {
            gte_ldv0(&verts[3]);
            gte_rtps();
            uv[3].b.u = uvbase->u + (verts[3].vx + 0x465) / 64;
            uv[3].b.v = uvbase->v + (verts[3].vy + 800) / 32;
            gte_avsz4();
            gte_stsxy2(&sxy[3]);
            if (sxy[0].vx < 0 && sxy[1].vx < 0 && sxy[2].vx < 0 && sxy[3].vx < 0)
                continue;
            if (sxy[0].vx > 512 && sxy[1].vx > 512 && sxy[2].vx > 512 && sxy[3].vx > 512)
                continue;
        } else {
            gte_avsz3();
            if (sxy[0].vx < 0 && sxy[1].vx < 0 && sxy[2].vx < 0)
                continue;
            if (sxy[0].vx > 512 && sxy[1].vx > 512 && sxy[2].vx > 512)
                continue;
        }
        gte_stotz(&z);
        z = (z >> 1) + 20;
        if (z < 10)
            continue;
        if (!(type & 1)) {
            PFT3 *ft3 = (PFT3 *)current->primptr;
            PF3 *f3 = (PF3 *)(ft3 + 1);
            ((P_TAG *)f3)->len = 4;
            f3->code = 0x20;
            ((P_TAG *)ft3)->len = 7;
            ft3->code = 0x24;
            f3->code |= 2;
            ft3->code |= 2;
            f3->r0 = 20; f3->g0 = 20; f3->b0 = 20;
            ft3->r0 = 30; ft3->g0 = 30;
            ft3->b0 = 34;
            ft3->xy0 = sxy[0];
            ft3->xy1 = sxy[1];
            ft3->xy2 = sxy[2];
            f3->xy0 = sxy[0];
            f3->xy1 = sxy[1];
            f3->xy2 = sxy[2];
            ft3->uv0clut = uv[0].w | clut;
            ft3->uv1tpage = uv[1].w | tpage;
            ft3->uv2 = uv[2].w;
            polys = (unsigned char *)(f3 + 1);
            func_00074D54(polys, 1, 1, 0x40, 0);
            addPrim(&current->ot[z], ft3);
            addPrim(&current->ot[z], f3);
            addPrim(&current->ot[z], polys);
            current->primptr += 0x40;
        } else {
            PFT4 *ft4 = (PFT4 *)current->primptr;
            PF4 *f4 = (PF4 *)(ft4 + 1);
            ((P_TAG *)f4)->len = 5;
            f4->code = 0x28;
            ((P_TAG *)ft4)->len = 9;
            ft4->code = 0x2C;
            f4->code |= 2;
            ft4->code |= 2;
            f4->r0 = 20; f4->g0 = 20; f4->b0 = 20;
            ft4->r0 = 30; ft4->g0 = 30;
            ft4->b0 = 34;
            ft4->xy0 = sxy[0];
            ft4->xy1 = sxy[1];
            ft4->xy2 = sxy[3];
            ft4->xy3 = sxy[2];
            f4->xy0 = sxy[0];
            f4->xy1 = sxy[1];
            f4->xy2 = sxy[3];
            f4->xy3 = sxy[2];
            ft4->uv0clut = uv[0].w | clut;
            ft4->uv1tpage = uv[1].w | tpage;
            ft4->uv2 = uv[3].w;
            ft4->uv3 = uv[2].w;
            polys = (unsigned char *)(f4 + 1);
            func_00074D54(polys, 1, 1, 0x40, 0);
            addPrim(&current->ot[z], ft4);
            addPrim(&current->ot[z], f4);
            addPrim(&current->ot[z], polys);
            current->primptr += 0x4C;
        }
    }
}
