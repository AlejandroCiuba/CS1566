/**
 * Test program to demonstrate matrix
 * and vector functions in a controlled
 * environment. Shown with error handling
 * for functions.
 * 
 * @author Alejandro Ciuba
 */

#include <stdlib.h>
#include <stdio.h>

#include "matrix_library/matrix_def.h" //Defines matrix and vector, as well as error codes
#include "matrix_library/matrix_ops.h" //Defines matrix and vector operations
#include "matrix_library/matrix_utility.h" //Defines utility functions for debugging and convenience

int main() {

    printf("\n===================== TESTING MATRIX LIBRARY =====================\n");

    vector4* v1 = (vector4*) malloc(sizeof(vector4));
    vector4* v1add = (vector4*) malloc(sizeof(vector4));
    vector4* v1dot = (vector4*) malloc(sizeof(vector4));
    for(int i = 0; i < 4; i++) ((GLfloat*) v1)[i] = i + 1;

    copy_vector(v1, v1add);
    copy_vector(v1, v1dot);

    printf("\nFirst Vector V1\n");
    if(print_vector_ptr(v1) != 0) return -1;

    printf("\nScalar Multiplication Of V1 With Value Of 3.0\n");
    if(scalar(v1, 3.0, 0) != 0) return -1;
    if(print_vector_ptr(v1) != 0) return -1;

    free(v1);

    //===================== PART 2 =====================

    vector4* v2 = (vector4*) malloc(sizeof(vector4));
    for(int i = 0; i < 4; i++) ((GLfloat*) v2)[i] = i + 5;

    printf("\nSecond Vector V2\n");
    if(print_vector_ptr(v2) != 0) return -1;

    printf("\nV1 + V2\n");

    vector4* result = zero_vector();
    vector4* arr[] = {v1add, v2};

    if(vector_add(arr, 2, result) != 0) return -1;
    if(print_vector_ptr(result) != 0) return -1;

    free(result);
    result = zero_vector();

    printf("\nV1 - V2\n");

    if(vector_sub(arr, 2, result) != 0) return -1;
    if(print_vector_ptr(result) != 0) return -1;

    free(result);

    //===================== PART 3 =====================

    printf("\nMagnitude of V1\n");
    GLfloat mag = 0.0;

    if(vector_mag(v1add, &mag) != 0) return -1;
    print_GLfloat(mag);

    if(vector_norm(v1add) != 0) return -1;
    print_vector_ptr(v1add); //If you didn't want to check
    
    free(v1add);

    //===================== PART 4 =====================

    printf("\nDot Product Of V1 And V2\n");
    GLfloat dot = 0.0;

    if(vector_dot(v1dot, v2, &dot) != 0) return -1;
    print_GLfloat(dot);

    //I have demonstrated enough my library's pointer handling
    //I will pass in local addresses from now on
    
    free(v1dot);
    free(v2);

    return 0;
}