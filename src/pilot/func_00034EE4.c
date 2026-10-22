/* Driver 2: data_cb_soundbank (weak match) */
typedef struct { unsigned char type; char pad[15]; } SQ;
extern SQ D_000A7BA4[];
extern int D_00097200, D_00097204, D_00097224, D_00097244, D_000972D4;
extern volatile int D_000972B0, D_000969EC;
extern volatile int D_000969F0;
extern void SendSBK();
extern void func_0007A53C();
extern void UpdateSpool();
extern void data_cb_regions();
extern void data_cb_textures();
extern void data_cb_misc();
extern void func_00034EE4();

void func_00034EE4(void)
{
    SQ *q;
    if (D_00097200) {
        q = D_000A7BA4;
        D_00097200 = 0;
        D_000969F0;
        SendSBK();
        D_00097244++;
        if (D_00097224) {
            D_000969F0++;
            if (D_00097204) {
                D_00097204 = 0;
                D_00097224 = 0;
                switch (q[D_000969F0].type) {
                case 0:
                    func_0007A53C(data_cb_regions);
                    break;
                case 1:
                    func_0007A53C(data_cb_textures);
                    break;
                case 3:
                    func_0007A53C(data_cb_misc);
                    break;
                case 2:
                    func_0007A53C(func_00034EE4);
                    break;
                }
            } else {
                func_0007A53C(0);
                if (D_000969F0 == D_000972B0) {
                    D_000972B0 = 0;
                    D_000969F0 = D_000972B0;
                    D_000969EC = D_000969F0;
                    D_000972D4 = D_000969EC;
                } else {
                    UpdateSpool();
                }
            }
        }
    }
}
