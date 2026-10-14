/* CalcPlayPos */
typedef struct { char pad[0x12]; unsigned short XMChannels; } MH;
typedef struct { char pad[0x4C]; int PatAdr2; char pad2[0x58 - 0x50]; char chnl[1]; } MS;
extern MS *D_00097814;
extern MH *D_00097818;
extern char *D_0009781C;
extern int D_0009783C;
extern int func_000722C0();
int func_00072150(int StartPos) {
    int SP = 0;
    int t, i;
    for (i = 0; i < StartPos; i++) {
        for (t = 0; t < D_00097818->XMChannels; t++) {
            D_0009783C = t;
            D_0009781C = D_00097814->chnl + t * 0x78;
            SP += func_000722C0(D_00097814->PatAdr2 + SP);
        }
    }
    return SP;
}
