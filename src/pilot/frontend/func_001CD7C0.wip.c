/* WIP 99.13: only the inner arm of the (D_000A801D & 4) branch differs: target copies the table base into v1 (addu v1,a0,zero) and stores sw 0x88(v1); here the inline helper gives addiu v1,a0,0x80 / sw 0x8(v1). Plain C (p[8].s = ...) gives 98.55 with no copy at all. The copy looks like an inline-param copy whose argument is not a bare pseudo; sel(p,...) and sel(p,8,...) get substituted away. */
/* Diagnosis (ov1-wip pass): the target's `addu v1,a0` is a pointer COPY that survives as its own global pseudo (used in both
   inner arms); every form tried here dies in cse1 (dump -ds: the copy's uses are rewritten to the older reg/v 80) or is
   coalesced. Tried: block-local `q = p` and `q = D_001CF710`, direct D_001CF710[8] stores, helper taking p with e[8] inside,
   helper with a dead `e++` after the stores, `p + 0` arg - all 98.55 (no copy). Next idea: something that invalidates
   p's cse equivalence between the copy and the stores (p reassigned in the arm), or a non-inlinable source of q. */
typedef struct { int a, b; char *s; int c; } T_001CF710;
extern T_001CF710 D_001CF710[];
extern int D_00096C90, D_00096C94, D_000967EC, D_00096784;
extern unsigned char D_000A801D;
extern char *D_001CF85C, *D_001CF860;
extern char D_001D32F0[], D_001D32F4[];
extern int sprintf();

static inline void sel(T_001CF710 *e, int on)
{
    if (on)
        e->s = D_001CF85C;
    else
        e->s = D_001CF860;
}

void func_001CD7C0(void)
{
    T_001CF710 *p;
    sprintf(D_001D32F0, "%d", D_00096C94 / 100 + 100);
    sprintf(D_001D32F4, "%d", D_00096C90 / 100 + 100);
    p = D_001CF710;
    p[4].s = D_001D32F0;
    p[2].s = D_001D32F4;
    if (D_000A801D & 4) {
        sel(&p[8], D_000967EC);
    } else {
        D_000967EC = 0;
        D_001CF710[8].s = D_001CF860;
    }
    p = D_001CF710;
    if (D_00096784)
        p[6].s = D_001CF85C;
    else
        p[6].s = D_001CF860;
}
