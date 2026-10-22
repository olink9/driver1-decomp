// find_lamp_streak
struct L { int a, b, c, d, id; char e, g, set; char f; };
extern struct L Known_Lamps[];
extern int D_000BBDD0[];
extern int D_00097448;
int find_lamp_streak(int id) {
    int i;
    for (i = 0; i < 20; i++) {
        if (Known_Lamps[i].id == id) {
            Known_Lamps[i].set = 1;
            return i;
        }
    }
    if (D_00097448 < 20) {
        D_000BBDD0[D_00097448++] = id;
    }
    return -1;
}
