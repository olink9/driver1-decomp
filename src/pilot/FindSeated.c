/* FindSeated */
typedef struct {
    int x, z;
    short rotation;
    unsigned char index, pad;
} SEATED;
typedef struct { char pad[0xC8]; int pos[3]; } PLAYER;

extern SEATED *D_000976CC;     /* seated_pedestrian */
extern int D_000976B8;         /* seated_count */
extern long long D_00096834;
extern PLAYER car_data[];    /* player */
extern int func_0004FFE8(unsigned char);
extern void func_00060F54(SEATED *, int);

SEATED *FindSeated(void)
{
    SEATED *seatedptr;
    int dx, dz, count1;
    PLAYER *pl;

    seatedptr = D_000976CC;
    if (*(int *)&D_00096834 == 7)
        return 0;
    if (!seatedptr)
        return 0;
    if (!(func_0004FFE8((int)seatedptr) & 1))
        return 0;

    count1 = 3;
    pl = car_data;
loop:
    seatedptr = D_000976CC + ++D_000976B8;
    if (seatedptr->rotation == 9999) {
        D_000976B8 = 0;
        seatedptr = D_000976CC;
    }
    if (seatedptr->index == 0) {
        dx = (seatedptr->x - pl->pos[0]) >> 12;
        dz = (seatedptr->z - pl->pos[2]) >> 12;
        count1--;
        if ((unsigned)(dx * dx + dz * dz) - 11U < 29) {
            func_00060F54(seatedptr, D_000976B8);
            return seatedptr;
        }
    }
    if (count1 != 0)
        goto loop;
    return 0;
}
