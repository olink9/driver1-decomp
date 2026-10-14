/* func_0006009C - no Driver 2 counterpart */
typedef struct {
    char pad0[0x3C];
    int flags;
    char pad40[4];
    short dir;
} OBJ_6009C;

extern int D_0009E714[];
extern int func_00021558(int x, int z);

void func_0006009C(OBJ_6009C *obj, int *pos)
{
    int x, z;
    int n, s, w, e;

    x = (pos[0] + D_0009E714[2]) / 1500 * 1500 + (750 - D_0009E714[2]);
    z = D_0009E714[3] - (D_0009E714[3] + (750 - pos[2])) / 1500 * 1500;

    n = (short)func_00021558(x, z - 1500);
    s = (short)func_00021558(x, z + 1500);
    w = (short)func_00021558(x - 1500, z);
    e = (short)func_00021558(x + 1500, z);

    switch (obj->dir) {
    case 1:
        if ((unsigned)n < 0x8000) {
            obj->flags |= 0x100;
            pos[2] = z - 0x226;
        } else {
            obj->flags &= ~0x100;
            pos[2] = z + 0x15E;
        }
        break;
    case 2:
        if ((unsigned)s < 0x8000) {
            obj->flags |= 0x100;
            pos[2] = z + 0x226;
        } else {
            obj->flags &= ~0x100;
            pos[2] = z - 0x15E;
        }
        break;
    case 4:
        if ((unsigned)e < 0x8000) {
            obj->flags |= 0x100;
            pos[0] = x + 0x226;
        } else {
            obj->flags &= ~0x100;
            pos[0] = x - 0x15E;
        }
        break;
    case 8:
        if ((unsigned)w < 0x8000) {
            obj->flags |= 0x100;
            pos[0] = x - 0x226;
        } else {
            obj->flags &= ~0x100;
            pos[0] = x + 0x15E;
        }
        break;
    }
}
