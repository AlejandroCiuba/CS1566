/**
 * Returns matrices for various graphics
 * Operations like scaling, translate,
 * And rotation.
 * 
 * @author Alejandro Ciuba
 */

#ifndef MATRIX_SHAPES_H
#define MATRIX_SHAPES_H

#include "matrix_def.h"

ERROR_NUM translate(GLfloat x, GLfloat y , GLfloat z, mat4x4* affine);

ERROR_NUM scaling(GLfloat x, GLfloat y , GLfloat z, mat4x4* affine);

ERROR_NUM rotate(GLfloat degree, char align, mat4x4* affine);

#endif