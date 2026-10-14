/* Driver 2: ProcessTextureInfo */
extern int D_0009755C, D_00097558, D_0009758C, D_00097570;
extern int *D_00097564, *D_00097594, *D_00097548;
extern int D_000BE2F8[];
extern int *D_000BE508[];

void func_00051A40(int *lump) {
    int i;
    int n;
    int *ptr;
    D_0009755C = lump[0];
    D_00097558 = lump[1];
    D_00097564 = lump + 2;
    ptr = D_00097564 + (D_0009755C + 1) * 2;
    for (i = 0; i < D_0009755C; i++) {
        n = *ptr;
        ptr++;
        D_000BE2F8[i] = n;
        D_000BE508[i] = ptr;
        ptr += n * 2;
        D_00097558 = n;
    }
    D_0009758C = *ptr;
    D_00097594 = ptr + 1;
    ptr = ptr + 1 + 32;
    D_00097570 = *ptr;
    D_00097548 = ptr + 1;
}
