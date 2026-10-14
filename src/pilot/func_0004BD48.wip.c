/* best 93.26: remaining diff is register allocation - target keeps &D_0009B0A4 (s7) and r*396 (s6) as separate pseudos and spills a->h/2 to 0x20(sp); here the R[r] sum is CSE'd instead */
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

extern REGION D_0009B0A4[];

#define ABS(v) ((v) < 0 ? -(v) : (v))

int func_0004BD48(BOX *boxes, int i, int j, int r) {
    BOX unused;
    BOX *a = &boxes[i];
    BOX *b = &boxes[j];
    int maxx, maxz, maxy, minx, minz, miny;
    int sa, sb;
    int t;

    if (!(a->flags & 0x3ff) && !(b->flags & 0x3ff)
        && abs(D_0009B0A4[r].y + a->y) <= a->h / 2 && abs(D_0009B0A4[r].y + b->y) <= b->h / 2) {
        if (!(a->flags & 0x400)) {
            t = a->w;
            a->flags = 0x400;
            a->w = a->d;
            a->d = t;
        }
        if (!(b->flags & 0x400)) {
            t = b->w;
            b->flags = 0x400;
            b->w = b->d;
            b->d = t;
        }

        maxx = a->x + a->w;
        if (maxx < b->x + b->w) maxx = b->x + b->w;
        maxz = a->z + a->d;
        if (maxz < b->z + b->d) maxz = b->z + b->d;
        maxy = a->y + a->h / 2;
        if (maxy < b->y + b->h / 2) maxy = b->y + b->h / 2;
        minx = a->x - a->w;
        if (b->x - b->w < minx) minx = b->x - b->w;
        minz = a->z - a->d;
        if (b->z - b->d < minz) minz = b->z - b->d;
        miny = a->y - a->h / 2;
        if (b->y - b->h / 2 < miny) miny = b->y - b->h / 2;

        sa = (D_0009B0A4[r].x - a->x > 0) ? 1 : -1;
        sb = (D_0009B0A4[r].x - b->x > 0) ? 1 : -1;
        if (sa == sb) {
            t = a->x + sa * a->w - b->x - sa * b->w;
            if (t > -50 && t < 50 && ABS(a->z - b->z) < a->d + (b->d + 50))
                goto merge;
        }
        sa = (D_0009B0A4[r].z - a->z > 0) ? 1 : -1;
        sb = (D_0009B0A4[r].z - b->z > 0) ? 1 : -1;
        if (sa == sb) {
            t = a->z + sa * a->d - b->z - sa * b->d;
            if (t > -50 && t < 50 && ABS(a->x - b->x) < a->w + (b->w + 50)) {
merge:
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
