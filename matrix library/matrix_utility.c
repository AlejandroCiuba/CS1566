/**
 * definitions for the matrix_utility.h functions
 * 
 * Created by Alejandro Ciuba
 */

#include "matrix_utility.h"
#include <stdio.h>

//Prints the given vector
void print_vector(const vector4 vec) {
    printf("[x : %f, y : %f, z : %f, w : %f]\n", vec.x, vec.y, vec.z, vec.w);
}

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
