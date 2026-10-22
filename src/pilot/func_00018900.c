/* func_00018900 - matched run3-r4.
 * The mfhi t0 vs v0 leftover (spilled HI retried into the first free GPR) was fixed by declaring the
 * tail callee with its real return type: func_00016F0C returns int (tested in func_000181B4/000154A8).
 * The value-returning call_value form changes v0's liveness, so reload no longer picks v0 for HI. */
typedef struct {
    unsigned char id;      /* 0x00 */
    unsigned char active;  /* 0x01 */
    char pad02[6];
    unsigned char f08;     /* 0x08 */
    char pad09[14];
    unsigned char f17;     /* 0x17 */
    int f18;               /* 0x18 */
    char pad1C[8];
    int dir;               /* 0x24 */
    int junc;              /* 0x28 */
    char pad2C[16];
    int f3C;               /* 0x3C */
    int f40;               /* 0x40 */
    int f44;               /* 0x44 */
    int x;                 /* 0x48 */
    int z;                 /* 0x4C */
} CIV;

typedef struct {
    char pad[0xC8];
    int x;
    int y;
    int z;
    char pad2[396 - 0xD4];
} PLAYER;

#define ABSD(a, b) ((a) - (b) < 0 ? (b) - (a) : (a) - (b))
extern CIV D_00098950[];
extern PLAYER D_0009B230[];
extern void func_00014FC4();
extern void func_00021820();
extern int func_00016F0C();

void func_00018900(CIV *cp)
{
    int pos[2];
    int tmp[6];
    int i;
    int px;
    CIV *c;

    c = D_00098950;

    func_00014FC4(cp->junc, cp->dir, 0, 2, 0xfe, pos);
    cp->x = D_0009B230[cp->id].x / 1500;
    px = pos[0] / 1500;
    cp->z = D_0009B230[cp->id].z / 1500;

    if (cp->dir & 1) {
        for (i = 0; i < 22; i++, c++) {
            if (cp->id != c->id && c->active) {
                if (cp->dir == 1) {
                    if (cp->junc == c->junc && c->z == cp->z
                        && c->x >= cp->x && ABSD(c->x, cp->x) < 5)
                        return;
                } else {
                    if (cp->junc == c->junc && c->z == cp->z
                        && cp->x >= c->x && ABSD(cp->x, c->x) < 5)
                        return;
                }
            }
        }
    } else {
        for (i = 0; i < 22; i++, c++) {
            if (cp->id != c->id && c->active) {
                if (cp->dir == 0) {
                    if (cp->junc == c->junc && c->x == px
                        && c->z >= cp->z && ABSD(c->z, cp->z) < 5)
                        return;
                } else {
                    if (cp->junc == c->junc && c->x == px
                        && cp->z >= c->z && ABSD(cp->z, c->z) < 5)
                        return;
                }
            }
        }
    }

    if (cp->dir & 1) {
        cp->f40 = pos[0];
        cp->f44 = 0x7fffffff;
    } else {
        cp->f40 = 0x7fffffff;
        cp->f44 = pos[1];
    }
    cp->f17 = 0x23;
    cp->active = 2;
    cp->f08 = 1;
    cp->f3C = 0x82;
    func_00021820(D_0009B230[cp->id].x, D_0009B230[cp->id].z, cp->dir, tmp);
    cp->f18 = tmp[0];
    func_00016F0C(cp, tmp);
}
