#include "PsyX_main.h"

#include "PsyX/PsyX_version.h"
#include "PsyX/PsyX_globals.h"
#include "PsyX/PsyX_public.h"
#include "PsyX/util/timer.h"

#include "gpu/PsyX_GPU.h"
#include "pad/PsyX_pad.h"

#include "platform.h"
#include "util/crash_handler.h"

#include "psx/libetc.h"
#include "psx/libgte.h"
#include "psx/libgpu.h"
#include "psx/libspu.h"

#include <assert.h>
#include <string.h>
#include <strings.h>

#include <stdio.h>
#include <SDL.h>

#include "PsyX/PsyX_render.h"

#ifdef _WIN32
#include <pla.h>
#endif // _WIN32

#ifdef __EMSCRIPTEN__
int strcasecmp(const char* _l, const char* _r)
{
	const u_char* l = (u_char*)_l, * r = (u_char*)_r;
	for (; *l && *r && (*l == *r || tolower(*l) == tolower(*r)); l++, r++);
	return tolower(*l) - tolower(*r);
}
#elif !defined(_WIN32)
#include <strings.h>
#endif

SDL_Window* g_window = NULL;
int g_swapInterval = 1;
int g_enableSwapInterval = 1;
int g_skipSwapInterval = 0;
timerCtx_t g_vblTimer;

int							g_cfg_swapInterval = 0;
PsyXKeyboardMapping			g_cfg_keyboardMapping;
PsyXControllerMapping		g_cfg_controllerMapping;
GameOnTextInputHandler		g_cfg_gameOnTextInput = NULL;

GameDebugKeysHandlerFunc	g_dbg_gameDebugKeys = NULL;
GameDebugMouseHandlerFunc	g_dbg_gameDebugMouse = NULL;
int							g_dbg_polygonSelected = 0;

enum EPsxCounters
{
	PsxCounter_VBLANK,

	PsxCounter_Num
};

volatile int g_psxSysCounters[PsxCounter_Num];

SDL_Thread* g_intrThread = NULL;
SDL_mutex* g_intrMutex = NULL;
volatile char g_stopIntrThread = 0;

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif

extern void(*vsync_callback)(void);

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif

extern int	PsyX_Pad_InitSystem();
extern void PsyX_Pad_Event_ControllerRemoved(Sint32 deviceId);
extern void PsyX_Pad_Event_ControllerAdded(Sint32 deviceId);

extern int	GR_InitialisePSX();
extern int	GR_InitialiseRender(char* windowName, int width, int height, int fullscreen);

extern void GR_ResetDevice();
extern void GR_Shutdown();
extern void GR_BeginScene();
extern void GR_EndScene();
extern void GR_UpdateSwapIntervalState(int swapInterval);

int g_vmode = -1;
int g_frameSkip = 0;

#ifdef __EMSCRIPTEN__

int g_emIntrInterval = -1;
int g_intrVMode = MODE_NTSC;
double g_emOldDate = 0;

void emIntrCallback(void* userData)
{
	double timestep = g_vmode == MODE_NTSC ? FIXED_TIME_STEP_NTSC : FIXED_TIME_STEP_PAL;

	int newVBlank = (Util_GetHPCTime(&g_vblTimer, 0) / timestep) + g_frameSkip;

	int diff = newVBlank - g_psxSysCounters[PsxCounter_VBLANK];

	while (diff--)
	{
		if (vsync_callback)
			vsync_callback();

		g_psxSysCounters[PsxCounter_VBLANK]++;
	}
}

EM_BOOL emIntrCallback2(double time, void* userData)
{
	emIntrCallback(userData);
	return g_stopIntrThread ? EM_FALSE : EM_TRUE;
}

#endif

int PsyX_Sys_SetVMode(int mode)
{
	int old = g_vmode;
	g_vmode = mode;

#ifdef __EMSCRIPTEN__
	if (old != g_vmode)
	{
		//if(g_emIntrInterval != -1)
		//	emscripten_clear_interval(g_emIntrInterval);
		g_stopIntrThread = 1;

		emscripten_sleep(100);

		g_stopIntrThread = 0;
		emscripten_set_timeout_loop(emIntrCallback2, 1.0, NULL);
	}
#endif

	return old;
}

int PsyX_Sys_GetVBlankCount()
{
	if (g_skipSwapInterval)
	{
		// extra speedup.
		// does not affect `vsync_callback` count
		g_psxSysCounters[PsxCounter_VBLANK] += 1;
		g_frameSkip++;
	}
	
	return g_psxSysCounters[PsxCounter_VBLANK];
}

/* Driver 1 port: the vblank clock sleeps instead of spinning.
 * PsyCross's interrupt thread busy-looped on gettimeofday (a whole core, ~100% CPU, all the time) and fired a vblank
 * when more than a timestep had passed since the previous one, then restarted the interval from that moment
 * (60 Hz, measured 60.000 Hz: the overshoot was a few us). Now: vblank n is due at base + n * timestep (no drift:
 * exactly 60 Hz NTSC / 50 Hz PAL, as before), the thread sleeps on an absolute monotonic deadline (Linux:
 * clock_nanosleep with 1 ns timer slack; measured: wakes 20-25 us late on average, < 0.2 ms at worst) and can spin
 * for the last D1_VBL_SPIN_US microseconds (default 0: the spin cost ~1.5% CPU for no audible or visible gain),
 * then runs the PS1 VSyncCallback (XM music) and bumps the counter as before. If the clock falls more
 * than 4 vblanks behind (suspend, debugger) the missed ones are dropped, as the old loop did, instead of being
 * fired in a burst. Waiters (VSync(0)/VSync(n) -> PsyX_WaitForTimestep, PsyX_Sys_WaitVBlankChange) block on a
 * condition variable the thread signals after each count. D1_VBLSTAT=1 prints the timing every 10 s. */
#if !defined(_WIN32) && !defined(__EMSCRIPTEN__)
#include <time.h>
#include <errno.h>
#ifdef __linux__
#include <sys/prctl.h>
#endif
#endif

static SDL_mutex* g_vblMutex = NULL;
static SDL_cond* g_vblCond = NULL;
static volatile long long g_vblLastNs = 0;	// when the last vblank was counted (D1_VBLSTAT)
extern char begin_scene_flag;
static int g_sceneBeginVbl = 0;				// the vblank count when the pending scene began (PsyX_WaitPresentVBlank)

static long long VblNowNs()
{
#if !defined(_WIN32) && !defined(__EMSCRIPTEN__)
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	return (long long)ts.tv_sec * 1000000000LL + ts.tv_nsec;
#else
	static Uint64 freq = 0;
	if (!freq)
		freq = SDL_GetPerformanceFrequency();
	Uint64 c = SDL_GetPerformanceCounter();
	return (long long)(c / freq) * 1000000000LL + (long long)((c % freq) * 1000000000ULL / freq);
#endif
}

static void VblSleepUntilNs(long long t)
{
#if !defined(_WIN32) && !defined(__EMSCRIPTEN__)
	struct timespec ts;
	ts.tv_sec = (time_t)(t / 1000000000LL);
	ts.tv_nsec = (long)(t % 1000000000LL);
	while (clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &ts, NULL) == EINTR)
		;
#else
	long long d = t - VblNowNs();
	if (d > 1000000)
		SDL_Delay((Uint32)(d / 1000000));
#endif
}

static inline void VblCpuRelax()
{
#if defined(__i386__) || defined(__x86_64__)
	__builtin_ia32_pause();
#endif
}

int intrThreadMain(void* data)
{
	Util_InitHPCTimer(&g_vblTimer);

#ifdef __linux__
	prctl(PR_SET_TIMERSLACK, 1UL, 0, 0, 0);	// wake on the deadline, not up to 50 us after it
#endif
	SDL_SetThreadPriority(SDL_THREAD_PRIORITY_HIGH);	// best effort (needs no privileges to fail quietly)

	const char* spinEnv = getenv("D1_VBL_SPIN_US");
	const long long spinNs = (spinEnv ? atoi(spinEnv) : 0) * 1000LL;
	const int stats = getenv("D1_VBLSTAT") && atoi(getenv("D1_VBLSTAT"));

	int mode = g_vmode;
	double periodNs = 1e9 / (mode == MODE_NTSC ? VBLANK_FREQUENCY_NTSC : VBLANK_FREQUENCY_PAL);
	long long base = VblNowNs();
	long long n = 1;					// vblank n is due at base + n * periodNs

	long long stStart = base, stLateSum = 0, stLateMax = 0, stOver1ms = 0, stCount = 0, stDropped = 0;

	while (!g_stopIntrThread)
	{
		if (g_vmode != mode)
		{
			// video mode change: the next vblank comes one new period after the last one
			base += (long long)((n - 1) * periodNs);
			n = 1;
			mode = g_vmode;
			periodNs = 1e9 / (mode == MODE_NTSC ? VBLANK_FREQUENCY_NTSC : VBLANK_FREQUENCY_PAL);
		}

		const long long due = base + (long long)(n * periodNs);
		long long now = VblNowNs();

		if (now < due)
		{
			if (due - now > spinNs)
				VblSleepUntilNs(due - spinNs);

			while ((now = VblNowNs()) < due)
				VblCpuRelax();
		}

		if (now - due > 4 * (long long)periodNs)
		{
			// fell behind (suspend/resume, a debugger stop): drop the missed vblanks, restart from now
			stDropped += (long long)((now - due) / periodNs);
			base = now;
			n = 1;
		}
		else
			n++;

		SDL_LockMutex(g_intrMutex);

		if (vsync_callback)
			vsync_callback();

		SDL_UnlockMutex(g_intrMutex);

		// do vblank events
		g_vblLastNs = now;
		g_psxSysCounters[PsxCounter_VBLANK]++;

		SDL_LockMutex(g_vblMutex);
		SDL_CondBroadcast(g_vblCond);
		SDL_UnlockMutex(g_vblMutex);

		if (stats)
		{
			const long long late = now - due;
			stLateSum += late;
			if (late > stLateMax)
				stLateMax = late;
			if (late > 1000000)
				stOver1ms++;
			if (++stCount == 600)
			{
				const long long t = VblNowNs();
				eprintinfo("vblank clock: %.4f Hz, late avg %lld us max %lld us, >1 ms %lld, dropped %lld\n",
					stCount * 1e9 / (double)(t - stStart), stLateSum / stCount / 1000, stLateMax / 1000, stOver1ms, stDropped);
				stStart = t;
				stLateSum = stLateMax = stOver1ms = stCount = stDropped = 0;
			}
		}
	}

	return 0;
}

/* Driver 1 port: block until the vblank counter differs from `last` (or timeoutMs passed); returns the counter.
 * For a game thread that polls VSync(-1) in a loop (pc_main.c __wrap_VSync). */
int PsyX_Sys_WaitVBlankChange(int last, int timeoutMs)
{
#ifndef __EMSCRIPTEN__
	if (g_vblMutex && !g_skipSwapInterval)
	{
		const Uint32 start = SDL_GetTicks();
		SDL_LockMutex(g_vblMutex);
		while (g_psxSysCounters[PsxCounter_VBLANK] == last && !g_stopIntrThread)
		{
			const Uint32 spent = SDL_GetTicks() - start;
			if ((int)spent >= timeoutMs)
				break;
			SDL_CondWaitTimeout(g_vblCond, g_vblMutex, timeoutMs - spent);
		}
		SDL_UnlockMutex(g_vblMutex);
	}
#endif
	return PsyX_Sys_GetVBlankCount();
}

/* Driver 1 port: frame pacing. A PS1 frame becomes visible at the vblank after the game flips to it; a scene
 * presented as soon as it is drawn comes out at a varying time after the vblank (the game's work: 1.5-7 ms in
 * Driver's in-game loop), so a compositor latching at a fixed phase sometimes shows it one refresh early or late.
 * Called before presenting a pending scene: if the vblank count is still the one the scene began in, block until
 * the next vblank (at most 50 ms); a scene that is already late is presented at once. Returns the vblank count. */
int PsyX_WaitPresentVBlank(void)
{
	if (begin_scene_flag && g_psxSysCounters[PsxCounter_VBLANK] == g_sceneBeginVbl)
		return PsyX_Sys_WaitVBlankChange(g_sceneBeginVbl, 50);
	return PsyX_Sys_GetVBlankCount();
}

static int PsyX_Sys_InitialiseCore()
{
#ifdef __EMSCRIPTEN__
	Util_InitHPCTimer(&g_vblTimer);
#else
	// Driver 1 port: the mutexes before the thread that uses them
	g_intrMutex = SDL_CreateMutex();
	g_vblMutex = SDL_CreateMutex();
	g_vblCond = SDL_CreateCond();
	if (NULL == g_intrMutex || NULL == g_vblMutex || NULL == g_vblCond)
	{
		eprinterr("SDL_CreateMutex failed: %s\n", SDL_GetError());
		return 0;
	}

	g_intrThread = SDL_CreateThread(intrThreadMain, "psyX_intr", NULL);

	if (NULL == g_intrThread)
	{
		eprinterr("SDL_CreateThread failed: %s\n", SDL_GetError());
		return 0;
	}
#endif
	return 1;
}

static void PsyX_Sys_InitialiseInput()
{
	g_cfg_keyboardMapping.kc_square = SDL_SCANCODE_X;
	g_cfg_keyboardMapping.kc_circle = SDL_SCANCODE_V;
	g_cfg_keyboardMapping.kc_triangle = SDL_SCANCODE_Z;
	g_cfg_keyboardMapping.kc_cross = SDL_SCANCODE_C;

	g_cfg_keyboardMapping.kc_l1 = SDL_SCANCODE_LSHIFT;
	g_cfg_keyboardMapping.kc_l2 = SDL_SCANCODE_LCTRL;
	g_cfg_keyboardMapping.kc_l3 = SDL_SCANCODE_LEFTBRACKET;

	g_cfg_keyboardMapping.kc_r1 = SDL_SCANCODE_RSHIFT;
	g_cfg_keyboardMapping.kc_r2 = SDL_SCANCODE_RCTRL;
	g_cfg_keyboardMapping.kc_r3 = SDL_SCANCODE_RIGHTBRACKET;

	g_cfg_keyboardMapping.kc_dpad_up = SDL_SCANCODE_UP;
	g_cfg_keyboardMapping.kc_dpad_down = SDL_SCANCODE_DOWN;
	g_cfg_keyboardMapping.kc_dpad_left = SDL_SCANCODE_LEFT;
	g_cfg_keyboardMapping.kc_dpad_right = SDL_SCANCODE_RIGHT;

	g_cfg_keyboardMapping.kc_select = SDL_SCANCODE_SPACE;
	g_cfg_keyboardMapping.kc_start = SDL_SCANCODE_RETURN;

	//----------------
	g_cfg_controllerMapping.gc_square = SDL_CONTROLLER_BUTTON_X;
	g_cfg_controllerMapping.gc_circle = SDL_CONTROLLER_BUTTON_B;
	g_cfg_controllerMapping.gc_triangle = SDL_CONTROLLER_BUTTON_Y;
	g_cfg_controllerMapping.gc_cross = SDL_CONTROLLER_BUTTON_A;

	g_cfg_controllerMapping.gc_l1 = SDL_CONTROLLER_BUTTON_LEFTSHOULDER;
	g_cfg_controllerMapping.gc_l2 = SDL_CONTROLLER_AXIS_TRIGGERLEFT | CONTROLLER_MAP_FLAG_AXIS;
	g_cfg_controllerMapping.gc_l3 = SDL_CONTROLLER_BUTTON_LEFTSTICK;

	g_cfg_controllerMapping.gc_r1 = SDL_CONTROLLER_BUTTON_RIGHTSHOULDER;
	g_cfg_controllerMapping.gc_r2 = SDL_CONTROLLER_AXIS_TRIGGERRIGHT | CONTROLLER_MAP_FLAG_AXIS;
	g_cfg_controllerMapping.gc_r3 = SDL_CONTROLLER_BUTTON_RIGHTSTICK;

	g_cfg_controllerMapping.gc_dpad_up = SDL_CONTROLLER_BUTTON_DPAD_UP;
	g_cfg_controllerMapping.gc_dpad_down = SDL_CONTROLLER_BUTTON_DPAD_DOWN;
	g_cfg_controllerMapping.gc_dpad_left = SDL_CONTROLLER_BUTTON_DPAD_LEFT;
	g_cfg_controllerMapping.gc_dpad_right = SDL_CONTROLLER_BUTTON_DPAD_RIGHT;

	g_cfg_controllerMapping.gc_select = SDL_CONTROLLER_BUTTON_BACK;
	g_cfg_controllerMapping.gc_start = SDL_CONTROLLER_BUTTON_START;

	g_cfg_controllerMapping.gc_axis_left_x = SDL_CONTROLLER_AXIS_LEFTX | CONTROLLER_MAP_FLAG_AXIS;
	g_cfg_controllerMapping.gc_axis_left_y = SDL_CONTROLLER_AXIS_LEFTY | CONTROLLER_MAP_FLAG_AXIS;
	g_cfg_controllerMapping.gc_axis_right_x = SDL_CONTROLLER_AXIS_RIGHTX | CONTROLLER_MAP_FLAG_AXIS;
	g_cfg_controllerMapping.gc_axis_right_y = SDL_CONTROLLER_AXIS_RIGHTY | CONTROLLER_MAP_FLAG_AXIS;

	PsyX_Pad_InitSystem();
}

#ifdef __GNUC__
#define _stricmp(s1, s2) SDL_strcasecmp(s1, s2)
#endif

// Keyboard mapping lookup
int PsyX_LookupKeyboardMapping(const char* str, int default_value)
{
	const char* scancodeName;
	int i;

	if (str)
	{
		if (!_stricmp("NONE", str))
			return SDL_SCANCODE_UNKNOWN;

		for (i = 0; i < SDL_NUM_SCANCODES; i++)
		{
			scancodeName = SDL_GetScancodeName((SDL_Scancode)i);

			if (strlen(scancodeName) && !_stricmp(scancodeName, str))
			{
				return i;
			}
		}
	}

	return default_value;
}

// Game controller mapping lookup
// Available controller binds(refer to SDL2 game controller)
//
// Axes:
//	leftx lefty
//	rightx righty
//	lefttrigger righttrigger
//
// NOTE: adding `-` before axis names makes it inverse, so `-leftx` inverse left stick X axis
//
// Buttons:
// 	a, b, x, y
// 	back guide start
// 	leftstick rightstick
// 	leftshoulder rightshoulder
// 	dpup dpdown dpleft dpright

int PsyX_LookupGameControllerMapping(const char* str, int default_value)
{
	const char* axisStr;
	const char* buttonOrAxisName;
	int i, axisFlags;

	if (str)
	{
		axisFlags = CONTROLLER_MAP_FLAG_AXIS;
		axisStr = str;

		if (*axisStr == '-')
		{
			axisFlags |= CONTROLLER_MAP_FLAG_INVERSE;
			axisStr++;
		}

		if (!_stricmp("NONE", str))
			return SDL_CONTROLLER_BUTTON_INVALID;

		// check buttons
		for (i = 0; i < SDL_CONTROLLER_BUTTON_MAX; i++)
		{
			buttonOrAxisName = SDL_GameControllerGetStringForButton((SDL_GameControllerButton)i);

			if (strlen(buttonOrAxisName) && !_stricmp(buttonOrAxisName, str))
			{
				return i;
			}
		}

		// Check axes
		for (i = 0; i < SDL_CONTROLLER_AXIS_MAX; i++)
		{
			buttonOrAxisName = SDL_GameControllerGetStringForAxis((SDL_GameControllerAxis)i);

			if (strlen(buttonOrAxisName) && !_stricmp(buttonOrAxisName, axisStr))
			{
				return i | axisFlags;
			}
		}
	}

	return default_value;
}

char* g_appNameStr = NULL;

void PsyX_GetWindowName(char* buffer)
{
#ifdef _DEBUG
	sprintf(buffer, "%s | Debug", g_appNameStr);
#else
	sprintf(buffer, "%s", g_appNameStr);
#endif
}

FILE* g_logStream = NULL;

// intialise logging
void PsyX_Log_Initialise()
{
	char appLogFilename[128];
	sprintf(appLogFilename, "%s.log", g_appNameStr);

	g_logStream = fopen(appLogFilename, "wb");

	if (!g_logStream)
		eprinterr("Error - cannot create log file '%s'\n", appLogFilename);
}

void PsyX_Log_Finalise()
{
	PsyX_Log_Warning("---- LOG CLOSED ----\n");

	if (g_logStream)
		fclose(g_logStream);

	g_logStream = NULL;
}

void PsyX_Log_Flush()
{
	if (g_logStream)
		fflush(g_logStream);
}

// spew types
typedef enum
{
	SPEW_NORM,
	SPEW_INFO,
	SPEW_WARNING,
	SPEW_ERROR,
	SPEW_SUCCESS,
}SpewType_t;

#ifdef _WIN32
static unsigned short g_InitialColor = 0xFFFF;
static unsigned short g_LastColor = 0xFFFF;
static unsigned short g_BadColor = 0xFFFF;
static WORD g_BackgroundFlags = 0xFFFF;
CRITICAL_SECTION g_SpewCS;
char g_bSpewCSInitted = 0;

static void Spew_GetInitialColors()
{
	// Get the old background attributes.
	CONSOLE_SCREEN_BUFFER_INFO oldInfo;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &oldInfo);
	g_InitialColor = g_LastColor = oldInfo.wAttributes & (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	g_BackgroundFlags = oldInfo.wAttributes & (BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY);

	g_BadColor = 0;
	if (g_BackgroundFlags & BACKGROUND_RED)
		g_BadColor |= FOREGROUND_RED;
	if (g_BackgroundFlags & BACKGROUND_GREEN)
		g_BadColor |= FOREGROUND_GREEN;
	if (g_BackgroundFlags & BACKGROUND_BLUE)
		g_BadColor |= FOREGROUND_BLUE;
	if (g_BackgroundFlags & BACKGROUND_INTENSITY)
		g_BadColor |= FOREGROUND_INTENSITY;
}

static WORD Spew_SetConsoleTextColor(int red, int green, int blue, int intensity)
{
	WORD ret = g_LastColor;

	g_LastColor = 0;
	if (red)	g_LastColor |= FOREGROUND_RED;
	if (green) g_LastColor |= FOREGROUND_GREEN;
	if (blue)  g_LastColor |= FOREGROUND_BLUE;
	if (intensity) g_LastColor |= FOREGROUND_INTENSITY;

	// Just use the initial color if there's a match...
	if (g_LastColor == g_BadColor)
		g_LastColor = g_InitialColor;

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), g_LastColor | g_BackgroundFlags);
	return ret;
}

static void Spew_RestoreConsoleTextColor(WORD color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color | g_BackgroundFlags);
	g_LastColor = color;
}

void Spew_ConDebugSpew(SpewType_t type, char* text)
{
	// Hopefully two threads won't call this simultaneously right at the start!
	if (!g_bSpewCSInitted)
	{
		Spew_GetInitialColors();
		InitializeCriticalSection(&g_SpewCS);
		g_bSpewCSInitted = 1;
	}

	WORD old;
	EnterCriticalSection(&g_SpewCS);
	{
		if (type == SPEW_NORM)
		{
			old = Spew_SetConsoleTextColor(1, 1, 1, 0);
		}
		else if (type == SPEW_WARNING)
		{
			old = Spew_SetConsoleTextColor(1, 1, 0, 1);
		}
		else if (type == SPEW_SUCCESS)
		{
			old = Spew_SetConsoleTextColor(0, 1, 0, 1);
		}
		else if (type == SPEW_ERROR)
		{
			old = Spew_SetConsoleTextColor(1, 0, 0, 1);
		}
		else if (type == SPEW_INFO)
		{
			old = Spew_SetConsoleTextColor(0, 1, 1, 1);
		}
		else
		{
			old = Spew_SetConsoleTextColor(1, 1, 1, 1);
		}

		OutputDebugStringA(text);
		printf("%s", text);

		Spew_RestoreConsoleTextColor(old);
	}
	LeaveCriticalSection(&g_SpewCS);
}
#endif

void PrintMessageToOutput(SpewType_t spewtype, char const* pMsgFormat, va_list args)
{
	static char pTempBuffer[4096];
	int len = 0;
	vsprintf(&pTempBuffer[len], pMsgFormat, args);

#ifdef WIN32
	Spew_ConDebugSpew(spewtype, pTempBuffer);
#elif defined(__EMSCRIPTEN__)
	if (spewtype == SPEW_INFO)
	{
		EM_ASM({
			console.info(UTF8ToString($0));
		}, pTempBuffer);
	}
	else if (spewtype == SPEW_WARNING)
	{
		EM_ASM({
			console.warn(UTF8ToString($0));
		}, pTempBuffer);
	}
	else if (spewtype == SPEW_ERROR)
	{
		EM_ASM({
			console.error(UTF8ToString($0));
		}, pTempBuffer);
	}
	else
	{
		EM_ASM({
			console.log(UTF8ToString($0));
		}, pTempBuffer);
	}
#else
	printf(pTempBuffer);
#endif

	if(g_logStream)
		fprintf(g_logStream, pTempBuffer);
}

void PsyX_Log(const char* fmt, ...)
{
	va_list		argptr;

	va_start(argptr, fmt);
	PrintMessageToOutput(SPEW_NORM, fmt, argptr);
	va_end(argptr);
}

void PsyX_Log_Info(const char* fmt, ...)
{
	va_list		argptr;

	va_start(argptr, fmt);
	PrintMessageToOutput(SPEW_INFO, fmt, argptr);
	va_end(argptr);
}

void PsyX_Log_Warning(const char* fmt, ...)
{
	va_list		argptr;

	va_start(argptr, fmt);
	PrintMessageToOutput(SPEW_WARNING, fmt, argptr);
	va_end(argptr);
}

void PsyX_Log_Error(const char* fmt, ...)
{
	va_list		argptr;

	va_start(argptr, fmt);
	PrintMessageToOutput(SPEW_ERROR, fmt, argptr);
	va_end(argptr);
}

void PsyX_Log_Success(const char* fmt, ...)
{
	va_list		argptr;

	va_start(argptr, fmt);
	PrintMessageToOutput(SPEW_SUCCESS, fmt, argptr);
	va_end(argptr);
}


void PsyX_Initialise(char* appName, int width, int height, int fullscreen)
{
	char windowNameStr[128];

	g_appNameStr = appName;

	InstallExceptionHandler();

	PsyX_Log_Initialise();
	PsyX_GetWindowName(windowNameStr);

#if defined(_WIN32) && defined(_DEBUG)
	if (AllocConsole())
	{
		freopen("CONOUT$", "w", stdout);
		SetConsoleTitleA(windowNameStr);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
	}
#endif

	eprintinfo("Initialising Psy-X %d.%d\n", PSYX_MAJOR_VERSION, PSYX_MINOR_VERSION);
	eprintinfo("Build date: %s:%s\n", PSYX_COMPILE_DATE, PSYX_COMPILE_TIME);

#if defined(__EMSCRIPTEN__)
	SDL_SetHint(SDL_HINT_EMSCRIPTEN_ASYNCIFY, "0");
#endif
	
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		eprinterr("Failed to initialise SDL\n");
		PsyX_Shutdown();
		return;
	}
	
	if (!GR_InitialiseRender(windowNameStr, width, height, fullscreen))
	{
		eprinterr("Failed to Intialise Window\n");
		PsyX_Shutdown();
		return;
	}

	if (!PsyX_Sys_InitialiseCore())
	{
		eprinterr("Failed to Intialise Psy-X Core.\n");
		PsyX_Shutdown();
		return;
	}

	if (!GR_InitialisePSX())
	{
		eprinterr("Failed to Intialise PSX.\n");
		PsyX_Shutdown();
		return;
	}

	PsyX_Sys_InitialiseInput();

	// set shutdown function (PSX apps usualy don't exit)
	atexit(PsyX_Shutdown);

	// disable cursor visibility
	SDL_ShowCursor(0);
}

void PsyX_GetScreenSize(int* screenWidth, int* screenHeight)
{
	SDL_GetWindowSize(g_window, screenWidth, screenHeight);
}

void PsyX_SetCursorPosition(int x, int y)
{
	SDL_WarpMouseInWindow(g_window, x, y);
}

extern void PsyX_SetCursorRelative(int enable)
{
	SDL_SetRelativeMouseMode((SDL_bool)enable);
}

void PsyX_Sys_DoDebugKeys(int nKey, char down); // forward decl
void PsyX_Sys_DoDebugMouseMotion(int x, int y, int dx, int dy);

void PsyX_Exit();

int g_activeKeyboardControllers = 0x1;
int g_altKeyState = 0;

void PsyX_Sys_DoPollEvent()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_CONTROLLERDEVICEADDED:
				PsyX_Pad_Event_ControllerAdded(event.cdevice.which);
				break;
			case SDL_CONTROLLERDEVICEREMOVED:
				PsyX_Pad_Event_ControllerRemoved(event.cdevice.which);
				break;
			case SDL_QUIT:
				PsyX_Exit();
				break;
			case SDL_WINDOWEVENT:
				switch (event.window.event)
				{
				case SDL_WINDOWEVENT_RESIZED:
				case SDL_WINDOWEVENT_SIZE_CHANGED:
					GR_ResetDevice();	// Driver 1 port: the display layout follows the window (scene size, bars)
					break;
				case SDL_WINDOWEVENT_CLOSE:
					PsyX_Exit();
					break;
				}
				break;
			case SDL_MOUSEMOTION:

				PsyX_Sys_DoDebugMouseMotion(event.motion.x, event.motion.y, event.motion.xrel, event.motion.yrel);
				break;
			case SDL_KEYDOWN:
			case SDL_KEYUP:
			{
				int nKey = event.key.keysym.scancode;

				if (nKey == SDL_SCANCODE_RALT)
				{
					g_altKeyState = (event.type == SDL_KEYDOWN);
				}
				else if (nKey == SDL_SCANCODE_RETURN)
				{
					if (g_altKeyState && event.type == SDL_KEYDOWN)
					{
						int fullscreen = SDL_GetWindowFlags(g_window) & SDL_WINDOW_FULLSCREEN > 0;

						SDL_SetWindowFullscreen(g_window, fullscreen ? 0 : SDL_WINDOW_FULLSCREEN_DESKTOP);

						GR_ResetDevice();
					}
					break;
				}

				// lshift/right shift
				if (nKey == SDL_SCANCODE_RSHIFT)
					nKey = SDL_SCANCODE_LSHIFT;
				else if (nKey == SDL_SCANCODE_RCTRL)
					nKey = SDL_SCANCODE_LCTRL;
				else if (nKey == SDL_SCANCODE_RALT)
					nKey = SDL_SCANCODE_LALT;

				if (g_cfg_gameOnTextInput && nKey == SDL_SCANCODE_BACKSPACE && event.type == SDL_KEYDOWN)
				{
					(g_cfg_gameOnTextInput)(NULL);
				}

				PsyX_Sys_DoDebugKeys(nKey, (event.type == SDL_KEYUP) ? 0 : 1);
				break;
			}
			case SDL_TEXTINPUT:
			{
				if(g_cfg_gameOnTextInput)
					(g_cfg_gameOnTextInput)(event.text.text);
				break;
			}			
		}
	}
}

char begin_scene_flag = 0;

char PsyX_BeginScene()
{
	PsyX_Sys_DoPollEvent();

	if (begin_scene_flag)
		return 0;

	assert(!begin_scene_flag);

	{
		int swapInterval = (g_cfg_swapInterval && g_enableSwapInterval && !g_skipSwapInterval) ? g_swapInterval : 0;

		// Maximum is (ScreenRefreshRate / 2). 
		// If our screen refresh rate is lower than our PSX vmode refresh rate, 
		// we reducing swap interval to maintain the framerate.
		// Example:
		//		target 60fps, 50hz screen = no interval (tearing)
		//		target 30fps, 50hz screen = 60hz interval (less tearing)
		//		target 30fps, 60hz screen = 30hz interval (no tearing)
		SDL_DisplayMode curMode;
		if (SDL_GetWindowDisplayMode(g_window, &curMode) == 0)
		{
			const int mode_frequency = g_vmode == MODE_NTSC ? VBLANK_FREQUENCY_NTSC : VBLANK_FREQUENCY_PAL;
			if (curMode.refresh_rate < mode_frequency)
				swapInterval--;
		}

		if (swapInterval < 0)
			swapInterval = 0;
		
		GR_UpdateSwapIntervalState(swapInterval);
	}

	GR_BeginScene();

	if (activeDrawEnv.isbg)
	{
		const RECT16 clipenv = activeDrawEnv.clip;
		const u_char r = activeDrawEnv.r0;
		const u_char g = activeDrawEnv.g0;
		const u_char b = activeDrawEnv.b0;

		// TODO: clear all affected backbuffers
		//GR_ClearVRAM(clipenv.x, clipenv.y, clipenv.w, clipenv.h, r, g, b);
		GR_Clear(clipenv.x, clipenv.y, clipenv.w, clipenv.h, r, g, b);
	}

	begin_scene_flag = 1;
	g_sceneBeginVbl = g_psxSysCounters[PsxCounter_VBLANK];

	PsyX_Log_Flush();

	return 1;
}

uint PsyX_CalcFPS();

/* Driver 1 port, test aids at present time (environment):
 *   D1_WINSHOT=n1,n2,..  writes win_<n>.ppm, the whole window as presented, at the first present at/after vblank n
 *   D1_VBLSTAT=1         every 10 s: the vblank clock (intrThreadMain) and the present cadence - how many presents
 *                        came 1, 2, 3, 4+ vblanks after the previous one, and how many shared a vblank with it */
static void PsyX_PresentDebug(int swapped)
{
	static int init = 0, stats = 0, shotLast = -1;
	static const char* shots = NULL;
	static int lastVbl = -1, hist[6], count = 0;
	static long long swapStart, swapSum, swapMax, latSum, latMax;

	if (swapped)
	{
		if (stats)
		{
			const long long now = VblNowNs(), d = now - swapStart, lat = swapStart - g_vblLastNs;
			swapSum += d;
			if (d > swapMax)
				swapMax = d;
			latSum += lat;
			if (lat > latMax)
				latMax = lat;
		}
		return;
	}

	if (!init)
	{
		init = 1;
		shots = getenv("D1_WINSHOT");
		stats = getenv("D1_VBLSTAT") && atoi(getenv("D1_VBLSTAT"));
	}

	const int vbl = g_psxSysCounters[PsxCounter_VBLANK];

	if (stats)
		swapStart = VblNowNs();

	if (shots)
	{
		int best = -1;
		for (const char* p = shots; p && *p; )
		{
			const int n = atoi(p);
			if (n > shotLast && n <= vbl && (best < 0 || n < best))
				best = n;
			p = strchr(p, ',');
			p = p ? p + 1 : NULL;
		}
		if (best >= 0)
		{
			char name[64];
			snprintf(name, sizeof(name), "win_%d.ppm", best);
			GR_DumpWindow(name);
			eprintinfo("vblank %d: %s (%dx%d)\n", vbl, name, g_displayWidth, g_displayHeight);
			shotLast = best;
		}
	}

	if (stats)
	{
		if (lastVbl >= 0)
		{
			int d = vbl - lastVbl;
			hist[d < 0 ? 0 : d > 4 ? 5 : d]++;
		}
		lastVbl = vbl;
		if (++count == 600)
		{
			eprintinfo("presents: same vblank %d, +1 %d, +2 %d, +3 %d, +4.. %d; vblank->present avg %lld max %lld us; swap avg %lld max %lld us\n",
				hist[0], hist[1], hist[2], hist[3], hist[4] + hist[5], latSum / 600 / 1000, latMax / 1000, swapSum / 600 / 1000, swapMax / 1000);
			memset(hist, 0, sizeof(hist));
			count = 0;
			swapSum = swapMax = latSum = latMax = 0;
		}
	}
}

void PsyX_EndScene()
{
	if (!begin_scene_flag)
		return;

	assert(begin_scene_flag);
	begin_scene_flag = 0;

#if USE_PGXP
	PGXP_ClearCache();
#endif

	GR_EndScene();
	
	GR_StoreFrameBuffer(activeDispEnv.disp.x, activeDispEnv.disp.y, activeDispEnv.disp.w, activeDispEnv.disp.h);

	// Driver 1 port: the scene goes into the window with the display options (PsyX_render.cpp "Display")
	GR_PresentScene();
	PsyX_PresentDebug(0);

	GR_SwapWindow();
	PsyX_PresentDebug(1);

	GR_ResumeScene();
	
	SDL_Delay(0);
}

#if !defined(__EMSCRIPTEN__) && !defined(__ANDROID__)
void PsyX_TakeScreenshot()
{
	// Driver 1 port: the scene framebuffer (g_windowWidth x g_windowHeight) is bound between frames
	u_char* pixels = (u_char*)malloc(g_windowWidth * g_windowHeight * 4);
	
#if defined(RENDERER_OGL)
	glReadPixels(0, 0, g_windowWidth, g_windowHeight, GL_BGRA, GL_UNSIGNED_BYTE, pixels);
#elif defined(RENDERER_OGLES)
	glReadPixels(0, 0, g_windowWidth, g_windowHeight, GL_RGBA, GL_UNSIGNED_BYTE, pixels);	// FIXME: is that correct format?
#endif

	SDL_Surface* surface = SDL_CreateRGBSurfaceFrom(pixels, g_windowWidth, g_windowHeight, 8 * 4, g_windowWidth * 4, 0, 0, 0, 0);

	SDL_SaveBMP(surface, "SCREENSHOT.BMP");
	SDL_FreeSurface(surface);

	free(pixels);
}
#endif

void PsyX_Sys_DoDebugMouseMotion(int x, int y, int dx, int dy)
{
	if (g_dbg_gameDebugMouse)
		g_dbg_gameDebugMouse(x, y, dx, dy);
}

void PsyX_Sys_DoDebugKeys(int nKey, char down)
{
	if (g_dbg_gameDebugKeys)
		g_dbg_gameDebugKeys(nKey, down);

#if 1 //def _DEBUG
	if (nKey == SDL_SCANCODE_BACKSPACE)
	{
		if (down)
			g_skipSwapInterval = 1;
		else
			g_skipSwapInterval = 0;
	}
#endif

	if (!down)
	{
		switch (nKey)
		{
#ifdef _DEBUG
		case SDL_SCANCODE_F1:
			g_dbg_wireframeMode ^= 1;
			eprintwarn("wireframe mode: %d\n", g_dbg_wireframeMode);
			break;

		case SDL_SCANCODE_F2:
			g_dbg_texturelessMode ^= 1;
			eprintwarn("textureless mode: %d\n", g_dbg_texturelessMode);
			break;
		case SDL_SCANCODE_UP:
		case SDL_SCANCODE_DOWN:
			if (g_dbg_emulatorPaused)
			{
				g_dbg_polygonSelected += (nKey == SDL_SCANCODE_UP) ? 3 : -3;
			}
			break;
		case SDL_SCANCODE_F10:
			eprintwarn("saving VRAM.TGA\n");
			GR_SaveVRAM("VRAM.TGA", 0, 0, VRAM_WIDTH, VRAM_HEIGHT, 1);
			break;
#endif
#if !defined(__EMSCRIPTEN__) && !defined(__ANDROID__)
		case SDL_SCANCODE_F12:
			eprintwarn("Saving screenshot...\n");
			PsyX_TakeScreenshot();
			break;
#endif
		case SDL_SCANCODE_F3:
			g_cfg_bilinearFiltering ^= 1;
			eprintwarn("filtering mode: %d\n", g_cfg_bilinearFiltering);
			break;
		case SDL_SCANCODE_F4:

			g_activeKeyboardControllers++;
			g_activeKeyboardControllers = g_activeKeyboardControllers % 4;

			if (g_activeKeyboardControllers == 0)
				g_activeKeyboardControllers++;

			eprintwarn("Active keyboard controller: %d\n", g_activeKeyboardControllers);
			break;
		case SDL_SCANCODE_F5:
			g_cfg_pgxpTextureCorrection ^= 1;
			break;
		case SDL_SCANCODE_F6:
			g_cfg_pgxpZBuffer ^= 1;
			break;
		}
	}
}

void PsyX_UpdateInput()
{
	// also poll events here
	PsyX_Sys_DoPollEvent();

	if(!g_altKeyState)
		PsyX_Pad_InternalPadUpdates();
}

uint PsyX_CalcFPS()
{
#define FPS_INTERVAL 1.0

	static unsigned int lastTime = 0;
	static unsigned int currentFps = 0;
	static unsigned int passedFrames = 0;

	lastTime = SDL_GetTicks();

	passedFrames++;
	if (lastTime < SDL_GetTicks() - FPS_INTERVAL * 1000)
	{
		lastTime = SDL_GetTicks();
		currentFps = passedFrames;
		passedFrames = 0;
	}

	return currentFps;
}

void PsyX_SetSwapInterval(int interval)
{
	g_swapInterval = interval;
}

void PsyX_EnableSwapInterval(int enable)
{
	g_enableSwapInterval = enable;
}

void PsyX_WaitForTimestep(int count)
{
#if 0 // defined(RENDERER_OGL) || defined(RENDERER_OGLES)
	glFinish(); // best time to complete GPU drawing
#endif

	// wait for vblank
	if (!g_skipSwapInterval)
	{	
		static int swapLastVbl = 0;

#ifdef __EMSCRIPTEN__
		int vbl;
		do
		{
			emscripten_sleep(0);
			vbl = PsyX_Sys_GetVBlankCount();
		}
		while (vbl - swapLastVbl < count);
#else
		// Driver 1 port: sleep on the vblank thread's signal instead of spinning (same condition)
		SDL_LockMutex(g_vblMutex);
		while (g_psxSysCounters[PsxCounter_VBLANK] - swapLastVbl < count && !g_stopIntrThread)
			SDL_CondWaitTimeout(g_vblCond, g_vblMutex, 100);
		SDL_UnlockMutex(g_vblMutex);
#endif

		swapLastVbl = PsyX_Sys_GetVBlankCount();
	}
}

void PsyX_Exit()
{
	// SDL_QUIT (window close, Steam's "Exit game" / SIGTERM, which SDL turns into SDL_QUIT) from the game's event
	// poll: exit() runs PsyX_Shutdown (atexit), now safe from the game context (Driver 1 port, see there)
	exit(0);
}

extern "C" void PsyX_SPUAL_ShutdownSound();

void PsyX_Shutdown()
{
	static int done = 0;

	if (!g_window || done)
		return;

	done = 1;
	fflush(NULL);

	// Driver 1 port: an orderly teardown. It used to destroy the window first: SDL then unloaded the GL driver and
	// GR_Shutdown's glDelete* calls jumped into unmapped memory (segfault on every window close / SIGTERM; D1_QUIT
	// used _exit to avoid it). Now: stop the vblank thread (no VSyncCallback - the XM player - runs into the SPU
	// while it goes away), close audio, free the GL objects while the context is current, then context and window.

	// quit vblank thread
	if (g_intrThread)
	{
		g_stopIntrThread = 1;

		int returnValue;
		SDL_WaitThread(g_intrThread, &returnValue);
		g_intrThread = NULL;
	}

	PsyX_SPUAL_ShutdownSound();

	GR_Shutdown();

	SDL_GLContext ctx = SDL_GL_GetCurrentContext();
	if (ctx)
	{
		SDL_GL_MakeCurrent(g_window, NULL);
		SDL_GL_DeleteContext(ctx);
	}

	SDL_DestroyWindow(g_window);
	g_window = NULL;

	SDL_QuitSubSystem(SDL_INIT_GAMECONTROLLER);

	SDL_Quit();

	UnInstallExceptionHandler();

	PsyX_Log_Finalise();
}
