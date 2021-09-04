/**
 * definitions for the matrix_utility.h functions
 * ERROR_NUM to return -2 to signify pointer problem
 * with matrix pointers
 * 
 * Created by Alejandro Ciuba
 */

#include "matrix_utility.h"
#include <stdio.h>

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
    if(vec == NULL) return -2;
    print_vector(*vec);
    return 0;
}

ERROR_NUM print_matrix_ptr(const mat4x4* mat) {
    if(mat == NULL) return -2;
    print_matrix(*mat);
    return 0;
}

ERROR_NUM print_GLfloat_ptr(const GLfloat* flt) {
    if(flt == NULL) return -2;
    print_GLfloat(*flt);
    return 0;
}