/**
 * A simple set of functions designed
 * For reading vertices compatible with
 * My vector4 and mat4x4 data-types
 * 
 * @author Alejandro Ciuba
 */

#ifndef FILE_READER_H
#define FILE_READER_H

#include <stdio.h>
#include "../matrix_library/matrix_def.h"

// Assumes that it is w * h * 3
ERROR_NUM load_raw(FILE* fp, void* texels, int width, int height);

ERROR_NUM load_count(FILE* fp, int* count);

ERROR_NUM load_va(FILE* fp, vector4* vertices, int count);

// FORMATED FOR COLUMN-MAJOR!!!
ERROR_NUM load_ma(FILE* fp, mat4x4* matrices, int count);

ERROR_NUM load_PLY_text(FILE* fp, vector4** vertices, int* count, vector2** texcoords);

ERROR_NUM save_raw(FILE* fp, void* texels, int width, int height);

ERROR_NUM save_va(FILE* fp, vector4* vertices, int count);

// FORMATED FOR COLUMN-MAJOR!!!
ERROR_NUM save_ma(FILE* fp, mat4x4* matrices, int count);

// REWINDS FILE TO BEGINNING 
// USE CUSTOM VIEW FUNCTION IF FILE FORMAT DIFFERS IN ANY WAY!!!
ERROR_NUM view_file(FILE* fp);

#endif