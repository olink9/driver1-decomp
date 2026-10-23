/* func_001C1F40: PsyQ addPrim macro (P_TAG bitfield) on current->ot[k], plain for loop. */
typedef struct { unsigned addr:24; unsigned len:8; } P_TAG;
typedef struct { unsigned int tag; int pad[9]; } T_001C1F40;
typedef struct { int a; unsigned int **ot; } DB_001C1F40;
extern DB_001C1F40 *current;
extern T_001C1F40 D_001D2C70[];
extern T_001C1F40 D_001D2CC0[];
#define setaddr(p, _addr) (((P_TAG *)(p))->addr = (unsigned)(_addr))
#define getaddr(p) (unsigned)(((P_TAG *)(p))->addr)
#define addPrim(ot, p) setaddr(p, getaddr(ot)), setaddr(ot, p)

void func_001C1F40(int n)
{
    int i;
    for (i = 0; i < n; i++) {
        addPrim(current->ot[5], &D_001D2C70[i]);
        addPrim(current->ot[4], &D_001D2CC0[i]);
    }
}
