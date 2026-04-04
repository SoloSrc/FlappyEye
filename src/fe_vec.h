// fe_vec.h
//
// Header file for anything that is Linear Algebra related. For example,
// creating a Vector2, adding it, multiplying it, etc.

#ifndef FE_VEC_H
#define FE_VEC_H

typedef struct D_Vector2 {
    float x;
    float y;
} D_Vector2;

D_Vector2 D_AddVector2(D_Vector2 v1, D_Vector2 v2);

#endif /* FE_VEC_H */
