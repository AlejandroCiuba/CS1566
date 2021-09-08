/**
 * Matrix and vector math operations
 * 
 * Created by Alejandro Ciuba
 */

#include "matrix_ops.h"
#include <stddef.h> //Needed here, but NOT in header file

//============== VECTOR AND MATRIX MATH FUNCTIONS ==============
//for args, 0 = vector, 1 = matrix4x4
ERROR_NUM scalar(const void* matrix, const GLfloat alpha, const char args) {

    if(matrix == NULL || (args != 0 && args != 1)) return MATLIB_POINTER_ERROR;

    GLfloat* mat = (GLfloat*) matrix;
    
    char limit;
    if(args == 0) limit = 4;
    else limit = 16;

    for(char i = 0; i < limit; i++) 
        *(mat + i) = *(mat + i) * alpha;

    return 0;
}
