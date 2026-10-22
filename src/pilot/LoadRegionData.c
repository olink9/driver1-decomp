/* Driver 2: LoadRegionData. RequestSpool is a static inline here. Key: ONE multi-set buffer variable for the cells / cell_objects targets, built in place (addr = base; addr += idx * size;) - a single-set temp gets sched1 birthing boost and the in-place += fixes the a0/a1 allocation. */
typedef struct {
    unsigned short offset;
    unsigned char connected_areas[2];
    unsigned char pvs_size;
    unsigned char cell_data_size[3];
    unsigned char super_region;
    unsigned char num_connected_areas;
    unsigned char roadm_size;
    unsigned char roadh_size;
} Spool;

typedef struct {
    unsigned char type;
    unsigned char data;
    unsigned short nsectors;
    unsigned int sector;
    char *addr;
    void (*func)();
} SPOOLQ;

typedef struct {
    short region_to_unpack;
    short target_barrel_region;
    int nsectors;
    char *cell_addr;
    char *roadm_addr;
    char *roadh_addr;
} SPL_REGIONINFO;

typedef struct { int x; int y; } XYPAIR;

extern long long D_00096834;
extern XYPAIR D_000BE0A4[][4];
extern SPOOLQ D_000A7BA4[];
extern volatile int spoolcounter;
extern unsigned short *D_000972AC;
extern char *D_00097254;
extern short D_0009728C[];
extern char *D_000AA7E8[];
extern int D_000AA7D4[];
extern int D_000AB040[];
extern char *D_0009724C;
extern long long D_000970D4;
extern long long D_00097108;
extern long long D_00097350;
extern int D_0009727C;
extern SPL_REGIONINFO D_000A7EA4[];
extern void func_0003542C();
extern void func_000354D8();
extern void func_0003490C();

static inline void RequestSpool(int type, int data, int offset, int loadsize, char *address, void (*func)())
{
    SPOOLQ *next;
    int sector;

    next = &D_000A7BA4[spoolcounter];
    sector = D_000BE0A4[*(int *)&D_00096834][3].x / 2048 + offset;
    next->type = type;
    next->data = data;
    next->sector = sector;
    next->nsectors = loadsize;
    next->addr = address;
    next->func = func;
    spoolcounter++;
}

int LoadRegionData(int region, int target_region)
{
    int offset;
    char *addr;
    char *roadmap_buffer;
    char *roadh_buffer;
    char *cell_buffer;
    Spool *spoolptr;

    D_0009728C[target_region] = region;
    spoolptr = (Spool *)(D_00097254 + D_000972AC[region]);
    offset = spoolptr->offset;
    roadmap_buffer = D_000AA7E8[target_region];
    RequestSpool(0, 0, offset, spoolptr->roadm_size, roadmap_buffer, func_0003542C);
    offset += spoolptr->roadm_size;
    roadh_buffer = roadmap_buffer + 0x1000;
    RequestSpool(0, 0, offset, spoolptr->roadh_size, roadh_buffer, func_000354D8);
    offset += spoolptr->roadh_size;
    cell_buffer = D_0009724C;
    RequestSpool(0, 0, offset, spoolptr->cell_data_size[1], cell_buffer, 0);
    offset += spoolptr->cell_data_size[1];
    addr = (char *)*(int **)&D_000970D4;
    addr += D_000AA7D4[target_region] * 4;
    RequestSpool(0, 0, offset, spoolptr->cell_data_size[0], addr, 0);
    offset += spoolptr->cell_data_size[0];
    addr = *(char **)&D_00097108;
    addr += (*(int *)&D_00097350 + D_000AB040[target_region]) * 16;
    RequestSpool(0, 0, offset, spoolptr->cell_data_size[2], addr, 0);
    offset += spoolptr->cell_data_size[2];
    RequestSpool(0, 0, offset, spoolptr->pvs_size, D_000AA7E8[target_region], func_0003490C);
    offset += spoolptr->pvs_size;
    D_000A7EA4[D_0009727C].nsectors = offset - spoolptr->offset;
    D_000A7EA4[D_0009727C].region_to_unpack = region;
    D_000A7EA4[D_0009727C].target_barrel_region = target_region;
    D_000A7EA4[D_0009727C].cell_addr = cell_buffer;
    D_000A7EA4[D_0009727C].roadm_addr = roadmap_buffer;
    D_000A7EA4[D_0009727C].roadh_addr = roadh_buffer;
    return 1;
}
