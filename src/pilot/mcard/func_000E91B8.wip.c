/* (run4-ov) Tried: block-local `char *p` (87.14 / 89.93), E *e pointer, static inline nm(s, x) wrapping the call
   (92.60 but the address folds to (c*257+f*17)*4 + base+0x20; constants still first). The then-arm constants-last order
   is not a luid tie: even with the address computed in an earlier statement sched1 puts the constants first. */
/* WIP 89.93 (was 87.14): the then-arm now addresses the name as `(E *)((char *)D[c].e + f*0x44)` so the two
   func_0005D164 arms stay separate (target keeps two copies). Left: SCHEDULE of the then-arm - target computes the whole
   address first (F' into a2, then C', then +8/+F'/+0x18) and sets a2/a3/stack args last; here both arms set the
   constants first and the then-arm cross-jumps into the else-arm's jal. Tried (ov1-wip M): block-local `char *p`,
   `E *e` pointer, int f / int c locals, struct-cast (INT1) reads of D_000EF13C/EF140 (all re-merge the arms).
   The s0/s1 swap (%hi(D_000EF140) vs D_000EE930 base) is new with this form. Untried: why sched1 puts the arg
   constants first in the target else-arm but last in the then-arm (read .sched for both arms). */
typedef struct { int a; short b; char pad[0x12]; char name[0x28]; unsigned char idx; unsigned char n; unsigned char f42; unsigned char f43; } E;
typedef struct { unsigned short s; char c2; char c3; void *p4; E e[15]; } H;
typedef struct { unsigned addr:24; unsigned len:8; } P_TAG;
typedef struct { P_TAG tag; int w[9]; } POLY_FT4;
typedef struct { char *primptr; P_TAG **ot; int f8; int id; } DB;
extern DB *current;
extern H D_000EE930[];
extern POLY_FT4 D_000EF8D0[];
extern int D_000ED7CC;
extern int D_000ED7C8;
extern int D_000ED7C4;
extern int D_000EF13C;
extern int D_000EF140;
extern int D_000EF144;
extern char D_000ED784[];
extern char D_000E6864[];
extern char D_000E686C[];
extern char D_000E687C[];
extern void func_000EABBC(), func_0005C8F4(), func_000EB104(), func_0005D164();
extern int PrintString();

/* draw the memory card screen: background, animated icons, both cards, selected save's name */
void func_000E91B8(void)
{
    int i;
    int x;

    D_000EF8D0[current->id].tag.addr = current->ot[12]->addr;
    current->ot[12]->addr = (unsigned int)&D_000EF8D0[current->id];
    (D_000EF8D0 + 2)[current->id].tag.addr = current->ot[12]->addr;
    current->ot[12]->addr = (unsigned int)&(D_000EF8D0 + 2)[current->id];
    if (D_000ED7CC == 0) {
        if (--D_000ED7C8 == 0) {
            D_000ED7C8 = 12;
            D_000ED7C4++;
        }
    }
    for (i = 0; i < 2; i++) {
        if ((D_000EE930[i].s & 2) || (D_000EE930[i].s & 4))
            func_000EABBC(i);
    }
    func_0005C8F4(D_000ED784, 0xBA);
    func_000EB104(0);
    func_000EB104(1);
    if ((D_000EE930[D_000EF13C].s & 2) && (D_000EE930[D_000EF13C].e[D_000EF140].f43 & 1)) {
        x = PrintString(D_000E6864, 0x18, 0xCE);
        if ((unsigned int)D_000EF144 < 3) {
            int f = D_000EF140 * 0x44;
            func_0005D164(((E *)((char *)D_000EE930[D_000EF13C].e + f))->name, x, 0xCE, 0x1000, 0xE00, 1);
        } else
            func_0005D164(D_000EE930[D_000EF13C].e[D_000EF140].name, x, 0xCE, 0x1000, 0xE00, 1);
    } else if (D_000EE930[D_000EF13C].s != 4 && D_000EE930[D_000EF13C].s != 1) {
        if (D_000EF144 < 3)
            PrintString(D_000E686C, 0x1E, 0xCE);
        else
            PrintString(D_000E687C, 0x1E, 0xCE);
    }
}
