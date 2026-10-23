extern char D_001C0A5C[], D_001C0A60[], D_001C0A68[], D_001C0A70[], D_001C0A78[];
extern char D_001D3240[], D_001D3260[], D_001D3230[], D_001D3250[], D_001D3270[];
extern void GetTextureDetails();

void func_001CBA18(void)
{
    GetTextureDetails(D_001C0A5C, D_001D3240);
    GetTextureDetails(D_001C0A60, D_001D3260);
    GetTextureDetails(D_001C0A68, D_001D3230);
    GetTextureDetails(D_001C0A70, D_001D3250);
    GetTextureDetails(D_001C0A78, D_001D3270);
}
