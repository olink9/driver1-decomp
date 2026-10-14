/* Driver 2: GetLong (xmplay.c) */
unsigned long func_0006FD8C(unsigned char *mpp)
{
    unsigned char a, b, c, d;

    a = *mpp;
    b = mpp[1];
    c = mpp[2];
    d = mpp[3];
    return a + (b << 8) + (c << 16) + (d << 24);
}
