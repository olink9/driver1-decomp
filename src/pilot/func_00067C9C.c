typedef struct { char c0; char pad1[3]; short s4, s6, s8, sA, sC, sE, s10, s12, s14; short pad16; int i18, i1c, i20; char pad24[266]; unsigned char c12e; } SRC;
typedef struct { int v[0x20]; } BOX;
extern int func_00067838();
/* Driver 2 name: unknown */
int func_00067C9C(SRC *a, SRC *b, int *out0, int *out1, int *out2)
{
    BOX box[2];
    int res[10];
    int i;
    BOX *d;
    SRC *p;
    SRC *q;
    d = &box[0];
    p = a;
    q = b;
    i = 0;
    do {
        d->v[0] = p->s4;
        d->v[1] = p->sA;
        d->v[2] = p->s10;
        d->v[8] = p->s6;
        d->v[9] = p->sC;
        d->v[10] = p->s12;
        d->v[4] = p->s8;
        d->v[5] = p->sE;
        d->v[6] = p->s14;
        d->v[12] = p->i18;
        d->v[13] = p->i1c;
        d->v[14] = p->i20;
        if (p->c12e == 1 && q->c0 != 0 && q->c12e != p->c12e) {
            d->v[16] = 0xA2;
            d->v[17] = 0x1D4;
        } else {
            d->v[16] = 0xBA;
            d->v[17] = 0x208;
        }
        d->v[18] = 0x5A;
        i++;
        d = &box[1];
        p = b;
        q = a;
    } while (i < 2);
    {
        int r = func_00067838(&box[0], &box[1], res);
        if (r) {
            *out0 = res[0];
            out1[0] = res[1];
            out1[1] = res[2];
            out1[2] = res[3];
            out2[0] = res[5];
            out2[1] = res[6];
            out2[2] = res[7];
        }
        return r;
    }
}
