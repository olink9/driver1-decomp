/* GetFreeSongID */
extern short D_0008D218[];
extern int D_00096FAC;
int GetFreeSongID(void) {
    int i;
    for (i = 0; i < D_00096FAC; i++) {
        if (D_0008D218[i] == -1) {
            D_0008D218[i] = 0;
            return i;
        }
    }
    return -1;
}
