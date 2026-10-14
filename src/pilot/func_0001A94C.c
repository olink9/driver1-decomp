struct S98950 { char a, b; char pad[0x4E]; };
struct S9B230 { char pad[0x18C]; };
extern struct S98950 D_00098950[];
extern struct S9B230 D_0009B230[];
extern void func_0001D608(void *, int);
void func_0001A94C(void) {
    int i;
    struct S9B230 *q = D_0009B230;
    struct S98950 *p = D_00098950;
    for (i = 0; i < 22; i++, q++, p++) {
        if (p->b != 0) func_0001D608(q, 0);
    }
}
