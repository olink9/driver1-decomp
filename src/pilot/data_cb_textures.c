/* Driver 2: data_cb_textures */
typedef struct { unsigned char type; char pad[15]; } SP;
extern int D_00097200, D_00097204, D_00097208, D_00097210, D_00097220, D_00097224, D_00097244, D_000972D4;
extern volatile int D_000972B0, D_000969EC, D_000969F0;
extern SP D_000A7BA4[];
extern void SendTPage();
extern void UpdateSpool();
extern void func_0007A53C();
extern void data_cb_regions();
extern void func_00034EE4();
extern void data_cb_misc();
extern void data_cb_textures();

void data_cb_textures(void)
{
    SP *s;
    if (D_00097200 == 0)
        return;
    D_000969F0;
    D_00097208 = D_00097210;
    D_00097200 = 0;
    s = D_000A7BA4;
    SendTPage();
    if (D_00097208 != 0)
        D_00097244++;
    D_00097210++;
    if (D_00097210 != 5)
        return;
    D_00097210 = 0;
    D_000969F0++;
    if (D_00097220 != 0)
        return;
    if (D_00097204 != 0) {
        D_00097204 = 0;
        D_00097224 = 0;
        switch (s[D_000969F0].type) {
        case 0: func_0007A53C(data_cb_regions); break;
        case 1: func_0007A53C(data_cb_textures); break;
        case 3: func_0007A53C(data_cb_misc); break;
        case 2: func_0007A53C(func_00034EE4); break;
        }
    } else {
        func_0007A53C(0);
        if (D_000969F0 == D_000972B0) {
            D_000972D4 = D_000969EC = D_000969F0 = D_000972B0 = 0;
        } else
            UpdateSpool();
    }
}
