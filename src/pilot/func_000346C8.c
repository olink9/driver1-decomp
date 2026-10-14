/* Driver 2: ProcessSpoolInfoLump */
extern long long D_00097098;
extern int D_00097238, D_0009723C, D_00097294, D_00097270, D_00097264, D_000972AC, D_00097254;
extern int D_000AB040[], D_000AA7D4[], D_000AAF90[], D_000AA7E8[];
extern int D_000AA7E4[], D_000AB050[];

void func_000346C8(int *lump)
{
    int size, i, slots, objs, alloc, m, n, r;
    int *a2, *a1;
    char *p = (char *)lump;

    size = *(int *)p;
    size <<= 11;
    p += 4;
    D_00097238 = *(int *)&D_00097098;
    if (size <= 0xFFFF)
        size = 0x10000;
    *(int *)&D_00097098 += size;
    alloc = *(int *)&D_00097098;
    slots = 0;
    objs = 0;
    size = *(int *)p;
    D_0009723C = (int)(p + 4);
    p += size + 4;
    n = *(int *)p;
    D_00097294 = (int)(p + 4);
    p += n * 16 + 4;
    D_00097270 = (int)p;
    p += n * 16;
    a1 = (int *)p;
    D_00097264 = n;
    for (i = 0; i < 4; i++) {
        D_000AA7E8[i] = alloc;
        D_000AAF90[i] = -1;
        D_000AA7D4[i] = slots;
        D_000AB040[i] = objs;
        slots += a1[i];
        objs += a1[i + 4];
        alloc += (a1[i + 8] + 0x7FF) & -0x800;
    }
    p += 0x30;
    D_000AA7E4[0] = slots;
    D_000AB050[0] = objs;
    *(int *)&D_00097098 = alloc;
    r = *(int *)p;
    D_000972AC = (int)(p + 4);
    p += r * 2 + 4;
    D_00097254 = (int)(p + 4);
}
