typedef struct { unsigned addr:24; unsigned len:8; } P_TAG;
typedef struct {
    unsigned tag;
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
typedef struct { short x, y, z; unsigned char u, v; } VTX;
typedef struct { char *primptr; P_TAG *ot; } DB;
typedef struct { short a, b; } CNT;
extern DB *current;
extern VTX *D_001CF318;
extern int *D_001CF31C;
extern unsigned int *D_001CF324;
extern unsigned char *D_001CF30C;
extern unsigned char *D_001CF310;
extern signed char *D_001CF314;
extern int D_001CF304, D_001D3280, D_001D3284, D_001D3288, D_001D3290;
extern CNT D_001D3298[5];
extern int func_001CBEFC();

#define gte_ldv0(r0) __asm__ volatile ( \
    "lwc2  $0, 0( %0 );" "lwc2  $1, 4( %0 )" : : "r"(r0))
#define gte_ldv3(r0, r1, r2) __asm__ volatile ( \
    "lwc2  $0, 0( %0 );" "lwc2  $1, 4( %0 );" "lwc2  $2, 0( %1 );" \
    "lwc2  $3, 4( %1 );" "lwc2  $4, 0( %2 );" "lwc2  $5, 4( %2 )" : : "r"(r0), "r"(r1), "r"(r2))
#define gte_rtps() __asm__ volatile ("nop;" "nop;" "rtps")
#define gte_rtpt() __asm__ volatile ("nop;" "nop;" "rtpt")
#define gte_nclip() __asm__ volatile ("nop;" "nop;" "nclip")
#define gte_stopz(r0) __asm__ volatile ("swc2  $24, 0( %0 )" : : "r"(r0) : "memory")
#define gte_stsxy(r0) __asm__ volatile ("swc2  $14, 0( %0 )" : : "r"(r0) : "memory")
#define gte_stsz(r0) __asm__ volatile ("swc2  $19, 0( %0 )" : : "r"(r0) : "memory")
#define gte_stsxy3c(r0) __asm__ volatile ( \
    "swc2  $12, 0( %0 );" "swc2  $13, 4( %0 );" "swc2  $14, 8( %0 )" : : "r"(r0) : "memory")
#define gte_stsz3c(r0) __asm__ volatile ( \
    "swc2  $17, 0( %0 );" "swc2  $18, 4( %0 );" "swc2  $19, 8( %0 )" : : "r"(r0) : "memory")

/* func_001CBE30, inlined */
static inline int func_001CBE30(unsigned char *p)
{
    int flag;
    gte_ldv3(&D_001CF318[p[0]], &D_001CF318[p[1]], &D_001CF318[p[2]]);
    gte_rtpt();
    gte_stsxy3c(D_001CF31C);
    if (D_001D3290 == 0) {
        gte_nclip();
        gte_stopz(&flag);
        if (flag < 0) return 1;
    }
    gte_stsz3c(D_001CF324);
    return 0;
}

/* textured quad with recursive subdivision near the screen edges (func_001CBAA4 is the triangle version) */
void func_001CC1D0(unsigned char *p)
{
    POLY_FT4 *t;
    int z;
    int i, j, k;
    int b;

    if (func_001CBE30(p) != 0)
        return;
    gte_ldv0(&D_001CF318[p[3]]);
    gte_rtps();
    gte_stsxy(D_001CF31C + 3);
    gte_stsz(D_001CF324 + 3);
    if (func_001CBEFC(4) != 0)
        return;
    if ((D_001D3298[0].b || D_001D3298[1].b || D_001D3298[2].b || D_001D3298[3].b) && D_001D3290 <= 0) {
        for (i = 0; i < 5; i++) {
            k = p[D_001CF30C[i]];
            b = p[D_001CF30C[i + 1]];
            D_001CF318[D_001D3288].x = (D_001CF318[k].x + D_001CF318[b].x) >> 1;
            D_001CF318[D_001D3288].y = (D_001CF318[k].y + D_001CF318[b].y) >> 1;
            D_001CF318[D_001D3288].z = (D_001CF318[k].z + D_001CF318[b].z) >> 1;
            D_001CF318[D_001D3288].u = (D_001CF318[k].u + D_001CF318[b].u) >> 1;
            D_001CF318[D_001D3288].v = (D_001CF318[k].v + D_001CF318[b].v) >> 1;
            D_001D3288++;
        }
        D_001D3290++;
        for (k = i = 0; i < 4; i++) {
            for (j = 0; j < 4; j++) {
                if (D_001CF314[k] >= 0)
                    D_001CF310[j] = p[D_001CF314[k]];
                else
                    D_001CF310[j] = D_001CF314[k] + D_001D3288;
                k++;
            }
            func_001CC1D0(D_001CF310);
        }
        D_001D3288 -= 5;
        D_001D3290 -= 1;
        return;
    }
    t = (POLY_FT4 *)current->primptr;
    ((P_TAG *)t)->len = 9;
    t->code = 0x2C;
    t->r0 = 0x80;
    t->g0 = 0x80;
    t->b0 = 0x80;
    *(long *)&t->x0 = D_001CF31C[0];
    *(long *)&t->x1 = D_001CF31C[1];
    *(long *)&t->x2 = D_001CF31C[3];
    *(long *)&t->x3 = D_001CF31C[2];
    *(long *)&t->u0 = *(unsigned short *)&D_001CF318[p[0]].u | D_001D3284;
    *(long *)&t->u1 = *(unsigned short *)&D_001CF318[p[1]].u | D_001D3280;
    *(long *)&t->u2 = *(unsigned short *)&D_001CF318[p[3]].u;
    *(long *)&t->u3 = *(unsigned short *)&D_001CF318[p[2]].u;
    switch (D_001CF304) {
    default:
        z = (D_001CF324[0] + D_001CF324[1] + D_001CF324[2] + D_001CF324[3]) >> 2;
        break;
    case 1:
        z = 0x107D;
        break;
    case 3:
        z = ((D_001CF324[0] + D_001CF324[1] + D_001CF324[2] + D_001CF324[3]) >> 2) + 0x28;
        break;
    case 2:
        z = ((D_001CF324[0] + D_001CF324[1] + D_001CF324[2] + D_001CF324[3]) >> 2) + 0x5A;
        break;
    }
    ((P_TAG *)t)->addr = current->ot[z].addr;
    current->ot[z].addr = (unsigned)t;
    current->primptr += sizeof(POLY_FT4);
}
