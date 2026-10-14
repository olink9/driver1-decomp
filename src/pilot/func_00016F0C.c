/* func_00016F0C - cop/civ car: choose exit at a junction (no Driver 2 match) */

typedef struct {
    short a;
    short road;
    unsigned short flags;
    short b;
} JEXIT;

typedef struct {
    char pad0[3];
    unsigned char flag;
    char pad4[4];
    JEXIT e[4];
    char pad28[4];
} JUNC; /* 44 bytes */

typedef struct {
    char pad[11];
    unsigned char dir;
    char padc[4];
} ROAD16;

typedef struct {
    unsigned char lanes[4];
    char pad[16];
} RINFO;

typedef struct {
    char pad0[3];
    unsigned char b3;
    unsigned char b4;
    char pad5[4];
    unsigned char lane;
    char padA[13];
    unsigned char b17;
    char pad18[12];
    int dir;
    int road;
    int target;
    int exit;
    int turn;
    char pad38[4];
    int x3C;
} CAR;

typedef struct {
    char pad[12];
    short junc;
} NODE;

extern long long D_00097038;
extern long long D_00097040;
extern long long D_00097044;
extern RINFO D_0009E744[];

#define junctions (*(JUNC **)&D_00097040)
#define roads16 (*(ROAD16 **)&D_00097044)

extern void func_00013A90();
extern int func_0004FFE8();

#define SETEXIT() \
    c = choices[r]; \
    car->exit = c; \
    car->target = roads[c]; \
    if (car->exit == car->dir) { \
        car->turn = 0; \
    } else { \
        car->turn = (((car->exit - car->dir) << 10) + 0x800 & 0xFFF) - 0x800 >= 0 ? 1 : -1; \
    }

int func_00016F0C(CAR *car, NODE *node)
{
    unsigned char count;
    unsigned char lane;
    JEXIT *e;
    short roads[4];
    unsigned char choices[4];
    unsigned char allowed[4];
    unsigned char t;
    unsigned char i;
    unsigned char r;
    int dir;
    int d;
    short rd;
    int sgn;
    JUNC *jn;
    unsigned char c;
    unsigned char n;

    count = 0;
    lane = 0;

    if (node->junc == -1 || node->junc == 0x3FFF)
        return 0;

    if (junctions[node->junc].flag) {
        allowed[3] = 1;
        allowed[2] = 1;
        allowed[1] = 1;
        allowed[0] = 1;
    } else {
        allowed[3] = 0;
        allowed[2] = 0;
        allowed[1] = 0;
        allowed[0] = 0;
    }

    if ((unsigned int)car->road < 0x4000) {
        lane = car->lane;
        dir = car->dir;
        t = D_0009E744[car->road].lanes[dir] - 1;
        switch (dir) {
        case 0:
            if (t == 0) {
                allowed[0] = 1;
                allowed[1] = 1;
                allowed[3] = 1;
            } else if (lane == 0) {
                allowed[0] = 1;
                allowed[1] = 1;
            } else if (lane == t) {
                allowed[0] = 1;
                allowed[3] = 1;
            } else {
                allowed[0] = 1;
            }
            break;
        case 2:
            if (t == 0) {
                allowed[2] = 1;
                allowed[1] = 1;
                allowed[3] = 1;
            } else if (lane == 0) {
                allowed[2] = 1;
                allowed[3] = 1;
            } else if (lane == t) {
                allowed[2] = 1;
                allowed[1] = 1;
            } else {
                allowed[2] = 1;
            }
            break;
        case 1:
            if (t == 0) {
                allowed[0] = 1;
                allowed[1] = 1;
                allowed[2] = 1;
            } else if (lane == 0) {
                allowed[2] = 1;
                allowed[1] = 1;
            } else if (lane == t) {
                allowed[1] = 1;
                allowed[0] = 1;
            } else {
                allowed[1] = 1;
            }
            break;
        case 3:
            if (t == 0) {
                allowed[0] = 1;
                allowed[3] = 1;
                allowed[2] = 1;
            } else if (lane == 0) {
                allowed[3] = 1;
                allowed[0] = 1;
            } else if (lane == t) {
                allowed[2] = 1;
                allowed[3] = 1;
            } else {
                allowed[3] = 1;
            }
            break;
        }
    } else {
        allowed[3] = 1;
        allowed[1] = 1;
        allowed[2] = 1;
        allowed[0] = 1;
    }

    car->x3C = 0x82;
    car->b4 = *(unsigned char *)&D_00097038 + 1;
    car->b3 = 0;

    e = junctions[node->junc].e;
    roads[0] = e[0].road;
    roads[1] = e[1].road;
    roads[2] = e[2].road;
    roads[3] = e[3].road;

    rd = roads[0];
    if (!(e[0].flags & 2) && rd != -1 && rd != car->road
        && roads16[rd].dir != 0x82 && allowed[0]) {
        choices[count++] = 0;
        t = D_0009E744[rd].lanes[0] - 1;
        if (car->dir == 0 && t < car->lane)
            count--;
    }
    rd = roads[2];
    if (!(e[2].flags & 2) && rd != -1 && rd != car->road
        && roads16[rd].dir != 0x80 && allowed[2]) {
        choices[count++] = 2;
        t = D_0009E744[rd].lanes[2] - 1;
        if (car->dir == 2 && t < car->lane)
            count--;
    }
    rd = roads[1];
    if (!(e[1].flags & 2) && rd != -1 && rd != car->road
        && roads16[rd].dir != 0x83 && allowed[1]) {
        choices[count++] = 1;
        t = D_0009E744[rd].lanes[1] - 1;
        if (car->dir == 1 && t < car->lane)
            count--;
    }
    rd = roads[3];
    if (!(e[3].flags & 2) && rd != -1 && rd != car->road
        && roads16[rd].dir != 0x81 && allowed[3]) {
        choices[count++] = 3;
        t = D_0009E744[rd].lanes[3] - 1;
        if (car->dir == 3 && t < car->lane)
            count--;
    }

    if (count == 0) {
        for (i = 0; i < 4; i++) {
            rd = roads[i];
            if (!(e[i].flags & 2) && rd != -1 && rd != car->road
                && roads16[rd].dir != ((i + 2) & 3) + 0x80)
                choices[count++] = i;
        }
        if (count == 0)
            return 0;
    }

    if (count == 1)
        car->b17 |= 4;

    jn = &junctions[node->junc];
    func_00013A90(jn->e[0].road, jn->e[1].road, jn->e[2].road, jn->e[3].road);

    r = func_0004FFE8(lane) % count;
    SETEXIT();

    if (count == 3 && car->turn < 0) {
        if (node->junc & 1) {
            if ((unsigned int)(car->exit - 2) < 2) {
                if ((n = r + 1) >= 3)
                    n -= count;
                r = n;
                SETEXIT();
            }
        } else {
            if ((unsigned int)car->exit < 2) {
                if ((n = r + 1) >= 3)
                    n -= count;
                r = n;
                SETEXIT();
            }
        }
    }
    return 1;
}
