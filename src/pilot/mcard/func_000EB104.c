typedef struct { unsigned addr : 24; unsigned len : 8; } TAG;
typedef struct { TAG t; int pad[5]; } PRIM;
typedef struct { TAG *a; TAG *b; } DB;
typedef struct { short s; char c2; char c3; void *p4; char pad[0x3FC]; } H;
extern H D_000EE930[];
extern PRIM D_000EF9C0[];
extern DB *current;
extern void func_0005D164();

void func_000EB104(int n)
{
    if (D_000EE930[n].p4) {
        D_000EF9C0[n].t.addr = current->b[6].addr;
        current->b[6].addr = (unsigned)&D_000EF9C0[n];
        if (n == 0)
            func_0005D164(D_000EE930[0].p4, 0x24, 0x5E, 0xE00, 0xE00, 1);
        else
            func_0005D164(D_000EE930[n].p4, 0x144, 0x5E, 0xE00, 0xE00, 1);
    }
}
