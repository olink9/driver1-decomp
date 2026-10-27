#ifndef EMULATOR_PUBLIC_H
#define EMULATOR_PUBLIC_H

#define CONTROLLER_MAP_FLAG_AXIS		0x4000
#define CONTROLLER_MAP_FLAG_INVERSE		0x8000

typedef struct
{
	int id;

	int kc_square, kc_circle, kc_triangle, kc_cross;

	int kc_l1, kc_l2, kc_l3;
	int kc_r1, kc_r2, kc_r3;

	int kc_start, kc_select;

	int kc_dpad_left, kc_dpad_right, kc_dpad_up, kc_dpad_down;
} PsyXKeyboardMapping;

typedef struct
{
	int id;

	// you can bind axis by adding CONTROLLER_MAP_AXIS_FLAG
	int gc_square, gc_circle, gc_triangle, gc_cross;

	int gc_l1, gc_l2, gc_l3;
	int gc_r1, gc_r2, gc_r3;

	int gc_start, gc_select;

	int gc_dpad_left, gc_dpad_right, gc_dpad_up, gc_dpad_down;

	int gc_axis_left_x, gc_axis_left_y;
	int gc_axis_right_x, gc_axis_right_y;
} PsyXControllerMapping;

typedef void(*GameDebugKeysHandlerFunc)(int nKey, char down);
typedef void(*GameDebugMouseHandlerFunc)(int x, int y, int dx, int dy);
typedef void(*GameOnTextInputHandler)(const char* buf);

//------------------------------------------------------------------------

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif

/* Mapped inputs */
extern PsyXControllerMapping		g_cfg_controllerMapping;
extern PsyXKeyboardMapping			g_cfg_keyboardMapping;
extern int							g_cfg_controllerToSlotMapping[2];

/* Game inputs */
extern GameOnTextInputHandler		g_cfg_gameOnTextInput;

/* Graphics configuration */
extern int							g_cfg_swapInterval;
extern int							g_cfg_pgxpZBuffer;
extern int							g_cfg_bilinearFiltering;
extern int							g_cfg_pgxpTextureCorrection;

/* Driver 1 port: display options (PsyX_render.cpp "Display"). Set them with PsyX_SetDisplayOptions. */
#define PSYX_DISPLAY_FIT		0	/* 4:3, as large as fits, centred (bars) */
#define PSYX_DISPLAY_INTEGER	1	/* 4:3 at a whole multiple of 320x240 */
#define PSYX_DISPLAY_FILL		2	/* 4:3 covering the window, overhang cropped */
#define PSYX_DISPLAY_STRETCH	3	/* the whole window */
extern int							g_cfg_displayMode;
extern int							g_cfg_displayFilter;	/* 1 linear, 0 nearest */
extern int							g_cfg_renderScale;		/* 0 = output size, N = N x 320x240 */

/* Debug inputs */
extern GameDebugKeysHandlerFunc		g_dbg_gameDebugKeys;
extern GameDebugMouseHandlerFunc	g_dbg_gameDebugMouse;

/* Usually called at the beginning of main function */
extern void PsyX_Initialise(char* windowName, int screenWidth, int screenHeight, int fullscreen);

/* Cleans all resources and closes open instances */
extern void PsyX_Shutdown(void);

/* Returns the screen size dimensions */
extern void PsyX_GetScreenSize(int* screenWidth, int* screenHeight);

/* Sets mouse cursor position */
extern void PsyX_SetCursorPosition(int x, int y);

/* Sets mouse relative movement */
extern void PsyX_SetCursorRelative(int enable);

/* Usually called after ClearOTag/ClearOTagR */
extern char PsyX_BeginScene(void);

/* Usually called after DrawOTag/DrawOTagEnv */
extern void PsyX_EndScene(void);

/* Explicitly updates emulator input loop */
extern void PsyX_UpdateInput(void);

/* Returns keyboard mapping index */
extern int PsyX_LookupKeyboardMapping(const char* str, int default_value);

/* Returns controller mapping index */
extern int PsyX_LookupGameControllerMapping(const char* str, int default_value);

/* Screen size of emulated PSX viewport with widescreen offsets */
extern void PsyX_GetPSXWidescreenMappedViewport(struct _RECT16* rect);

/* Waits for timer */
extern void PsyX_WaitForTimestep(int count);

/* Changes swap interval state */
extern void PsyX_EnableSwapInterval(int enable);

/* Changes swap interval interval interval */
extern void PsyX_SetSwapInterval(int interval);

/* Driver 1 port: display mode (PSYX_DISPLAY_*), upscale filter (1 linear, 0 nearest), render scale (0 = auto);
 * applied at the next frame */
extern void PsyX_SetDisplayOptions(int mode, int filter, int renderScale);

/* Driver 1 port: blocks until the vblank counter is no longer `last` (at most timeoutMs); returns the counter */
extern int PsyX_Sys_WaitVBlankChange(int last, int timeoutMs);

/* Driver 1 port: before presenting a pending scene - waits for the next vblank if the scene began in the current
 * one (presents then land on the vblank, as a PS1 flip does); returns the vblank count */
extern int PsyX_WaitPresentVBlank(void);

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif

#endif