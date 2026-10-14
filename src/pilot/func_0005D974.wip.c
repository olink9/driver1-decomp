typedef struct { char pad0[0x18]; short s18; char pad1a[2]; short s1c; char pad1e[4]; short s22; char pad24[0x14];
 void *p38; short s3c; short s3e_unused; short s40; char pad42[6]; char c48; char pad49[3]; int i4c; char c50; char pad51[4]; unsigned char c55; unsigned char c56; } OBJ;
extern long long D_000C015C, D_000C0164, D_000C0168;
extern char D_0008B8BC[], D_0008B8F8[], D_0008B934[], D_0008B970[], D_0008B9AC[];
extern int func_0004FFE8();
/* Driver 2 name: unknown */
void func_0005D974(OBJ *o)
{
    int a, b;
    o->s40 = 0x800;
    o->s18 = 0;
    o->s1c = 0;
    a = func_0004FFE8(1);
    b = func_0004FFE8(1);
    o->c48 = 0;
    o->s22 = 10;
    o->c50 = 10;
    o->c55 = a % 5 + (b % 5) * 16;
    switch (o->c56) {
    case 3:
        o->i4c = *(int *)&D_000C0164;
        o->p38 = D_0008B934;
        o->c50 = 30;
        break;
    case 6:
        o->i4c = *(int *)&D_000C0168;
        break;
    case 0:
    default:
        o->i4c = *(int *)&D_000C015C;
        break;
    }
    if (o->c56 == 4) {
        o->p38 = D_0008B9AC;
    } else if (o->c56 != 3) {
        switch (func_0004FFE8(1) % 2) {
        case 0: o->p38 = D_0008B8BC; break;
        case 1: o->p38 = D_0008B8F8; break;
        case 2: o->p38 = D_0008B970; break;
        }
    }
}
