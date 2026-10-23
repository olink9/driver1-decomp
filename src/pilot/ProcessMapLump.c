/* FAKEMATCH: empty __asm__ used as a compiler hint (no instruction emitted) */
/* Driver 2: ProcessMapLump (approx.) */
extern long long D_0009708C;
extern long long D_000973A8;
extern long long D_0009735C;
extern long long D_00097364;
extern long long D_00096A24;
extern long long D_00096A28;
extern long long D_00096A2C;
extern long long D_00097384;
extern long long D_00097348;
extern long long D_00097304;
extern long long D_0009733C;
extern long long D_00097394;
extern long long D_0009736C;
extern long long D_00097398;
extern long long D_00097368;
extern long long D_0009737C;
extern long long D_00097374;
extern long long D_00097350;
extern long long D_00097108;
extern long long D_00097534;

int printf(const char *fmt, ...);
extern void func_00039748();
extern void func_00079DB0();

void ProcessMapLump(char *lump)
{
    int *p;
    int cells_across, cells_down, cellsize, num_regions, region_size;
    int regions_across, regions_down, half_across, half_down, f14, f18;

    p = (int *)lump;
    __asm__("" : "=r"(p) : "0"(p));   /* forces the separate copy of lump the target keeps in v1 */
    cells_across = p[0];
    region_size = p[4];
    regions_across = cells_across / region_size;
    cells_down = p[1];
    regions_down = cells_down / region_size;
    cellsize = p[2];
    *(char **)&D_0009708C = lump;
    half_across = cells_across / 2;
    f14 = p[5];
    f18 = p[6];
    num_regions = p[3];
    half_down = cells_down / 2;
    *(int *)&D_0009735C = 9;
    *(int *)&D_00097364 = 0x13;
    *(int *)&D_000973A8 = 0x169;
    *(int *)&D_00096A24 = cells_across;
    *(int *)&D_00096A28 = cells_down;
    *(int *)&D_00096A2C = cellsize;
    *(int *)&D_00097384 = f14;
    *(int *)&D_00097348 = f18;
    *(int *)&D_00097304 = num_regions;
    *(int *)&D_0009733C = region_size;
    *(int *)&D_00097394 = half_across;
    *(int *)&D_0009736C = half_down;
    *(int *)&D_00097398 = half_across * cellsize;
    *(int *)&D_00097368 = half_down * cellsize;
    *(int *)&D_0009737C = regions_across;
    *(int *)&D_00097374 = regions_down - 1;
    lump += 0x2c;

    if (num_regions > 350) {
        printf("MAX_REGIONS is too small , allocate at least %d !\n", num_regions);
        while (*(volatile int *)&D_00097534 != 0x78654321)
            __asm__ volatile("break 1024");
    }
    if (*(int *)&D_00096A2C != 3000) {
        printf("Error LevEdit cellsize %d whilst PSX cellsize %d\n", ((int **)&D_0009708C)[0][2], 3000);
        while (*(volatile int *)&D_00097534 != 0x78654321)
            __asm__ volatile("break 1024");
    }
    *(int *)&D_00097350 = *(int *)lump;
    lump += 4;
    func_00039748();
    func_00079DB0(*(int *)&D_00097108, lump, *(int *)&D_00097350 << 4);
}
