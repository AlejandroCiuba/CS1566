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

// projection for perspective
mat4x4 mvm = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
GLuint mvm_location;

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

void init(void) {

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

    // Locate and use transformation matrix per, we need a separate variable for perm_location so we can change it in display()
    mvm_location = glGetUniformLocation(program, "model_view");

    // Location of texture, like location of ctm
    glUniform1i(glGetUniformLocation(program, "texture"), 0);

    // Choose to use either color or texture
    glUniform1i(glGetUniformLocation(program, "use_texture"), use_texture);

    printf("\ntexture_location: %i\n", glGetUniformLocation(program, "texture"));
    
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glClearColor(.7, .7, .7, 1.0);
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

void display(void) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_LINE);

    // Allows for affine matrices: location, # of matrices, transpose, pointer to the matrix you want to send
    glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &ctm);

    // Allows for affine matrices: location, # of matrices, transpose, pointer to the matrix you want to send
    glUniformMatrix4fv(mvm_location, 1, GL_FALSE, (GLfloat *) &mvm);

    // Allows for affine matrices: location, # of matrices, transpose, pointer to the matrix you want to send
    glUniformMatrix4fv(perm_location, 1, GL_FALSE, (GLfloat *) &perm);

    glDrawArrays(GL_TRIANGLES, 0, num_vertices);

    glutSwapBuffers();
}

void reshape(int width, int height) {glViewport(0, 0, 512, 512);}

void mouse(int button, int state, int x, int y) {}

// Captures all movement of the mouse when GLUT_LEFT_BUTTON and GLUT_DOWN
void motion(int x, int y) {}

// ===================== CAMERA COORDINATES =====================
// Spawn-point and Reset Points
vector4 eye = {0, 0, 0, 1};
vector4 look = {0, 0, -1, 1};
vector4 up = {0, 1, 0, 0};
GLfloat radius = 1; // Used for zooming in and out
int turns = 0; // Used to describe the # of turns from the original position

vector4 reye = {0, 0, 0, 1};
vector4 rlook = {0, 0, -1, 1};
vector4 rup = {0, 1, 0, 0};
GLfloat rradius = 1; // Used for zooming in and out
int rturns = 0; // Used to describe the # of turns from the original position

// Animations
typedef enum {BASE, LOOK_UP, LOOK_DOWN, LOOK_RIGHT, LOOK_LEFT, ZOOM_IN, ZOOM_OUT, RESET, ANIM_NUM} animation;
animation anim = BASE;

void keyboard(unsigned char key, int mousex, int mousey) {
    if(key == 'q') quit_program();
    	
    if(key == 'v') for(int i = 0; i < num_vertices; i++) print_vector_ptr(&vertices[i]);

    if(key == 't' && use_texture != -1) {glUniform1i(glGetUniformLocation(program, "use_texture"), use_texture = !use_texture); glutPostRedisplay();}// CURSED, but idc cuz it keeps it small

    // ===================== CAMERA MOVEMENT =====================
    if(key == 'w') anim = LOOK_UP;
    else if(key == 's') anim = LOOK_DOWN;
    else if(key == 'a') anim = LOOK_LEFT;
    else if(key == 'd') anim = LOOK_RIGHT;

    if(key == '+') anim = ZOOM_IN;
    else if(key == '-') anim = ZOOM_OUT;

    if(key == 'r') anim = RESET;
}

void idle() {

    GLfloat degree = 1;
    // ===================== ANIMATIONS =====================
    if(anim == LOOK_UP) {
        // Calculate the degree using the normalized dot product
        vector4 eye_vec = zero_vector;
        vector_sub((vector4*[2]){&eye, &look}, 2, &eye_vec);
        vector_norm(&eye_vec);
        GLfloat dot = 0.0f;
        vector_dot(&up, &eye_vec, &dot);
        if(acos(dot) * 180 / M_PI > 1) {
            GLfloat y = radius * cos((degree * --turns - 270) * M_PI / 180);
            GLfloat z = radius * (sin((degree * turns - 270) * M_PI / 180) - 1);
            eye = (vector4) {eye.x, y, z, 1.0};
            look_at(&eye, &look, &up, &mvm);
            anim = BASE;
        }
    }
    else if(anim == LOOK_DOWN) {
        // Calculate the degree using the normalized dot product
        vector4 eye_vec = zero_vector;
        vector_sub((vector4*[2]){&eye, &look}, 2, &eye_vec);
        vector_norm(&eye_vec);
        GLfloat dot = 0.0f;
        vector_dot(&up, &eye_vec, &dot);
        if(acos(dot) * 180 / M_PI < 179) {
            GLfloat y = radius * cos((degree * ++turns - 270) * M_PI / 180);
            GLfloat z = radius * (sin((degree * turns - 270) * M_PI / 180) - 1);
            eye = (vector4) {eye.x, y, z, 1.0};
            look_at(&eye, &look, &up, &mvm);
            anim = BASE;
        }
    }
    else if(anim == LOOK_RIGHT) {
        GLfloat relative_radius = 1.75f;
        GLfloat x = relative_radius * cos((degree * --turns + 90) * M_PI / 180);
        GLfloat z = relative_radius * (sin((degree * turns + 90) * M_PI / 180) - 1);
        eye = (vector4) {x, eye.y, z, 1.0};
        look_at(&eye, &look, &up, &mvm);
        anim = BASE;
    }
    else if(anim == LOOK_LEFT) {
        GLfloat relative_radius = 1.75f;
        GLfloat x = relative_radius * cos((degree * ++turns + 90) * M_PI / 180);
        GLfloat z = relative_radius * (sin((degree * turns + 90) * M_PI / 180) - 1);
        eye = (vector4) {x, eye.y, z, 1.0};
        look_at(&eye, &look, &up, &mvm);
        anim = BASE;
    }
    else if(anim == ZOOM_IN) {
        anim = BASE;
    }
    else if(anim == ZOOM_OUT) {
        anim = BASE;
    }
    else if(anim == RESET) {
        eye = reye;
        look = rlook;
        up = rup;
        radius = rradius;
        turns = rturns;
        look_at(&eye, &look, &up, &mvm);
        printf("\nRESET\n");
        anim = BASE;
    }
    look_at(&eye, &look, &up, &mvm);
    glutPostRedisplay();
}

// Obtains the user's input for the file to load
int menu() {return 0;}

// I wrote the coloring of the cube here to keep the original shapes.c cleaner
void color_rubix(vector4* colors, int num_vertices);
void color_cube(vector4* colors, color* face_colors);

int main(int argc, char **argv) {

    // ===================== LOAD-IN RUBIX CUBE =====================
    rubix_cube(vertices = (vector4*) malloc(sizeof(vector4) * num_vertices));
    
    // Load texels
    width = 320;
    height = 320;
    texels = (GLubyte*) malloc(sizeof(GLubyte) * width * height * 3);

    //Load sphere texture
    image = fopen("texture01.raw", "r");
    load_raw(image, texels, width, height);
    fclose(image);

    // Assign color and print statistics
    //random_colors(colors = (vector4*) malloc(sizeof(vector4) * num_vertices), num_vertices);
    color_rubix(colors = (vector4*) malloc(sizeof(vector4) * num_vertices), num_vertices);

    // Demonstrates how texture scaling is only done AFTER creation, things should be in the ratio you plan to see them
    mat4x4 sc, tra, fin;
    scal((affine){.5,.5,.5}, &sc);
    trans((affine){0,0,-1.75}, &tra);
    mat_mult((mat4x4[2]){tra, sc}, 2, &fin);
    matxvar(&fin, vertices, num_vertices, vertices);

    // ===================== CHANGE CAMERA LOCATION =====================
    vector4 co = zero_vector;
    com(vertices, num_vertices, &co);
    copy_vector(&co, &look);
    copy_vector(&look, &rlook);
    radius = rradius = look.z * -1;
    look_at(&eye, &look, &up, &mvm);

    printf("\nCENTER OF THE RUBIX CUBE\n");
    print_vector(co);

    printf("\nMODEL-VIEW MATRIX\n");
    print_matrix(mvm);
    printf("\nEYE\n");
    print_vector(eye);
    printf("\nLOOK\n");
    print_vector(look);
    printf("\nUP\n");
    print_vector(up);

    // ===================== WORLD VIEW =====================
    view world = {-1, 1, 1, -1, -1, -10};
    perspective(&world, &perm);
    printf("\nPERSPECTIVE MATRIX\n");
    print_matrix(perm);

    printf("VERTEX COUNT: %d\n",  num_vertices);

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

// I wrote the coloring of the cube here to keep the original shapes.c cleaner
void color_rubix(vector4* colors, int num_vertices) {

    if(colors == NULL) return;

    // ===================== YELLOW SIDE =====================
    // Bottom
    color_cube(colors, (color[6]){BLACK, BLUE, BLACK, ORANGE, BLACK, YELLOW});
    color_cube(colors + 36, (color[6]){BLACK, BLACK, BLACK, ORANGE, BLACK, YELLOW});
    color_cube(colors + 72, (color[6]){GREEN, BLACK, BLACK, ORANGE, BLACK, YELLOW});

    // Mid
    color_cube(colors + 108, (color[6]){BLACK, BLUE, BLACK, ORANGE, BLACK, BLACK});
    color_cube(colors + 144, (color[6]){BLACK, BLACK, BLACK, ORANGE, BLACK, BLACK});
    color_cube(colors + 180, (color[6]){GREEN, BLACK, BLACK, ORANGE, BLACK, BLACK});

    // Top
    color_cube(colors + 216, (color[6]){BLACK, BLUE, BLACK, ORANGE, WHITE, BLACK});
    color_cube(colors + 252, (color[6]){BLACK, BLACK, BLACK, ORANGE, WHITE, BLACK});
    color_cube(colors + 288, (color[6]){GREEN, BLACK, BLACK, ORANGE, WHITE, BLACK});

    // ===================== MIDDLE PART =====================
    // Bottom
    color_cube(colors + 324, (color[6]){BLACK, BLUE, BLACK, BLACK, BLACK, YELLOW});
    color_cube(colors + 360, (color[6]){BLACK, BLACK, BLACK, BLACK, BLACK, YELLOW});
    color_cube(colors + 396, (color[6]){GREEN, BLACK, BLACK, BLACK, BLACK, YELLOW});

    // Mid
    color_cube(colors + 432, (color[6]){BLACK, BLUE, BLACK, BLACK, BLACK, BLACK});
    color_cube(colors + 468, (color[6]){BLACK, BLACK, BLACK, BLACK, BLACK, BLACK});
    color_cube(colors + 504, (color[6]){GREEN, BLACK, BLACK, BLACK, BLACK, BLACK});

    // Top
    color_cube(colors + 540, (color[6]){BLACK, BLUE, BLACK, BLACK, WHITE, BLACK});
    color_cube(colors + 576, (color[6]){BLACK, BLACK, BLACK, BLACK, WHITE, BLACK});
    color_cube(colors + 612, (color[6]){GREEN, BLACK, BLACK, BLACK, WHITE, BLACK});

    // ===================== RED SIDE =====================
    // Bottom
    color_cube(colors + 648, (color[6]){BLACK, BLUE, RED, BLACK, BLACK, YELLOW});
    color_cube(colors + 684, (color[6]){BLACK, BLACK, RED, BLACK, BLACK, YELLOW});
    color_cube(colors + 720, (color[6]){GREEN, BLACK, RED, BLACK, BLACK, YELLOW});

    // Mid
    color_cube(colors + 756, (color[6]){BLACK, BLUE, RED, BLACK, BLACK, BLACK});
    color_cube(colors + 792, (color[6]){BLACK, BLACK, RED, BLACK, BLACK, BLACK});
    color_cube(colors + 828, (color[6]){GREEN, BLACK, RED, BLACK, BLACK, BLACK});

    // Top
    color_cube(colors + 864, (color[6]){BLACK, BLUE, RED, BLACK, WHITE, BLACK});
    color_cube(colors + 900, (color[6]){BLACK, BLACK, RED, BLACK, WHITE, BLACK});
    color_cube(colors + 936, (color[6]){GREEN, BLACK, RED, BLACK, WHITE, BLACK});
}

// Front, Back, Right, Left, Up, Down
void color_cube(vector4* colors, color* face_colors) {

    if(colors == NULL || face_colors == NULL) return;

    for(int i = 0; i < 6; i++)
        const_color(colors + (i * 6), 6, face_colors[i]);
}