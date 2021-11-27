/*
 * template.c
 *
 * An OpenGL source code template.
 */


#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../Catorce/matrix_library/initShader.h"
#include "../Catorce/matrix_library/matrix_def.h"
#include "../Catorce/matrix_library/matrix_utility.h"
#include "../Catorce/matrix_library/matrix_ops.h"
#include "../Catorce/shapes_library/shapes.h"
#include "../Catorce/shapes_library/affine.h"
#include "../Catorce/other/file_reader.h"
#include "../Catorce/camera/camera.h"

#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

// MAIN PROGRAM HERE SO WE CAN DO ANYTHING WE WANT!!!
GLuint program = -1;

// ===================== UNIFORM VARIABLES =====================
// ctm for manipulating shapes
mat4x4 ctm = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
GLuint ctm_location; // Need for display()

// projection for perspective
mat4x4 perm = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
GLuint perm_location;

// Texels for texture
GLubyte* texels;
int width = 0, height = 0;
FILE* image = NULL;

// Option to use texture or color, set to 1 by default (ASSUMES TEXTURE)
int use_texture = 0;

// ===================== VERTEX ATTRIBUTES =====================
vector4* vertices;
vector4* colors;
vector2* texcoords;

int num_vertices = 972;

void init(void)
{
    // Load the vertex and fragment shaders
    program = initShader("vshader.glsl", "fshader.glsl");
    glUseProgram(program);
    
    // Puts texel array onto graphics pipeline
    GLuint mytex[1];
    glGenTextures(1, mytex);
    glBindTexture(GL_TEXTURE_2D, mytex[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texels);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

    // idk...
    int param;
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &param);

    // Use this to transport between here and graphics pipeline
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Set aside memory on Graphics Card
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vector4) * 2 * num_vertices + sizeof(vector2) * num_vertices, NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vector4) * num_vertices, vertices);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vector4) * num_vertices, sizeof(vector4) * num_vertices, colors);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vector4) * 2 * num_vertices, sizeof(vector2) * num_vertices, texcoords);

    // Use this for passing the position info into the vertex and fragment shaders
    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    // Use this for passing the color info into the vertex and fragment shaders
    GLuint vColor = glGetAttribLocation(program, "vColor");
    glEnableVertexAttribArray(vColor);
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *) (sizeof(vector4) * num_vertices));

    // Use this for passing the texture coordinate info into the vertex and fragment shaders
    GLuint vTexCoord = glGetAttribLocation(program, "vTexCoord");
    glEnableVertexAttribArray(vTexCoord);
    glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid *) (sizeof(vector4) * 2 * num_vertices + 0));

    // Locate and use transformation matrix ctm, we need a separate variable for ctm_location so we can change it in display()
    ctm_location = glGetUniformLocation(program, "ctm");

    // Locate and use transformation matrix per, we need a separate variable for perm_location so we can change it in display()
    perm_location = glGetUniformLocation(program, "projection");

    // Location of texture, like location of ctm
    glUniform1i(glGetUniformLocation(program, "texture"), 0);

    // Choose to use either color or texture
    glUniform1i(glGetUniformLocation(program, "use_texture"), use_texture);

    printf("\ntexture_location: %i\n", glGetUniformLocation(program, "texture"));
    
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glDepthRange(1,0);
}

// Exits program, freeing all alocated memory
void quit_program() {
        glutLeaveMainLoop();
        free(vertices);
        free(colors);
        free(texels);
        free(texcoords);
        printf("\nEXIT SUCCESSFUL\n");
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_LINE);

    // Allows for affine matrices: location, # of matrices, transpose, pointer to the matrix you want to send
    glUniformMatrix4fv(glGetUniformLocation(program, "ctm"), 1, GL_FALSE, (GLfloat *) &ctm);

    // Allows for affine matrices: location, # of matrices, transpose, pointer to the matrix you want to send
    glUniformMatrix4fv(perm_location, 1, GL_FALSE, (GLfloat *) &perm);

    glDrawArrays(GL_TRIANGLES, 0, num_vertices);

    glutSwapBuffers();
}

void reshape(int width, int height)
{
    glViewport(0, 0, 512, 512);
}

// ===================== ROTATION =====================
// Calculate z-Axis point and degree
GLfloat z_treatment(GLfloat x, GLfloat y) {return sqrt(1 - pow(x, 2) - pow(y, 2));}
GLfloat rad_to_degrees(GLfloat rad) {return rad * 180 / M_PI;}

// World points used to determine the axis of rotation via cross product
vector4 world_points_1 = {0,0,0,1}, world_points_2 = {0,0,FP_NAN,1}; // Yes...
vector4 center = {0,0,0,1};
mat4x4 final_rot = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};

// ====================================================
// Keep the previous ctm to not reset between every call
mat4x4 ctm_base = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};

// Handles zoom in and zoom out
// Global Variables associated with scale
affine s = {1,1,1};
mat4x4 final_scal = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};

void mouse(int button, int state, int x, int y) {

    // ===================== SCROLLING SIZE =====================
    mat4x4 sc = identity;

    if(button == 3) s = (affine) {s.x + .02, s.y + .02, s.z + .02};
    else if(button == 4) s = (affine) {s.x - .02, s.y - .02, s.z - .02};
    else s = (affine) {1,1,1};

    // Resize management
    scal(s, &sc);
    copy_matrix(&sc, &final_scal);

    // ===================== TURNING OBJECT =====================
    // Captures the beginning of the rotation (The first in the cross product)
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        screen_to_world(&(vector4){x, y, 0, 1}, &world_points_1, 512, 512, z_treatment);
        if(!isnan(world_points_1.z)) copy_matrix(&ctm, &ctm_base); // Keep the current ctm to modify
    }
    else if(button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        final_rot = identity;
        copy_matrix(&ctm, &ctm_base); // Keep the current ctm and stay there
    }
}

// Captures all movement of the mouse when GLUT_LEFT_BUTTON and GLUT_DOWN
void motion(int x, int y) {

    vector4 screen_points = {x, y, 0, 1};
    vector4 cross = {0,0,0,1};
    GLfloat rad = 0.0f;

    // Gets the second point in the cross product (constantly changes)
    screen_to_world(&screen_points, &world_points_2, 512, 512, z_treatment);
        
    // If inside sphere of rotation calculate the cross product vector and normalize it
    // Also obtain the amount of rotation and rotate around said axis said degrees
    if(!(isnan(world_points_1.z) || isnan(world_points_2.z))) {

        world_points_1.w = 0.0f;
        world_points_2.w = 0.0f;
        vector_norm(&world_points_1);
        vector_norm(&world_points_2);

        vector_cross(&world_points_1, &world_points_2, &cross);
        vector_norm(&cross);
        vector_dot(&world_points_1, &world_points_2, &rad);

        rotate_arb(rad_to_degrees(acos(rad)), &cross, &center, &final_rot);
    } else {final_rot = identity; copy_matrix(&ctm, &ctm_base);} // Don't do anything else otherwise
}

void keyboard(unsigned char key, int mousex, int mousey)
{
    if(key == 'q') quit_program();
    	
    if(key == 'd') for(int i = 0; i < num_vertices; i++) print_vector_ptr(&vertices[i]);

    if(key == 'r') {final_rot = identity; final_scal = identity; ctm_base = identity;}

    if(key == 't' && use_texture != -1) {glUniform1i(glGetUniformLocation(program, "use_texture"), use_texture = !use_texture); glutPostRedisplay();}// CURSED, but idc cuz it keeps it small

    // ===================== CHANGING SIZE VIA KEYBOARD =====================
    mat4x4 sc = identity;

    // Optional keyboard input
    if(key == '+') s = (affine) {s.x + .02, s.y + .02, s.z + .02};
    else if(key == '-') s = (affine) {s.x - .02, s.y - .02, s.z - .02};

    // Resize management
    scal(s, &sc);
    copy_matrix(&sc, &final_scal);
}

void idle() {
    // Take the values gotten from the rotation and scaling and apply it via the ctm
    mat_mult((mat4x4[3]){final_rot, final_scal, ctm_base}, 3, &ctm);
    glutPostRedisplay();
}

// Obtains the user's input for the file to load
int menu() {
    return 0;
}

int main(int argc, char **argv)
{   
    //sphere(vertices = (vector4*) malloc(sizeof(vector4) * num_vertices), num_vertices, 1, 16);
    //rect3D(vertices = (vector4*) malloc(sizeof(vector4) * num_vertices), .5, .5, .5);

    rubix_cube(vertices = (vector4*) malloc(sizeof(vector4) * num_vertices));
    
    // Load texels
    width = 320;
    height = 320;
    texels = (GLubyte*) malloc(sizeof(GLubyte) * width * height * 3);

    //Load sphere texture
    image = fopen("texture01.raw", "r");
    load_raw(image, texels, width, height);
    fclose(image);

    // We only want the top fourth of the texture, change texture scale on x and y to 4, then apply texture
    // texture_scale(4, -4);
    // texturize3D(texcoords = (vector2*) malloc(sizeof(vector2) * num_vertices), num_vertices, SPHERE, vertices);

    // Assign color and print statistics
    random_colors(colors = (vector4*) malloc(sizeof(vector4) * num_vertices), num_vertices);

    // Demonstrates how texture scaling is only done AFTER creation, things should be in the ratio you plan to see them
    mat4x4 sc, tra, fin;
    scal((affine){.5,.5,.5}, &sc);
    trans((affine){0,0,-1.75}, &tra);
    mat_mult((mat4x4[2]){tra, sc}, 2, &fin);
    matxvar(&fin, vertices, num_vertices, vertices);

    // ===================== WORLD VIEW =====================
    view world = {-1, 1, 1, -1, -1, -10};
    perspective(&world, &perm);
    printf("\nPERSPECTIVE MATRIX\n");
    print_matrix(perm);

    printf("VERTEX COUNT: %d\n",  num_vertices);
    
    // Get center of mass
    com(vertices, num_vertices, &center);

    // Create program and initialize the viewing windos
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(512, 512);
    glutInitWindowPosition(100,100);
    glutCreateWindow("Project 4");

    // Assign the functions to their jobs
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutIdleFunc(idle);

    // Start the program and event-driven loop
    glewInit();
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutReshapeFunc(reshape);
    glutMainLoop();

    return 0;
}
