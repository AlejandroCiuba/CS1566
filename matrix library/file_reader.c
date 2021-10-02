/**
 * Function bodies for file_reader.h
 * 
 * @author Alejandro Ciuba
 */

#include "file_reader.h"
#include "matrix_utility.h"
#include <stdlib.h>

ERROR_NUM load_va(FILE* fp, vector4* vertices, int* count) {
    
    if(fp == NULL) return MATLIB_FILE_ERROR;
    if(vertices != NULL) return MATLIB_VECTOR_ERROR;

    int r = fscanf(fp, "%d", count);
    if(r == EOF) return MATLIB_FILE_FORMAT_ERROR;

    vertices = (vector4*) malloc(sizeof(vector4) * (*count));
    if(vertices == NULL) return MATLIB_ALLOC_ERROR;

    int i = 0;

    while(r != EOF) {
        r = fscanf(fp, "%f, %f, %f, %f", &vertices[i].x, &vertices[i].y, &vertices[i].z, &vertices[i].w); 
        print_vector_ptr(&vertices[i]);
        if(r == EOF) return 0;
        i++;
    }

    return 0;
}

ERROR_NUM load_ma(FILE* fp, mat4x4* matrices, int* count) {
    
    if(fp == NULL) return MATLIB_FILE_ERROR;
    if(matrices != NULL) return MATLIB_VECTOR_ERROR;

    int r = fscanf(fp, "%d", count);
    if(r == EOF) return MATLIB_FILE_FORMAT_ERROR;

    matrices = (mat4x4*) malloc(sizeof(mat4x4) * (*count));
    if(matrices == NULL) return MATLIB_ALLOC_ERROR;

    int i = 0;

    while(r != EOF) {
        r = fscanf(fp, "%f, %f, %f, %f", &matrices[i].x.x, &matrices[i].x.y, &matrices[i].x.z, &matrices[i].x.w);
        if(r == EOF) return MATLIB_FILE_FORMAT_ERROR;
        r = fscanf(fp, "%f, %f, %f, %f", &matrices[i].y.x, &matrices[i].y.y, &matrices[i].y.z, &matrices[i].y.w);
        if(r == EOF) return MATLIB_FILE_FORMAT_ERROR;
        r = fscanf(fp, "%f, %f, %f, %f", &matrices[i].z.x, &matrices[i].z.y, &matrices[i].z.z, &matrices[i].z.w);
        if(r == EOF) return MATLIB_FILE_FORMAT_ERROR;
        r = fscanf(fp, "%f, %f, %f, %f", &matrices[i].w.x, &matrices[i].w.y, &matrices[i].w.z, &matrices[i].w.w);
        if(r == EOF) return MATLIB_FILE_FORMAT_ERROR; 
        i++;
    }

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
