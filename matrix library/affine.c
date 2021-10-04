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

#include <stdio.h>

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

    GLfloat minx = vertices[0].x, miny = vertices[0].y, minz = vertices[0].z, maxx = vertices[0].x, maxy = vertices[0].y, maxz = vertices[0].z;
    for(int i = 1; i < count; i++) {
        if(vertices[i].x < minx) minx = vertices[i].x;
        else if(vertices[i].x > maxx) maxx = vertices[i].x;
        if(vertices[i].y < miny) miny = vertices[i].y;
        else if(vertices[i].y > maxy) maxy = vertices[i].y;
        if(vertices[i].z < minz) minz = vertices[i].z;
        else if(vertices[i].z > maxz) maxz = vertices[i].z;
    }
    
    com->x = maxx - ((maxx - minx) / 2);
    com->y = maxy - ((maxy - miny) / 2);
    com->z = maxz - ((maxz - minz) / 2);
    com->w = 1;

    return 0;
}