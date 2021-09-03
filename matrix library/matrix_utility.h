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

#endif