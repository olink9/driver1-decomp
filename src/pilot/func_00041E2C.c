/* Driver 2: InitDebrisNames (debris.c) - Driver 1 has a different texture list */
typedef struct {
    unsigned char u0, v0, u1, v1, u2, v2, u3, v3;
    unsigned short tpageid;
    unsigned short clutid;
    char pad[4];
} TEXTURE_DETAILS;

extern long long D_00096834;    /* absolute (8-byte) globals - see pilot/AGENT.md */
extern long long D_00096854;
extern long long D_00096CD0;
extern TEXTURE_DETAILS D_000B736C;
extern TEXTURE_DETAILS D_000B737C;
extern TEXTURE_DETAILS D_000B7EA8;
extern TEXTURE_DETAILS D_000B7EB8;
extern TEXTURE_DETAILS D_000B7F2C;
extern TEXTURE_DETAILS D_000B81B8;
extern TEXTURE_DETAILS D_000B8FD8;
extern TEXTURE_DETAILS D_000B9028;
extern TEXTURE_DETAILS D_000B9038;
extern TEXTURE_DETAILS D_000B90C0;
extern TEXTURE_DETAILS D_000B90D0;
extern TEXTURE_DETAILS D_000B90E0;
extern TEXTURE_DETAILS D_000B90F0;
extern TEXTURE_DETAILS D_000B9110;
extern TEXTURE_DETAILS D_000B9120;
extern TEXTURE_DETAILS D_000B9130;
extern TEXTURE_DETAILS D_000B9140;
extern TEXTURE_DETAILS D_000B9150;
extern TEXTURE_DETAILS D_000B9160;
extern TEXTURE_DETAILS D_000B9170;
extern TEXTURE_DETAILS D_000B9180;
extern TEXTURE_DETAILS D_000B9190;
extern TEXTURE_DETAILS D_000B91A0;
extern TEXTURE_DETAILS D_000B91B0;
extern TEXTURE_DETAILS D_000B91D0;
extern TEXTURE_DETAILS D_000B91E0;
extern TEXTURE_DETAILS D_000B91F0;
extern TEXTURE_DETAILS D_000B9200;
extern TEXTURE_DETAILS D_000B9210;
extern TEXTURE_DETAILS D_000B9220;
extern TEXTURE_DETAILS D_000B9230;
extern TEXTURE_DETAILS D_000B9240;
extern TEXTURE_DETAILS D_000B9250;
extern TEXTURE_DETAILS D_000B9260;
extern TEXTURE_DETAILS D_000B9270;
extern TEXTURE_DETAILS D_000B9280;
extern TEXTURE_DETAILS D_000B9290;
extern TEXTURE_DETAILS D_000BABA0;
extern TEXTURE_DETAILS D_000BABB0;
extern TEXTURE_DETAILS D_000BB520;
extern TEXTURE_DETAILS D_000BBA50;
extern TEXTURE_DETAILS D_000BBA60;
extern TEXTURE_DETAILS D_000BBA70;
extern TEXTURE_DETAILS D_000BBC20;
extern TEXTURE_DETAILS D_000BBDB0;
extern TEXTURE_DETAILS D_000BBDC0;
extern TEXTURE_DETAILS D_000BBE24;
extern TEXTURE_DETAILS D_000BBE34;
extern TEXTURE_DETAILS D_000BBE44;

void GetTextureDetails(char *name, TEXTURE_DETAILS *info);   /* Driver 2: GetTextureDetails */
void func_0005CB68(int clut, int a, int b);                /* Driver 2: SetCLUT16Flags */

void func_00041E2C(void)
{
    GetTextureDetails("SMOKE", &D_000B91B0);
    GetTextureDetails("DEBRIS", &D_000B9260);
    if (*(int *)&D_00096854 == 0)
        GetTextureDetails("LITTER", &D_000B90D0);
    GetTextureDetails("SUN2", &D_000B9210);
    GetTextureDetails("SUN2", &D_000B9170);
    GetTextureDetails("SKID", &D_000BBE44);
    GetTextureDetails("FLARE1", &D_000B9130);
    GetTextureDetails("SPLASH", &D_000B9200);
    GetTextureDetails("SWBIRD1", &D_000BBE24);
    GetTextureDetails("SWBIRD2", &D_000BBE34);
    GetTextureDetails("SUN2", &D_000B9110);
    GetTextureDetails("SKYSUN", &D_000B9240);
    GetTextureDetails("DROP", &D_000B90F0);
    GetTextureDetails("DIGIT", &D_000B9140);
    func_0005CB68(D_000B9140.clutid, 6, 0);
    GetTextureDetails("COLLON", &D_000B9280);
    GetTextureDetails("JEANS1", &D_000B9150);
    GetTextureDetails("ARM1", &D_000B9250);
    GetTextureDetails("FOREARM1", &D_000B91A0);
    GetTextureDetails("CHEST1", &D_000B9220);
    GetTextureDetails("LOWHEAD", &D_000B9120);
    *(int *)&D_00096CD0 = 1;
    GetTextureDetails("ADDCAM", &D_000BBA70);
    GetTextureDetails("FRAMEADV", &D_000B81B8);
    GetTextureDetails("AUTO", &D_000B9038);
    GetTextureDetails("CHASEC", &D_000BBC20);
    GetTextureDetails("CHOOSECA", &D_000B7F2C);
    GetTextureDetails("CLOCK", &D_000B9028);
    GetTextureDetails("DELCAM", &D_000BABB0);
    GetTextureDetails("EDITCAM", &D_000B90C0);
    GetTextureDetails("FIXEDCA", &D_000BB520);
    GetTextureDetails("INCAR", &D_000B8FD8);
    GetTextureDetails("LENSCHA", &D_000BABA0);
    GetTextureDetails("LOOKCAR", &D_000BBDC0);
    GetTextureDetails("MOVECMP", &D_000BBDB0);
    GetTextureDetails("MOVECAM", &D_000B7EB8);
    GetTextureDetails("OK", &D_000B736C);
    GetTextureDetails("PAUSE", &D_000B9290);
    GetTextureDetails("PLAYCAM", &D_000B737C);
    GetTextureDetails("PLAYPAU", &D_000BBA60);
    GetTextureDetails("SAVE2CA", &D_000BBA50);
    GetTextureDetails("RESTREP", &D_000B7EA8);
    GetTextureDetails("SEA", &D_000B9180);
    *(int *)&D_00096CD0 = 0;
    D_000B9120.u1 = D_000B9120.u3 = D_000B9120.u0 + 8;
    if (*(int *)&D_00096834 == 2 || *(int *)&D_00096834 == 6) {
        GetTextureDetails("STOPWTCH", &D_000B9160);
        GetTextureDetails("TRAIL", &D_000B9190);
        GetTextureDetails("ELPAPEL", &D_000B91F0);
        GetTextureDetails("JOYPAD", &D_000B9270);
        GetTextureDetails("CROSS", &D_000B91D0);
        GetTextureDetails("LINE", &D_000B90E0);
    }
    GetTextureDetails("LIGHTREF", &D_000B9230);
    GetTextureDetails("SUN2", &D_000B91E0);
}
