/**
 * Function bodies for file_reader.h
 * 
 * @author Alejandro Ciuba
 */

#include "file_reader.h"

ERROR_NUM load_va(FILE* fp, vector4* vertices) {
    
    if(fp == NULL) return MATLIB_FILE_ERROR;
    if(vertices == NULL) return MATLIB_VECTOR_ERROR;
}

ERROR_NUM load_ma(FILE* fp, mat4x4* matrices) {
    return 0;
}

ERROR_NUM save_va(FILE* fp, vector4* vertices, int count) {
    return 0;
}

ERROR_NUM save_ma(FILE* fp, mat4x4* matrices, int count) {
    return 0;
}

ERROR_NUM view_file(FILE* fp) {
    return 0;
}
