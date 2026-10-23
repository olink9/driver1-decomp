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
} POLY_FT3;
typedef struct { unsigned short x, y, z; unsigned short uv; } DST;
typedef struct { char *primptr; P_TAG *ot; } DB;
extern DB *current;
extern DST *D_001CF318;
extern int *D_001CF31C;
extern unsigned int *D_001CF324;
extern int D_001CF304, D_001D3280, D_001D3284;
extern int func_001CBE30(), func_001CBEFC();

void func_001CBAA4(unsigned char *p)
{
    POLY_FT3 *t;
    int z;

    if (func_001CBE30(p) != 0)
        return;
    if (func_001CBEFC(3) != 0)
        return;
    t = (POLY_FT3 *)current->primptr;
    ((P_TAG *)t)->len = 7;
    t->code = 0x24;
    t->r0 = 0x80;
    t->g0 = 0x80;
    t->b0 = 0x80;
    *(long *)&t->x0 = D_001CF31C[0];
    *(long *)&t->x1 = D_001CF31C[1];
    *(long *)&t->x2 = D_001CF31C[2];
    *(long *)&t->u0 = D_001CF318[p[0]].uv | D_001D3284;
    *(long *)&t->u1 = D_001CF318[p[1]].uv | D_001D3280;
    *(long *)&t->u2 = D_001CF318[p[2]].uv;
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
    if (z < 8)
        z = 8;
    else if (z >= 0x1000)
        z = 0xFFF;
    ((P_TAG *)t)->addr = current->ot[z].addr;
    current->ot[z].addr = (unsigned)t;
    current->primptr += sizeof(POLY_FT3);
}
