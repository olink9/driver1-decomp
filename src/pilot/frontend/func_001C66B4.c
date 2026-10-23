extern int D_00096870, D_00096784, D_000967EC;
extern char D_000A801D;
extern int D_001CE7BC[];
extern int D_001CE898[];
extern int D_001CE890[];

void func_001C66B4(void)
{
    D_001CE7BC[10] = D_001CE898[D_00096870];
    D_001CE7BC[18] = D_001CE890[D_00096784];
    if (D_000A801D & 4) {
        D_001CE7BC[26] = D_001CE890[D_000967EC];
    } else {
        D_000967EC = 0;
        D_001CE7BC[26] = D_001CE890[0];
    }
}
