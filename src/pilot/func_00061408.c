typedef struct { int x, y, z; } V;
typedef struct { V pos; short pad; short d; char pad2[2]; } T;
extern int SquareRoot0();

void func_00061408(char *p, int *q)
{
    int a[3];
    int d;
    if (!(p[0x36] & 0x10)) {
        a[0] = q[0] - ((int *)p)[0];
        a[1] = q[1] - ((int *)p)[1];
        a[2] = q[2] - ((int *)p)[2];
        d = SquareRoot0(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
        *(short *)(p + 0x34) = (d << 12) / *(int *)(p + 0x30);
    }
}
