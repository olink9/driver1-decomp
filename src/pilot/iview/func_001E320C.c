typedef struct { char type, on, b2, b3, b4, flag; char pad6[0x10 - 6]; char b10, b11; char pad[0x50 - 0x12]; } ENT;
typedef struct { int x, y, z; } V3;
typedef struct { V3 *p; V3 v; } UNDO;
extern ENT D_00099030[];
extern int D_001E42C8;
extern int D_001E4038;
extern UNDO *D_001E40A0;
extern char *D_00097098;
extern void ClearMem(unsigned char *p, int n);
extern void func_0003EB0C();

void func_001E320C(void)
{
    D_001E42C8 = -1;
    ClearMem((unsigned char *)D_00099030, 0x50);
    D_00099030[0].b2 = 1;
    D_00099030[0].b10 = 0;
    D_00099030[0].b11 = 0;
    D_00099030[0].on = 0;
    D_00099030[0].type = 0x16;
    D_00099030[0].flag = 0;
    func_0003EB0C(0x17, 0);
    D_001E4038 = 0;
    D_001E40A0 = (UNDO *)D_00097098;
    D_00097098 += 0xC80;
}
