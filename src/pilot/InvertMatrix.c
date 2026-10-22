#include "common.h"
void InvertMatrix(MATRIX *a, MATRIX *b) { // InvertMatrix
    b->m[0][0] = a->m[0][0];
    b->m[0][1] = a->m[1][0];
    b->m[0][2] = a->m[2][0];
    b->m[1][0] = a->m[0][1];
    b->m[1][1] = a->m[1][1];
    b->m[1][2] = a->m[2][1];
    b->m[2][0] = a->m[0][2];
    b->m[2][1] = a->m[1][2];
    b->m[2][2] = a->m[2][2];
}
