/**
 * Returns matrices for various graphics
 * Operations like scaling, transformation,
 * And rotation.
 * 
 * @author Alejandro Ciuba
 */

#include "affine.h"
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

ERROR_NUM trans(affine loc, mat4x4* affine) {return translate(loc.x, loc.y, loc.z, affine);}

ERROR_NUM scal(affine size, mat4x4* affine) {return scaling(size.x, size.y, size.z, affine);}

//Gets the center of mass, relative to the shapes current position
ERROR_NUM com(vector4* vertices, int count, vector4* com) {

    if(vertices == NULL || com == NULL) return MATLIB_VECTOR_ERROR;

    //Get the furthest points along the x, y, and z axis from each other
    com->x = 0; com->y = 0; com->z = 0; com->w = 1;
    for(int i = 0; i < count; i++)
        for(int j = i + 1; j < count; j++) {
            if(vertices[i].x - vertices[j].x > com->x || vertices[j].x - vertices[i].x > com->x) 
                com->x = (vertices[i].x > vertices[j].x)? vertices[i].x - vertices[j].x : vertices[j].x - vertices[i].x;
            if(vertices[i].y - vertices[j].y > com->y || vertices[j].y - vertices[i].y > com->y) 
                com->y = (vertices[i].y > vertices[j].y)? vertices[i].y - vertices[j].y : vertices[j].y - vertices[i].y;
            if(vertices[i].z - vertices[j].z > com->z || vertices[j].z - vertices[i].z > com->z) 
                com->z = (vertices[i].z > vertices[j].z)? vertices[i].z - vertices[j].z : vertices[j].z - vertices[i].z;
        }

    return 0;
}