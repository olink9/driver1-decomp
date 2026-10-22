extern char D_000BD498[];
void ClearChannelFields(int i) {
    int off;
    char *base;
    char *v; char *q;
    base = D_000BD498;
    off = i * 0x84;
    v = base + off;
    q = base + 0x74;
    *(int *)(v + 0x48) = -10000;
    *(short *)(v + 0x50) = 0x1000;
    v[0x40] = 0;
    v[0x41] = 0;
    *(short *)(v + 0x42) = 0;
    *(int *)(v + 0x44) = 0;
    *(int *)(v + 0x4C) = 0x1000;
    *(short *)(v + 0x52) = 0x1000;
    *(int *)(v + 0x5C) = 0;
    *(int *)(v + 0x60) = 0;
    *(int *)(v + 0x64) = 0;
    *(int *)(v + 0x68) = 0;
    *(char **)(v + 0x70) = q + off;
    *(int *)(v + 0x74) = 0;
    *(int *)(v + 0x78) = 0;
    *(int *)(v + 0x7C) = 0;
}
