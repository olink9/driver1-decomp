/* per-mission/per-car extra-object setup dispatch (switch on D_00096834, then on kind) */
extern long long D_00097050, D_00097054;
extern long long D_00096834;
extern long long D_000970E8;
extern void func_0004343C();
extern void func_00042EBC();
extern void func_00042C8C();
extern void func_0005326C();
extern void func_00053600();
extern void func_00053764();
extern void func_0004A718();
extern void func_0004A83C();

typedef struct {
    char pad[13];
    unsigned char dir;
} OBJ;

void func_00052CDC(OBJ *obj, int kind)
{
    unsigned short angle;
    unsigned char t;
    char pad[16];

    angle = obj->dir << 6;
    switch (*(int *)&D_00096834) {
    case 0:
        switch (kind) {
        case 0:
            if ((unsigned short)(angle + 39) < 79 || (unsigned short)(angle - 2001) < 79)
                t = *(unsigned char *)&D_00097054;
            else
                t = *(unsigned char *)&D_00097050;
            switch (t) {
            case 1: case 2:
                func_0004343C(obj, -0x5B4, -0x406, -0x50, 0x200);
                func_0004343C(obj, -0x17C, -0x406, -0x50, 0x200);
                break;
            case 4:
                func_0004343C(obj, -0x573, -0x406, -0x50, 0x400);
                func_0004343C(obj, -0x13B, -0x406, -0x50, 0x400);
                break;
            case 3:
                func_0004343C(obj, -0x532, -0x406, -0x50, 0x800);
                func_0004343C(obj, -0xFA, -0x406, -0x50, 0x800);
                break;
            }
            break;
        case 1:
            if ((unsigned short)(angle + 39) < 79 || (unsigned short)(angle - 2001) < 79)
                t = *(unsigned char *)&D_00097054;
            else
                t = *(unsigned char *)&D_00097050;
            switch (t) {
            case 1: case 2:
                func_0004343C(obj, -0x5FA, -0x410, -0x50, 0x200);
                func_0004343C(obj, -0x1B8, -0x410, -0x50, 0x200);
                break;
            case 4:
                func_0004343C(obj, -0x5B9, -0x410, -0x50, 0x400);
                func_0004343C(obj, -0x177, -0x410, -0x50, 0x400);
                break;
            case 3:
                func_0004343C(obj, -0x578, -0x410, -0x50, 0x800);
                func_0004343C(obj, -0x136, -0x410, -0x50, 0x800);
                break;
            }
            break;
        case 2:
            if (*(int *)&D_000970E8)
                func_00042C8C(obj, 0, -0x2BC, 0, 0);
            break;
        case 3:
            if (*(int *)&D_000970E8) {
                func_00042EBC(obj, 0x271, -0x546, 0, 1);
                func_00042EBC(obj, -0x271, -0x546, 0, 1);
            }
            break;
        case 4:
            if (*(int *)&D_000970E8)
                func_00042EBC(obj, -0x258, -0x5AA, 0, 1);
            break;
        case 5:
            func_0005326C(obj);
            break;
        case 6:
            func_00053600(obj);
            break;
        }
        break;
    case 1:
        switch (kind) {
        case 0:
        case 1:
            func_00053764(angle, obj, -0x2EE);
            break;
        case 2:
            if (*(int *)&D_000970E8)
                func_00042EBC(obj, -0x2EE, -0x73A, 0, 1);
            break;
        case 3:
            if (*(int *)&D_000970E8)
                func_00042EBC(obj, -0x12C, -0x6D6, 0, 1);
            break;
        case 4:
            if (*(int *)&D_000970E8)
                func_00042C8C(obj, 0, -0x578, 0, 1);
            break;
        case 5:
            if (*(int *)&D_000970E8)
                func_00042C8C(obj, 0, -0x546, 0, 1);
            break;
        }
        break;
    case 2:
        break;
    case 3:
        switch (kind) {
        case 0:
            func_00053764(angle, obj, -0x302);
            break;
        case 1:
            func_00053764(angle, obj, -0x2DA);
            break;
        case 2:
            func_00042EBC(obj, -0x384, -0x92E, 0, 1);
            break;
        case 3:
            func_00042EBC(obj, 0x44C, -0x866, 0, 2);
            func_00042EBC(obj, -0x44C, -0x866, 0, 2);
            break;
        }
        break;
    case 4:
        break;
    case 5:
        switch (kind) {
        case 0:
            func_00053764(angle, obj, -0x3E6);
            break;
        case 1:
            func_00053764(angle, obj, -0x3DE);
            break;
        case 2:
            if (*(int *)&D_000970E8)
                func_00042EBC(obj, -0x2EE, -0x8CA, 0, 2);
            break;
        case 3:
            if (*(int *)&D_000970E8) {
                func_00042EBC(obj, 0x5AA, -0xF0A, 0, 3);
                func_00042EBC(obj, -0x5AA, -0xF0A, 0, 3);
            }
            break;
        case 4:
            if (*(int *)&D_000970E8)
                func_00042C8C(obj, 0, -0x492, 0, 0);
            break;
        case 5:
            func_0004A718(obj);
            break;
        case 6:
            func_0004A83C(obj);
            break;
        }
        break;
    }
}
