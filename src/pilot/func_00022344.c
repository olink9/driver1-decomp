/* Driver 2: ProcessLumps */
extern int D_00096804;
extern int D_00096834;
extern char *D_00097078;
extern int D_000970A4;
extern int D_000970D0;
extern char *D_000970F0;
extern char *D_00097114;
extern long long D_00097568;
extern long long D_000975A8;
extern char D_0009E714[];

extern void InitModelNames();
extern void func_000298B0();
extern void func_000298BC();
extern void ProcessMapLump();
extern void func_0003181C();
extern void ProcessSpoolInfoLump();
extern void func_00039450();
extern void func_000394E0();
extern void func_00039570();
extern void func_000395B0();
extern void func_000395D8();
extern void func_00039668();
extern void ProcessTextureInfo();
extern void func_00052434();
extern void func_000527BC();
extern void func_00054140();
extern void func_0005D89C();
extern void func_00061214();

void func_00022344(char *lump_ptr)
{
    int numLumps;
    int i;
    int lump_type;
    int lump_size;

    numLumps = *(int *)lump_ptr;
    lump_ptr += 4;

    for (i = 0; i < numLumps; i++) {
        lump_type = *(int *)lump_ptr;
        lump_size = *(int *)(lump_ptr + 4);
        lump_ptr += 8;

        switch (lump_type) {
        case 5:
            *(char **)&D_00097568 = lump_ptr;
            break;
        case 12:
            *(char **)&D_000975A8 = lump_ptr;
            break;
        case 34:
            ProcessTextureInfo(lump_ptr);
            break;
        case 7:
            func_00039450(D_0009E714, lump_ptr);
            break;
        case 25:
            D_000970F0 = lump_ptr;
            break;
        case 28:
            D_00097078 = lump_ptr;
            break;
        case 2:
            D_00097114 = lump_ptr;
            break;
        case 22:
            func_0005D89C(lump_ptr, lump_size);
            D_000970D0 = 1;
            break;
        case 1:
            func_00052434(lump_ptr, lump_size);
            if (D_00096804 == 0)
                func_000527BC(D_00097078, 0);
            InitModelNames();
            func_0003181C(D_00096834, 1);
            break;
        case 26:
            ProcessSpoolInfoLump(lump_ptr);
            ProcessMapLump(D_00097114, 0);
            break;
        case 8:
            func_000394E0(lump_ptr, lump_size);
            break;
        case 9:
            func_00039570(lump_ptr, lump_size);
            break;
        case 16:
            func_000395D8(lump_ptr, lump_size);
            break;
        case 17:
            func_00039668(lump_ptr, lump_size);
            break;
        case 10:
            func_000395B0(lump_ptr, lump_size);
            break;
        case 20:
            func_000298B0(lump_ptr, lump_size);
            break;
        case 24:
            func_00054140(lump_ptr, lump_size);
            D_000970A4 = 1;
            break;
        case 33:
            func_00061214(lump_ptr, lump_size);
            break;
        case 21:
            func_000298BC(lump_ptr, lump_size);
            break;
        }
        lump_ptr += (lump_size + 3) & ~3;
    }
}
