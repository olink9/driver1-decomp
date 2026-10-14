void func_0007C7D4(int, int);
void SpuSetReverb(int);
void func_0004D5F8(int on) {
    if (on) {
        func_0007C7D4(1, 0xFFFDFF);
        SpuSetReverb(1);
    } else {
        func_0007C7D4(0, 0xFFFFFF);
        SpuSetReverb(0);
    }
}
