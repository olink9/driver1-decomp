/* func_000287A4: draw a subdivided model's polygons (per-poly mode from a packed 3-bit stream). */
typedef struct { short x, y, z, pad; } SV;
typedef struct { unsigned short vx, vy; } DV;

typedef struct {
    unsigned short flags;      /* 0x00 */
    char pad2[0xE - 0x2];
    unsigned short num_polys;  /* 0x0E */
    SV *vertices;              /* 0x10 */
    int *polys;                /* 0x14 */
    char pad18[0x24 - 0x18];
    unsigned int modes;        /* 0x24 */
} MODEL;

extern DV *D_00096938;
extern int *D_0009693C;
extern int D_00089F1C[];

extern void func_00029448();
extern void func_00027A4C();
extern void func_00027E08();
extern void func_00027620();
extern void func_000277D4();
extern void func_00028340();
extern int printf(const char *, ...);

#define gte_ldsxy3(r0, r1, r2) __asm__ volatile ( \
    "mtc2 %0, $12;" "mtc2 %2, $14;" "mtc2 %1, $13" : : "r"(r0), "r"(r1), "r"(r2))
#define gte_nclip() __asm__ volatile ("nop;" "nop;" "nclip")
#define gte_stopz(r0) __asm__ volatile ("swc2 $24, 0(%0)" : : "r"(r0) : "memory")

void func_000287A4(MODEL *model)
{
    int opz;
    int *clut;
    int *poly;
    SV *verts;
    int uv0;
    int uv1;
    int tpage;
    unsigned int modes;
    int i;
    int *scratch;
    unsigned char mode;
    int idx;

    uv1 = uv0 = 0;
    clut = (int *)0x1F8001F0;
    mode = uv1;
    i = 0;
    scratch = (int *)0x1F800000;
    modes = model->modes;
    modes >>= 2;
    poly = model->polys;
    verts = model->vertices;
    for (i = 0; i < model->num_polys; i++) {
        if (mode != 2) {
            uv0 = poly[2];
            uv1 = poly[3];
            tpage = clut[0];
        } else {
            tpage = (((unsigned char *)poly)[1] & 1) + 1;
            tpage = clut[tpage];
        }
        idx = poly[1];
        switch (mode) {
        case 0:
            if (model->flags & 0x42)
                func_00029448(idx, uv0, uv1, verts);
            else
                func_00027A4C(idx, uv0, uv1, verts);
            if (*(int *)model & 0x40000080)
                func_00027E08(poly[0], tpage, 16, scratch);
            else
                func_00027E08(poly[0], tpage, 16, scratch + 1);
            break;
        case 1:
            func_00027A4C(idx, uv0, uv1, verts);
            func_00027E08(poly[0], tpage, 16, scratch + 2);
            break;
        case 2:
            func_00027620(idx, verts);
            func_00028340(tpage, 4, scratch + 2);
            break;
        case 3:
            func_000277D4(idx, uv0, uv1, verts);
            func_00027E08(poly[0], tpage, 4, scratch + 2);
            break;
        case 4:
            func_000277D4(idx, uv0, uv1, verts);
            opz = 0;
            if ((unsigned short)(D_00096938[0].vx + 0xFF) < 0x3FF)
                opz = 1;
            if ((unsigned short)(D_00096938[1].vx + 0xFF) < 0x3FF)
                opz++;
            if ((unsigned short)(D_00096938[2].vx + 0xFF) < 0x3FF)
                opz++;
            if (opz >= 2)
                gte_ldsxy3(D_0009693C[0], D_0009693C[1], D_0009693C[2]);
            else
                gte_ldsxy3(D_0009693C[7], D_0009693C[9], D_0009693C[8]);
            gte_nclip();
            gte_stopz(&opz);
            if (opz >= 0)
                func_00027E08(poly[0], tpage, 4, scratch + 2);
            break;
        default:
            printf("error!! = %d\n", mode);
            break;
        }
        mode = modes & 7;
        modes >>= 3;
        poly = (int *)((char *)poly + D_00089F1C[*(unsigned char *)poly]);
    }
}
