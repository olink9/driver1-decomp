/* SubdivShadow - Driver 2: SubdivShadow (SHADOW.C) */

typedef struct { char *primptr; unsigned long *ot; int pad; int id; } DB;
typedef struct { unsigned addr:24; unsigned len:8; } P_TAG;

typedef struct {
    unsigned long tag;
    unsigned long rgbc;
    unsigned long xy0;
    unsigned short uv0, clut;
    unsigned long xy1;
    unsigned short uv1, tpage;
    unsigned long xy2;
    unsigned short uv2, pad1;
    unsigned long xy3;
    unsigned short uv3, pad2;
} FT4;

typedef struct { DB *p; int pad; } DBP8;
extern DBP8 current;
#define current (current.p)

#define addPrim(ot, p) (((P_TAG *)(p))->addr = ((P_TAG *)(ot))->addr, ((P_TAG *)(ot))->addr = (unsigned long)(p))


void SubdivShadow(int z0, int z1, int z2, int z3, FT4 *sps)
{
    FT4 *spd;
    DB *db;
    int i;

    spd = (FT4 *)current->primptr;
    current->primptr += sizeof(FT4) * 8;

    spd[0].tag = spd[1].tag = spd[2].tag = spd[3].tag =
        spd[4].tag = spd[5].tag = spd[6].tag = spd[7].tag = sps->tag;
    spd[0].rgbc = spd[1].rgbc = spd[2].rgbc = spd[3].rgbc =
        spd[4].rgbc = spd[5].rgbc = spd[6].rgbc = spd[7].rgbc = sps->rgbc;

    for (i = 0; i < 8; i++) {
        *(unsigned long *)&spd[i].uv0 = *(unsigned long *)&sps->uv0;
        *(unsigned long *)&spd[i].uv1 = *(unsigned long *)&sps->uv1;
        *(unsigned long *)&spd[i].uv2 = *(unsigned long *)&sps->uv2;
        *(unsigned long *)&spd[i].uv3 = *(unsigned long *)&sps->uv3;
    }

    {
        unsigned long A0, A1, E0, E1, C0, C1, B0, B1, D0, D1;

        i = 0x08000800; /* bias so packed x/y halves average without borrow */
        A0 = sps->xy0 + i;
        A1 = sps->xy1 + i;
        E0 = sps->xy2 + i;
        E1 = sps->xy3 + i;
        C0 = ((A0 + E0) >> 1);
        B0 = (A0 + C0) >> 1;
        C1 = ((A1 + E1) >> 1);
        B1 = (A1 + C1) >> 1;
        D0 = (E0 + C0) >> 1;
        D1 = (E1 + C1) >> 1;
        spd[0].xy0 = (A0) - i;
        spd[0].xy1 = ((A0 + A1) >> 1) - i;
        spd[4].xy0 = ((A0 + A1) >> 1) - i;
        spd[4].xy1 = (A1) - i;
        spd[0].xy2 = (B0) - i;
        spd[0].xy3 = ((B0 + B1) >> 1) - i;
        spd[4].xy2 = ((B0 + B1) >> 1) - i;
        spd[4].xy3 = (B1) - i;
        spd[1].xy0 = (B0) - i;
        spd[1].xy1 = ((B0 + B1) >> 1) - i;
        spd[5].xy0 = ((B0 + B1) >> 1) - i;
        spd[5].xy1 = (B1) - i;
        spd[1].xy2 = (C0) - i;
        spd[1].xy3 = ((C0 + C1) >> 1) - i;
        spd[5].xy2 = ((C0 + C1) >> 1) - i;
        spd[5].xy3 = (C1) - i;
        spd[2].xy0 = (C0) - i;
        spd[2].xy1 = ((C0 + C1) >> 1) - i;
        spd[6].xy0 = ((C0 + C1) >> 1) - i;
        spd[6].xy1 = (C1) - i;
        spd[2].xy2 = (D0) - i;
        spd[2].xy3 = ((D0 + D1) >> 1) - i;
        spd[6].xy2 = ((D0 + D1) >> 1) - i;
        spd[6].xy3 = (D1) - i;
        spd[3].xy0 = (D0) - i;
        spd[3].xy1 = ((D0 + D1) >> 1) - i;
        spd[7].xy0 = ((D0 + D1) >> 1) - i;
        spd[7].xy1 = (D1) - i;
        spd[3].xy2 = (E0) - i;
        spd[3].xy3 = ((E0 + E1) >> 1) - i;
        spd[7].xy2 = ((E0 + E1) >> 1) - i;
        spd[7].xy3 = (E1) - i;
    }
    {
        unsigned long A0, A1, E0, E1, C0, C1, B0, B1, D0, D1;

        A0 = sps->uv0 & 0xFEFF;
        A1 = sps->uv1 & 0xFEFF;
        E0 = sps->uv2 & 0xFEFF;
        E1 = sps->uv3 & 0xFEFF;
        C0 = ((A0 + E0) >> 1) & 0xFEFF;
        B0 = (A0 + C0) >> 1;
        C1 = ((A1 + E1) >> 1) & 0xFEFF;
        B1 = (A1 + C1) >> 1;
        D0 = (E0 + C0) >> 1;
        D1 = (E1 + C1) >> 1;
        spd[0].uv0 = A0;
        spd[0].uv1 = (A0 + A1) >> 1;
        spd[4].uv0 = (A0 + A1) >> 1;
        spd[4].uv1 = A1;
        spd[0].uv2 = B0;
        spd[0].uv3 = (B0 + B1) >> 1;
        spd[4].uv2 = (B0 + B1) >> 1;
        spd[4].uv3 = B1;
        spd[1].uv0 = B0;
        spd[1].uv1 = (B0 + B1) >> 1;
        spd[5].uv0 = (B0 + B1) >> 1;
        spd[5].uv1 = B1;
        spd[1].uv2 = C0;
        spd[1].uv3 = (C0 + C1) >> 1;
        spd[5].uv2 = (C0 + C1) >> 1;
        spd[5].uv3 = C1;
        spd[2].uv0 = C0;
        spd[2].uv1 = (C0 + C1) >> 1;
        spd[6].uv0 = (C0 + C1) >> 1;
        spd[6].uv1 = C1;
        spd[2].uv2 = D0;
        spd[2].uv3 = (D0 + D1) >> 1;
        spd[6].uv2 = (D0 + D1) >> 1;
        spd[6].uv3 = D1;
        spd[3].uv0 = D0;
        spd[3].uv1 = (D0 + D1) >> 1;
        spd[7].uv0 = (D0 + D1) >> 1;
        spd[7].uv1 = D1;
        spd[3].uv2 = E0;
        spd[3].uv3 = (E0 + E1) >> 1;
        spd[7].uv2 = (E0 + E1) >> 1;
        spd[7].uv3 = E1;
    }

    db = current;
    addPrim(db->ot + ((z0 * 7 + z3) >> 6), &spd[0]);
    addPrim(db->ot + ((z0 * 5 + z3 * 3) >> 6), &spd[1]);
    addPrim(db->ot + ((z0 * 3 + z3 * 5) >> 6), &spd[2]);
    addPrim(db->ot + ((z0 + z3 * 7) >> 6), &spd[3]);
    addPrim(db->ot + ((z1 * 7 + z2) >> 6), &spd[4]);
    addPrim(db->ot + ((z1 * 5 + z2 * 3) >> 6), &spd[5]);
    addPrim(db->ot + ((z1 * 3 + z2 * 5) >> 6), &spd[6]);
    addPrim(db->ot + ((z1 + z2 * 7) >> 6), &spd[7]);
}
