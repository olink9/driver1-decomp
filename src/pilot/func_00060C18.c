struct P { int x, y, z; char p0[0xE]; short dir; char p1[4]; short vx; char p2[2]; short vz; char p3[0x2A]; unsigned char speed; };
struct SC { short s, c; };
extern struct SC D_0008EAA8[];
void func_00060C18(struct P *p, int turn) {
    int dir;
    int speed;
    dir = (p->dir + turn) & 0xFFF;
    p->dir = dir;
    dir += 0x800;
    p->x -= p->vx;
    p->z -= p->vz;
    speed = p->speed;
    p->vx = (speed * D_0008EAA8[dir & 0xFFF].s) >> 12;
    p->vz = (speed * D_0008EAA8[dir & 0xFFF].c) >> 12;
}
