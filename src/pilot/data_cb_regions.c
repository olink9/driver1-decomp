/* data_cb_regions */
typedef struct { unsigned char type; char pad[11]; void (*func)(void); } SQ;
extern int D_00097200, D_00097224, D_00097204;
extern volatile int spoolpos_writing, spoolcounter, D_000969EC, D_000972D4;
extern SQ D_000A7BA4[];
extern void func_0007A53C(), UpdateSpool(), data_cb_textures(), func_00034EE4(), data_cb_misc();
extern void data_cb_regions();

void data_cb_regions(void)
{
    SQ *cur;
    if (D_00097200 == 0) return;
    cur = &D_000A7BA4[spoolpos_writing];
    D_00097200 = 0;
    if (cur->func != 0) cur->func();
    spoolpos_writing++;
    if (D_00097224 == 0) return;
    if (D_00097204 != 0) {
        D_00097204 = 0;
        D_00097224 = 0;
        switch (D_000A7BA4[spoolpos_writing].type) {
        case 0: func_0007A53C(data_cb_regions); break;
        case 1: func_0007A53C(data_cb_textures); break;
        case 3: func_0007A53C(data_cb_misc); break;
        case 2: func_0007A53C(func_00034EE4); break;
        }
    } else {
        func_0007A53C(0);
        if (spoolpos_writing == spoolcounter) {
            D_000972D4 = D_000969EC = spoolpos_writing = spoolcounter = 0;
        } else {
            UpdateSpool();
        }
    }
}
