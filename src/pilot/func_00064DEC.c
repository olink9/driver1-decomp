/* func_00064DEC: walk a node path from obj->head, find the point `dist` units along it (approximate length
 * max + min*13/32) and write x/z to out; past the end, aim at D_000C4C6C. D_00097710 = next two nodes share s.
 * Fix (run3-r4): `ax = dx;` written before the dz subtraction and `if (dx < 0) ax = -dx;` after it. */
typedef struct N { struct N *next; int p4; int x; int z; int p10[2]; short s; } N;
typedef struct { char pad[0x758]; N *head; char pad2[0x790-0x75C]; int d; } S;
extern int D_00097710;
extern int D_000C4C6C[];

void func_00064DEC(int a, int dist, int *out)
{
    N *prev, *cur;
    int pz, px, dx, az, t, r0, r1;
    prev = ((S *)a)->head;
    if (!prev) { ((S *)a)->d = 0; return; }
    cur = prev->next;
    while (cur) {
        {int ax;
        px = prev->x;
        dx = cur->x - px;
        pz = prev->z;
        ax = dx;
        a = cur->z - pz;
        if (dx < 0) ax = -dx;
        if (a < 0) az = -a; else az = a;
        if (ax < az) { t = ax; ax = az; az = t; }
        az = ax + (az * 13 >> 5);}
        if (dist < az) goto done;
        prev = cur;
        cur = cur->next;
        dist -= az;
    }
    {int ax;
    dx = D_000C4C6C[0];
    px = prev->x;
    pz = prev->z;
    dx -= px;
    ax = dx;
    a = D_000C4C6C[2] - pz;
    if (dx < 0) ax = -dx;
    if (a < 0) az = -a; else az = a;
    if (ax < az) { t = ax; ax = az; az = t; }
    az = ax + (az * 13 >> 5);}
done:
    if (dist < 0) dist = 0;
    r0 = dx * dist / az;
    r1 = a * dist / az;
    out[0] = px + r0;
    out[2] = pz + r1;
    a = 0;
    if (prev && cur) {
        if (prev->s == cur->s && cur->next)
            a = prev->s == cur->next->s;
    }
    D_00097710 = a;
}
