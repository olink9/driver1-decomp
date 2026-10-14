/* MangleWheelModels (Driver 2) */
typedef struct { unsigned char u, v; } __attribute__((aligned(2))) UV_INFO;

typedef struct {
    unsigned char id, texture_set, texture_id, spare;
    unsigned char v0, v1, v2, v3;
    UV_INFO uv0, uv1, uv2, uv3;
    unsigned char r, g, b, pad;
} POLYFT4;

typedef struct {
    char pad[0xE];
    short num_polys;
    int pad2;
    POLYFT4 *poly_block;
} MODEL;

extern long long D_000970AC;
extern long long D_000970C0;
extern long long D_000970C4;
extern long long D_00096834;
#define gCleanWheelModelPtr (*(MODEL **)&D_000970AC)
#define gFastWheelModelPtr (*(MODEL **)&D_000970C0)
#define gDamWheelModelPtr (*(MODEL **)&D_000970C4)
#define GameLevel (*(int *)&D_00096834)

void func_0003C6CC(void)
{
    UV_INFO tmpUV2;
    unsigned char tmpUV;
    POLYFT4 *src;
    POLYFT4 *s3, *s2;
    MODEL *m;
    int i, j;

    for (i = 0; i < 3; i++)
    {
        switch (i)
        {
        case 0:
        default:
            m = gCleanWheelModelPtr;
            break;
        case 1:
            m = gFastWheelModelPtr;
            break;
        case 2:
            m = gDamWheelModelPtr;
            break;
        }

        src = m->poly_block;
        src[0] = src[2];
        src[1] = src[3];
        src[2] = src[4];
        src[3] = src[6];

        src[2].v0 = 16;
        src[2].v1 = 17;
        src[2].v2 = 23;
        src[2].v3 = 22;
        src[3].v0 = 21;
        src[3].v1 = 20;
        src[3].v2 = 19;
        src[3].v3 = 18;

        s3 = src + 3;
        s2 = src + 2;
        tmpUV = (src->uv0.u >> 1) + (src->uv2.u >> 1);
        s3->uv3.u = tmpUV;
        s3->uv2.u = tmpUV;
        s3->uv1.u = tmpUV;
        s3->uv0.u = tmpUV;
        s2->uv3.u = tmpUV;
        s2->uv2.u = tmpUV;
        s2->uv1.u = tmpUV;
        src[2].uv0.u = tmpUV;

        tmpUV = (src->uv0.v >> 1) + (src->uv2.v >> 1);
        s3->uv3.v = tmpUV;
        s3->uv2.v = tmpUV;
        s3->uv1.v = tmpUV;
        s3->uv0.v = tmpUV;
        s2->uv3.v = tmpUV;
        s2->uv2.v = tmpUV;
        s2->uv1.v = tmpUV;
        src[2].uv0.v = tmpUV;

        m->num_polys = 4;

        for (j = 0; j < 2; j++)
        {
            tmpUV2 = src[j].uv0;
            src[j].uv0 = src[j].uv1;
            src[j].uv1 = src[j].uv2;
            src[j].uv2 = src[j].uv3;
            src[j].uv3 = tmpUV2;
        }
    }

    if (GameLevel == 3)
        gCleanWheelModelPtr = gFastWheelModelPtr;
}
