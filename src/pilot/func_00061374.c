typedef struct { int s[3]; int pad[3]; int e[3]; char pad2[0x34 - 0x24]; short t; unsigned char flags; } LERP;

void func_00061374(LERP *p, int *out) {
    int t = p->t;
    if (p->flags & 0x10) return;
    out[0] = p->s[0] + ((t * (p->e[0] - p->s[0]) + 0x800) >> 12);
    out[1] = p->s[1] + ((t * (p->e[1] - p->s[1]) + 0x800) >> 12);
    out[2] = p->s[2] + ((t * (p->e[2] - p->s[2]) + 0x800) >> 12);
}
