/* ShowLight1 (Driver 2) */
typedef struct { short vx, vy, vz, pad; } SV;
typedef struct { unsigned char u0, v0, u1, v1, u2, v2, u3, v3; unsigned short tpageid, clutid; } TD;
typedef struct { unsigned char *primptr; unsigned long **ot; } DB;
typedef struct { unsigned long tag; unsigned char r0, g0, b0, code; short x0, y0; unsigned char u0, v0; unsigned short clut;
  short x1, y1; unsigned char u1, v1; unsigned short tpage; short x2, y2; unsigned char u2, v2; unsigned short pad2;
  short x3, y3; unsigned char u3, v3; unsigned short pad3; } PFT4;
extern long long current;
#define current (*(DB **)&current)
extern int LightSortCorrect;

#define ldv3() __asm__ volatile("lwc2 $0,%0;lwc2 $1,%1;lwc2 $2,%2;lwc2 $3,%3;lwc2 $4,%4;lwc2 $5,%5" : : "m"(vert[0].vx),"m"(vert[0].vz),"m"(vert[1].vx),"m"(vert[1].vz),"m"(vert[2].vx),"m"(vert[2].vz))
#define rtpt() __asm__ volatile("nop;nop;rtpt")
#define rtps() __asm__ volatile("nop;nop;rtps")
#define stsxy3(a,b,c) __asm__ volatile("swc2 $12,0(%0);swc2 $13,0(%1);swc2 $14,0(%2)" : : "r"(a),"r"(b),"r"(c) : "memory")
#define stsxy(a) __asm__ volatile("swc2 $14,0(%0)" : : "r"(a) : "memory")
#define stsz(a) __asm__ volatile("swc2 $19,0(%0)" : : "r"(a) : "memory")
#define ldv0(r0) __asm__ volatile("lwc2 $0,0(%0);lwc2 $1,4(%0)" : : "r"(r0))

void ShowLight1(void *v1, unsigned char *col, short size, TD *texture) {
    SV vert[4];
    int z;
    PFT4 *poly;

    vert[2].vx = (-size * 3) >> 1;
    vert[0].vx = (-size * 3) >> 1;
    vert[3].vx = (size * 3) >> 1;
    vert[1].vx = (size * 3) >> 1;
    vert[3].vz = 0;
    vert[2].vz = 0;
    vert[1].vz = 0;
    vert[0].vz = 0;
    vert[1].vy = -size;
    vert[0].vy = -size;
    vert[3].vy = size;
    vert[2].vy = size;
    ldv3();
    rtpt();
    poly = (PFT4 *)current->primptr;
    poly->u0 = texture->u0;
    poly->v0 = texture->v0;
    poly->u1 = texture->u1;
    poly->v1 = texture->v1;
    poly->u2 = texture->u2;
    poly->v2 = texture->v2;
    poly->u3 = texture->u3;
    poly->v3 = texture->v3;
    ((unsigned char *)poly)[3] = 9;
    ((unsigned char *)poly)[7] = 0x2E;
    poly->r0 = col[0];
    poly->g0 = col[1];
    poly->b0 = col[2];
    stsxy3(&poly->x0, &poly->x1, &poly->x2);
    stsz(&z);
    if (z > 39) {
        ldv0(&vert[3]);
        rtps();
        poly->tpage = texture->tpageid | 0x20;
        poly->clut = texture->clutid;
        z = (z >> 3) + LightSortCorrect;
        if (z < 0) z = 0;
        stsxy(&poly->x3);
        poly->tag = (poly->tag & 0xff000000) | ((unsigned long)current->ot[z] & 0xffffff);
        current->ot[z] = (unsigned long *)(((unsigned long)current->ot[z] & 0xff000000) | ((unsigned long)poly & 0xffffff));
        current->primptr += 0x28;
    }
}
