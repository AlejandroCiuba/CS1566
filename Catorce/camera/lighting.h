/**
 * @file lighting.h
 * @author Alejandro Ciuba
 * @brief Lighting functions for the Catorce Matrix OpenGL Library
 * @version 0.1
 * @date 2021-12-07 
 */

#ifndef LIGHTING_H
#define LIGHTING_H

#include "math.h"
#include "../matrix_library/matrix_def.h"

// Calculates the normals for every 3 vertices (assumes triangle-based shapes)
ERROR_NUM surface_normals(vector4* vertices, int count, vector4* normals_array);

#endif