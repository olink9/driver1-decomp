/* r3-11 (run2-r3) findings - two remaining diffs:
 * (1) Target computes tpage2send<<6 (s1) and &D_00087030 (s3) BEFORE the LoadImage call and adds them after it
 *     (addu a1,s1,s3). Assigning clutptr before the call gives one summed s-reg instead; two locals a,b assigned
 *     before the call are single-use and update_equiv_regs sinks them past the call. What reproduces the shape:
 *     the SAME two int locals reused in the else branch (b = (int)D_000BE278; a = tpage2send + b; loads/stores via
 *     *(unsigned char *)a and *(unsigned char *)(old + b)) - matches the target's s1/s3 use there too. Copy kept at
 *     build/scratch/r3-11/k5_keep.c (93.10). But then global-alloc order is b(.33) > a(.30) > slot(.30) > tpage(.107)
 *     while the target needs slot > a > tpage > b (b's priority < .107): only the register names are left wrong.
 * (2) cluts.w=64 / tpage.w/h stores and addr += 4 order in the then-branch: all 5040 orders of the 7 field stores on
 *     the a/b form leave it unchanged (93.10) - likely follows from the register choice (anti-deps), fix (1) first.
 * Tried without effect: D2 statement order, clutptr in for/inline/index forms (i/2, j, i>>1), decl order of a/b.
 */
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
