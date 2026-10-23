typedef struct { int pos[3]; int dir[3]; int angle; } CamPt;
typedef struct { unsigned char from, to; } TRANS;
extern TRANS D_001CEF70[];
extern int *D_001D3160[];
extern CamPt *D_001D3190[];
extern CamPt *D_001D31B8;
extern int D_001D3200, D_001D3204, D_001D3208, D_001D320C, D_001D321C, D_001CEF88;

int func_001C9A18(int to)
{
    int found = 0;
    int i;

    D_001D3204 = to;
    for (i = 0; i < 10; i++) {
        if (D_001D3200 == D_001CEF70[i].from && D_001D3204 == D_001CEF70[i].to) {
            D_001D320C = 1;
            D_001D321C = D_001D3160[i][10];
            D_001D31B8 = D_001D3190[i];
            D_001D3200 = D_001CEF70[i].from;
            D_001D3204 = D_001CEF70[i].to;
            found = 1;
            break;
        }
        if (D_001D3200 == D_001CEF70[i].to && D_001D3204 == D_001CEF70[i].from) {
            D_001D320C = 0;
            D_001D321C = D_001D3160[i][10];
            D_001D31B8 = &D_001D3190[i][D_001D321C - 1];
            D_001D3200 = D_001CEF70[i].to;
            D_001D3204 = D_001CEF70[i].from;
            found = 1;
            break;
        }
    }
    if (!found) {
        D_001D320C = 0;
        D_001D321C = D_001D3160[D_001D3204][10];
        D_001D31B8 = &D_001D3190[D_001D3204][D_001D321C - 1];
        D_001D3200 = D_001CEF70[D_001D3204].to;
        D_001D3204 = D_001CEF70[D_001D3204].from;
    } else {
        D_001CEF88 = 1;
        D_001D3208 = 0;
    }
    return D_001D321C;
}
