/**
 * Function bodies for file_reader.h
 * 
 * @author Alejandro Ciuba
 */

#include "file_reader.h"

ERROR_NUM load_raw(FILE* fp, GLubyte texels[][][], int width, int height) {

    if(fp == NULL) return MATLIB_FILE_ERROR;
    fread(vertices, width * height * 3, 1, fp);
    return 0;
}

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

    if(fp == NULL) return MATLIB_FILE_ERROR;
    if(vertices == NULL) return MATLIB_POINTER_ERROR;

    if(fprintf(fp, "%d\n", count) == EOF) return MATLIB_FILE_ERROR;
    for(int i = 0; i < count; i++) 
        if(fprintf(fp, "%f,%f,%f,%f\n", vertices[i].x, vertices[i].y, vertices[i].z, vertices[i].w) == EOF) return MATLIB_FILE_ERROR;

    return 0;
}

//FORMATED FOR COLUMN-MAJOR!!!
ERROR_NUM save_ma(FILE* fp, mat4x4* matrices, int count) {
    if(fp == NULL) return MATLIB_FILE_ERROR;
    if(matrices == NULL) return MATLIB_POINTER_ERROR;

    if(fprintf(fp, "%d\n", count) == EOF) return MATLIB_FILE_ERROR;
    for(int i = 0; i < count; i++) {
        if(fprintf(fp, "%f,%f,%f,%f\n", matrices[i].x.x, matrices[i].y.x, matrices[i].z.x, matrices[i].w.x) == EOF) return MATLIB_FILE_ERROR;
        if(fprintf(fp, "%f,%f,%f,%f\n", matrices[i].x.y, matrices[i].y.y, matrices[i].z.y, matrices[i].w.y) == EOF) return MATLIB_FILE_ERROR;
        if(fprintf(fp, "%f,%f,%f,%f\n", matrices[i].x.z, matrices[i].y.z, matrices[i].z.z, matrices[i].w.z) == EOF) return MATLIB_FILE_ERROR;
        if(fprintf(fp, "%f,%f,%f,%f\n", matrices[i].x.w, matrices[i].y.w, matrices[i].z.w, matrices[i].w.w) == EOF) return MATLIB_FILE_ERROR;
    }
    
    return 0;
}

//REWINDS FILE TO BEGINNING 
//USE CUSTOM VIEW FUNCTION IF FILE FORMAT DIFFERS IN ANY WAY!!!
ERROR_NUM view_file(FILE* fp) {

    if(fp == NULL) return MATLIB_FILE_ERROR;
    rewind(fp);

    int count = -1; vector4 vert = {-1,-1,-1,-1};

    printf("\n===================== FILE STATISTICS =====================\n");

    if(fscanf(fp, "%d", &count) == EOF) return MATLIB_FILE_FORMAT_ERROR;
    printf("COUNT: %d\n", count);

    for(int i = 0; i < count; i++) {
        if(fscanf(fp, "%f,%f,%f,%f", &vert.x, &vert.y, &vert.z, &vert.w) == EOF) return MATLIB_FILE_FORMAT_ERROR;
        if(i < 10) printf("ROW   %d: x %.4f | y %.4f | z %.4f | w %.4f\n", i, vert.x, vert.y, vert.z, vert.w);
        else if(i < 100) printf("ROW  %d: x %.4f | y %.4f | z %.4f | w %.4f\n", i, vert.x, vert.y, vert.z, vert.w);
        else printf("ROW %d: x %.4f | y %.4f | z %.4f | w %.4f\n", i, vert.x, vert.y, vert.z, vert.w);
    }

    return 0;
}