extern int D_0009762C;
extern char D_00097658;
extern long long D_00096897;
extern long long D_000970D8;
extern int *D_00097668;
extern int D_00097620;
extern short *D_0009765C;
extern int D_0008A8F0[3];
extern int D_0008A8F4[];
extern int D_0009B0A4[];

void func_0005C4C8(void)
{
    int v[3];
    if (D_0009762C == 90) {
        D_00097658 = 1;
        *(char *)&D_00096897 = 1;
        if (*(int *)&D_000970D8 + D_00097620 < *D_00097668) {
            D_0008A8F0[0] = D_0009765C[0] << 10;
            v[0] = D_0008A8F0[0];
            D_0008A8F0[2] = D_0009765C[1] << 10;
            v[2] = D_0008A8F0[2];
        } else {
            D_0008A8F0[0] = D_0009B0A4[0x917];
            v[0] = D_0008A8F0[0];
            D_0008A8F0[2] = D_0009B0A4[0x919];
            v[2] = D_0008A8F0[2];
        }
        D_0008A8F4[0] = -1000;
    }
    if (D_0009762C == 0) {
        D_0009762C = D_00097620;
        D_0009765C += 2;
    } else {
        D_0009762C = D_0009762C - 1;
    }
}
