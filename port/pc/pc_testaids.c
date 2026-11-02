/* pc_testaids.c - PC-only test aids (port/README.md "Test aids"); the PS1 build never sees them.
 *
 * D1_LADDER=<cmd>[:city[:flags]] / D1_MISSION=<n>: start the Undercover career at a chosen step of the ladder
 * (data\mladder.psx, ladder.c func_0006EB74). Hooked into main.c func_00024754 case 0 (Undercover -> New Game): the
 * new game then takes the path a loaded save takes - D_000967F0 = 1 with the save block's first four words
 * D_000A083C[0..3] = { unlocked cities, ladder command index, city, ladder flag bits } (what case 1 of
 * func_0006EB74 stores into D_000868DC[0..3] before every apartment screen and the memory card saves) - so the
 * ladder interpreter starts at that command with that city/flag state, exactly as it would on arriving there.
 * D1_MISSION=n picks the first command that plays mission n (ladder op 2). city/flags default to the state the
 * ladder has when it first reaches the command (a walk from command 0: op 0 sets the city, op 4 a flag bit,
 * op 1 offers its entries). D1_LADDER=list prints the decoded ladder (stderr) and starts nothing.
 * D1_LADDER_FMV=1: the ladder's movies play (sets D1_FMV=1 when the career starts), while the boot/intro movie of a
 * D1_PAD run stays skipped, so the menu script keeps its timing. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#define setenv(n, v, o) _putenv_s((n), (v))
#endif

extern int D_000A083C[];          /* the memory card's career block (frontend newgame.c/hiscores.c) */
extern int func_0006EACC(void);   /* ladder.c: loads data\mladder.psx to 0x1FE000 */
extern int LoadfileSeg(char *name, char *addr, int offset, int loadsize);   /* system.c */
extern int *func_000687D4(int *p);                                         /* mission.c: script op skip */
extern struct { short a, b; } D_0008C1E8[];                                /* mission.c: mission -> MISSION.BLK segment */
extern unsigned char D_000C6F04[];                                          /* mission.c: MISSION (0x2E8 bytes) */
extern int MapHeight(int *pos);

typedef struct { int op, a, b, c; } LCMD;
typedef struct { short x; unsigned short id; int val; } LENT;

#define LADDER_MAX 512

static const char *k_ops[] = { "city", "choice", "mission", "fmv", "setflag", "ifflag", "if_failed",
                               "goto", "ret_failed", "end_failed" };

/* ---- continuation missions (script op 89: "start where the last mission left the car") ----
 * The second parts (missions 53-64) start the player car at MISSION.saved (D_000C6F04 + 0x58: x, y, z, dir, ...),
 * which op 88 of the first part's script fills from car_data[0] when that part is passed. Started directly there is
 * no first part, so the test aid fills `saved` with the first part's own start (op 19 x/z, op 52 direction): a valid
 * spot in the same city (the real one is wherever the player finished part 1; D1_LADDER_POS=x,z[,dir] gives it).
 * The height is taken when the level is loaded (PC_TestLadderPlace, main.c: before func_000682C0 places the car),
 * with the formula the game's default start uses (civ_ai.c func_000148C4: MapHeight + 0xA4). */
static int s_place;

static int mission_script(int mission, int *buf, int max)   /* -> words read, 0 if none */
{
    int i, seg, v[2], n;
    for (i = 0; D_0008C1E8[i].a < mission && D_0008C1E8[i].a != 0x7FFF; i++)
        ;
    seg = D_0008C1E8[i].b - (D_0008C1E8[i].a - mission);
    if (seg < 0)
        return 0;
    LoadfileSeg("MISSIONS\\MISSION.BLK", (char *)v, seg * 4, 8);
    n = v[1] - v[0];
    if (n <= 0 || n > max * 4)
        return 0;
    return LoadfileSeg("MISSIONS\\MISSION.BLK", (char *)buf, v[0], n) / 4;
}

/* op 19 start x/z, op 52 direction, op 89 continuation: returns 1 if the script has op 89 */
static int scan_script(int mission, int *x, int *z, int *dir, int *has19)
{
    static int buf[0x4000];
    int n = mission_script(mission, buf, 0x4000), cont = 0;
    int *p = buf, *end = buf + n;
    *has19 = 0;
    while (p < end) {
        int *q;
        if (*p == 19) { *x = p[1]; *z = p[2]; *has19 = 1; }
        else if (*p == 52) *dir = p[1];
        else if (*p == 89) cont = 1;
        q = func_000687D4(p);
        if (q <= p)
            q = p + 1;
        p = q;
    }
    return cont;
}

/* the mission command whose success chain (op 9/3/7/4) reaches `target` with no mission between, or -1 */
static int previous_part(LCMD *cmds, int n, int target)
{
    int i, k, guard;
    for (i = 0; i < n; i++) {
        if (cmds[i].op != 2 || i == target)
            continue;
        for (k = cmds[i].b, guard = 0; k >= 0 && k < n && guard < n; guard++) {
            if (k == target)
                return i;
            if (cmds[k].op == 9 || cmds[k].op == 3 || cmds[k].op == 7 || cmds[k].op == 4) k = cmds[k].b;
            else break;
        }
    }
    return -1;
}

static void setup_continuation(LCMD *cmds, int n, int cmd)
{
    int target = -1, k, guard, first = -1, x = 0, z = 0, dir = 0, has19 = 0;
    const char *pos = getenv("D1_LADDER_POS");
    /* the mission this step plays: follow the step's chain (movies, gotos) to the first op 2 */
    for (k = cmd, guard = 0; k >= 0 && k < n && guard < n; guard++) {
        if (cmds[k].op == 2) { target = k; break; }
        if (cmds[k].op == 3 || cmds[k].op == 7 || cmds[k].op == 0 || cmds[k].op == 4) k = cmds[k].b;
        else break;
    }
    if (target < 0 || !scan_script(cmds[target].a, &x, &z, &dir, &has19))
        return;
    /* the first part (a chain 55 -> 56 goes back to the part that has a start of its own) */
    for (k = target, guard = 0; guard < 8; guard++) {
        int c;
        first = previous_part(cmds, n, k);
        if (first < 0)
            break;
        c = scan_script(cmds[first].a, &x, &z, &dir, &has19);
        if (has19 || !c)
            break;
        k = first;
    }
    if (pos && *pos) {
        char *e;
        x = strtol(pos, &e, 0);
        if (*e == ',') z = strtol(e + 1, &e, 0);
        if (*e == ',') dir = strtol(e + 1, &e, 0);
    } else if (first >= 0) {
        if (!has19) {
            fprintf(stderr, "[testaid] mission %d continues mission %d, which has no start position: set D1_LADDER_POS\n",
                    cmds[target].a, cmds[first].a);
            return;
        }
    } else {
        fprintf(stderr, "[testaid] mission %d continues an earlier one (not found in the ladder): set D1_LADDER_POS\n",
                cmds[target].a);
        return;
    }
    memset(D_000C6F04 + 0x58, 0, 0x2C);
    ((int *)(D_000C6F04 + 0x58))[0] = x;
    ((int *)(D_000C6F04 + 0x58))[2] = z;
    ((int *)(D_000C6F04 + 0x58))[3] = dir;
    s_place = 1;
    fprintf(stderr, "[testaid] mission %d continues mission %d: player car starts at x=%d z=%d dir=%d (%s)\n",
            cmds[target].a, first >= 0 ? cmds[first].a : -1, x, z, dir,
            pos && *pos ? "D1_LADDER_POS" : "the first part's own start");
}

/* main.c func_00022818, before the continuation start is applied (func_000682C0(car_data, &MISSION.cur)) */
void PC_TestLadderPlace(int *cur)
{
    if (!s_place)
        return;
    s_place = 0;
    cur[1] = MapHeight(cur) + 0xA4;
}

int PC_TestLadder(void)
{
    const char *lad = getenv("D1_LADDER"), *mis = getenv("D1_MISSION");
    static int city[LADDER_MAX], flags[LADDER_MAX], seen[LADDER_MAX], queue[LADDER_MAX];
    int *p, n, nent, i, head = 0, tail = 0, cmd = -1, c, f, maxcity;
    LCMD *cmds;
    LENT *ents;

    if ((!lad || !*lad) && (!mis || !*mis))
        return 0;
    func_0006EACC();
    p = (int *)PC_RAM(0x1FE000);
    n = p[0];
    cmds = (LCMD *)&p[1];
    nent = p[4 * n + 1];
    ents = (LENT *)&p[4 * n + 2];
    if (n <= 0 || n > LADDER_MAX) {
        fprintf(stderr, "[testaid] mladder.psx: bad command count %d\n", n);
        return 0;
    }
    /* first-arrival state per command */
    memset(seen, 0, sizeof(seen));
    seen[0] = 1;
    city[0] = 0;
    flags[0] = 0;
    queue[tail++] = 0;
    while (head < tail) {
        int k = queue[head++], next[4], nn = 0, j;
        LCMD *m = &cmds[k];
        c = city[k];
        f = flags[k];
        switch (m->op) {
        case 0: c = m->a; next[nn++] = m->b; break;
        case 1:
            if (m->a >= 0 && m->a < nent)
                for (j = 0; j < 4; j++)
                    if (ents[m->a * 4 + j].x == 2)
                        next[nn++] = ents[m->a * 4 + j].val;
            break;
        case 4: f |= 1 << m->a; next[nn++] = m->b; break;
        case 5: case 6: next[nn++] = m->b; next[nn++] = m->c; break;
        case 2: case 3: case 7: case 9: next[nn++] = m->b; break;
        default: break;
        }
        for (j = 0; j < nn; j++)
            if (next[j] >= 0 && next[j] < n && !seen[next[j]]) {
                seen[next[j]] = 1;
                city[next[j]] = c;
                flags[next[j]] = f;
                queue[tail++] = next[j];
            }
    }
    if (lad && !strcmp(lad, "list")) {
        for (i = 0; i < n; i++) {
            LCMD *m = &cmds[i];
            fprintf(stderr, "[testaid] %3d %-10s a=%-4d b=%-4d c=%-4d city=%d flags=%d", i,
                    m->op >= 0 && m->op < 10 ? k_ops[m->op] : "?", m->a, m->b, m->c, city[i], flags[i]);
            if (m->op == 1 && m->a >= 0 && m->a < nent) {
                int j;
                for (j = 0; j < 4; j++)
                    if (ents[m->a * 4 + j].x == 2)
                        fprintf(stderr, " [job %d -> %d]", ents[m->a * 4 + j].id, ents[m->a * 4 + j].val);
            }
            fprintf(stderr, "\n");
        }
        return 0;
    }
    if (mis && *mis) {
        int want = atoi(mis);
        for (i = 0; i < n && cmd < 0; i++)
            if (cmds[i].op == 2 && cmds[i].a == want)
                cmd = i;
        if (cmd < 0) {
            fprintf(stderr, "[testaid] D1_MISSION=%d: no ladder command plays it\n", want);
            return 0;
        }
    } else {
        char *e;
        cmd = strtol(lad, &e, 10);
        if (cmd < 0 || cmd >= n) {
            fprintf(stderr, "[testaid] D1_LADDER=%s: command out of range (0..%d)\n", lad, n - 1);
            return 0;
        }
        if (*e == ':') {
            city[cmd] = strtol(e + 1, &e, 10);
            if (*e == ':')
                flags[cmd] = strtol(e + 1, &e, 0);
        }
    }
    if (getenv("D1_LADDER_FMV") && atoi(getenv("D1_LADDER_FMV")))
        setenv("D1_FMV", "1", 1);
    setup_continuation(cmds, n, cmd);
    maxcity = city[cmd] + 1;
    D_000A083C[0] = maxcity;
    D_000A083C[1] = cmd;
    D_000A083C[2] = city[cmd];
    D_000A083C[3] = flags[cmd];
    fprintf(stderr, "[testaid] career starts at ladder command %d (%s a=%d), city %d, flags 0x%x%s\n", cmd,
            cmds[cmd].op >= 0 && cmds[cmd].op < 10 ? k_ops[cmds[cmd].op] : "?", cmds[cmd].a, city[cmd], flags[cmd],
            seen[cmd] ? "" : " (not reachable from the start)");
    return 1;
}
