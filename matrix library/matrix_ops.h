/**
 * Defines various matrix and vector math operations
 * 
 * Created by Alejandro Ciuba
 */

#ifndef MATRIX_OPS_H
#define MATRIX_OPS_H

#include "matrix_def.h"

//============== VECTOR AND MATRIX MATH FUNCTIONS ==============
ERROR_NUM scalar(const void* matrix, const GLfloat alpha, const char args);

//============== VECTOR MATH FUNCTIONS ==============
//Add any number of vectors at once
ERROR_NUM vector_add(vector4* vectors[], int count, vector4* result);

//Subtract any number of vectors at once
ERROR_NUM vector_sub(vector4* vectors[], int count, vector4* result);

//Returns that vectors magnitude
ERROR_NUM vector_mag(const vector4* vec, GLfloat* result);

#endif