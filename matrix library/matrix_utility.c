/**
 * definitions for the matrix_utility.h functions
 * ERROR_NUM to return -2 to signify pointer problem
 * with matrix pointers and return -3 to signify
 * memory allocation problems
 * 
 * Created by Alejandro Ciuba
 */

#include "matrix_utility.h"
#include <stdio.h>
#include <stdlib.h>

//Prints the given vector
void print_vector(const vector4 vec) {printf("[x : %f, y : %f, z : %f, w : %f]\n", vec.x, vec.y, vec.z, vec.w);}

//Prints the given 4x4 matrix
void print_matrix(const mat4x4 mat) {

    printf("| x : %f, %f, %f, %f |\n| y : %f, %f, %f, %f |\n| z : %f, %f, %f, %f |\n| w : %f, %f, %f, %f |\n", 
            mat.x.x, mat.y.x, mat.z.x, mat.w.x, 
            mat.x.y, mat.y.y, mat.z.y, mat.w.y, 
            mat.x.z, mat.y.z, mat.z.z, mat.w.z, 
            mat.x.w, mat.y.w, mat.z.w, mat.w.w);
}

//Prints the given GLfloat 
void print_GLfloat(const GLfloat flt) {printf("GLfloat : %f\n", flt);}

//Pointer versions of debugging statements
ERROR_NUM print_vector_ptr(const vector4* vec) {

    if(vec == NULL) return MATLIB_POINTER_ERROR;
    print_vector(*vec);
    return 0;
}

ERROR_NUM print_matrix_ptr(const mat4x4* mat) {

    if(mat == NULL) return MATLIB_POINTER_ERROR;
    print_matrix(*mat);
    return 0;
}

ERROR_NUM print_GLfloat_ptr(const GLfloat* flt) {

    if(flt == NULL) return MATLIB_POINTER_ERROR;
    print_GLfloat(*flt);
    return 0;
}

//Returns a pointer to a 0 vector4
vector4* zero_vector() {
    
    vector4* vec = (vector4*) malloc(sizeof(vector4));
    if(vec == NULL) return NULL;
    vec->x = 0;
    vec->y = 0;
    vec->z = 0;
    vec->w = 0;
    return vec;
}

//Returns a pointer to a 0 matrix
mat4x4* zero_matrix() {

    mat4x4* mat = (mat4x4*) malloc(sizeof(mat4x4));

    if(mat == NULL) return NULL;
    mat->x.x = 0;
    mat->x.y = 0;
    mat->x.z = 0;
    mat->x.w = 0;

    mat->y.x = 0;
    mat->y.y = 0;
    mat->y.z = 0;
    mat->y.w = 0;

    mat->z.x = 0;
    mat->z.y = 0;
    mat->z.z = 0;
    mat->z.w = 0;

    mat->w.x = 0;
    mat->w.y = 0;
    mat->w.z = 0;
    mat->w.w = 0;
    return mat;
}

//Copies stuff from one vector to another
ERROR_NUM copy_vector(const vector4* og, vector4* cpy) {

    if(og == NULL || cpy == NULL) return MATLIB_POINTER_ERROR;

    cpy->x = og->x; 
    cpy->y = og->y; 
    cpy->z = og->z; 
    cpy->w = og->w;

    return 0;
}

//Makes a vector4 pointer into a mat4x4 pointer with the values
//Copied into every column from the original vector4
//Note: WE DO NOT FREE THE VECTOR HERE
mat4x4* vec_to_mat(const vector4* vec) {

    if(vec == NULL) return NULL;

    mat4x4* mat = (mat4x4*) malloc(sizeof(mat4x4));
    if(mat == NULL) return NULL;

    mat->x = *vec;
    mat->y = *vec;
    mat->z = *vec;
    mat->w = *vec;

    return mat;
}

//Creates an matrix/vector filled with the
//passed in value
ERROR_NUM fill_matrix(mat4x4* mat, const GLfloat flt) {

    if(mat == NULL) return MATLIB_POINTER_ERROR;
    fill_vector(&mat->x, flt);
    fill_vector(&mat->y, flt);
    fill_vector(&mat->z, flt);
    fill_vector(&mat->w, flt);
    return 0;
}

ERROR_NUM fill_vector(vector4* vec, const GLfloat flt) {

    if(vec == NULL) return MATLIB_POINTER_ERROR;
    vec->x = vec->y = vec->z = vec->w = flt;
    return 0;
}