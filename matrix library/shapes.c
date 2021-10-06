/**
 * Creates shapes in vertex-array form
 * Compatible with OpenGL
 * 
 * @author Alejandro Ciuba
 */

#include "shapes.h"
#include "affine.h"
#include "matrix_ops.h"

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

    return 0;
}

//Assumes triangle-based implementation
ERROR_NUM circle(vector4* vertices, int count, GLfloat radius, vector4 origin, char align) {

    if(vertices == NULL || count == 0 || count % 3 != 0) return MATLIB_POINTER_ERROR;

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
        vertices[i * 3 + base_vertices + 1] = vertices[i * 3 + 2];
        vertices[i * 3 + base_vertices + 2] = tip;
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

ERROR_NUM flat_torus(vector4* vertices, int count, GLfloat inner, GLfloat outer, vector4 origin) {

    if(vertices == NULL || outer == 0 || count % 3 != 0 || count % 2 != 0) return MATLIB_POINTER_ERROR;

    int triangles = count / 3;
    int outward = triangles / 2;
    GLfloat deg_per_triangle = (GLfloat) 360 / outward;


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

ERROR_NUM sphere(vector4* vertices, int count, int hor_band, GLfloat radius) {

    if(vertices == NULL || radius <= 0 || count % 3 != 0 || count % 2 != 0) return MATLIB_POINTER_ERROR;

    int bands = 6;
    GLfloat theta = 30;
    GLfloat phi = 30;

    //Generate the bottom
    vector4 base = {0,-1,0,1}, point;
    mat4x4 ro;
    rotate(-phi * ((bands - 2) / 2), 'z', &ro);
    matxvec(&ro, &(vector4){1,0,0,1}, &point);

    for(int i = 0; i < 12; i++) {

        vertices[i * 3] = base;
        rotate(theta * (i + 1), 'y', &ro);
        matxvec(&ro, &point, &vertices[(i * 3) + 1]);
        rotate(theta * i, 'y', &ro);
        matxvec(&ro, &point, &vertices[(i * 3) + 2]);
    }

    vector4 point2;
    //Generate the middle parts
    for(int i = 1; i < bands / 2; i++) {

        rotate((int) (-phi * ((bands - (i + 1)) / 2)), 'z', &ro);
        matxvec(&ro, &(vector4){1,0,0,1}, &point);

        rotate((int) (-phi * ((bands - (i + 3)) / 2)), 'z', &ro);
        matxvec(&ro, &(vector4){1,0,0,1}, &point2);

        for(int j = 0; j < 12; j++) {

            rotate(theta * (j + 1), 'y', &ro);
            matxvec(&ro, &point, &vertices[(72 * (i - 1)) + (j * 6) + 36]);
            rotate(theta * j, 'y', &ro);
            matxvec(&ro, &point2, &vertices[(72 * (i - 1)) + (j * 6) + 37]);
            rotate(theta * j, 'y', &ro);
            matxvec(&ro, &point, &vertices[(72 * (i - 1)) + (j * 6) + 38]);

            vertices[(72 * (i - 1)) + (j * 6) + 39] = vertices[(72 * (i - 1)) + (j * 6) + 37];
            vertices[(72 * (i - 1)) + (j * 6) + 40] = vertices[(72 * (i - 1)) + (j * 6) + 36];
            rotate(theta * (j + 1), 'y', &ro);
            matxvec(&ro, &point2, &vertices[(72 * (i - 1)) + (j * 6) + 41]);
        }
    }

    int offset = count / 2;
    base = (vector4){0,1,0,1};

    rotate(phi * ((bands - 2) / 2), 'z', &ro);
    matxvec(&ro, &(vector4){1,0,0,1}, &point);

    for(int i = 0; i < 12; i++) {

        vertices[i * 3 + offset] = base;
        rotate(-theta * (i + 1), 'y', &ro);
        matxvec(&ro, &point, &vertices[(i * 3) + 1 + offset]);
        rotate(-theta * i, 'y', &ro);
        matxvec(&ro, &point, &vertices[(i * 3) + 2 + offset]);
    }

    //Generate the middle parts
    for(int i = 1; i < bands / 2; i++) {

        rotate((int) (phi * ((bands - (i + 1)) / 2)), 'z', &ro);
        matxvec(&ro, &(vector4){1,0,0,1}, &point);

        rotate((int) (phi * ((bands - (i + 3)) / 2)), 'z', &ro);
        matxvec(&ro, &(vector4){1,0,0,1}, &point2);

        for(int j = 0; j < 12; j++) {

            rotate(-theta * (j + 1), 'y', &ro);
            matxvec(&ro, &point, &vertices[(72 * (i - 1)) + (j * 6) + 36 + offset]);
            rotate(-theta * j, 'y', &ro);
            matxvec(&ro, &point2, &vertices[(72 * (i - 1)) + (j * 6) + 37 + offset]);
            rotate(-theta * j, 'y', &ro);
            matxvec(&ro, &point, &vertices[(72 * (i - 1)) + (j * 6) + 38 + offset]);

            vertices[(72 * (i - 1)) + (j * 6) + 39 + offset] = vertices[(72 * (i - 1)) + (j * 6) + 37 + offset];
            vertices[(72 * (i - 1)) + (j * 6) + 40 + offset] = vertices[(72 * (i - 1)) + (j * 6) + 36 + offset];
            rotate(-theta * (j + 1), 'y', &ro);
            matxvec(&ro, &point2, &vertices[(72 * (i - 1)) + (j * 6) + 41 + offset]);
        }
    }

    return 0;
}

ERROR_NUM torus(vector4* vertices, int count, int bands, GLfloat inner_radius, GLfloat thickness_radius) {

    if(vertices == NULL || count <= 0) return MATLIB_POINTER_ERROR;
    /*int verts_per_ring = count / bands;
    GLfloat deg_per_band = (GLfloat) (360 / bands);
    vector4 base[verts_per_ring];
    mat4x4 rot, tran, final;

    for(int i = 0; i < bands; i++) {
        circle(base, verts_per_ring, .25, (vector4){0,0,0,1}, 'z');
        trans((affine){radius, 0, 0}, &tran);
        rotate(radius * sin((deg_per_band * i) * M_PI / 180), 'y', &rot);
        matxmat(&rot, &tran, &final);
        matxvar(&final, base, verts_per_ring, vertices + (i * verts_per_ring));
    }*/
    int verts_per_band = count / bands / 2;
    GLfloat true_radius = inner_radius + thickness_radius;
    GLfloat deg_per_band = (GLfloat) (360 / bands);
    mat4x4 rot, ro, tran, final;
    vector4 ban[verts_per_band];

    //Produce all rings
    for(int i = 0; i < bands * 2; i++) {
        //band(ban, verts_per_band, thickness_radius, .5);
        circle(ban, verts_per_band, thickness_radius, (vector4){0,0,0,1}, 'z');
        //rotate(90, 'x', &rot);
        //rotate(-deg_per_band * i, 'y', &ro);
        //trans((affine){true_radius * cos((deg_per_band * i) * M_PI / 180), 0, true_radius * sin((deg_per_band * i) * M_PI / 180)}, &tran);
        //mat_mult((mat4x4[2]){tran, ro}, 2, &final);
        trans((affine){true_radius, 0, 0}, &tran);
        rotate(-deg_per_band * i, 'y', &ro);
        matxmat(&ro, &tran, &final);
        matxvar(&final, ban, verts_per_band, vertices + (i * verts_per_band));
    }

    for(int i = 0; i < bands; i++) {
        for(int j = 1; j < verts_per_band; j+=3) {
            if(j % 2 == 0)
                vertices[j + (verts_per_band * i)] = vertices[((j - 1) + (verts_per_band * (i + 1))) % (count / 2)];
            else if(i == 0) {
                vertices[j + (verts_per_band * i)] = vertices[((j - 1) + (verts_per_band * (bands - 1)))];
                vector4 temp = vertices[(j - 1) + (verts_per_band * i)];
                vertices[(j - 1) + (verts_per_band * i)] = vertices[j + (verts_per_band * i)];
                vertices[j + (verts_per_band * i)] = temp;
            }
            else {
                vertices[j + (verts_per_band * i)] = vertices[j + (verts_per_band * (i - 1))];
                vector4 temp = vertices[(j - 1) + (verts_per_band * i)];
                vertices[(j - 1) + (verts_per_band * i)] = vertices[j + (verts_per_band * i)];
                vertices[j + (verts_per_band * i)] = temp;
            }
        }
    }

    /*for(int i = 0; i < bands; i++) {
        for(int j = 1; j < verts_per_band; j+=3) {
            if(j % 2 != 0 && i != 0)
                vertices[(j + (verts_per_band * i)) + (count / 2)] = vertices[((j - 1) + (verts_per_band * (i + 1))) + (count / 2)];
            else if(i == 0) {
                vertices[j + (verts_per_band * i) + (count / 2)] = vertices[((j - 1) + (verts_per_band * (bands - 1)))];
                vector4 temp = vertices[(j - 1) + (verts_per_band * i) + (count / 2)];
                vertices[(j - 1) + (verts_per_band * i) + (count / 2)] = vertices[j + (verts_per_band * i) + (count / 2)];
                vertices[j + (verts_per_band * i)] = temp;
            }
            else {
                vertices[j + (verts_per_band * i) + (count / 2)] = vertices[j + (verts_per_band * (i - 1)) + (count / 2)];
                vector4 temp = vertices[(j - 1) + (verts_per_band * i) + (count / 2)];
                vertices[(j - 1) + (verts_per_band * i) + (count / 2)] = vertices[j + (verts_per_band * i) + (count / 2)];
                vertices[j + (verts_per_band * i) + (count / 2)] = temp;
            }
        }
    }*/

    for(int i = 0; i < bands; i++) {
        for(int j = 1; j < verts_per_band; j +=3) {
            if(j % 2 != 0)
                vertices[j + (verts_per_band * i) + count / 2] = vertices[((j + 1) + (verts_per_band * (i + 1)) + count / 2) % (count / 2)];
            else {
                vertices[j + (verts_per_band * i) + (count / 2)] = vertices[j + (verts_per_band * (i - 1)) + (count / 2)];
                vector4 temp = vertices[(j + 1) + (verts_per_band * i) + (count / 2)];
                vertices[(j + 1) + (verts_per_band * i) + (count / 2)] = vertices[j + (verts_per_band * i) + (count / 2)];
                vertices[j + (verts_per_band * i) + (count / 2)] = temp;
            }
        }
    }
    return 0;
}

ERROR_NUM band(vector4* vertices, int count, GLfloat radius, GLfloat length) {

    if(vertices == NULL || count <= 0) return MATLIB_POINTER_ERROR;
    if(count % 6 != 0) count = count - (count % 6);

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