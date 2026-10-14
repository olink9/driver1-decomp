struct E { unsigned short a, b; int p; int c; int d; int e; };
struct F { int a, b, c; };
extern int D_000972BC;
extern struct E D_000A7EA4[];
extern struct F D_000A7F44;
extern void func_00035558();
void func_0003575C(void) {
    int i = D_000972BC;
    int a = D_000A7EA4[i].a;
    int b = D_000A7EA4[i].b;
    int c = D_000A7EA4[i].c;
    D_000A7F44.a = a;
    D_000A7F44.b = b;
    D_000A7F44.c = c;
    func_00035558(b, c);
}
