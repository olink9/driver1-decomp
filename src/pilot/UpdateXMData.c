/* UpdateXMData */
typedef struct { char pad[0x40]; int PCounter; } XM;
extern int D_00097844, D_00096FAC, D_00097834, D_00097830, D_0009782C;
extern XM *D_00097814;
extern XM *D_000C7888[];
extern void UpdateWithTimer();
void UpdateXMData(void) {
    int i;
    D_00097834 = 0;
    D_00097830 = 0;
    D_0009782C = 0;
    if (D_00097844) return;
    for (i = 0; i < D_00096FAC; i++) {
        D_00097814 = D_000C7888[i];
        UpdateWithTimer(i);
        if (D_00097814->PCounter == 5) {
            UpdateWithTimer(i);
            D_00097814->PCounter = 0;
        }
    }
}
