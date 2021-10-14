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
#include "matrix_def.h"

typedef unsigned char file_type;

#define VA_FORMAT 0
#define MA_FORMAT 1
#define BASIC_PLY_FORMAT 2
#define RAW_IMAGE 3

ERROR_NUM load_raw(FILE* fp, GLubyte texels[][][], int width, int height);

ERROR_NUM load_count(FILE* fp, int* count);

ERROR_NUM load_va(FILE* fp, vector4* vertices, int count);

ERROR_NUM load_ma(FILE* fp, mat4x4* matrices, int count);

ERROR_NUM save_va(FILE* fp, vector4* vertices, int count);

ERROR_NUM save_ma(FILE* fp, mat4x4* matrices, int count);

ERROR_NUM view_file(FILE* fp);

#endif