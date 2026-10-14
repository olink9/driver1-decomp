typedef struct { char pad[10]; unsigned char f; char pad2[1]; } E12;
extern int D_000976C0;
extern E12 *D_000976CC;
extern int D_000C0340[];
extern int func_0005FBE0();
extern void func_0006009C();
extern void func_00060328();
extern void func_000604F8();
extern void func_0005D974();

void func_0005FFB0(char *p)
{
    int r;
    char *q = p + 0x28;

    r = func_0005FBE0(0, *(int *)p, *(int *)(p + 8), q);
    if (r == 0) {
        int n = D_000976C0;
        int *arr = D_000C0340;
        int s = *(short *)(p + 0x42);
        *(int *)(p + 0x3C) = 0;
        *(short *)(p + 0x1C) = 0;
        *(short *)(p + 0x1A) = 0;
        *(short *)(p + 0x18) = 0;
        n--;
        D_000976C0 = n;
        arr[n] = s;
        if (*(unsigned char *)(p + 0x56) == 6) {
            E12 *e = &D_000976CC[*(unsigned char *)(p + 0x53)];
            if (e->f != 0)
                e->f = 0;
        }
    } else {
        *(short *)(p + 0x44) = r;
        func_0006009C(p, p);
        func_00060328(p, q);
        func_000604F8(p);
        func_0005D974(p);
    }
}
