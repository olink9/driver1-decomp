/* Driver 2: data_cb_misc */
typedef struct { unsigned char type; char pad[11]; void (*func)(); } SQ;
extern volatile int spoolpos_writing, D_000969EC, spoolcounter, D_000972D4;
extern int D_00097200, D_00097204, D_00097224;
extern SQ D_000A7BA4[];
extern void func_0007A53C();
extern void UpdateSpool();
extern void data_cb_regions(), data_cb_textures(), func_00034EE4(), data_cb_misc();

void data_cb_misc(void)
{
    void (*f)();
    if (D_00097200 == 0) return;
    f = D_000A7BA4[spoolpos_writing].func;
    D_00097200 = 0;
    if (f != 0) (*f)();
    spoolpos_writing++;
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
            spoolcounter = 0;
            spoolpos_writing = spoolcounter;
            D_000969EC = spoolpos_writing;
            D_000972D4 = D_000969EC;
        } else UpdateSpool();
    }
}
