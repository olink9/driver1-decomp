/* SetupRain */
struct R { int pad[4]; unsigned short p; unsigned short q; };
extern int D_00097428;
extern short D_000B8028[];
extern struct R D_000B81C8[];
void SetupRain(void) {
    D_00097428 = 0;
    do {
        D_000B8028[D_00097428] = D_00097428;
        D_000B81C8[D_00097428].q = D_00097428;
        D_00097428++;
    } while (D_00097428 < 180);
    D_00097428 = 0;
}
