typedef struct { int x, y, z, w; } V;
typedef struct { int x, y, z; } O;
typedef struct { O *obj; unsigned char n; char pad[3]; } LS;
extern int D_0008A8F0[];
extern int D_00097480;
extern int D_00097464;
extern LS D_000B8190[];
extern int D_000B9170[];
extern int D_000B9230[];
extern int func_000776E4();
extern int func_0001F5B0();
extern int func_0004315C();
extern void func_0004449C();
extern void func_0004428C();
extern void func_0004A398();

void func_00042C8C(O *o, int a1, int a2, int a3, int big)
{
    V v1, v2, v3;
    unsigned char c1[4] = { 140, 140, 140, 0 };
    unsigned char c2[4] = { 35, 35, 35, 0 };
    int size;
    int i;
    int h;
    unsigned char c;
    int id;
    LS *p;

    p = D_000B8190;
    id = (int)o + a1;
    v1.x = o->x - D_0008A8F0[0] + a1;
    v1.z = o->z - D_0008A8F0[2] + a3;
    size = 100;
    if (big == 0)
        size = 50;
    for (i = 0; i < 4; i++, p++) {
        if (o == p->obj) {
            if (p->n >= 3)
                return;
            c = (func_000776E4() & 0x3F) - (p->n * 32 - 90);
            c1[0] = c1[1] = c1[2] = c;
            c2[0] = c2[1] = c2[2] = c >> 1;
            break;
        }
    }
    v1.y = o->y - D_0008A8F0[1] + a2;
    v2.x = v1.x;
    v2.z = v1.z;
    h = func_0001F5B0(&v1);
    c1[3] = 0;
    D_00097480 = -30;
    v2.y = -h - D_0008A8F0[1];
    v3 = v1;
    D_00097464 = func_0004315C(id);
    if (D_00097464 >= 0)
        c1[3] = 0x60;
    func_0004449C(&v3, c1, size, D_000B9170);
    func_0004428C(&v3, c2, size * 3, D_000B9170);
    func_0004A398(&v2, c2, (short)size * 2, D_000B9230);
    D_00097480 = -10;
}
