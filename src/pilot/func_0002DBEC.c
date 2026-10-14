#include "gte.h"

extern char D_000BBE84[];   /* Driver 2: inv_camera_matrix */

/* Driver 2: Set_Inv_CameraMatrix */
void func_0002DBEC(void)
{
    gte_SetColorMatrix(D_000BBE84);
}
