/* CheckUnpackNewRegions */
typedef struct {
    int xoffset;
    int zoffset;
} AREA_LOAD_INFO;

typedef struct {
    short vx, vy, vz, pad;
} SVEC;

typedef struct {
    unsigned short offset;
    char pad[6];
    unsigned char super_region;
} Spool;

extern long long D_0009711C;
extern long long D_00096884;
extern int D_00097320;
extern int D_0009730C;
extern int D_0009733C;
extern int D_00096A24;
extern int D_0009731C;
extern int D_00097314;
extern int current_region;
extern int D_000A8024[];
extern short D_0009728C[];
extern long long D_000972AC;
extern long long D_00097254;
extern int D_00097324;
extern int D_00097344;

extern void ClearRegion();
extern void UnpackRegion();

void CheckUnpackNewRegions(void)
{
    AREA_LOAD_INFO regions_to_unpack[3];
    SVEC sortregions[4];
    unsigned short sortorder[4];
    int leftright_unpack, topbottom_unpack, num_regions_to_unpack;
    int force_load_boundary;
    int i, j, k, sortcount, x, z;
    int target_region, region_to_unpack;
    unsigned short sort;
    unsigned short *srcsort, *destsort;
    Spool *spoolptr;

    leftright_unpack = 0;
    topbottom_unpack = 0;
    num_regions_to_unpack = 0;

    if (*(int *)&D_0009711C != 0)
        return;

    force_load_boundary = 12;
    if (*(int *)&D_00096884 == 0)
        force_load_boundary = 17;

    if (D_00097320 < force_load_boundary) {
        if (D_0009730C != 0) {
            num_regions_to_unpack = 1;
            leftright_unpack = 1;
            x = -1;
            goto store;
        }
    } else if (D_0009733C - force_load_boundary < D_00097320) {
        if (D_0009730C < D_00096A24 / D_0009733C) {
            num_regions_to_unpack = 1;
            leftright_unpack = 2;
            x = 1;
        store:
            regions_to_unpack[0].xoffset = x;
            regions_to_unpack[0].zoffset = 0;
        }
    }

    if (D_0009731C < force_load_boundary) {
        if (D_00097314 != 0) {
            regions_to_unpack[num_regions_to_unpack].xoffset = 0;
            regions_to_unpack[num_regions_to_unpack].zoffset = -1;
            num_regions_to_unpack++;
            topbottom_unpack = 1;
        }
    } else if (D_0009733C - force_load_boundary < D_0009731C && D_00097314 != 0) {
        regions_to_unpack[num_regions_to_unpack].xoffset = 0;
        regions_to_unpack[num_regions_to_unpack].zoffset = 1;
        num_regions_to_unpack++;
        topbottom_unpack = 2;
    }

    if (num_regions_to_unpack == 2) {
        if (topbottom_unpack == 1) {
            if (leftright_unpack == 1) {
                num_regions_to_unpack = 3;
                regions_to_unpack[2].xoffset = -1;
                regions_to_unpack[2].zoffset = -1;
            } else {
                num_regions_to_unpack = 3;
                regions_to_unpack[2].xoffset = 1;
                regions_to_unpack[2].zoffset = -1;
            }
        } else {
            if (leftright_unpack == 1) {
                num_regions_to_unpack = 3;
                regions_to_unpack[2].xoffset = -1;
                regions_to_unpack[2].zoffset = 1;
            } else {
                num_regions_to_unpack = 3;
                regions_to_unpack[2].xoffset = 1;
                regions_to_unpack[2].zoffset = 1;
            }
        }
    }

    i = 0;
    for (sortcount = i; i < num_regions_to_unpack; i++) {
        x = regions_to_unpack[i].xoffset;
        z = regions_to_unpack[i].zoffset;

        region_to_unpack = current_region + x + z * (D_00096A24 / D_0009733C);
        target_region = ((D_0009730C + x) & 1) + ((D_00097314 + z) & 1) * 2;

        if (region_to_unpack != D_000A8024[target_region] && D_0009728C[target_region] == -1) {
            ClearRegion(target_region);

            if ((*(unsigned short **)&D_000972AC)[region_to_unpack] == 0xffff) {
                D_000A8024[target_region] = region_to_unpack;
            } else {
                spoolptr = (Spool *)(*(char **)&D_00097254 + (*(unsigned short **)&D_000972AC)[region_to_unpack]);

                if (D_00097324 == -1 && spoolptr->super_region != 0xff)
                    D_00097344 = spoolptr->super_region;

                sortregions[sortcount].vx = region_to_unpack;
                sortregions[sortcount].vy = target_region;
                sortregions[sortcount].vz = spoolptr->offset;
                sortorder[sortcount] = sortcount;
                sortcount++;
            }
        }
    }

    for (k = 0; k < sortcount; k++) {
        for (j = k + 1; j < sortcount; j++) {
            if (sortregions[sortorder[k]].vz > sortregions[sortorder[j]].vz) {
                sort = sortorder[k];
                sortorder[k] = sortorder[j];
                sortorder[j] = sort;
            }
        }
        UnpackRegion(sortregions[sortorder[k]].vx, sortregions[sortorder[k]].vy);
    }
}
