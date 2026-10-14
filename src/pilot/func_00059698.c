typedef struct { char pad[0x14]; int v14; char pad2[9]; char c21; char c22; char c23; } E;
extern E *D_00097608;
extern E *D_00096DB4;
extern void func_00058B2C();
extern void func_00058DE0();

void func_00059698(void) {
    int i;
    char pad[40];
    for (i = 0; i < 60; i++) {
        D_00097608[i].v14 = 100000;
        D_00097608[i].c23 = i;
        D_00097608[i].c21 = 0x7F;
        D_00097608[i].c22 = 0xFF;
    }
    D_00096DB4 = 0;
    func_00058B2C(0);
    func_00058DE0(-1);
    D_00096DB4->c21 = 0x7F;
    D_00096DB4->c22 = 0xFF;
}
