extern int D_0009751C;
extern volatile char D_0009750C;
extern char D_0009750D;
extern int D_00097510;
extern int D_00097514;
extern void func_0007A580();
extern void func_0007A27C();
extern void func_0007A53C();
extern void func_0007A12C();
extern void func_000505E0();
extern void func_0005167C();
extern void sector_ready();

void loadsectors(int addr, int sector, int nsectors) {
    int pos[2];
    D_0009751C = addr;
    D_0009750C = 0;
    D_00097510 = sector;
    D_00097514 = nsectors;
    D_0009750D = D_0009750C;
    func_0007A580(sector, pos);
    func_0007A27C(0x1B, pos);
    func_0007A53C(func_0005167C);
    func_0007A12C(sector_ready);
    do { } while (D_0009750C == 0);
    func_000505E0();
}
