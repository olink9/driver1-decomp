extern int D_00086EC0[];
extern int D_0008AAFC[];
extern long long D_00097520;
typedef struct { int a, b; } P2;
extern P2 D_000BE0A4[8][4];
extern char D_00010910[];
extern void CdInit();
extern int CdSearchFile();
extern int sprintf();
extern int CdPosToInt();
extern void func_0007A580();
extern void CdControl();
extern void CdRead();
extern int CdReadSync();

void func_000503F4(void)
{
    char name[0x88];
    int sz[8];
    char file[0x18];
    int i, j;
    int *base;

    base = (int *)0xD0208;
    CdInit();
    for (i = 0; i < 8; i++) {
        sz[i] = 0;
        sprintf(name, D_00010910, D_00086EC0[i]);
        if (CdSearchFile(file, name) != 0)
            sz[i] = CdPosToInt(file);
    }
    for (i = 0; i < 8; i++) {
        if (sz[i] != 0) {
            func_0007A580(sz[i], file);
            CdControl(2, file, 0);
            CdRead(1, 0xD0200, 0x80);
            while (CdReadSync(0, name + 0x80) != 0)
                ;
            for (j = 0; j < 4; j++) {
                D_000BE0A4[i][j].a = base[j * 2] + (sz[i] << 11);
                D_000BE0A4[i][j].b = base[j * 2 + 1];
            }
        }
    }
    for (i = 0; i < 1; i++) {
        sz[i] = 0;
        sprintf(name, D_00010910, D_0008AAFC[i]);
        if (CdSearchFile(file, name) != 0)
            ((int *)&D_00097520)[i] = CdPosToInt(file);
    }
}
