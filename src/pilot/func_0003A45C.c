/* no Driver 2 counterpart */
typedef struct {
    char *primptr;
    unsigned long *ot;
} DB;

typedef struct {
    char *primptr;
    unsigned long *ot;
    int f8;
    void *fC;
    int tpage;
    int clut;
    int f18;
    void *f1C;
    int f20;
} PLOT;

typedef struct {
    int a0, a1;
    int b0, b1;
    int c0, c1;
    int d0, d1;
    int e0, e1;
    int f0, f1;
    int g0, g1;
    int h;
} SEG;

extern long long current;
#define current (*(DB **)&current)
extern long long D_000974B4;
extern long long D_000972F0;
extern long long combointensity;
extern long long D_000970EC;
extern long long D_00096990;
extern int D_00096A3C;
extern char D_000BE9FC[];
extern char D_000BC7AC[];
extern char D_000BC8AC[];
extern SEG D_000AF5B0;
extern unsigned short texture_pages[];
extern unsigned short texture_cluts[][32];
extern void func_0003DF94();
extern void func_0003E244();

void func_0003A45C(SEG *s, int mode)
{
    PLOT plot;

    plot.primptr = current->primptr;
    plot.ot = current->ot + 8;
    plot.f8 = 0;
    plot.fC = D_000BE9FC;
    plot.tpage = (texture_pages[*(int *)&D_000974B4] | 0x20) << 16;
    plot.clut = texture_cluts[*(int *)&D_000974B4][0] << 16;
    plot.f18 = *(int *)&D_000972F0;
    plot.f1C = D_000BC7AC;
    plot.f20 = mode;
    if (mode)
        plot.f1C = D_000BC8AC;

    plot.ot = current->ot + 28;
    func_0003DF94(s->g0, s->g1, s->h, &plot, 0, 0);

    plot.ot = current->ot + 8;
    plot.f8 = *(int *)&combointensity & 0xFFFFFF;
    if (*(int *)&D_000970EC && s == &D_000AF5B0)
        plot.ot = current->ot + D_00096A3C;
    func_0003DF94(s->a0, s->a1, s->h, &plot, 0, 0);
    func_0003E244(s->b0, s->b1, s->h, &plot, 0, 0);

    plot.ot = current->ot + 8;
    func_0003DF94(s->c0, s->c1, s->h, &plot, *(int *)&D_00096990, 0);
    func_0003E244(s->d0, s->d1, s->h, &plot, *(int *)&D_00096990, 0);

    func_0003DF94(s->e0, s->e1, s->h, &plot, 0, 1);
    func_0003E244(s->f0, s->f1, s->h, &plot, 0, 1);

    current->primptr = plot.primptr;
}
