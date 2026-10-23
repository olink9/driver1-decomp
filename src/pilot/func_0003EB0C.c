typedef struct { short x, y, z, w; } SV;
typedef struct { char pad0[0xC]; unsigned short cnt; unsigned short n; SV *src; char *p14; } TBL;
typedef struct { char pad[0x114]; int a[6]; char pad2[0x17C - 0x12C]; short h; char pad3[0x18C - 0x17E]; } ENT;
extern TBL *D_000A1AB0[];
extern SV D_000B09BC[];
extern char D_000AF97A[];
extern char D_000B08B8[];
extern ENT car_data[];
extern void func_0003ECCC();
/* Driver 2 name: unknown */
void func_0003EB0C(int idx, unsigned int type)
{
    SV *src;
    SV *dst;
    int i;
    char *buf;
    char *p;
    int j;
    int k;

    if (type > 0x10)
        return;
    dst = &D_000B09BC[idx * 128];
    src = D_000A1AB0[type]->src;
    i = D_000A1AB0[type]->cnt;
    i--;
    while (i != -1) {
        int vx = src->x;
        int vy = src->y;
        int vz = src->z;
        dst->x = vx;
        dst->y = vy;
        dst->z = vz;
        src++;
        dst++;
        i--;
    }
    p = D_000A1AB0[type]->p14;
    if (idx != 0)
        buf = &D_000AF97A[idx * 150];
    else
        buf = D_000B08B8;
    for (k = 0; k < D_000A1AB0[type]->n; k++)
        buf[k] = 0;
    func_0003ECCC(p, buf, D_000A1AB0[type]->n);
    for (j = 5; j >= 0; j--)
        car_data[idx].a[j] = 0;
    car_data[idx].h = 0;
}
