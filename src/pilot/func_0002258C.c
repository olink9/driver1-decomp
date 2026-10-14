/* LoadGameLevel */
typedef struct { int v[8]; } CL;
extern long long D_000976CC, D_0009722C;
extern int D_00096834, D_00096804, D_00097098;
extern CL D_000BE0A4[];
extern void func_000511EC(), func_000512FC(), func_00022344(), func_00051ADC();
extern void func_0004C1C8(), func_00041E2C(), func_0002A5CC(), func_000317B0();

void func_0002258C(void)
{
    int sector;
    int nsectors;
    int s2;
    int s0;

    *(int *)&D_000976CC = 0;
    func_000511EC(0);
    sector = D_000BE0A4[D_00096834].v[0] / 2048;
    nsectors = D_000BE0A4[D_00096834].v[1] / 2048;
    func_000512FC(0xD8600, sector, nsectors);
    sector += nsectors;
    func_00022344(0xD8608, nsectors << 11);
    func_00051ADC(&sector);
    s2 = D_00097098;
    sector = D_000BE0A4[D_00096834].v[4] / 2048;
    nsectors = D_000BE0A4[D_00096834].v[5] / 2048;
    s0 = nsectors << 11;
    D_00097098 = s2 + s0;
    func_000512FC(s2, sector, nsectors);
    sector += nsectors;
    func_00022344(s2 + 8, s0);
    *(int *)&D_0009722C = D_000BE0A4[D_00096834].v[6];
    if (D_00096804 == 0) {
        func_0004C1C8();
        func_00041E2C();
        func_0002A5CC();
        func_000317B0();
    }
    func_000511EC(1);
}
