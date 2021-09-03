/**
 * Matrix definition header for the rest of the
 * Matrix library. Defines GLfoat, vector4, and
 * maxtrix 4x4 needed for OpenGL functions
 * 
 * Created by Alejandro Ciuba
 */

#ifndef MATRIX_DEF_H
#define MATRIX_DEF_H

//Defines datatype for OpenGL
typedef float GLfloat;

//Defines base Calumn Vector needed for OpenGL functions
typedef struct vector4 {
    GLfloat x;
    GLfloat y;
    GLfloat z;
    GLfloat w;
} vector4;

typedef struct matrix4x4 {
    vector4 x;
    vector4 y;
    vector4 z;
    vector4 w;
} mat4x4;

#endif