typedef struct { int x, y, z; } V3;
typedef struct { char p0[0xC]; unsigned flags; V3 *pos; char p1[0x2C - 0x14]; } ENT;
typedef struct { char p0[0x130]; ENT e[6]; } OBJ;
extern short D_0008EAAA[];
extern int ratan2();

/* Driver 2 name: unknown */
int func_00068054(OBJ *a, V3 *pos)
{
    ENT *e;

    V3 df;
    int dx, dz, ang, dist, d, c;


    for (e = a->e; e < a->e + 6; e++) {
        if (e->flags & 1) {
            V3 *q = e->pos;
            dx = q->x - pos->x;
            if (dx < 0) dx = pos->x - q->x;
            df.x = dx;
            dz = q->z - pos->z;
            if (dz < 0) dz = pos->z - q->z;
            df.z = dz;
            ang = ratan2(dz, dx);
            if ((unsigned)((ang & 0x7ff) - 0x200) > 0x400) {
                c = D_0008EAAA[(ang & 0xfff) * 2];
                d = df.x;
                if (d > 0x7ffff)
                    dist = ((d << 9) / c) << 3;
                else
                    dist = (d << 12) / c;
            } else {
                c = D_0008EAAA[(ang & 0xfff) * 2 - 1];
                d = df.z;
                if (d > 0x7ffff)
                    dist = ((d << 9) / c) << 3;
                else
                    dist = (d << 12) / c;
            }
            if ((unsigned)dist < 3000)
                return 1;
        }
    }
    return 0;
}
