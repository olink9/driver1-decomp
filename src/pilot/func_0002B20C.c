/* func_0002B20C: clipAgainstZ (Driver 2, SHADOW.C) */
typedef struct { short vx, vy, vz, pad; } SVECTOR;

extern SVECTOR D_000A1C58[];   /* cv */
extern int D_00097170;         /* lastcv */
extern int D_00097160;         /* numcv */

void func_0002B20C(void)
{
    SVECTOR *curr;
    SVECTOR *prev;
    SVECTOR *dst;
    int flags;
    int srccount;
    int dstcount;
    int q;
    int pz;
    int d;

    dstcount = 0;
    curr = D_000A1C58 + D_00097170;
    srccount = D_00097160;
    prev = curr - srccount + 1;
    dst = curr + 2;
    flags = (prev->vz > 0) << 1;
    for (srccount--; srccount >= 0; srccount--) {
        q = curr->vz;
        flags >>= 1;
        if (q > 0)
            flags |= 2;
        switch (flags) {
        case 0:
            break;
        case 2:
            pz = prev->vz;
            d = pz - q;
            dst->vx = (curr->vx * pz - prev->vx * q) / d;
            dst->vy = (curr->vy * pz - prev->vy * q) / d;
            dst->pad = (curr->pad * pz - prev->pad * q) / d;
            dst->vz = 0;
            dst--;
            *dst = *curr;
            dst--;
            dstcount += 2;
            break;
        case 1:
            pz = prev->vz;
            d = pz - q;
            dst->vx = (curr->vx * pz - prev->vx * q) / d;
            dst->vy = (curr->vy * pz - prev->vy * q) / d;
            dst->pad = (curr->pad * pz - prev->pad * q) / d;
            dst->vz = 0;
            dst--;
            dstcount++;
            break;
        default:
            *dst = *curr;
            dst--;
            dstcount++;
            break;
        }
        prev = curr;
        curr--;
    }
    D_00097160 = dstcount;
    D_00097170 += 2;
}
