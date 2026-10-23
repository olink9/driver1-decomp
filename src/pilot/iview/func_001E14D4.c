typedef struct { char *primptr; unsigned int *ot; } DB;
typedef struct { unsigned addr:24; unsigned len:8; } P_TAG;
typedef struct {
    unsigned char u0, v0, u1, v1, u2, v2, u3, v3;
    unsigned short tpageid;
    unsigned short clutid;
    char pad[4];
} TEXTURE_DETAILS;
typedef struct {
    unsigned int tag;
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


extern DB *current;
extern TEXTURE_DETAILS D_000B91F0;
extern unsigned char D_001E35A0[];
extern int D_001E335C;
extern int D_001E3588;
extern unsigned char cameraview;
extern unsigned char tracking_car;
extern int func_001E1B70(int i);
extern void func_001E1A14(int i);
extern void func_001E1BCC(void);

void func_001E14D4(void)
{
    POLY_FT4 *poly;
    int i, cnt, k;

    k = 0;
    poly = (POLY_FT4 *)current->primptr;
    poly->x0 = 0x17C;
    poly->y0 = 10;
    poly->x1 = 0x1FC;
    poly->y1 = 10;
    poly->x2 = 0x17C;
    poly->y2 = 0x83;
    poly->x3 = 0x1FC;
    poly->y3 = 0x83;
    poly->u0 = D_000B91F0.u0;
    poly->v0 = D_000B91F0.v0;
    poly->u1 = D_000B91F0.u1;
    poly->v1 = D_000B91F0.v1;
    poly->u2 = D_000B91F0.u2;
    poly->v2 = D_000B91F0.v2;
    poly->u3 = D_000B91F0.u3;
    poly->v3 = D_000B91F0.v3;
    ((P_TAG *)poly)->len = 9;
    poly->code = 0x2C;
    poly->r0 = 100;
    poly->g0 = 100;
    poly->b0 = 100;
    poly->tpage = D_000B91F0.tpageid | 0x20;
    poly->clut = D_000B91F0.clutid;
    ((P_TAG *)poly)->addr = ((P_TAG *)&current->ot[3])->addr;
    ((P_TAG *)&current->ot[3])->addr = (unsigned int)poly;
    current->primptr += sizeof(POLY_FT4);

    cnt = 0;
    for (i = 0; i < 9; i++) {
        if (D_001E35A0[i]) {
            if (k == 0)
                k = func_001E1B70(i);
            if (D_001E35A0[i] >= 2)
                func_001E1A14(i);
            cnt++;
        }
    }
    func_001E1BCC();
    if (cnt == 9 && D_001E335C != 2) {
        D_001E3588 = 0;
        D_001E335C = 2;
        cameraview = 0;
        tracking_car = 1;
    }
}
