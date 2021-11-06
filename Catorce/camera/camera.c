/**
 * Camera and world projection-view library
 * Compabitble with OpenGL
 * 
 *  @author Alejandro Ciuba
 */

#include "camera.h"
#include "../matrix_library/matrix_ops.h"
#include "../matrix_library/matrix_utility.h"
#include <stdlib.h>

// Returns the matrix needed to generate a model-view function given the camera descriptors
ERROR_NUM model_view(vector4* VRP, vector4* VPN, vector4* VUP, mat4x4* result) {

    if(VRP == NULL || VPN == NULL || VUP == NULL) return MATLIB_POINTER_ERROR;
    // if(VRP->w != 0 || VPN->w != VUP->w) return MATLIB_VECTOR_ERROR;

    // Define the new camera frame axes (n,u,v) -> n is VPN
    vector4 n = zero_vector;
    copy_vector(VPN, &n);

    // Vector v
    vector4 v = zero_vector;
    GLfloat scalar_num = 0.0f;
    GLfloat scalar_denom = 0.0f;

    vector_dot(VUP, VPN, &scalar_num);
    vector_dot(VPN, VPN, &scalar_denom);

    vector4 VPN_scalar;
    copy_vector(VPN, &VPN_scalar);

    scalar(&VPN_scalar, scalar_num / scalar_denom, 0);
    vector_sub((vector4*[2]) {VUP, &VPN_scalar}, 2, &v);

    // Vector u
    vector4 u = zero_vector;
    vector_cross(&v, VPN, &u);

    // Normalize the vectors
    vector_norm(&n);
    vector_norm(&v);
    vector_norm(&u);

    // We need to make a matrix M such that we Rotate all points and then Translate; therefore: M = TR
    // Step 1: Rotation Matrix
    mat4x4 ro = zero_matrix;
    ro = (mat4x4) {{u.x, v.x, n.x, 0}, {u.y, v.y, n.y, 0}, {u.z, v.z, n.z, 0}, {0, 0, 0, 1}};
    
    // Step 2: Translation Matrix
    mat4x4 tra = zero_matrix;
    tra = (mat4x4) {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {-(VRP->x), -(VRP->y), -(VRP->z), 1}};
    
    // Step 3: Combine into M = RT
    matxmat(&ro, &tra, result);

    return 0;
}

// Returns the matrix necessary for an orthographic perspective of a given scene
// Takes in front-lower-left (left, bottom, near) and back-top-right (right, top, far)
// NOTE: near = 1.0 and far = -1.0
ERROR_NUM orthographic(view* world_view, mat4x4* ortho_matrix) {

    if(world_view == NULL || ortho_matrix == NULL) return MATLIB_POINTER_ERROR;

    ortho_matrix->x = (vector4) {2 / (world_view->right - world_view->left), 0, 0, 0};
    ortho_matrix->y = (vector4) {0, 2 / (world_view->top - world_view->bottom), 0, 0};
    ortho_matrix->z = (vector4) {0, 0, 2 / (world_view->near - world_view->far), 0};
    ortho_matrix->w = (vector4) {-(world_view->right + world_view->left) / (world_view->right - world_view->left),
                                 -(world_view->top + world_view->bottom) / (world_view->top - world_view->bottom),
                                 -(world_view->near + world_view->far) / (world_view->near - world_view->far), 1};

    return 0;
}

// Returns the matrix necessary for a frustrum projection of a given scene
// Takes in front-lower-left (left, bottom, near) and back-top-right (right, top, far)
// NOTE: near = 1.0 and far = -1.0
ERROR_NUM perspective(view* world_view, mat4x4* frust_matrix) {

    if(world_view == NULL || frust_matrix == NULL) return MATLIB_POINTER_ERROR;

    frust_matrix->x = (vector4) {-(2 * world_view->near) / (world_view->right - world_view->left), 0, 0, 0};
    frust_matrix->y = (vector4) {0, -(2 * world_view->near) / (world_view->top - world_view->bottom), 0, 0};
    frust_matrix->z = (vector4) {(world_view->left + world_view->right) / (world_view->right - world_view->left), 
                                 (world_view->bottom + world_view->top) / (world_view->top - world_view->bottom), 
                                 (world_view->near + world_view->far) / (world_view->far - world_view->near), -1};
    frust_matrix->w = (vector4) {0, 0, -(2 * world_view->near * world_view->far) / (world_view->far - world_view->near)};

    return 0;
}