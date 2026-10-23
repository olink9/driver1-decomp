/* WIP 98.65% (ov1-opus): x(D_001D2DC0) and the D_00096808 value swap a0/v1, and the two luis are in the
 * other order.  Diagnosis (cc1 -dc/-dS/-dl dumps):
 *  - `return x + 1` / `r = x + 1; ... return r` always combines into a hard `v0 = x + 1`, sched1 then sinks it
 *    below the store, x dies there and local-alloc gives x v0's suggestion (92.3).
 *  - Target needs: r = x + 1 kept as a pseudo (copy to v0 not combined) AND scheduled above the sum, with x
 *    dying at the sum.  Modifying x after r (below) blocks combine and pins r with an anti-dep (98.65), but then
 *    the sum lands in x's register instead of the D_00096808 value's.
 *  - A modification merged away by combine (`x = -x; D -= x`) still blocks combine (reg_last_set), but sched1
 *    sinks r below the store again (tie-break: lower luid placed later).  ~280 natural variants swept, none >98.65.
 *  - `D_00096808 += (x = D_001D2DC0); return x + 1;` gives the target's lui/lw order (92.88).
 */
extern int D_00096808;
extern int D_001D2DC0;
extern char D_001CDFC0[];
extern char D_001CDFE8[];
extern void func_001C11E4();
extern void DrawSync();
extern void VSync();
extern void func_001C2114();
extern void SetDispMask();

int func_001C4A7C(void)
{
    int x, r;
    func_001C11E4();
    DrawSync(0);
    VSync(0);
    func_001C2114(D_001CDFC0, D_001CDFE8, 0);
    SetDispMask(0);
    x = D_001D2DC0;
    r = x + 1;
    x += D_00096808;
    D_00096808 = x;
    return r;
}
