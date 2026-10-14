typedef struct { char c[8]; } B;
extern B D_00096E28[];
extern long long D_00097534;
extern void func_000744C4();

void func_0005B70C(void) {
    B buf;
    buf = D_00096E28[0];
    if (*(int *)&D_00097534 == 0) {
        *(short *)(buf.c + 2) = 0x100;
        func_000744C4(&buf, 0, 0);
    } else {
        func_000744C4(&buf, 0, 0x100);
    }
}
