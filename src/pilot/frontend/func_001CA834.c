typedef struct { int a, b, c, d; } E_32B0;
extern E_32B0 D_001D32B0[];
extern int D_001CEF88, D_001CF09C, D_001CF0A0, D_001CF288, D_001CF28C;
extern unsigned short D_00097538;
extern void func_001CA270(), func_001C9A18(), func_001CAC70(), func_001CACE0();

int func_001CA834(void)
{
    unsigned short pad;

    if (D_001CEF88 == 0) {
        pad = D_00097538;
        if (pad & 0x10) {
            func_001CA270(0, 0, 0, 0x40);
            func_001C9A18(4);
            return -1;
        } else if (pad & 0x8000) {
            if (--D_001CF09C < 0)
                D_001CF09C = 3;
            func_001CAC70();
        } else if (pad & 0x2000) {
            if (++D_001CF09C >= 4)
                D_001CF09C = 0;
            func_001CAC70();
        } else if (pad & 0x40) {
            switch (D_001CF09C) {
            case 0:
                if (--D_001CF0A0 < 0)
                    D_001CF0A0 = D_001CF288 - 1;
                func_001CACE0(D_001D32B0[D_001CF0A0].a);
                break;
            case 1:
                func_001CACE0(D_001D32B0[D_001CF0A0].a);
                break;
            case 2:
                if (++D_001CF0A0 >= D_001CF288)
                    D_001CF0A0 = 0;
                func_001CACE0(D_001D32B0[D_001CF0A0].a);
                break;
            case 3:
                if (D_001D32B0[D_001CF0A0].b) {
                    D_001CF28C = D_001CF0A0;
                    return 1;
                }
                break;
            default:
                D_001CF09C = 0;
                break;
            }
        }
    }
    return 0;
}
