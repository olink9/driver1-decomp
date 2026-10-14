/* XM_SetMasterVol */
extern short D_0008D218[];
extern char *D_000C7888[];
extern char *D_00097810;
void func_000735DC(int id, unsigned char vol) {
    if (D_0008D218[id] == -1) return;
    if (vol <= 128) {
        D_00097810 = D_000C7888[id];
        D_00097810[0x28] = vol;
    }
}
