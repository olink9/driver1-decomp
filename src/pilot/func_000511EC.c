extern char D_000974FC;
extern void func_0007A3B0(int a, char *b, int c);
void func_000511EC(int a0) {
    if (a0)
        D_000974FC = 0x84;
    else
        D_000974FC = 0x80;
    func_0007A3B0(0xE, &D_000974FC, 0);
}
