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
ERROR_NUM vector_add(vector4* vectors[], int count, vector4* result) {

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
ERROR_NUM vector_sub(vector4* vectors[], int count, vector4* result) {

    if(vectors[0] == NULL) return MATLIB_POINTER_ERROR;

    result->x = vectors[0]->x;
    result->y = vectors[0]->y;
    result->z = vectors[0]->z;
    result->w = vectors[0]->w;

    //Test for all vectors if any equal null
    for(int i = 1; i < count; i++) {

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

//Normalizes the given vector
ERROR_NUM vector_norm(const vector4* vec) {

    if(vec == NULL) return MATLIB_POINTER_ERROR;
    GLfloat mag = 0.0f;
    //Safe since everything is guaranteed to be established
    vector_mag(vec, &mag);

    if(mag == 0.0f) return MATLIB_NAN_ERROR;
    scalar(vec, 1 / mag, 0);

    return 0;
}

//Returns the Dot Product of 2 given vectors
ERROR_NUM vector_dot(const vector4* vec1, const vector4* vec2, GLfloat* result) {

    if(vec1 == NULL || vec2 == NULL) return MATLIB_POINTER_ERROR;
    *result = (vec1->x * vec2->x) + (vec1->y * vec2->y) + (vec1->z * vec2->z) + (vec1->w * vec2->w);
    return 0;
}

//============== MATRIX MATH FUNCTIONS ==============
//Add any number of matrices at once
ERROR_NUM matrix_add(mat4x4* matrices[], int count, mat4x4* result) {

    for(int i = 0; i < count; i++) {
        
        if(matrices[i] == NULL) return MATLIB_POINTER_ERROR;

        result->x.x += matrices[i]->x.x;
        result->x.y += matrices[i]->x.y;
        result->x.z += matrices[i]->x.z;
        result->x.w += matrices[i]->x.w;

        result->y.x += matrices[i]->y.x;
        result->y.y += matrices[i]->y.y;
        result->y.z += matrices[i]->y.z;
        result->y.w += matrices[i]->y.w;

        result->z.x += matrices[i]->z.x;
        result->z.y += matrices[i]->z.y;
        result->z.z += matrices[i]->z.z;
        result->z.w += matrices[i]->z.w;

        result->w.x += matrices[i]->w.x;
        result->w.y += matrices[i]->w.y;
        result->w.z += matrices[i]->w.z;
        result->w.w += matrices[i]->w.w;
    }

    return 0;
}

//Subtract any number of matrices at once
ERROR_NUM matrix_sub(mat4x4* matrices[], int count, mat4x4* result) {

    if(matrices[0] == NULL) return MATLIB_POINTER_ERROR;

    result->x.x += matrices[0]->x.x;
    result->x.y += matrices[0]->x.y;
    result->x.z += matrices[0]->x.z;
    result->x.w += matrices[0]->x.w;

    result->y.x += matrices[0]->y.x;
    result->y.y += matrices[0]->y.y;
    result->y.z += matrices[0]->y.z;
    result->y.w += matrices[0]->y.w;

    result->z.x += matrices[0]->z.x;
    result->z.y += matrices[0]->z.y;
    result->z.z += matrices[0]->z.z;
    result->z.w += matrices[0]->z.w;

    result->w.x += matrices[0]->w.x;
    result->w.y += matrices[0]->w.y;
    result->w.z += matrices[0]->w.z;
    result->w.w += matrices[0]->w.w;

    for(int i = 1; i < count; i++) {

        if(matrices[i] == NULL) return MATLIB_POINTER_ERROR;

        result->x.x -= matrices[i]->x.x;
        result->x.y -= matrices[i]->x.y;
        result->x.z -= matrices[i]->x.z;
        result->x.w -= matrices[i]->x.w;

        result->y.x -= matrices[i]->y.x;
        result->y.y -= matrices[i]->y.y;
        result->y.z -= matrices[i]->y.z;
        result->y.w -= matrices[i]->y.w;

        result->z.x -= matrices[i]->z.x;
        result->z.y -= matrices[i]->z.y;
        result->z.z -= matrices[i]->z.z;
        result->z.w -= matrices[i]->z.w;

        result->w.x -= matrices[i]->w.x;
        result->w.y -= matrices[i]->w.y;
        result->w.z -= matrices[i]->w.z;
        result->w.w -= matrices[i]->w.w;
    }

    return 0;
}

//Matrix-matrix multiplication, 2 ONLY
ERROR_NUM matxmat(mat4x4* matrix1, mat4x4* matrix2, mat4x4* result) {

    if(matrix1 == NULL || matrix2 == NULL) return MATLIB_POINTER_ERROR;

    result->x.x = (matrix1->x.x * matrix2->x.x) + (matrix1->y.x * matrix2->x.y) + (matrix1->z.x * matrix2->x.z) + (matrix1->w.x * matrix2->x.w); 
    result->x.y = (matrix1->x.y * matrix2->x.x) + (matrix1->y.y * matrix2->x.y) + (matrix1->z.y * matrix2->x.z) + (matrix1->w.y * matrix2->x.w);
    result->x.z = (matrix1->x.z * matrix2->x.x) + (matrix1->y.z * matrix2->x.y) + (matrix1->z.z * matrix2->x.z) + (matrix1->w.z * matrix2->x.w); 
    result->x.w = (matrix1->x.w * matrix2->x.x) + (matrix1->y.w * matrix2->x.y) + (matrix1->z.w * matrix2->x.z) + (matrix1->w.w * matrix2->x.w); 

    return 0;
}

//Matrix-vector multiplication, 2 ONLY
ERROR_NUM matxvec(mat4x4* matrix, vector4* vector, vector4* result) {
    return 0;
}
