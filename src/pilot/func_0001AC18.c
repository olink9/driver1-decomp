typedef struct {
    char pad0[2]; unsigned char flag; char pad3[7];
    struct { short id; char pad[6]; } exits[4];
    char pad2a[2];
} JN;
typedef struct {
    short s0; short a; short b; char pad[10];
} RD;
extern short D_00096FE8;
extern long long D_00097040;
extern long long D_00097044;
extern long long D_00097050;
extern long long D_00097054;
extern long long D_00097058;
#define junctions (*(JN **)&D_00097040)
#define roads (*(RD **)&D_00097044)

int func_0001AC18(int road)
{
    short found = -1;
    int i;
    int j;
    short cur;
    short other;

    if (D_00096FE8 == -1)
        return 0;
    i = 0;
    if (D_00096FE8 == road)
        return 0;
    cur = D_00096FE8;
    other = roads[road].b;
    for (i = 0; i < 4; i++) {
        if (junctions[roads[road].a].exits[i].id == cur) {
            found = roads[road].a;
            break;
        }
    }
    if (found == -1) {
        i = 0;
        if (other == -1)
            return 0;
        for (i = 0; i < 4; i++) {
            if (junctions[other].exits[i].id == D_00096FE8) {
                found = other;
                break;
            }
        }
        if (found == -1)
            return 0;
    }
    
    if (junctions[found].flag != 1)
        return 0;
    for (j = 0; j < 4; j++) {
        if (junctions[found].exits[j].id == road)
            break;
    }
    switch (i) {
    case 0: if (j == 3) return 0; break;
    case 1: if (j == 0) return 0; break;
    case 2: if (j == 1) return 0; break;
    case 3: if (j == 2) return 0; break;
    }
    if (i & 1) {
        if (*(unsigned char *)&D_00097050 != 1)
            return 0;
        if (*(unsigned int *)&D_00097058 < 0x127)
            return 1;
    } else if (*(unsigned char *)&D_00097054 == 1) {
        if (*(unsigned int *)&D_00097058 < 0x127)
            return 1;
    }
    return 0;
}
