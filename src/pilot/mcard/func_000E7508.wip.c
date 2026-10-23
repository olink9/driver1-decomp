/* WIP 96.93: control flow, calls, jump table (mode 4 and 6 both checksum the save via func_000EC4DC) and statement
 * order all match; the ONLY diff left is one register choice. The target keeps the "CHECKING" string (D_000E66AC,
 * `msg`) in s6 for the whole function and spills loop.c's hoisted %hi(D_000EFC00) (rematerialised as `lui t0` at
 * each use); here the hoisted HIGH (7 refs / 250, REG_EQUIV) outranks msg (7 refs / 306, REG_EQUIV) in global-alloc,
 * so msg is rematerialised (`lui t0; addiu t0` at its 4 uses) and %hi(D_000EFC00) gets s7, shifting done to s6.
 * Tried: msg as hoisted constant (no user var), msg set earlier/later/twice/chained, volatile/long long/goto loop for
 * D_000EFC00, h as hoisted vs user var, extra refs via duplicated case arms. Needs one more ref on msg (8 refs ->
 * floor_log2 3) or a non-REG_EQUIV msg.
 * (ov1-wip M) .greg numbers: done (91) 8/322 -> 745, HIGH(D_000EFC00) 240 7/250 -> 560, h 85 7/252 -> 555,
 * msg 84 7/306 -> 457. Target has msg s6 / done s7, so msg must beat done too: needs >745 (8 refs -> 784, or 7 refs
 * without the REG_EQUIV doubling -> 915). Both HIGH and the hoisted constant 2 are reloaded into t0 in the target
 * (spilled), buf is spilled to 0x64(sp) in both. Tried: pre-loop stores of the literal (CSE still uses msg), a
 * block-local `s` copied into msg. */
typedef struct { short x, y, w, h; } RECT;
typedef struct DB { int a; char *b; int c; char pad[0x74]; } DB;
typedef struct { char fname[0x18]; char title[0x24]; int size; unsigned char idx; unsigned char n; unsigned char f42; unsigned char f43; } E;
typedef struct { unsigned short s; unsigned char c2; char c3; void *p4; E e[15]; } H;
extern H D_000EE930[];
extern DB *current;
extern DB *D_00097524;
extern short D_00097538;
extern int D_000EF144;
extern int D_000ED870;
extern int D_000EE900;
extern int D_000EFC00;
extern int D_000ED7CC;
extern int D_000ED7C4;
extern int D_000ED7C8;
extern int D_000EF138;
extern int D_000EF13C;
extern int D_000EF140;
extern char D_000ED784[];
extern char D_000E6608[];
extern char D_000E6610[];
extern char D_000E6618[];
extern char D_000E6620[];
extern char D_000E6628[];
extern char D_000E6634[];
extern char D_000E6644[];
extern char D_000E6650[];
extern char D_000E6664[];
extern char D_000E6670[];
extern char D_000E6680[];
extern char D_000E6690[];
extern char D_000E66AC[];
extern char D_000E66C4[];
extern void func_00074374();
extern void DrawSync();
extern void VSync();
extern void SetDispMask();
extern void func_00077E48();
extern void func_00077E98();
extern void func_00077ED8();
extern void func_00078114();
extern int func_00078E4C();
extern void func_0007457C();
extern void func_000514E0();
extern int sprintf();
extern int printf();
extern void func_000E8DD4();
extern void func_000E7B2C();
extern void func_000EA484();
extern void func_000E950C(), func_000E91B8(), func_000E90D8();
extern int func_000E96F8();
extern int func_000E9CE0();
extern int func_000EC4DC();

/* memory card screen main loop: mode 0-2 load, 3-6 save */
void func_000E7508(int mode, unsigned char *buf, unsigned int size)
{
    char str[16];
    RECT rect;
    int r[2];
    unsigned int st;
    char *msg;
    H *h;
    int ev;
    int port;
    int retry;
    int busy;
    int scr;
    int done;

    scr = 0;
    port = 0;
    done = 0;
    r[1] = 0;
    r[0] = 0;
    rect.x = 0;
    rect.y = 0;
    rect.w = 0x200;
    rect.h = 0x200;
    func_00074374(&rect, 0, 0, 0);
    DrawSync(0);
    func_00077E48();
    if (size + 0x200 <= 0x2000)
        sprintf(str, D_000E6608);
    else
        sprintf(str, D_000E6610);
    D_000ED870 = size / 8192 + (size % 8192 != 0);
    switch (mode) {
    case 0:
        D_000EF144 = 0;
        sprintf(D_000ED784, D_000E6618, D_000E6620);
        break;
    case 1:
        D_000EF144 = 1;
        sprintf(D_000ED784, D_000E6628, D_000E6620);
        break;
    case 2:
        D_000EF144 = 2;
        sprintf(D_000ED784, D_000E6634, D_000E6620);
        break;
    case 3:
        D_000EF144 = 5;
        sprintf(D_000ED784, D_000E6644, D_000E6650, size / 8192 + (size % 8192 != 0), str);
        break;
    case 4:
        D_000EF144 = 3;
        sprintf(D_000ED784, D_000E6644, D_000E6664, size / 8192 + (size % 8192 != 0), str);
        *(int *)(buf + 0x5E0) = D_000EE900 = func_000EC4DC(buf, 0x5E0);
        break;
    case 5:
        D_000EF144 = 4;
        sprintf(D_000ED784, D_000E6644, D_000E6670, size / 8192 + (size % 8192 != 0), str);
        break;
    case 6:
        D_000EF144 = 6;
        sprintf(D_000ED784, D_000E6644, D_000E6680, size / 8192 + (size % 8192 != 0), str);
        *(int *)(buf + 0x5E0) = D_000EE900 = func_000EC4DC(buf, 0x5E0);
        break;
    default:
        printf(D_000E6690);
        return;
    }
    func_000E8DD4(0);
    func_000E8DD4(1);
    msg = D_000E66AC;
    D_000EE930[0].c3 = 1;
    D_000EE930[1].c3 = 1;
    D_000ED7CC = 1;
    D_000ED7C4 = 0;
    D_000ED7C8 = 12;
    D_000EF138 = 0;
    D_000EF13C = 0;
    D_000EE930[0].p4 = msg;
    D_000EE930[1].p4 = msg;
    D_000EF140 = 0;
    func_000EA484();
    DrawSync(0);
    retry = 2;
    func_0007457C(current->b, 0x1080);
    busy = 1;
    func_0007457C(D_00097524->b, 0x1080);
    VSync(0);
    func_00077ED8(scr);
    D_000EFC00 = 0;
    if (!done) {
    h = D_000EE930;
    do {
        ev = func_00078E4C(1, &r[0], &r[1]);
        if (D_000EFC00 == 2)
            SetDispMask(1);
        switch (ev) {
        case 0:
            break;
        case -1:
            retry = 2;
            busy = 0;
            D_000ED7CC = 0;
            scr ^= 0x10;
            port ^= 1;
            func_00077ED8(scr);
            break;
        case 1:
            switch (r[0]) {
            case 1:
                switch (r[1]) {
                case 0:
                    if (h[port].c3) {
                        h[port].p4 = msg;
                        func_00078114(scr);
                        busy = 1;
                    }
                    break;
                case 3:
                    h[port].p4 = msg;
                    func_00078114(scr);
                    busy = 1;
                    break;
                default:
                    func_000E8DD4(port);
                    break;
                }
                break;
            case 2:
                st = r[1];
                switch (st) {
                case 0:
                    busy = 1;
                    func_000E7B2C(port);
                    break;
                case 3:
                    busy = 1;
                    func_000E7B2C(port);
                    break;
                case 4:
                    if (retry) {
                        func_00078114(scr);
                        retry--;
                    } else {
                        func_000E8DD4(port);
                        h[port].p4 = D_000E66C4;
                        h[port].s = st;
                    }
                    break;
                default:
                    func_000E8DD4(port);
                    break;
                }
                break;
            }
            break;
        }
        if (!busy) {
            func_000514E0();
            switch (D_000EF144) {
            case 0:
            case 1:
            case 2:
                done = func_000E96F8(D_00097538, buf, size);
                break;
            case 3:
            case 4:
            case 5:
            case 6:
                done = func_000E9CE0(D_00097538, buf, size);
                break;
            }
        }
        func_000E950C();
        func_000E91B8();
        func_000E90D8();
        D_000EFC00++;
    } while (!done);
    }
    DrawSync(0);
    VSync(0);
    func_00078E4C(0, &r[0], &r[1]);
    func_00077E98();
    func_0007457C(current->b, 0x1080);
    func_0007457C(D_00097524->b, 0x1080);
    DrawSync(0);
    func_000E90D8();
    func_000E90D8();
}
