/* r3-13 (run2-r3) 94.20 (was 92.37). Cause of the old dx/ax register swap: global-alloc priority - ax (12 refs/34)
 * outranked dx (9/43), so ax took v1. Fixes: final block `dx = D[0]; ...; dx -= px;` (D[0] loaded before the base
 * addiu -> lui/addiu tie in v0 like the target, and +2 refs on dx), loop block `dx = cur->x; dx -= px;` (+2 refs).
 * Left: the loop block now loads cur->x straight into dx (v1) where the target loads into v0 and does
 * `subu v1,v0,t4` (= `dx = cur->x - px` single statement, which drops dx's refs and swaps v1/t0 back), and the
 * final block's delay slot (target `addu t0,v1; bgez; subu a0` vs ours `subu a0; bgez; addu t0`).
 * Tried: 25 abs() spellings, block-local ax in either/both blocks, decl orders, 16 load-order combos. */
typedef struct N { struct N *next; int p4; int x; int z; int p10[2]; short s; } N;
typedef struct { char pad[0x758]; N *head; char pad2[0x790-0x75C]; int d; } S;
extern int D_00097710;
extern int D_000C4C6C[];

void func_00064DEC(int a, int dist, int *out)
{
    N *prev, *cur;
    int pz, px, dx, dz, az, t, len, r0, r1;
    prev = ((S *)a)->head;
    if (!prev) { ((S *)a)->d = 0; return; }
    cur = prev->next;
    if (cur) {
    loop:
        {int ax;
        px = prev->x;
        dx = cur->x;
        dx -= px;
        pz = prev->z;
        a = cur->z - pz;
        if (dx < 0) ax = -dx; else ax = dx;
        if (a < 0) az = -a; else az = a;
        if (ax < az) { t = ax; ax = az; az = t; }
        az = ax + (az * 13 >> 5);}
        if (dist < az) goto done;
        prev = cur;
        cur = cur->next;
        dist -= az;
        if (cur) goto loop;
    }
    {int ax;
    dx = D_000C4C6C[0];
    px = prev->x;
    pz = prev->z;
    dx -= px;
    a = D_000C4C6C[2] - pz;
    if (dx < 0) ax = -dx; else ax = dx;
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
