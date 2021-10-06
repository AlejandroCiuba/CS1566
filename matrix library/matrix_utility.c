/**
 * definitions for the matrix_utility.h functions
 * 
 * Created by Alejandro Ciuba
 */

#include "matrix_utility.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//Prints the given vector
void print_vector(vector4 vec) {printf("[x : %.4f, y : %.4f, z : %.4f, w : %.4f]\n", vec.x, vec.y, vec.z, vec.w);}

//Prints the given 4x4 matrix
void print_matrix(mat4x4 mat) {

    printf("| x : %.4f, %.4f, %.4f, %.4f |\n| y : %.4f, %.4f, %.4f, %.4f |\n| z : %.4f, %.4f, %.4f, %.4f |\n| w : %.4f, %.4f, %.4f, %.4f |\n", 
            mat.x.x, mat.y.x, mat.z.x, mat.w.x, 
            mat.x.y, mat.y.y, mat.z.y, mat.w.y, 
            mat.x.z, mat.y.z, mat.z.z, mat.w.z, 
            mat.x.w, mat.y.w, mat.z.w, mat.w.w);
}

//Prints the given GLfloat 
void print_GLfloat(GLfloat flt) {printf("GLfloat : %.4f\n", flt);}

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
ERROR_NUM zero_vector(vector4* vec) {
    if(vec == NULL) return MATLIB_POINTER_ERROR;

    vec->x = 0;
    vec->y = 0;
    vec->z = 0;
    vec->w = 0;

    return 0;
}

//Returns a pointer to a 0 matrix
ERROR_NUM zero_matrix(mat4x4* mat) {

    if(mat == NULL) return MATLIB_POINTER_ERROR;
    
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

    return 0;
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

//Tells if two vectors are equal
bool vector_equal(vector4* vec1, vector4* vec2) {

    if((vec1 == NULL && vec2 != NULL)|| (vec2 == NULL && vec1 != NULL)) return false;
    else if(vec1 == NULL && vec2 == NULL) return true;
    else if(vec1 == vec2) return true;
    else return vec1->x == vec2->x && vec1->y == vec2->y && vec1->z == vec2->z && vec1->w == vec2->w;
}

//Tells if two matrices are equal
bool matrix_equal(mat4x4* mat1, mat4x4* mat2) {
    
    if((mat1 == NULL && mat2 != NULL) || (mat2 == NULL && mat1 != NULL)) return false;
    else if(mat1 == NULL && mat2 == NULL) return true;
    else if(mat1 == mat2) return true;
    else return vector_equal(&(mat1->x), &(mat2->x)) && vector_equal(&(mat1->y), &(mat2->y)) 
    && vector_equal(&(mat1->z), &(mat2->z)) && vector_equal(&(mat1->w), &(mat2->w));
}

//Convert screen coordinates to world coordinates
ERROR_NUM screen_to_world(vector4* screen, vector4* result, int x_screen, int y_screen) {

    if(screen == NULL || result == NULL) return MATLIB_POINTER_ERROR;

    if(x > 255 && y > 255) {
        result->x = (screen->x - (x_screen / 2)) / (x_screen / 2); 
        result->y = (screen->y - (y_screen / 2)) / (y_screen / 2);
        result->z = sqrt(1 - pow(result->x, 2) - pow(result->y, 2));
        result->w = 1.0;
    }
    else if(x <= 255 && y > 255) {
        result->x = (screen->x - ((x_screen / 2) - 1)) / ((x_screen / 2) - 1);
        result->y = (screen->y - (y_screen / 2)) / (y_screen / 2);
        result->z = sqrt(1 - pow(result->x, 2) - pow(result->y, 2));
        result->w = 1.0;
    }
    else if(x <= 255 && y <= 255) {
        result->x = (screen->x - ((x_screen / 2) - 1)) / ((x_screen / 2) - 1);
        result->y = (screen->y - ((y_screen / 2) - 1)) / ((y_screen / 2) - 1);
        result->z = sqrt(1 - pow(result->x, 2) - pow(result->y, 2));
        result->w = 1.0;
    }
    else if(x > 255 && y <= 255) {
        result->x = (screen->x - (x_screen / 2)) / (x_screen / 2); 
        result->y = (screen->y - ((y_screen / 2) - 1)) / ((y_screen / 2) - 1);
        result->z = sqrt(1 - pow(result->x, 2) - pow(result->y, 2));
        result->w = 1.0;
    }
    else return MATLIB_NAN_ERROR;

    return 0;
}
