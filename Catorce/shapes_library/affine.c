/**
 * Returns matrices for various graphics
 * Operations like scaling, transformation,
 * And rotation.
 * 
 * @author Alejandro Ciuba
 */

#include "affine.h"
#include "../matrix_library/matrix_ops.h"
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

//Order indexed [0-3)
ERROR_NUM rot(affine degrees, int* order, mat4x4* affine) {

    if(affine == NULL) return MATLIB_POINTER_ERROR;
    if(order == NULL) return MATLIB_POINTER_ERROR;

    mat4x4 xyz[3];
    xyz[0] = xyz[1] = xyz[2] = (mat4x4) {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};

    if(degrees.x != 0) rotate(degrees.x, 'x', &xyz[0]);
    if(degrees.y != 0) rotate(degrees.y, 'y', &xyz[1]);
    if(degrees.z != 0) rotate(degrees.z, 'z', &xyz[2]);

    mat_mult((mat4x4[3]) {xyz[order[0]], xyz[order[1]], xyz[order[2]]}, 3, affine);

    return 0;
}

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

//Rotate around a given arbitrary vector
ERROR_NUM rotate_arb(GLfloat degree, vector4* axis, vector4* com, mat4x4* aff) {

    if(axis == NULL || aff == NULL) return MATLIB_POINTER_ERROR;

    //Put it all together
    mat4x4 t1, rx1, ry1, rz, ry2, rx2, t2;

    trans((affine){-com->x, -com->y, -com->z}, &t1); trans((affine){com->x, com->y, com->z}, &t2);

    rx1 = (mat4x4){{1,0,0,0},{0,axis->z / sqrt(pow(axis->y, 2) + pow(axis->z, 2)), axis->y / sqrt(pow(axis->y, 2) + pow(axis->z, 2)),0},{0,-axis->y / sqrt(pow(axis->y, 2) + pow(axis->z, 2)),axis->z / sqrt(pow(axis->y, 2) + pow(axis->z, 2)),0},{0,0,0,1}};
    transpose_sep(&rx1, &rx2);

    ry1 = (mat4x4){{sqrt(pow(axis->y, 2) + pow(axis->z, 2)), 0, axis->x, 0},{0,1,0,0},{-axis->x,0,sqrt(pow(axis->y, 2) + pow(axis->z, 2)),0},{0,0,0,1}};
    transpose_sep(&ry1, &ry2);

    rotate(degree, 'z', &rz);

    mat_mult((mat4x4[7]) {t2, rx2, ry2, rz, ry1, rx1, t1}, 7, aff);

    return 0;
}