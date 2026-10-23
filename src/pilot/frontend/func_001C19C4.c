typedef struct { unsigned addr:24; unsigned len:8; } P_TAG;
typedef struct { int a; unsigned int **ot; } DB;
typedef struct { int pad0[2]; short h8; short pad1[4]; short h12; } T20;
typedef struct { int tag; int pad[9]; } P40;
extern DB *current;
extern T20 D_001CDCD8[];
typedef struct { P40 m[3]; } P120;
extern P120 D_001CF888[];
extern void memset();
extern void func_001C2014();

void func_001C19C4(void)
{
    char col[4];
    int i;
    memset(col, 0, 4);
    for (i = 7; i < 11; i++) {
        P40 *p = &D_001CF888[i].m[D_001CDCD8[i + 1].h8];
        ((P_TAG *)p)->addr = ((P_TAG *)current->ot[D_001CDCD8[i + 1].h12])->addr;
        ((P_TAG *)current->ot[D_001CDCD8[i + 1].h12])->addr = (unsigned)p;
    }
    func_001C2014(0, 0, col, 0x140, 0x30, 10);
    func_001C2014(0, 0x1B0, col, 0x140, 0x50, 10);
}
