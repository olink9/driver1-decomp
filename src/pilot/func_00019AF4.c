typedef struct {
    int x;
    int y;
    int z;
    char pad0C[0x18 - 0x0C];
    unsigned char f18;
    char pad19;
    short f1A;
    short f1C;
    short f1E;
    char pad20[4];
    unsigned char f24;
    unsigned char f25;
    unsigned char count;
    unsigned char dir;
    unsigned char f28;
    unsigned char ids[1];
} OBJ;
typedef struct {
    char f0;
    unsigned char f1;
    char pad2[3];
    unsigned char f5;
    char pad6[2];
    unsigned char f8;
    char pad9[80 - 9];
} ENT;
typedef struct { int a; int b; int c; int d; int e; } NODE;
typedef struct { char pad[1960]; } SND;
extern ENT D_00098950[];
extern SND D_000C4690[];
extern unsigned char D_00099AF0[];
extern int D_00099120[];
extern int D_0009677C, D_00096FD0, D_00096FF0;
extern unsigned char D_00096FC4;
extern volatile long long CameraCnt;
extern long long D_0009704C;
extern int func_00019DAC();
extern void func_000197E0(), func_00063AB4(), func_0001A008();

int func_00019AF4(OBJ *p)
{
    int x;
    int z;
    int node;
    unsigned char dir;
    unsigned char *ptr;
    unsigned char *end;
    int id;
    ENT *e;
    int snd;
    NODE *n;

    x = p->x;
    z = p->z;
    node = p->f1C;
    dir = p->dir;
    if (func_00019DAC(p, x, z) == 0)
        return 0;
    ptr = p->ids;
    end = p->ids + p->count;
    while (ptr < end) {
        id = *ptr;
        e = &D_00098950[id];
        snd = e->f5;
        if (e->f1 != 0 && D_0009677C != 0) {
            if (*(volatile int *)&CameraCnt != D_00096FD0 || id != D_00096FF0) {
                D_00096FF0 = id;
                D_00096FD0 = *(volatile int *)&CameraCnt;
            }
        }
        if (snd != 0)
            func_00063AB4(&D_000C4690[e->f5]);
        if (e->f1 == 9)
            func_0001A008(D_00099120);
        e->f1 = 0;
        e->f8 = 0;
        ptr++;
        D_00099AF0[--D_00096FC4] = id;
    }
    func_000197E0(x, z, p->f1A, dir, p->f24, 0);
    p->f28 = 2;
    p->f1E = 450;
    if (node != -1) {
        n = &(*(NODE **)&D_0009704C)[node];
        switch (dir) {
        case 0:
        case 2:
            x = n->a * 1500;
            break;
        case 1:
        case 3:
            z = -(n->d * 1500);
            break;
        }
        func_000197E0(x, z, node, dir, p->f25 + p->f18, p->f18);
    }
    return 1;
}
