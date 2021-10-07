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

//Prints the given vector
void print_vector(vector4 vec);

//Prints the given 4x4 matrix
void print_matrix(mat4x4 mat);

//Prints the given GLfloat 
void print_GLfloat(GLfloat flt);

//Pointer versions of print debugging functions

ERROR_NUM print_vector_ptr(const vector4* vec);

ERROR_NUM print_matrix_ptr(const mat4x4* mat);

ERROR_NUM print_GLfloat_ptr(const GLfloat* flt);

//Returns a pointer to a 0 vector4
ERROR_NUM zero_vector(vector4* vec);

//Returns a pointer to a 0 matrix
ERROR_NUM zero_matrix(mat4x4* mat);

//Copies stuff from one vector to another
ERROR_NUM copy_vector(const vector4* og, vector4* cpy);

ERROR_NUM copy_matrix(const mat4x4* og, mat4x4* cpy);

//Makes a vector4 into a mat4x4 with the values
//Copied into every column
mat4x4* vec_to_mat(const vector4* vec);

//Creates an matrix/vector filled with the
//passed in value
ERROR_NUM fill_matrix(mat4x4* mat, const GLfloat flt);

ERROR_NUM fill_vector(vector4* vec, const GLfloat flt);

//Tells if two vectors are equal
bool vector_equal(vector4* vec1, vector4* vec2);

//Tells if two matrices are equal
bool matrix_equal(mat4x4* mat1, mat4x4* mat2);

//Convert screen coordinates to world coordinates
ERROR_NUM screen_to_world(vector4* screen, vector4* result, int x_screen, int y_screen, GLfloat z_treatment(GLfloat, GLfloat));

#endif