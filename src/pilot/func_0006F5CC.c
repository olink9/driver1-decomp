extern int D_00096F98;
extern short D_00096F9C;
extern int D_000977F8;
extern int D_00097800;
extern int D_00097808;
extern long long D_000C784C[];
extern void SsSetSerialVol();
extern void func_0007A3B0();
extern void func_0007A580();

void func_0006F5CC(int a, int b, int c)
{
    unsigned char buf[8];
    int pad[2];
    int out[2];

    if (D_00096F98 == 1) return;
    switch (a) {
    case 1:
        D_000977F8 = D_00097800;
        buf[1] = b;
        D_00096F9C = b & 0xff;
        break;
    case 8:
        D_000977F8 = *(int *)&D_000C784C[b];
        buf[1] = c;
        D_00096F9C = c & 0xff;
        break;
    case 4:
        D_000977F8 = D_00097800;
        D_00096F9C = (buf[1] = c + 4) & 0xff;
        break;
    case 0x10:
        D_000977F8 = D_00097808;
        buf[1] = c;
        D_00096F9C = c & 0xff;
        break;
    default:
        return;
    }
    buf[0] = 1;
    SsSetSerialVol(0, 0x7f, 0x7f);
    func_0007A3B0(0xD, buf, out);
    func_0007A580(D_000977F8, pad);
    func_0007A3B0(0x1B, pad, out);
    D_00096F98 = 1;
}
