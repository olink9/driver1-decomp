typedef struct { int state; char pad[0x34]; } E_8671C;
extern E_8671C D_0008671C[];
extern int D_000967D8, D_000967F4, D_00096800, D_00096808, D_0009681C, D_00096820, D_00096834;
extern int D_0009D8DC[];
extern int D_001CDE5C, D_001D2E48, D_001D2E50;
extern volatile unsigned short D_00097538;
extern void func_000514E0(), func_00024754(), func_001C2114(), func_001C19C4(), func_001C11E4(), func_001C753C();

int func_001C70F0(void)
{
    unsigned char found = 0;
    int i;
    int old;
    int ret;
    unsigned short pad;

    D_001D2E50 = 0;
    D_001D2E48 = 0;
    D_0008671C[6].state = -1;
    for (i = 0; i < D_000967D8; i++) {
        if (D_0008671C[i].state == -2) {
            if (!found) {
                found = 1;
                D_001D2E48 = i;
            }
            D_001D2E50++;
        }
    }
    D_001CDE5C = 6;
    func_001C2114(0, 0, 1);
    for (;;) {
        func_000514E0();
        if (D_00097538 & 0x4000) {
            old = D_001D2E48;
            if (++D_001D2E48 >= 8)
                D_001D2E48 = 0;
            while (D_0008671C[D_001D2E48].state != -2) {
                if (old == D_001D2E48)
                    break;
                if (++D_001D2E48 >= 8)
                    D_001D2E48 = 0;
            }
        } else if (D_00097538 & 0x1000) {
            old = D_001D2E48;
            if (--D_001D2E48 < 0)
                D_001D2E48 = 7;
            while (D_0008671C[D_001D2E48].state != -2) {
                if (old == D_001D2E48)
                    break;
                if (--D_001D2E48 < 0)
                    D_001D2E48 = 7;
            }
        }
        pad = D_00097538;
        if (pad & 0x40) {
            if (D_001D2E50 > 0) {
                switch (D_001D2E48) {
                case 0:
                    D_00096820 = 0;
                    D_0009681C = 0;
                    D_00096834 = 7;
                    D_00096800 = 1;
                    D_00096808 = 5;
                    D_0009D8DC[0] = 0;
                    D_0009D8DC[1] = 0;
                    D_0009D8DC[2] = 0;
                    D_0009D8DC[3] = 0;
                    D_0009D8DC[4] = 0;
                    D_000967F4 = 7;
                    func_00024754();
                    D_001D2E50 = 0;
                    for (i = 0; i < D_000967D8; i++) {
                        if (D_0008671C[i].state == -2) {
                            if (!found) {
                                found = 1;
                                D_001D2E48 = i;
                            }
                            D_001D2E50++;
                        }
                    }
                    break;
                case 3:
                    D_0009D8DC[0] ^= 1;
                    break;
                case 1:
                    D_0009681C ^= 1;
                    break;
                case 2:
                    D_00096820 ^= 1;
                    break;
                case 4:
                    D_0009D8DC[1] ^= 1;
                    break;
                case 5:
                    D_0009D8DC[2] ^= 1;
                    break;
                case 7:
                    D_0009D8DC[4] ^= 1;
                    break;
                }
            }
        } else if (pad & 0x10) {
            ret = -1;
            break;
        }
        func_001C19C4();
        func_001C11E4();
        func_001C753C(0, 50);
    }
    D_001CDE5C = 1;
    return ret;
}
