struct S { unsigned short a, b, c, d; };
extern unsigned short D_0008C324[], D_0008C33C[];
void func_0006B97C(struct S *p) {
    unsigned short *s = D_0008C324, *t = D_0008C33C;
    unsigned short v;
    struct S *end = (struct S *)((char *)p + 0x58);
    for (; p < end; p++) {
        v = *s++;
        p->a = v;
        p->b = v;
        p->d = 0;
        p->c = *t++;
    }
}
