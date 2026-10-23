extern short D_0008D248[];
void JPClearSPUFlags(int a0) {
    int i;
    for (i = 0; i < 24; i++)
        if (D_0008D248[i] == a0)
            D_0008D248[i] = 0;
}
