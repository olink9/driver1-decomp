/* GetSmokeDrift */
typedef struct { int vx, vy, vz, pad; } VECTOR;
extern int D_00096AB8;          /* SmokeCnt */
extern long long CameraCnt;    /* CameraCnt */
extern short rcossin_tbl[];      /* rcossin_tbl */
extern int D_00096C14, D_00096C18, D_00096C1C; /* SinTabIndex1..3 */
extern int D_000973F0, D_000973F4; /* SinX, CosX */
extern int D_00096C20;          /* WindMagnitude */

void GetSmokeDrift(VECTOR *Wind)
{
    if (D_00096AB8 != *(int *)&CameraCnt) {
        D_00096AB8 = *(int *)&CameraCnt;
        D_00096C14 += 3;
        D_00096C18 += 5;
        D_00096C1C += 7;
        D_00096C14 &= 0xfff;
        D_00096C18 &= 0xfff;
        D_00096C1C &= 0xfff;
        D_000973F0 = *(short *)((int)rcossin_tbl + D_00096C14 * 4) + *(short *)((int)rcossin_tbl + D_00096C18 * 4);
        D_000973F4 = *(short *)((int)rcossin_tbl + ((D_00096C14 + 0x400) & 0xfff) * 4) + *(short *)((int)rcossin_tbl + ((D_00096C18 + 0x400) & 0xfff) * 4);
        D_00096C20 = (short)*(unsigned short *)((int)rcossin_tbl + D_00096C1C * 4) >> 10;
    }
    Wind->vy = 0;
    Wind->vx = (D_00096C20 * D_000973F4) >> 12;
    Wind->vz = (-D_00096C20 * D_000973F0) >> 12;
}
