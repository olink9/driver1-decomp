typedef struct { int vx, vy, vz, pad; } VEC;
typedef struct { unsigned char type; char pad[3]; short m[3][3]; } OBJ;
typedef struct { char pad[5]; unsigned char f5; char pad2[74]; } INFO;

extern INFO D_00098950[];
extern void func_0003FAE8();

int func_0003FC98(OBJ *p, VEC *v, int speed, unsigned char *q)
{
    VEC dir;
    VEC side;
    int fwd, lat;
    int dirn;
    char flag;

    if (speed < 0x1000)
        return 0;

    dir.vx = p->m[0][2];
    dir.vy = p->m[1][2];
    dir.vz = p->m[2][2];
    fwd = (dir.vx * v->vx + dir.vy * v->vy + dir.vz * v->vz + 0x800) >> 12;
    side.vx = p->m[0][0];
    side.vy = p->m[1][0];
    side.vz = p->m[2][0];
    lat = (side.vx * v->vx + side.vy * v->vy + side.vz * v->vz + 0x800) >> 12;

    if (lat > 0) {
        if (fwd < 0x105) {
            if (fwd >= -0x103)
                dirn = 2;
            else
                dirn = 3;
        } else
            dirn = 1;
    } else {
        if (fwd < 0x105) {
            if (fwd >= -0x103)
                dirn = 5;
            else
                dirn = 4;
        } else
            dirn = 0;
    }

    if (p->type == 0) {
        speed = ((speed / 350 + 0x200) * 3) / 8;
        if (speed >= 0x478)
            speed = 0x477;
    } else {
        speed = ((speed / 400 + 0x400) * 7) / 8;
    }

    flag = 1;
    if (p->type == 0x17)
        flag = *q != 0;
    else if (p->type == 0 || *q == 0 || D_00098950[p->type - 1].f5 == 0 || D_00098950[*q - 1].f5 == 0)
        flag = 0;

    func_0003FAE8(p, dirn, speed, flag);
    return 1;
}
