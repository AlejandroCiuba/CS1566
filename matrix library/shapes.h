/**
 * Creates vertex arrays of basic shapes
 * Compatible with OpenGL
 * 
 * @author Alejandro Ciuba
 */

#ifndef SHAPES_H
#define SHAPES_H

#include "matrix_def.h"

typedef vector4 color;

extern color RED;
extern color GREEN;
extern color BLUE;

typedef struct vector2 {
    GLfloat x;
    GLfloat y;
} vector2;

typedef unsigned char shape;

#define RECTANGLE 0
#define CIRCLE 1
#define FLAT_TORUS 2
#define CONE 3
#define BAND 4
#define SPHERE 5
#define CUSTOM 6

//Assigns a color per shape (assumes triangle based implementation)
ERROR_NUM random_colors(vector4* colors, const int num_vertices);

//Assigns one color to a series of faces
ERROR_NUM const_color(vector4* colors, const int num_vertices, color face_color);

ERROR_NUM texturize(vector2* texcoords, const int count, shape type, void custom(vector2*, int));

//Assumes triangle-based implementation
ERROR_NUM circle(vector4* vertices, int count, GLfloat radius, vector4 origin, char align);

ERROR_NUM cone(vector4* vertices, int count, GLfloat radius, GLfloat height, vector4 tip, char align);

//Assumes 6 vertices
ERROR_NUM rectangle(vector4* vertices, GLfloat height, GLfloat width, vector4 origin);

ERROR_NUM flat_torus(vector4* vertices, int count, GLfloat inner, GLfloat outer, vector4 origin);

ERROR_NUM band(vector4* vertices, int count, GLfloat radius, GLfloat length);

ERROR_NUM sphere(vector4* vertices, int count, int bands, GLfloat radius);

#endif