/* FAKEMATCH: `rg = 0;` after the second y test is a dead store whose only job is to invalidate CSE's
 * copy of the region address sum (base + r*396), so the .x access recomputes it from the still-live
 * &car_data (s7) and r*396 (s6) pseudos as the target does. No natural reassignment of rg was found
 * (re-assigning rg = base + r, rg += r on a copy, assignment inside the condition all get CSE'd away).
 * Driver 2 name unknown. Merges two axis-aligned boxes b into a when they touch along x or z
 * (run3-r4: MAX/MIN macros fixed the load order, duplicated merge bodies (cross-jumped) fixed the
 * priorities, nested `if (...) return 0;` fixed the first branch's delay slot, `base = D; rg = base + r`
 * kept the full &D pseudo). */
typedef struct {
    int x, y, z, pad;
    int w, d;
    int flags;
    int h;
} BOX;

typedef struct {
    char pad[0x18];
    int x, y, z;
    char pad2[396 - 0x24];
} REGION;

extern REGION car_data[];

#define MAX(A,B) ((A) > (B) ? (A) : (B))
#define MIN(A,B) ((A) < (B) ? (A) : (B))
#define ABS(v) ((v) < 0 ? -(v) : (v))

int func_0004BD48(BOX *boxes, int i, int j, int r) {
    BOX unused;
    BOX *a = &boxes[i];
    BOX *b = &boxes[j];
    int maxx, maxz, maxy, minx, minz, miny;
    int sa, sb;
    int t;
    int tmp;
    REGION *rg;
    REGION *base;

    if (a->flags & 0x3ff) return 0;
    if (b->flags & 0x3ff) return 0;

    base = car_data;
    rg = base + r;
    if (abs(rg->y + a->y) <= a->h / 2) {
        if (abs(rg->y + b->y) > b->h / 2) return 0;
        rg = 0; /* FAKEMATCH: dead store, see header */
        if (!(a->flags & 0x400)) {
            tmp = a->w;
            a->flags = 0x400;
            a->w = a->d;
            a->d = tmp;
        }
        if (!(b->flags & 0x400)) {
            tmp = b->w;
            b->flags = 0x400;
            b->w = b->d;
            b->d = tmp;
        }

        maxx = MAX(a->x + a->w, b->x + b->w);
        maxz = MAX(a->z + a->d, b->z + b->d);
        maxy = MAX(a->y + a->h / 2, b->y + b->h / 2);
        minx = MIN(a->x - a->w, b->x - b->w);
        minz = MIN(a->z - a->d, b->z - b->d);
        miny = MIN(a->y - a->h / 2, b->y - b->h / 2);

        t = car_data[r].x;
        sa = (t - a->x > 0) ? 1 : -1;
        sb = (t - b->x > 0) ? 1 : -1;
        if (sa == sb) {
            t = a->x + sa * a->w - b->x - sa * b->w;
            if (t > -50 && t < 50 && ABS(a->z - b->z) < a->d + (b->d + 50))
            {
                a->x = (maxx + minx) / 2;
                a->y = (maxy + miny) / 2;
                a->z = (maxz + minz) / 2;
                a->w = (maxx - minx) / 2;
                a->d = (maxz - minz) / 2;
                a->h = ABS(maxy - miny);
                b->w = 0;
                return 1;
            }
        }
        t = car_data[r].z;
        sa = (t - a->z > 0) ? 1 : -1;
        sb = (t - b->z > 0) ? 1 : -1;
        if (sa == sb) {
            t = a->z + sa * a->d - b->z - sa * b->d;
            if (t > -50 && t < 50 && ABS(a->x - b->x) < a->w + (b->w + 50)) {
                a->x = (maxx + minx) / 2;
                a->y = (maxy + miny) / 2;
                a->z = (maxz + minz) / 2;
                a->w = (maxx - minx) / 2;
                a->d = (maxz - minz) / 2;
                a->h = ABS(maxy - miny);
                b->w = 0;
                return 1;
            }
        }
    }
    return 0;
}
