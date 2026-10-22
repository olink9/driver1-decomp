/* func_0006466C - no Driver 2 counterpart */
typedef struct NODE {
    struct NODE *next;
    char pad4[0xC];
    int *cell;
    char pad14[0x1B - 0x14];
    unsigned char f1B;
} NODE;

typedef struct {
    int f0;
    int f4;
    int f8;
    char padC[0x14 - 0xC];
    int f14;
    int f18;
    int f1C;
    char pad20[0x2C - 0x20];
    int f2C;
    char pad30[0x48 - 0x30];
    int f48;
    int f4C;
    char pad50[0x58 - 0x50];
    NODE *head;
    char pad5C[0x790 - 0x5C];
    int f790;
    char pad794[0x79C - 0x794];
    int f79C;
} OBJ;

typedef struct {
    unsigned char f0;
    unsigned char f1;
    unsigned char f2;
    unsigned char f3;
    unsigned char f4;
    unsigned char f5;
    char pad[0x50 - 6];
} ENT;

typedef struct {
    unsigned char v;
    char pad[7];
} B8;

extern B8 D_000976DC;
extern long long D_000976E8;
extern long long D_000976EC;
extern int D_00097748;
extern int D_0009774C;
extern ENT D_00098950[];
extern int car_data[];
extern int D_000C039C[];
extern unsigned char D_000C439C[];
extern int func_00063114(OBJ *obj, int x, int z, int idx);
extern int func_00063E54(OBJ *obj, ENT *e);
extern void func_00064360(OBJ *obj, int *dx, int *dz, int force);

static inline int dist(OBJ *obj)
{
    int a, b, c, d;
    int dx, dz;

    a = obj->f48;
    b = car_data[0xC8 / 4];
    dx = a - b;
    if (dx < 0)
        dx = b - a;
    c = obj->f4C;
    d = car_data[0xD0 / 4];
    dz = c - d;
    if (dz < 0)
        dz = d - c;
    if (dz < dx)
        return dx + dz / 2;
    return dz + dx / 2;
}

static inline int grid(int x, int z)
{
    int r;

    r = (z + 32) * 64 + 32;
    return x + r;
}

void func_0006466C(OBJ *obj)
{
    int dx;
    int dz;
    int i;
    int flag;
    int cnt;
    int k;
    NODE *p;
    NODE *next;
    int idx;
    int x;
    int z;
    int ret;
    ENT *e;
    int any;
    int cx, cz;

    D_000976DC.v = 0;
    e = D_00098950;
    for (i = 0; i < 22; i++, e++) {
        if (e->f1 != 0 && e->f5 != obj->f0 + 1 && func_00063E54(obj, e) != 0) {
            D_000C439C[D_000976DC.v++] = i;
        }
    }
    flag = 0;
    any = D_000976DC.v != 0;
    if (obj->f790 == 0 ||
        (obj->f14 != obj->f18 && (obj->f14 >= 2 || any)) ||
        (obj->f79C == 0 && dist(obj) > 3000))
        flag = 1;
    if (flag != 1)
        return;

    func_00064360(obj, &dx, &dz, 0);
    *(int *)&D_000976E8 = 0;
    if (obj->f1C - 2 < obj->f14)
        obj->f790 = 0;
    if (obj->f790 == flag) {
        cnt = 0;
        *(int *)&D_000976EC = 0;
        k = --obj->f14;
        p = (NODE *)&obj->head;
        while (k--)
            p = p->next;
        while (p != 0) {
            *(int *)&D_000976EC += 1;
            next = p->next;
            if (p->f1B == 0)
                cnt++;
            if (cnt >= 2) {
                if (next == 0 || next->f1B == 0)
                    break;
            }
            p = next;
        }
        if (*(int *)&D_000976EC >= obj->f1C - obj->f14)
            obj->f790 = 0;
        if (obj->f790 == 1) {
            k = obj->f14 + *(int *)&D_000976EC;
            p = (NODE *)&obj->head;
            while (k--) {
                if (p->next == 0)
                    goto fallback;
                p = p->next;
            }
            idx = p->cell - D_000C039C;
            cx = idx % 64 - 32 - (D_00097748 - obj->f4);
            cz = idx / 64 - 32 - (D_0009774C - obj->f8);
            idx = grid(cx, cz);
            ret = func_00063114(obj, cx, cz, idx);
            goto done;
        }
    }
fallback:
    idx = grid(-dx, -dz);
    x = idx % 64 - 32;
    z = idx / 64 - 32;
    ret = func_00063114(obj, x, z, idx);
done:
    if (ret == 1 && *(int *)&D_000976E8 >= 2) {
        obj->f2C = 60;
        obj->f14 = ret;
        obj->f18 = ret;
        obj->f1C = *(int *)&D_000976E8;
        obj->f790 = ret;
    } else {
        obj->f790 = 0;
    }
    obj->f4 = D_00097748;
    obj->f8 = D_0009774C;
}
