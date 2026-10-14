extern int D_00086EC0[];
extern int D_0008AAFC[];
extern long long D_00097520;
extern int D_000BE0A4[];
extern char D_00010910[];
extern void CdInit();
extern int CdSearchFile();
extern int sprintf();
extern int func_0007A684();
extern void func_0007A580();
extern void func_0007A3B0();
extern void func_0007DCBC();
extern int func_0007DE58();

void func_000503F4(void)
{
    char name[0x88];
    int sz[8];
    char file[0x18];
    int i, j;
    int *src;
    int *dst;
    int *p;
    int k;
    int *base;

    base = (int *)0xD0208;
    CdInit();
    for (i = 0; i < 8; i++) {
        sz[i] = 0;
        sprintf(name, D_00010910, D_00086EC0[i]);
        if (CdSearchFile(file, name) != 0)
            sz[i] = func_0007A684(file);
    }
    for (i = 0; i < 8; i++) {
        if (sz[i] != 0) {
            func_0007A580(sz[i], file);
            func_0007A3B0(2, file, 0);
            func_0007DCBC(1, 0xD0200, 0x80);
            while (func_0007DE58(0, name + 0x80) != 0)
                ;
            src = base;
            k = i * 32;
            for (j = 0; j < 4; j++) {
                dst = (int *)((char *)D_000BE0A4 + k);
                k += 8;
                dst[0] = src[0] + (sz[i] << 11);
                dst[1] = src[1];
                src += 2;
            }
        }
    }
    for (i = 0, p = D_0008AAFC; i < 1; i++, p++) {
        sz[i] = 0;
        sprintf(name, D_00010910, *p);
        if (CdSearchFile(file, name) != 0)
            ((int *)&D_00097520)[i] = func_0007A684(file);
    }
}
