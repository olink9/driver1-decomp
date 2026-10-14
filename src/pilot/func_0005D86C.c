struct E { char a[4]; unsigned char b; char c[3]; };
extern struct E D_000BF2EC[];
int func_0005D86C(int a)
{
    int r;
    if (a != 255)
        r = D_000BF2EC[a].b;
    else
        r = 0;
    return r;
}
