extern int D_001D320C, D_001CEF88, D_001CEF84, D_001D3200, D_001D3204, D_001D31B8;
extern int D_001D3190[];
extern void func_001C9CF0();

void func_001C9928(int a0)
{
    D_001CEF88 = 0;
    if (D_001D320C == 0) {
        if (a0 != 0) {
            D_001CEF84++;
            if (D_001CEF84 >= 5) D_001CEF84 = 0;
            D_001D31B8 = D_001D3190[D_001D3200];
        } else {
            D_001D31B8 = D_001D3190[D_001D3204];
        }
    } else if (D_001D320C == 1) {
        if (a0 == 0) {
            D_001CEF84--;
            if (D_001CEF84 < 0) D_001CEF84 = 4;
            D_001D31B8 = D_001D3190[D_001D3200];
        } else {
            D_001D31B8 = D_001D3190[D_001D3204];
        }
    }
    D_001D3204 = D_001CEF84;
    D_001D3200 = D_001CEF84;
    func_001C9CF0();
}
