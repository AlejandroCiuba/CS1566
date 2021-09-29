/**
 * Returns matrices for various graphics
 * Operations like scaling, transformation,
 * And rotation.
 * 
 * @author Alejandro Ciuba
 */

#include "matrix_shapes.h"
#include "matrix_ops.h"
#include <stddef.h>
#include <math.h>

ERROR_NUM translate(GLfloat x, GLfloat y , GLfloat z, mat4x4* affine) {

    if(affine == NULL) return MATLIB_POINTER_ERROR;

    affine->x = (vector4) {1,0,0,0};
    affine->y = (vector4) {0,1,0,0};
    affine->z = (vector4) {0,0,1,0};
    affine->w = (vector4) {x,y,z,1};
    
    return 0;
}

ERROR_NUM scaling(GLfloat x, GLfloat y , GLfloat z, mat4x4* affine) {

    if(affine == NULL) return MATLIB_POINTER_ERROR;

    affine->x = (vector4) {x,0,0,0};
    affine->y = (vector4) {0,y,0,0};
    affine->z = (vector4) {0,0,z,0};
    affine->w = (vector4) {0,0,0,1};
    
    return 0;
}

ERROR_NUM rotate(GLfloat degree, char align, mat4x4* affine) {

    if(affine == NULL) return MATLIB_POINTER_ERROR;
    else if(align != 'x' && align != 'y' && align != 'z') return MATLIB_MATRIX_ERROR;
    
    GLfloat rads = (GLfloat) degree / 180 * M_PI;

    if(align == 'z') {
        affine->x = (vector4) {cos(rads), sin(rads), 0, 0};
        affine->y = (vector4) {-1 * sin(rads), cos(rads), 0, 0};
        affine->z = (vector4) {0,0,1,0};
        affine->w = (vector4) {0,0,0,1};
    }
    else if(align == 'x') {
        affine->x = (vector4) {1,0,0,0};
        affine->y = (vector4) {0, cos(rads), sin(rads), 0};
        affine->z = (vector4) {0, -1 * sin(rads), cos(rads), 0};
        affine->w = (vector4) {0,0,0,1};
    }
    else {
        affine->x = (vector4) {cos(rads), 0, -1 * sin(rads), 0};
        affine->y = (vector4) {0,1,0,0};
        affine->z = (vector4) {sin(rads), 0, cos(rads), 0};
        affine->w = (vector4) {0,0,0,1};
    }

    return 0;
}