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

    //TESTING VECTOR ADDITION
    printf("\nNOW TESTING VECTOR ADDITION\n");
    printf("VECTOR 1 (PREVIOUS VECTOR):\n");
    print_vector(vec);

    printf("\nVECTOR 2 (PREVIOUS VECTOR WITH ALPHA 2:\n");
    vector4 vec2 = vec;
    scalar(&vec2, 2, 0);
    print_vector(vec2);

    vector4* vec3_ptr = (vector4*) malloc(sizeof(vector4));

    vector4* vec_arr[] = {&vec, &vec2};

    if(vector_add(vec_arr, 2, vec3_ptr) == -2) goto pointer_err;

    printf("\nFINAL RESULT:\n");
    print_vector_ptr(vec3_ptr);

    free(vec3_ptr);

    //TESTING VECTOR MAGNITUDE
    printf("\nTESTING VECTOR MAGNITUDE\n");
    vector4 vec4 = {1.1, 2.2, 3.3, 4.4};
    GLfloat mag = 0.0;

    print_vector(vec4);
    print_GLfloat(mag);
    if(vector_mag(&vec4, &mag) == -2) goto pointer_err;
    printf("\nRESULT: %f\n", mag);

    //TESTING VECTOR NORMALIZATION
    printf("\nTESTING VECTOR NORMALIZATION\n");
    vector4 alltwos = {2.0,2.0,2.0,2.0};
    print_vector(alltwos);

    vector_norm(&alltwos);
    print_vector(alltwos);

    //TESTING DOT PRODUCT
    printf("\nTESTING DOT PRODUCT\n");
    vector4 allthrees = {3.0,3.0,3.0,3.0};
    GLfloat dotty = 0.0f;

    print_vector(allthrees);
    vector_dot(&allthrees, &allthrees, &dotty);
    print_GLfloat(dotty);

    //TESTING MATRIX ADDITION
    printf("\nTESTING MATRIX ADDITION\n");
    vector4 matrix_vec = {1.1,2.2,3.3,4.4};
    mat4x4 matrix1 = {matrix_vec, matrix_vec, matrix_vec, matrix_vec};

    print_matrix(matrix1);
    printf("\n");

    vector4 matrix_vec2 = {2.2,3.3,4.4,5.5};
    mat4x4 matrix2 = {matrix_vec2, matrix_vec2, matrix_vec2,matrix_vec2};

    print_matrix(matrix2);
    printf("\n");

    vector4 matrix_vec3 = {3.3,4.4,5.5,6.6};
    mat4x4 matrix3 = {matrix_vec3,matrix_vec3,matrix_vec3,matrix_vec3};
    
    print_matrix(matrix3);
    printf("\n");


    mat4x4* matrices[] = {&matrix1, &matrix2, &matrix3};
    mat4x4* result = zero_matrix();
 
    matrix_add(matrices, 3, result);
    print_matrix(*result);

    //TESTING MATRIX SUBTRACTION
    printf("\nTESTING MATRIX SUBTRACTION\nSAMEVECTORS\n");

    mat4x4* result2 = zero_matrix(); 
    mat4x4* matrices2[] = {result, &matrix3, &matrix2};

    matrix_sub(matrices2, 3, result2);
    print_matrix_ptr(result2);

    free(result2);

    //TESTING MATRIX MULTIPLICATION
    printf("\nTESTING MATRIX MULTIPLICATION\n");

    mat4x4 basic = {{1,2,3,4},{2,3,4,5},{3,4,5,6},{4,5,6,7}};
    mat4x4 basic2 = basic;

    print_matrix(basic);

    mat4x4* basic3 = zero_matrix();

    matxmat(&basic, &basic2, basic3);

    if(print_matrix_ptr(basic3) == -2) goto pointer_err;

    free(basic3);

    //TESTING MATRIX VECTOR MULTIPLICATION
    printf("\nTESTING MATRIX-VECTOR MULTIPLICATION\n");

    vector4 final_vec = {1,1,1,1};
    vector4* basic4 = zero_vector();

    matxvec(&basic, &final_vec, basic4);

    if(print_vector_ptr(basic4) == -2) goto pointer_err;

    //TESTING TRANSPOSE
    printf("\nTESTING TRANSPOSE\n");

    basic.y.x = 7;
    print_matrix(basic);
    printf("\n");
    transpose(&basic);
    print_matrix(basic);

    mat4x4* ident = (mat4x4*) malloc(sizeof(mat4x4));
    identity(ident);

    print_matrix_ptr(ident);

    printf("Successful run\n");

    return 0;

    //Error handling
    pointer_err:
        printf("Something went wrong\n");
        return -1;
}