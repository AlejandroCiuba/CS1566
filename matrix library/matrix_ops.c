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

ERROR_NUM vector_cross(const vector4* vec1, const vector4* vec2, vector4* result) {

    if(vec1 == NULL || vec2 == NULL || result == NULL) return MATLIB_POINTER_ERROR;

    result->x = (vec1->y * vec2->z) - (vec1->z * vec2->y);
    result->y = (vec1->z * vec2->x) - (vec1->x * vec2->z);
    result->z = (vec1->x * vec2->y) - (vec1->y * vec2->x);
    result->w = 0.0f;

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

    if(matrix1 == NULL || matrix2 == NULL || result == NULL) return MATLIB_POINTER_ERROR;

    result->x.x = (matrix1->x.x * matrix2->x.x) + (matrix1->y.x * matrix2->x.y) + (matrix1->z.x * matrix2->x.z) + (matrix1->w.x * matrix2->x.w); 
    result->x.y = (matrix1->x.y * matrix2->x.x) + (matrix1->y.y * matrix2->x.y) + (matrix1->z.y * matrix2->x.z) + (matrix1->w.y * matrix2->x.w);
    result->x.z = (matrix1->x.z * matrix2->x.x) + (matrix1->y.z * matrix2->x.y) + (matrix1->z.z * matrix2->x.z) + (matrix1->w.z * matrix2->x.w); 
    result->x.w = (matrix1->x.w * matrix2->x.x) + (matrix1->y.w * matrix2->x.y) + (matrix1->z.w * matrix2->x.z) + (matrix1->w.w * matrix2->x.w);

    result->y.x = (matrix1->x.x * matrix2->y.x) + (matrix1->y.x * matrix2->y.y) + (matrix1->z.x * matrix2->y.z) + (matrix1->w.x * matrix2->y.w); 
    result->y.y = (matrix1->x.y * matrix2->y.x) + (matrix1->y.y * matrix2->y.y) + (matrix1->z.y * matrix2->y.z) + (matrix1->w.y * matrix2->y.w);
    result->y.z = (matrix1->x.z * matrix2->y.x) + (matrix1->y.z * matrix2->y.y) + (matrix1->z.z * matrix2->y.z) + (matrix1->w.z * matrix2->y.w); 
    result->y.w = (matrix1->x.w * matrix2->y.x) + (matrix1->y.w * matrix2->y.y) + (matrix1->z.w * matrix2->y.z) + (matrix1->w.w * matrix2->y.w); 

    result->z.x = (matrix1->x.x * matrix2->z.x) + (matrix1->y.x * matrix2->z.y) + (matrix1->z.x * matrix2->z.z) + (matrix1->w.x * matrix2->z.w); 
    result->z.y = (matrix1->x.y * matrix2->z.x) + (matrix1->y.y * matrix2->z.y) + (matrix1->z.y * matrix2->z.z) + (matrix1->w.y * matrix2->z.w);
    result->z.z = (matrix1->x.z * matrix2->z.x) + (matrix1->y.z * matrix2->z.y) + (matrix1->z.z * matrix2->z.z) + (matrix1->w.z * matrix2->z.w); 
    result->z.w = (matrix1->x.w * matrix2->z.x) + (matrix1->y.w * matrix2->z.y) + (matrix1->z.w * matrix2->z.z) + (matrix1->w.w * matrix2->z.w); 

    result->w.x = (matrix1->x.x * matrix2->w.x) + (matrix1->y.x * matrix2->w.y) + (matrix1->z.x * matrix2->w.z) + (matrix1->w.x * matrix2->w.w); 
    result->w.y = (matrix1->x.y * matrix2->w.x) + (matrix1->y.y * matrix2->w.y) + (matrix1->z.y * matrix2->w.z) + (matrix1->w.y * matrix2->w.w);
    result->w.z = (matrix1->x.z * matrix2->w.x) + (matrix1->y.z * matrix2->w.y) + (matrix1->z.z * matrix2->w.z) + (matrix1->w.z * matrix2->w.w); 
    result->w.w = (matrix1->x.w * matrix2->w.x) + (matrix1->y.w * matrix2->w.y) + (matrix1->z.w * matrix2->w.z) + (matrix1->w.w * matrix2->w.w); 

    return 0;
}

//Matrix-vector multiplication, 2 ONLY
ERROR_NUM matxvec(mat4x4* matrix, vector4* vector, vector4* result) {

    if(matrix == NULL || vector == NULL || result == NULL) return MATLIB_POINTER_ERROR;

    result->x = (matrix->x.x * vector->x) + (matrix->y.x * vector->y) + (matrix->z.x * vector->z) + (matrix->w.x * vector->w);
    result->y = (matrix->x.y * vector->x) + (matrix->y.y * vector->y) + (matrix->z.y * vector->z) + (matrix->w.y * vector->w);
    result->z = (matrix->x.z * vector->x) + (matrix->y.z * vector->y) + (matrix->z.z * vector->z) + (matrix->w.z * vector->w);
    result->w = (matrix->x.w * vector->x) + (matrix->y.w * vector->y) + (matrix->z.w * vector->z) + (matrix->w.w * vector->w);

    return 0;
}

//Transpose a matrix
ERROR_NUM transpose(mat4x4* matrix) {

    if(matrix == NULL) return MATLIB_POINTER_ERROR;

    mat4x4 store = *matrix;

    matrix->x.x = store.x.x; matrix->x.y = store.y.x; matrix->x.z = store.z.x; matrix->x.w = store.w.x;
    matrix->y.x = store.x.y; matrix->y.y = store.y.y; matrix->y.z = store.z.y; matrix->y.w = store.w.y;
    matrix->z.x = store.x.z; matrix->z.y = store.y.z; matrix->z.z = store.z.z; matrix->z.w = store.w.z;
    matrix->w.x = store.x.w; matrix->w.y = store.y.w; matrix->w.z = store.z.w; matrix->w.w = store.w.w;

    return 0;
}

//Transpose a matrix with a separate matrix to store the results
ERROR_NUM transpose_sep(mat4x4* matrix, mat4x4* result) {

    if(matrix == NULL || result == NULL) return MATLIB_POINTER_ERROR;

    result->x.x = matrix->x.x; result->x.y = matrix->y.x; result->x.z = matrix->z.x; result->x.w = matrix->w.x;
    result->y.x = matrix->x.y; result->y.y = matrix->y.y; result->y.z = matrix->z.y; result->y.w = matrix->w.y;
    result->z.x = matrix->x.z; result->z.y = matrix->y.z; result->z.z = matrix->z.z; result->z.w = matrix->w.z;
    result->w.x = matrix->x.w; result->w.y = matrix->y.w; result->w.z = matrix->z.w; result->w.w = matrix->w.w;

    return 0;
}

//Inverse of a matrix
ERROR_NUM inverse(mat4x4* matrix, mat4x4* inverse) {
    return 0;
}

//Returns the identity matrix
ERROR_NUM identity(mat4x4* identity) {

    if(identity == NULL) return MATLIB_POINTER_ERROR;

    for(char i = 0; i < 16; i++) *(((GLfloat*) identity) + i) = (i % 5 == 0)? 1 : 0;

    return 0;
}

//Returns a minor matrix for a given matrix
ERROR_NUM minor(mat4x4* matrix, mat4x4* minor){

    if(matrix == NULL || minor == NULL) return MATLIB_POINTER_ERROR;

    mat4x4 mat = *matrix;

    typedef GLfloat det_mat[9];

    //ROWxCOL for det_mat but COLxROW for mat
    //NO X IN ROW
    det_mat mat11 = {mat.y.y, mat.z.y, mat.w.y, mat.y.z, mat.z.z, mat.w.z, mat.y.w, mat.z.w, mat.w.w};
    det_mat mat12 = {mat.x.y, mat.z.y, mat.w.y, mat.x.z, mat.z.z, mat.w.z, mat.x.w, mat.z.w, mat.w.w};
    det_mat mat13 = {mat.x.y, mat.y.y, mat.w.y, mat.x.z, mat.y.z, mat.w.z, mat.x.w, mat.y.w, mat.w.w};
    det_mat mat14 = {mat.x.y, mat.y.y, mat.z.y, mat.x.z, mat.y.z, mat.z.z, mat.x.w, mat.y.w, mat.z.w};

    //NO Y IN ROW
    det_mat mat21 = {mat.y.x, mat.z.x, mat.w.x, mat.y.z, mat.z.z, mat.w.z, mat.y.w, mat.z.w, mat.w.w};
    det_mat mat22 = {mat.x.x, mat.z.x, mat.w.x, mat.x.z, mat.z.z, mat.w.z, mat.x.w, mat.z.w, mat.w.w};
    det_mat mat23 = {mat.x.x, mat.y.x, mat.w.x, mat.x.z, mat.y.z, mat.w.z, mat.x.w, mat.y.w, mat.w.w};
    det_mat mat24 = {mat.x.x, mat.y.x, mat.z.x, mat.x.z, mat.y.z, mat.z.z, mat.x.w, mat.y.w, mat.z.w};

    det_mat mat31;
    det_mat mat32;
    det_mat mat33;
    det_mat mat34;

    det_mat mat41;
    det_mat mat42;
    det_mat mat43;
    det_mat mat44;

    return 0;
}

//Flips signs of a matrix, affects original matrix
ERROR_NUM cofactor(mat4x4* matrix) {
    return 0;
}

//Calculates the determinant of a matrix
ERROR_NUM determinant(mat4x4* matrix, GLfloat* result) {
    return 0;
}