#include "../PsyX_main.h"

#include "psx/libspu.h"
#include "psx/libetc.h"
#include "psx/libmath.h"
#include "PsyX_SPUAL.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

#ifndef __EMSCRIPTEN__
#include <AL/efx.h>
#endif

// TODO: XA via port/pc/pc_xa.c (PsyX_SPUAL_XAQueue); ADSR: Driver 1 port, see "ADSR envelope"

static const char* getALCErrorString(int err)
{
	switch (err)
	{
	case ALC_NO_ERROR:
		return "AL_NO_ERROR";
	case ALC_INVALID_DEVICE:
		return "ALC_INVALID_DEVICE";
	case ALC_INVALID_CONTEXT:
		return "ALC_INVALID_CONTEXT";
	case ALC_INVALID_ENUM:
		return "ALC_INVALID_ENUM";
	case ALC_INVALID_VALUE:
		return "ALC_INVALID_VALUE";
	case ALC_OUT_OF_MEMORY:
		return "ALC_OUT_OF_MEMORY";
	default:
		return "AL_UNKNOWN";
	}
}

static const char* getALErrorString(int err)
{
	switch (err)
	{
	case AL_NO_ERROR:
		return "AL_NO_ERROR";
	case AL_INVALID_NAME:
		return "AL_INVALID_NAME";
	case AL_INVALID_ENUM:
		return "AL_INVALID_ENUM";
	case AL_INVALID_VALUE:
		return "AL_INVALID_VALUE";
	case AL_INVALID_OPERATION:
		return "AL_INVALID_OPERATION";
	case AL_OUT_OF_MEMORY:
		return "AL_OUT_OF_MEMORY";
	default:
		return "AL_UNKNOWN";
	}
}

#define SPU_REALMEMSIZE			(512 * 1024)
#define SPU_MEMSIZE				(2048*1024)		// SPU_REALMEMSIZE

typedef struct
{
	u_char samplemem[SPU_MEMSIZE];
	u_char* writeptr;
} SPUMemory;

static SPUMemory s_SpuMemory;

// Driver 1 port: ADSR envelope (defined below)
static Uint64	s_spuClockBase = 0;
static SDL_Thread* s_envThread = NULL;
static volatile int s_envThreadStop = 0;
static int EnvThreadMain(void* data);
static SDL_mutex* g_SpuMutex = NULL;
static int g_spuInit = 0;
static int s_spuMallocVal = 0;

typedef struct
{
	SpuVoiceAttr attr;	// .voice is Id of this channel

	ALuint alBuffer;
	ALuint alSource;
	ushort sampledirty;
	ushort reverb;

	// Driver 1 port: the voice's ADSR envelope (see "ADSR envelope" below)
	ushort adsr1, adsr2;	// the SPU registers 1F801C08h/0Ah, as libspu's SpuSetVoiceAttr builds them
	int keyed;				// libspu's software key-on bit (SpuSetKey)
	int envPhase;			// ENV_OFF .. ENV_RELEASE
	int envLevel;			// ENVX, 0..7FFFh
	int envWait;			// 44.1 kHz ticks until the next envelope step
	double envTime;			// SPU clock (44.1 kHz ticks) the envelope has been advanced to
	int envBaked;			// attack/decay are baked into the PCM head (fast envelopes)
	int envScale;			// the level the baked head ends on: the AL gain is volGain * envLevel / envScale after it
	float volGain;			// from the volume registers
} SPUALVoice;

const int s_spuVoiceCount = 24;

SPUALVoice	g_SpuVoices[s_spuVoiceCount];
ALCdevice*	g_ALCdevice = NULL;
ALCcontext* g_ALCcontext = NULL;
int			g_SPUMuted = 0;
ALuint		g_ALEffectSlots[2];
int			g_currEffectSlotIdx = 0;
ALuint		g_nAlReverbEffect = 0;
int			g_enableSPUReverb = 0;
int			g_ALEffectsSupported = 0;

#ifndef __EMSCRIPTEN__

LPALGENEFFECTS alGenEffects = NULL;
LPALDELETEEFFECTS alDeleteEffects = NULL;
LPALEFFECTI alEffecti = NULL;
LPALEFFECTF alEffectf = NULL;
LPALGENAUXILIARYEFFECTSLOTS alGenAuxiliaryEffectSlots = NULL;
LPALDELETEAUXILIARYEFFECTSLOTS alDeleteAuxiliaryEffectSlots = NULL;
LPALAUXILIARYEFFECTSLOTI alAuxiliaryEffectSloti = NULL;
LPALAUXILIARYEFFECTSLOTF alAuxiliaryEffectSlotf = NULL;

#endif // __EMSCRIPTEN__

static void InitOpenAlEffects()
{
	g_ALEffectsSupported = 0;
#ifndef __EMSCRIPTEN__
	if (!alcIsExtensionPresent(g_ALCdevice, ALC_EXT_EFX_NAME))
	{
		eprintf("PSX SPU effects are NOT supported!\n");
		return;
	}

	alGenEffects = (LPALGENEFFECTS)alGetProcAddress("alGenEffects");
	alDeleteEffects = (LPALDELETEEFFECTS)alGetProcAddress("alDeleteEffects");
	alEffecti = (LPALEFFECTI)alGetProcAddress("alEffecti");
	alEffectf = (LPALEFFECTF)alGetProcAddress("alEffectf");
	alGenAuxiliaryEffectSlots = (LPALGENAUXILIARYEFFECTSLOTS)alGetProcAddress("alGenAuxiliaryEffectSlots");
	alDeleteAuxiliaryEffectSlots = (LPALDELETEAUXILIARYEFFECTSLOTS)alGetProcAddress("alDeleteAuxiliaryEffectSlots");
	alAuxiliaryEffectSloti = (LPALAUXILIARYEFFECTSLOTI)alGetProcAddress("alAuxiliaryEffectSloti");
	alAuxiliaryEffectSlotf = (LPALAUXILIARYEFFECTSLOTF)alGetProcAddress("alAuxiliaryEffectSlotf");

	int max_sends = 0;
	alcGetIntegerv(g_ALCdevice, ALC_MAX_AUXILIARY_SENDS, 1, &max_sends);

	// make reverb effect slot
	g_currEffectSlotIdx = 0;
	alGenAuxiliaryEffectSlots(1, g_ALEffectSlots);

	// make reverb effect
	alGenEffects(1, &g_nAlReverbEffect);
	alEffecti(g_nAlReverbEffect, AL_EFFECT_TYPE, AL_EFFECT_REVERB);

	// setup defaults of effect
	alEffectf(g_nAlReverbEffect, AL_REVERB_GAIN, 0.45f);
	alEffectf(g_nAlReverbEffect, AL_REVERB_GAINHF, 0.25f);
	alEffectf(g_nAlReverbEffect, AL_REVERB_DECAY_TIME, 2.0f);
	alEffectf(g_nAlReverbEffect, AL_REVERB_DECAY_HFRATIO, 0.9f);
	alEffectf(g_nAlReverbEffect, AL_REVERB_REFLECTIONS_DELAY, 0.08f);
	alEffectf(g_nAlReverbEffect, AL_REVERB_REFLECTIONS_GAIN, 0.2f);
	alEffectf(g_nAlReverbEffect, AL_REVERB_DIFFUSION, 0.9f);
	alEffectf(g_nAlReverbEffect, AL_REVERB_DENSITY, 0.1f);
	alEffectf(g_nAlReverbEffect, AL_REVERB_AIR_ABSORPTION_GAINHF, 0.1f);

	g_ALEffectsSupported = 1;

	eprintf("PSX SPU effects are supported and initialized\n");

	alAuxiliaryEffectSloti(g_ALEffectSlots[g_currEffectSlotIdx], AL_EFFECTSLOT_EFFECT, g_nAlReverbEffect);
#endif // __EMSCRIPTEN__
}

int PsyX_SPUAL_InitSound()
{
	if (!g_SpuMutex)
		g_SpuMutex = SDL_CreateMutex();

	if (!g_spuInit)
		memset(&s_SpuMemory, 0, sizeof(s_SpuMemory));

	g_spuInit = 1;

	int numDevices, alErr, i;
	const char* devices;
	const char* devStrptr;

	// out_channel_formats snd_outputchannels
	static int al_context_params[] =
	{
		ALC_FREQUENCY, 44100,
#ifndef __EMSCRIPTEN__
		ALC_MAX_AUXILIARY_SENDS, 2,
#endif
		0
	};

	if (g_ALCdevice)
		return 1;

	numDevices = 0;

	// Init openAL
	// check devices list

	devStrptr = alcGetString(NULL, ALC_DEVICE_SPECIFIER);
	devices = devStrptr;

	// go through device list (each device terminated with a single NULL, list terminated with double NULL)
	while ((*devStrptr) != '\0')
	{
		eprintinfo("found sound device: %s\n", devStrptr);
		devStrptr += strlen(devStrptr) + 1;
		numDevices++;
	}

	if(numDevices == 0)
		return 0;
	
	g_ALCdevice = alcOpenDevice(NULL);

	alErr = AL_NO_ERROR;

	if (!g_ALCdevice)
	{
		alErr = alcGetError(NULL);
		eprinterr("alcOpenDevice: NULL DEVICE error: %s\n", getALCErrorString(alErr));
		return 0;
	}

#ifndef __EMSCRIPTEN__
	g_ALCcontext = alcCreateContext(g_ALCdevice, al_context_params);
#else
	g_ALCcontext = alcCreateContext(g_ALCdevice, NULL);
#endif

	alErr = alcGetError(g_ALCdevice);
	if (alErr != AL_NO_ERROR)
	{
		eprinterr("alcCreateContext error: %s\n", getALCErrorString(alErr));
		return 0;
	}

	alcMakeContextCurrent(g_ALCcontext);

	alErr = alcGetError(g_ALCdevice);
	if (alErr != AL_NO_ERROR)
	{
		eprinterr("alcMakeContextCurrent error: %s\n", getALCErrorString(alErr));
		return 0;
	}

	// Setup defaults
	alListenerf(AL_GAIN, 1.0f);
	alDistanceModel(AL_NONE);

	// create channels
	for (i = 0; i < s_spuVoiceCount; i++)
	{
		SPUALVoice* voice = &g_SpuVoices[i];
		memset(voice, 0, sizeof(SPUALVoice));

		alGenSources(1, &voice->alSource);
		alGenBuffers(1, &voice->alBuffer);
#ifdef AL_SOFT_source_resampler
		alSourcei(voice->alSource, AL_SOURCE_RESAMPLER_SOFT, 2);	// Use cubic resampler
#endif
		alSourcei(voice->alSource, AL_SOURCE_RELATIVE, AL_TRUE);
		alSourcef(voice->alSource, AL_MAX_GAIN, 64.0f);	// Driver 1 port: envelope gain past a baked head
		alSourcef(voice->alSource, AL_GAIN, 0.0f);
	}


	InitOpenAlEffects();

	// Driver 1 port: the ADSR envelope clock and thread
	if (!s_spuClockBase)
		s_spuClockBase = SDL_GetPerformanceCounter();

	if (!s_envThread)
	{
		s_envThreadStop = 0;
		s_envThread = SDL_CreateThread(EnvThreadMain, "SPU envelope", NULL);
	}

	return 1;
}

// Driver 1 port: the XA stream source (PsyX_SPUAL_XAQueue, end of file)
#define XA_NUM_BUFFERS	64		// ~3.4 s of 37.8 kHz stereo sectors
static ALuint	s_xaSource = AL_NONE;
static ALuint	s_xaBuffers[XA_NUM_BUFFERS];

void PsyX_SPUAL_ShutdownSound()
{
	g_spuInit = 0;

	if (s_envThread)		// Driver 1 port: the envelope thread
	{
		s_envThreadStop = 1;
		SDL_WaitThread(s_envThread, NULL);
		s_envThread = NULL;
	}

#ifndef __EMSCRIPTEN__
	if (!g_ALCcontext)
		return;

	for (int i = 0; i < s_spuVoiceCount; i++)
	{
		SPUALVoice* voice = &g_SpuVoices[i];
		alDeleteSources(1, &voice->alSource);
		alDeleteBuffers(1, &voice->alBuffer);
	}

	if (s_xaSource != AL_NONE)		// Driver 1 port: the XA stream
	{
		alSourceStop(s_xaSource);
		alDeleteSources(1, &s_xaSource);
		alDeleteBuffers(XA_NUM_BUFFERS, s_xaBuffers);
		s_xaSource = AL_NONE;
	}

	if (g_ALEffectsSupported)
	{
		alDeleteEffects(1, &g_nAlReverbEffect);
		g_ALEffectsSupported = AL_NONE;
		alDeleteAuxiliaryEffectSlots(1, g_ALEffectSlots);
	}

	alcDestroyContext(g_ALCcontext);
	alcCloseDevice(g_ALCdevice);

	g_ALCcontext = NULL;
	g_ALCdevice = NULL;
#endif // __EMSCRIPTEN__
}

//--------------------------------------------------------------------------------

int PsyX_SPUAL_Alloc(int size)
{
	int addr = s_spuMallocVal;
	s_spuMallocVal += size;

	if (s_spuMallocVal > SPU_MEMSIZE)
		return -1;

	return addr;
}

int PsyX_SPUAL_InitAlloc(int num, char* top)
{
	s_spuMallocVal = 0;
	return 0;
}

void PsyX_SPUAL_Free(u_int addr)
{
	s_spuMallocVal = 0;
}

u_int PsyX_SPUAL_SetTransferStartAddr(u_int addr)
{
	s_SpuMemory.writeptr = s_SpuMemory.samplemem + addr;

	if (addr > SPU_MEMSIZE)
		return 0;

	if (addr < 0x1010)
		return 0;

	return 1;
}

u_int PsyX_SPUAL_Write(u_char* addr, u_int size)
{
	//if (0x7EFF0 < size)
	//	size = 0x7EFF0;

	volatile int wptr_ofs = s_SpuMemory.writeptr - s_SpuMemory.samplemem;

	if (wptr_ofs + size > SPU_REALMEMSIZE)
	{
		eprintf("SPU WARNING: SpuWrite exceeded SPU_REALMEMSIZE by %d bytes!\n", wptr_ofs + size - SPU_REALMEMSIZE);
	}
	assert(size > 0 && wptr_ofs + size < SPU_MEMSIZE);

	// simply copy to the writeptr
	memcpy(s_SpuMemory.writeptr, addr, size);

#if 0 // BANK TEST
	{
		static short waveBuffer[SPU_MEMSIZE];

		ALuint alSource;
		ALuint alBuffer;

		alGenSources(1, &alSource);
		alGenBuffers(1, &alBuffer);

		int loopStart = 0, loopLen = 0;
		int count = decodeSound(addr, size, waveBuffer, &loopStart, &loopLen);

		// update AL buffer
		alBufferData(alBuffer, AL_FORMAT_MONO16, waveBuffer, count * sizeof(short), 11000);

		// set the buffer
		alSourcei(alSource, AL_BUFFER, alBuffer);
		alSourcef(alSource, AL_GAIN, 1.0f);// TODO: panning
		alSourcef(alSource, AL_PITCH, 1);

		alSourcePlay(alSource);
		int status;
		do
		{
			alGetSourcei(alSource, AL_SOURCE_STATE, &status);
		} while (status == AL_PLAYING);

		alSourceStop(alSource);

		alDeleteSources(1, &alSource);
		alDeleteBuffers(1, &alBuffer);
	}
#endif

	return size;
}

u_int PsyX_SPUAL_Read(u_char* addr, u_int size)
{
	volatile int rptr_ofs = s_SpuMemory.writeptr - s_SpuMemory.samplemem;

	if (rptr_ofs + size > SPU_REALMEMSIZE)
	{
		eprintf("SPU WARNING: SpuRead exceeded SPU_REALMEMSIZE by %d bytes!\n", rptr_ofs + size - SPU_REALMEMSIZE);
	}
	assert(size > 0 && rptr_ofs + size < SPU_MEMSIZE);

	// simply copy to the writeptr
	memcpy(addr, s_SpuMemory.writeptr, size);

	return size;
}

// PSX ADPCM decoding routine - decodes a single sample.
// Driver 1 port: the SPU's integer formula (psx-spx "SPU ADPCM"): filters 0..4 as n/64, shift 13..15 acts
// as 9, clamped to 16 bits. The float version (pow() per sample) cost milliseconds per key-on.
static const int K0[5] = { 0, 60, 115, 98, 122 };
static const int K1[5] = { 0, 0, -52, -55, -60 };

static short vagToPcm(u_char soundParameter, int soundData, int* vagPrev1, int* vagPrev2)
{
	int shift = soundParameter & 0x0F;
	int filter = (soundParameter >> 4) & 0x0F;
	int s;

	if (shift > 12)
		shift = 9;
	if (filter > 4)
		filter = 4;

	if (soundData > 7)
		soundData -= 16;

	s = (soundData << 12) >> shift;
	s += ((*vagPrev1) * K0[filter] + (*vagPrev2) * K1[filter] + 32) >> 6;

	if (s > 32767)
		s = 32767;
	if (s < -32768)
		s = -32768;

	(*vagPrev2) = (*vagPrev1);
	(*vagPrev1) = s;

	return (short)s;
}

typedef enum 
{
	LoopEnd = 1 << 0,		// Jump to repeat address after this block
							// 1 - Copy repeatAddress to currentAddress AFTER this block
							//     set ENDX (TODO: Immediately or after this block?)
							// 0 - Nothing

	Repeat = 1 << 1,		// Takes an effect only with LoopEnd bit set.
							// 1 - Loop normally
							// 0 - Loop and force Release

	LoopStart = 1 << 2,		// Mark current address as the beginning of repeat
							// 1 - Load currentAddress to repeatAddress
							// 0 - Nothing
} ADPCM_FLAGS;


// Main decoding routine - Takes PSX ADPCM formatted audio data and converts it to PCM. It also extracts the looping information if used.
static int decodeSound(u_char* iData, int soundSize, short* oData, int* loopStart, int* loopLength, int breakOnEnd /*= 0*/)
{
	u_char sp;
	u_char flag;
	int sd = 0;
	int vagPrev1 = 0;
	int vagPrev2 = 0;
	int k = 0;

	int loopStrt = 0, loopEnd = 0;
	int breakOn = -1;

	for (int i = 0; i < soundSize; i++)
	{
		if (i % 16 == 0)
		{
			sp = iData[i];
			flag = iData[i+1];
			i += 2;
		}

		// Driver 1 port: the output buffer is SPU_REALMEMSIZE shorts; a sample without an end flag
		// (or a bad start address) used to decode past it
		if (k + 2 > SPU_REALMEMSIZE)
		{
			eprintf("SPU WARNING: sample has no end flag within the buffer, truncated\n");
			return k;
		}

		sd = (int)iData[i] & 0xF;
		oData[k++] = vagToPcm(sp, sd, &vagPrev1, &vagPrev2);

		sd = ((int)iData[i] >> 4) & 0xF;
		oData[k++] = vagToPcm(sp, sd, &vagPrev1, &vagPrev2);

		if (breakOnEnd && k == breakOn)
			return k;

		if (breakOn == -1)
		{
			// flags parsed
			if (flag & LoopStart)
			{
				// Driver 1 port: the loop starts at the beginning of this block (k counts the 2 samples
				// already decoded from it); was k + 26, one block late
				loopStrt = k - 2;
			}

			if (flag & LoopEnd)
			{
				loopEnd = k + 26;

				if (flag & Repeat)
				{
					*loopStart = loopStrt;
					*loopLength = loopEnd - loopStrt;
				}

				if (breakOnEnd)
					breakOn = k + 26;
			}
		}
	}

	return soundSize;
}

//--------------------------------------------------------------------------------
// Driver 1 port: ADSR envelope (psx-spx "SPU Voice Volume Envelope"; the rate/step rules as DuckStation's SPU).
// Every voice runs the SPU's envelope state machine on a 44.1 kHz clock (real time, like the PsyCross vblank
// clock): key-on starts the attack from 0, key-off starts the release, a voice whose envelope reaches 0 in
// release is stopped. The AL source plays the decoded sample, so the envelope is applied as gain:
//  - attack/decay phases that settle within ENV_BAKE_TICKS (the usual case, and every envelope Driver sets) are
//    multiplied into the PCM at key-on (sample-accurate, mapped through the key-on pitch), so short ramps are
//    exact instead of quantised to the gain updates;
//  - everything slower (and sustain/release) is AL_GAIN, updated by the envelope thread every ENV_TICK_MS.
// libspu's key status (SpuGetKeyStatus/SpuGetAllKeysStatus) is the software key-on bit plus ENVX != 0.

enum { ENV_OFF = 0, ENV_ATTACK, ENV_DECAY, ENV_SUSTAIN, ENV_RELEASE };

#define SPU_CLOCK_RATE		44100
#define ENV_BAKE_TICKS		(SPU_CLOCK_RATE / 50)	// 20 ms
#define ENV_RELEASE_CUT		(SPU_CLOCK_RATE / 500)	// a release this short (2 ms) stops the voice at once
#define ENV_TICK_MS			4

static double SpuClockNow()
{
	Uint64 c = SDL_GetPerformanceCounter() - s_spuClockBase;
	return (double)c * SPU_CLOCK_RATE / (double)SDL_GetPerformanceFrequency();
}

typedef struct
{
	int phase, level, wait;
} EnvState;

// the rate/direction/mode of the current phase
static void EnvPhaseParams(const SPUALVoice* v, int phase, int* rate, int* decr, int* expo)
{
	switch (phase)
	{
	case ENV_ATTACK:
		*rate = (v->adsr1 >> 8) & 0x7F;  *decr = 0; *expo = (v->adsr1 >> 15) & 1;
		break;
	case ENV_DECAY:
		*rate = ((v->adsr1 >> 4) & 0xF) << 2;  *decr = 1; *expo = 1;
		break;
	case ENV_SUSTAIN:
		*rate = (v->adsr2 >> 6) & 0x7F;  *decr = (v->adsr2 >> 14) & 1; *expo = (v->adsr2 >> 15) & 1;
		break;
	default:	// release
		*rate = (v->adsr2 & 0x1F) << 2;  *decr = 1; *expo = (v->adsr2 >> 5) & 1;
		break;
	}
}

static int EnvSustainLevel(const SPUALVoice* v)
{
	int sl = ((v->adsr1 & 0xF) + 1) << 11;
	return sl > 0x7FFF ? 0x7FFF : sl;
}

// sustain that cannot move any more (or release at 0): nothing left to step
static int EnvSettled(const SPUALVoice* v, const EnvState* e)
{
	if (e->phase == ENV_OFF)
		return 1;

	if (e->phase == ENV_SUSTAIN)
	{
		int rate, decr, expo;
		EnvPhaseParams(v, ENV_SUSTAIN, &rate, &decr, &expo);
		return decr ? e->level == 0 : e->level == 0x7FFF;
	}
	return 0;
}

// one envelope step: applies the step and returns the ticks until the next one
static int EnvStep(const SPUALVoice* v, EnvState* e)
{
	int rate, decr, expo, shift, step, ticks;

	EnvPhaseParams(v, e->phase, &rate, &decr, &expo);

	shift = rate >> 2;
	step = decr ? -8 + (rate & 3) : 7 - (rate & 3);

	if (shift < 11)
	{
		step *= 1 << (11 - shift);
		ticks = 1;
	}
	else
		ticks = 1 << (shift - 11);

	if (expo)
	{
		if (decr)
			step = (step * e->level) >> 15;
		else if (e->level >= 0x6000)
		{
			if (rate < 40)
				step >>= 2;
			else if (rate >= 44)
				ticks <<= 2;
			else
			{
				step >>= 1;
				ticks <<= 1;
			}
		}
	}

	e->level += step;
	if (e->level < 0)
		e->level = 0;
	if (e->level > 0x7FFF)
		e->level = 0x7FFF;

	switch (e->phase)
	{
	case ENV_ATTACK:
		if (e->level >= 0x7FFF)
			e->phase = ENV_DECAY;
		break;
	case ENV_DECAY:
		if (e->level <= EnvSustainLevel(v))
			e->phase = ENV_SUSTAIN;
		break;
	case ENV_RELEASE:
		if (e->level == 0)
			e->phase = ENV_OFF;
		break;
	}

	return ticks;
}

// run the envelope for n ticks of the 44.1 kHz clock
static void EnvAdvance(const SPUALVoice* v, EnvState* e, int n)
{
	while (n > 0 && !EnvSettled(v, e))
	{
		if (e->wait > n)
		{
			e->wait -= n;
			return;
		}
		n -= e->wait;
		e->wait = EnvStep(v, e);
	}
}

static void VoiceEnvUpdate(SPUALVoice* voice, double now)
{
	EnvState e = { voice->envPhase, voice->envLevel, voice->envWait };
	double dt = now - voice->envTime;
	int n;

	if (dt < 1.0)
		return;

	n = dt > 0x10000000 ? 0x10000000 : (int)dt;
	voice->envTime += n;

	EnvAdvance(voice, &e, n);

	voice->envPhase = e.phase;
	voice->envLevel = e.level;
	voice->envWait = e.wait;
}

// the AL gain: volume x envelope (the baked head carries attack/decay itself) x SpuSetMute
static void VoiceApplyGain(SPUALVoice* voice)
{
	float env;

	if (voice->alSource == AL_NONE)
		return;

	if (voice->envBaked && (voice->envPhase == ENV_ATTACK || voice->envPhase == ENV_DECAY))
		env = 1.0f;
	else
		env = (float)voice->envLevel / (float)voice->envScale;

	alSourcef(voice->alSource, AL_GAIN, g_SPUMuted ? 0.0f : voice->volGain * env);
}

// key-on: the envelope restarts at 0. If attack + decay settle within ENV_BAKE_TICKS, returns the per-tick
// envelope over that time in envOut (scaled to the level it settles on, which becomes envScale) and its length.
static int VoiceEnvKeyOn(SPUALVoice* voice, double now, float* envOut, int maxTicks)
{
	EnvState e = { ENV_ATTACK, 0, 1 };
	int t;

	voice->envPhase = ENV_ATTACK;
	voice->envLevel = 0;
	voice->envWait = 1;
	voice->envTime = now;
	voice->envBaked = 0;
	voice->envScale = 0x7FFF;

	// simulate: where does the envelope stop changing fast?
	for (t = 0; t < maxTicks; t++)
	{
		EnvAdvance(voice, &e, 1);
		envOut[t] = (float)e.level;

		if (e.phase == ENV_SUSTAIN || e.phase == ENV_OFF)
		{
			// a sustain that moves slower than full scale per bake window stays on the gain
			int rate, decr, expo, shift, stepmag;
			EnvPhaseParams(voice, ENV_SUSTAIN, &rate, &decr, &expo);
			shift = rate >> 2;
			stepmag = (8 << (shift < 11 ? 11 - shift : 0)) / (shift > 11 ? 1 << (shift - 11) : 1);

			if (EnvSettled(voice, &e) || stepmag * maxTicks < 0x8000)
			{
				int i;
				if (e.level <= 0)
					return 0;

				for (i = 0; i <= t; i++)
					envOut[i] /= (float)e.level;

				voice->envScale = e.level;
				return t + 1;
			}
		}
	}
	return 0;
}

static void UpdateVoiceSample(SPUALVoice* voice)
{
	static short waveBuffer[SPU_REALMEMSIZE];
	int loopStart, loopLen, count;
	ALuint alSource, alBuffer;

	//if (!voice->sampledirty)
	//	return;

	voice->sampledirty = 0;

	alSource = voice->alSource;
	alBuffer = voice->alBuffer;

	if (alSource == AL_NONE)
		return;

	loopStart = 0;
	loopLen = 0;

	count = decodeSound(s_SpuMemory.samplemem + voice->attr.addr, SPU_MEMSIZE - voice->attr.addr, waveBuffer, &loopStart, &loopLen, 1);

	if (count == 0)
		return;

#if 0	// sample test
	{
		ALuint aalSource;
		ALuint aalBuffer;

		alGenSources(1, &aalSource);
		alGenBuffers(1, &aalBuffer);

		// update AL buffer
		alBufferData(aalBuffer, AL_FORMAT_MONO16, waveBuffer, count * sizeof(short), 11000);

		// set the buffer
		alSourcei(aalSource, AL_BUFFER, aalBuffer);
		alSourcef(aalSource, AL_GAIN, 1.0f);// TODO: panning
		alSourcef(aalSource, AL_PITCH, 1);

		alSourcePlay(aalSource);
		int status;
		do
		{
			alGetSourcei(aalSource, AL_SOURCE_STATE, &status);
		} while (status == AL_PLAYING);

		alSourceStop(aalSource);

		alDeleteSources(1, &aalSource);
		alDeleteBuffers(1, &aalBuffer);
	}
#endif

	// Driver 1 port: key-on restarts the envelope; a fast attack/decay is multiplied into the head of the
	// PCM. Source sample j plays at 44.1 kHz tick j * 4096 / pitch.
	{
		static float envHead[ENV_BAKE_TICKS];
		int ticks = VoiceEnvKeyOn(voice, SpuClockNow(), envHead, ENV_BAKE_TICKS);

		if (ticks > 0)
		{
			int pitch = voice->attr.pitch ? voice->attr.pitch : 0x1000;
			int jEnd = (int)(((long long)ticks * pitch + 4095) / 4096);

			voice->envBaked = 1;

			// the looped part must stay unscaled: then the head plays at full gain, as before
			if (jEnd <= count && (loopLen <= 0 || loopStart >= jEnd))
			{
				for (int j = 0; j < jEnd; j++)
				{
					int t = (int)((long long)j * 4096 / pitch);
					if (t >= ticks)
						break;
					waveBuffer[j] = (short)(waveBuffer[j] * envHead[t]);
				}
			}
		}
	}

	alSourcei(alSource, AL_BUFFER, 0);
	alBufferData(alBuffer, AL_FORMAT_MONO16, waveBuffer, count * sizeof(short), 44100);

	if (loopLen > 0)
	{
		// Driver 1 port: loopStart is a sample index from the data's LoopStart flag (0 = from the start
		// address when the sample has none). It used to add loop_addr - addr, a byte distance, which with
		// loop_addr never set (Driver only sets WDSA) made it negative, so the whole sample looped.
		if (loopStart > 0 && loopStart + loopLen <= count)
		{
			int sampleOffs[] = { loopStart, loopStart + loopLen };
			alBufferiv(alBuffer, AL_LOOP_POINTS_SOFT, sampleOffs);
		}

		alSourcei(alSource, AL_LOOPING, AL_TRUE);
	}
	else
	{
		//int sampleOffs[] = { 0, 0 };
		//alBufferiv(alBuffer, AL_LOOP_POINTS_SOFT, sampleOffs);
		alSourcei(alSource, AL_LOOPING, AL_FALSE);
	}

	// set the buffer
	alSourcei(alSource, AL_BUFFER, alBuffer);
}

// Driver 1 port: SPUCNT bit 14 mutes the SPU output; voices keep running (and can be keyed on) underneath,
// the CD input (XA) is not affected (psx-spx "SPU Control Register").
int PsyX_SPUAL_SetMute(int on_off)
{
	int old_state = g_SPUMuted;

	if (!g_spuInit)
	{
		g_SPUMuted = on_off;
		return old_state;
	}

	SDL_LockMutex(g_SpuMutex);
	g_SPUMuted = on_off;
	for (int i = 0; i < s_spuVoiceCount; i++)
		VoiceApplyGain(&g_SpuVoices[i]);
	SDL_UnlockMutex(g_SpuMutex);

	return old_state;
}

void PsyX_SPUAL_GetVoiceVolume(int vNum, short* volL, short* volR)
{
	if (volL)
		*volL = g_SpuVoices[vNum].attr.volume.left;

	if (volR)
		*volR = g_SpuVoices[vNum].attr.volume.right;
}

void PsyX_SPUAL_GetVoicePitch(int vNum, u_short* pitch)
{
	*pitch = g_SpuVoices[vNum].attr.pitch;
}

void PsyX_SPUAL_SetVoiceAttr(SpuVoiceAttr* psxAttrib)
{
	if (!g_spuInit)
		return;

	const float STEREO_FACTOR = 3.0f;

	// Driver 1 port: mask 0 sets every attribute (libspu SpuSetVoiceAttr)
	const u_int mask = psxAttrib->mask ? psxAttrib->mask : 0xFFFFFFFF;
	
	SDL_LockMutex(g_SpuMutex);

	for (int i = 0; i < s_spuVoiceCount; i++)
	{
		if ((psxAttrib->voice & SPU_VOICECH(i)) == 0)
			continue;

		SPUALVoice* voice = &g_SpuVoices[i];
		ALuint alSource = voice->alSource;

		if (alSource == AL_NONE)
			continue;
		
		// update sample
		if ((mask & SPU_VOICE_WDSA) || (mask & SPU_VOICE_LSAX))
		{
			if (mask & SPU_VOICE_WDSA)
			{
				if (voice->attr.addr != psxAttrib->addr)
					voice->sampledirty++;

				voice->attr.addr = psxAttrib->addr;
			}
			
			if (mask & SPU_VOICE_LSAX)
			{
				if(voice->attr.loop_addr != psxAttrib->loop_addr)
					voice->sampledirty++;

				voice->attr.loop_addr = psxAttrib->loop_addr;
			}			
		}

		// update volume
		if ((mask & SPU_VOICE_VOLL) || (mask & SPU_VOICE_VOLR))
		{
			if (mask & SPU_VOICE_VOLL)
				voice->attr.volume.left = psxAttrib->volume.left;

			if (mask & SPU_VOICE_VOLR)
				voice->attr.volume.right = psxAttrib->volume.right;

			// Driver 1 port: a negative volume is a phase-inverted side (xmplay.c DoDolbySS's surround
			// channels: L = v, R = -v); mono AL sources can't invert one side, so take the magnitude. The
			// signed average made those channels (nearly) silent.
			float left_gain = fabsf((float)(voice->attr.volume.left)) / (float)(16384);
			float right_gain = fabsf((float)(voice->attr.volume.right)) / (float)(16384);

			if(left_gain > 1.0f)
				left_gain = 1.0f;

			if(right_gain > 1.0f)
				right_gain = 1.0f;

			float pan = (acosf(left_gain) + asinf(right_gain)) / ((float)(M_PI)); // average angle in [0,1]
			pan = 2.0f * pan - 1.0f; // convert to [-1, 1]
			pan = pan * 0.5f; // 0.5 = sin(30') for a +/- 30 degree arc
			alSource3f(alSource, AL_POSITION, pan * STEREO_FACTOR, 0, -sqrtf(1.0f - pan * pan));

			voice->volGain = (left_gain + right_gain) * 0.5f;
			VoiceApplyGain(voice);
		}

		// update pitch
		if (mask & SPU_VOICE_PITCH)
		{
			ALint state;
			alGetSourcei(alSource, AL_SOURCE_STATE, &state);

			if (psxAttrib->pitch == 0 && state == AL_PLAYING)
				alSourcePause(alSource);
			else if (voice->attr.pitch == 0 && state == AL_PAUSED)
				alSourcePlay(alSource);

			voice->attr.pitch = psxAttrib->pitch;

			const float pitch = (float)(voice->attr.pitch) / 4096.0f;
			alSourcef(alSource, AL_PITCH, pitch);
		}
		
		// Driver 1 port: ADSR, built into the two registers exactly as libspu's SpuSetVoiceAttr does (Driver's
		// copy at 0x7C984): raw ADSR1/ADSR2 first, then the fields; a mode only counts together with its
		// rate (a_mode 5 = exponential attack; s_mode 1/3/5/7 = linear inc/dec, exp inc/dec, absent = linear
		// dec; r_mode 7 = exponential release); rates/levels are clamped to their field widths.
		if (mask & SPU_VOICE_ADSR_ADSR1)
			voice->adsr1 = psxAttrib->adsr1;

		if (mask & SPU_VOICE_ADSR_ADSR2)
			voice->adsr2 = psxAttrib->adsr2;

		if (mask & SPU_VOICE_ADSR_AR)
		{
			int ar = psxAttrib->ar > 0x7F ? 0x7F : psxAttrib->ar;
			int m = ((mask & SPU_VOICE_ADSR_AMODE) && psxAttrib->a_mode == SPU_VOICE_EXPIncN) ? 0x80 : 0;
			voice->adsr1 = (voice->adsr1 & 0xFF) | ((ar | m) << 8);
		}

		if (mask & SPU_VOICE_ADSR_DR)
		{
			int dr = psxAttrib->dr > 0xF ? 0xF : psxAttrib->dr;
			voice->adsr1 = (voice->adsr1 & 0xFF0F) | (dr << 4);
		}

		if (mask & SPU_VOICE_ADSR_SR)
		{
			int sr = psxAttrib->sr > 0x7F ? 0x7F : psxAttrib->sr;
			int m = 0x100;

			if (mask & SPU_VOICE_ADSR_SMODE)
			{
				switch (psxAttrib->s_mode)
				{
				case SPU_VOICE_LINEARIncN:	m = 0x000; break;
				case SPU_VOICE_EXPIncN:		m = 0x200; break;
				case SPU_VOICE_EXPDec:		m = 0x300; break;
				}
			}
			voice->adsr2 = (voice->adsr2 & 0x3F) | ((sr | m) << 6);
		}

		if (mask & SPU_VOICE_ADSR_RR)
		{
			int rr = psxAttrib->rr > 0x1F ? 0x1F : psxAttrib->rr;
			int m = ((mask & SPU_VOICE_ADSR_RMODE) && psxAttrib->r_mode == SPU_VOICE_EXPDec) ? 0x20 : 0;
			voice->adsr2 = (voice->adsr2 & 0xFFC0) | rr | m;
		}

		if (mask & SPU_VOICE_ADSR_SL)
		{
			int sl = psxAttrib->sl > 0xF ? 0xF : psxAttrib->sl;
			voice->adsr1 = (voice->adsr1 & 0xFFF0) | sl;
		}
	}
	SDL_UnlockMutex(g_SpuMutex);
}

// AL source still sounding? (a non-looping sample that played to its end stops by itself: on the SPU its end
// flag forces the release, which ends the envelope)
static int VoiceSourceAlive(SPUALVoice* voice)
{
	ALint state;
	alGetSourcei(voice->alSource, AL_SOURCE_STATE, &state);
	return state == AL_PLAYING || state == AL_PAUSED;
}

static void VoiceKeyOff(SPUALVoice* voice, double now)
{
	voice->keyed = 0;

	if (voice->envPhase == ENV_OFF)
	{
		alSourceStop(voice->alSource);
		return;
	}

	VoiceEnvUpdate(voice, now);

	voice->envPhase = ENV_RELEASE;
	voice->envWait = 1;

	// a release that ends within ENV_RELEASE_CUT (Driver's are all 1-2 samples) ends the voice now
	{
		EnvState e = { ENV_RELEASE, voice->envLevel, 1 };
		EnvAdvance(voice, &e, ENV_RELEASE_CUT);

		if (e.phase == ENV_OFF)
		{
			voice->envPhase = ENV_OFF;
			voice->envLevel = 0;
			alSourceStop(voice->alSource);
			return;
		}
	}

	VoiceApplyGain(voice);
}

// Driver 1 port: D1_SPU_LOG=1 prints every key-on/off (time in 44.1 kHz ticks, voice, ADSR registers, the
// baked attack length, volume, pitch) - for checking envelopes against a WAV capture
static int SpuLogEnabled()
{
	static int s_log = -1;
	if (s_log < 0)
	{
		const char* e = getenv("D1_SPU_LOG");
		s_log = e && *e && *e != '0';
	}
	return s_log;
}

void PsyX_SPUAL_SetKey(int on_off, u_int voice_bit)
{
	if (!g_spuInit)
		return;

	SDL_LockMutex(g_SpuMutex);
	double now = SpuClockNow();

	for (int i = 0; i < s_spuVoiceCount; i++)
	{
		if ((voice_bit & SPU_VOICECH(i)) == 0)
			continue;

		SPUALVoice* voice = &g_SpuVoices[i];
		ALuint alSource = voice->alSource;

		if (alSource == AL_NONE)
			continue;

		// Driver 1 port: a muted SPU still keys voices on (they play silently, SpuSetMute)
		if (on_off)
		{
			alSourceStop(alSource);
			UpdateVoiceSample(voice);		// restarts the envelope
			voice->keyed = 1;
			VoiceApplyGain(voice);

			if (SpuLogEnabled())
				printf("[spu] %.0f on  v%d adsr %04X %04X baked %d scale %d vol %d,%d pitch %04X addr %X\n", now, i,
					voice->adsr1, voice->adsr2, voice->envBaked, voice->envScale, voice->attr.volume.left,
					voice->attr.volume.right, voice->attr.pitch, voice->attr.addr);

			// pitch 0 (XM_Pause) keeps a voice paused on PC; the envelope runs on regardless, as on the SPU
			if (voice->attr.pitch != 0)
				alSourcePlay(alSource);
			else
			{
				alSourcePlay(alSource);
				alSourcePause(alSource);
			}
		}
		else
		{
			if (SpuLogEnabled() && voice->envPhase != ENV_OFF)
				printf("[spu] %.0f off v%d phase %d level %d\n", now, i, voice->envPhase, voice->envLevel);

			VoiceKeyOff(voice, now);
		}
	}
	SDL_UnlockMutex(g_SpuMutex);
}

// libspu: key bit set -> SPU_ON (ENVX != 0) / SPU_ON_ENV_OFF; clear -> SPU_OFF_ENV_ON (still releasing) / SPU_OFF
static int VoiceKeyStatus(SPUALVoice* voice, double now)
{
	int envOn;

	if (voice->alSource == AL_NONE)
		return SPU_OFF;

	if (voice->envPhase != ENV_OFF)
	{
		if (!VoiceSourceAlive(voice))
		{
			voice->envPhase = ENV_OFF;
			voice->envLevel = 0;
		}
		else
			VoiceEnvUpdate(voice, now);
	}

	// right after key-on ENVX can still be 0: the voice is in its attack, count it as sounding
	envOn = voice->envPhase != ENV_OFF;

	if (voice->keyed)
		return envOn ? SPU_ON : SPU_ON_ENV_OFF;

	return envOn ? SPU_OFF_ENV_ON : SPU_OFF;
}

int PsyX_SPUAL_GetKeyStatus(u_int voice_bit)
{
	int status = SPU_OFF;

	if (!g_spuInit)
		return SPU_OFF;

	SDL_LockMutex(g_SpuMutex);
	double now = SpuClockNow();

	for (int i = 0; i < s_spuVoiceCount; i++)
	{
		if (voice_bit != SPU_VOICECH(i))
			continue;

		status = VoiceKeyStatus(&g_SpuVoices[i], now);
		break;
	}

	SDL_UnlockMutex(g_SpuMutex);

	return status;
}

void PsyX_SPUAL_GetAllKeysStatus(char* status)
{
	if (!g_spuInit)
	{
		memset(status, 0, s_spuVoiceCount);
		return;
	}

	SDL_LockMutex(g_SpuMutex);
	double now = SpuClockNow();

	for (int i = 0; i < s_spuVoiceCount; i++)
		status[i] = VoiceKeyStatus(&g_SpuVoices[i], now);

	SDL_UnlockMutex(g_SpuMutex);
}

// the envelope thread: advances every sounding voice's envelope and updates its gain
static int EnvThreadMain(void* data)
{
	while (!s_envThreadStop)
	{
		SDL_Delay(ENV_TICK_MS);

		if (!g_spuInit)
			continue;

		SDL_LockMutex(g_SpuMutex);
		double now = SpuClockNow();

		for (int i = 0; i < s_spuVoiceCount; i++)
		{
			SPUALVoice* voice = &g_SpuVoices[i];

			if (voice->alSource == AL_NONE || voice->envPhase == ENV_OFF)
				continue;

			if (!VoiceSourceAlive(voice))
			{
				voice->envPhase = ENV_OFF;
				voice->envLevel = 0;
				continue;
			}

			int oldLevel = voice->envLevel, oldPhase = voice->envPhase;
			VoiceEnvUpdate(voice, now);

			if (voice->envPhase == ENV_OFF)
			{
				// release finished
				alSourceStop(voice->alSource);
				continue;
			}

			if (voice->envLevel != oldLevel || voice->envPhase != oldPhase)
				VoiceApplyGain(voice);
		}

		SDL_UnlockMutex(g_SpuMutex);
	}
	return 0;
}

int PsyX_SPUAL_SetReverb(int on_off)
{
	int old_state = g_enableSPUReverb;
	g_enableSPUReverb = on_off;

	if (!g_spuInit)
		return old_state;
#ifndef __EMSCRIPTEN__
	// switch if needed
	if (g_ALEffectsSupported && old_state != g_enableSPUReverb)
	{
		if (g_enableSPUReverb)
		{
			alAuxiliaryEffectSloti(g_ALEffectSlots[g_currEffectSlotIdx], AL_EFFECTSLOT_EFFECT, g_nAlReverbEffect);
		}
		else
		{
			g_currEffectSlotIdx = 0;
			alAuxiliaryEffectSloti(g_ALEffectSlots[0], AL_EFFECTSLOT_EFFECT, AL_EFFECT_NULL);
			alAuxiliaryEffectSloti(g_ALEffectSlots[1], AL_EFFECTSLOT_EFFECT, AL_EFFECT_NULL);
		}
	}
#endif // __EMSCRIPTEN__
	return old_state;
}

int PsyX_SPUAL_GetReverbState()
{
	return g_enableSPUReverb;
}

u_int PsyX_SPUAL_SetReverbVoice(int on_off, u_int voice_bit)
{
	if (!g_spuInit)
		return 0;

	if (!g_ALEffectsSupported)
		return 0;

	SDL_LockMutex(g_SpuMutex);

	for (int i = 0; i < s_spuVoiceCount; i++)
	{
		if ((voice_bit & SPU_VOICECH(i)) == 0)
			continue;

		SPUALVoice* voice = &g_SpuVoices[i];
		ALuint alSource = voice->alSource;
		if (alSource == AL_NONE)
			continue;

		voice->reverb = on_off > 0;
#ifndef __EMSCRIPTEN__
		if (on_off)
			alSource3i(alSource, AL_AUXILIARY_SEND_FILTER, g_ALEffectSlots[g_currEffectSlotIdx], 0, AL_FILTER_NULL);
		else
			alSource3i(alSource, AL_AUXILIARY_SEND_FILTER, AL_EFFECTSLOT_NULL, 0, AL_FILTER_NULL);
#endif // __EMSCRIPTEN__
	}

	SDL_UnlockMutex(g_SpuMutex);

	return 0;
}

u_int PsyX_SPUAL_GetReverbVoice()
{
	u_int bits = 0;
	for (int i = 0; i < s_spuVoiceCount; i++)
	{
		SPUALVoice* voice = &g_SpuVoices[i];
		if (voice->reverb)
			bits |= SPU_KEYCH(i);
	}
	return bits;
}
//--------------------------------------------------------------------------------
// Driver 1 port: XA-ADPCM stream output (the SPU's CD input). port/pc/pc_xa.c decodes the sectors the drive
// sends to the SPU and queues them here as 16-bit PCM on one streaming source.

static ALuint	s_xaFree[XA_NUM_BUFFERS];
static int		s_xaNumFree = 0;
static float	s_xaGain = 1.0f;

static void XA_Init()
{
	if (s_xaSource != AL_NONE || !g_ALCcontext)
		return;

	alGenSources(1, &s_xaSource);
	alGenBuffers(XA_NUM_BUFFERS, s_xaBuffers);
	for (int i = 0; i < XA_NUM_BUFFERS; i++)
		s_xaFree[i] = s_xaBuffers[i];
	s_xaNumFree = XA_NUM_BUFFERS;

	alSourcei(s_xaSource, AL_SOURCE_RELATIVE, AL_TRUE);
	alSource3f(s_xaSource, AL_POSITION, 0, 0, 0);
	alSourcef(s_xaSource, AL_GAIN, s_xaGain);
}

static void XA_Reclaim()
{
	ALint processed = 0;
	alGetSourcei(s_xaSource, AL_BUFFERS_PROCESSED, &processed);
	while (processed-- > 0 && s_xaNumFree < XA_NUM_BUFFERS)
	{
		ALuint buf;
		alSourceUnqueueBuffers(s_xaSource, 1, &buf);
		s_xaFree[s_xaNumFree++] = buf;
	}
}

extern "C" void PsyX_SPUAL_XAQueue(const short* pcm, int frames, int stereo, int rate)
{
	if (!g_spuInit || !g_ALCcontext || frames <= 0)
		return;

	SDL_LockMutex(g_SpuMutex);
	XA_Init();
	XA_Reclaim();

	if (s_xaNumFree > 0)
	{
		ALuint buf = s_xaFree[--s_xaNumFree];
		alBufferData(buf, stereo ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16, pcm, frames * (stereo ? 4 : 2), rate);
		alSourceQueueBuffers(s_xaSource, 1, &buf);

		ALint state;
		alGetSourcei(s_xaSource, AL_SOURCE_STATE, &state);
		if (state != AL_PLAYING)		// first sector, or the queue ran dry
			alSourcePlay(s_xaSource);
	}
	else
	{
		// more than XA_NUM_BUFFERS sectors behind the output: drop this one
		static int dropped = 0;
		if ((dropped++ & 1023) == 0)
			eprintf("SPU WARNING: XA output %d sectors behind, sector dropped (%d so far)\n", XA_NUM_BUFFERS, dropped);
	}

	SDL_UnlockMutex(g_SpuMutex);
}

extern "C" void PsyX_SPUAL_XAStop(void)
{
	if (!g_spuInit || s_xaSource == AL_NONE)
		return;

	SDL_LockMutex(g_SpuMutex);
	alSourceStop(s_xaSource);		// all queued buffers become processed
	XA_Reclaim();
	SDL_UnlockMutex(g_SpuMutex);
}

// SsSetSerialVol: 0..127 per side (CD input volume)
extern "C" void PsyX_SPUAL_XASetVolume(int left, int right)
{
	s_xaGain = (float)(left + right) / 254.0f;
	if (s_xaGain < 0.0f)
		s_xaGain = 0.0f;
	if (s_xaGain > 1.0f)
		s_xaGain = 1.0f;

	if (!g_spuInit || s_xaSource == AL_NONE)
		return;

	SDL_LockMutex(g_SpuMutex);
	alSourcef(s_xaSource, AL_GAIN, s_xaGain);
	SDL_UnlockMutex(g_SpuMutex);
}

//--------------------------------------------------------------------------------
// Driver 1 port: SpuSetReverbDepth -> the reverb slot's gain (the EFX reverb's own gain is tuned for a
// half-scale depth, 4000h, which is what Driver sets with its Hall mode), SpuSetCommonAttr's master volume ->
// the listener gain (3FFFh = 1; the CD input goes through the main volume too)

extern "C" void PsyX_SPUAL_SetReverbDepth(int left, int right)
{
	float g = (float)(abs(left) > abs(right) ? abs(left) : abs(right)) / (float)0x4000;

	if (g > 1.0f)
		g = 1.0f;

	if (!g_spuInit || !g_ALEffectsSupported)
		return;
#ifndef __EMSCRIPTEN__
	SDL_LockMutex(g_SpuMutex);
	alAuxiliaryEffectSlotf(g_ALEffectSlots[0], AL_EFFECTSLOT_GAIN, g);
	SDL_UnlockMutex(g_SpuMutex);
#endif
}

extern "C" void PsyX_SPUAL_SetMasterVolume(int left, int right)
{
	float g = (float)(abs(left) + abs(right)) / (float)(2 * 0x3FFF);

	if (g > 1.0f)
		g = 1.0f;

	if (!g_spuInit || !g_ALCcontext)
		return;

	SDL_LockMutex(g_SpuMutex);
	alListenerf(AL_GAIN, g);
	SDL_UnlockMutex(g_SpuMutex);
}
