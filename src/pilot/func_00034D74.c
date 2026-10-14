/* data_cb_regions */
typedef struct { unsigned char type; char pad[11]; void (*func)(void); } SQ;
extern int D_00097200, D_00097224, D_00097204;
extern volatile int D_000969F0, D_000972B0, D_000969EC, D_000972D4;
extern SQ D_000A7BA4[];
extern void func_0007A53C(), func_00033604(), func_0003497C(), func_00034EE4(), func_00035174();
extern void func_00034D74();

void func_00034D74(void)
{
    SQ *cur;
    if (D_00097200 == 0) return;
    cur = &D_000A7BA4[D_000969F0];
    D_00097200 = 0;
    if (cur->func != 0) cur->func();
    D_000969F0++;
    if (D_00097224 == 0) return;
    if (D_00097204 != 0) {
        D_00097204 = 0;
        D_00097224 = 0;
        switch (D_000A7BA4[D_000969F0].type) {
        case 0: func_0007A53C(func_00034D74); break;
        case 1: func_0007A53C(func_0003497C); break;
        case 3: func_0007A53C(func_00035174); break;
        case 2: func_0007A53C(func_00034EE4); break;
        }
    } else {
        func_0007A53C(0);
        if (D_000969F0 == D_000972B0) {
            D_000972D4 = D_000969EC = D_000969F0 = D_000972B0 = 0;
        } else {
            func_00033604();
        }
    }
}
