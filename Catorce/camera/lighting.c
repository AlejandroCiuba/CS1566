/**
 * @file lighting.c
 * @author Alejandro Ciuba
 * @brief Implementations for the lighting.h file
 * @version 0.1
 * @date 2021-12-07
 */

#include "lighting.h"
#include "../matrix_library/matrix_ops.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

// Calculates the normals for every 3 vertices (assumes triangle-based shapes)
ERROR_NUM surface_normals(vector4* vertices, int count, vector4* normals_array) {

    if(vertices == NULL || normals_array == NULL || count <= 0) return MATLIB_POINTER_ERROR;

    // Vectors created by subtracting p2 by p1 and p3 by p2, and the the cross product
    vector4 p21 = zero_vector;
    vector4 p32 = zero_vector;
    vector4 cross = zero_vector;

    for(int i = 0; i < count; i+=3) {
        // Subtract
        vector_sub((vector4*[2]){&vertices[i + 1], &vertices[i]}, 2, &p21);
        vector_sub((vector4*[2]){&vertices[i + 2], &vertices[i + 1]}, 2, &p32);
        // Get the cross product and normalize it
        vector_cross(&p21, &p32, &cross);
        vector_norm(&cross);
        // Store the normal for the three points
        normals_array[i] = normals_array[i + 1] = normals_array[i + 2] = cross;
    }

    return 0;
}