/* Driver 2: DoVolSlide */
typedef struct { char pad[0x10]; unsigned short vbtick; } MS;
typedef struct { char pad[0x60]; unsigned char tmpvolume; } XM;
extern MS *D_00097814;
extern XM *D_0009781C;

void func_000713DC(unsigned char dat) {
    if (!D_00097814->vbtick) return;
    D_0009781C->tmpvolume += dat >> 4;
    if (D_0009781C->tmpvolume > 128) D_0009781C->tmpvolume = 128;
    D_0009781C->tmpvolume -= dat & 0xf;
    if (D_0009781C->tmpvolume < 64) D_0009781C->tmpvolume = 64;
}
