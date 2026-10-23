/* func_0004D208 - InitMusic(musicnum) (Driver 2 name) */
extern long long D_000967BC;
#define D_000967BC_ (*(char *)&D_000967BC)
extern long long D_00096834;
#define D_00096834_ (*(int *)&D_00096834)
extern int D_00096C70;
extern long long D_00096C78;
#define D_00096C78_ (*(int *)&D_00096C78)
extern long long D_00096C88;
#define D_00096C88_ (*(int *)&D_00096C88)
extern long long D_00096C8C;
#define D_00096C8C_ (*(int *)&D_00096C8C)
extern long long D_00097098;
extern long long D_00097298;
extern long long D_000974CC;
extern int D_000974BC;
extern int D_000974C4;
extern char D_000106C0[];
extern void LoadSoundBank();
extern void AllocateReverb();
extern void LoadfileSeg();
extern void InitXMData();
extern int XM_Init();
extern void XM_Pause();
extern int func_00073804();
extern int func_00073880();
extern int func_00073888();
extern int func_000738CC();
extern void func_000738D4();
#define P (*(int *)&D_00097098)

void func_0004D208(int a0)
{
    int buf[3];
    int x, y;
    char *addr; /* name, then reused for the XM header sizes */
    int w;
    int v;
    int t;

    D_00096C70 = 0;
    addr = D_000106C0;
    if (D_000967BC_) {
        if (D_00096834_ == 2) {
            x = 5;
            y = 0x4000;
        } else {
            x = 3;
            y = 0x2000;
        }
        AllocateReverb(x, y);
        *(int *)&D_00097298 = a0;
        LoadfileSeg(addr, buf, a0 << 3, 12);
        t = P;
        v = buf[1] - buf[0];
        w = buf[2] - buf[1];
        *(int *)&D_000974CC = t;
        D_000974C4 = t;
        P = t + ((v + 3) & -4);
        D_000974BC = P;
        LoadfileSeg(addr, t, buf[0], v + w);
        if (D_00096C88_ == -1) {
            D_00096C8C_ = func_00073804();
            LoadSoundBank(D_000974BC, w, 2);
            addr = (char *)func_000738CC();
            func_000738D4(P);
            P = P + (int)addr;
            addr = (char *)func_00073880();
            func_00073888(P);
            P = P + (int)addr;
        }
    }
    InitXMData(D_000974C4, 0, 0);
    D_00096C88_ = XM_Init(D_00096C8C_, 0, 0, 16, 1, -1, 0, 0);
    if (D_00096C78_)
        XM_Pause(D_00096C88_);
}
