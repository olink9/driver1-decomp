/* Driver 2: ProcessTextureInfo */
extern int D_0009755C;
extern int D_00097558, D_0009758C, D_00097570;
extern int *D_00097564, *D_00097594, *D_00097548;
extern int D_000BE2F8[];
extern int *D_000BE508[];
void func_00051A40(int *ptr) {
    int i; int k;
    D_0009755C = *ptr;
    D_00097558 = *(int *)((char *)ptr + 4);
    ptr += 2;
    D_00097564 = ptr;
    ptr += (D_0009755C + 1) * 2;
    for (i = 0; i < D_0009755C; i++) {
        k = *ptr;
        ptr++;
        D_000BE2F8[i] = k;
        D_000BE508[i] = ptr;
        ptr += k * 2;
        D_00097558 = k;
    }
    D_0009758C = *ptr;
    D_00097594 = ptr + 1;
    ptr = ptr + 1 + 32;
    D_00097570 = *ptr;
    D_00097548 = ptr + 1;
}
