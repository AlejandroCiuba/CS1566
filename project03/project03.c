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
#include "../Catorce/other/array_list.h"
#include "../Catorce/camera/camera.h"

#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

// MAIN PROGRAM HERE SO WE CAN DO ANYTHING WE WANT!!!
GLuint program = -1;

// ===================== UNIFORM VARIABLES =====================
// ctm for manipulating shapes
mat4x4 ctm = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
GLuint ctm_location; // Need for display()

// model_view for camera position
mat4x4 mvm = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
GLuint mvm_location;

// perspective projection for world view
mat4x4 perm = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
GLuint perm_location;

// Texels for texture
GLubyte* texels;
int width = 0, height = 0;
FILE* image = NULL;

// ===================== VERTEX ATTRIBUTES =====================
vector4* vertices;
vector4* colors;
vector2* texcoords;

int num_vertices = 3600;

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

    // Locate and use transformation matrix model_view, we need a separate variable for mvm_location so we can change it in display()
    mvm_location = glGetUniformLocation(program, "mvm");

    // Locate and use transformation matrix perspective, we need a separate variable for perm_location so we can change it in display()
    perm_location = glGetUniformLocation(program, "perm");

    // Location of texture, like location of ctm
    glUniform1i(glGetUniformLocation(program, "texture"), 0);

    printf("\ntexture_location: %i\n", glGetUniformLocation(program, "texture"));
    
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 1.0);
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
    glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &ctm);

    // Allows for affine matrices: location, # of matrices, transpose, pointer to the matrix you want to send
    glUniformMatrix4fv(mvm_location, 1, GL_FALSE, (GLfloat *) &mvm);

    // Allows for affine matrices: location, # of matrices, transpose, pointer to the matrix you want to send
    glUniformMatrix4fv(perm_location, 1, GL_FALSE, (GLfloat *) &perm);

    glDrawArrays(GL_TRIANGLES, 0, num_vertices);

    glutSwapBuffers();
}

void reshape(int width, int height)
{
    glViewport(0, 0, 512, 512);
}
// Spawn-point
vector4 eye = {-10.3687, 1.5, 64.5486, 1};
vector4 look = {-9.3687, 1.5, 64.3577, 1};
vector4 up = {0, 1, 0, 0};

// Walk distance
GLfloat walk = .75;

// Right-Left turn degrees
GLfloat degree_lr = 2.5;

// Animations used for this project
typedef enum {MAP, EXPLORE, WALK_FORWARD, WALK_BACKWARD, WALK_RIGHT, WALK_LEFT, LOOK_UP, LOOK_DOWN, LOOK_RIGHT, LOOK_LEFT, NONE} animations;
animations curr_anim = NONE;


void keyboard(unsigned char key, int mousex, int mousey)
{
    if(key == 'q') quit_program();
    	
    if(key == 'v') for(int i = 0; i < num_vertices; i++) print_vector_ptr(&vertices[i]);

    if(key == 'c') {
        printf("\nMODEL-VIEW MATRIX\n");
        print_matrix(mvm);
        printf("\nEYE\n");
        print_vector(eye);
        printf("\nLOOK\n");
        print_vector(look);
        printf("\nUP\n");
        print_vector(up);
    }
    if(key == 'x') {
        printf("\nPERSPECTIVE MATRIX\n");
        print_matrix(perm);
    }

    // ===================== WALK TRIGGERS =====================
    if(key == 'w') curr_anim = WALK_FORWARD;
    else if(key == 's') curr_anim = WALK_BACKWARD;
    else if(key == 'a') curr_anim = WALK_LEFT;
    else if(key == 'd') curr_anim = WALK_RIGHT;
}

// Used for special keys, arrow keys, and F keys
void special(int key, int x, int y) {

    if(key == GLUT_KEY_RIGHT) curr_anim = LOOK_RIGHT;
    else if(key == GLUT_KEY_LEFT) curr_anim = LOOK_LEFT;
    else if(key == GLUT_KEY_UP) curr_anim = LOOK_UP;
    else if(key == GLUT_KEY_DOWN) curr_anim = LOOK_DOWN;

}

void idle() {

    // ===================== ANIMATIONS =====================
    if(curr_anim == WALK_FORWARD) {
        vector4 temp1, temp2 = zero_vector, temp3 = zero_vector;
        vector_sub((vector4*[2]) {&look, &eye}, 2, &temp1);
        vector_norm(&temp1);
        scalar(&temp1, walk, 0);
        vector_add((vector4*[2]) {&temp1, &eye}, 2, &temp2);
        copy_vector(&temp2, &eye);
        copy_vector(&look, &temp2);
        vector_add((vector4*[2]) {&temp1, &temp2}, 2, &temp3);
        copy_vector(&temp3, &look);
        look_at(&eye, &look, &up, &mvm);
        curr_anim = NONE;
    }
    else if(curr_anim == WALK_BACKWARD) {
        vector4 temp1, temp2 = zero_vector, temp3 = zero_vector;
        vector_sub((vector4*[2]) {&look, &eye}, 2, &temp1);
        vector_norm(&temp1);
        scalar(&temp1, -walk, 0);
        vector_add((vector4*[2]) {&temp1, &eye}, 2, &temp2);
        copy_vector(&temp2, &eye);

        copy_vector(&look, &temp2);
        vector_add((vector4*[2]) {&temp1, &temp2}, 2, &temp3);
        copy_vector(&temp3, &look);
        look_at(&eye, &look, &up, &mvm);
        curr_anim = NONE;
    }
    else if(curr_anim == WALK_RIGHT) {
        vector4 right = look;
        mat4x4 ro = zero_matrix;
        rotate_arb(-90, &up, &eye, &ro);
        matxvec(&ro, &right, &right);
        vector4 temp1, temp2 = zero_vector, temp3 = zero_vector;
        vector_sub((vector4*[2]) {&right, &eye}, 2, &temp1);
        vector_norm(&temp1);
        scalar(&temp1, walk, 0);
        vector_add((vector4*[2]) {&temp1, &eye}, 2, &temp2);
        copy_vector(&temp2, &eye);
        copy_vector(&look, &temp2);
        vector_add((vector4*[2]) {&temp1, &temp2}, 2, &temp3);
        copy_vector(&temp3, &look);
        look_at(&eye, &look, &up, &mvm);
        curr_anim = NONE;
    }
    else if(curr_anim == WALK_LEFT) {
        vector4 left = look;
        mat4x4 ro = zero_matrix;
        rotate_arb(90, &up, &eye, &ro);
        matxvec(&ro, &left, &left);
        vector4 temp1, temp2 = zero_vector, temp3 = zero_vector;
        vector_sub((vector4*[2]) {&left, &eye}, 2, &temp1);
        vector_norm(&temp1);
        scalar(&temp1, walk, 0);
        vector_add((vector4*[2]) {&temp1, &eye}, 2, &temp2);
        copy_vector(&temp2, &eye);
        copy_vector(&look, &temp2);
        vector_add((vector4*[2]) {&temp1, &temp2}, 2, &temp3);
        copy_vector(&temp3, &look);
        look_at(&eye, &look, &up, &mvm);
        curr_anim = NONE;
    }
    else if(curr_anim == LOOK_RIGHT) {
        mat4x4 ro = zero_matrix;
        rotate_arb(-degree_lr, &up, &eye, &ro);
        matxvec(&ro, &look, &look);
        look_at(&eye, &look, &up, &mvm);
        curr_anim = NONE;
    }
    else if(curr_anim == LOOK_LEFT) {
        mat4x4 ro = zero_matrix;
        rotate_arb(degree_lr, &up, &eye, &ro);
        matxvec(&ro, &look, &look);
        look_at(&eye, &look, &up, &mvm);
        curr_anim = NONE;
    }
    else if(curr_anim == LOOK_UP) {
        if(look.y < .995) {
            vector4 VPN = zero_vector;
            vector_sub((vector4*[2]) {&eye, &look}, 2, &VPN);
            vector4 axis = zero_vector;
            vector_cross(&VPN, &up, &axis);
            printf("\nAXIS\n");
            print_vector(axis);
            mat4x4 ro = zero_matrix;
            rotate_arb(degree_lr, &axis, &eye, &ro);
            matxvec(&ro, &look, &look);
            look_at(&eye, &look, &up, &mvm);
        }
        curr_anim = NONE;
    }
    else if(curr_anim == LOOK_DOWN) {
        if(look.y > -.995) {
            vector4 VPN = zero_vector;
            vector_sub((vector4*[2]) {&eye, &look}, 2, &VPN);
            vector4 axis = zero_vector;
            vector_cross(&VPN, &up, &axis);
            mat4x4 ro = zero_matrix;
            rotate_arb(-degree_lr, &axis, &eye, &ro);
            matxvec(&ro, &look, &look);
            look_at(&eye, &look, &up, &mvm);
            curr_anim = NONE;
        }
    }

    glutPostRedisplay();
}

int main(int argc, char **argv)
{   

    // ===================== FILE PARSING =====================
    // .obj file parsing
    FILE* fp = fopen("city/city.obj", "r");
    load_OBJ(fp, &vertices, &num_vertices, &texcoords);
    fclose(fp);

    // .data file parsing
    width = 1024;
    height = 1024;
    image = fopen("city/city.data", "r");
    texels = (GLubyte*) malloc(width * height * 3);
    load_raw(image, texels, width, height);
    fclose(image);
    
    // Perform t = t - 1 for all texcoords (s, t)
    // Specific to this .obj file so I didn't put it in load_OBJ()
    for(int i = 0; i < num_vertices; i++)
        texcoords[i].y = 1 - texcoords[i].y;
    // ===================== POSITION CITY =====================
    // Get the smallest x, y, and z and translate by them
    vector4 small, dummy;

    smallest(vertices, num_vertices, 'x', &dummy);
    small.x = dummy.x;

    smallest(vertices, num_vertices, 'y', &dummy);
    small.y = dummy.y;

    smallest(vertices, num_vertices, 'z', &dummy);
    small.z = dummy.z;

    mat4x4 tra, sc, fin;
    trans((affine){-small.x, -small.y, -small.z}, &tra);
    scal((affine){100, 100, 100}, &sc);
    matxmat(&sc, &tra, &fin);
    
    // Apply to the city
    matxvar(&fin, vertices, num_vertices, vertices);

    // ===================== CREATE GROUND =====================

    // ===================== CHANGE CAMERA LOCATION =====================
    look_at(&eye, &look, &up, &mvm);

    printf("\nMODEL-VIEW MATRIX\n");
    print_matrix(mvm);
    printf("\nEYE\n");
    print_vector(eye);
    printf("\nLOOK\n");
    print_vector(look);
    printf("\nUP\n");
    print_vector(up);

    // ===================== WORLD VIEW =====================
    view world = {-1, 1, 1, -1, -1, -1000};
    perspective(&world, &perm);
    printf("\nPERSPECTIVE MATRIX\n");
    print_matrix(perm);

    printf("VERTEX COUNT: %d\n",  num_vertices);

    // Create program and initialize the viewing windos
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(512, 512);
    glutInitWindowPosition(100,100);
    glutCreateWindow("Project 3");

    // Assign the functions to their jobs
    glutIdleFunc(idle);

    // Start the program and event-driven loop
    glewInit();
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special); // Needed for keyboard
    glutReshapeFunc(reshape);
    glutMainLoop();

    return 0;
}