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
ERROR_NUM vector_add(const vector4** vectors, int count, vector4* final);

#endif