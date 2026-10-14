typedef struct { int vx, vy, vz, pad; } V;
typedef struct { int x, y, z; } I3;
typedef struct { char p0[0x38]; int a38; char p1[0x34]; I3 *p70; int o74, o78, o7c; } S;
extern V D_0008A8F0;
extern long long D_00096C48, D_00096C4A, D_00096C4C;
extern long long D_000970E4, D_00096A48;
extern void func_00061374();
extern int SquareRoot0();
extern int ratan2();
extern void func_00077D6C();
extern void func_0004EB7C();

void func_0003D6C0(S *s)
{
    V pos, d;
    V *cam = &D_0008A8F0;
    V *pp = &pos;

    func_00061374(&s->a38, cam);
    pos.vx = s->p70->x + s->o74;
    pos.vy = s->p70->y + s->o78;
    pos.vz = s->p70->z + s->o7c;
    d.vx = pos.vx - D_0008A8F0.vx;
    d.vy = pp->vy - cam->vy;
    d.vz = pp->vz - cam->vz;
    *(short *)&D_00096C48 = (0x400 - ratan2(SquareRoot0(d.vx * d.vx + d.vz * d.vz), d.vy)) & 0xfff;
    *(short *)&D_00096C4A = -ratan2(d.vx, d.vz) & 0xfff;
    *(short *)&D_00096C4C = 0;
    func_00077D6C(0x100);
    *(int *)&D_000970E4 = 0x100;
    *(int *)&D_00096A48 = 10000;
    func_0004EB7C();
}
