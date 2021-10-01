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

ERROR_NUM load_va(FILE* fp, vector4* vertices);

ERROR_NUM load_ma(FILE* fp, mat4x4* matrices);

ERROR_NUM save_va(FILE* fp, vector4* vertices, int count);

ERROR_NUM save_ma(FILE* fp, mat4x4* matrices, int count);

ERROR_NUM view_file(FILE* fp);

#endif