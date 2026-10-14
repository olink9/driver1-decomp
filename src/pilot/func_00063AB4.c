struct A { char pad[0x78C]; int z; char pad2[0x14]; unsigned char *p; };
struct B { char pad[5]; char f; char pad2[0x4A]; };
extern struct B D_00098950[];
extern int D_00097764, D_00097744;
void func_00063AB4(struct A *a) {
    D_00098950[*a->p - 1].f = 0;
    a->z = 0;
    if (--D_00097764 == 0 && D_00097744 < 256)
        D_00097744 = 256;
}
