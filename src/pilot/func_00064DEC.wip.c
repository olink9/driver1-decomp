typedef struct N { struct N *next; int p4; int x; int z; int p10[3]; short s; } N;
typedef struct { char pad[0x758]; N *head; char pad2[0x790-0x75C]; int d; } S;
extern int D_00097710;
extern int D_000C4C6C[];

void func_00064DEC(int a, int dist, int *out)
{
    N *prev, *cur;
    int px, pz, dx, dz, ax, az, t, len, r0, r1;
    prev = ((S *)a)->head;
    if (!prev) { ((S *)a)->d = 0; return; }
    cur = prev->next;
    if (cur) {
        do {
            px = prev->x;
            pz = prev->z;
            dx = cur->x - px;
            a = cur->z - pz;
            ax = abs(dx);
            az = abs(a);
            if (ax < az) { t = ax; ax = az; az = t; }
            len = ax + (az * 13 >> 5);
            if (dist < len) goto done;
            prev = cur;
            cur = cur->next;
            dist -= len;
        } while (cur);
    }
    px = prev->x;
    pz = prev->z;
    dx = D_000C4C6C[0] - px;
    a = D_000C4C6C[2] - pz;
    ax = abs(dx);
    az = abs(a);
    if (ax < az) { t = ax; ax = az; az = t; }
    len = ax + (az * 13 >> 5);
done:
    if (dist < 0) dist = 0;
    r0 = dx * dist / len;
    r1 = a * dist / len;
    out[0] = px + r0;
    out[2] = pz + r1;
    a = 0;
    if (prev && cur) {
        if (prev->s == cur->s && cur->next)
            a = prev->s == cur->next->s;
    }
    D_00097710 = a;
}
