/**
 * Designed to test header files
 */
#include "matrix_def.h"
#include "matrix_utility.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    //TEST BASE MATRICES
    vector4 vec_test = {2.4, 2.343, 2.33, 4.33};
    print_vector(vec_test);

    mat4x4 mat_test = {vec_test, vec_test, vec_test, vec_test};
    print_matrix(mat_test);

    print_GLfloat(mat_test.y.z);

    //TEST WITH POINTERS

    vector4* vec_ptr = &vec_test;
    if(print_vector_ptr(vec_ptr) == -2) goto err;

    mat4x4* mat_ptr =  &mat_test;
    if(print_matrix_ptr(mat_ptr) == -2) goto err;

    if(print_GLfloat_ptr(&(mat_ptr->y.z)) == -2) goto err;

    printf("Successful run\n");

    return 0;

    //Error handling
    err:
        printf("Something went wrong\n");
        return -1;
}