typedef struct { short vx, vy, vz, pad; } SVECTOR;
extern SVECTOR *D_001CF318;
extern int *D_001CF31C, *D_001CF324;
extern int D_001D3290;

#define gte_ldv3(r0, r1, r2) __asm__ volatile ( \
    "lwc2  $0, 0( %0 );" "lwc2  $1, 4( %0 );" "lwc2  $2, 0( %1 );" \
    "lwc2  $3, 4( %1 );" "lwc2  $4, 0( %2 );" "lwc2  $5, 4( %2 )" : : "r"(r0), "r"(r1), "r"(r2))
#define gte_rtpt() __asm__ volatile ("nop;" "nop;" "rtpt")
#define gte_nclip() __asm__ volatile ("nop;" "nop;" "nclip")
#define gte_stopz(r0) __asm__ volatile ("swc2  $24, 0( %0 )" : : "r"(r0) : "memory")
#define gte_stsxy3c(r0) __asm__ volatile ( \
    "swc2  $12, 0( %0 );" "swc2  $13, 4( %0 );" "swc2  $14, 8( %0 )" : : "r"(r0) : "memory")
#define gte_stsz3c(r0) __asm__ volatile ( \
    "swc2  $17, 0( %0 );" "swc2  $18, 4( %0 );" "swc2  $19, 8( %0 )" : : "r"(r0) : "memory")

int func_001CBE30(unsigned char *p)
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
