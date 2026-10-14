extern long long D_00097530;
typedef struct { char *prim; unsigned int *ot; } DB;
#define cur (*(DB **)&D_00097530)
extern int func_0007B1E8();
typedef struct { unsigned int tag, col; short xy[6]; } P;
typedef struct { unsigned char pad, a, b, c; } T;
void func_00035BC8(void)
{
    T *t = *(T **)0x1F80003C;
    short *sc = *(short **)0x1F80001C;
    int *v = (int *)0x1F800044;
    DB *db = cur;
    P *p = (P *)db->prim;
    int z;
    DB *d;
    if (func_0007B1E8(v[t->a], v[t->b], v[t->c]) > 0) {
        p->col = 0;
        z = (sc[t->a] + sc[t->b] + sc[t->c]) / 3;
        ((unsigned char *)p)[3] = 4;
        ((unsigned char *)p)[7] = 0x20;
        p->xy[0] = ((short *)v)[t->a * 2];
        p->xy[1] = ((short *)v)[t->a * 2 + 1];
        p->xy[2] = ((short *)v)[t->b * 2];
        p->xy[3] = ((short *)v)[t->b * 2 + 1];
        p->xy[4] = ((short *)v)[t->c * 2];
        p->xy[5] = ((short *)v)[t->c * 2 + 1];
        d = cur;
        p->tag = (p->tag & 0xFF000000) | (d->ot[z] & 0xFFFFFF);
        d->ot[z] = (d->ot[z] & 0xFF000000) | ((unsigned int)p & 0xFFFFFF);
        d->prim += 0x14;
    }
}
