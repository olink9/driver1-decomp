struct S { short a, b, c, d; };
extern short D_0008C324[], D_0008C33C[];
void func_0006B97C(struct S *p) {
    short *s = D_0008C324, *t = D_0008C33C;
    struct S *q;
    struct S *end = p + 11;
    short v;
    for (q = p; q < end; q++) {
        v = *s++;
        q->a = v;
        q->b = v;
        q->c = *t++;
        q->d = 0;
    }
}
