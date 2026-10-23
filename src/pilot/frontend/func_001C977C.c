extern int D_001CEF64, D_001CEF68;
extern char D_001D30F0[], D_001D3100[];
extern void func_001CA408();

void func_001C977C(void)
{
    D_001CEF64 += D_001CEF68;
    if (D_001CEF64 > 255) {
        D_001CEF64 = 255;
        D_001CEF68 = -D_001CEF68;
    }
    if (D_001CEF64 < 0) {
        D_001CEF64 = 0;
        D_001CEF68 = -D_001CEF68;
    }
    func_001CA408(30, 194, D_001D30F0, (unsigned char)D_001CEF64, (unsigned char)D_001CEF64, (unsigned char)D_001CEF64, 0, 1);
    func_001CA408(452, 194, D_001D3100, (unsigned char)D_001CEF64, (unsigned char)D_001CEF64, (unsigned char)D_001CEF64, 0, 1);
}
