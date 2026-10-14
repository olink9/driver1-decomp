typedef struct {
    int x, y, z;
    char p0[0x14];
    short vx, vy, vz;
    char p1[0x14];
    unsigned flags;
    char p2[8];
    unsigned char frame;
    char p3[3];
    unsigned char *anim;
    char p4[6];
    unsigned char type;
} OBJ;

extern long long D_0009687C;
extern int D_000C015C[];
extern int D_000C0164[];
extern int func_0001F5B0();
extern void func_000610A4();

void func_00060920(OBJ *o)
{
    int lim;

    if (*(int *)&D_0009687C != 0)
        return;
    o->x += o->vx;
    o->z += o->vz;
    lim = 40 - func_0001F5B0(o);
    if (o->y < lim || (o->flags & 0x1000)) {
        o->y += o->vy;
        o->vy += 8;
    }
    if (lim < o->y) {
        o->y = lim;
        o->vy = 10;
    }
    if (o->flags & 0x1000) {
        if (o->frame == 2) {
            o->vx <<= 1;
            o->vz <<= 1;
        } else if (o->frame == 14) {
            o->vx >>= 1;
            o->vz >>= 1;
        }
        if (o->frame >= (o->anim[0] - 2) * 2) {
            func_000610A4(o);
            if (o->type == 2)
                o->anim = (unsigned char *)D_000C015C[0];
            else
                o->anim = (unsigned char *)D_000C0164[0];
            o->flags &= ~0x1000;
        }
    }
    o->frame += o->anim[1];
    if ((o->anim[0] - 1) * 2 < o->frame)
        o->frame = 0;
}
