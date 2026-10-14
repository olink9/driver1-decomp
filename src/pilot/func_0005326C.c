typedef struct { int x, y, z; } V3;
typedef struct {
    V3 v;
    char pad;
    unsigned char b;
} OBJ;
typedef struct {
    OBJ *cur;
    V3 v1;
    V3 v2;
    short s;
    unsigned char c;
} CAM;
extern long long D_00097704, D_00097708;
extern V3 D_0008AE7C[];
extern CAM D_000BE754;

void func_0005326C(OBJ *o) {
    CAM *c;
    if (*(int *)&D_00097704 == 0 || *(int *)&D_00097708 < 0x293) {
        if (D_0008AE7C[0].x == o->v.x && D_0008AE7C[0].z == o->v.z) {
            c = &D_000BE754;
            if (c->cur != o) {
                if (c->cur != 0) o->v = c->v1;
                c->v1 = o->v;
                c->v2 = c->v1;
                c->c = o->b << 6;
                c->cur = o;
                c->s = 0;
                o->v.x = 0xFD46FEC0;
            }
        }
    }
}
