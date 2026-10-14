/* func_0001EEB4 - no Driver 2 counterpart (FindSurface-like: map height + normal) */

typedef struct { int vx, vy, vz, pad; } VEC;

typedef struct {
    char pad0[0x20];
    int nx;
    int ny;
    int nz;
    int d;
    short type;
    short pad32;
} SUB;

typedef struct {
    char pad0[6];
    short count;
    SUB sub[1];
} SURF;

extern int D_0009E714[];
extern long long D_00096834;
extern long long D_00097374;
extern long long D_00096858;
extern long long D_000970FC;
extern long long D_000970DC;
typedef struct { unsigned char v; char pad[7]; } B8;
extern B8 D_000970F8;
extern int D_00096FF8;
extern int D_00096FFC;
extern int D_00097004;
extern char D_00097028;
extern int *D_0009D8F4[];
extern SURF *D_0009D904[];
extern unsigned short *D_000A0E20[];
extern short D_0008EAA8[];
extern int func_00024418();
extern int func_00024370();

int func_0001EEB4(VEC *pos, VEC *normal, VEC *out, int flag)
{
    int x, z;
    int cx, cz;
    int tile;
    int q;
    unsigned int info;
    short h;
    SURF *surf;
    SUB *sp;
    int n;
    int ht;
    int rx, rz;
    int nx, ny, nz, t;
    int pad[4];
    int px, pz;

    px = pos->vx;
    out->vx = px;
    x = D_0009E714[2] + px;
    pz = pos->vz;
    out->vz = pz;
    t = pz - 750;
    z = D_0009E714[3] - t;
    cx = x / 1500;
    cz = z / 1500;

    if ((unsigned)cx < D_0009E714[0] && (unsigned)cz < D_0009E714[1]) {
        cx = (cx & 0x7f) + ((cx & ~0x7f) >> 4);
        if (cx >= 120)
            cx -= 120;
        cz = (cz & 0x7f) + ((cz & ~0x7f) >> 4);
        if (cz >= 120)
            cz -= 120;

        q = 0;
        if (cx >= 60) {
            cx -= 60;
            q = 1;
        }
        if (cz >= 60) {
            cz -= 60;
            if (!(*(int *)&D_00097374 & 1))
                q += 2;
        } else {
            if (*(int *)&D_00097374 & 1)
                q += 2;
        }

        info = ((int (*)[60])D_0009D8F4[q])[cz][cx];
        h = info;
        if (!*(int *)&D_00096858)
            h = info & 0xfff8;

        if (info & 4)
            D_00096FF8 = 1;

        D_00096FFC = (D_000A0E20[(info >> 16) & 0x3ff][1] >> 14) & 1;
        if (D_00096FFC && !flag)
            D_00097028 = 1;

        if (!flag && (D_000A0E20[(info >> 16) & 0x3ff][0] & 0x80) && pos->vy < 220) {
            out->vy = D_0008EAA8[(*(int *)&D_000970FC & 0xfff) * 2] >> 7;
            if (!D_00097004)
                out->vy = out->vy - (100 - *(int *)&D_000970DC) / 2;
            normal->vy = 4096;
            normal->vx = 0;
            normal->vz = 0;
            D_000970F8.v = 1;
            return 4096;
        }

        D_000970F8.v = 0;
        tile = (info >> 16) & 0x3ff;
        if (tile < 900)
            surf = D_0009D904[tile];
        else
            surf = 0;

        if (surf) {
            rx = x % 1500 - 750;
            rz = z % 1500 - 750;
            sp = surf->sub;
            if (info & 0x40000000) {
                x = rz;
                z = rx;
            } else {
                x = rx;
                z = -rz;
            }
            if (info & 0x80000000) {
                x = -x;
                z = -z;
            }
            for (n = surf->count; n > 0; n--, sp++) {
                int r;
                if (sp->type == 4)
                    r = func_00024418(x, z, sp);
                else
                    r = func_00024370(x, z, sp);
                if (r) {
                    nx = sp->nx;
                    nz = sp->nz;
                    ny = sp->ny;
                    ht = (x * nx + z * nz) / 4096 - sp->d;
                    ht -= h;
                    if (info & 0x40000000) {
                        t = nx;
                        nx = nz;
                        nz = -t;
                    }
                    if (!(info & 0x80000000)) {
                        nx = -nx;
                        nz = -nz;
                    }
                    normal->vx = nx;
                    normal->vy = ny;
                    normal->vz = nz;
                    out->vy = ht;
                    if (D_00096FFC) {
                        out->vy = ht + (D_0008EAA8[((x + z) << 2) & 0x1ffc] >> 9);
                        return 2048;
                    }
                    return 4096;
                }
            }
        }

        if (*(int *)&D_00096834 == 1 && out->vx > -0x543A8 && out->vx < -0x50334 &&
            out->vz <= 0x412F7 && out->vz > 0x3D284)
            out->vy = -1500;
        else
            out->vy = -h;
        normal->vx = 0;
        normal->vy = 4096;
        normal->vz = 0;
        if (D_00096FFC) {
            out->vy += D_0008EAA8[((x + z) << 2) & 0x1ffc] >> 9;
            return 2048;
        }
        return 4096;
    }
    normal->vx = 0;
    normal->vy = 4096;
    normal->vz = 0;
    out->vy = 0;
    return 4096;
}
