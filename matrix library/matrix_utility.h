/**
 * Matrix functions designed to serve
 * as utility functions to assist in
 * debugging various errors you can 
 * encounter with them.
 * 
 * Created by Alejandro Ciuba
 */

#ifndef MATRIX_UTILITY_H
#define MARTRIX_UTILITY_H

#include "matrix_def.h"

typedef char ERROR_NUM;

//Prints the given vector
void print_vector(const vector4 vec);

//Prints the given 4x4 matrix
void print_matrix(const mat4x4 mat);

//Prints the given GLfloat 
void print_GLfloat(const GLfloat flt);

//Pointer versions of print debugging functions

ERROR_NUM print_vector_ptr(const vector4* vec);

ERROR_NUM print_matrix_ptr(const mat4x4* mat);

ERROR_NUM print_GLfloat_ptr(const GLfloat* flt);

//Makes a vector4 into a mat4x4 with the values
//Copied into every column
mat4x4* vec_to_mat(vector4* vec);

#endif