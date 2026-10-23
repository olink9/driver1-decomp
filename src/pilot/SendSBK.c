/* SendSBK */
extern int D_00097238, D_00097244, D_00097274, D_00097228, D_00097280, D_0009729C;
extern long long D_00096C88;
extern unsigned long D_000BCE74[], D_000BD484[];
extern void func_0004E40C(), func_0004E348(), SpuSetTransferMode(), SpuSetTransferStartAddr(), SpuRead();

/* Driver 2: SendSBK (SPOOL.C) */
void SendSBK(void)
{
    char *address;
    int slength;
    unsigned long i, n;
    unsigned long *source, *dest;

    address = (char *)(D_00097238 + 0x8000);
    address += (D_00097244 & 1) << 12;
    if (D_00097274 == 0) {
        address += 4;
        if (*(int *)&D_00096C88 != -1) {
            func_0004E40C();
            func_0004E348();
        }
        dest = D_000BCE74;
        D_00097228 = *(int *)address;
        n = D_00097228 << 2;
        source = (unsigned long *)(address + 4);
        for (i = 0; i < n; i++)
            dest[i] = source[i];
        address += D_00097228 * 16 + 4;
        slength = 0xFF8 - D_00097228 * 16;
        D_0009729C = D_000BD484[D_00097280];
    } else {
        slength = 0x1000;
    }
    SpuSetTransferMode(0);
    SpuSetTransferStartAddr(D_0009729C);
    SpuRead(address, (slength + 63) & -64);
    D_00097274++;
    D_0009729C += slength;
}
