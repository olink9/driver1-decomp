extern int D_001E3458[];
extern int D_001E3478[];
extern int func_000396A8(int key);

void func_001E2D70(int sel)
{
    int i;
    unsigned short *m;

    for (i = 0; D_001E3458[i] != 0; i++) {
        m = (unsigned short *)func_000396A8(D_001E3458[i]);
        if (m != 0) {
            if (i != sel)
                *m |= 0x10;
            else
                *m &= ~0x10;
        }
    }
    for (i = 0; D_001E3478[i] != 0; i++) {
        m = (unsigned short *)func_000396A8(D_001E3478[i]);
        if (m != 0) {
            if (i != sel)
                *m |= 0x10;
            else
                *m &= ~0x10;
        }
    }
}
