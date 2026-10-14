typedef struct { short x, y, w, h; } RECT;
typedef struct { int a, b, c, d; } W16;
typedef struct { int a, b, c, d; } __attribute__((packed)) U16;
extern char D_00010F18[];
extern char D_000BF2EC[];
extern W16 D_000BF6EC[];
extern short D_000976A4;
extern long long D_000976A6;
extern int D_00097684;
extern int D_00097694;
extern void func_00050E28();
extern void func_00079DB0();
extern int func_0007A7F4();
extern void func_00074404();
extern int GetTPage();
extern int DrawSync();

/* LoadFont */
void func_0005C99C(void)
{
	RECT dest;
	char *file;
	int i;
	int nchars;
	unsigned short *pclut;
	W16 *d;
	W16 *s;

	func_00050E28(D_00010F18, 0xF8600);
	file = (char *)0xF8604;
	nchars = *(int *)0xF8600;
	nchars <<= 3;
	func_00079DB0(D_000BF2EC, file, nchars);
	file += nchars;
	s = (W16 *)file;
	d = D_000BF6EC;
	if (((int)d & 3) != 0) {
		U16 *us = (U16 *)s, *ud = (U16 *)d;
		U16 *e = us + 16;
		do { *ud++ = *us++; } while (us != e);
	} else {
		W16 *e = s + 16;
		do { *d++ = *s++; } while (s != e);
	}
	file += 256;
	D_00097684 = (unsigned short)func_0007A7F4(D_000976A4, *(short *)&D_000976A6);
	pclut = (unsigned short *)file;
	for (i = 0; i < 16; i++)
		pclut[i] &= 0x7fff;
	pclut[5] |= 0x8000;
	pclut[13] |= 0x8000;
	pclut[10] = 0;
	func_00074404(&D_000976A4, file);
	file += 32;
	dest.x = 0x3c0; dest.y = 0x1cb;
	dest.w = 0x40; dest.h = 0x35;
	D_00097694 = (unsigned short)GetTPage(0, 0, 0x3c0, 0x1cb);
	func_00074404(&dest, file);
	DrawSync(0);
}
