#include "gte.h"

extern char inv_camera_matrix[];   /* Driver 2: inv_camera_matrix */

/* Driver 2: Set_Inv_CameraMatrix */
void func_0002DBEC(void)
{
    gte_SetColorMatrix(inv_camera_matrix);
}
