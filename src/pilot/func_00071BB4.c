/* XM_DoFullUpdate */
typedef struct { int pad0; int hdr; char pad1[0x2C]; unsigned char play; char pad2[0xB98 - 0x35]; int jup; } Song;
typedef struct { char pad[0x10]; unsigned short vb; } Dummy;
extern Song *D_000C7888[];
extern int D_000C7970[];
extern Song *D_00097814;
extern int D_00097818;
extern void func_00072684(), func_0007232C(), func_0007282C(), func_00073208(), func_00071C74();
void func_00071BB4(int sc)
{
    unsigned char s0;
    D_00097814 = D_000C7888[sc];
    s0 = D_00097814->play;
    if (s0 == 1) {
        D_00097818 = D_000C7970[D_00097814->hdr];
        if (D_00097814->jup == 0) {
            func_00072684();
            func_0007232C();
        }
        D_00097814->jup = 0;
        func_0007282C();
        if (*(unsigned short *)((char *)D_00097814 + 0x10) == s0)
            func_00073208();
        func_00071C74(sc);
    }
}
