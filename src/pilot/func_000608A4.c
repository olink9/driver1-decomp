struct O { char pad[0x48]; unsigned char a; char pad2[5]; unsigned char b; };
struct O2 { char pad[0x48]; unsigned char a; char pad2[3]; unsigned char *p; char pad3[4]; unsigned char b; };
extern int func_0004FFE8();
void func_000608A4(struct O2 *o) {
    if (o->b == 0) {
        o->a += o->p[1];
        if (o->a >= (o->p[0] - 1) * 2) {
            o->a = 0;
            o->b = func_0004FFE8(1);
        }
    } else {
        o->b = o->b - 1;
    }
}
