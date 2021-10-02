/**
 * Function bodies for file_reader.h
 * 
 * @author Alejandro Ciuba
 */

#include "file_reader.h"

ERROR_NUM load_count(FILE* fp, int* count) {

    if(fp == NULL) return MATLIB_FILE_ERROR;
    if(count == NULL) return MATLIB_POINTER_ERROR;
    if(fscanf(fp, "%d", count) <= EOF) return MATLIB_FILE_FORMAT_ERROR;
    return 0;
}

ERROR_NUM load_va(FILE* fp, vector4* vertices, int count) {
    
    if(fp == NULL) return MATLIB_FILE_ERROR;
    if(vertices == NULL) return MATLIB_VECTOR_ERROR;

    for(int i = 0; i < count; i++)
        if(fscanf(fp, "%f,%f,%f,%f", &vertices[i].x, &vertices[i].y, &vertices[i].z, &vertices[i].w) == EOF) return MATLIB_FILE_FORMAT_ERROR;

    return 0;
}

//FORMATED FOR COLUMN-MAJOR!!!
ERROR_NUM load_ma(FILE* fp, mat4x4* matrices, int count) {
    
    if(fp == NULL) return MATLIB_FILE_ERROR;
    if(matrices == NULL) return MATLIB_VECTOR_ERROR;

    for(int i = 0; i < count; i++) {
        if(fscanf(fp, "%f,%f,%f,%f", &matrices[i].x.x, &matrices[i].y.x, &matrices[i].z.x, &matrices[i].w.x) == EOF) return MATLIB_FILE_FORMAT_ERROR;
        if(fscanf(fp, "%f,%f,%f,%f", &matrices[i].x.y, &matrices[i].y.y, &matrices[i].z.y, &matrices[i].w.y) == EOF) return MATLIB_FILE_FORMAT_ERROR;
        if(fscanf(fp, "%f,%f,%f,%f", &matrices[i].x.z, &matrices[i].y.z, &matrices[i].z.z, &matrices[i].w.z) == EOF) return MATLIB_FILE_FORMAT_ERROR;
        if(fscanf(fp, "%f,%f,%f,%f", &matrices[i].x.w, &matrices[i].y.w, &matrices[i].z.w, &matrices[i].w.w) == EOF) return MATLIB_FILE_FORMAT_ERROR;
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