/* byte checksum */
int func_000EC4DC(unsigned char *p, unsigned int n)
{
    int sum;
    unsigned int i;

    sum = 0;
    for (i = 0; i < n; i++)
        sum += *p++;
    return sum;
}
