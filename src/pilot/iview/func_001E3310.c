struct SQ;
extern int D_00096874;
extern char PlaySpeech(struct SQ *q, int sound);

void func_001E3310(struct SQ *q, int sound)
{
    if (D_00096874 == 0)
        PlaySpeech(q, sound);
}
