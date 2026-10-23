typedef struct {
    int x;
    int pad4;
    int z;
    int padC;
    int f10[2];
    unsigned char f18;
    unsigned char pad19;
    short f1A;
    short f1C;
    char pad1E[6];
    unsigned char f24;
    unsigned char f25;
    unsigned char f26;
    unsigned char dir;
    unsigned char flag;
    unsigned char list[1];
} P;

typedef struct { int a; int b; int c; int d; int e; } NODE;
typedef struct { unsigned char b[20]; } JN;
typedef struct {
    unsigned char id;
    unsigned char active;
    unsigned char pad2[3];
    unsigned char f5;
    unsigned char pad6[2];
    unsigned char f8;
    unsigned char pad9[80 - 9];
} CAR;
typedef struct { char pad[1960]; } SND;

extern long long D_0009704C;
extern long long D_0009B16C;
extern long long D_0009B174;
extern long long D_00096870;
extern volatile long long CameraCnt;
extern int D_0009677C;
extern unsigned char D_00096FC4;
extern int D_00096FD0;
extern int D_00096FF0;
extern JN D_0009E744[];
extern CAR D_00098950[];
extern SND D_000C4690[];
extern unsigned char D_00099AF0[];
extern char D_00099120[];
extern short func_00020DB4();
extern int func_00019AF4();
extern void func_0001A024();
extern void func_0001A008();
extern void func_00063AB4();

void func_0001943C(P *p)
{
    int dir;
    int off;
    NODE *q;
    int n;
    int r;
    int id;
    unsigned char c;
    CAR *cp;
    int s;
    int f5;

    if (p->flag != 1)
        return;
    dir = p->dir;
    q = &(*(NODE **)&D_0009704C)[p->f1A];
    off = -30000;
    if (dir & 1) {
        p->z = -q->d * 1500;
        if (dir == 1)
            off = 30000;
        p->x = *(int *)&D_0009B16C + off;
    } else {
        p->x = q->a * 1500;
        if (dir == 0)
            off = 30000;
        p->z = *(int *)&D_0009B174 + off;
    }
    r = p->f1C = func_00020DB4(p->x, p->z, dir, p->f10);
    if (r != -1) {
        JN *j = &D_0009E744[r];
        p->f25 = j->b[dir] + j->b[(dir + 2) & 3];
    } else {
        p->f25 = 0;
        p->f18 = 0;
    }
    n = p->f24 + p->f25 + p->f18;
    if (*(int *)&D_00096870 != 0) if (*(int *)&D_00096870 >= 0) if (*(int *)&D_00096870 < 3) {
        if (p->f18 == 0 && r != -1)
            n += n - 2;
        else
            n += n - 1;
    }
    if (p->f26 < n) {
        c = D_00096FC4;
        if (c == 22) {
            id = -1;
        } else {
            D_00096FC4 = c + 1;
            id = D_00099AF0[c];
        }
        if (id != -1) {
            p->list[p->f26++] = id;
        }
    } else if (n < p->f26) {
        do {
            s = p->list[--p->f26];
            cp = &D_00098950[s];
            f5 = cp->f5;
            if (cp->active && D_0009677C) {
                if (*(volatile int *)&CameraCnt != D_00096FD0 || s != D_00096FF0) {
                    D_00096FF0 = s;
                    D_00096FD0 = *(volatile int *)&CameraCnt;
                }
            }
            if (f5)
                func_00063AB4(&D_000C4690[cp->f5]);
            if (cp->active == 9)
                func_0001A008(D_00099120);
            cp->active = 0;
            cp->f8 = 0;
            D_00099AF0[--D_00096FC4] = s;
        } while (n < p->f26);
    }
    if (p->f26 == n) {
        if (!func_00019AF4(p))
            func_0001A024(p);
    }
}
