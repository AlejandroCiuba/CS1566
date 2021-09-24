/**
 * Creates shapes in vertex-array form
 * Compatible with OpenGL
 * 
 * @author Alejandro Ciuba
 */

#include "shapes.h"
#include "matrix_utility.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

//Define extern RGB here
vector4 RED = {1,0,0,1}; vector4 GREEN = {0,1,0,1}; vector4 BLUE = {0,0,1,1};

ERROR_NUM random_colors(vector4* colors, const int num_vertices) {
    
    if(colors == NULL || num_vertices <= 0) return MATLIB_POINTER_ERROR;

    //Seed it
    srand(time(NULL));

    for(int i = 0; i < num_vertices / 3; i++) {

        GLfloat red = (GLfloat) ((rand() % 10) + 1) / 10;
        GLfloat green = (GLfloat) ((rand() % 10) + 1) / 10;
        GLfloat blue = (GLfloat) ((rand() % 10) + 1) / 10;

        colors[i * 3].x = red;
        colors[i * 3].y = green;
        colors[i * 3].z = blue;
        colors[i * 3].w = 1.0;

        colors[i * 3 + 1].x = red;
        colors[i * 3 + 1].y = green;
        colors[i * 3 + 1].z = blue;
        colors[i * 3 + 1].w = 1.0;
        
        colors[i * 3 + 2].x = red;
        colors[i * 3 + 2].y = green;
        colors[i * 3 + 2].z = blue;
        colors[i * 3 + 2].w = 1.0;
    }

    print_vector(colors[0]);

    return 0;
}

//Assigns one color to a series of faces
ERROR_NUM const_color(vector4* colors, const int num_vertices, color face_color) {

    if(colors == NULL || num_vertices <= 0) return MATLIB_POINTER_ERROR;

    for(int i = 0; i < num_vertices / 3; i++) {

        GLfloat red = face_color.x;
        GLfloat green = face_color.y;
        GLfloat blue = face_color.z;

        colors[i * 3].x = red;
        colors[i * 3].y = green;
        colors[i * 3].z = blue;
        colors[i * 3].w = 1.0;

        colors[i * 3 + 1].x = red;
        colors[i * 3 + 1].y = green;
        colors[i * 3 + 1].z = blue;
        colors[i * 3 + 1].w = 1.0;
        
        colors[i * 3 + 2].x = red;
        colors[i * 3 + 2].y = green;
        colors[i * 3 + 2].z = blue;
        colors[i * 3 + 2].w = 1.0;
    }

    print_vector(colors[0]);

    return 0;
}

//Assumes triangle-based implementation
ERROR_NUM circle(vector4* vertices, int count, GLfloat radius, vector4 origin, char align) {

    if(vertices == NULL || count == 0 || count % 3 != 0) return MATLIB_POINTER_ERROR;

    //Calculate how many triangles there will be based on the count
    int num_of_triangles = count / 3;

    //Get how many degrees each triangle "bends"
    GLfloat deg_per_triangle = (GLfloat) 360 / num_of_triangles;

    print_GLfloat(deg_per_triangle);
    switch(align) {

        case 'z':

            for(int i = 0; i < num_of_triangles; i++) {

                vertices[i * 3].x = (GLfloat) (radius * cos((deg_per_triangle * (i + 1)) * M_PI / 180) + origin.x);
                vertices[i * 3].y = (GLfloat) (radius * sin((deg_per_triangle * (i + 1)) * M_PI / 180) + origin.y);
                vertices[i * 3].z = origin.z;
                vertices[i * 3].w = 1.0;

                vertices[i * 3 + 1].x = origin.x;
                vertices[i * 3 + 1].y = origin.y;
                vertices[i * 3 + 1].z = origin.z;
                vertices[i * 3 + 1].w = 1.0;

                vertices[i * 3 + 2].x = (GLfloat) (radius * cos((deg_per_triangle * i) * M_PI / 180) + origin.x);
                vertices[i * 3 + 2].y = (GLfloat) (radius * sin((deg_per_triangle * i) * M_PI / 180) + origin.y);
                vertices[i * 3 + 2].z = origin.z;
                vertices[i * 3 + 2].w = 1.0;
            }

            break;
        
        case 'y':

            for(int i = 0; i < num_of_triangles; i++) {

                vertices[i * 3].x = (GLfloat) (radius * cos((deg_per_triangle * (i + 1)) * M_PI / 180) + origin.x);
                vertices[i * 3].y = origin.y;
                vertices[i * 3].z = (GLfloat) (radius * sin((deg_per_triangle * (i + 1)) * M_PI / 180) + origin.z);
                vertices[i * 3].w = 1;

                vertices[i * 3 + 1].x = origin.x;
                vertices[i * 3 + 1].y = origin.y;
                vertices[i * 3 + 1].z = origin.z;
                vertices[i * 3 + 1].w = 1.0;

                vertices[i * 3 + 2].x = (GLfloat) (radius * cos((deg_per_triangle * i) * M_PI / 180) + origin.x);
                vertices[i * 3 + 2].y = origin.y;
                vertices[i * 3 + 2].z = (GLfloat) (radius * sin((deg_per_triangle * i) * M_PI / 180) + origin.z);
                vertices[i * 3 + 2].w = 1;
            }

            break;
        
        case 'x':

        for(int i = 0; i < num_of_triangles; i++) {

                vertices[i * 3].x = origin.x;
                vertices[i * 3].y = (GLfloat) (radius * cos((deg_per_triangle * (i + 1)) * M_PI / 180) + origin.y);
                vertices[i * 3].z = (GLfloat) (radius * sin((deg_per_triangle * (i + 1)) * M_PI / 180) + origin.z);
                vertices[i * 3].w = 1;

                vertices[i * 3 + 1].x = origin.x;
                vertices[i * 3 + 1].y = origin.y;
                vertices[i * 3 + 1].z = origin.z;
                vertices[i * 3 + 1].w = 1.0;

                vertices[i * 3 + 2].x = origin.x;
                vertices[i * 3 + 2].y = (GLfloat) (radius * cos((deg_per_triangle * i) * M_PI / 180) + origin.y);
                vertices[i * 3 + 2].z = (GLfloat) (radius * sin((deg_per_triangle * i) * M_PI / 180) + origin.z);
                vertices[i * 3 + 2].w = 1;
            }

            break;

        default:
            return MATLIB_VECTOR_ERROR;
    }

    return 0;
}

ERROR_NUM cone(vector4* vertices, int count, GLfloat radius, GLfloat height, vector4 tip, char align) {

    if(vertices == NULL || count <= 0 || count % 3 != 0) return MATLIB_POINTER_ERROR;

    int base_vertices = count / 2;
    vector4 base_origin = {tip.x,tip.y,tip.z,1};

    if(align == 'y')
        base_origin.y = tip.y - height;
    else if(align == 'x')
        base_origin.x = tip.x - height;
    else if(align =='z')
        base_origin.z = tip.z - height;
    else
        return MATLIB_VECTOR_ERROR;

    //Make base 
    if(circle(vertices, base_vertices, radius, base_origin, align) != 0) return MATLIB_VECTOR_ERROR;

    int triangles = base_vertices / 3;

    //Make "cone" part
    for(int i = 0; i < triangles; i++) {

        vertices[i * 3 + base_vertices] = vertices[i * 3];
        vertices[i * 3 + base_vertices + 1] = tip;
        vertices[i * 3 + base_vertices + 2] = vertices[i * 3 + 2];
    }

    return 0;
}

//Assumes 6 vertices
ERROR_NUM rectangle(vector4* vertices, GLfloat height, GLfloat width, vector4 origin, char align) {

    if(vertices == NULL || height == 0 || width == 0) return MATLIB_POINTER_ERROR;


    //Vertex 1-4
    vertices[0] = (vector4) {origin.x - (width / 2), origin.y - (height / 2), origin.z, 1};
    vertices[1] = (vector4) {origin.x + (width / 2), origin.y - (height / 2), origin.z, 1};
    vertices[2] = (vector4) {origin.x + (width / 2), origin.y + (height / 2), origin.z, 1};
    vertices[3] = (vector4) {origin.x - (width / 2), origin.y - (height / 2), origin.z, 1};
    vertices[4] = (vector4) {origin.x + (width / 2), origin.y + (height / 2), origin.z, 1};
    vertices[5] = (vector4) {origin.x - (width / 2), origin.y + (height / 2), origin.z, 1};

    return 0;
}

ERROR_NUM flat_taurus(vector4* vertices, int count, GLfloat inner, GLfloat outer, vector4 origin) {

    if(vertices == NULL || outer == 0 || count % 3 != 0 || count % 2 != 0) return MATLIB_POINTER_ERROR;

    int triangles = count / 3;
    int outward = triangles / 2;
    int deg_per_triangle = 360 / outward;


    for(int i = 0; i < triangles / 2; i += 3) {

        vertices[i].x = (GLfloat) (inner * cos((deg_per_triangle * i) * M_PI / 180));
        vertices[i].y = (GLfloat) (inner * sin((deg_per_triangle * i) * M_PI / 180));
        vertices[i].z = 0;
        vertices[i].w = 1;

        vertices[i + 1].x = (GLfloat) (outer * cos((deg_per_triangle * i) * M_PI / 180));
        vertices[i + 1].y = (GLfloat) (outer * sin((deg_per_triangle * i) * M_PI / 180));
        vertices[i + 1].z = 0;
        vertices[i + 1].w = 1;

        vertices[i + 2].x = (GLfloat) (outer * cos((deg_per_triangle * (i + 1)) * M_PI / 180));
        vertices[i + 2].y = (GLfloat) (outer * sin((deg_per_triangle * (i + 1)) * M_PI / 180));
        vertices[i + 2].z = 0;
        vertices[i + 2].w = 1;
    }

    for(int i = triangles / 2; i < triangles; i += 3) {

        vertices[i].x = (GLfloat) (inner * cos((deg_per_triangle * i) * M_PI / 180));
        vertices[i].y = (GLfloat) (inner * sin((deg_per_triangle * i) * M_PI / 180));
        vertices[i].z = 0;
        vertices[i].w = 1;

        vertices[i + 1].x = (GLfloat) (outer * cos((deg_per_triangle * (i + 1)) * M_PI / 180));
        vertices[i + 1].y = (GLfloat) (outer * sin((deg_per_triangle * (i + 1)) * M_PI / 180));
        vertices[i + 1].z = 0;
        vertices[i + 1].w = 1;

        vertices[i + 2].x = (GLfloat) (inner * cos((deg_per_triangle * (i + 1)) * M_PI / 180));
        vertices[i + 2].y = (GLfloat) (inner * sin((deg_per_triangle * (i + 1)) * M_PI / 180));
        vertices[i + 2].z = 0;
        vertices[i + 2].w = 1;
    }

    return 0;
}