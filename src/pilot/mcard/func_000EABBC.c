typedef struct { int a; short b; char pad[0x3A]; unsigned char idx; unsigned char n; unsigned char f42; unsigned char f43; } E;
typedef struct { unsigned short s; char c2; char c3; void *p4; E e[15]; } H;
typedef struct { unsigned addr:24; unsigned len:8; } P_TAG;
typedef struct {
    P_TAG tag;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    unsigned char u0, v0; unsigned short clut;
    short x1, y1;
    unsigned char u1, v1; unsigned short tpage;
    short x2, y2;
    unsigned char u2, v2; unsigned short pad1;
    short x3, y3;
    unsigned char u3, v3; unsigned short pad2;
} POLY_FT4;
typedef struct { P_TAG tag; unsigned char r0, g0, b0, code; char rest[0x10]; } POLY_F4;
typedef struct { char *primptr; P_TAG *ot; } DB;
extern DB *current;
extern H D_000EE930[];
extern int D_000ED7C4;
extern POLY_FT4 D_000EF150[2][15];
extern POLY_F4 D_000EF600[2][15];

#define setUV4(p, _u0, _v0, _u1, _v1, _u2, _v2, _u3, _v3) \
    (p)->u0 = (_u0), (p)->v0 = (_v0), (p)->u1 = (_u1), (p)->v1 = (_v1), \
    (p)->u2 = (_u2), (p)->v2 = (_v2), (p)->u3 = (_u3), (p)->v3 = (_v3)

/* animate the save-slot icons of one card (frame = (tick + i) % frames) */
void func_000EABBC(int port)
{
    int i;

    if (port < 3) {
        for (i = 0; i < 15; i++) {
            if (D_000EE930[port].e[i].f43 & 1) {
                switch ((D_000ED7C4 + i) % D_000EE930[port].e[i].f42) {
                case 0:
                    setUV4(&D_000EF150[port][i], i * 16, port * 48, i * 16 + 15, port * 48,
                           i * 16, port * 48 + 15, i * 16 + 15, port * 48 + 15);
                    break;
                case 1:
                    setUV4(&D_000EF150[port][i], i * 16, port * 48 + 16, i * 16 + 15, port * 48 + 16,
                           i * 16, port * 48 + 31, i * 16 + 15, port * 48 + 31);
                    break;
                case 2:
                    setUV4(&D_000EF150[port][i], i * 16, port * 48 + 32, i * 16 + 15, port * 48 + 32,
                           i * 16, port * 48 + 47, i * 16 + 15, port * 48 + 47);
                    break;
                }
                D_000EF150[port][i].tag.addr = current->ot[8].addr;
                current->ot[8].addr = (unsigned int)&D_000EF150[port][i];
            } else {
                D_000EF600[port][i].tag.addr = current->ot[8].addr;
                current->ot[8].addr = (unsigned int)&D_000EF600[port][i];
            }
        }
    }
}
