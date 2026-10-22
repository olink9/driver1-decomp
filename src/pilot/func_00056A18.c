typedef struct { int x, y, z; } V3;
typedef struct {
    int pad0[3];
    int flags;
    int p10;
    int pad14[3];
    V3 off;
} OBJ;
extern int D_0008B4F8[];
extern long long CameraCnt;
extern V3 D_000BE9B4;
extern void func_00067E58(), func_00056754();

void func_00056A18(OBJ *o)
{
    V3 v;
    int *s1 = D_0008B4F8;
    int fl;

    if (o != 0) {
        fl = o->flags;
        if ((fl & 0x20) || o->p10 == 0)
            return;
        func_00067E58(o, &v);
        v.x += o->off.x;
        v.y += o->off.y;
        v.z += o->off.z;
        if (fl & 4)
            s1 += 3;
        else if (fl & 2)
            s1 += 6;
        else if (fl & 8)
            s1 += 9;
    } else {
        v.x = D_000BE9B4.x;
        v.y = D_000BE9B4.y;
        v.z = D_000BE9B4.z;
        fl = 0;
    }
    v.y -= (*(int *)&CameraCnt % 16) * 64;
    func_00056754(s1, &v, fl & 0x80);
}
