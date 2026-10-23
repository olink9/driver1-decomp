/* Driver 2: LoadfileSeg */
extern char D_0008AB0C[];      /* currentfilename */
extern char D_00096CC8[];      /* ";1" */
extern char D_000BE1A4[];      /* currentfileinfo (CdlFILE) */
extern volatile int D_00097534;   /* volatile: assert loop reloads it each pass (fixes the v0/v1 swap) */
typedef struct { char c[9]; } S9 __attribute__((aligned(4)));
extern S9 D_00010920;          /* "\\DRIVER\\" */
extern char *func_0007E8EC();  /* strcat */
extern int func_0007B228();    /* strcmp */
extern char *strcpy();
extern int CdSearchFile();
extern int CdPosToInt();
extern void func_0007A580();   /* CdIntToPos */
extern int CdControl();
extern int CdRead();
extern int CdReadSync();
int printf(const char *fmt, ...);

int LoadfileSeg(char *name, char *addr, int offset, int loadsize)
{
    char namebuffer[128];
    unsigned char result[8];
    unsigned char sectorbuffer[2048];
    char info[24];
    char pos[8];
    int sector, nsectors, toload, first, i, end, r;

    *(S9 *)namebuffer = D_00010920;
    func_0007E8EC(namebuffer, name);
    func_0007E8EC(namebuffer, D_00096CC8);
    if (func_0007B228(D_0008AB0C, namebuffer) != 0) {
        strcpy(D_0008AB0C, namebuffer);
        r = CdSearchFile(D_000BE1A4, namebuffer);
        if (r == 0 || r == -1) {
            printf("can't load %s\n", namebuffer);
            while (D_00097534 != 0x78654321)
                __asm__ volatile("break 1024");
        }
    }
    sector = offset / 2048;
    sector += CdPosToInt(D_000BE1A4);
    toload = loadsize;
    if (offset & 0x7ff) {
        func_0007A580(sector, pos);
        CdControl(2, pos, 0);
        CdRead(1, sectorbuffer, 0x80);
        while (CdReadSync(0, result) != 0)
            ;
        i = offset & 0x7ff;
        first = 2048 - i;
        if (toload <= first) {
            end = i + toload;
            for (; i < end; i++)
                *addr++ = sectorbuffer[i];
            return loadsize;
        }
        for (; i < 2048; i++)
            *addr++ = sectorbuffer[i];
        toload -= first;
        sector++;
    }
    nsectors = toload / 2048;
    if (nsectors != 0) {
        if ((int)addr & 3) {
            printf("\naddress invalid %s \n", namebuffer);
            while (D_00097534 != 0x78654321)
                __asm__ volatile("break 1024");
        }
        func_0007A580(sector, pos);
        CdControl(2, pos, 0);
        CdRead(nsectors, addr, 0x80);
        while (CdReadSync(0, result) != 0)
            ;
        addr += nsectors * 2048;
        toload -= nsectors * 2048;
        sector += nsectors;
    }
    if (toload > 0) {
        func_0007A580(sector, pos);
        CdControl(2, pos, 0);
        CdRead(1, sectorbuffer, 0x80);
        while (CdReadSync(0, result) != 0)
            ;
        for (i = 0; i < toload; i++)
            *addr++ = sectorbuffer[i];
    }
    return loadsize;
}
