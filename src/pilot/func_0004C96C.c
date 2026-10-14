/* PlaySpeech */
struct SQ { int count; int slot[7]; };
char func_0004C96C(struct SQ *q, int sound) {
    int i, c = q->count;
    if (c < 7) {
        i = c;
        q->count = c + 1;
        i--;
        while (i != -1) {
            q->slot[i + 1] = q->slot[i];
            i--;
        }
        q->slot[0] = sound;
        return 1;
    }
    return 0;
}
