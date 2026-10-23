extern char D_000974FC;
extern void CdControl(int a, char *b, int c);
void func_000511EC(int a0) {
    if (a0)
        D_000974FC = 0x84;
    else
        D_000974FC = 0x80;
    CdControl(0xE, &D_000974FC, 0);
}
