typedef struct { int a; int idx; int *d; } S35558;
typedef struct { unsigned short v; unsigned short w; } P35558;
extern S35558 D_000A7F44;
extern P35558 D_000AA7D4[];
extern long long D_000970E0;
extern long long D_0009733C;
extern long long D_00097534;
extern int D_000972A0;
extern char D_0001052C[];
extern int printf();
#define OUT (*(unsigned short **)&D_000970E0)

static inline void fill(int k) { OUT[k] = 0xFFFF; }
void func_00035558(void)
{
    int idx = D_000A7F44.idx;
    int n = *(int *)&D_0009733C;
    int base = idx * n * n;
    int *d;
    int mode;
    unsigned short *src;
    int i;

    D_000972A0 = 0;
    d = D_000A7F44.d;
    d += 1;
    mode = *d;
    d += 1;
    src = (unsigned short *)d;
    if (mode == 0) {
        /* guarded do-while: a plain for lets cse_around_loop drop the bound copy */
        if (n * n > 0) {
            i = 0;
            do {
                OUT[base + i] = 0xFFFF;
            } while (++i < n * n);
        }
    } else if (mode == 1) {
        for (i = 0; i < n * n; i++) {
            unsigned short s = *src++;
            if (s != 0xFFFF)
                s += D_000AA7D4[idx].v;
            OUT[base + i] = s;
        }
    } else if (mode == 2) {
        int bit = 0x8000;
        unsigned short flags = *src++;
        for (i = 0; i < n * n; i++) {
            unsigned short s;
            if (flags & bit) {
                s = *src++;
                s += D_000AA7D4[idx].v;
            } else
                s = 0xFFFF;
            OUT[base + i] = s;
            bit >>= 1;
            if (!bit) {
                bit = 0x8000;
                flags = *src++;
            }
        }
    } else {
        printf(D_0001052C);
        while (*(volatile int *)&D_00097534 != 0x78654321)
            __asm__ volatile("break 1024");
    }
}
