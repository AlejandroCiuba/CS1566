/**
 * Designed to test header files
 * and various matrix library functions
 */
#include "matrix_def.h"
#include "matrix_utility.h"
#include "matrix_ops.h"
#include <stdio.h>
#include <stdlib.h>

int main() {

    //TEST BASE MATRICES
    printf("NOW TESTING PRINT FUNCTIONS\n");

    vector4 vec_test = {2.4, 2.343, 2.33, 4.33};
    print_vector(vec_test);

    mat4x4 mat_test = {vec_test, vec_test, vec_test, vec_test};
    print_matrix(mat_test);

    print_GLfloat(mat_test.y.z);

    //TEST WITH POINTERS
    vector4* vec_ptr = &vec_test;
    if(print_vector_ptr(vec_ptr) == -2) goto pointer_err;

    mat4x4* mat_ptr =  &mat_test;
    if(print_matrix_ptr(mat_ptr) == -2) goto pointer_err;

    if(print_GLfloat_ptr(&(mat_ptr->y.z)) == -2) goto pointer_err;

    //TEST FOR NULL POINTER ERROR
    /*vector4* vec_null = NULL;
    if(print_vector_ptr(vec_null) == -2) goto err;*/

    vector4* vec_ptr2 = (vector4*) malloc(sizeof(vector4));
    vec_ptr2->x = 694.5;
    vec_ptr2->y = 23.5;
    vec_ptr2->z = 3456.22;
    vec_ptr2->w = 2342.2342;

    printf("\nTESTING VEC_TO_MAT()\n");
    mat4x4* vec_mat_ptr = vec_to_mat(vec_ptr2);
    if(print_matrix_ptr(vec_mat_ptr) == -2) goto pointer_err;

    printf("\nNotice how we do not free the vector in vec_to_mac\n");
    if(print_vector_ptr(vec_ptr2) == -2) goto pointer_err;

    free(vec_mat_ptr);
    free(vec_ptr2);

    //TESTING SCALR OPERATION ON MATRIX AND VECTOR
    printf("\nNOW TESTING SCALAR OPERATION ON VECTOR AND MATRIX\n");

    vector4 vec = {1, 2, 3, 4};
    print_vector(vec);

    char alpha_string[6];
    printf("\nALPHA WILL BE(Max 6 digist): ");
    fgets(alpha_string, 6, stdin);
    GLfloat alpha = atof(alpha_string);

    printf("\nAFTER SCALAR %f IS APPLIED: \n", alpha);

    scalar(&vec, alpha, 0);
    print_vector(vec);

    printf("Successful run\n");

    return 0;

    //Error handling
    pointer_err:
        printf("Something went wrong\n");
        return -1;
}