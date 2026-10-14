/* Absolute-addressed globals (lui + %lo, never $gp) are declared as 8-byte
 * objects so cc1 emits the same macro loads/stores; see pilot/AGENT.md. */
extern long long D_000967A8;     /* NumRoads */
extern long long D_00097534;
extern long long D_00097044;

void func_0004FB70(char *dest, char *source);   /* Driver 2: Getlong */
int printf(const char *fmt, ...);

void func_000394E0(char *p)
{
    int magic;

    func_0004FB70((char *)&D_000967A8, p);
    if (*(int *)&D_000967A8 > 422) {
        printf("MAX_ROADS = %d NumRoads = %d, guess what you gotta do!\n", 422, *(int *)&D_000967A8);
        magic = *(int *)&D_00097534;
        while (magic != 0x78654321)
            __asm__ volatile("break 1024");   /* aspsx encodes break n as n << 6 */
    }
    p += 4;
    *(char **)&D_00097044 = p;
}
