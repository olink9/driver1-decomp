typedef struct { int x, y, z; int w3; char pad[0x18]; int e28, e2c, e30; char pad2[0x08]; int e3c; char pad3[0x10]; char b50; char b51; char b52; char b53; char b54; char b55; char b56; char pad4[1]; } P;
typedef struct { int a, b, c, d; unsigned char e, f, g, h; } En;
extern P D_000C0104[];
extern int camera_position[];
extern long long cameraview;
extern char D_000976C8;
extern void func_000618C8();
extern void func_0005D974();
extern int MapHeight();

void func_00061A14(char *o)
{
	unsigned char n;
	int c, z;
	unsigned char i;
	P *d;
	En *e;
	n = *(unsigned char *)(o + 0xD0);
	c = n & 0xff;
	if (c != 0) {
		*(int *)(o + 0x84) = camera_position[0];
		*(int *)(o + 0x88) = camera_position[1];
		z = camera_position[2];
		*(char *)(o + 0x90) = *(char *)&cameraview;
		D_000976C8 = 0;
		*(int *)(o + 0x8C) = z;
		func_000618C8(o + 0x38);
		*(char *)&cameraview = 6;
		*(int *)(o + 0x74) = 0;
		*(int *)(o + 0x78) = -300;
		*(int *)(o + 0x7C) = 0;
		func_000618C8(o);
		d = D_000C0104;
		*(P **)(o + 0x70) = d;
		e = (En *)(o + 0x94);
		for (i = 0; i < n; i++) {
			d->b56 = e->e;
			func_0005D974(d);
			d->x = *(int *)o + e->a;
			d->z = *(int *)(o + 8) + *(int *)((char *)e + 8);
			d->y = -MapHeight(d);
			d->e28 = *(int *)(o + 0x18);
			d->e2c = *(int *)(o + 0x1c);
			d->e30 = *(int *)(o + 0x20);
			d->e3c = *(int *)((char *)e + 0xc) | 0x50;
			d->b55 = e->f;
			d->b50 = e->g;
			e++;
			d--;
		}
		*(char *)(o + 0xD1) = 1;
	}
}
