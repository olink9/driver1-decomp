extern int D_00097370;
extern long long D_000975A8;
extern int D_000A0E20[];
extern int func_0007B228();

int func_000396A8(int key) {
    char *s = *(char **)&D_000975A8;
    int i;
    for (i = 0; i < D_00097370; i++) {
        if (func_0007B228(s, key) == 0) return D_000A0E20[i];
        while (*s++) ;
    }
    return 0;
}
