/* Driver 1 PC port - gameplay mods (port/README.md "Mods"). PC only: the PS1 build never sees this file.
 *
 * Settings persist in a small text file, driver1_mods.ini in the working directory (D1_MODS_INI overrides the
 * path), one `key=value` per line; unknown keys and `#` comments are kept out of the way (not rewritten).
 *
 * Mod 1 - skip the garage test ("the interview", IVIEW city, mission 0xAE) that opens a new Undercover career.
 * Key `skip_garage_test` (1 = skip, the default; 0 = play it as the original). Toggled in game on the
 * Undercover "New Game / Load Saved Game" screen (third entry, X to toggle). The skip itself is in main.c
 * func_00024754 case 0; the menu entry is added in frontend/newgame.c and drawn by the hook below. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"
#include "frontend.h"

extern MENU_TEXT D_001CE9E8[];          /* FRONTEND.BIN: the New Game / Load Saved Game labels */
extern void func_000E6F14(short x, short y, char font, char justify, unsigned char *rgb);
extern void func_000E6F98();

#define INI_DEFAULT "driver1_mods.ini"

static int s_loaded;
static int s_skip_garage = 1;

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
    }
    fclose(f);
    printf("[mods] %s: skip_garage_test=%d\n", ini_path(), s_skip_garage);
}

static void mods_save(void)
{
    FILE *f = fopen(ini_path(), "w");
    if (!f) {
        printf("[mods] cannot write %s\n", ini_path());
        return;
    }
    fprintf(f, "# Driver 1 PC port mods (port/README.md \"Mods\"); also set in game:\n"
               "# Undercover > New Game screen, \"Skip Garage Test\".\n"
               "skip_garage_test=%d\n", s_skip_garage);
    fclose(f);
    printf("[mods] saved %s: skip_garage_test=%d\n", ini_path(), s_skip_garage);
}

int PC_ModSkipGarage(void)
{
    mods_load();
    return s_skip_garage;
}

/* ---- the menu entry: a third MENU_ITEM/MENU_TEXT next to the screen's own two (newgame.c) ---- */
static char s_label[32];
static MENU_TEXT s_text = { 320, 280, 0, 2, 0, s_label, { 0xFF, 0xFF, 0xFF, 0xFF } };
static MENU_ITEM s_items[3];

static void ng_label(void)
{
    sprintf(s_label, "Skip Garage Test: %s", PC_ModSkipGarage() ? "On" : "Off");
}

static void ng_enter(void) { s_text.font = 1; }
static void ng_leave(void) { s_text.font = 0; }
static int ng_action(void)
{
    s_skip_garage = !PC_ModSkipGarage();
    ng_label();
    mods_save();
    return 0;    /* stay on the screen */
}

/* The New Game / Load Saved Game items (FRONTEND.BIN D_001CEA18, reloaded with the overlay: copied each time) plus
 * ours, linked in a vertical ring 0 -> 1 -> 2 -> 0. Left/right stay put, as on the original two. */
MENU_ITEM *PC_ModNewGameItems(MENU_ITEM *orig)
{
    s_items[0] = orig[0];
    s_items[1] = orig[1];
    s_items[0].up = 2; s_items[0].down = 1;
    s_items[1].up = 0; s_items[1].down = 2;
    s_items[2].enter = (void (*)())ng_enter;
    s_items[2].leave = (void (*)())ng_leave;
    s_items[2].action = (int (*)())ng_action;
    s_items[2].left = 2; s_items[2].right = 2; s_items[2].up = 1; s_items[2].down = 0;
    ng_label();
    return s_items;
}

/* Called by the menu-label drawer (fecore.c func_001C1BD4 and the slide-in copy in wips_pc.c func_001C2114)
 * after it drew a label list: on the New Game screen, draw our label the same way. */
void PC_ModMenuExtra(void *tbl)
{
    if (tbl != (void *)D_001CE9E8 || !s_label[0])
        return;
    func_000E6F14(s_text.x, s_text.y, s_text.font, s_text.justify, (unsigned char *)&s_text.col);
    func_000E6F98(s_text.text);
}
