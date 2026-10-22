/* Driver 2: LoadInAreaTSets */
typedef struct {
    unsigned short gfx_offset;
    unsigned short model_offset;
    char pad4[4];
    unsigned char model_size;
    char pad9;
    unsigned char num_tpages;
    char padb[5];
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

extern AREA_DATA *D_00097294;
extern unsigned char *AreaTPages;
extern char *D_00097238;
extern long long D_00096834;
extern long long D_00097560;
extern XYPAIR D_000BE0A4[][4];
extern SPOOLQ D_000A7BA4[];
extern int spoolcounter;
extern int D_000972B4;
extern int D_000A7F54[];
extern unsigned char tpageslots[];
extern unsigned char D_000BE278[];
extern void SendTPage();

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

void LoadInAreaTSets(int area)
{
    unsigned char *tpages;
    int ntpages_to_load;
    int i;
    int slot;
    int j;
    int offset;
    int availableslots[16];
    int navailable;
    char *loadaddr;

    navailable = 0;
    tpages = AreaTPages + area * 16;
    loadaddr = D_00097238 + 0xA000;
    slot = *(int *)&D_00097560;
    ntpages_to_load = D_00097294[area].num_tpages;

    for (; slot < 15; slot++) {
        if (tpageslots[slot] == 0xFF) {
            availableslots[navailable++] = slot;
        } else {
            for (j = 0; j < ntpages_to_load; j++) {
                if (tpageslots[slot] == tpages[j])
                    break;
            }
            if (j == ntpages_to_load)
                availableslots[navailable++] = slot;
        }
    }

    offset = D_00097294[area].gfx_offset;

    for (i = 0; i < ntpages_to_load; i++) {
        if (D_000BE278[*tpages] == 0) {
            if (navailable <= 0)
                return;
            navailable--;
            D_000A7F54[D_000972B4 * 2] = availableslots[navailable];
        } else {
            D_000A7F54[D_000972B4 * 2] = D_000BE278[*tpages] - 1;
        }

        RequestSpool(1, 0, offset, 17, loadaddr, SendTPage);
        offset += 17;
        D_000A7F54[D_000972B4 * 2 - 1] = *tpages;
        D_000972B4++;
        tpages++;
    }
}

