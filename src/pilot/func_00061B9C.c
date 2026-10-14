typedef struct { unsigned short key; char b; } N;
extern N *D_000C43B4[];
extern char D_000976E0[];
extern int D_000976F4;
N *func_00061B9C(void) {
    int i = 1;
    int c;
    N *top;
    N *sent = (N *)D_000976E0;
    N *last;
    unsigned key;
    top = D_000C43B4[1];
    last = D_000C43B4[D_000976F4];
    D_000C43B4[D_000976F4--] = sent;
    key = last->key;
    for (;;) {
        c = i * 2;
        if (D_000C43B4[c + 1]->key < D_000C43B4[c]->key) c++;
        if (D_000C43B4[c]->key < key) {
            D_000C43B4[i] = D_000C43B4[c];
            i = c;
        } else break;
    }
    D_000C43B4[i] = last;
    top->b = 0xFF;
    return top;
}
