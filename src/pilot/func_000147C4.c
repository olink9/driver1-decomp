typedef struct { short x, y; } A;
typedef struct { short x, y; } E __attribute__((aligned(4)));
extern E D_0008EAA8[];
extern A D_00099B08[];
extern void func_000774C4();
void func_000147C4(void)
{
    A last, v;
    E e;
    A *out = D_00099B08;
    int i;
    func_000774C4(&last, 0, 4);
    for (i = 0; i < 0x1000; i += 0x20) {
        e = D_0008EAA8[i & 0xFFF];
        v.x = e.x;
        v.y = e.y;
        v.x = (v.x * 19 + 0x800) >> 12;
        v.y = (v.y * 19 + 0x800) >> 12;
        if (last.y != v.y || last.x != v.x) {
            out->x = v.x;
            out->y = v.y;
            out++;
            last.x = v.x;
            last.y = v.y;
        }
    }
}
