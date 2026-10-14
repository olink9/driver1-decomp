typedef struct { int x, y, z; } V3;
typedef struct {
    V3 p;
    int padp[3];
    V3 q;
    char pad[0x30 - 0x24];
    int dist;
    char pad2[2];
    unsigned char flags;
} OBJ;
extern void func_000614DC();
extern int SquareRoot0();

int func_000618C8(OBJ *o) {
    V3 d;
    V3 *q;
    func_000614DC(o);
    q = &o->q;
    func_000614DC(q);
    if (!(o->flags & 0x10)) {
        d.x = o->p.x - o->q.x;
        d.y = o->p.y - q->y;
        d.z = o->p.z - q->z;
        o->dist = SquareRoot0(d.x * d.x + d.y * d.y + d.z * d.z);
    }
    switch (o->flags & 0xC) {
    case 0:
        break;
    case 4:
        o->p.x += o->q.x;
        o->p.y += o->q.y;
        o->p.z += o->q.z;
        break;
    case 8:
        o->q.x += o->p.x;
        o->q.y += o->p.y;
        o->q.z += o->p.z;
        break;
    }
}
