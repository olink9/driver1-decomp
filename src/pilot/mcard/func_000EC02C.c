typedef struct { int a; short b; char pad[0x3A]; unsigned char idx; unsigned char n; char f42; char f43; } E;
typedef struct { unsigned short s; char c2; char c3; void *p4; E e[15]; } H;
extern H D_000EE930[];
extern unsigned int D_000EF144;
extern int D_000EF13C;
extern int D_000EF138;
extern int D_000EF140;
extern void func_000EC260();

/* save-slot grid input: d-pad moves the cursor (3 x 5), 0x20 switches card */
void func_000EC02C(int pad)
{
    if (D_000EF144 < 3) {
        if (!(D_000EE930[D_000EF13C].s & 2)) {
            D_000EF13C ^= 1;
            D_000EF140 = 0;
            D_000EF138 ^= 0x10;
            return;
        }
    } else {
        if (!(D_000EE930[D_000EF13C].s & 2) && !(D_000EE930[D_000EF13C].s & 4)) {
            D_000EF13C ^= 1;
            D_000EF140 = 0;
            D_000EF138 ^= 0x10;
            return;
        }
    }
    if (D_000EE930[0].c3)
        return;
    if (D_000EE930[1].c3)
        return;
    if (pad & 0x1000) {
        if (D_000EF140 >= 3)
            D_000EF140 -= 3;
    } else if (pad & 0x4000) {
        if (D_000EF140 < 12)
            D_000EF140 += 3;
    } else if (pad & 0x8000) {
        if (D_000EF140 > 0)
            D_000EF140--;
    } else if (pad & 0x2000) {
        if (D_000EF140 < 14)
            D_000EF140++;
    } else if (pad & 0x20) {
        if (D_000EF144 < 3) {
            if (!(D_000EE930[D_000EF13C ^ 1].s & 2))
                return;
            D_000EF13C ^= 1;
            D_000EF140 = 0;
            D_000EF138 ^= 0x10;
        } else {
            if (!(D_000EE930[D_000EF13C ^ 1].s & 2) && !(D_000EE930[D_000EF13C ^ 1].s & 4))
                return;
            D_000EF13C ^= 1;
            D_000EF140 = 0;
            D_000EF138 ^= 0x10;
        }
    } else {
        return;
    }
    func_000EC260();
}
