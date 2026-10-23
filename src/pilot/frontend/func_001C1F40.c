typedef struct { unsigned int tag; int pad[9]; } T_001C1F40;
typedef struct { int a; unsigned int **ot; } DB_001C1F40;
extern DB_001C1F40 *current;
extern T_001C1F40 D_001D2C70[];
extern T_001C1F40 D_001D2CC0[];

void func_001C1F40(int n)
{
    int i = 0;
    if (n > 0) {
        DB_001C1F40 *c = current;
        do {
            D_001D2C70[i].tag = (D_001D2C70[i].tag & 0xFF000000) | (*c->ot[5] & 0xFFFFFF);
            *c->ot[5] = (*c->ot[5] & 0xFF000000) | ((unsigned int)&D_001D2C70[i] & 0xFFFFFF);
            D_001D2CC0[i].tag = (D_001D2CC0[i].tag & 0xFF000000) | (*c->ot[4] & 0xFFFFFF);
            *c->ot[4] = (*c->ot[4] & 0xFF000000) | ((unsigned int)&D_001D2CC0[i] & 0xFFFFFF);
            i++;
        } while (i < n);
    }
}
