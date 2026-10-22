struct P { int x, y, z; char p0[0xE]; short dir; char p1[4]; short vx; char p2[2]; short vz; char p3[0x2A]; unsigned char speed; };
extern short rcossin_tbl[];
void SetPedestrianTurn(struct P *p, int turn) {
    int dir;
    int speed;
    dir = (p->dir + turn) & 0xFFF;
    p->dir = dir;
    dir += 0x800;
    p->x -= p->vx;
    p->z -= p->vz;
    speed = p->speed;
    p->vx = (speed * rcossin_tbl[(dir & 0xFFF) * 2]) >> 12;
    p->vz = (speed * rcossin_tbl[(dir & 0xFFF) * 2 + 1]) >> 12;
}
