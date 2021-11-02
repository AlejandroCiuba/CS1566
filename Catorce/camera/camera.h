/**
 * Camera and world projection-view library
 * Compabitble with OpenGL
 * 
 *  @author Alejandro Ciuba
 */

#include "../matrix_library/matrix_def.h"

//Special 6x1 Vector Column to define view-frames
typedef struct view {
    GLfloat left, 
            right, 
            top, 
            bottom, 
            near, 
            far;
} view;

// Returns the matrix needed to generate a model-view function given the camera descriptors
ERROR_NUM model_view();

// Returns the matrix necessary for an orthographic perspective of a given scene
// Takes in front-lower-left (left, bottom, near) and back-top-right (right, top, far)
// NOTE: near = 1.0 and far = -1.0
ERROR_NUM orthographic(view* world_view, mat4x4* ortho_matrix);