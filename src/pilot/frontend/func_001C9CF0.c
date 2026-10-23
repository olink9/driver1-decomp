typedef struct { int x, y, z; } V3s;
typedef struct { int x, y, z, pad; } V4s;
typedef struct { short vx, vy, vz, pad; } SVEC;
typedef struct { int pos[3]; int dir[3]; int angle; } CamPt;
extern V3s camera_position, D_001D31C0;
extern V4s D_001D31D0, D_001D31F0, D_001D31E0;
extern SVEC D_00096C48;
extern CamPt *D_001D31B8;
extern int D_001D320C;
extern short rcossin_tbl[];
extern void func_001C9E58();
extern int ratan2();

void func_001C9CF0(void)
{
    V4s a, b;
    CamPt *p = D_001D31B8;
    int ang;

    D_001D31C0.x = camera_position.x = p->pos[0];
    D_001D31C0.y = camera_position.y = p->pos[1];
    D_001D31C0.z = camera_position.z = p->pos[2];
    D_001D31D0.x = a.x = p->dir[0];
    D_001D31D0.y = a.y = p->dir[1];
    D_001D31D0.z = a.z = p->dir[2];
    ang = p->angle & 0xfff;
    D_001D31F0.x = b.x = rcossin_tbl[ang * 2];
    D_001D31F0.y = b.y = -rcossin_tbl[ang * 2 + 1];
    D_001D31F0.z = b.z = 0;
    func_001C9E58(&a, &b);
    D_001D31E0.x = D_00096C48.vx = 0;
    D_001D31E0.y = D_00096C48.vy = ratan2(camera_position.x - a.x, a.z - camera_position.z) & 0xfff;
    if (D_001D320C == 0)
        D_001D31B8--;
    else
        D_001D31B8++;
}
