/**
 * Function bodies for file_reader.h
 * 
 * @author Alejandro Ciuba
 */

#include "file_reader.h"
#include "array_list.h"
#include <stdlib.h>
#include <string.h>

#include <stdio.h>
#include "../matrix_library/matrix_utility.h"

// Assumes that it is w * h * 3
ERROR_NUM load_raw(FILE* fp, void* texels, int width, int height) {

    if(fp == NULL || texels == NULL) return MATLIB_FILE_ERROR;
    if(fread(texels, width * height * 3, 1, fp) != EOF) return MATLIB_FILE_FORMAT_ERROR;
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

// FORMATED FOR COLUMN-MAJOR!!!
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

// Assumes properties and triangle faces
// VERTICES WILL GET WIPED
ERROR_NUM load_PLY_text(FILE* fp, vector4** vertices, int* count, vector2** texcoords) {

    if(fp == NULL) return MATLIB_FILE_ERROR;

    // Rewind
    rewind(fp);

    // File-style check and Little Endian Check
    char ply_check[4];
    if(fscanf(fp, "%s", ply_check) == EOF) return MATLIB_FILE_FORMAT_ERROR;
    if(strcmp(ply_check, "ply\0") != 0) return MATLIB_FILE_FORMAT_ERROR;

    char format[3][21]; 
    if(fscanf(fp, "%s %s %s", format[0], format[1], format[2]) == EOF) return MATLIB_FILE_FORMAT_ERROR;
    if(strcmp(format[0], "format") != 0 || strcmp(format[1], "binary_little_endian") != 0 || strcmp(format[2], "1.0") != 0) return MATLIB_FILE_FORMAT_ERROR;

    // Get the elements and faces for vertices
    char elements[50];
    int vertex_num = -1;
    int face_num = -1;
    do {
        if(fscanf(fp, "%s", elements) == EOF) return MATLIB_FILE_FORMAT_ERROR;
    } while(strcmp(elements, "element") != 0);
    
    if(fscanf(fp, "%s %d", elements, &vertex_num) == EOF) return MATLIB_FILE_FORMAT_ERROR;
    if(vertex_num == -1) return MATLIB_FILE_FORMAT_ERROR;

    do {
        if(fscanf(fp, "%s", elements) == EOF) return MATLIB_FILE_FORMAT_ERROR;
    } while(strcmp(elements, "element") != 0);

    if(fscanf(fp, "%s %d", elements, &face_num) == EOF) return MATLIB_FILE_FORMAT_ERROR;
    if(face_num == -1) return MATLIB_FILE_FORMAT_ERROR;

    // Get the elements and faces for textures
    int tex_num = -1;
    int ftex_num = -1;
    do {
        if(fscanf(fp, "%s", elements) == EOF) return MATLIB_FILE_FORMAT_ERROR;
    } while(strcmp(elements, "element") != 0);
    
    if(fscanf(fp, "%s %d", elements, &tex_num) == EOF) return MATLIB_FILE_FORMAT_ERROR;
    if(vertex_num == -1) return MATLIB_FILE_FORMAT_ERROR;

    do {
        if(fscanf(fp, "%s", elements) == EOF) return MATLIB_FILE_FORMAT_ERROR;
    } while(strcmp(elements, "element") != 0);

    if(fscanf(fp, "%s %d", elements, &ftex_num) == EOF) return MATLIB_FILE_FORMAT_ERROR;
    if(face_num == -1) return MATLIB_FILE_FORMAT_ERROR;

    do {
        if(fscanf(fp, "%s", elements) == EOF) return MATLIB_FILE_FORMAT_ERROR;
    } while(strcmp(elements, "end_header") != 0);
    if(fscanf(fp, "%c", elements) == EOF) return MATLIB_FILE_FORMAT_ERROR;
    
    // ===================== PART I: VERTICES =====================
    // Scan in the vertices
    vector4* temp_refs = (vector4*) malloc(sizeof(vector4) * vertex_num);
    if(temp_refs == NULL) return MATLIB_ALLOC_ERROR;

    for(int i = 0; i < vertex_num; i++) if(fread(&temp_refs[i], 4, 3, fp) == EOF) return MATLIB_FILE_FORMAT_ERROR;


    // Load in the vertices
    *count = face_num * 3;
    *vertices = (vector4*) malloc(sizeof(vector4) * (*count));
    if(vertices == NULL) return MATLIB_ALLOC_ERROR;

    unsigned char check = 1;

    typedef struct list {
        int v1;
        int v2;
        int v3;
    } list;

    list faces = {-1, -1, -1};

    for(int i = 0; i < face_num; i++) {
        if(fread(&check, 1, 1, fp) == EOF) return MATLIB_FILE_FORMAT_ERROR;
        if(check != 3) return MATLIB_FILE_FORMAT_ERROR;
        if(fread(&faces, 4, 3, fp) == EOF) return MATLIB_FILE_FORMAT_ERROR;

        (*vertices)[i * 3] = (vector4){temp_refs[faces.v1].x, temp_refs[faces.v1].y, temp_refs[faces.v1].z, 1.0};
        (*vertices)[i * 3 + 1] = (vector4){temp_refs[faces.v2].x, temp_refs[faces.v2].y, temp_refs[faces.v2].z, 1.0};
        (*vertices)[i * 3 + 2] = (vector4){temp_refs[faces.v3].x, temp_refs[faces.v3].y, temp_refs[faces.v3].z, 1.0};
    }

    free(temp_refs);

    // ===================== PART 2: TEXTURES =====================
    // Scan in the texture coordinates
    vector2* tex_refs = (vector2*) malloc(sizeof(vector2) * tex_num);
    if(tex_refs == NULL) return MATLIB_ALLOC_ERROR;

    //Extra byte from unneeded layer information (SHOULD BE 0)
    unsigned char extra = -1;

    for(int i = 0; i < tex_num; i++) {
        if(fread(&extra, 1, 1, fp) == EOF) return MATLIB_FILE_FORMAT_ERROR;
        if(extra != 0) return MATLIB_FILE_FORMAT_ERROR;
        if(fread(&tex_refs[i], 4, 2, fp) == EOF) return MATLIB_FILE_FORMAT_ERROR;
    }

    // Load in the texture coordinates
    *texcoords = (vector2*) malloc(sizeof(vector4) * (*count)); // SHOULD BE THE SAME WITH IMPLEMENTATION
    if(texcoords == NULL) return MATLIB_ALLOC_ERROR;

    faces = (list){-1, -1, -1};
    int num = -1; //NOT NEEDED

    for(int i = 0; i < ftex_num; i++) {
        if(fread(&extra, 1, 1, fp) == EOF) return MATLIB_FILE_FORMAT_ERROR;
        if(fread(&num, 4, 1, fp) == EOF) return MATLIB_FILE_FORMAT_ERROR;
        if(extra != 0) return MATLIB_FILE_FORMAT_ERROR;
        if(fread(&check, 1, 1, fp) == EOF) return MATLIB_FILE_FORMAT_ERROR;
        if(check != 3) return MATLIB_FILE_FORMAT_ERROR;
        if(fread(&faces, 4, 3, fp) == EOF) return MATLIB_FILE_FORMAT_ERROR;  

        (*texcoords)[i * 3] = (vector2){tex_refs[faces.v1].x, tex_refs[faces.v1].y};
        (*texcoords)[i * 3 + 1] = (vector2){tex_refs[faces.v2].x, tex_refs[faces.v2].y};
        (*texcoords)[i * 3 + 2] = (vector2){tex_refs[faces.v3].x, tex_refs[faces.v3].y};
    }

    free(tex_refs);

    return 0;
}

ERROR_NUM load_PLY_color(FILE* fp, vector4** vertices, int* count, vector4** colors) {

    if(fp == NULL) return MATLIB_FILE_ERROR;

    // Rewind
    rewind(fp);

    // File-style check and Little Endian Check
    char ply_check[4];
    if(fscanf(fp, "%s", ply_check) == EOF) return MATLIB_FILE_FORMAT_ERROR;
    if(strcmp(ply_check, "ply\0") != 0) return MATLIB_FILE_FORMAT_ERROR;

    char format[3][21]; 
    if(fscanf(fp, "%s %s %s", format[0], format[1], format[2]) == EOF) return MATLIB_FILE_FORMAT_ERROR;
    if(strcmp(format[0], "format") != 0 || strcmp(format[1], "binary_little_endian") != 0 || strcmp(format[2], "1.0") != 0) return MATLIB_FILE_FORMAT_ERROR;

    // Get the elements and faces for vertices
    char elements[50];
    int vertex_num = -1;
    int face_num = -1;
    do {
        if(fscanf(fp, "%s", elements) == EOF) return MATLIB_FILE_FORMAT_ERROR;
    } while(strcmp(elements, "element") != 0);
    
    if(fscanf(fp, "%s %d", elements, &vertex_num) == EOF) return MATLIB_FILE_FORMAT_ERROR;
    if(vertex_num == -1) return MATLIB_FILE_FORMAT_ERROR;

    do {
        if(fscanf(fp, "%s", elements) == EOF) return MATLIB_FILE_FORMAT_ERROR;
    } while(strcmp(elements, "element") != 0);

    if(fscanf(fp, "%s %d", elements, &face_num) == EOF) return MATLIB_FILE_FORMAT_ERROR;
    if(face_num == -1) return MATLIB_FILE_FORMAT_ERROR;

    do {
        if(fscanf(fp, "%s", elements) == EOF) return MATLIB_FILE_FORMAT_ERROR;
    } while(strcmp(elements, "end_header") != 0);
    if(fscanf(fp, "%c", elements) == EOF) return MATLIB_FILE_FORMAT_ERROR;
    
    // ===================== VERTICES WITH RGB COLOR =====================
    // Scan in the vertices and RGB colors
    vector4* temp_refs = (vector4*) malloc(sizeof(vector4) * vertex_num);

    //Create a struct to store the colors
    typedef struct RGB {
        unsigned char R;
        unsigned char G;
        unsigned char B;
    } RGB;

    RGB* color_data = (RGB*) malloc(sizeof(RGB) * vertex_num);

    // ERROR-HANDLING
    if(temp_refs == NULL || color_data == NULL) {
        free(temp_refs);
        free(color_data);
        return MATLIB_ALLOC_ERROR;
    }

    for(int i = 0; i < vertex_num; i++) {
        // Vertex Position then Vertex Color
        if(fread(&temp_refs[i], 4, 3, fp) == EOF) return MATLIB_FILE_FORMAT_ERROR;
        if(fread(&color_data[i], 1, 3, fp) == EOF) return MATLIB_FILE_FORMAT_ERROR;
    }

    // Load in the vertices
    *count = face_num * 3;
    *vertices = (vector4*) malloc(sizeof(vector4) * (*count));
    *colors = (vector4*) malloc(sizeof(vector4) * (*count));

    // ERROR HANDLING
    if(vertices == NULL || colors == NULL) {
        free(vertices);
        free(colors);
        return MATLIB_ALLOC_ERROR;
    }

    unsigned char check = 1;

    typedef struct list {
        int v1;
        int v2;
        int v3;
    } list;

    list faces = {-1, -1, -1};
    
    for(int i = 0; i < face_num; i++) {
        if(fread(&check, 1, 1, fp) == EOF) return MATLIB_FILE_FORMAT_ERROR;
        if(check != 3) return MATLIB_FILE_FORMAT_ERROR;
        if(fread(&faces, 4, 3, fp) == EOF) return MATLIB_FILE_FORMAT_ERROR;
        
        // Load correct position
        (*vertices)[i * 3] = (vector4){temp_refs[faces.v1].x, temp_refs[faces.v1].y, temp_refs[faces.v1].z, 1.0};
        (*vertices)[i * 3 + 1] = (vector4){temp_refs[faces.v2].x, temp_refs[faces.v2].y, temp_refs[faces.v2].z, 1.0};
        (*vertices)[i * 3 + 2] = (vector4){temp_refs[faces.v3].x, temp_refs[faces.v3].y, temp_refs[faces.v3].z, 1.0};

        //Load correct color
        (*colors)[i * 3] = (vector4){(GLfloat) (color_data[faces.v1].R / 255.00), (GLfloat) (color_data[faces.v1].G / 255.00), (GLfloat) (color_data[faces.v1].B / 255.00), 1.0};
        (*colors)[i * 3 + 1] = (vector4){(GLfloat) (color_data[faces.v2].R / 255.00), (GLfloat) (color_data[faces.v2].G / 255.00), (GLfloat) (color_data[faces.v2].B / 255.00), 1.0};
        (*colors)[i * 3 + 2] = (vector4){(GLfloat) (color_data[faces.v3].R / 255.00), (GLfloat) (color_data[faces.v3].G / 255.00), (GLfloat) (color_data[faces.v3].B / 255.00), 1.0};
    }

    free(temp_refs);
    free(color_data);

    return 0;
}

// Specific layout... Like all my file loaders lmao
// Can hold up to 15-sided polygons with vertex points
// Less than 256 characters long
ERROR_NUM load_OBJ(FILE* fp, vector4** vertices, int* count, vector2** texcoords) {

    if(fp == NULL ) return MATLIB_FILE_ERROR;
    
    // Literally made array_list.c for this so...
    // Rewind
    rewind(fp);

    // ===================== PART I: LOAD REFERENCE VERTEX ARRAY =====================
    // Make array_list which will be used to store the vertices
    arl* ref_arr = init_arl(1024, sizeof(vector4));
    vector4 dummy = {0,0,0,1};

    char header[] = "\0";
    if(fscanf(fp, "%s", header) == EOF) return MATLIB_FILE_FORMAT_ERROR;

    while(strcmp(header, "v") == 0) {
        if(fscanf(fp, "%f %f %f", &dummy.x, &dummy.y, &dummy.z) == EOF) return MATLIB_FILE_FORMAT_ERROR;
        ref_arr = append(&dummy, ref_arr);
        if(fscanf(fp, "%s", header) == EOF) return MATLIB_FILE_FORMAT_ERROR;
    }

    // ===================== PART II: LOAD REFERENCE TEXTURE COORDINATES ARRAY =====================
    // Make array_list which will be used to store the texture coordinates
    arl* ref_tex = init_arl(1024, sizeof(vector2));
    vector2 dummy2 = {0,0};

    while(strcmp(header, "vt") == 0) {
        if(fscanf(fp, "%f %f", &dummy2.x, &dummy2.y) == EOF) return MATLIB_FILE_FORMAT_ERROR;
        ref_tex = append(&dummy2, ref_tex);
        if(fscanf(fp, "%s", header) == EOF) return MATLIB_FILE_FORMAT_ERROR;
    }
    
    // ===================== PART III: THE FINALE =====================
    // Did someone say waste more memory?
    arl* main_vec = init_arl(1024, sizeof(vector4));
    arl* main_tex = init_arl(1024, sizeof(vector2));
    char intermediate_step[256];
    char* token = NULL;

    // File parsing was a mistake
    int index_array[30];
    int actual_length = 0;

    // DID SOMEONE SAY BAD CODE???
    // Read in the first set since that's missing the f at the beginning
    if(fgets(intermediate_step, 256, fp) == NULL) return MATLIB_FILE_FORMAT_ERROR;
    token = strtok(intermediate_step, " /");

    while(token != NULL) {
        index_array[actual_length++] = atoi(token) - 1; //.obj array starts at 1
        token = strtok(NULL, " /");
    }

    // THE POWER OF MATH!!!
    for(int i = 0; i < (actual_length / 2) - 2; i++) {
        main_vec = append(get_shallow(index_array[0], ref_arr), main_vec);
        main_tex = append(get_shallow(index_array[1], ref_tex), main_tex);

        main_vec = append(get_shallow(index_array[2 * (i + 1)], ref_arr), main_vec);
        main_tex = append(get_shallow(index_array[2 * (i + 1) + 1], ref_tex), main_tex);

        main_vec = append(get_shallow(index_array[2 * (i + 2)], ref_arr), main_vec);
        main_tex = append(get_shallow(index_array[2 * (i + 2) + 1], ref_tex), main_tex);
    }
    
    // Main loop
    while(fgets(intermediate_step, 256, fp) != NULL) {
        token = NULL;
        actual_length = 0;
        token = strtok(intermediate_step, "f /");

        while(token != NULL) {
            index_array[actual_length++] = atoi(token) - 1; //.obj array starts at 1
            token = strtok(NULL, "f /");
        }
        
        // THE POWER OF MATH!!!
        for(int i = 0; i < (actual_length / 2) - 2; i++) {
            main_vec = append(get_shallow(index_array[0], ref_arr), main_vec);
            main_tex = append(get_shallow(index_array[1], ref_tex), main_tex);

            main_vec = append(get_shallow(index_array[2 * (i + 1)], ref_arr), main_vec);
            main_tex = append(get_shallow(index_array[2 * (i + 1) + 1], ref_tex), main_tex);

            main_vec = append(get_shallow(index_array[2 * (i + 2)], ref_arr), main_vec);
            main_tex = append(get_shallow(index_array[2 * (i + 2) + 1], ref_tex), main_tex);
        }
    }
    
    // ===================== PART IV: THE EPILOGUE =====================
    // Free the ref arrays
    free_arl(ref_arr);
    free_arl(ref_tex);

    // Get count and malloc vertices and texcoords
    // They SHOULD be the same size!!!
    *count = main_vec->size;
    *vertices = (vector4*) malloc(sizeof(vector4) * (*count));
    *texcoords = (vector2*) malloc(sizeof(vector2) * (*count));
    
    // Copy everything from the main_vec and main_tex into the vertex array and texture coordinate arrays
    for(int i = 0; i < main_vec->size; i++) {
        (*vertices)[i] = *((vector4*)get_shallow(i, main_vec));
        (*texcoords)[i] = *((vector2*)get_shallow(i, main_tex));
    }

    // Free main_vec and main_tex
    free_arl(main_vec);
    free_arl(main_tex);
    
    // And with that, the nightmare is over... For now...
    return 0;
}

ERROR_NUM save_raw(FILE* fp, void* texels, int width, int height) {

    if(fp == NULL || texels == NULL) return MATLIB_FILE_ERROR;
    if(fwrite(texels, width * height * 3, 1, fp) != EOF) return MATLIB_FILE_FORMAT_ERROR;
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

// FORMATED FOR COLUMN-MAJOR!!!
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

// REWINDS FILE TO BEGINNING 
// USE CUSTOM VIEW FUNCTION IF FILE FORMAT DIFFERS IN ANY WAY!!!
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