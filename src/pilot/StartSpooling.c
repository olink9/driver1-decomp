/* Driver 2: StartSpooling */
extern int D_000972B0;
extern int D_000972D4;
extern unsigned char D_000971E0;
extern unsigned char D_000971E8;
extern long long D_00096DC4;
extern void CdControl();
extern void func_00034808();
extern void UpdateSpool();

void StartSpooling(void) {
    int v;
    if (D_000972B0 == 0 || D_000972D4 != 0) return;
    D_000971E0 = 0x80;
    CdControl(14, &D_000971E0, &D_000971E8);
    if (D_000971E8 & 0x15) func_00034808();
    D_000972D4 = 1;
    UpdateSpool();
    v = *(unsigned char *)&D_00096DC4;
loop: if (!v) return; v = D_000972D4; goto loop;
}
