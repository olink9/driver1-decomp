/* func_0003181C: resolve the model of every object in an object list and reset its state */

typedef struct {
    int pad0;
    int pad4;
    int x8;
    int xC;
} DEF_b062;

typedef struct {
    DEF_b062 *def;
    int flags;
    short x8;
    unsigned short xA;
    int key;
    int model;
    int x14;
    int x18;
    int x1C;
    char pad20[8];
    short x28;
    unsigned char x2A;
    char pad2B;
    short x2C;
    short x2E;
    short x30;
    short x32;
    short x34;
    short x36;
} OBJ2_b062;

extern OBJ2_b062 **D_00089E84[];
extern int D_000971DC, D_000971D4, D_000971CC, D_000971D8;
extern long long D_00096808;
extern long long D_00097534;
extern int modelpointers[];
extern int func_000396A8();
int printf(const char *fmt, ...);

void func_0003181C(int idx, int load)
{
    OBJ2_b062 **pp = D_00089E84[idx];
    OBJ2_b062 *o;
    int i;

    o = *pp++;
    D_000971DC = 0;
    D_000971D4 = 0;
    D_000971CC = 0;
    D_000971D8 = 0;
    if (o != 0) do {
        if (load) {
            o->model = func_000396A8(o->key);
            if (o->model == 0) {
                printf("Model %s was not found\n", o->key);
                while (*(int *)&D_00097534 != 0x78654321)
                    __asm__ volatile("break 1024");
            } else {
                i = 0;
                while (o->model != modelpointers[i])
                    i++;
                o->x28 = i;
            }
        }
        if (o->flags & 0x40000) {
            if (*(int *)&D_00096808 != 0x14)
                continue;
            o->x14 = o->def->x8;
            o->x18 = o->x30;
            o->x1C = o->def->xC;
            if (o->flags & 0x10)
                o->x8 = o->xA;
            else
                o->x8 = 0;
            if ((o->flags & 0x8000) && *(int *)&D_00096808 == 0xD)
                o->x8 = 0x82;
            o->x2A = 2;
            o->flags = (o->flags & 0xFFFFFF) | 0x9000000;
            if (o->x2E != 0x7D00)
                o->x2C = 100;
            else
                o->x2C = 0x7D00;
        } else {
            if ((o->flags & 0x10000) && *(int *)&D_00096808 == 0x18)
                o->x14 = -200000;
            else
                o->x14 = o->def->x8;
            o->x18 = o->x30;
            o->x1C = o->def->xC;
            if (o->flags & 0x10)
                o->x8 = o->xA;
            else
                o->x8 = 0;
            if ((o->flags & 0x8000) && *(int *)&D_00096808 == 0xD)
                o->x8 = 0x82;
            o->x2A = 2;
            o->flags = (o->flags & 0xFFFFFF) | 0x9000000;
            if (o->x2E != 0x7D00)
                o->x2C = 100;
            else
                o->x2C = 0x7D00;
        }
        o->x32 = 0;
        o->x34 = 0;
        o->x36 = 0;
    } while ((o = *pp++) != 0);
}
