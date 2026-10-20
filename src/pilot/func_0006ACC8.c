/* time-string formatter: "mm:ss" (+ ".hh") from 1/16384 ticks, 0x7FFFFFFF = no time */
extern char D_00096EDC[], D_00096EE4[], D_00096EE8[], D_000110C8[];
extern int sprintf();

unsigned char func_0006ACC8(char *buf, int ticks, unsigned char flag)
{
    unsigned char hs = 0;
    unsigned char len;
    int q;

    if (ticks == 0x7FFFFFFF) {
        len = sprintf(buf, D_00096EDC);
        if (flag) {
            sprintf(buf + len, D_00096EE4);
        }
    } else {
        hs = ((ticks % 16384) * 100) / 16384;
        q = ticks / 16384;
        len = sprintf(buf, D_000110C8, (unsigned char)((short)q / 60), (short)((short)q % 60));
        if (flag) {
            sprintf(buf + len, D_00096EE8, hs);
        }
    }
    return hs;
}
