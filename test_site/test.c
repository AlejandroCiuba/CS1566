/**
 * Small test site to replace the old one
 * 
 * @author Alejandro Ciuba
 */

#include "../Catorce/matrix_library/matrix_def.h"
#include "../Catorce/matrix_library/matrix_ops.h"
#include "../Catorce/matrix_library/matrix_utility.h"

#include "../Catorce/shapes_library/affine.h"
#include "../Catorce/shapes_library/shapes.h"

#include "../Catorce/camera/camera.h"

#include "../Catorce/other/file_reader.h"

#include "../other_programs/array_list.h"

#include <stdlib.h>
#include <stdio.h>

int main() {

    // Test model-view function
    // User input VRP vpn and VUP
    /*vector4 vrp = zero_vector, vpn = zero_vector, vup = zero_vector;

    printf("\nPlease Input The VRP: ");
    scanf("%f %f %f", &vrp.x, &vrp.y, &vrp.z); vrp.w = 1.0f;

    printf("\nPlease Input The vpn: ");
    scanf("%f %f %f", &vpn.x, &vpn.y, &vpn.z); vpn.w = 0.0f;

    printf("\nPlease Input The VUP: ");
    scanf("%f %f %f", &vup.x, &vup.y, &vup.z); vup.w = 0.0f;

    mat4x4 mv = zero_matrix;
    model_view(&vrp, &vpn, &vup, &mv);
    print_matrix(mv);*/

    // Test array_list
    arl* my_ar = init_arl(64);
    printf("\nARRAY LIST INITIALIZED WITH:\n\tCAPACITY: %d\n\tSIZE: %d\n", my_ar->capacity, my_ar->size);

    //Adds values to my_ar up to 58
    for(int i = 0; i < 58; i++)
        my_ar = add(&i, my_ar, sizeof(int));

    printf("\nARRAY LIST NOW WITH:\n\tCAPACITY: %d\n\tSIZE: %d\n", my_ar->capacity, my_ar->size);

    //Prints values
    for(int i = 0; i < my_ar->size; i++)
        printf("%d ", *(int*) get_shallow(i, my_ar));
    printf("\n");

    //Adds values to my_ar past initial capacity
    for(int i = 0; i < 88; i++)
        my_ar = add(&i, my_ar, sizeof(int));

    printf("\nARRAY LIST NOW WITH:\n\tCAPACITY: %d\n\tSIZE: %d\n", my_ar->capacity, my_ar->size);

    //Prints values
    for(int i = 0; i < my_ar->size; i++)
        printf("%d ", *(int*) get_shallow(i, my_ar));
    printf("\n");

    //Replaces values in my_ar
    for(int i = 58; i < my_ar->size; i++)
        my_ar = replace(&i, i, my_ar, sizeof(int));

    printf("\nARRAY LIST NOW WITH:\n\tCAPACITY: %d\n\tSIZE: %d\n", my_ar->capacity, my_ar->size);

    //Prints values
    for(int i = 0; i < my_ar->size; i++)
        printf("%d ", *(int*) get_shallow(i, my_ar));
    printf("\n");

    //Free my_ar
    free_arl(my_ar);
    printf("EXIT SUCCESSFUL\n");
    return 0;
}