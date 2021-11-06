/**
 * Returns matrices for various graphics
 * Operations like scaling, translate,
 * And rotation.
 * 
 * @author Alejandro Ciuba
 */

#ifndef AFFINE_H
#define AFFINE_H

#include "../matrix_library/matrix_def.h"

// Special struct to make scaling and translate easier
typedef struct affine {
    GLfloat x;
    GLfloat y;
    GLfloat z;
} affine;

ERROR_NUM translate(GLfloat x, GLfloat y , GLfloat z, mat4x4* affine);

ERROR_NUM scaling(GLfloat x, GLfloat y , GLfloat z, mat4x4* affine);

ERROR_NUM rotate(GLfloat degree, char align, mat4x4* affine);

ERROR_NUM trans(affine loc, mat4x4* affine);

ERROR_NUM scal(affine size, mat4x4* affine);

//Order indexed [0-3)
ERROR_NUM rot(affine degrees, int* order, mat4x4* affine);

//Gets the center of mass, relative to the shapes current position
ERROR_NUM com(vector4* vertices, int count, vector4* com);

//Rotate around a given arbitrary vector
ERROR_NUM rotate_arb(GLfloat degree, vector4* axis, vector4* com, mat4x4* aff);

#endif