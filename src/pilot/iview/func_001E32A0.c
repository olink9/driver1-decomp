typedef struct { int x, y, z; } V3;
typedef struct { V3 *p; V3 v; } UNDO;
extern int D_001E4038;
extern UNDO *D_001E40A0;

void func_001E32A0(void)
{
    V3 *p;

    while (D_001E4038 != 0) {
        p = D_001E40A0[D_001E4038 - 1].p;
        p->x = D_001E40A0[D_001E4038 - 1].v.x;
        p->y = D_001E40A0[D_001E4038 - 1].v.y;
        D_001E4038--;
        p->z = D_001E40A0[D_001E4038].v.z;
    }
}
