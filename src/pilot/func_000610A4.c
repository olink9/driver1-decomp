extern char D_0008EAA8[];
#define SN(i) (*(short *)(S + ((i) & 0xfff) * 4))
#define CS(i) (*(short *)(C + ((i) & 0xfff) * 4))
extern short func_00060AD4();

typedef struct {
    int x; int p1; int z; char pad0[0x0E]; short rot; char pad1[4]; short vx; char pad2[2]; short vz;
    char pad3[0x2A]; unsigned char sp; signed char cnt;
} CarX;

void func_000610A4(CarX *c)
{
    int t = c->sp;
    char *S = D_0008EAA8;
    int s = t + 7;
    char *C = D_0008EAA8 + 2;
    while (1) {
        int a = SN(c->rot + 0x800) * 125 >> 9;
        int b = CS(c->rot + 0x800) * 125 >> 9;
        if (func_00060AD4(c->x + a, c->z + b) == 0) {
            c->rot += 0x80;
            c->cnt++;
            if (c->cnt >= 0x20) return;
        } else {
            c->x -= c->vx;
            c->z -= c->vz;
            c->vx = s * SN(c->rot + 0x800) >> 12;
            c->vz = s * CS(c->rot + 0x800) >> 12;
            c->cnt = 0x20;
            return;
        }
    }
}
