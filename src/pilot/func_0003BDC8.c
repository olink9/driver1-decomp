/* func_0003BDC8: sorts a model's polygons into 7 per-pass draw lists (no Driver 2 counterpart found) */

typedef struct {
    int a, b, c, d, e, f;
} PolyEnt;

typedef struct {
    int n0; PolyEnt *p0;
    int n1; PolyEnt *p1;
    int n2; PolyEnt *p2;
    int n3; PolyEnt *p3;
    int n4; PolyEnt *p4;
    int n5; PolyEnt *p5;
    int n6; PolyEnt *p6;
    int x38;
} PolyLists;

typedef struct {
    char pad0[0xE];
    unsigned short num_polys;
    int x10;
    unsigned char *polys;
} ModelHdr;

typedef struct {
    char pad[0x10];
    int v;
} Ent20;

extern int D_000973B0;
extern PolyEnt D_000AB728[];
extern unsigned short texture_cluts[][32];
extern unsigned short texture_pages[];
extern int D_00089F1C[];
extern Ent20 D_000BE9FC[][30];
extern long long D_000975CC;

void func_0003BDC8(PolyLists *out, ModelHdr *hdr, int reset, int flag, int flag2)
{
    int pass;
    int count;
    int i;
    unsigned char *poly;
    unsigned char *pg;
    PolyEnt *p;
    unsigned char type;

    if (reset)
        D_000973B0 = 0;

    if (hdr == 0 || ((unsigned int)hdr & 0xFFFFFFF) > 0x800000) {
        out->n2 = 0;
        out->n3 = 0;
        out->n0 = 0;
        out->n1 = 0;
        out->n6 = 0;
        out->n4 = 0;
        out->n5 = 0;
        return;
    }

    out->x38 = hdr->x10;

    for (pass = 0; pass < 7; pass++) {
        count = D_000973B0;
        poly = hdr->polys;

        switch (pass) {
        case 0: out->p2 = &D_000AB728[D_000973B0]; break;
        case 1: out->p3 = &D_000AB728[D_000973B0]; break;
        case 2: out->p0 = &D_000AB728[D_000973B0]; break;
        case 3: out->p1 = &D_000AB728[D_000973B0]; break;
        case 4: out->p6 = &D_000AB728[D_000973B0]; break;
        case 5: out->p4 = &D_000AB728[D_000973B0]; break;
        case 6: out->p5 = &D_000AB728[D_000973B0]; break;
        }

        for (i = 0; count < 666 && i < hdr->num_polys; i++) {
            p = &D_000AB728[count];
            type = poly[0];

            switch (type & 0x1F) {
            case 0:
                if (pass == 4) {
                    p->a = poly[1] + ((poly[2] + (poly[3] << 8)) << 8);
                    p->b = 0;
                    p->c = 0;
                    p->d = 0;
                    p->e = 0;
                    p->f = i;
                    count++;
                }
                break;
            case 1:
                if (pass == 4) {
                    p->a = poly[4] + ((poly[5] + (poly[6] << 8)) << 8);
                    p->b = 0;
                    p->c = 0;
                    p->d = 0;
                    p->e = 0;
                    p->f = i;
                    p++;
                    p->a = poly[4] + ((poly[6] + (poly[7] << 8)) << 8);
                    p->b = 0;
                    p->c = 0;
                    p->d = 0;
                    p->e = 0;
                    p->f = i;
                    count += 2;
                }
                break;
            case 20:
                if (pass == 2) {
                    p->a = poly[4] + ((poly[5] + (poly[6] << 8)) << 8);
                    p->b = 0;
                    p->c = (texture_cluts[poly[1]][*(unsigned short *)(poly + 2)] << 16) | *(unsigned short *)(poly + 8);
                    p->d = (texture_pages[poly[1]] << 16) | *(unsigned short *)(poly + 10);
                    p->e = *(unsigned short *)(poly + 12);
                    count++;
                    p->f = i;
                }
                break;
            case 21:
                if (pass == 3) {
                    p->a = poly[4] + ((poly[5] + ((poly[7] + (poly[6] << 8)) << 8)) << 8);
                    p->b = 0;
                    p->c = (texture_cluts[poly[1]][*(unsigned short *)(poly + 2)] << 16) | *(unsigned short *)(poly + 8);
                    p->d = (texture_pages[poly[1]] << 16) | *(unsigned short *)(poly + 10);
                    p->e = (*(unsigned short *)(poly + 12) << 16) | *(unsigned short *)(poly + 14);
                    count++;
                    p->f = i;
                }
                break;
            case 22:
                pg = poly;
                if (pass == 0 && flag) {
                    D_000BE9FC[poly[1] - *(int *)&D_000975CC][*(unsigned short *)(poly + 2)].v =
                        texture_cluts[poly[1]][*(unsigned short *)(poly + 2)];
                    p->a = poly[4] + ((poly[5] + (poly[6] << 8)) << 8);
                    p->b = poly[8] + ((poly[9] + (poly[10] << 8)) << 8);
                    p->c = (((char *)&D_000BE9FC[poly[1] - *(int *)&D_000975CC][*(unsigned short *)(poly + 2)] - (char *)D_000BE9FC) << 14) | *(unsigned short *)(poly + 12);
                    p->d = (texture_pages[poly[1]] << 16) | *(unsigned short *)(poly + 14);
                    p->e = *(unsigned short *)(poly + 16);
                    count++;
                    p->f = i;
                } else if ((flag2 ? pass == 5 : pass == 2) && !flag) {
                    p->a = pg[4] + ((pg[5] + (pg[6] << 8)) << 8);
                    p->b = pg[8] + ((pg[9] + (pg[10] << 8)) << 8);
                    p->c = (texture_cluts[pg[1]][*(unsigned short *)(pg + 2)] << 16) | *(unsigned short *)(pg + 12);
                    p->d = (texture_pages[pg[1]] << 16) | *(unsigned short *)(pg + 14);
                    p->e = *(unsigned short *)(pg + 16);
                    count++;
                    p->f = i;
                }
                break;
            case 23:
                pg = poly;
                if (pass == 1 && flag) {
                    D_000BE9FC[poly[1] - *(int *)&D_000975CC][*(unsigned short *)(poly + 2)].v =
                        texture_cluts[poly[1]][*(unsigned short *)(poly + 2)];
                    p->a = poly[4] + ((poly[5] + ((poly[7] + (poly[6] << 8)) << 8)) << 8);
                    p->b = poly[8] + ((poly[9] + ((poly[11] + (poly[10] << 8)) << 8)) << 8);
                    p->c = (((char *)&D_000BE9FC[poly[1] - *(int *)&D_000975CC][*(unsigned short *)(poly + 2)] - (char *)D_000BE9FC) << 14) | *(unsigned short *)(poly + 12);
                    p->d = (texture_pages[poly[1]] << 16) | *(unsigned short *)(poly + 14);
                    p->e = (*(unsigned short *)(poly + 16) << 16) | *(unsigned short *)(poly + 18);
                    count++;
                    p->f = i;
                } else if ((flag2 ? pass == 6 : pass == 3) && !flag) {
                    p->a = pg[4] + ((pg[5] + ((pg[7] + (pg[6] << 8)) << 8)) << 8);
                    p->b = pg[8] + ((pg[9] + ((pg[11] + (pg[10] << 8)) << 8)) << 8);
                    p->c = (texture_cluts[pg[1]][*(unsigned short *)(pg + 2)] << 16) | *(unsigned short *)(pg + 12);
                    p->d = (texture_pages[pg[1]] << 16) | *(unsigned short *)(pg + 14);
                    p->e = (*(unsigned short *)(pg + 16) << 16) | *(unsigned short *)(pg + 18);
                    count++;
                    p->f = i;
                }
                break;
            }
            poly += D_00089F1C[type & 0x1F];
        }

        switch (pass) {
        case 0: out->n2 = count - D_000973B0; break;
        case 1: out->n3 = count - D_000973B0; break;
        case 2: out->n0 = count - D_000973B0; break;
        case 3: out->n1 = count - D_000973B0; break;
        case 4: out->n6 = count - D_000973B0; break;
        case 5: out->n4 = count - D_000973B0; break;
        case 6: out->n5 = count - D_000973B0; break;
        }
        D_000973B0 = count;
    }
}
