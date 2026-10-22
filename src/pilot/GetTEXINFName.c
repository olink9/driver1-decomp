typedef struct { short a; unsigned short nameoffset; int b; } TEXINF;

extern int D_0009755C;
extern char *D_00097568;
extern int D_000BE2F8[];
extern TEXINF *D_000BE508[];
extern int func_0007B228();

/* Driver 2: GetTEXINFName */
TEXINF *GetTEXINFName(char *name, char *out)
{
    int loc[2];
    int *q = loc;
    int i, j, texamt;
    TEXINF *texinf = 0;
    TEXINF *res;
    char *nametable = D_00097568;

    for (i = 0; i < D_0009755C; i++) {
        texamt = D_000BE2F8[i];
        texinf = D_000BE508[i];
        for (j = 0; j < texamt; j++) {
            if (!func_0007B228(nametable + texinf[j].nameoffset, name)) {
                *q = i;
                loc[1] = j;
                res = &texinf[j];
                goto done;
            }
        }
    }
    res = texinf;
done:
    out[0] = loc[0];
    out[1] = loc[1];
    return res;
}
