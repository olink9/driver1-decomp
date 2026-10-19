/* InitAnimatingObjects (D2 name, probable) */
extern long long D_00096834;
extern int D_0008AE5C[];
typedef struct { int a, b, c, d; } E16;
extern E16 *D_0008AE3C[];
extern int D_0008AE8C[];
extern long long D_00097370;
extern int D_000975AC;
typedef struct { unsigned short flags; short pad; short id; } M;
extern M *D_000A0E20[];
extern M *func_000396A8();

void func_00052A78(void)
{
    int k, i, j;
    int s0;
    E16 *p;
    M *r;
    int idx = *(int *)&D_00096834;

    D_000975AC = D_0008AE5C[idx];
    s0 = 0;
    if (D_000975AC != 0) {
        p = D_0008AE3C[idx];
        for (i = 0; i < D_000975AC; i++, p++) {
            r = func_000396A8(p->c);
            if (r) {
                for (s0 = 0; r != D_000A0E20[s0]; s0++)
                    ;
                p->b = s0;
                r->flags |= 4;
            }
            for (j = 0; j < *(int *)&D_00097370; j++) {
                M *t = D_000A0E20[j];
                if (t->id == s0)
                    t->flags |= 4;
            }
        }
    }
    {
        int *q = D_0008AE8C;
        if (q[1]) {
            do {
                int a = q[1];
                q[0] = (int)func_000396A8(a);
                q += 5;
            } while (q[1]);
        }
    }
}
