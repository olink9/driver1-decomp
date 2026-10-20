typedef struct N { struct N *next; int p1, x, z; } N;
typedef struct {
    char pad0[0x14]; int idx; int pad18; int count; char pad20[0x38]; N *head;
    char pad5c[0x6fc]; N *cur; int px; int pad760; int pz; char pad768[0x28]; int active;
} S;

int func_00064C90(S *s)
{
    int px = s->px;
    int pz = s->pz;
    int i, len, t, adx, adz, dz, dx, az, ax;
    N *b, *a;
    if (s->active == 0)
        return 0;
    i = s->idx;
    i--;
    a = (N *)((char *)s + 0x58);
    while (i != -1) {
        a = a->next;
        i--;
    }
    b = a->next;
    while (b) {
        ax = a->x;
        az = a->z;
        dx = b->x - ax;
        dz = b->z - az;
if (dx < 0) adx = -dx; else adx = dx; if (dz < 0) adz = -dz; else adz = dz;
        if (adx < adz) {
            int tmp = adx;
            adx = adz;
            adz = tmp;
        }
        len = adx + (adz * 13 >> 5);
        if (len > 0) {
            t = ((px - ax) * dx + (pz - az) * dz) / len;
            if (t < len) {
                s->cur = a;
                return t;
            }
        }
        if (s->count - 3 < s->idx)
            break;
        a = b;
        s->idx++;
        b = b->next;
    }
    s->active = 0;
    return 0;
}
