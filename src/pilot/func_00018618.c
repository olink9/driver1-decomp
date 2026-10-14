/* civ car: try to start a parked car / pick spawn spot (unnamed) */
typedef struct {
    unsigned char id;
    unsigned char active;
    char pad2[7];
    unsigned char c9;
    char padA[0x17 - 0xA];
    unsigned char flags;
    int f18;
    int f1C;
    char pad20[4];
    int dir;
    int road;
    char pad2C[0x40 - 0x2C];
    int f40;
    int f44;
    int x;
    int z;
} CIV;
typedef struct { char pad[0xA]; unsigned char cA; char padB[5]; } ROAD;
typedef struct { int x, z; } POS2;

extern CIV D_00098950[];
extern long long D_00097044;
extern int func_0004FFE8();
extern void func_00014FC4();

#define DIFF(a, b) ((a) >= (b) ? ABS((a) - (b)) : ABS((b) - (a)))
#define ABS(x) ((x) < 0 ? -(x) : (x))

void func_00018618(CIV *cp)
{
    CIV *c;
    POS2 pos;
    int i;
    int cx, cz;

    c = D_00098950;
    if (cp->flags & 9)
        return;
    if ((func_0004FFE8(cp->id) & 0x1F) != cp->id)
        return;
    if ((*(ROAD **)&D_00097044)[cp->road].cA == 0)
        return;
    if (cp->c9 != 0)
        return;
    if (cp->f18 < 0x4650)
        return;
    if (cp->f1C < 0xBB8)
        return;
    func_00014FC4(cp->road, cp->dir, 0, 1, 0, &pos);
    cx = pos.x / 1500;
    cz = pos.z / 1500;
    if (cp->dir & 1) {
        for (i = 0; i < 22; i++, c++) {
            if (c->active && cp->id != c->id && c->z == cz) {
                if (DIFF(c->x, cp->x) < 8)
                    return;
            }
        }
    } else {
        for (i = 0; i < 22; i++, c++) {
            if (c->active && cp->id != c->id && c->x == cx) {
                if (DIFF(c->z, cp->z) < 8)
                    return;
            }
        }
    }
    switch (cp->dir) {
    case 0:
        cp->f40 = pos.x + 0x1EE;
        break;
    case 2:
        cp->f40 = pos.x - 0x1EE;
        break;
    case 1:
        cp->f44 = pos.z - 0x1BE;
        break;
    case 3:
        cp->f44 = pos.z + 0x1BE;
        break;
    }
    cp->flags |= 0x11;
}
