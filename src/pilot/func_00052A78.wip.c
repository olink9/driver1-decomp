extern int D_00096834_dummy;
extern long long D_00096834;
extern int D_0008AE5C[];
extern int D_0008AE3C[];
extern int D_0008AE8C[];
extern long long D_00097370;
extern int D_000975AC;
extern unsigned short *D_000A0E20[];
extern unsigned short *func_000396A8();

void func_00052A78(void)
{
    int k, i, j;
    int s0;
    int *p;
    unsigned short *r;
    int idx = *(int *)&D_00096834;

    D_000975AC = D_0008AE5C[idx];
    s0 = 0;
    if (D_000975AC != 0) {
        p = (int *)D_0008AE3C[idx] + 1;
        for (i = 0; i < D_000975AC; i++) {
            r = func_000396A8(p[1]);
            if (r) {
                for (s0 = 0; r != D_000A0E20[s0]; s0++)
                    ;
                p[0] = s0;
                *r |= 4;
            }
            k = *(int *)&D_00097370;
            for (j = 0; j < k; j++) {
                unsigned short *t = D_000A0E20[j];
                if (((short *)t)[2] == s0)
                    *t |= 4;
            }
            p += 4;
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
