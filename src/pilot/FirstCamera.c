struct S { char pad[0x22]; unsigned char prev; };
extern struct S *LastChange;
int FirstCamera(void)
{
    if (!LastChange || LastChange->prev == 255)
        return 1;
    return 0;
}
