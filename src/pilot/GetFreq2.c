/* GetFreq2 */
extern int D_00096FB4;
extern unsigned short D_0008D278[];
int GetFreq2(int period) {
    int okt;
    period = D_00096FB4 - period;
    okt = period / 768;
    return D_0008D278[period % 768] >> (5 - okt);
}
