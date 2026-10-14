typedef struct { short vx, vy, vz, pad; } SVEC;
typedef struct { short vx, vy; } DVEC;
typedef struct {
    char pad[0xC];
    unsigned short num_vertices;
    unsigned short num_polys;
    SVEC *vertices;
    unsigned char *polys;
} MODEL;
extern int D_000974E8;
extern unsigned char D_0008AA8C[];
extern int D_00089F1C[];
extern int RotTransPers3();
extern void func_0004F34C();

int func_0004F7C8(MODEL *model, int base)
{
    char pad[0x60];
    long p;
    long flag;
    DVEC *sxy;
    short *depth;
    SVEC *verts;
    unsigned char *poly;
    int i;
    int y;
    int result;
    unsigned int idx;
    int z;

    sxy = (DVEC *)0x1F800044;
    depth = (short *)0x1F800240;
    verts = model->vertices;
    for (i = 0; i < model->num_vertices + 3; i += 3) {
        z = 0x1080 - RotTransPers3(&verts[i], &verts[i + 1], &verts[i + 2],
                                   &sxy[i], &sxy[i + 1], &sxy[i + 2], &p, &flag);
        depth[i] = depth[i + 1] = depth[i + 2] = z;
    }
    idx = 0x33ADF3;
    if (sxy[11].vy < sxy[10].vy || depth[10] < 0) {
        D_000974E8 = 0;
        result = -1;
    } else {
        result = 0x100;
        D_000974E8 = 1;
    }
    if (depth[10] < 0)
        return result;
    if (D_000974E8 == 0) {
        for (i = 4; i >= 0; i--) {
            y = sxy[idx & 0x1F].vy;
            if (result < y)
                result = y;
            idx >>= 5;
        }
    } else {
        for (i = 4; i >= 0; i--) {
            y = sxy[idx & 0x1F].vy;
            if (y < result)
                result = y;
            idx >>= 5;
        }
    }
    poly = model->polys;
    *(MODEL **)0x1F800018 = model;
    *(short **)0x1F80001C = depth;
    for (i = 0; i < model->num_polys; i++) {
        *(unsigned char **)0x1F80003C = poly;
        func_0004F34C(D_0008AA8C[base + i]);
        poly += D_00089F1C[*poly];
    }
    return result;
}
