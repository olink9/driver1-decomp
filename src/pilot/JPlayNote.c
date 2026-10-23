/* Driver 2: JPlayNote (XMPLAY.C) */
typedef struct {
    unsigned char pad0[0x18];
    unsigned char nothing;      /* 0x18 */
    unsigned char pad1[0x34 - 0x19];
    unsigned char ins;          /* 0x34 */
    unsigned char vol;          /* 0x35 */
    unsigned char dat;          /* 0x36 */
    unsigned char pad2;
    unsigned char eff;          /* 0x38 */
} XMCHANNEL;

extern XMCHANNEL *XMC;
#define XMC XMC

extern void SetNote(unsigned char note);
extern void SetInstr(unsigned char ins);
extern void SetPer(void);

int JPlayNote(unsigned char *j, int pmsk)
{
    unsigned char b;
    int ret = 1;
    unsigned char note = 0;

    b = *j;

    XMC->eff = 0;
    XMC->vol = 0;
    XMC->dat = 0;

    if (b == 0x80) {
        XMC->nothing = 1;
        return ret;
    }

    j++;

    if ((b & 0x80) != 0) {
        if ((b & 1) != 0) {
            if (pmsk != 0) {
                XMC->nothing = 0;
                note = *j;
                SetNote(note - 1);
            }
            j++;
            ret++;
        }

        if ((b & 2) != 0) {
            if (pmsk != 0) {
                XMC->ins = *j;
                SetInstr(XMC->ins - 1);
            }
            j++;
            ret++;
        }
        if (((b & 3) != 0) && (note != 97) && (pmsk != 0))
            SetPer();

        if ((b & 4) != 0) {
            XMC->nothing = 0;
            XMC->vol = *j;
            j++;
            ret++;
        }

        if ((b & 8) != 0) {
            XMC->nothing = 0;
            XMC->eff = *j;
            j++;
            ret++;
        }
        if ((b & 16) != 0) {
            XMC->dat = *j;
            ret++;
        }
    } else {
        XMC->nothing = 0;
        note = b;

        if (pmsk != 0) {
            SetNote(note - 1);
            XMC->ins = *j;
            SetInstr(XMC->ins - 1);
            if (note != 97)
                SetPer();
            XMC->vol = *(j + 1);
            XMC->eff = *(j + 2);
            XMC->dat = *(j + 3);
        }
        ret = 5;
    }
    return ret;
}
