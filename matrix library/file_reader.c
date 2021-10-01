/**
 * Function bodies for file_reader.h
 * 
 * @author Alejandro Ciuba
 */

#include "file_reader.h"
#include <stdlib.h>

ERROR_NUM load_va(FILE* fp, vector4* vertices, int* count) {
    
    if(fp == NULL) return MATLIB_FILE_ERROR;
    if(vertices != NULL) return MATLIB_VECTOR_ERROR;

    int r = 0
    if(r = fscanf(fp, "%d", count) == EOF) return MATLIB_FILE_FORMAT_ERROR;

    vertices = (vector4*) malloc(sizeof(vector4) * (*count));

    int i = 0;

    while(r != EOF) {
       r = fscanf(fp, "%f, %f, %f, %f", vertices[i].x, vertices[i].y, vertices[i].z, vertices[i++].w); 
    }

    return 0;
}

ERROR_NUM load_ma(FILE* fp, mat4x4* matrices, int* count) {
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

//Keep track of what you're freeing
//File generated vertices and matrices
ERROR_NUM free_va(vector4* vertices) {free(vertices); return 0;}

ERROR_NUM free_ma(mat4x4* matrices) {free(matrices); return 0;}
