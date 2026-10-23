/* Driver 2: Getlong (convert.c) */
void Getlong(char *dest, char *source)
{
    *dest = *source;
    dest[1] = source[1];
    dest[2] = source[2];
    dest[3] = source[3];
}
