typedef struct M {
    int w0, w1, w2, w3;
    int off10, off14, off18, off1c, off20;
} M;
extern long long D_0009712C;
extern long long D_00097370;
extern int modelpointers[];

void func_00052434(int *p)
{
    int n, i;
    int *q;
    n = *p++;
    *(int *)&D_0009712C = n;
    *(int *)&D_00097370 = n;
    for (i = 0; i < n; i++) {
        int sz = *p++;
        modelpointers[i] = (int)p;
        p = (int *)((char *)p + sz);
    }
    for (i = 0; i < n; i++) {
        char *a = (char *)modelpointers[i];
        if (*(int *)(a - 4) != 0 && *(short *)(a + 4) != -1) {
            char *b = (char *)modelpointers[*(short *)(a + 4)];
            if (*(int *)(b + 0x20) != 0)
                *(int *)(a + 0x20) = *(int *)(b + 0x20) + (int)b;
            *(int *)(a + 0x10) = *(int *)(b + 0x10) + (int)b;
            *(int *)(a + 0x18) = *(int *)(b + 0x18) + (int)b;
            *(int *)(a + 0x1c) = *(int *)(b + 0x1c) + (int)b;
        }
    }
    for (i = 0; i < n; i++) {
        char *a = (char *)modelpointers[i];
        if (*(int *)(a - 4) != 0) {
            if (*(short *)(a + 4) == -1) {
                if (*(int *)(a + 0x20) != 0)
                    *(int *)(a + 0x20) = *(int *)(a + 0x20) + (int)a;
                *(int *)(a + 0x10) = *(int *)(a + 0x10) + (int)a;
                *(int *)(a + 0x18) = *(int *)(a + 0x18) + (int)a;
                *(int *)(a + 0x1c) = *(int *)(a + 0x1c) + (int)a;
            }
            *(int *)(a + 0x14) = *(int *)(a + 0x14) + (int)a;
        }
    }
}
