/**
 * Designed to test header files
 */
#include "matrix_def.h"
#include "matrix_utility.h"
#include <stdio.h>

int main() {
    vector4 vec_test = {2.4, 2.343, 2.33, 4.33};
    print_vector(vec_test);

    mat4x4 mat = {vec_test, vec_test, vec_test, vec_test};
    print_matrix(mat);

    print_GLfloat(mat.y.z);

    printf("Successful run\n");

    return 0;

    //Error handling
    err:
        printf("Something went wrong");
}