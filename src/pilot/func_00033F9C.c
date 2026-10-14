/* Driver 2: CheckLoadAreaData (with LoadInAreaModels / RequestSpool inlined) */
typedef struct {
    char pad0[2];
    unsigned char connected_areas[6];
    unsigned char super_region;
    unsigned char num_connected_areas;
} Spool;

typedef struct {
    unsigned short gfx_offset;
    unsigned short model_offset;
    char pad4[4];
    unsigned char model_size;
    char pad9[7];
} AREA_DATA;

typedef struct {
    unsigned char type;
    unsigned char data;
    unsigned short nsectors;
    unsigned int sector;
    char *addr;
    void (*func)();
} SPOOLQ;

typedef struct { int x; int y; } XYPAIR;

extern long long D_0009732C;
#define current_region (*(int *)&D_0009732C)
extern unsigned short *D_000972AC;
extern char *D_00097254;
extern long long D_00097324;
#define old_region (*(int *)&D_00097324)
extern int D_000972C0;
extern long long D_00096884;
extern long long D_0009733C;
extern int D_00097230;
extern long long D_00097344;
extern int D_000969E8;
extern AREA_DATA *D_00097294;
extern char *D_00097238;
extern char *D_000972D0;
extern long long D_00096834;
extern XYPAIR D_000BE0A4[][4];
extern SPOOLQ D_000A7BA4[];
extern int D_000972B0;
extern void func_00033B10(int area);
extern void func_00033F5C();

static inline void RequestSpool(int type, int data, int offset, int loadsize, char *address, void (*func)())
{
    SPOOLQ *next;
    int sector;

    next = &D_000A7BA4[D_000972B0];
    sector = D_000BE0A4[*(int *)&D_00096834][3].x / 2048 + offset;
    next->type = type;
    next->data = data;
    next->sector = sector;
    next->nsectors = loadsize;
    next->addr = address;
    next->func = func;
    D_000972B0++;
}

static inline void LoadInAreaModels(int area)
{
    int offset;
    int length;

    offset = D_00097294[area].model_offset;
    length = D_00097294[area].model_size;
    D_000972D0 = D_00097238 + (length - 1) * 2048;
    RequestSpool(3, 0, offset, length, D_00097238, func_00033F5C);
}

void func_00033F9C(int cellx, int cellz)
{
    int nAreas;
    int load;
    int boundary;
    int none;
    int i;
    Spool *spoolptr;

    i = old_region;
    spoolptr = (Spool *)(D_00097254 + D_000972AC[current_region]);
    load = 0;
    none = -1;

    if (i == none && spoolptr->super_region != 0xFF)
        goto use_super;
    {
        nAreas = spoolptr->num_connected_areas;

        if (i == none)
            D_000972C0 = i;
        else if (spoolptr->super_region != 0xFF || nAreas == 0)
            return;

        boundary = 13;
        if (*(int *)&D_00096884 == 0)
            boundary = 15;

        for (i = 0; i < nAreas; i++) {
            if (D_000972C0 != (spoolptr->connected_areas[i] & 0x3f)) {
                D_00097230 = spoolptr->connected_areas[i] >> 6;
                switch (D_00097230) {
                case 0:
                    if (cellz > *(int *)&D_0009733C - boundary)
                        load = 1;
                    break;
                case 1:
                    if (cellx > *(int *)&D_0009733C - boundary)
                        load = 1;
                    break;
                case 2:
                    if (cellz < boundary)
                        load = 1;
                    break;
                case 3:
                    if (cellx < boundary)
                        load = 1;
                    break;
                }
            }
            if (load)
                break;
        }

        if (i == nAreas) {
            if (D_000972C0 != -1)
                return;
            if (*(int *)&D_00097344 == -1)
                return;
            D_000972C0 = *(int *)&D_00097344;
        } else {
            D_000972C0 = spoolptr->connected_areas[i] & 0x3f;
        }
        goto done;
    }
use_super:
    D_000972C0 = spoolptr->super_region;
done:

    D_000969E8 = 1;
    func_00033B10(D_000972C0);
    LoadInAreaModels(D_000972C0);
}
