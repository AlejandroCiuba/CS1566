/**
 * Defines various matrix and vector math operations
 * 
 * Created by Alejandro Ciuba
 */

#ifndef MATRIX_OPS_H
#define MATRIX_OPS_H

#include "matrix_def.h"

extern mat4x4 identity, zero_matrix;
extern vector4 zero_vector, x_axis, y_axis, z_axis;

// ============== VECTOR AND MATRIX MATH FUNCTIONS ==============

// For args, 0 = vector, 1 = matrix4x4
ERROR_NUM scalar(const void* matrix, const GLfloat alpha, const char args);

// ============== VECTOR MATH FUNCTIONS ==============

// Add any number of vectors at once
ERROR_NUM vector_add(vector4* vectors[], int count, vector4* result);

// Subtract any number of vectors at once
ERROR_NUM vector_sub(vector4* vectors[], int count, vector4* result);

// Returns that vectors magnitude
ERROR_NUM vector_mag(const vector4* vec, GLfloat* result);

// Normalizes the given vector
ERROR_NUM vector_norm(const vector4* vec);

// Normalize the given vector separately
ERROR_NUM vector_norm_sep(const vector4* vec, vector4* result);

// Returns the Dot Product of 2 given vectors
ERROR_NUM vector_dot(const vector4* vec1, const vector4* vec2, GLfloat* result);

ERROR_NUM vector_cross(const vector4* vec1, const vector4* vec2, vector4* result);

// ============== MATRIX MATH FUNCTIONS ==============

// Add any number of matrices at once
ERROR_NUM matrix_add(mat4x4* matrices[], int count, mat4x4* result);

// Subtract any number of matrices at once
ERROR_NUM matrix_sub(mat4x4* matrices[], int count, mat4x4* result);

// Matrix-matrix multiplication, 2 ONLY
ERROR_NUM matxmat(mat4x4* matrix1, mat4x4* matrix2, mat4x4* result);

// Matrix-vector multiplication, 2 ONLY
ERROR_NUM matxvec(mat4x4* matrix, vector4* vector, vector4* result);

// Matrix-vector array multiplication
ERROR_NUM matxvar(mat4x4* matrix, vector4* varray, int varray_count, vector4* result);

// Transpose a matrix
ERROR_NUM transpose(mat4x4* matrix);

// Transpose a matrix with a separate matrix to store the results
ERROR_NUM transpose_sep(mat4x4* matrix, mat4x4* result);

// Inverse of a matrix
ERROR_NUM inverse(mat4x4* matrix, mat4x4* inverse);

// Returns the identity matrix
ERROR_NUM identity_fill(mat4x4* identity);

// Returns a minor matrix for a given matrix
ERROR_NUM minor(mat4x4* matrix, mat4x4* minor);

// Flips signs of a matrix, affects original matrix
ERROR_NUM cofactor(mat4x4* matrix);

// Calculates the determinant of a matrix
ERROR_NUM determinant(mat4x4* matrix, GLfloat* result);

// Multiply any number of matrices in a certain order
ERROR_NUM mat_mult(mat4x4* matrices, int count, mat4x4* result);

#endif