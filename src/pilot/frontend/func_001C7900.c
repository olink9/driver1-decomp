typedef struct { char name[8]; unsigned int time; } REC;
typedef struct { int a, b; char *text; int d; } ITEM;
extern int D_001CECB8, D_001CEC5C, D_001D2F28;
extern char *D_001CE09C[], *D_001CEC60[];
extern char D_001C084C[], D_001C0854[], D_001C0858[], D_001C085C[], D_001C0864[], D_001C07C0[], D_001C0870[];
extern char D_001D2E68[], D_001D3038[][32], D_001CEC80[][9];
extern ITEM D_001CEA88[];
extern REC *D_001D2E58;
extern int sprintf();
extern char *func_0006E418();
extern unsigned char func_0006ACC8(char *buf, int ticks, unsigned char flag);

void func_001C7900(void)
{
    char buf[256];
    unsigned int k;
    int i;

    switch (D_001CECB8) {
    case 0x11:
        k = 0;
        D_001D2F28 = 8;
        break;
    case 0x12:
        k = 1;
        D_001D2F28 = 8;
        break;
    case 0x13:
        k = 2;
        D_001D2F28 = 4;
        break;
    case 0x16:
        k = 3;
        D_001D2F28 = 4;
        break;
    default:
        return;
    }
    if (k < 2)
        sprintf(buf, D_001C084C, D_001CE09C[D_001CEC5C >> 1], (D_001CEC5C & 1) + 1);
    else if (k == 2)
        sprintf(buf, D_001C0854, D_001CE09C[D_001CEC5C]);
    else
        sprintf(buf, D_001C0858, D_001CEC5C + 1);
    sprintf(D_001D2E68, D_001C085C, D_001CEC60[k], buf);
    D_001CEA88[0].text = D_001D2E68;
    {
        REC *r = (REC *)func_0006E418(D_001CECB8, D_001CEC5C);
        D_001D2E58 = r;
    }
    for (i = 0; i < 5; i++) {
        if (D_001CECB8 == 0x11) {
            sprintf(D_001D3038[i], D_001C0864, D_001D2E58[i].time >> 25);
            func_0006ACC8(D_001CEC80[i], D_001D2E58[i].time & 0x1FFFFFF, 1);
        } else {
            func_0006ACC8(D_001CEC80[i], D_001D2E58[i].time, 1);
            sprintf(D_001D3038[i], D_001C07C0);
        }
    }
    for (i = 0; i < 5; i++) {
        D_001CEA88[i * 4 + 2].text = D_001D2E58[i].name;
        D_001CEA88[i * 4 + 3].text = D_001CEC80[i];
        D_001CEA88[i * 4 + 4].text = D_001D3038[i];
    }
    if (D_001CECB8 == 0x16) {
        k = i * 4 + 1;
        func_0006ACC8(D_001CEC80[i], D_001D2E58[i].time, 1);
        D_001CEA88[k].text = D_001C0870;
        D_001CEA88[k + 1].text = D_001D2E58[i].name;
        D_001CEA88[k + 2].text = D_001CEC80[i];
    } else {
        k = i * 4 + 1;
        D_001CEA88[k].text = D_001C07C0;
        D_001CEA88[k + 1].text = D_001C07C0;
        D_001CEA88[k + 2].text = D_001C07C0;
    }
}
