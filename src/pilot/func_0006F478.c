/* Driver 2: SetXAVolume (xaplay.c) */
void SsSetSerialVol(short s_num, short voll, short volr);
void func_0006F478(int volume){ volume = 127 - volume / -78; if (volume == -1) volume = 0; SsSetSerialVol(0, volume, volume); }
