void ClearMem(unsigned char *p, int n)
{
    unsigned char *end = p + n;

    while (((int)p & 3) && p < end)
        *p++ = 0;
    while (p <= (unsigned char *)((int *)end - 1)) {
        *(int *)p = 0;
        p += 4;
    }
    while (p < end)
        *p++ = 0;
}
