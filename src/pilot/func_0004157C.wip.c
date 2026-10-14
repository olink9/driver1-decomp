typedef struct {
    int x, y, z;
    int pad;
    short a, b, c, pad2, d, e;
} PARTICLE;

extern PARTICLE D_000B7E1C[];
extern int D_00097420;
extern int func_000776E4();

void func_0004157C(int *pos, int n)
{
    PARTICLE *p = D_000B7E1C;
    int i, t;

    n++;
    if (n > 3) n = 4;
    if (p->d == 0) {
        D_00097420 = n;
        i = n;
        while (i--) {
            p->x = (pos[0] - 32) + (func_000776E4() & 0x3f);
            p->y = (pos[1] - 32) + (func_000776E4() & 0x3f);
            p->z = (pos[2] - 32) + (func_000776E4() & 0x3f);
            p->a = (func_000776E4() & 0x3f) - 32;
            p->b = -5 - (func_000776E4() & 0xf);
            p->c = (func_000776E4() & 0x3f) - 32;
            p->d = 60;
            p->e = 8;
            p++;
        }
    }
}
