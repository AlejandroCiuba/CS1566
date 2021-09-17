/**
 * Matrix and vector math operations
 * 
 * Created by Alejandro Ciuba
 */

#include "matrix_ops.h"
#include <math.h>
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

    if(matrix == NULL || inverse == NULL) return MATLIB_POINTER_ERROR;

    mat4x4 min = {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};

    if(minor(matrix, &min) == MATLIB_POINTER_ERROR) return MATLIB_POINTER_ERROR;

    GLfloat det_inv = 1 / ((matrix->x.x * min.x.x) - (matrix->x.y * min.x.y) + (matrix->x.z * min.x.z) - (matrix->x.w * min.x.w));

    cofactor(&min);
    transpose(&min);

    scalar(&min, det_inv, 1);

    *inverse = min; 

    return 0;
}

//Returns the identity matrix
ERROR_NUM identity(mat4x4* identity) {

    if(identity == NULL) return MATLIB_POINTER_ERROR;

    identity->x.x = 1; identity->y.x = 0; identity->z.x = 0; identity->w.x = 0;
    identity->x.y = 0; identity->y.y = 1; identity->z.y = 0; identity->w.y = 0;
    identity->x.z = 0; identity->y.z = 0; identity->z.z = 1; identity->w.z = 0;
    identity->x.w = 0; identity->y.w = 0; identity->z.w = 0; identity->w.w = 1;

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

    //NO Z IN ROW
    det_mat mat31 = {mat.y.x, mat.z.x, mat.w.x, mat.y.y, mat.z.y, mat.w.y, mat.y.w, mat.z.w, mat.w.w};
    det_mat mat32 = {mat.x.x, mat.z.x, mat.w.x, mat.x.y, mat.z.y, mat.w.y, mat.x.w, mat.z.w, mat.w.w};
    det_mat mat33 = {mat.x.x, mat.y.x, mat.w.x, mat.x.y, mat.y.y, mat.w.y, mat.x.w, mat.y.w, mat.w.w};
    det_mat mat34 = {mat.x.x, mat.y.x, mat.z.x, mat.x.y, mat.y.y, mat.z.y, mat.x.w, mat.y.w, mat.z.w};

    //NO W IN ROW
    det_mat mat41 = {mat.y.x, mat.z.x, mat.w.x, mat.y.y, mat.z.y, mat.w.y, mat.y.z, mat.z.z, mat.w.z};
    det_mat mat42 = {mat.x.x, mat.z.x, mat.w.x, mat.x.y, mat.z.y, mat.w.y, mat.x.z, mat.z.z, mat.w.z};
    det_mat mat43 = {mat.x.x, mat.y.x, mat.w.x, mat.x.y, mat.y.y, mat.w.y, mat.x.z, mat.y.z, mat.w.z};
    det_mat mat44 = {mat.x.x, mat.y.x, mat.z.x, mat.x.y, mat.y.y, mat.z.y, mat.x.z, mat.y.z, mat.z.z};

    //Put them in an array
    GLfloat fl11 = (mat11[0] * mat11[4] * mat11[8]) + (mat11[1] * mat11[5] * mat11[6]) + (mat11[2] * mat11[3] * mat11[7])
     - (mat11[6] * mat11[4] * mat11[2]) - (mat11[7] * mat11[5] * mat11[0]) - (mat11[8] * mat11[3] * mat11[1]); 
    GLfloat fl12 = (mat12[0] * mat12[4] * mat12[8]) + (mat12[1] * mat12[5] * mat12[6]) + (mat12[2] * mat12[3] * mat12[7])
     - (mat12[6] * mat12[4] * mat12[2]) - (mat12[7] * mat12[5] * mat12[0]) - (mat12[8] * mat12[3] * mat12[1]); 
    GLfloat fl13 = (mat13[0] * mat13[4] * mat13[8]) + (mat13[1] * mat13[5] * mat13[6]) + (mat13[2] * mat13[3] * mat13[7])
     - (mat13[6] * mat13[4] * mat13[2]) - (mat13[7] * mat13[5] * mat13[0]) - (mat13[8] * mat13[3] * mat13[1]); 
    GLfloat fl14 = (mat14[0] * mat14[4] * mat14[8]) + (mat14[1] * mat14[5] * mat14[6]) + (mat14[2] * mat14[3] * mat14[7])
     - (mat14[6] * mat14[4] * mat14[2]) - (mat14[7] * mat14[5] * mat14[0]) - (mat14[8] * mat14[3] * mat14[1]); 

    GLfloat fl21 = (mat21[0] * mat21[4] * mat21[8]) + (mat21[1] * mat21[5] * mat21[6]) + (mat21[2] * mat21[3] * mat21[7])
     - (mat21[6] * mat21[4] * mat21[2]) - (mat21[7] * mat21[5] * mat21[0]) - (mat21[8] * mat21[3] * mat21[1]); 
    GLfloat fl22 = (mat22[0] * mat22[4] * mat22[8]) + (mat22[1] * mat22[5] * mat22[6]) + (mat22[2] * mat22[3] * mat22[7])
     - (mat22[6] * mat22[4] * mat22[2]) - (mat22[7] * mat22[5] * mat22[0]) - (mat22[8] * mat22[3] * mat22[1]); 
    GLfloat fl23 = (mat23[0] * mat23[4] * mat23[8]) + (mat23[1] * mat23[5] * mat23[6]) + (mat23[2] * mat23[3] * mat23[7])
     - (mat23[6] * mat23[4] * mat23[2]) - (mat23[7] * mat23[5] * mat23[0]) - (mat23[8] * mat23[3] * mat23[1]); 
    GLfloat fl24 = (mat24[0] * mat24[4] * mat24[8]) + (mat24[1] * mat24[5] * mat24[6]) + (mat24[2] * mat24[3] * mat24[7])
     - (mat24[6] * mat24[4] * mat24[2]) - (mat24[7] * mat24[5] * mat24[0]) - (mat24[8] * mat24[3] * mat24[1]); 

    GLfloat fl31 = (mat31[0] * mat31[4] * mat31[8]) + (mat31[1] * mat31[5] * mat31[6]) + (mat31[2] * mat31[3] * mat31[7])
     - (mat31[6] * mat31[4] * mat31[2]) - (mat31[7] * mat31[5] * mat31[0]) - (mat31[8] * mat31[3] * mat31[1]); 
    GLfloat fl32 = (mat32[0] * mat32[4] * mat32[8]) + (mat32[1] * mat32[5] * mat32[6]) + (mat32[2] * mat32[3] * mat32[7])
     - (mat32[6] * mat32[4] * mat32[2]) - (mat32[7] * mat32[5] * mat32[0]) - (mat32[8] * mat32[3] * mat32[1]); 
    GLfloat fl33 = (mat33[0] * mat33[4] * mat33[8]) + (mat33[1] * mat33[5] * mat33[6]) + (mat33[2] * mat33[3] * mat33[7])
     - (mat33[6] * mat33[4] * mat33[2]) - (mat33[7] * mat33[5] * mat33[0]) - (mat33[8] * mat33[3] * mat33[1]); 
    GLfloat fl34 = (mat34[0] * mat34[4] * mat34[8]) + (mat34[1] * mat34[5] * mat34[6]) + (mat34[2] * mat34[3] * mat34[7])
     - (mat34[6] * mat34[4] * mat34[2]) - (mat34[7] * mat34[5] * mat34[0]) - (mat34[8] * mat34[3] * mat34[1]);

    GLfloat fl41 = (mat41[0] * mat41[4] * mat41[8]) + (mat41[1] * mat41[5] * mat41[6]) + (mat41[2] * mat41[3] * mat41[7])
     - (mat41[6] * mat41[4] * mat41[2]) - (mat41[7] * mat41[5] * mat41[0]) - (mat41[8] * mat41[3] * mat41[1]); 
    GLfloat fl42 = (mat42[0] * mat42[4] * mat42[8]) + (mat42[1] * mat42[5] * mat42[6]) + (mat42[2] * mat42[3] * mat42[7])
     - (mat42[6] * mat42[4] * mat42[2]) - (mat42[7] * mat42[5] * mat42[0]) - (mat42[8] * mat42[3] * mat42[1]); 
    GLfloat fl43 = (mat43[0] * mat43[4] * mat43[8]) + (mat43[1] * mat43[5] * mat43[6]) + (mat43[2] * mat43[3] * mat43[7])
     - (mat43[6] * mat43[4] * mat43[2]) - (mat43[7] * mat43[5] * mat43[0]) - (mat43[8] * mat43[3] * mat43[1]); 
    GLfloat fl44 = (mat44[0] * mat44[4] * mat44[8]) + (mat44[1] * mat44[5] * mat44[6]) + (mat44[2] * mat44[3] * mat44[7])
     - (mat44[6] * mat44[4] * mat44[2]) - (mat44[7] * mat44[5] * mat44[0]) - (mat44[8] * mat44[3] * mat44[1]);

    vector4 vecx = {fl11, fl21, fl31, fl41};
    vector4 vecy = {fl12, fl22, fl32, fl42};
    vector4 vecz = {fl13, fl23, fl33, fl43};
    vector4 vecw = {fl14, fl24, fl34, fl44}; 

    minor->x = vecx;
    minor->y = vecy;
    minor->z = vecz;
    minor->w = vecw;

    return 0;
}

//Flips signs of a matrix, affects original matrix
ERROR_NUM cofactor(mat4x4* matrix) {

    if(matrix == NULL) return MATLIB_POINTER_ERROR;

    matrix->y.x *= -1; matrix->w.x *= -1;
    matrix->x.y *= -1; matrix->z.y *= -1;
    matrix->y.z *= -1; matrix->w.z *= -1;
    matrix->x.w *= -1; matrix->z.w *= -1;

    return 0;
}

//Calculates the determinant of a matrix
ERROR_NUM determinant(mat4x4* matrix, GLfloat* result) {

    if(matrix == NULL || result == NULL) return MATLIB_POINTER_ERROR;

    mat4x4 mat = *matrix;

    typedef GLfloat det_mat[9];

    //ROWxCOL for det_mat but COLxROW for mat
    //NO X IN ROW
    det_mat mat11 = {mat.y.y, mat.z.y, mat.w.y, mat.y.z, mat.z.z, mat.w.z, mat.y.w, mat.z.w, mat.w.w};
    det_mat mat12 = {mat.x.y, mat.z.y, mat.w.y, mat.x.z, mat.z.z, mat.w.z, mat.x.w, mat.z.w, mat.w.w};
    det_mat mat13 = {mat.x.y, mat.y.y, mat.w.y, mat.x.z, mat.y.z, mat.w.z, mat.x.w, mat.y.w, mat.w.w};
    det_mat mat14 = {mat.x.y, mat.y.y, mat.z.y, mat.x.z, mat.y.z, mat.z.z, mat.x.w, mat.y.w, mat.z.w};

    //Put them in an array
    GLfloat fl11 = (mat11[0] * mat11[4] * mat11[8]) + (mat11[1] * mat11[5] * mat11[6]) + (mat11[2] * mat11[3] * mat11[7])
     - (mat11[6] * mat11[4] * mat11[2]) - (mat11[7] * mat11[5] * mat11[0]) - (mat11[8] * mat11[3] * mat11[1]); 
    GLfloat fl12 = (mat12[0] * mat12[4] * mat12[8]) + (mat12[1] * mat12[5] * mat12[6]) + (mat12[2] * mat12[3] * mat12[7])
     - (mat12[6] * mat12[4] * mat12[2]) - (mat12[7] * mat12[5] * mat12[0]) - (mat12[8] * mat12[3] * mat12[1]); 
    GLfloat fl13 = (mat13[0] * mat13[4] * mat13[8]) + (mat13[1] * mat13[5] * mat13[6]) + (mat13[2] * mat13[3] * mat13[7])
     - (mat13[6] * mat13[4] * mat13[2]) - (mat13[7] * mat13[5] * mat13[0]) - (mat13[8] * mat13[3] * mat13[1]); 
    GLfloat fl14 = (mat14[0] * mat14[4] * mat14[8]) + (mat14[1] * mat14[5] * mat14[6]) + (mat14[2] * mat14[3] * mat14[7])
     - (mat14[6] * mat14[4] * mat14[2]) - (mat14[7] * mat14[5] * mat14[0]) - (mat14[8] * mat14[3] * mat14[1]); 

    *result = (mat.x.x * fl11) - (mat.y.x * fl12) + (mat.z.x * fl13) - (mat.w.x * fl14);

    return 0;
}