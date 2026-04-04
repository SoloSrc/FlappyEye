#include "fe_vec.h"

D_Vector2 D_AddVector2(D_Vector2 v1, D_Vector2 v2)
{
    return (D_Vector2){
        .x = v1.x + v2.x,
        .y = v1.y + v2.y
    };
}
