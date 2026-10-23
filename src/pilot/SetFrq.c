void SpuSetVoicePitch(int voice, unsigned short pitch);

void SetFrq(int voice, int pitch)
{
    SpuSetVoicePitch(voice, pitch);
}
