struct E { char pad[0x42]; short h; char pad2[0x64 - 0x44]; };
struct O { char pad[2]; unsigned char b; char pad2[0xC5]; char x[4]; int r; };
extern long long D_00096858;
extern struct E car_cosmetics[];
extern int MapHeight();
void func_00020A64(struct O *o) {
    int v;
    if (*(int *)&D_00096858) {
        v = MapHeight(o->x - 0) + 0xA3;
    } else {
        v = MapHeight(o->x - 0) - car_cosmetics[o->b].h - (o->b == 0);
    }
    o->r = v;
}
