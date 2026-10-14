/* debug/cheat key handler: switch on D_0009684C */
typedef struct { short vx, vy, vz, pad; } SVECTOR;
typedef struct { int vx, vy, vz, pad; } VECTOR;
typedef struct {
    char pad0[4];
    char mat[0xC8 - 4];
    int x, y, z;
    char pad1[0xF8 - 0xD4];
    short dir;
    char pad2[0x104 - 0xFA];
    int f104;
} CARX;

extern int D_0009684C;
extern int D_0009709C;
extern unsigned char D_00096894;
extern unsigned char D_00096895;
extern int D_0009687C;
extern int D_00096868;
extern unsigned char D_00096898;
extern unsigned char D_00096899;
extern int D_00096888;
extern int D_00096884;
extern int D_0009681C;
extern int D_0009683C;
extern int D_00096834;
extern long long D_00096AA8;
extern long long D_00096AA9;
extern long long D_00097424;
extern long long D_00096780;
extern long long D_0009677C;
extern CARX D_0009B0A4;
extern void func_0004DF28();
extern void func_0003EAE0();
extern void func_000774C4();
extern void func_0001B4A4();
extern void RotMatrix();

void func_00025C68(void)
{
    VECTOR pos;
    SVECTOR rot2;
    SVECTOR rot;

    switch (D_0009684C) {
    case 1:
        D_0009709C = 1;
        D_00096894 = 0;
        D_0009687C = 0;
        D_00096868 = 0;
        D_0009684C = 0;
        func_0004DF28();
        D_00096898 = 0;
        D_00096899 = 0;
        D_00096888 = 0;
        break;
    case 2:
        D_0009681C ^= 1;
        break;
    case 3:
        func_0003EAE0(0);
        break;
    case 4:
        func_000774C4(&rot, 0, 8);
        rot.vy = D_0009B0A4.dir;
        rot2 = rot;
        pos.vx = D_0009B0A4.x;
        pos.vy = D_0009B0A4.y - 100;
        pos.vz = D_0009B0A4.z;
        func_0001B4A4(&D_0009B0A4, &pos);
        D_0009B0A4.f104 = 0;
        RotMatrix(&rot2, D_0009B0A4.mat);
        break;
    case 5:
        *(char *)&D_00096AA8 = 0xB4;
        *(int *)&D_00097424 = 10000;
        *(char *)&D_00096AA9 = 0;
        break;
    case 6:
        *(char *)&D_00096AA9 = 0xB4;
        *(char *)&D_00096AA8 = 0;
        *(int *)&D_00097424 = 10000;
        break;
    case 7:
        *(char *)&D_00096AA8 = 0;
        *(int *)&D_00097424 = 0;
        *(char *)&D_00096AA9 = 0;
        break;
    case 9:
        D_0009709C = 1;
        D_00096894 = 0;
        D_0009687C = 0;
        D_00096868 = 0;
        D_0009684C = 0;
        func_0004DF28();
        D_00096898 = 0;
        D_00096899 = 0;
        D_00096888 = 0;
        if (++D_0009683C >= 4)
            D_0009683C = 0;
        break;
    case 10:
        D_0009709C = 1;
        D_00096894 = 0;
        D_0009687C = 0;
        D_0009684C = 0;
        func_0004DF28();
        D_00096888 = 0x17;
        D_00096884 = 0x17;
        *(int *)&D_00096780 = 0;
        *(int *)&D_0009677C = 1;
        if (D_00096834 == 2 || D_00096834 == 6)
            D_00096898 = 7;
        else
            D_00096898 = 6;
        D_00096899 = 0;
        break;
    case 11:
        D_0009709C = 1;
        D_00096894 = 0;
        D_0009687C = 0;
        D_0009684C = 0;
        func_0004DF28();
        D_00096884 = 0x17;
        D_00096888 = 0;
        if (D_00096834 == 2 || D_00096834 == 6)
            D_00096898 = 8;
        else
            D_00096898 = 3;
        D_00096899 = 0;
        *(int *)&D_00096780 = 1;
        *(int *)&D_0009677C = 0;
        break;
    case 12:
        D_00096899 = 1;
        D_00096895 = 1;
        D_0009709C = 1;
        D_00096894 = 0;
        D_0009687C = 0;
        D_0009684C = 0;
        func_0004DF28();
        break;
    }
}
