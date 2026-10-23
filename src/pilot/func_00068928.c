/* mission script command parser (no Driver 2 counterpart) */
typedef struct { int w[11]; } CP;

typedef struct {
    char pad0[0x58];
    CP saved;
    CP cur;
    char padB0[0x12C - 0xB0];
    int f12C;
    char pad130[0x2A4 - 0x130];
    int x, y, z;
    char pad2B0[0x2DE - 0x2B0];
    unsigned char f2DE;
    unsigned char f2DF;
    unsigned char f2E0;
} MS;

extern MS D_000C6F04;
extern int D_000977A8;
extern int D_000977A4;
extern long long gNight;
extern long long D_00096AA8;
extern long long D_00096834;
extern long long D_00096898;
extern long long D_000970EC;
extern long long D_0009683C;
extern long long D_000967BF;
extern long long D_00096810;
extern long long D_00096F80;
extern int *func_000687D4();

void func_00068928(int *pp, int *end)
{
    int v;
    int *p = pp;

    while (p < end) {
        switch (*p) {
        case 19:
            D_000C6F04.f2DE = 1;
            D_000C6F04.x = p[1];
            D_000C6F04.z = p[2];
            break;
        case 52:
            D_000C6F04.f12C = p[1];
            break;
        case 40:
            D_000977A8 = 1;
        case 31:
            *(int *)&gNight = 1;
            break;
        case 39:
            D_000977A4 = 1;
            break;
        case 30:
            *(char *)&D_00096AA8 = 0x80;
            break;
        case 34:
            *(int *)&D_00096834 = p[1];
            break;
        case 61:
            *(char *)&D_00096898 = 3;
            break;
        case 63:
            *(int *)&D_000970EC = 1;
            break;
        case 64:
            *(int *)&D_0009683C = p[1];
            break;
        case 66:
            ((char *)&D_00096AA8)[1] = 0x80;
            break;
        case 67:
            *(char *)&D_000967BF = p[1];
            break;
        case 84:
            D_000C6F04.f2DF = p[1];
            break;
        case 89:
            if (((char *)&D_00096898)[1] == 0)
                D_000C6F04.cur = D_000C6F04.saved;
            D_000C6F04.f2DE = 1;
            D_000C6F04.f2E0 = 1;
            D_000C6F04.x = D_000C6F04.cur.w[0];
            D_000C6F04.y = D_000C6F04.cur.w[1];
            D_000C6F04.z = D_000C6F04.cur.w[2];
            D_000C6F04.f12C = D_000C6F04.cur.w[3];
            break;
        case 105:
            {
                int **tbl = (int **)&D_000C6F04;
                v = p[1];
                if (v < 0)
                    v = ~v;
                tbl[v] = p + 2;
            }
            break;
        case 110:
            *(int *)&D_00096810 = p[1];
            break;
        case 108:
            *(int *)&D_00096F80 = 1;
            break;
        }
        p = func_000687D4(p);
    }
}
