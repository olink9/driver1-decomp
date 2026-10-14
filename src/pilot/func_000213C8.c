/* Driver 1 func_000213C8 */
typedef struct { int v[4]; unsigned char type; char pad[3]; } E20;
typedef struct { int a[4]; } Q;
typedef struct { char pad[6]; unsigned char n; unsigned char bits; char pad2[8]; } E16;
extern E20 *D_0009704C;
extern E16 *D_00097044;

int func_000213C8(int idx, unsigned char side, Q *out)
{
    E16 *t = &D_00097044[idx];
    unsigned char type = D_0009704C[idx].type;
    int n;
    unsigned char bits, cnt, diff;

    if (type == 0) {
        if (side == 1 || side == 3)
            return 0;
    }
    if (type == 1) {
        if (side == 0 || side == 2)
            return 0;
    }
    out->a[0] = D_0009704C[idx].v[0];
    out->a[1] = D_0009704C[idx].v[1];
    out->a[2] = D_0009704C[idx].v[2];
    out->a[3] = D_0009704C[idx].v[3];
    if (type == 0) {
        n = t->n; bits = t->bits; cnt = 0;
        while (n) { if (bits & 1) cnt++; n--; bits >>= 1; }
        diff = t->n - cnt;
        if (side == 0)
            out->a[0] += diff;
        else
            out->a[2] -= cnt;
    } else {
        n = t->n; bits = t->bits; cnt = 0;
        while (n) { if (bits & 1) cnt++; n--; bits >>= 1; }
        diff = t->n - cnt;
        if (side == 3)
            out->a[3] -= cnt;
        else
            out->a[1] += diff;
    }
    return 1;
}
