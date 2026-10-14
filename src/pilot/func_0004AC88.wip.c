extern int D_000BC784[];
extern int D_000BC798[];
extern unsigned int *D_000974A4;

void func_0004AC88(int a, int b, int c)
{
    int na, nb, pa, pb, k;
    unsigned int x, y;
    unsigned int m8 = -8;

    na = D_000BC784[a];
    nb = D_000BC784[b];
    pa = D_000BC798[a];
    pb = D_000BC798[b];
    k = D_000BC798[c];
    k = k;
    while (k < 0x12B) {
        if (na == 0) {
            if (nb == 0)
                break;
            x = m8;
        } else
            x = D_000974A4[pa];
        if (nb != 0)
            y = D_000974A4[pb];
        else
            y = m8;
        if (x == y) {
            pa++;
            na--;
            pb++;
            nb--;
            D_000974A4[k] = y;
        } else if (x < y) {
            pa++;
            na--;
            D_000974A4[k] = x;
        } else {
            pb++;
            nb--;
            D_000974A4[k] = y;
        }
        k++;
    }
    D_000BC784[c] = k - D_000BC798[c];
}
