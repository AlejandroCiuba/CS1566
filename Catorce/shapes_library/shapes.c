/**
 * Creates shapes in vertex-array form
 * Compatible with OpenGL
 * 
 * @author Alejandro Ciuba
 */

#include "shapes.h"
#include "affine.h"
#include "../matrix_library/matrix_ops.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

// Define extern RGB here
vector4 RED = {1,0,0,1}; vector4 GREEN = {0,1,0,1}; vector4 BLUE = {0,0,1,1}; vector4 YELLOW = {1,1,0,1}; vector4 ORANGE = {1,.647,0.1};
vector4 BLACK = {0,0,0,1}; vector4 WHITE = {1,1,1,1};

// Scaling factor for textures
static GLfloat scale_y = 2, scale_x = 2;

ERROR_NUM random_colors(vector4* colors, int num_vertices) {
    
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

    return 0;
}

// Assigns one color to a series of faces
ERROR_NUM const_color(vector4* colors, int num_vertices, color face_color) {

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

    return 0;
}

// GLfloat* other is an array which contains other info needed for FLAT_TORUS as follows:
// FLAT_TORUS other[2] = {inner radius, outer radius}
ERROR_NUM texturize2D(vector2* texcoords, int count, shape type, GLfloat* other) {
    
    if(texcoords == NULL || count == 0) return MATLIB_POINTER_ERROR;
    else if(type == FLAT_TORUS && other == NULL) return MATLIB_POINTER_ERROR;

    switch(type) {

        case RECTANGLE:
            texcoords[0] = (vector2) {0.0, 1.0};
            texcoords[1] = (vector2) {1.0, 1.0};
            texcoords[2] = (vector2) {1.0, 0.0};
            texcoords[3] = (vector2) {0.0, 1.0};
            texcoords[4] = (vector2) {1.0, 0.0};
            texcoords[5] = (vector2) {0.0, 0.0};
            break;
        case FLAT_TORUS:; // bruh
            int tri = count / 3;
            int outward = tri / 2;
            GLfloat deg = 360.00 / outward;

            for(int i = 0; i < outward; i ++) {

                texcoords[i * 3].x = (GLfloat) ((.5 / (other[1] / other[0])) * cos((-deg * i) * M_PI / 180) + .5);
                texcoords[i * 3].y = (GLfloat) ((.5 / (other[1] / other[0])) * sin((-deg * i) * M_PI / 180) + .5);
                
                texcoords[i * 3 + 1].x = (GLfloat) (.5 * cos((-deg * i) * M_PI / 180) + .5);
                texcoords[i * 3 + 1].y = (GLfloat) (.5 * sin((-deg * i) * M_PI / 180) + .5);

                texcoords[i * 3 + 2].x = (GLfloat) (.5 * cos((-deg * (i + 1)) * M_PI / 180) + .5);
                texcoords[i * 3 + 2].y = (GLfloat) (.5 * sin((-deg * (i + 1)) * M_PI / 180) + .5);
            }

            for(int i = 0; i < outward; i++) {

                texcoords[i * 3 + outward * 3].x = texcoords[i * 3].x;//(GLfloat) (inner * cos((deg_per_triangle * i) * M_PI / 180));
                texcoords[i * 3 + outward * 3].y = texcoords[i * 3].y;//(GLfloat) (inner * sin((deg_per_triangle * i) * M_PI / 180));

                texcoords[i * 3 + 1 + outward * 3].x = texcoords[i * 3 + 2].x;//(GLfloat) (outer * cos((deg_per_triangle * (i + 1)) * M_PI / 180));
                texcoords[i * 3 + 1 + outward * 3].y = texcoords[i * 3 + 2].y;//(GLfloat) (outer * sin((deg_per_triangle * (i + 1)) * M_PI / 180));

                texcoords[i * 3 + 2 + outward * 3].x = (GLfloat) ((.5 / (other[1] / other[0])) * cos((-deg * (i + 1)) * M_PI / 180) + .5);
                texcoords[i * 3 + 2 + outward * 3].y = (GLfloat) ((.5 / (other[1] / other[0])) * sin((-deg * (i + 1)) * M_PI / 180) + .5);
            }
            break;
        case CIRCLE:
            tri = count / 3;
            deg = 360.00 / tri;
            for(int i = 0; i < tri; i++) {
                texcoords[i * 3] = (vector2) {.5 * cos(-deg * (i + 1) * M_PI / 180) +.5, .5 * sin(-deg * (i + 1) * M_PI / 180) + .5};
                texcoords[i * 3 + 1] = (vector2) {.5, .5};
                texcoords[i * 3 + 2] = (vector2) {.5 * cos(-deg * i * M_PI / 180) + .5, .5 * sin(-deg * i * M_PI / 180)+ .5};
            }
        default:
            return MATLIB_NAN_ERROR;
    }
    
    return 0;
}

ERROR_NUM texturize3D(vector2* texcoords, int count, shape type, vector4* vertices) {

    if(texcoords == NULL || count == 0) return MATLIB_POINTER_ERROR;

    switch(type) {
        case SPHERE:
            for(int i = 0; i < count; i++) texcoords[i] = (vector2) {(vertices[i].x + 1) / scale_x, -(vertices[i].y + 1) / scale_y}; // Why 4 and 8 always for 1024?
            break;
        case BAND:
            for(int i = 0; i < count / 2; i++) 
                texturize2D((vector2[6]){{vertices[i].x, vertices[i].y},{vertices[i + 1].x, vertices[i + 1].y}}, 6, RECTANGLE, NULL); // INCOMPLETE
            break;
        case CONE:
        default:
            return MATLIB_NAN_ERROR;
    }

    return 0;
}

// Changes the scaling when applying textures
ERROR_NUM texture_scale(GLfloat x, GLfloat y) {

    if(x == 0 || y == 0) return MATLIB_NAN_ERROR;
    scale_x = x; scale_y = y;
    return 0;
}

// Assumes triangle-based implementation
ERROR_NUM circle(vector4* vertices, int count, GLfloat radius, vector4 origin, char align) {

    if(vertices == NULL || count == 0) return MATLIB_POINTER_ERROR;
    count -= (count % 3);

    //Calculate how many triangles there will be based on the count
    int num_of_triangles = count / 3;

    //Get how many degrees each triangle "bends"
    GLfloat deg_per_triangle = (GLfloat) 360 / num_of_triangles;

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

    if(vertices == NULL || count <= 0) return MATLIB_POINTER_ERROR;
    count -= (count % 6);

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
        vertices[i * 3 + base_vertices + 1] = vertices[i * 3 + 2];
        vertices[i * 3 + base_vertices + 2] = tip;
    }

    return 0;
}

// Assumes 6 vertices
ERROR_NUM rectangle(vector4* vertices, GLfloat height, GLfloat width, vector4 origin) {

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

ERROR_NUM flat_torus(vector4* vertices, int count, GLfloat inner, GLfloat outer) {

    if(vertices == NULL || outer == 0) return MATLIB_POINTER_ERROR;
    count -= (count % 6);

    int triangles = count / 3;
    int outward = triangles / 2;
    GLfloat deg_per_triangle = 360.00 / outward;


    for(int i = 0; i < outward; i ++) {

        vertices[i * 3].x = (GLfloat) (inner * cos((deg_per_triangle * i) * M_PI / 180));
        vertices[i * 3].y = (GLfloat) (inner * sin((deg_per_triangle * i) * M_PI / 180));
        vertices[i * 3].z = 0;
        vertices[i * 3].w = 1;
        
        vertices[i * 3 + 1].x = (GLfloat) (outer * cos((deg_per_triangle * i) * M_PI / 180));
        vertices[i * 3 + 1].y = (GLfloat) (outer * sin((deg_per_triangle * i) * M_PI / 180));
        vertices[i * 3 + 1].z = 0;
        vertices[i * 3 + 1].w = 1;

        vertices[i * 3 + 2].x = (GLfloat) (outer * cos((deg_per_triangle * (i + 1)) * M_PI / 180));
        vertices[i * 3 + 2].y = (GLfloat) (outer * sin((deg_per_triangle * (i + 1)) * M_PI / 180));
        vertices[i * 3 + 2].z = 0;
        vertices[i * 3 + 2].w = 1;
    }

    for(int i = 0; i < outward; i++) {

        vertices[i * 3 + outward * 3].x = vertices[i * 3].x;//(GLfloat) (inner * cos((deg_per_triangle * i) * M_PI / 180));
        vertices[i * 3 + outward * 3].y = vertices[i * 3].y;//(GLfloat) (inner * sin((deg_per_triangle * i) * M_PI / 180));
        vertices[i * 3 + outward * 3].z = 0;
        vertices[i * 3 + outward * 3].w = 1;

        vertices[i * 3 + 1 + outward * 3].x = vertices[i * 3 + 2].x;//(GLfloat) (outer * cos((deg_per_triangle * (i + 1)) * M_PI / 180));
        vertices[i * 3 + 1 + outward * 3].y = vertices[i * 3 + 2].y;//(GLfloat) (outer * sin((deg_per_triangle * (i + 1)) * M_PI / 180));
        vertices[i * 3 + 1 + outward * 3].z = 0;
        vertices[i * 3 + 1 + outward * 3].w = 1;

        vertices[i * 3 + 2 + outward * 3].x = (GLfloat) (inner * cos((deg_per_triangle * (i + 1)) * M_PI / 180));
        vertices[i * 3 + 2 + outward * 3].y = (GLfloat) (inner * sin((deg_per_triangle * (i + 1)) * M_PI / 180));
        vertices[i * 3 + 2 + outward * 3].z = 0;
        vertices[i * 3 + 2 + outward * 3].w = 1;
    }

    return 0;
}

ERROR_NUM band(vector4* vertices, int count, GLfloat radius, GLfloat length) {

    if(vertices == NULL || count <= 0) return MATLIB_POINTER_ERROR;
    count = count - (count % 6);

    int vert_per_ring = count / 2;

    circle(vertices, vert_per_ring, radius, (vector4){0,length / 2,0,1}, 'y');
    circle(&vertices[vert_per_ring], vert_per_ring, radius, (vector4){0,-length / 2,0,1}, 'y');

    vector4 temp = {0,0,0,0};

    for(int i = 1; i < vert_per_ring; i+=3) {
        vertices[i] = vertices[i + vert_per_ring - 1];
        temp = vertices[i + vert_per_ring - 1];
        vertices[i + vert_per_ring - 1] = vertices[i + 1];
        vertices[i + vert_per_ring] = temp;
    }

    return 0;
}

// Bands must be a power of 2!!!
ERROR_NUM sphere(vector4* vertices, int count, GLfloat radius, int bands) {

    if(vertices == NULL || count == 0 || bands % 2 != 0) return MATLIB_POINTER_ERROR;
    count -= (count % (6 * (bands - 1)));

    //calculate the number of rectangles per band
    int rects = count / (6 * (bands - 1));
    GLfloat theta = (GLfloat) (360.00 / rects); //Bruh this bug
    GLfloat phi = (GLfloat) (90.00 / (bands / 2));
    
    //Generate the bottom
    vector4 base = {0,-radius,0,1}, point;
    mat4x4 ro;
    rotate(-phi * ((bands - 2) / 2), 'z', &ro);
    matxvec(&ro, &(vector4){radius,0,0,1}, &point);
    
    for(int i = 0; i < rects; i++) {

        vertices[i * 3] = base;
        rotate(theta * (i + 1), 'y', &ro);
        matxvec(&ro, &point, &vertices[(i * 3) + 1]);
        rotate(theta * i, 'y', &ro);
        matxvec(&ro, &point, &vertices[(i * 3) + 2]);
    }

    vector4 point2;
    //Generate the middle parts
    for(int i = 1; i < bands / 2; i++) {

        rotate(-phi * ((int)(bands / 2) - i), 'z', &ro);
        matxvec(&ro, &(vector4){radius,0,0,1}, &point);

        rotate(-phi * ((int)(bands / 2) - (i + 1)), 'z', &ro);
        matxvec(&ro, &(vector4){radius,0,0,1}, &point2);

        for(int j = 0; j < rects; j++) {

            rotate(theta * (j + 1), 'y', &ro);
            matxvec(&ro, &point, &vertices[((rects * 6) * (i - 1)) + (j * 6) + (rects * 3)]);
            rotate(theta * j, 'y', &ro);
            matxvec(&ro, &point2, &vertices[((rects * 6) * (i - 1)) + (j * 6) + (rects * 3 + 1)]);
            rotate(theta * j, 'y', &ro);
            matxvec(&ro, &point, &vertices[((rects * 6) * (i - 1)) + (j * 6) + (rects * 3 + 2)]);

            vertices[((rects * 6) * (i - 1)) + (j * 6) + (rects * 3 + 3)] = vertices[((rects * 6) * (i - 1)) + (j * 6) + (rects * 3 + 1)];
            vertices[((rects * 6) * (i - 1)) + (j * 6) + (rects * 3 + 4)] = vertices[((rects * 6) * (i - 1)) + (j * 6) + (rects * 3)];
            rotate(theta * (j + 1), 'y', &ro);
            matxvec(&ro, &point2, &vertices[((rects * 6) * (i - 1)) + (j * 6) + (rects * 3 + 5)]);

        }
    }

    int offset = count / 2;
    base = (vector4){0,radius,0,1};

    rotate(phi * ((bands - 2) / 2), 'z', &ro);
    matxvec(&ro, &(vector4){radius,0,0,1}, &point);

    for(int i = 0; i < rects; i++) {

        vertices[i * 3 + offset] = base;
        rotate(-theta * (i + 1), 'y', &ro);
        matxvec(&ro, &point, &vertices[(i * 3) + 1 + offset]);
        rotate(-theta * i, 'y', &ro);
        matxvec(&ro, &point, &vertices[(i * 3) + 2 + offset]);
    }

    //Generate the middle parts
    for(int i = 1; i < bands / 2; i++) {

        rotate(phi * ((int)(bands / 2) - i), 'z', &ro);
        matxvec(&ro, &(vector4){radius,0,0,1}, &point);

        rotate(phi * ((int)(bands / 2) - (i + 1)), 'z', &ro);
        matxvec(&ro, &(vector4){radius,0,0,1}, &point2);

        for(int j = 0; j < rects; j++) {

            rotate(-theta * (j + 1), 'y', &ro);
            matxvec(&ro, &point, &vertices[((rects * 6) * (i - 1)) + (j * 6) + (rects * 3) + offset]);
            rotate(-theta * j, 'y', &ro);
            matxvec(&ro, &point2, &vertices[((rects * 6) * (i - 1)) + (j * 6) + (rects * 3 + 1) + offset]);
            rotate(-theta * j, 'y', &ro);
            matxvec(&ro, &point, &vertices[((rects * 6) * (i - 1)) + (j * 6) + (rects * 3 + 2) + offset]);

            vertices[((rects * 6) * (i - 1)) + (j * 6) + (rects * 3 + 3) + offset] = vertices[((rects * 6) * (i - 1)) + (j * 6) + (rects * 3 + 1) + offset];
            vertices[((rects * 6) * (i - 1)) + (j * 6) + (rects * 3 + 4) + offset] = vertices[((rects * 6) * (i - 1)) + (j * 6) + (rects * 3) + offset];
            rotate(-theta * (j + 1), 'y', &ro);
            matxvec(&ro, &point2, &vertices[((rects * 6) * (i - 1)) + (j * 6) + (rects * 3 + 5) + offset]);
        }
    }

    return 0;
}

ERROR_NUM torus(vector4* vertices, int count, int bands, GLfloat radius, GLfloat band_radius) {

    if(vertices == NULL || count == 0) return MATLIB_POINTER_ERROR;
    count -= (count % (6 * bands));

    GLfloat theta = 360.00 / bands;
    GLfloat band_center = radius - band_radius;
    int rects_per_band = count / (6 * bands);

    mat4x4 ro1, ro2, tran, final;

    //Make the bands
    for(int i = 0; i < bands; i++) {
        band(vertices + (rects_per_band * 6 * i), rects_per_band * 6, band_radius, .25);
        rotate(90, 'x', &ro1);
        trans((affine){band_center, 0, 0}, &tran);
        rotate(-theta * i, 'y', &ro2);
        mat_mult((mat4x4[3]){ro2, tran, ro1}, 3, &final);
        matxvar(&final, vertices + (rects_per_band * 6 * i), rects_per_band * 6, vertices + (rects_per_band * 6 * i));
    }

    return 0;
}

ERROR_NUM rect3D(vector4* vertices, GLfloat length, GLfloat width, GLfloat height) {

    if(vertices == NULL) return MATLIB_POINTER_ERROR;

    // I just programmed it manually
    // Face 1
    vertices[0] = (vector4) {width / 2.0, -height / 2.0, length / 2.0, 1.0};
    vertices[1] = (vector4) {width / 2.0, height / 2.0, length / 2.0, 1.0};
    vertices[2] = (vector4) {-width / 2.0, -height / 2.0, length / 2.0, 1.0};
    vertices[3] = vertices[2];
    vertices[4] = vertices[1];
    vertices[5] = (vector4) {-width / 2.0, height / 2.0, length / 2.0, 1.0};

    // Face 2
    vertices[6] = (vector4) {width / 2.0, -height / 2.0, -length / 2.0, 1.0};
    vertices[7] = (vector4) {-width / 2.0, -height / 2.0, -length / 2.0, 1.0};
    vertices[8] = (vector4) {width / 2.0, height / 2.0, -length / 2.0, 1.0};
    vertices[9] = vertices[8];
    vertices[10] = vertices[7];
    vertices[11] = (vector4) {-width / 2.0, height / 2.0, -length / 2.0, 1.0};

    // Face 3
    vertices[12] = vertices[8];
    vertices[13] = vertices[1];
    vertices[14] = vertices[6];
    vertices[15] = vertices[6];
    vertices[16] = vertices[1];
    vertices[17] = vertices[0];

    // Face 4
    vertices[18] = vertices[5];
    vertices[19] = vertices[11];
    vertices[20] = vertices[7];
    vertices[21] = vertices[7];
    vertices[22] = vertices[2];
    vertices[23] = vertices[5];

    // Face 5
    vertices[24] = vertices[1];
    vertices[25] = vertices[11];
    vertices[26] = vertices[5];
    vertices[27] = vertices[8];
    vertices[28] = vertices[11];
    vertices[29] = vertices[1];

    // Face 6
    vertices[30] = vertices[0];
    vertices[31] = vertices[2];
    vertices[32] = vertices[7];
    vertices[33] = vertices[0];
    vertices[34] = vertices[7];
    vertices[35] = vertices[6];

    return 0;
}

// 27 Cubes, 132 vertices per cubit
ERROR_NUM rubix_cube(vector4* vertices) {

    if(vertices == NULL) return MATLIB_POINTER_ERROR;

    // Make the cubes
    for(int i = 0; i < 27; i++) {
        mat4x4 tra;
        trans((affine){.5,.5,.5},&tra);
        cubit(vertices + (i * 132));
        matxvar(&tra, vertices + (i * 132), 132, vertices + (i * 132));
    }

    // Transpose Everything
    mat4x4 tra;
    int i = 0;
    for(float x = 0; x < 3; x++) {
        for(float y = 0; y < 3; y++) {
            for(float z = 0; z < 3; z++) {
                trans((affine){x, y, z}, &tra);
                matxvar(&tra, vertices + (i * 132), 132, vertices + (i * 132));
                i++;
            }
        }
    }
    
    // Move it all back to the center
    vector4 co = zero_vector;
    com(vertices, 3564, &co);
    trans((affine){-co.x, -co.y, -co.z}, &tra);
    matxvar(&tra, vertices, 3564, vertices);

    return 0;
}

// Assumes enough for 1 cubit (132 vertices!)
ERROR_NUM cubit(vector4* vertices) {

    if(vertices == NULL) return MATLIB_POINTER_ERROR;

    for(int i = 0; i < 6; i++)
        rectangle(vertices + (i * 6), .8, .8, (vector4){0,0,.5});

    // Rotate
    // Back
    mat4x4 ro;
    rotate(180, 'y', &ro);
    matxvar(&ro, vertices + 6, 6, vertices + 6);

    // Right
    rotate(-90, 'y', &ro);
    matxvar(&ro, vertices + 6, 6, vertices + 12);

    // Left
    rotate(90, 'y', &ro);
    matxvar(&ro, vertices + 6, 6, vertices + 18);

    // Bottom
    rotate(90, 'x', &ro);
    matxvar(&ro, vertices + 6, 6, vertices + 24);

    // Top
    rotate(-90, 'x', &ro);
    matxvar(&ro, vertices + 6, 6, vertices + 30);

    for(int i = 0; i < 12; i++) {

        vertices[36 + (i * 6)] = vertices[5];
        vertices[36 + ((i * 6) + 1)] = vertices[2];
        vertices[36 + ((i * 6) + 2)] = vertices[29];
        vertices[36 + ((i * 6) + 3)] = vertices[5];
        vertices[36 + ((i * 6) + 4)] = vertices[29];
        vertices[36 + ((i * 6) + 5)] = vertices[28];
    }

    // Rotate Top
    for(int i = 0; i < 4; i++) {
        rotate((GLfloat)(90 * (i + 1)), 'y', &ro);
        matxvar(&ro, &vertices[36 + (i * 6)], 6, &vertices[36 + (i * 6)]);
    }

    // Rotate Bottom
    for(int i = 0; i < 4; i++) {
        rotate(90, 'x', &ro);
        matxvar(&ro, &vertices[60 + (i * 6)], 6, &vertices[60 + (i * 6)]);
        rotate((GLfloat)(90 * (i + 1)), 'y', &ro);
        matxvar(&ro, &vertices[60 + (i * 6)], 6, &vertices[60 + (i * 6)]);
    }

    // Rotate to sides
    for(int i = 0; i < 4; i++) {
        rotate(90, 'z', &ro);
        matxvar(&ro, &vertices[84 + (i * 6)], 6, &vertices[84 + (i * 6)]);
        rotate((GLfloat)(90 * (i + 1)), 'y', &ro);
        matxvar(&ro, &vertices[84 + (i * 6)], 6, &vertices[84 + (i * 6)]);
    }

    // Fill in the triangles
    for(int i = 0; i < 8; i++) {
        vertices[108 + (i * 3)] = vertices[2];
        vertices[109 + (i * 3)] = vertices[17];
        vertices[110 + (i * 3)] = vertices[29];
    }

    // Rotate Top
    for(int i = 0; i < 4; i++) {
        rotate((GLfloat)(90 * (i + 1)), 'y', &ro);
        matxvar(&ro, &vertices[108 + (i * 3)], 3, &vertices[108 + (i * 3)]);
    }

    // Rotate Bottom
    for(int i = 0; i < 4; i++) {
        rotate(90, 'x', &ro);
        matxvar(&ro, &vertices[120 + (i * 3)], 3, &vertices[120 + (i * 3)]);
        rotate((GLfloat)(90 * (i + 1)), 'y', &ro);
        matxvar(&ro, &vertices[120 + (i * 3)], 3, &vertices[120 + (i * 3)]);
    }

    return 0;
}