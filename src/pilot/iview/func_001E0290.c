/* FAKEMATCH: unused 'char pad16[16]' local only sizes the stack frame (0x50; gear spill at sp+0x20) - the original
   probably had a 16-byte local whose uses were optimised away. Everything else is plain C. */
/* IVIEW entry/per-frame update (called from main). */
typedef struct {
    char pad0[0x40];
    int f40, f44, f48;
    int f4C;
    int f50;
    char pad54[0xC8 - 0x54];
    int fC8, fCC, fD0;
    char padD4[0x104 - 0xD4];
    int f104;
    char pad108[0x17C - 0x108];
    short f17C;
    char pad17E[0x18C - 0x17E];
} CAR;

typedef struct { int vx, vy, vz, pad; } VEC;

extern CAR car_data[];
extern CAR D_0009D438[];
extern VEC camera_position;
extern int D_00096874, D_00096888, D_00096858, D_00096890, D_00096A6C, D_00096878;
extern int D_000970A8, D_0009709C, D_000970F4, CameraCnt, pauseflag, FastForward;
extern unsigned char D_00096899;
extern short D_0009752C;
extern char cameraview, tracking_car;
extern char D_000BC9BC[];

extern char D_001E0000[], D_001E0010[], D_001E0028[], D_001E0044[], D_001E0064[], D_001E0090[];
extern char D_001E00B0[], D_001E00C4[], D_001E00DC[];
extern char D_001E3490[], D_001E3508[];
extern unsigned char D_001E33F4[][15];
extern int D_001E4048[];
extern int D_001E3358, D_001E335C, D_001E3360, D_001E3364, D_001E34F8, D_001E3500, D_001E3558;
extern int D_001E3570, D_001E3588, D_001E3590, D_001E35B8, D_001E4028, D_001E4030, D_001E4080;
extern int D_001E4088, D_001E4090, D_001E4098, D_001E4248, D_001E4260, D_001E4268, D_001E4278;
extern int D_001E4288, D_001E42A0, D_001E42A8, D_001E42C0, D_001E42D0, D_001E42D8;

extern void SetTextColour(unsigned char, unsigned char, unsigned char);
extern void PrintString();
extern int sprintf();
extern int func_0004C934();
extern int func_0004FFE8();
extern void func_0005C8F4();
extern void func_001E282C(), func_001E232C(), func_001E16D8(), func_001E1248(), func_001E14D4();
extern int func_001E19E4();
extern void func_001E3310();

void func_001E0290(void)
{
    char pad16[16];
    CAR *cp;
    int pad;
    int zone;
    int state;
    int t;
    int speed;
    int spin;
    int gear;
    int flag;
    int i;
    int n;
    int k;
    int match;

    zone = 0;
    SetTextColour(127, 127, 127);
    if (D_00096874)
        cp = D_0009D438;
    else
        cp = &car_data[D_00096888];
    if (D_00096874 || D_00096899)
        pad = D_000970F4;
    else
        pad = D_0009752C;
    if (D_00096858) {
        func_001E282C();
        func_001E232C();
    }
    if (D_00096874) {
        {
            int kk;
            t = CameraCnt & 0x1F;
            if (t < 16)
                kk = t;
            else
                kk = 32 - t;
            SetTextColour(kk * 8, 0, 0);
        }
        PrintString(D_001E0000, 0xD2, 0x1E);
        D_00096890 = 0x17;
        cameraview = 1;
        tracking_car = 1;
        camera_position.vx = -0x2904;
        camera_position.vz = -15000;
        camera_position.vy = -500;
        func_001E16D8(20);
    } else {
        func_001E1248();
    }
    func_001E14D4();
    if (!pauseflag)
        D_001E3588++;
    state = D_001E335C;
    if (state == 0 || state == 4) {
        if (!D_00096899)
            D_00096A6C = (D_00096A6C + 0x40) & 0xFFF;
        if (state == 0) {
            D_001E4098 = 0x1F;
            if (D_00096874)
                sprintf(D_001E3490, D_001E0010);
            else
                sprintf(D_001E3490, D_001E0028);
        }
        D_001E335C = 4;
        D_001E42D8 = 0;
        if (D_001E3588 > 60) {
            D_001E335C = 1;
            D_00096A6C = 0x800;
            D_00096878 = 0;
            D_001E3588 = 0;
            return;
        }
    } else if (state == 2) {
        if (!D_00096899)
            D_00096A6C = (D_00096A6C - 0x16) & 0xFFF;
        if (!D_00096874)
            D_001E4098 = 0x1F;
        sprintf(D_001E3490, D_001E0044);
        if (!D_001E42D8) {
            D_001E42D8 = 1;
            func_001E3310(D_000BC9BC, 0x6B);
        }
        if (func_0004C934(D_000BC9BC)) {
            D_000970A8 = 1;
            D_0009709C = 1;
        }
    } else if (state == 3) {
        if (!D_00096874)
            D_00096A6C = (D_00096A6C - 0x16) & 0xFFF;
        if (D_001E35B8 == 4) {
            D_001E4098 = 0x1F;
            sprintf(D_001E3490, D_001E0064);
            if (!D_001E42D8) {
                D_001E42D8 = 1;
                func_001E3310(D_000BC9BC, 0x69);
            }
        } else {
            D_001E4098 = 0x1F;
            sprintf(D_001E3490, D_001E0090);
            if (!D_001E42D8) {
                D_001E42D8 = 1;
                func_001E3310(D_000BC9BC, 0x6A);
            }
        }
        if (!D_00096874)
            cameraview = 0;
        tracking_car = 1;
        if (func_0004C934(D_000BC9BC))
            D_0009709C = 1;
    } else {
        if (D_001E4278 < 0x1000) {
            if (!pauseflag)
                D_001E4278 += D_001E42D0;
        } else {
            D_00096878 = 1;
            if (cp->f40 < 6 && cp->f44 < 6 && cp->f48 < 6) {
                D_001E335C = 3;
                D_001E3588 = 0;
                if (!D_00096874)
                    cameraview = 0;
                tracking_car = 1;
                return;
            }
        }
        speed = cp->f104 >> 12;
        if (speed < 0) {
            D_001E42A0 = 1;
            speed = -speed;
        } else {
            D_001E42A0 = 0;
        }
        if (D_001E42A8 < speed)
            D_001E42A8 = speed;
        spin = cp->f50 >> 12;
        if (spin < 0)
            spin = -spin;
        if (D_001E4248 < spin)
            D_001E4248 = spin;
        gear = cp->f17C;
        if (D_001E35B8 == 4) {
            D_001E335C = 3;
            D_00096878 = 1;
            D_001E3588 = 0;
            return;
        }
        {
            int x = cp->fC8;
            int z = cp->fD0;

            if ((unsigned)(x + 0x43FF) < 0x1000) {
                if ((unsigned)(z + 0x2800) < 0x1000)
                    zone = 5;
                else if ((unsigned)(z + 0x3400) < 0xC00)
                    zone = 4;
                else if ((unsigned)(z + 0x4000) < 0xC00)
                    zone = 3;
                else if ((unsigned)(z + 0x4C00) < 0xC00)
                    zone = 2;
                else if ((unsigned)(z + 0x5C00) < 0x1000)
                    zone = 1;
            } else if ((unsigned)(x + 0x1CE1) < 0x10E2) {
                if ((unsigned)(z + 0x2800) < 0x1000)
                    zone = 6;
                else if ((unsigned)(z + 0x3400) < 0xC00)
                    zone = 7;
                else if ((unsigned)(z + 0x4000) < 0xC00)
                    zone = 8;
                else if ((unsigned)(z + 0x4C00) < 0xC00)
                    zone = 9;
                else if ((unsigned)(z + 0x5C00) < 0x1000)
                    zone = 10;
            } else {
                zone = 0;
            }
        }
        if (zone != D_001E4048[0]) {
            n = 12;
            while (n--)
                D_001E4048[n + 1] = D_001E4048[n];
            D_001E4048[0] = zone;
            i = 6;
            while (i--) {
                match = 1;
                n = D_001E33F4[i][0];
                while (n--) {
                    if (D_001E33F4[i][n + 2] != D_001E4048[n]) {
                        match = 0;
                        break;
                    }
                }
                if (match) {
                    D_001E3558 += D_001E33F4[i][1];
                    D_001E4028 = 0;
                    D_001E4030 = 0;
                    D_001E3570 = 0;
                    if (i < 4)
                        func_001E19E4(2);
                    else
                        func_001E19E4(8);
                    break;
                }
                match = 1;
                n = D_001E33F4[i][0];
                k = 2;
                while (n--) {
                    if (D_001E33F4[i][k] != D_001E4048[n]) {
                        match = 0;
                        break;
                    }
                    k++;
                }
                if (match) {
                    D_001E3558 += D_001E33F4[i][1];
                    D_001E4028 = 0;
                    D_001E4030 = 0;
                    D_001E3570 = 0;
                    if (i < 4)
                        func_001E19E4(2);
                    else
                        func_001E19E4(8);
                }
            }
        }
        if (spin > 300) {
            if (D_001E3500) {
                if (D_001E4090)
                    D_001E4090--;
                else if (func_001E19E4(4))
                    func_001E3310(D_000BC9BC, 0x68);
            } else {
                D_001E3500 = 1;
                D_001E4090 = 35;
            }
        } else {
            D_001E3500 = 0;
        }
        if (spin > 250) {
            if (D_001E34F8) {
                if (D_001E4088)
                    D_001E4088--;
                else
                    func_001E19E4(3);
            } else if (pad & 0x10) {
                D_001E34F8 = 1;
                D_001E4088 = 20;
            }
        } else {
            D_001E34F8 = 0;
        }
        if (D_001E4028 > 0)
            D_001E4028--;
        if (D_001E4030 > 0)
            D_001E4030--;
        if (D_001E3570 > 0)
            D_001E3570--;
        if (D_001E4268 > 0)
            D_001E4268--;
        if (D_001E4260 > 0)
            D_001E4260--;
        flag = 0;
        if (D_001E3358 && D_001E4268 == 0) {
            flag = 1;
            D_001E4268 = 30;
            sprintf(D_001E3490, D_001E00B0);
            D_001E4098 = 25;
            func_001E3310(D_000BC9BC, (func_0004FFE8(CameraCnt) & 1) ? 0x66 : 0x65);
        }
        if (gear - D_001E42C0 >= 2 && D_001E4268 == 0) {
            D_001E4268 = 30;
            D_001E4098 = 25;
            sprintf(D_001E3490, D_001E00C4);
            flag = 1;
            func_001E3310(D_000BC9BC, (func_0004FFE8(CameraCnt) & 1) ? 0x64 : 0x65);
        }
        D_001E42C0 = gear;
        D_001E3358 = 0;
        if (flag && D_001E35B8 < 4)
            D_001E35B8++;
        if (D_001E42A0 && speed > 100)
            D_001E4288 = 1;
        if (spin > 200 && D_001E4288) {
            if (D_001E3364 == 0)
                func_001E19E4(5);
            else
                D_001E3364--;
        } else {
            D_001E4288 = 0;
            D_001E3364 = 20;
        }
        if (cp->f104 > 0xAA000) {
            D_001E3590 = 1;
            D_001E3360 = 20;
        } else if (!(pad & 0x80) || flag) {
            D_001E3590 = 0;
        }
        if (D_001E3590) {
            if (D_001E3360)
                D_001E3360--;
            else
                func_001E19E4(7);
        }
        if (cp->f104 > 0xB4000)
            D_001E4080++;
        if (flag || cp->f104 < 0)
            D_001E4080 = 0;
        if (D_001E4080 >= 6 && cp->f104 < 0x14000) {
            if (func_001E19E4(6))
                func_001E3310(D_000BC9BC, 0x67);
            D_001E4080 = 0;
        }
        if ((pad & 0x20) && speed > 50)
            func_001E19E4(0);
        if ((pad & 0x10) && speed > 50)
            func_001E19E4(1);
    }
    if (D_001E4098 > 0) {
        if (D_001E4098 < 13)
            SetTextColour(D_001E4098 << 3, D_001E4098 << 3, D_001E4098 << 3);
        else
            SetTextColour(127, 127, 127);
        if (!FastForward)
            func_0005C8F4(D_001E3490, 0xB4);
        if (--D_001E4098 == 0)
            sprintf(D_001E3508, D_001E00DC);
    }
}
