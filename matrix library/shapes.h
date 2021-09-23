/**
 * Creates vertex arrays of basic shapes
 * Compatible with OpenGL
 * @author Alejandro Ciuba
 */

#ifndef SHAPES_H
#define SHAPES_H

#include "matrix_def.h"

//Assigns a color per shape (assumes triangle based implementation)
ERROR_NUM random_colors(vector4* colors, const int num_vertices);

//Assumes triangle-based implementation
ERROR_NUM circle(vector4* vertices, int count, GLfloat radius, vector4 origin, char align);

ERROR_NUM cone(vector4* vertices, int count, GLfloat radius, GLfloat height, vector4 tip, char align);

#endif