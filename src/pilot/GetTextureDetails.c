/* Driver 2: GetTextureDetails */
typedef struct {
    unsigned char coords[8];
    unsigned short tpageid;
    unsigned short clutid;
    unsigned char texture_number;
    unsigned char texture_page;
} TD;

typedef struct {
    short pad0;
    unsigned short nameoffset;
    unsigned char x, y, w, h;
} TEXINF;

extern int D_0009755C;
extern char *D_00097568;
extern int D_00096CD0;
extern int D_0009754C;
extern char D_00096CD8[];
extern unsigned short texture_pages[];
extern unsigned short texture_cluts[][32];
extern int D_000BE2F8[];
extern TEXINF *D_000BE508[];
extern int func_0007B228();
extern void GetTextureDetails();

void GetTextureDetails(name, info)
char *name;
TD *info;
{
    int i, j;
    int texamt;
    TEXINF *texinf;
    char *nametable;

    nametable = D_00097568;
    for (i = 0; i < D_0009755C; i++) {
        texamt = D_000BE2F8[i];
        texinf = D_000BE508[i];
        for (j = 0; j < texamt; j++) {
            if (func_0007B228(nametable + texinf[j].nameoffset, name) == 0 &&
                (D_00096CD0 == 0 || i == D_0009754C)) {
                info->tpageid = texture_pages[i];
                info->clutid = texture_cluts[i][j];
                info->texture_number = j;
                info->texture_page = i;
                info->coords[0] = texinf[j].x;
                info->coords[1] = texinf[j].y;
                info->coords[2] = texinf[j].x + texinf[j].w - 1;
                info->coords[3] = texinf[j].y;
                info->coords[4] = texinf[j].x;
                info->coords[5] = texinf[j].y + texinf[j].h - 1;
                info->coords[6] = texinf[j].x + texinf[j].w - 1;
                info->coords[7] = texinf[j].y + texinf[j].h - 1;
                return;
            }
        }
    }
    D_00096CD0 = 0;
    GetTextureDetails(D_00096CD8, info);
}
