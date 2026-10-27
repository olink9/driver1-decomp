/* Driver 1 PC port - gameplay mods (port/README.md "Mods"). PC only: the PS1 build never sees this file.
 *
 * Settings persist in a small text file, driver1_mods.ini in the working directory (D1_MODS_INI overrides the
 * path), one `key=value` per line; unknown keys and `#` comments are kept out of the way (not rewritten).
 *
 * Mod 1 - skip the garage test ("the interview", IVIEW city, mission 0xAE) that opens a new Undercover career.
 * Key `skip_garage_test` (1 = skip, the default; 0 = play it as the original). Toggled in game on the
 * Undercover "New Game / Load Saved Game" screen (third entry, X to toggle). The skip itself is in main.c
 * func_00024754 case 0; the menu entry is added in frontend/newgame.c and drawn by the hook below.
 *
 * Mod 2 - display options (PsyCross, port/psycross/src/render/PsyX_render.cpp "Display"):
 *   display_mode   = fit | integer | fill | stretch   (default fit: 4:3 as large as fits, black bars)
 *   display_filter = linear | nearest                 (default linear; the upscale filter, used when the
 *                                                      picture is rendered smaller than it is shown)
 *   render_scale   = auto | 1..8                      (default auto: render at the size shown; N: N x the
 *                                                      PS1 display mode, 1 = its own pixel grid)
 * Same screen, entries 4-6 ("Screen", "Filter", "Resolution"): X or right steps forward, left back; applied at the
 * next frame and saved at once. PsyCross reads them at start-up through PC_ModDisplayInit (a weak hook). */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"
#include "frontend.h"

extern void PsyX_SetDisplayOptions(int mode, int filter, int renderScale);   /* PsyX_public.h (C++ side) */

extern MENU_TEXT D_001CE9E8[];          /* FRONTEND.BIN: the New Game / Load Saved Game labels */
extern void func_000E6F14(short x, short y, char font, char justify, unsigned char *rgb);
extern void func_000E6F98();

#define INI_DEFAULT "driver1_mods.ini"

static int s_loaded;
static int s_skip_garage = 1;
static int s_disp_mode = 0;          /* PSYX_DISPLAY_FIT, _INTEGER, _FILL, _STRETCH */
static int s_disp_filter = 1;        /* 1 linear, 0 nearest */
static int s_render_scale = 0;       /* 0 auto, 1..8 */

static const char *const k_mode_names[4] = { "fit", "integer", "fill", "stretch" };
#define RENDER_SCALE_MAX 4           /* the menu cycles auto, 1..4; the ini takes up to 8 */

static const char *ini_path(void)
{
    const char *p = getenv("D1_MODS_INI");
    return (p && *p) ? p : INI_DEFAULT;
}

static void mods_load(void)
{
    FILE *f;
    char line[256];

    if (s_loaded)
        return;
    s_loaded = 1;
    f = fopen(ini_path(), "r");
    if (!f) {
        printf("[mods] %s not found: defaults (skip_garage_test=%d)\n", ini_path(), s_skip_garage);
        return;
    }
    while (fgets(line, sizeof line, f)) {
        char *k = line, *v, *e;
        while (*k == ' ' || *k == '\t')
            k++;
        if (*k == '#' || *k == ';' || !(v = strchr(k, '=')))
            continue;
        *v++ = 0;
        for (e = v + strlen(v); e > v && (e[-1] == '\n' || e[-1] == '\r' || e[-1] == ' '); )
            *--e = 0;
        for (e = k + strlen(k); e > k && (e[-1] == ' ' || e[-1] == '\t'); )
            *--e = 0;
        if (!strcmp(k, "skip_garage_test"))
            s_skip_garage = atoi(v) != 0;
        else if (!strcmp(k, "display_mode")) {
            int i;
            for (i = 0; i < 4; i++)
                if (!strcmp(v, k_mode_names[i]) || (v[0] == '0' + i && !v[1]))
                    s_disp_mode = i;
        } else if (!strcmp(k, "display_filter"))
            s_disp_filter = !(strcmp(v, "nearest") == 0 || strcmp(v, "0") == 0);
        else if (!strcmp(k, "render_scale")) {
            int n = atoi(v);
            s_render_scale = (n >= 1 && n <= 8) ? n : 0;
        }
    }
    fclose(f);
    printf("[mods] %s: skip_garage_test=%d display_mode=%s display_filter=%s render_scale=%d\n", ini_path(),
           s_skip_garage, k_mode_names[s_disp_mode], s_disp_filter ? "linear" : "nearest", s_render_scale);
}

static void mods_save(void)
{
    FILE *f = fopen(ini_path(), "w");
    if (!f) {
        printf("[mods] cannot write %s\n", ini_path());
        return;
    }
    fprintf(f, "# Driver 1 PC port mods (port/README.md \"Mods\"); also set in game:\n"
               "# Undercover > New Game screen, \"Skip Garage Test\", \"Screen\", \"Filter\", \"Resolution\".\n"
               "skip_garage_test=%d\n"
               "# display_mode: fit (4:3, bars) | integer (4:3, whole multiples of 320x240) | fill (4:3, cropped) | stretch\n"
               "display_mode=%s\n"
               "# display_filter: linear | nearest (upscaling, when render_scale is not auto)\n"
               "display_filter=%s\n"
               "# render_scale: auto (the size shown) | 1..8 (x the PS1 display mode: 1 = PS1 pixels)\n",
            s_skip_garage, k_mode_names[s_disp_mode], s_disp_filter ? "linear" : "nearest");
    if (s_render_scale)
        fprintf(f, "render_scale=%d\n", s_render_scale);
    else
        fprintf(f, "render_scale=auto\n");
    fclose(f);
    printf("[mods] saved %s\n", ini_path());
}

int PC_ModSkipGarage(void)
{
    mods_load();
    return s_skip_garage;
}

/* PsyCross's renderer calls this once at start-up (weak reference in PsyX_render.cpp GR_InitialisePSX) */
void PC_ModDisplayInit(void)
{
    mods_load();
    PsyX_SetDisplayOptions(s_disp_mode, s_disp_filter, s_render_scale);
}

/* ---- the menu entries: MENU_ITEM/MENU_TEXTs after the screen's own two (newgame.c) ----
 * Items 0-1 are the screen's (New Game, Load Saved Game), 2 Skip Garage Test, 3 Screen, 4 Filter, 5 Resolution,
 * in a vertical ring. The game's menu driver (fecore.c func_001C26E4) handles left/right by calling enter() of the
 * item named in the current item's left/right field without selecting it: items 6.. are such "phantoms", one per
 * option and direction, whose enter() steps the option (they are never selected, so they draw nothing). */
#define NG_OWN    2
#define NG_OPTS   4                                  /* skip garage, screen, filter, resolution */
#define NG_ITEMS  (NG_OWN + NG_OPTS + 2 * NG_OPTS)

static char s_label[NG_OPTS][40];
static MENU_TEXT s_text[NG_OPTS];
static MENU_ITEM s_items[NG_ITEMS];

static void ng_labels(void)
{
    static const char *const mode[4] = { "4:3", "4:3 Integer", "Zoom (crop)", "Stretch" };
    mods_load();
    sprintf(s_label[0], "Skip Garage Test: %s", s_skip_garage ? "On" : "Off");
    sprintf(s_label[1], "Screen: %s", mode[s_disp_mode]);
    sprintf(s_label[2], "Filter: %s", s_disp_filter ? "Smooth" : "Sharp");
    if (s_render_scale)
        sprintf(s_label[3], "Resolution: %dx%s", s_render_scale, s_render_scale == 1 ? " (PS1)" : "");
    else
        sprintf(s_label[3], "Resolution: Full");
}

static void ng_step(int opt, int dir)
{
    mods_load();
    switch (opt) {
    case 0:
        s_skip_garage = !s_skip_garage;
        break;
    case 1:
        s_disp_mode = (s_disp_mode + dir + 4) % 4;
        break;
    case 2:
        s_disp_filter = !s_disp_filter;
        break;
    case 3:
        s_render_scale = (s_render_scale > RENDER_SCALE_MAX ? 0 : s_render_scale + dir + RENDER_SCALE_MAX + 1)
                         % (RENDER_SCALE_MAX + 1);
        break;
    }
    if (opt)
        PsyX_SetDisplayOptions(s_disp_mode, s_disp_filter, s_render_scale);
    ng_labels();
    mods_save();
}

/* per-option handlers (MENU_ITEM takes plain function pointers) */
#define NG_OPT_FUNCS(k) \
    static void ng_enter_##k(void) { s_text[k].font = 1; } \
    static void ng_leave_##k(void) { s_text[k].font = 0; } \
    static int ng_action_##k(void) { ng_step(k, 1); return 0; /* stay on the screen */ } \
    static void ng_prev_##k(void) { ng_step(k, -1); } \
    static void ng_next_##k(void) { ng_step(k, 1); }
NG_OPT_FUNCS(0)
NG_OPT_FUNCS(1)
NG_OPT_FUNCS(2)
NG_OPT_FUNCS(3)

static void (*const k_enter[NG_OPTS])(void) = { ng_enter_0, ng_enter_1, ng_enter_2, ng_enter_3 };
static void (*const k_leave[NG_OPTS])(void) = { ng_leave_0, ng_leave_1, ng_leave_2, ng_leave_3 };
static int (*const k_action[NG_OPTS])(void) = { ng_action_0, ng_action_1, ng_action_2, ng_action_3 };
static void (*const k_prev[NG_OPTS])(void) = { ng_prev_0, ng_prev_1, ng_prev_2, ng_prev_3 };
static void (*const k_next[NG_OPTS])(void) = { ng_next_0, ng_next_1, ng_next_2, ng_next_3 };

/* The New Game / Load Saved Game items (FRONTEND.BIN D_001CEA18, reloaded with the overlay: copied each time) plus
 * ours, linked in a vertical ring 0 -> 1 -> .. -> 5 -> 0. Left/right on the original two stay put, as before. */
MENU_ITEM *PC_ModNewGameItems(MENU_ITEM *orig)
{
    const int last = NG_OWN + NG_OPTS - 1;
    int i;
    s_items[0] = orig[0];
    s_items[1] = orig[1];
    for (i = 0; i <= last; i++) {
        s_items[i].up = i == 0 ? last : i - 1;
        s_items[i].down = i == last ? 0 : i + 1;
    }
    for (i = 0; i < NG_OPTS; i++) {
        MENU_ITEM *m = &s_items[NG_OWN + i];
        MENU_ITEM *prev = &s_items[NG_OWN + NG_OPTS + 2 * i], *next = prev + 1;
        m->enter = (void (*)())k_enter[i];
        m->leave = (void (*)())k_leave[i];
        m->action = (int (*)())k_action[i];
        m->left = NG_OWN + NG_OPTS + 2 * i;
        m->right = m->left + 1;
        *prev = *m;                                  /* phantoms: only enter() is ever called */
        *next = *m;
        prev->enter = (void (*)())k_prev[i];
        next->enter = (void (*)())k_next[i];
        s_text[i].x = 320;
        s_text[i].y = 280 + 32 * i;
        s_text[i].font = 0;                          /* newgame.c leaves only its first NG_COUNT items */
        s_text[i].justify = 2;
        s_text[i].text = s_label[i];
        s_text[i].col.r = s_text[i].col.g = s_text[i].col.b = s_text[i].col.cd = 0xFF;
    }
    ng_labels();
    return s_items;
}

/* Called by the menu-label drawer (fecore.c func_001C1BD4 and the slide-in copy in wips_pc.c func_001C2114)
 * after it drew a label list: on the New Game screen, draw our labels the same way. */
void PC_ModMenuExtra(void *tbl)
{
    int i;
    if (tbl != (void *)D_001CE9E8 || !s_label[0][0])
        return;
    for (i = 0; i < NG_OPTS; i++) {
        func_000E6F14(s_text[i].x, s_text[i].y, s_text[i].font, s_text[i].justify, (unsigned char *)&s_text[i].col);
        func_000E6F98(s_text[i].text);
    }
}
