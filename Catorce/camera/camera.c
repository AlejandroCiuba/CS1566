/**
 * Camera and world projection-view library
 * Compabitble with OpenGL
 * 
 *  @author Alejandro Ciuba
 */

#include "camera.h"
#include <stdlib.h>

// Returns the matrix needed to generate a model-view function given the camera descriptors
ERROR_NUM model_view();

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