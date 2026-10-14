/* Driver 2: StepCars */
typedef struct { char pad[3]; unsigned char f; } C;
extern int D_00097030;
extern C *D_0009AC48[];
extern void func_0001D6E4();
extern void func_0004C72C();

void func_0001D648(void) {
    C **p;
    C **end;
    end = D_0009AC48 + D_00097030;
    p = D_0009AC48;
    while (p < end) {
        func_0001D6E4(*p, (*p)->f == 1);
        p++;
    }
    p = D_0009AC48;
    while (p < end) func_0004C72C(*p++);
}
