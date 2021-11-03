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

#include <stdlib.h>
#include <stdio.h>

int main() {

    // Test model-view function
    // User input VRP vpn and VUP
    vector4 vrp = zero_vector, vpn = zero_vector, vup = zero_vector;

    printf("\nPlease Input The VRP: ");
    scanf("%f %f %f", &vrp.x, &vrp.y, &vrp.z); vrp.w = 1.0f;

    printf("\nPlease Input The vpn: ");
    scanf("%f %f %f", &vpn.x, &vpn.y, &vpn.z); vpn.w = 0.0f;

    printf("\nPlease Input The VUP: ");
    scanf("%f %f %f", &vup.x, &vup.y, &vup.z); vup.w = 0.0f;

    mat4x4 mv = zero_matrix;
    model_view(&vrp, &vpn, &vup, &mv);
    print_matrix(mv);

    return 0;
}