/* func_00032F98 - no Driver 2 counterpart: count loaded neighbouring regions */
typedef struct {
    int x;
    int z;
} OFS;

extern long long D_00097320;
extern long long D_0009731C;
extern long long D_0009730C;
extern long long D_00097314;
extern long long D_0009732C;
extern long long D_0009733C;
extern long long D_00096A24;
extern long long D_00096834;
extern short D_0009728C[];
extern unsigned short *D_000972AC;
extern int D_000969E8;
extern int D_00097230;

#define cellX      (*(int *)&D_00097320)
#define cellZ      (*(int *)&D_0009731C)
#define regionX    (*(int *)&D_0009730C)
#define regionZ    (*(int *)&D_00097314)
#define regionIdx  (*(int *)&D_0009732C)
#define regionSize (*(int *)&D_0009733C)
#define regionsAll (*(int *)&D_00096A24)
#define gameMode   (*(int *)&D_00096834)

int func_00032F98(void)
{
    OFS ofs[19];
    int xs = 0;
    int zs = 0;
    int n = 0;
    int i;
    int count;
    int idx;
    int r;

    if (cellX < 9) {
        if (regionX != 0) {
            ofs[n].x = -1;
            ofs[n].z = 0;
            n++;
            xs = 1;
        }
    } else if (regionSize - 9 < cellX && regionX < regionsAll / regionSize) {
        ofs[n].x = 1;
        ofs[n].z = 0;
        n++;
        xs = 2;
    }
    if (cellZ < 9) {
        if (regionZ != 0) {
            ofs[n].x = 0;
            ofs[n].z = -1;
            n++;
            zs = 1;
        }
    } else if (regionSize - 9 < cellZ && regionZ != 0) {
        ofs[n].x = 0;
        ofs[n].z = 1;
        n++;
        zs = 2;
    }
    if (n == 2) {
        if (zs == 1) {
            n = 3;
            if (xs == 1) {
                ofs[2].x = -1;
                ofs[2].z = -1;
            } else {
                ofs[2].x = 1;
                ofs[2].z = -1;
            }
        } else {
            n = 3;
            if (xs == 1) {
                ofs[2].x = -1;
                ofs[2].z = 1;
            } else {
                ofs[2].x = 1;
                ofs[2].z = 1;
            }
        }
    }
    count = 0;
    for (i = 0; i < n; i++) {
        idx = regionIdx + ofs[i].x + ofs[i].z * (regionsAll / regionSize);
        r = D_0009728C[((regionX + ofs[i].x) & 1) + (((regionZ + ofs[i].z) & 1) << 1)];
        if (r != -1 && D_000972AC[idx] != 0xFFFF && r == idx)
            count++;
    }
    if (D_000969E8 != 0) {
        switch (D_00097230) {
        case 0:
            if (cellZ >= 25)
                count++;
            break;
        case 2:
            if (gameMode == 1 && regionIdx == 0xDF && cellX < 10)
                break;
            if (cellZ < 6)
                count++;
            break;
        case 1:
            if (cellX >= 25)
                count++;
            break;
        case 3:
            if (gameMode == 5 && regionIdx == 0x48) {
                if (cellX < 5)
                    count++;
            } else if (cellX < 6)
                count++;
            break;
        }
    }
    return count;
}
