struct E { char pad[0x42]; short h; char pad2[0x64 - 0x44]; };
struct O { char pad[2]; unsigned char b; char pad2[0xC5]; char x[4]; int r; };
extern long long D_00096858;
extern struct E D_0009AF04[];
extern int func_0001F5B0();
void func_00020A64(struct O *o) {
    int v;
    if (*(int *)&D_00096858) {
        v = func_0001F5B0(o->x - 0) + 0xA3;
    } else {
        v = func_0001F5B0(o->x - 0) - D_0009AF04[o->b].h - (o->b == 0);
    }
    o->r = v;
}
