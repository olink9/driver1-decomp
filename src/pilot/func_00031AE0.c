/* func_00031AE0: per-frame update + draw of every object in an object list
 * (ground-following objects, smoke emitters, sparks on rotation change). No Driver 2 counterpart.
 * rcossin table: the cos half is addressed as D_0008EAA8[i*2 + 1] (= D_0008EAAA); that form is what
 * makes GCC emit the la-macro (t0) sequence of the target. */
typedef struct { short vx, vy, vz, pad; } SVECTOR;
typedef struct { int vx, vy, vz, pad; } VECTOR;
typedef struct { short m[3][3]; int t[3]; } MATRIX;

typedef struct {
    void *def;
    int flags;
    short x8;
    unsigned short xA;
    int key;
    void *model;
    int x14;
    int x18;
    int x1C;
    short x20;
    short x22;
    short x24;
} OBJ;

extern OBJ **D_00089E84[];
extern VECTOR D_0008A8F0;
extern long long D_00097120;
extern MATRIX D_000BBE84;
extern short D_0008EAA8[];
extern SVECTOR D_00089EC4[];

extern int func_0001F5B0();
extern int ratan2();
extern int func_0004FFE8();
extern int func_000398B0();
extern void func_00047A90();
extern void func_0002DBB4();
extern int func_0002C5F0();
extern int func_000776E4();
extern void func_0004E9C4();
extern void func_0004EA6C();
extern void func_00079D20();
extern void func_0004EAC0();
extern void func_0002E6B8();
extern void func_0004E948();
extern void func_0002E230();
extern void func_000774C4();
extern void func_00047FA4();

static inline int add850(int y)
{
    return y + 850;
}

void func_00031AE0(int idx)
{
    MATRIX m;
    VECTOR pos;
    SVECTOR rot;
    VECTOR v;
    VECTOR p;
    VECTOR vel;
    VECTOR sp;
    VECTOR spvel;
    OBJ **pp = D_00089E84[idx];
    OBJ *o;
    int flags;
    int diff;
    int h1, h2;
    int a;
    int i;
    VECTOR *pp2;
    VECTOR *pvel;
    VECTOR *psp;
    void *model;

    o = *pp++;
    if (o != 0) do {
        flags = o->flags;
        diff = o->x24;
        if (flags & 4) {
            if ((o->x22 - 0x400) & 0x800) {
                v.vx = o->x14;
                v.vz = o->x1C + 250;
                v.vy = o->x18;
                h1 = -func_0001F5B0(&v);
                v.vz -= 500;
                h2 = -func_0001F5B0(&v);
            } else {
                v.vx = o->x14 + 250;
                v.vz = o->x1C;
                v.vy = o->x18;
                h1 = -func_0001F5B0(&v);
                v.vx -= 500;
                h2 = -func_0001F5B0(&v);
            }
            o->x24 = ratan2(h2 - h1, 500);
            o->x18 = (h2 + h1) >> 1;
        }
        if (flags & 0x200) {
            a = o->x22;
            p.vy = (func_0004FFE8(3) & 0x3F) - 60;
            p.vx = (D_0008EAA8[(a & 0xFFF) * 2] * 125) >> 9;
            p.vz = (D_0008EAA8[(a & 0xFFF) * 2 + 1] * 125) >> 9;
            p.vx += o->x14 + (func_0004FFE8(0) & 0x7F);
            p.vz += o->x1C + (func_0004FFE8(1) & 0x7F);
            vel.vx = 0;
            vel.vy = 10;
            vel.vz = 0;
            pp2 = &p;
            if (o->x8 > 60 && func_000398B0(pp2) == 1)
                func_00047A90(pp2, 200, 400, 5, o->x8 - 60, &vel, 0);
        }
        func_0002DBB4();
        if ((flags & 0x1000000) && func_0002C5F0(&o->x14, 800) != -1 &&
            ((flags & 0x400) || func_000398B0(&o->x14) != 0)) {
            if (flags & 0x100) {
                pos.vx = o->x14 - D_0008A8F0.vx;
                pos.vy = o->x18 - add850(D_0008A8F0.vy);
                pos.vz = o->x1C - D_0008A8F0.vz;
                model = *(void **)&D_00097120;
                rot.vx = o->x20;
                rot.vy = func_000776E4() & 0xFFF;
                rot.vz = o->x24;
                func_0004E9C4(&m, &rot);
                func_0004EA6C(&m, func_000776E4() & 0xFFF);
                func_00079D20(&D_000BBE84);
                func_0004EAC0(&pos);
                diff -= o->x24;
                if (diff < 0)
                    diff = -diff;
                func_0002E6B8(model, &m, &pos);
            }
            model = o->model;
            pos.vx = o->x14 - D_0008A8F0.vx;
            pos.vy = o->x18 - D_0008A8F0.vy;
            pos.vz = o->x1C - D_0008A8F0.vz;
            rot.vx = o->x20;
            rot.vy = o->x22 + 0x400;
            rot.vz = o->x24;
            func_0004E948(&m, &rot);
            func_00079D20(&D_000BBE84);
            func_0004EAC0(&pos);
            diff -= o->x24;
            if (diff < 0)
                diff = -diff;
            if (flags & 0x400) {
                func_0002E6B8(model, &m, &pos);
            } else if (!(flags & 4) && o->x18 < D_0008A8F0.vy) {
                if (flags & 0x20000) {
                    if (abs(pos.vx) + abs(pos.vz) < 35000)
                        func_0002E230(model, &m, &pos, 600);
                } else {
                    func_0002E230(model, &m, &pos, 40);
                }
            } else {
                func_0002E230(model, &m, &pos, 0);
            }
            if ((flags & 4) && diff > 5) {
                func_00079D20(&m);
                i = 0;
                pvel = &spvel;
                psp = &sp;
                for (; i < 4; i++) {
                    func_000774C4(pvel, 0, 16);
                    sp.vx = D_00089EC4[i].vx;
                    sp.vy = D_00089EC4[i].vy;
                    sp.vz = D_00089EC4[i].vz;
                    func_0004EAC0(psp);
                    sp.vx += o->x14;
                    sp.vy += o->x18;
                    sp.vz += o->x1C;
                    func_00047FA4(psp, pvel, 5, 0);
                }
            }
        }
    } while ((o = *pp++) != 0);
}
