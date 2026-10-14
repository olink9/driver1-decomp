struct O { unsigned char a; char p1[7]; char c8; char p2[2]; unsigned char cB; char p3[0x1C]; int i28; };
extern int func_0004FFE8(int);
extern void func_00018900(struct O *);
void func_00017D74(struct O *o) {
    o->c8 = 0;
    if (o->cB != 0) {
        o->cB = o->cB - 1;
    } else if (o->i28 != -1) {
        if ((func_0004FFE8(o->a) & 0x1F) == o->a) func_00018900(o);
    }
}
