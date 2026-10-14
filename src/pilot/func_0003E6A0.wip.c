/* Driver 2: DentCar (DENTING.C) - Driver 1 version takes the car id */

typedef struct { short vx, vy, vz, pad; } SVECTOR;

typedef struct {
    char pad[0xC];
    unsigned short num_vertices;
    unsigned short num_polys;
    SVECTOR *vertices;
} MODEL;

typedef struct {
    char pad[0x114];
    int damage[6];
    char pad2[396 - 0x114 - 24];
} CAR_DATA;

typedef struct {
    char pad[0x10];
    unsigned char model;
    char pad2[80 - 0x11];
} CAR_INFO;

extern CAR_DATA D_0009B0A4[];
extern CAR_INFO D_00098950[];
extern long long D_0009683C;
extern MODEL *D_000A1AD0[];
extern MODEL *D_000A1AB0[];
extern SVECTOR D_000B09BC[][128];
extern unsigned char D_000AF97A[][150];
extern unsigned char D_000B08B8[];
extern unsigned char (*D_000973E0)[6][50];
extern unsigned char (*D_000973E4)[6][50];
extern unsigned char (*D_000973D0)[6][70];
extern unsigned char (*D_000973D4)[6][70];

void func_0003E6A0(int id)
{
    SVECTOR *VertPtr;
    int MaxDamage;
    unsigned char VertIndex;
    int PolyIndex;
    MODEL *DamModel;
    MODEL *CleanModel;
    MODEL *TmpModel;
    SVECTOR *DamVertPtr;
    SVECTOR *CleanVertPtr;
    unsigned char *DamPtr;
    int model;
    int Zone;
    int Damage;
    int VertNo;
    int count;

    VertPtr = D_000B09BC[id];
    MaxDamage = 0;

    if (id != 0) {
        model = D_00098950[id - 1].model;
        TmpModel = D_000A1AB0[model];
        DamModel = D_000A1AD0[model];
    } else {
        TmpModel = D_000A1AB0[0];
        DamModel = D_000A1AD0[0];
        model = *(int *)&D_0009683C;
    }

    DamVertPtr = DamModel->vertices;
    CleanVertPtr = TmpModel->vertices;

    CleanModel = D_000A1AB0[id != 0 ? model : 0];

    for (count = 0; count < CleanModel->num_vertices; count++) {
        VertPtr->pad = 0;
        VertPtr++;
    }

    VertPtr = D_000B09BC[id];

    for (Zone = 0; Zone < 6; Zone++) {
        Damage = D_0009B0A4[id].damage[Zone];
        if (Damage > MaxDamage)
            MaxDamage = Damage;

        VertNo = 0;
        while (VertNo < 50) {
            if (id == 0)
                VertIndex = D_000973E4[model][Zone][VertNo];
            else
                VertIndex = D_000973E0[model][Zone][VertNo];

            if (VertIndex == 0xFF)
                break;

            if (VertPtr[VertIndex].pad != 0)
                VertPtr[VertIndex].pad = (VertPtr[VertIndex].pad + Damage) >> 1;
            else
                VertPtr[VertIndex].pad += Damage;

            VertNo++;
        }
    }

    for (count = 0; count < CleanModel->num_vertices; count++) {
        VertPtr->vx = CleanVertPtr->vx + (((DamVertPtr->vx - CleanVertPtr->vx) * VertPtr->pad) >> 12);
        VertPtr->vy = CleanVertPtr->vy + (((DamVertPtr->vy - CleanVertPtr->vy) * VertPtr->pad) >> 12);
        VertPtr->vz = CleanVertPtr->vz + (((DamVertPtr->vz - CleanVertPtr->vz) * VertPtr->pad) >> 12);
        DamVertPtr++;
        CleanVertPtr++;
        VertPtr++;
    }

    if (id != 0)
        DamPtr = D_000AF97A[id];
    else
        DamPtr = D_000B08B8;

    for (count = 0; count < CleanModel->num_polys; count++)
        DamPtr[count] = 0;

    for (Zone = 0; Zone < 6; Zone++) {
        Damage = D_0009B0A4[id].damage[Zone] >> 10;

        for (count = 0; count < 70; count++) {
            if (id == 0)
                PolyIndex = D_000973D4[model][Zone][count];
            else
                PolyIndex = D_000973D0[model][Zone][count];

            if (PolyIndex == 0xFF)
                break;

            DamPtr[PolyIndex] += Damage;

            if (id != 0) {
                if (DamPtr[PolyIndex] > 1)
                    DamPtr[PolyIndex] = 1;
            } else {
                if (DamPtr[PolyIndex] > 3)
                    DamPtr[PolyIndex] = 3;
            }
        }
    }

    for (count = 0; count < CleanModel->num_polys; count++) {
        if (id != 0)
            DamPtr[count] <<= 7;
        else
            DamPtr[count] <<= 6;
    }
}
