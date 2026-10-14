/* Driver 2: XM_PlayStop */
typedef struct { char pad[0x34]; unsigned char play; } XMS;
extern short D_0008D218[];
extern XMS *D_000C7888[];
extern XMS *D_00097810;
extern void func_00073324(int);
void func_0006FAAC(int id)
{
    if (D_0008D218[id] == -1)
        return;
    D_00097810 = D_000C7888[id];
    if (!D_00097810->play)
        return;
    D_00097810->play = 0;
    func_00073324(id);
}
