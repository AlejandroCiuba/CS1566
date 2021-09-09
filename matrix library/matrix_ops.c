/**
 * Matrix and vector math operations
 * 
 * Created by Alejandro Ciuba
 */

#include "matrix_ops.h"
#include "math.h"
#include <stddef.h> //Needed here, but NOT in header file

//============== VECTOR AND MATRIX MATH FUNCTIONS ==============
//for args, 0 = vector, 1 = matrix4x4
ERROR_NUM scalar(const void* matrix, const GLfloat alpha, const char args) {

    if(matrix == NULL || (args != 0 && args != 1)) return MATLIB_POINTER_ERROR;

    GLfloat* mat = (GLfloat*) matrix;
    
    char limit;
    if(args == 0) limit = 4;
    else limit = 16;

    for(char i = 0; i < limit; i++) 
        *(mat + i) = *(mat + i) * alpha;

    return 0;
}

//============== VECTOR MATH FUNCTIONS ==============
//Add any number of vectors at once
ERROR_NUM vector_add(const vector4* vectors[], int count, vector4* result) {

    //Test for all vectors if any equal null
    for(int i = 0; i < count; i++) {

        if(vectors[i] == NULL) return MATLIB_POINTER_ERROR;

        result->x += vectors[i]->x;
        result->y += vectors[i]->y;
        result->z += vectors[i]->z;
        result->w += vectors[i]->w;
    }

    return 0;
}

//Subtract any number of vectors at once
ERROR_NUM vector_sub(const vector4* vectors[], int count, vector4* result) {

    //Test for all vectors if any equal null
    for(int i = 0; i < count; i++) {

        if(vectors[i] == NULL) return MATLIB_POINTER_ERROR;

        result->x -= vectors[i]->x;
        result->y -= vectors[i]->y;
        result->z -= vectors[i]->z;
        result->w -= vectors[i]->w;
    }

    return 0;
}

//Returns that vectors magnitude
ERROR_NUM vector_mag(const vector4* vec, GLfloat* result) {
    if(vec == NULL) return MATLIB_POINTER_ERROR;
    *result = (GLfloat) sqrt(pow(vec->x, 2) + pow(vec->y, 2) + pow(vec->z, 2) + pow(vec->w, 2));
    return 0;
}