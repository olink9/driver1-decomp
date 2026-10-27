#include "psx/libetc.h"

#include "../PsyX_main.h"
#include "PsyX/PsyX_public.h"

#include <SDL_timer.h>

#ifdef DEBUG
char scratchData[4096 + 8];
char* _scratchData = scratchData + 4;
#else
char scratchData[4096];
char* _scratchData = scratchData;
#endif

void(*vsync_callback)(void) = NULL;

int StopCallback(void)
{
	PSYX_UNIMPLEMENTED();
	return 0;
}

int ResetCallback(void)
{
	int old = (int)vsync_callback;
	vsync_callback = NULL;
	return old;
}

int VSync(int mode)
{
	if (mode < 0)
	{
		// don't wait but still return vblank count
		return PsyX_Sys_GetVBlankCount();
	}

	if (mode == 0)
	{
		PsyX_WaitForTimestep(1);
	}
	else if (mode > 1)
	{
		// Driver 1 port: VSync(n), n > 1, blocks until n vblanks have passed since the last VSync(0)/VSync(n)
		// returned (PsyQ libetc), the same reference PsyX_WaitForTimestep keeps. Driver: xaplay.c's XA
		// fade-out (func_0006F4D8: VSync(2) per volume step) ran in no time without it.
		PsyX_WaitForTimestep(mode);
	}
	// mode 1 never blocks: it returns the time since that last VSync (PsyQ: in hblanks; here the vblank
	// count, as before - Driver only uses it for xmplay.c's D_00096FA0 timing statistic, on the interrupt thread)

	return PsyX_Sys_GetVBlankCount();
}

int VSyncCallback(void(*f)(void))
{
	int old = (int)vsync_callback;
	vsync_callback = f;
	return old;
}

int SetVideoMode(int mode)
{
#ifdef DEBUG
	// debug marks for overflow cheks
	*(uint*)&scratchData[0] = 0xdeadb33f;
	*(uint*)&scratchData[4096 + 4] = 0xdeadb33f;
#endif
	
	return PsyX_Sys_SetVMode(mode);
}

int GetVideoMode()
{
	return g_vmode;
}

void PadInit(int mode)
{
	PSYX_UNIMPLEMENTED();

	// TODO: call PadInitDirect
}

u_int PadRead(int id)
{
	PSYX_UNIMPLEMENTED();
	
	// TODO: return pad data as u_int
	return 0;
}

void PadStop(void)
{
	PSYX_UNIMPLEMENTED();

	// TODO: stop pad reads
}