typedef struct { short vx, vy, vz, pad; } SVEC;
extern short D_0008EAAA[];
extern SVEC D_000C6CF8[];

void func_00066CCC(void)
{
    int i;

    for (i = 0; i < 0x12; i += 2) {
        D_000C6CF8[i].vy = 100;
        D_000C6CF8[i].vx = (D_0008EAAA[(i * 256 & 0xFFF) * 2] * 512 + 0x800) >> 12;
        D_000C6CF8[i].vz = (D_0008EAAA[(i * 256 & 0xFFF) * 2 - 1] * 512 + 0x800) >> 12;
        D_000C6CF8[i + 1].vy = -160;
        D_000C6CF8[i + 1].vx = (D_0008EAAA[(i * 256 + 0x80 & 0xFFF) * 2] * 490 + 0x800) >> 12;
        D_000C6CF8[i + 1].vz = (D_0008EAAA[(i * 256 + 0x80 & 0xFFF) * 2 - 1] * 490 + 0x800) >> 12;
    }
    for (i = 0x12; i < 0x24; i += 2) {
        D_000C6CF8[i].vy = -160;
        D_000C6CF8[i].vx = (D_0008EAAA[(i * 256 + 0x80 & 0xFFF) * 2] * 490 + 0x800) >> 12;
        D_000C6CF8[i].vz = (D_0008EAAA[(i * 256 + 0x80 & 0xFFF) * 2 - 1] * 490 + 0x800) >> 12;
        D_000C6CF8[i + 1].vy = -400;
        D_000C6CF8[i + 1].vx = (D_0008EAAA[(i * 256 + 0x100 & 0xFFF) * 2] * 330 + 0x800) >> 12;
        D_000C6CF8[i + 1].vz = (D_0008EAAA[(i * 256 + 0x100 & 0xFFF) * 2 - 1] * 330 + 0x800) >> 12;
    }
    for (i = 0x24; i < 0x36; i += 2) {
        D_000C6CF8[i].vy = -400;
        D_000C6CF8[i].vx = (D_0008EAAA[(i * 256 + 0x100 & 0xFFF) * 2] * 330 + 0x800) >> 12;
        D_000C6CF8[i].vz = (D_0008EAAA[(i * 256 + 0x100 & 0xFFF) * 2 - 1] * 330 + 0x800) >> 12;
        D_000C6CF8[i + 1].vy = -512;
        D_000C6CF8[i + 1].vx = (D_0008EAAA[(i * 256 + 0x180 & 0xFFF) * 2] * 100 + 0x800) >> 12;
        D_000C6CF8[i + 1].vz = (D_0008EAAA[(i * 256 + 0x180 & 0xFFF) * 2 - 1] * 100 + 0x800) >> 12;
    }
}
