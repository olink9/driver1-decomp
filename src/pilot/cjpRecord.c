/* Driver 2: cjpRecord (RecordWaypoint and Put inlined) */
typedef struct { int vx, vy, vz, pad; } VEC;
typedef struct { char pad[0xC8]; VEC pos; char pad2[0x18C - 0xC8 - 16]; } CAR;
typedef struct { unsigned char pad, analogue, run; } PADRECORD;
typedef struct { short x, y; } WAYPOINT;

extern CAR car_data[];
extern long long D_00096888;            /* player car index */
extern int D_0009762C;                  /* TimeToWay */
extern short D_00097634;                /* PlayerWaypoints */
extern int D_00097620;                  /* way_distance */
extern WAYPOINT *D_0009765C;            /* PlayerWayRecordPtr */
extern PADRECORD *D_000BEED4[];         /* PadRecordBuffer per stream */
extern PADRECORD *D_000BEEE0[];         /* PadRecordBufferEnd per stream */
extern long long CameraCnt;            /* CameraCnt */
extern long long D_00096899;            /* NoPlayerControl */
extern long long D_00096898;            /* ReplayMode */
extern long long D_000967F4;
extern int D_00096DD8;                  /* ClearCameras */
extern int *ReplayParameterPtr;                 /* ReplayParameterPtr */
extern unsigned char D_0008B6C8[];      /* AnalogueUnpack */
extern void PrintString();

static inline void RecordWaypoint(void)
{
    if (D_0009762C == 0) {
        if (D_00097634 < 150) {
            D_00097634++;
            D_0009765C->x = (car_data[*(int *)&D_00096888].pos.vx + 0x200) / 1024;
            D_0009765C->y = (car_data[*(int *)&D_00096888].pos.vz + 0x200) / 1024;
            D_0009765C++;
            D_0009762C = D_00097620;
        }
    } else
        D_0009762C--;
}

static inline int Put(int stream, unsigned int *pt0)
{
    PADRECORD *padbuf;
    PADRECORD **pp;
    unsigned int t0;

    pp = &D_000BEED4[stream];
    padbuf = *pp;
    t0 = *pt0;
    if (padbuf + 1 >= D_000BEEE0[stream])
        return 0;
    if (*(int *)&CameraCnt == 0) {
        padbuf->pad = t0 >> 8;
        (*pp)->analogue = t0;
        (*pp)->run = 0;
    } else if (padbuf->pad != ((t0 >> 8) & 0xff) || padbuf->analogue != (t0 & 0xff) || padbuf->run == 143) {
        (*pp)++;
        (*pp)->pad = t0 >> 8;
        (*pp)->analogue = t0;
        (*pp)->run = 0;
    } else
        padbuf->run++;
    return 1;
}

/* the NoPlayerControl test is repeated in the target: an inline's return label stops CSE folding it */
static inline void ClearCams(void)
{
    if (*(unsigned char *)&D_00096899 == 0)
        D_00096DD8 = 1;
}

void cjpRecord(int stream, unsigned int *ppad, signed char *psteer, char *ptype)
{
    unsigned int t0;
    int t1;

    t0 = *ppad;
    t1 = *psteer;
    RecordWaypoint();
    t0 &= 0xA0FC;
    if ((*ptype & 2) == 0)
        t1 = 0;
    else if (t1 < -45)
        t1 = (-t1 - 45) / 12 + 1;
    else if (t1 > 45)
        t1 = (t1 - 45) / 12 + 9;
    else
        t1 = 8;
    t0 |= (t1 & 0xF) << 8;
    if (Put(stream, &t0) != 0) {
        ClearCams();
        if (*(unsigned char *)&D_00096899 == 0 && *(unsigned char *)&D_00096898 != 3 && *(unsigned char *)&D_00096898 != 8 && *(int *)&D_000967F4 != 22)
            *ReplayParameterPtr = *(int *)&CameraCnt;
    } else
        PrintString("Out of Tape", 20, 0xD0);
    t1 = (t0 >> 8) & 0xF;
    t0 &= 0xA0FC;
    if (t1 == 0) {
        *psteer = 0;
        *ptype = 0;
    } else {
        *psteer = D_0008B6C8[t1];
        *ptype = 2;
    }
    *ppad = t0;
}
