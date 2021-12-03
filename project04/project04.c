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

int num_vertices = 3564;

// ===================== CAMERA COORDINATES =====================
// Spawn-point and Reset Points
vector4 eye = {0.01, 0.01, 1, 1};
vector4 look = {0, 0, 0, 1};
vector4 up = {0, 1, 0, 0};
GLfloat radius = 1; // Used for zooming in and out
int turns = 0; // Used to describe the # of turns from the original position

vector4 reye = {0.01, 0.01, 1, 1};
vector4 rlook = {0, 0, 0, 1};
vector4 rup = {0, 1, 0, 0};
GLfloat rradius = 1; // Used for zooming in and out
int rturns = 0; // Used to describe the # of turns from the original position

// Center of Objects Mass
vector4 co = {0,0,0,1};

// Camera Movements
typedef enum {BASE, LOOK_UP, LOOK_DOWN, LOOK_RIGHT, LOOK_LEFT, ZOOM_IN, ZOOM_OUT, RESET, CAM_NUM} camera;
camera cam = BASE;

// ===================== RUBIX REPRESENTATION =====================
// Rubix Cube Animations
typedef enum {FRONT, BACK, LEFT, RIGHT, TOP, BOTTOM, DEFAULT, NONE, ANIM_NUM} animation;
animation anim = NONE;

// I wrote the coloring of the cube here to keep the original shapes.c cleaner
void color_rubix(vector4* colors, int num_vertices);
void color_cube(vector4* colors, color* face_colors);

// The Cubits are labelled 0-26 and stored into 6 2D arrays representing each face
// The 0 - 26 are determined by how many cubits one needs to traverse in the Rubix Cube
// Array to find it. These are their starting positions in the cube,
int front[3][3] = {{8,17,26},{5,14,23},{2,11,20}};
int back[3][3] = {{24,15,6},{21,12,3},{18,9,0}};
int left[3][3] = {{6,7,8},{3,4,5},{0,1,2}};
int right[3][3] = {{26,25,24},{23,22,21},{20,19,18}};
int top[3][3] = {{6,15,24},{7,16,25},{8,17,26}};
int bottom[3][3] = {{2,11,20},{1,10,19},{0,9,18}};

// The array of ctms which correspond to 1 cubit each
mat4x4 ctm_rubix[27];

// Functions that manipulate the rubix cube visuals and logic
void rot_grid(animation side);
void rot_cubits(mat4x4* ctms, GLfloat deg, animation side);

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
    for(int i = 0; i < 27; i++) {
        glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &ctm_rubix[i]);

        // Allows for affine matrices: location, # of matrices, transpose, pointer to the matrix you want to send
        glUniformMatrix4fv(mvm_location, 1, GL_FALSE, (GLfloat *) &mvm);

        // Allows for affine matrices: location, # of matrices, transpose, pointer to the matrix you want to send
        glUniformMatrix4fv(perm_location, 1, GL_FALSE, (GLfloat *) &perm);

        glDrawArrays(GL_TRIANGLES, (i * 132), 132);
    }

    glutSwapBuffers();
}

void reshape(int width, int height) {glViewport(0, 0, 512, 512);}

void mouse(int button, int state, int x, int y) {}

// Captures all movement of the mouse when GLUT_LEFT_BUTTON and GLUT_DOWN
void motion(int x, int y) {}

void keyboard(unsigned char key, int mousex, int mousey) {

    if(key == 'q') quit_program();
    	
    if(key == 'v') for(int i = 0; i < num_vertices; i++) {printf("%d: ", i); print_vector_ptr(&vertices[i]);}

    // ===================== CAMERA MOVEMENT =====================
    if(key == 'w') cam = LOOK_UP;
    else if(key == 's') cam = LOOK_DOWN;
    else if(key == 'a') cam = LOOK_LEFT;
    else if(key == 'd') cam = LOOK_RIGHT;

    if(key == '+') cam = ZOOM_IN;
    else if(key == '-') cam = ZOOM_OUT;

    if(key == 'r') cam = RESET;

    // ===================== RUBIX CUBE ANIMATIONS =====================
    if(key == '1') anim = FRONT;
    else if(key == '2') anim = BACK;
    else if(key == '3') anim = LEFT;
    else if(key == '4') anim = RIGHT;
    else if(key == '5') anim = TOP;
    else if(key == '6') anim = BOTTOM;
    else if(key == '7') anim = DEFAULT;
}

void idle() {

    GLfloat degree = 5;
    // ===================== CAMERA MOVEMENTS =====================
    if(cam == LOOK_UP) {
        if(turns < 17) {
            turns++;
            // Calculate the cross product
            vector4 cross = zero_vector;
            vector4 eye_vec = {eye.x - look.x, eye.y - look.y, eye.z - look.z, 0};
            vector_cross(&up, &eye_vec, &cross);
            vector_norm(&cross);
            mat4x4 ro = zero_matrix;
            rotate_arb(-degree, &cross, &co, &ro);
            matxvec(&ro, &eye, &eye);
            look_at(&eye, &look, &up, &mvm);
        }
        cam = BASE;
    }
    else if(cam == LOOK_DOWN) {
        if(turns > -17) {
            turns--;
            // Calculate the cross product
            vector4 cross = zero_vector;
            vector4 eye_vec = {eye.x - look.x, eye.y - look.y, eye.z - look.z, 0};
            vector_cross(&up, &eye_vec, &cross);
            vector_norm(&cross);
            mat4x4 ro = zero_matrix;
            rotate_arb(degree, &cross, &co, &ro);
            matxvec(&ro, &eye, &eye);
            look_at(&eye, &look, &up, &mvm);
        }
        cam = BASE;
    }
    else if(cam == LOOK_RIGHT) {
        mat4x4 ro = zero_matrix;
        rotate_arb(degree, &up, &co, &ro);
        matxvec(&ro, &eye, &eye);
        look_at(&eye, &look, &up, &mvm);
        cam = BASE;
    }
    else if(cam == LOOK_LEFT) {
        mat4x4 ro = zero_matrix;
        rotate_arb(-degree, &up, &co, &ro);
        matxvec(&ro, &eye, &eye);
        look_at(&eye, &look, &up, &mvm);
        cam = BASE;
    }
    else if(cam == ZOOM_IN) {
        mat4x4 sca = zero_matrix;
        scal((affine){.9,.9,.9}, &sca);
        matxvec(&sca, &eye, &eye);
        look_at(&eye, &look, &up, &mvm);
        cam = BASE;
    }
    else if(cam == ZOOM_OUT) {
        mat4x4 sca = zero_matrix;
        scal((affine){1.1,1.1,1.1}, &sca);
        matxvec(&sca, &eye, &eye);
        look_at(&eye, &look, &up, &mvm);
        cam = BASE;
    }
    else if(cam == RESET) {
        radius = rradius;
        eye = reye;
        look = rlook;
        up = rup;
        look_at(&eye, &look, &up, &mvm);
        cam = BASE;
        printf("\nRESET\n");
    }

    // Apply changes to modal-view matrix
    look_at(&eye, &look, &up, &mvm);

    // ===================== CAMERA MOVEMENTS =====================
    GLfloat rot_cubits_deg = 2;
    if(anim == FRONT) {
        rot_cubits(&ctm_rubix, -rot_cubits_deg, FRONT);
        //rot_grid(FRONT);
        anim = NONE;
    }
    else if(anim == BACK) {
        rot_grid(BACK);
        anim = NONE;
    }
    else if(anim == LEFT) {
        rot_grid(LEFT);
        anim = NONE;
    }
    else if(anim == RIGHT) {
        rot_grid(RIGHT);
        anim = NONE;
    }
    else if(anim == TOP) {
        rot_grid(TOP);
        anim = NONE;
    }
    else if(anim == BOTTOM) {
        rot_grid(BOTTOM);
        anim = NONE;
    }
    else if(anim == DEFAULT) {
        anim = NONE;
    }
    glutPostRedisplay();
}

// Obtains the user's input for the file to load
int menu() {return 0;}

int main(int argc, char **argv) {

    // ===================== LOAD-IN RUBIX CUBE =====================
    rubix_cube(vertices = (vector4*) malloc(sizeof(vector4) * num_vertices));
    //cubit(vertices = (vector4*) malloc(sizeof(vector4) * num_vertices));
    // Assign color and print statistics
    //random_colors(colors = (vector4*) malloc(sizeof(vector4) * num_vertices), num_vertices);
    color_rubix(colors = (vector4*) malloc(sizeof(vector4) * num_vertices), num_vertices);

    // Set all RUbix Cube CTMs to identity
    for(int i = 0; i < 27; i++) ctm_rubix[i] = identity;

    // Move Rubix Cube to the origin
    com(vertices, num_vertices, &co);
    mat4x4 tra = zero_matrix;
    mat4x4 sc = zero_matrix;
    mat4x4 fin = zero_matrix;
    trans((affine){-co.x, -co.y, -co.z}, &tra);
    scal((affine){.2,.2,.2}, &sc);
    mat_mult((mat4x4[2]){sc,tra}, 2, &fin);
    matxvar(&fin, vertices, num_vertices, vertices);
    // ===================== CHANGE CAMERA LOCATION =====================
    radius = rradius = 2;
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
    view world = {-.1, .1, .1, -.1, -.1, -10};
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

    // Fill black
    for(int i = 0; i < num_vertices; i++) colors[i] = BLACK;

    // ===================== YELLOW SIDE =====================
    // Bottom
    color_cube(colors, (color[6]){BLACK, BLUE, BLACK, ORANGE, BLACK, YELLOW});
    color_cube(colors + 132, (color[6]){BLACK, BLACK, BLACK, ORANGE, BLACK, YELLOW});
    color_cube(colors + 264, (color[6]){GREEN, BLACK, BLACK, ORANGE, BLACK, YELLOW});

    // Mid
    color_cube(colors + 396, (color[6]){BLACK, BLUE, BLACK, ORANGE, BLACK, BLACK});
    color_cube(colors + 528, (color[6]){BLACK, BLACK, BLACK, ORANGE, BLACK, BLACK});
    color_cube(colors + 660, (color[6]){GREEN, BLACK, BLACK, ORANGE, BLACK, BLACK});

    // Top
    color_cube(colors + 792, (color[6]){BLACK, BLUE, BLACK, ORANGE, WHITE, BLACK});
    color_cube(colors + 924, (color[6]){BLACK, BLACK, BLACK, ORANGE, WHITE, BLACK});
    color_cube(colors + 1056, (color[6]){GREEN, BLACK, BLACK, ORANGE, WHITE, BLACK});

    // ===================== MIDDLE PART =====================
    // Bottom
    color_cube(colors + 1188, (color[6]){BLACK, BLUE, BLACK, BLACK, BLACK, YELLOW});
    color_cube(colors + 1320, (color[6]){BLACK, BLACK, BLACK, BLACK, BLACK, YELLOW});
    color_cube(colors + 1452, (color[6]){GREEN, BLACK, BLACK, BLACK, BLACK, YELLOW});

    // Mid
    color_cube(colors + 1584, (color[6]){BLACK, BLUE, BLACK, BLACK, BLACK, BLACK});
    color_cube(colors + 1716, (color[6]){BLACK, BLACK, BLACK, BLACK, BLACK, BLACK});
    color_cube(colors + 1848, (color[6]){GREEN, BLACK, BLACK, BLACK, BLACK, BLACK});

    // Top
    color_cube(colors + 1980, (color[6]){BLACK, BLUE, BLACK, BLACK, WHITE, BLACK});
    color_cube(colors + 2112, (color[6]){BLACK, BLACK, BLACK, BLACK, WHITE, BLACK});
    color_cube(colors + 2244, (color[6]){GREEN, BLACK, BLACK, BLACK, WHITE, BLACK});

    // ===================== RED SIDE =====================
    // Bottom
    color_cube(colors + 2376, (color[6]){BLACK, BLUE, RED, BLACK, BLACK, YELLOW});
    color_cube(colors + 2508, (color[6]){BLACK, BLACK, RED, BLACK, BLACK, YELLOW});
    color_cube(colors + 2640, (color[6]){GREEN, BLACK, RED, BLACK, BLACK, YELLOW});

    // Mid
    color_cube(colors + 2772, (color[6]){BLACK, BLUE, RED, BLACK, BLACK, BLACK});
    color_cube(colors + 2904, (color[6]){BLACK, BLACK, RED, BLACK, BLACK, BLACK});
    color_cube(colors + 3036, (color[6]){GREEN, BLACK, RED, BLACK, BLACK, BLACK});

    // Top
    color_cube(colors + 3168, (color[6]){BLACK, BLUE, RED, BLACK, WHITE, BLACK});
    color_cube(colors + 3300, (color[6]){BLACK, BLACK, RED, BLACK, WHITE, BLACK});
    color_cube(colors + 3432, (color[6]){GREEN, BLACK, RED, BLACK, WHITE, BLACK});
}

// Front, Back, Right, Left, Up, Down
void color_cube(vector4* colors, color* face_colors) {

    if(colors == NULL || face_colors == NULL) return;

    for(int i = 0; i < 6; i++)
        const_color(colors + (i * 6), 6, face_colors[i]);
}

// Functions that manipulate the rubix cube visuals and logic
void rot_grid(animation side) {
    
    int cube[6][3][3] = {{{front[0][0], front[0][1], front[0][2]},{front[1][0], front[1][1], front[1][2]},{front[2][0], front[2][1], front[2][2]}}, 
                        {{back[0][0], back[0][1], back[0][2]},{back[1][0], back[1][1], back[1][2]},{back[2][0], back[2][1], back[2][2]}}, 
                        {{left[0][0], left[0][1], left[0][2]},{left[1][0], left[1][1], left[1][2]},{left[2][0], left[2][1], left[2][2]}}, 
                        {{right[0][0], right[0][1], right[0][2]},{right[1][0], right[1][1], right[1][2]},{right[2][0], right[2][1], right[2][2]}}, 
                        {{top[0][0], top[0][1], top[0][2]},{top[1][0], top[1][1], top[1][2]},{top[2][0], top[2][1], top[2][2]}}, 
                        {{bottom[0][0], bottom[0][1], bottom[0][2]},{bottom[1][0], bottom[1][1], bottom[1][2]},{bottom[2][0], bottom[2][1], bottom[2][2]}}};
    
    int temp[9] = {cube[side][0][0],cube[side][0][1],cube[side][0][2],
                    cube[side][1][0],cube[side][1][1],cube[side][1][2],
                    cube[side][2][0],cube[side][2][1],cube[side][2][2]};
    
    // Rotate the face
    cube[side][0][0] = temp[3]; cube[side][0][1] = temp[0]; cube[side][0][2] = temp[1];
    cube[side][1][0] = temp[6]; /*THE CENTER NEVER MOVES!*/ cube[side][1][2] = temp[2];
    cube[side][2][0] = temp[7]; cube[side][2][1] = temp[8]; cube[side][2][2] = temp[5];
    
    // Change the appropriate sides accordingly
    switch(side) {
        case FRONT:
            // Rotate the face
            front[0][0] = temp[3]; front[0][1] = temp[0]; front[0][2] = temp[1];
            front[1][0] = temp[6]; /*THE CENTER NEVER MOVES!*/ front[1][2] = temp[2];
            front[2][0] = temp[7]; front[2][1] = temp[8]; front[2][2] = temp[5];

            left[0][2] = cube[FRONT][0][0];
            left[1][2] = cube[FRONT][1][0];
            left[2][2] = cube[FRONT][2][0];

            right[0][0] = cube[FRONT][0][2];
            right[1][0] = cube[FRONT][1][2];
            right[2][0] = cube[FRONT][2][2];

            top[2][0] = cube[FRONT][0][0];
            top[2][1] = cube[FRONT][0][1];
            top[2][2] = cube[FRONT][0][2];

            bottom[0][0] = cube[FRONT][2][0];
            bottom[0][1] = cube[FRONT][2][1];
            bottom[0][2] = cube[FRONT][2][2];
            break;
        case BACK:
            // Rotate the face
            back[0][0] = temp[3]; back[0][1] = temp[0]; back[0][2] = temp[1];
            back[1][0] = temp[6]; /*THE CENTER NEVER MOVES!*/ back[1][2] = temp[2];
            back[2][0] = temp[7]; back[2][1] = temp[8]; back[2][2] = temp[5];

            left[0][0] = cube[BACK][0][2];
            left[1][0] = cube[BACK][1][2];
            left[2][0] = cube[BACK][2][2];

            right[0][2] = cube[BACK][0][0];
            right[1][2] = cube[BACK][1][0];
            right[2][2] = cube[BACK][2][0];

            top[0][0] = cube[BACK][0][2];
            top[0][1] = cube[BACK][0][1];
            top[0][2] = cube[BACK][0][0];

            bottom[2][0] = cube[BACK][2][2];
            bottom[2][1] = cube[BACK][2][1];
            bottom[2][2] = cube[BACK][2][0];
            break;
        case LEFT:
            // Rotate the face
            left[0][0] = temp[3]; left[0][1] = temp[0]; left[0][2] = temp[1];
            left[1][0] = temp[6]; /*THE CENTER NEVER MOVES!*/ left[1][2] = temp[2];
            left[2][0] = temp[7]; left[2][1] = temp[8]; left[2][2] = temp[5];

            front[0][0] = cube[LEFT][0][2];
            front[1][0] = cube[LEFT][1][2];
            front[2][0] = cube[LEFT][2][2];

            back[0][2] = cube[LEFT][0][0];
            back[1][2] = cube[LEFT][1][0];
            back[2][2] = cube[LEFT][2][0];

            top[0][0] = cube[LEFT][0][0];
            top[1][0] = cube[LEFT][0][1];
            top[2][0] = cube[LEFT][0][2];

            bottom[0][0] = cube[LEFT][2][2];
            bottom[1][0] = cube[LEFT][2][1];
            bottom[2][0] = cube[LEFT][2][0];
            break;
        case RIGHT:
            // Rotate the face
            right[0][0] = temp[3]; right[0][1] = temp[0]; right[0][2] = temp[1];
            right[1][0] = temp[6]; /*THE CENTER NEVER MOVES!*/ right[1][2] = temp[2];
            right[2][0] = temp[7]; right[2][1] = temp[8]; right[2][2] = temp[5];

            front[0][2] = cube[RIGHT][0][0];
            front[1][2] = cube[RIGHT][1][0];
            front[2][2] = cube[RIGHT][2][0];

            back[0][0] = cube[RIGHT][0][2];
            back[1][0] = cube[RIGHT][1][2];
            back[2][0] = cube[RIGHT][2][2];

            top[2][2] = cube[RIGHT][0][0];
            top[1][2] = cube[RIGHT][0][1];
            top[0][2] = cube[RIGHT][0][2];

            bottom[0][2] = cube[RIGHT][2][0];
            bottom[1][2] = cube[RIGHT][2][1];
            bottom[2][2] = cube[RIGHT][2][2];
            break;
        case TOP:
            // Rotate the face
            top[0][0] = temp[3]; top[0][1] = temp[0]; top[0][2] = temp[1];
            top[1][0] = temp[6]; /*THE CENTER NEVER MOVES!*/ top[1][2] = temp[2];
            top[2][0] = temp[7]; top[2][1] = temp[8]; top[2][2] = temp[5];

            front[0][0] = cube[TOP][2][0];
            front[0][1] = cube[TOP][2][1];
            front[0][2] = cube[TOP][2][2];

            back[0][0] = cube[TOP][0][2];
            back[0][1] = cube[TOP][0][1];
            back[0][2] = cube[TOP][0][0];

            left[0][0] = cube[TOP][0][0];
            left[0][1] = cube[TOP][1][0];
            left[0][2] = cube[TOP][2][0];

            right[0][0] = cube[TOP][2][2];
            right[0][1] = cube[TOP][1][2];
            right[0][2] = cube[TOP][0][2];
            break;
        case BOTTOM:
            // Rotate the face
            bottom[0][0] = temp[3]; bottom[0][1] = temp[0]; bottom[0][2] = temp[1];
            bottom[1][0] = temp[6]; /*THE CENTER NEVER MOVES!*/ bottom[1][2] = temp[2];
            bottom[2][0] = temp[7]; bottom[2][1] = temp[8]; bottom[2][2] = temp[5];

            front[2][0] = cube[BOTTOM][0][0];
            front[2][1] = cube[BOTTOM][0][1];
            front[2][2] = cube[BOTTOM][0][2];

            back[2][2] = cube[BOTTOM][2][0];
            back[2][1] = cube[BOTTOM][2][1];
            back[2][0] = cube[BOTTOM][2][2];

            left[2][0] = cube[BOTTOM][2][0];
            left[2][1] = cube[BOTTOM][1][0];
            left[2][2] = cube[BOTTOM][0][0];

            right[2][0] = cube[BOTTOM][0][2];
            right[2][1] = cube[BOTTOM][1][2];
            right[2][2] = cube[BOTTOM][2][2];
            break;
        default:
            break;
    }
}

void rot_cubits(mat4x4* ctms, GLfloat deg, animation side) {

    if(ctms == NULL) return;

    // Rotate the ctms in the side
    switch(side) {
        case FRONT:
            for(int i = 0; i < 3; i++)
                for(int j = 0; j < 3; j++)
                    rotate_arb(deg, &z_axis, &co, &ctm_rubix[front[i][j]]);
            break;
        case BACK:
            for(int i = 0; i < 3; i++)
                for(int j = 0; j < 3; j++)
                    rotate_arb(deg, &z_axis, &co, &ctm_rubix[back[i][j]]);
            break;
        case LEFT:
            for(int i = 0; i < 3; i++)
                for(int j = 0; j < 3; j++)
                    rotate_arb(deg, &x_axis, &co, &ctm_rubix[left[i][j]]);
            break;
        case RIGHT:
            for(int i = 0; i < 3; i++)
                for(int j = 0; j < 3; j++)
                    rotate_arb(deg, &x_axis, &co, &ctm_rubix[right[i][j]]);
            break;
        case TOP:
            for(int i = 0; i < 3; i++)
                for(int j = 0; j < 3; j++)
                    rotate_arb(deg, &y_axis, &co, &ctm_rubix[top[i][j]]);
            break;
        case BOTTOM:
            for(int i = 0; i < 3; i++)
                for(int j = 0; j < 3; j++)
                    rotate_arb(deg, &y_axis, &co, &ctm_rubix[bottom[i][j]]);
            break;
        default:
            break; // HATE
    }
}