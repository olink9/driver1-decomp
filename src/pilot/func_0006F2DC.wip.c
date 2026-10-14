typedef struct { char b[24]; } CdlFILE;
extern int D_0008D1F4[];
extern int D_000C784C[][2];
extern char D_0001170C[];
extern char D_00011724[];
extern int D_00097800;
extern int D_00097808;
extern int CdSearchFile();
extern int func_0007A684();

void func_0006F2DC(unsigned char flags, int idx)
{
    CdlFILE f;
    char pad[24];
    int i;

    if (flags == 0) {
        for (i = 0; i < 5; i++) {
            CdSearchFile(&f, D_0008D1F4[i]);
            D_000C784C[i][0] = func_0007A684(&f);
        }
        CdSearchFile(&f, D_0001170C);
        D_00097800 = func_0007A684(&f);
        CdSearchFile(&f, D_00011724);
        D_00097808 = func_0007A684(&f);
        return;
    }
    if (flags & 8) {
        if (idx == -1) {
            for (i = 0; i < 5; i++) {
                CdSearchFile(&f, D_0008D1F4[i]);
                D_000C784C[i][0] = func_0007A684(&f);
            }
        } else {
            CdSearchFile(&f, D_0008D1F4[idx]);
            D_000C784C[idx][0] = func_0007A684(&f);
        }
    }
    if ((flags & 1) || (flags & 4)) {
        CdSearchFile(&f, D_0001170C);
        D_00097800 = func_0007A684(&f);
    }
    if (flags & 0x10) {
        CdSearchFile(&f, D_00011724);
        D_00097808 = func_0007A684(&f);
    }
}
