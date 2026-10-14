extern void (*D_0008BA08[])(unsigned char *); int func_0004FFE8(int);
int func_0005FECC(unsigned char *p){ int n; n = func_0004FFE8(0) % 8;
 switch (p[0x56]) { case 1: D_0008BA08[n](p); break; case 2: D_0008BA08[n](p); break; case 3: D_0008BA08[n](p); break; case 4: D_0008BA08[n](p); break; case 5: D_0008BA08[n](p); break; } return n; }
