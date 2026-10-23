/* wip 94.16: all stores/loop match; left: target sched1 puts the Loadfile name arg (lui/addiu a0, D_001E06A8) at the very top, so the prim constants get a1/a2 instead of a0/a1, and pos=0x80 lands right after DecDCTvlcBuild. Statement-order sweeps (prim fields, alloc block, name local, prototypes) don't move it. */
/* CRED entry point: set up the two face sprites, allocate buffers, load GFX\FACES.TEX, init credits state */
typedef struct {
    unsigned int tag;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    unsigned char u0, v0;
    unsigned short clut;
    short x1, y1;
    unsigned char u1, v1;
    unsigned short tpage;
    short x2, y2;
    unsigned char u2, v2;
    unsigned short pad1;
    short x3, y3;
    unsigned char u3, v3;
    unsigned short pad2;
} POLY_FT4;

typedef struct {
    unsigned short type;
    unsigned char a, b;
    int p;
} CREDLINE;

extern int D_00097074;
extern int D_00097098;
extern char D_001E06A8[];
extern POLY_FT4 D_001E2790[];
extern int D_001E27F0, D_001F3870, D_001F3818;
extern int D_001E27F8[];
extern int D_001E1AB8, D_001F3808, D_001E1F88, D_001F3800, D_001E1AC4, D_001F3848, D_001F3820;
extern int D_001E27E8, D_001E1ABC, D_001E27E0, D_001F3860;
extern int D_001F3810[];
extern CREDLINE D_001E1ADC[];
extern int D_001E1AC8[];
extern int D_001E1F90[];
extern int Loadfile();
extern void func_00073AC4();
extern void DecDCTvlcBuild();

void func_001E06B8(void)
{
    POLY_FT4 *p;
    CREDLINE *l;
    int *y;
    int pos;
    int i;

    D_00097074 = 0;
    p = D_001E2790;
    p->clut = 0x703B;
    p->tpage = 0x3E;
    p->u0 = 0x80; p->v0 = 0x80; p->u1 = 0xD0; p->v1 = 0x80; p->u2 = 0x80; p->v2 = 0xBF; p->u3 = 0xD0; p->v3 = 0xBF;
    D_001E2790[1].tpage = 0x3E;
    D_001E2790[1].clut = 0x707B;
    p++;
    p->u0 = 0; p->v0 = 0xC0; p->u1 = 0x50; p->v1 = 0xC0; p->u2 = 0; p->v2 = 0xFF; p->u3 = 0x50; p->v3 = 0xFF;
    D_001E27F0 = D_00097098;
    D_001F3870 = D_00097098 += 0x2800;
    D_001F3818 = D_00097098 += 0x3000;
    D_00097098 += Loadfile(D_001E06A8, D_00097098);
    func_00073AC4(0);
    DecDCTvlcBuild(D_001E27F8);

    D_001E1AB8 = 0;
    D_001F3808 = 0;
    D_001E1F88 = 0;
    D_001F3800 = -1;
    D_001E1AC4 = 0;
    D_001F3848 = 0x200;
    D_001F3820 = -0x100;
    D_001E27E8 = 0;
    D_001E1ABC = 0;
    D_001F3810[1] = -1;
    D_001F3810[0] = -1;
    D_001E27E0 = 0;
    D_001F3860 = -2;

    pos = 0x80;
    i = 0;
    while (D_001E1ADC[i].type != 10) {
        D_001E1F90[i] = pos;
        pos += D_001E1AC8[D_001E1ADC[i].type];
        i++;
    }
}
