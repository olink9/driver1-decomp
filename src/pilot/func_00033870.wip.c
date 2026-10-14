/* func_00033870 - Driver 2: SendTPage */
typedef struct {
    short x, y, w, h;
} RECT16;

typedef struct {
    int tpage;
    int slot;
} TSETINFO;

typedef struct {
    short vx, vy;
} SXYPAIR;

extern int D_00097238;              /* model_spool_buffer */
extern int D_00097250;              /* tsetpos */
extern int D_00097208;              /* nTPchunks */
extern int D_00097244;              /* loadbank_write */
extern int D_000972B4;              /* tsetcounter */
typedef struct {
    short x, y;
} RECTXY;
extern RECTXY D_00097218;           /* tpage.x/y */
extern short D_0009721C;            /* tpage.w */
extern short D_0009721E;            /* tpage.h */
extern int D_000A7F50[];            /* tsetinfo */
extern unsigned char D_000BE278[];  /* tpageloaded */
extern unsigned char D_000BE4F8[];  /* tpageslots */
extern SXYPAIR D_000BE708[];        /* slot_tpagepos */
extern SXYPAIR D_000BE1BC[];        /* slot_clutpos */
extern unsigned long D_00087030[][16]; /* texture_cluts */
extern unsigned short D_00086F30[]; /* texture_pages */

extern void func_00074404(RECT16 *rect, void *p); /* LoadImage */

#define getClut(x, y) (((y) << 6) | (((x) >> 4) & 0x3f))
#define getTPage(tp, abr, x, y) \
    ((((tp) & 0x3) << 7) | (((abr) & 0x3) << 5) | (((y) & 0x100) >> 4) | (((x) & 0x3ff) >> 6) | (((y) & 0x200) << 2))

void func_00033870(void)
{
    unsigned char old;
    unsigned long *clutptr;
    int slot;
    int npalettes;
    int tpage2send;
    int i;
    char *addr;
    RECT16 cluts;

    addr = (char *)D_00097238 + 0xA000;
    tpage2send = D_000A7F50[D_00097250 * 2];
    slot = D_000A7F50[D_00097250 * 2 + 1];

    if (D_00097208 == 0) {
        addr += 0x4000;
        if (slot != D_000BE278[tpage2send] - 1) {
            npalettes = *(int *)addr;
            addr += 4;

            D_00097218.x = D_000BE708[slot].vx;
            D_0009721C = 64;
            D_0009721E = 64;
            D_00097218.y = D_000BE708[slot].vy;

            cluts.w = 64;
            cluts.x = D_000BE1BC[slot].vx;
            cluts.y = D_000BE1BC[slot].vy;
            cluts.h = npalettes / 4 + 1;

            func_00074404(&cluts, addr);

            clutptr = D_00087030[tpage2send];
            i = 0;

            while (i < npalettes) {
                clutptr[0] = getClut(cluts.x + 16, cluts.y) << 0x10 | getClut(cluts.x, cluts.y);
                clutptr[1] = getClut(cluts.x + 48, cluts.y) << 0x10 | getClut(cluts.x + 32, cluts.y);
                clutptr += 2;
                cluts.y++;
                i += 4;
            }

            D_00086F30[tpage2send] = getTPage(0, 0, D_00097218.x, D_00097218.y);
        }
    } else {
        addr += (D_00097244 & 1) << 13;
        if (slot != D_000BE278[tpage2send] - 1) {
            func_00074404((RECT16 *)&D_00097218, addr);
            D_00097218.y += D_0009721E;
        }

        if (D_00097208 == 4) {
            old = D_000BE4F8[slot];
            D_000BE4F8[slot] = tpage2send;
            D_000BE278[old] = 0;
            D_000BE278[tpage2send] = slot + 1;
            D_00097250++;
            if (D_00097250 == D_000972B4) {
                D_000972B4 = 0;
                D_00097250 = 0;
            }
        }
    }
}
