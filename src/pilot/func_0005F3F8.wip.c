/* FAKEMATCH (if it ever matches): do { } while (0) wrappers here only shift register priorities. */
/* r3-06 (99.85): do{}while(0) around the flags==0 walk block raises p's global-alloc priority (loop-depth ref weight)
 * and fixes the p/spd s2/s3 swap; a second do{}while(0) around ox/oz fixes block-1 scheduling. The despawn tail is
 * func_0005F06C inlined (kill) and the 0xFF store uses a SLOT *q local (ptr-first addu). LEFT: block 1 local-alloc -
 * target has the andi/sll chain in v0 and addr1/load1 in v1 (ours: addr1 v0, product in a0); needs the sll-result qty to
 * outrank addr1 (needs addr2 scheduled before load1 in sched1). Tried: h1-h6, j1/j2, k1-k5, l1/l2, x1-x6, y1-y6, z1-z4. */
/* func_0005F3F8: per-frame update of the 20 pedestrian/cop slots (Driver 1 only) */
typedef struct {
    int x, y, z;
    char pad0C[0xC];
    short s18, angle, s1C, pad1E;
    short vx, vy, vz, pad26;
    char pad28[0x14];
    int flags;
    short pad40, s42;
    char pad44[4];
    unsigned char b48, pad49[3];
    int i4C;
    unsigned int speed : 8;
    int c51 : 8;
    int c52 : 8;
    unsigned int idx : 8;
    char pad54[2];
    unsigned char state, pad57;
} PED;
typedef struct { char pad0[0x40]; int f40; int pad44; int f48; char pad4C[0x7C]; int x; int pad; int z; char padD4[0x30];
                 int f104; char pad108[0x74]; short s17C; char pad17E[0xE]; } CAR;
typedef struct { char pad[0xA]; unsigned char fA; char padB; } SLOT;
typedef struct { int vx, vy, vz, pad; } VEC;

extern PED D_000BFA7C[];
extern CAR D_0009B0A4[];
extern short D_0008EAA8[];
extern int D_000976BC;
extern int D_000976C0;
extern unsigned char D_000976C8;
extern SLOT *D_000976CC;
extern long long D_00097080;
extern long long D_000C0160;
extern int D_000C0340[];
extern char D_000C017C[];
extern char D_000988C8[];
extern unsigned char D_000C6F01[];
extern void func_000605D8();
extern void func_0005F0EC();
extern void func_0005FA9C();
extern void func_00060C9C();
extern short func_00060AD4();
extern void func_00060C18();
extern CAR *func_000607AC();
extern void func_000650E4();
extern void func_0005FF70();
extern void func_0005FF90();
extern int ratan2();

static inline void kill(PED *o)
{
    D_000976C0 = D_000976C0 - 1;
    o->flags = 0;
    o->s1C = 0;
    o->angle = 0;
    o->s18 = 0;
    D_000C0340[D_000976C0] = o->s42;
    if (o->state == 6) {
        SLOT *q = &D_000976CC[o->idx];
        if (q->fA) q->fA = 0;
    }
}

void func_0005F3F8(void)
{
    VEC d;
    VEC dir;
    int i;
    PED *p;
    CAR *car;
    int spd;
    int spd2;
    int a;
    int dot;
    int r;
    int oz, ox;
    int turn;
    int snd;
    int off;
    int b;

    D_000976BC = D_0009B0A4[0].f104 >> 10;
    D_000976BC = D_000976BC + D_0009B0A4[0].s17C;
    func_000605D8();
    if (D_000976C8 != 0)
        func_0005F0EC();

    p = D_000BFA7C;
    i = 20;
    while (i-- != 0) {
        if (!(p->flags & 0x10))
            goto next;
        func_0005FA9C(p);
        if (p->flags & 0x40) {
            func_00060C9C(p, D_000C017C);
        } else {
            do {
            if (!(p->flags & 0x1000)) {
                func_0005FA9C(p);
                r = 500;
                if (p->state != 6) {
                    spd = p->speed;
                    if (p->c51 == 0 && p->c52 == 9)
                        a = p->angle + 0x800;
                    else
                        a = p->angle + 0x73A;
                    a &= 0xFFF;
    do {
                    ox = (D_0008EAA8[a * 2] * r) >> 12;
                    oz = (D_0008EAA8[a * 2 + 1] * r) >> 12;
    } while (0);
                    if (func_00060AD4(p->x + ox, p->z + oz) == 0) {
                        if (p->c51 == 0 && p->c52 == 9) {
                            b = (p->angle + 0xA00) & 0xFFF;
                            ox = (D_0008EAA8[b * 2] * r) >> 12;
                            oz = (D_0008EAA8[b * 2 + 1] * r) >> 12;
                            if (func_00060AD4(p->x + ox, p->z + oz) != 0)
                                p->flags |= 0x2000;
                            else
                                p->flags &= ~0x2000;
                        }
                        turn = -0x80;
                        if (p->flags & 0x2000)
                            turn = 0x80;
                        if (p->c51 < 0x20) {
                            func_00060C18(p, turn);
                            p->c51++;
                        } else {
                            p->vx = (spd * D_0008EAA8[((p->angle + 0x800) & 0xFFF) * 2]) >> 12;
                            p->vz = (spd * D_0008EAA8[((p->angle + 0x800) & 0xFFF) * 2 + 1]) >> 12;
                        }
                        p->c52 = 0;
                    } else {
                        p->c51 = 0;
                        if (p->c52++ >= 9) {
                            p->angle = (p->angle + 0x200) & 0xFC00;
                            p->vx = (spd * D_0008EAA8[((p->angle + 0x800) & 0xFFF) * 2]) >> 12;
                            p->vz = (spd * D_0008EAA8[((p->angle + 0x800) & 0xFFF) * 2 + 1]) >> 12;
                            p->c52 = 9;
                        }
                    }
                }
            }
            } while (0);
            car = func_000607AC(p);
            if (car != 0) {
                spd2 = 0x19;
                if (car == &D_0009B0A4[0] && !(p->flags & 0x1000)) {
                    snd = 0x1000;
                    if (*(int *)&D_00097080 != 0)
                        snd = 0x800;
                    func_000650E4(D_000988C8, 1, snd);
                    D_000C6F01[0] = 1;
                }
                d.vx = car->x - p->x;
                d.vz = car->z - p->z;
                dir.vy = 0;
                d.vy = 0;
                dir.vx = car->f48;
                dir.vz = -car->f40;
                dot = (dir.vx * d.vx + dir.vy * d.vy + dir.vz * d.vz + 0x800) >> 12;
                off = 0;
                if (dot >= 0)
                    off = 0x800;
                p->angle = (off - ratan2(car->f48, car->f40)) & 0xFFF;
                p->vy = 0;
                if (p->flags & 0x1000) {
                    if (p->b48 < 3)
                        spd2 = 0x19;
                    else
                        spd2 = 0x32;
                }
                p->vx = (spd2 * D_0008EAA8[((p->angle + 0x800) & 0xFFF) * 2]) >> 12;
                p->vz = (spd2 * D_0008EAA8[((p->angle + 0x800) & 0xFFF) * 2 + 1]) >> 12;
                if (!(p->flags & 0x1000)) {
                    p->i4C = *(int *)&D_000C0160;
                    p->b48 = 0;
                }
                p->flags |= 0x1000;
                if (p->state == 6) {
                    SLOT *q = &D_000976CC[p->idx];
                    q->fA = 0xFF;
                    p->state = 2;
                    p->speed = 0x14;
                }
            }
        }
        switch (p->state) {
        case 1:
        case 5:
            break;
        case 2:
        case 3:
        case 4:
            func_0005FF70(p);
            break;
        case 6:
            func_0005FF90(p);
            break;
        }
        if (p->flags & 0x10) {
            int dx = p->x - D_0009B0A4[0].x;
            int dz = p->z - D_0009B0A4[0].z;
            if (dx * dx + dz * dz > 0x2B80D840) {
                kill(p);
            }
        }
    next:
        p++;
    }
}
