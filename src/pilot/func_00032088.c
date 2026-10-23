/* func_00032088: move every object of an object list along its waypoint path (no Driver 2 counterpart).
 * Same object layout as func_0003181C (whose reset branch is the inline reset() here). The x24 clamp
 * matches only with the comparison written `x24 > x8 * 2` and the stored value as `x8 << 1`
 * (a different expression stops CSE reusing the compare's x8*2, so x8 is reloaded with lhu). */

typedef struct {
    int x;
    int z;
} WP;

typedef struct {
    WP *def;
    int flags;
    short x8;
    short xA;
    int key;
    int model;
    int x14;
    int x18;
    int x1C;
    short x20;
    short x22;
    short x24;
    char pad26[2];
    short x28;
    unsigned char x2A;
    char pad2B;
    short x2C;
    short x2E;
    short x30;
    short x32;
    short x34;
    short x36;
} OBJ;

typedef struct {
    int vx, vy, vz;
} VEC3;

typedef struct {
    int x, y, z;
    char c;
    char d;
    short e;
} REC;

extern OBJ **D_00089E84[];
extern unsigned char D_00089EA4[];
extern short rcossin_tbl[];
extern int D_000971CC, D_000971D0, D_000971D4, D_000971D8, D_000971DC;
extern long long D_00096808;
extern long long D_00096834;
extern VEC3 D_000A7AAC;
extern REC D_000A7ABC[];

extern int SquareRoot0();
extern int ratan2();

static inline void reset(OBJ *o)
{
    if ((o->flags & 0x10000) && *(int *)&D_00096808 == 0x18)
        o->x14 = -200000;
    else
        o->x14 = o->def[1].x;
    o->x18 = o->x30;
    o->x1C = o->def[1].z;
    if (o->flags & 0x10)
        o->x8 = o->xA;
    else
        o->x8 = 0;
    if ((o->flags & 0x8000) && *(int *)&D_00096808 == 0xD)
        o->x8 = 0x82;
    o->x2A = 2;
    o->flags = (o->flags & 0xFFFFFF) | 0x9000000;
    if (o->x2E != 0x7D00)
        o->x2C = 100;
    else
        o->x2C = 0x7D00;
    o->x32 = 0;
    o->x34 = 0;
    o->x36 = 0;
}

void func_00032088(int idx)
{
    char pad[48];
    OBJ **pp = D_00089E84[idx];
    OBJ *o;
    int flags;
    int dx, dz;
    int lim;
    int dist;
    int head;
    int a, d;
    int old;

    D_000971D0 = 0;
    o = *pp++;
    if (o != 0) do {
        flags = o->flags;
        if (flags & 0x200) {
            if (D_000971DC) {
                D_000A7AAC.vx = o->x14;
                D_000A7AAC.vy = o->x18;
                D_000A7AAC.vz = o->x1C;
                D_000971CC = 1;
            }
        } else if (flags & 0x18000) {
            D_000A7AAC.vx = o->x14;
            D_000A7AAC.vy = o->x18;
            D_000A7AAC.vz = o->x1C;
        }
        if (!(flags & 0x1000000))
            continue;
        if (!(flags & 0x8000000)) {
            if ((flags & 0x8000) && *(int *)&D_00096808 == 0xD) {
                o->x8 += 2;
                if (o->x8 > 0x104)
                    o->x8 = 0x104;
            } else {
                o->x8 += 1;
                if (o->x8 > o->xA)
                    o->x8 = o->xA;
            }
        }
        if ((flags & 0x10000) && *(int *)&D_00096808 == 0x18) {
            D_000A7AAC.vx = o->x14;
            D_000A7AAC.vy = o->x18;
            D_000A7AAC.vz = o->x1C;
            if (o->x8 > 0x28)
                D_000971D8 = 1;
            if (o->x8 < 10 && D_000971D8)
                D_000971D4 = 1;
        }
        if ((flags & 0x40000) && D_000971DC) {
            D_000A7AAC.vx = o->x14;
            D_000A7AAC.vy = o->x18;
            D_000A7AAC.vz = o->x1C;
            if (o->x8 > 0x28)
                D_000971D8 = 1;
            if (o->x18 < -6000)
                D_000971D4 = 1;
        }
        lim = 0xA0;
        if (flags & 0x80) {
            lim = 0x3C;
            if (!D_000971CC && (flags & 0x8000) && o->x2A >= 7 && o->x8 < 10)
                D_000971D4 = 1;
            if (!(flags & 0x6000000)) {
                if (!(o->x22 & 0x400)) {
                    if (o->x14 < o->def[o->x2A].x) {
                        o->x14 += 8;
                        if (o->x14 > o->def[o->x2A].x)
                            o->x14 = o->def[o->x2A].x;
                    } else {
                        o->x14 -= 8;
                        if (o->x14 < o->def[o->x2A].x)
                            o->x14 = o->def[o->x2A].x;
                    }
                } else {
                    if (o->x1C < o->def[o->x2A].z) {
                        o->x1C += 8;
                        if (o->x1C > o->def[o->x2A].z)
                            o->x1C = o->def[o->x2A].z;
                    } else {
                        o->x1C -= 8;
                        if (o->x1C < o->def[o->x2A].z)
                            o->x1C = o->def[o->x2A].z;
                    }
                }
            }
        }
        dx = o->x14 - o->def[o->x2A].x;
        dz = o->x1C - o->def[o->x2A].z;
        if (flags & 0x100)
            lim = 0xA0;
        if ((unsigned)(dx + 10000) > 20000 || (unsigned)(dz + 10000) > 20000)
            dist = 10000;
        else {
            dist = SquareRoot0(dx * dx + dz * dz);
            if (dist < 6000 || (flags & 0x6000000)) {
                if (o->x8 > lim || (flags & 0x8000000)) {
                    if (flags & 0x80) {
                        if ((flags & 0x8000) && *(int *)&D_00096808 == 0xD)
                            o->x8 -= 10;
                        else
                            o->x8 -= 5;
                    } else
                        o->x8 -= 3;
                    if (o->x8 < 0)
                        o->x8 = 0;
                }
            }
        }

        if (flags & 0x40) {
            if (D_000971CC && o->x2A >= 3 && o->x8 < 10)
                D_000971D4 = 1;
            ratan2(dx, dz);
            if (o->def[o->x2A + 1].x == 0xFFFFFF && o->x8 > 0x28)
                o->x8 -= 2;
            {
            int tx, tz;
            tx = o->x14 - o->def[o->x2A].x;
            tz = o->x1C - o->def[o->x2A].z;
            head = (unsigned short)o->x22;
            if (o->x2C == 0 && !(o->flags & 0x8000000)) {
                a = ratan2(tx, tz) & 0xFFF;
                d = 0x800 - ((o->x22 - a + 0x800) & 0xFFF);
                if (d < o->x34) {
                    o->x34 -= 0x20;
                    if (o->x34 < d)
                        o->x34 = d;
                } else if (d > o->x34) {
                    o->x34 += 0x20;
                    if (o->x34 > d)
                        o->x34 = d;
                }
                if (flags & 1)
                    head += o->x34 >> 6;
                else
                    head += o->x34 >> 4;
                if (flags & 0x100)
                    o->x20 = o->x34;
                else
                    o->x20 = o->x34 >> 2;
            }
            }
            if (flags & 0x100) {
                if (o->x24 > o->x8 * 2) {
                    o->x24 -= 8;
                    if (o->x24 < o->x8 * 2)
                        o->x24 = o->x8 << 1;
                } else {
                    o->x24 += 8;
                    if (o->x24 > o->x8 * 2)
                        o->x24 = o->x8 << 1;
                }
            } else {
                o->x24 = D_00089EA4[((o->x14 + o->x18) >> 8) & 0x1F];
                o->x24 = o->x24 - (o->x8 >> 1);
            }
        } else if (flags & 0x2000000) {
            if (((o->x22 - 0x20) & 0x3FF) == 0)
                o->flags &= ~0x2000000;
            else
                o->x22 = (o->x22 - 0x20) & 0xFFF;
            head = (unsigned short)o->x22;
        } else if (flags & 0x4000000) {
            if (((o->x22 + 0x20) & 0x3FF) == 0)
                o->flags &= ~0x4000000;
            else
                o->x22 = (o->x22 + 0x20) & 0xFFF;
            head = (unsigned short)o->x22;
        } else {
            head = ratan2(dx, dz) & 0xFFF;
        }
        if (!(flags & 2))
            o->x22 = head;

        if (D_000971DC == 1 && (flags & 8)) {
            D_000971DC = 2;
            o->x2C = 0;
        }
        if (o->x2C != 0)
            o->x2C--;
        else
            o->flags &= ~0x8000000;
        if (flags & 0x100) {
            o->x8 = -(o->x18 + 1000);
            if (o->x8 < 0)
                o->x8 = 0;
            o->x8 = o->x8 >> 5;
        }
        o->x14 -= (o->x8 * rcossin_tbl[(head & 0xFFF) * 2]) >> 12;
        o->x1C -= (o->x8 * rcossin_tbl[(head & 0xFFF) * 2 + 1]) >> 12;

        if ((flags & 0x100) && o->x2C == 0) {
            d = -10;
            if (o->x18 < -4000)
                d = -20;
            else if (o->x18 < -2000)
                d = -30;
            else if (o->x18 < -1000)
                d = -40;
            o->x18 += d;
        }
        if ((flags & 0x2000) && o->x2C == 0) {
            if (*(int *)&D_00096834 == 0)
                o->x18 += 10;
            else
                o->x18 += 20;
            if (o->x18 > 300)
                o->x18 = 300;
        }
        if ((flags & 0x4000) && o->x2C == 0) {
            o->x18 -= 40;
            if (o->x18 < -40000)
                o->x18 = -40000;
        }
        if (dist < 1500 && !(o->flags & 0x6000000)) {
            old = o->x2A;
            if (flags & 0x10000000)
                o->x2A = old - 1;
            else
                o->x2A = old + 1;
            if (!(flags & 0x1010) && o->def[o->x2A].x == 0xFFFFFF && (o->x8 >= 7 || (flags & 8))) {
                o->x2A = old;
                o->flags |= 0x8000000;
                o->x2C = o->x2E;
            } else if (o->def[o->x2A].x == 0xFFFFFF) {
                o->x8 = 0;
                o->x2C = o->x2E;
                if (!(flags & 0x10)) {
                    if (flags & 0x10000000) {
                        o->flags &= ~0x10000000;
                        o->x2A += 2;
                    } else {
                        o->flags |= 0x10000000;
                        o->x2A -= 2;
                    }
                } else {
                    o->x2A = 1;
                    reset(o);
                }
            } else {
                if (flags & 0x20)
                    o->x2C = o->x2E;
                if (flags & 0x80) {
                    int ex, ez;
                    ex = o->def[old].x - o->def[o->x2A].x;
                    ez = o->def[old].z - o->def[o->x2A].z;
                    if (!(flags & 0x6000000)) {
                        a = ratan2(ex, ez) & 0xFFF;
                        if (((o->x22 - a + 0x800) & 0xFFF) - 0x800 < 0)
                            o->flags |= 0x4000000;
                        else
                            o->flags |= 0x2000000;
                        if (flags & 0x80)
                            o->x22 = (o->x22 + 0x200) & 0xC00;
                    }
                }
            }
        }
        if (flags & 1) {
            D_000A7ABC[D_000971D0].x = o->x14;
            D_000A7ABC[D_000971D0].y = o->x18;
            D_000A7ABC[D_000971D0].z = o->x1C;
            D_000A7ABC[D_000971D0].d = (o->x22 + 0x400) >> 6;
            D_000A7ABC[D_000971D0].e = o->x28;
            D_000971D0++;
        }
    } while ((o = *pp++) != 0);
}
