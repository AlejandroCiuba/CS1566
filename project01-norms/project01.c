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
#include "../Catorce/camera/lighting.h"

#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

mat4x4 ctm = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
GLuint ctm_location; 

vector4* vertices;
vector4* colors;

int num_vertices = 3600;

FILE* fp = NULL;

void init(void)
{
    GLuint program = initShader("vshader.glsl", "fshader.glsl");
    glUseProgram(program);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vector4) * 2 * num_vertices, NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vector4) * num_vertices, vertices);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vector4) * num_vertices, sizeof(vector4) * num_vertices, colors);

    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    GLuint vColor = glGetAttribLocation(program, "vColor");
    glEnableVertexAttribArray(vColor);
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *) (sizeof(vector4) * num_vertices));

    //Locate and use transformation matrix ctm
    ctm_location = glGetUniformLocation(program, "ctm");

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glDepthRange(1,0);
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_LINE);

    //Allows for affine matrices: location, # of matrices, transpose, pointer to the matrix you want to send
    glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &ctm);

    glDrawArrays(GL_TRIANGLES, 0, num_vertices);

    glutSwapBuffers();
}

void reshape(int width, int height)
{
    glViewport(0, 0, 512, 512);
}

//===================== ROTATION =====================
//Calculate z-Axis point and degree
GLfloat z_treatment(GLfloat x, GLfloat y) {return sqrt(1 - pow(x, 2) - pow(y, 2));}
GLfloat rad_to_degrees(GLfloat rad) {return rad * 180 / M_PI;}

vector4 world_points_1 = {0,0,0,1}, world_points_2 = {0,0,FP_NAN,1}; //Yes...
vector4 center = {0,0,0,1};
mat4x4 final_rot = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};

//====================================================
//Keep the previous ctm to not reset between every call
mat4x4 ctm_base = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};

//Handles zoom in and zoom out
//Global Variables associated with scale
affine s = {1,1,1};
mat4x4 final_scal = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};

void mouse(int button, int state, int x, int y) {

    //===================== SCROLLING SIZE =====================
    mat4x4 sc = identity;

    if(button == 3) s = (affine) {s.x + .02, s.y + .02, s.z + .02};
    else if(button == 4) s = (affine) {s.x - .02, s.y - .02, s.z - .02};
    else s = (affine) {1,1,1};

    //Resize management
    scal(s, &sc);
    copy_matrix(&sc, &final_scal);

    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        screen_to_world(&(vector4){x, y, 0, 1}, &world_points_1, 512, 512, z_treatment);
        if(!isnan(world_points_1.z)) copy_matrix(&ctm, &ctm_base);
    }
    else if(button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        final_rot = identity;
        copy_matrix(&ctm, &ctm_base);
    }
}

//Captures all movement of the mouse when GLUT_LEFT_BUTTON and GLUT_DOWN
void motion(int x, int y) {

    vector4 screen_points = {x, y, 0, 1};
    vector4 cross = {0,0,0,1};
    GLfloat rad = 0.0f;

    screen_to_world(&screen_points, &world_points_2, 512, 512, z_treatment);
        
    //If inside sphere of rotation
    if(!(isnan(world_points_1.z) || isnan(world_points_2.z))) {

        world_points_1.w = 0.0f;
        world_points_2.w = 0.0f;
        vector_norm(&world_points_1);
        vector_norm(&world_points_2);

        vector_cross(&world_points_1, &world_points_2, &cross);
        vector_norm(&cross);
        vector_dot(&world_points_1, &world_points_2, &rad);

        rotate_arb(rad_to_degrees(acos(rad)), &cross, &center, &final_rot);
    } else {final_rot = identity; copy_matrix(&ctm, &ctm_base);}
}

void keyboard(unsigned char key, int mousex, int mousey)
{
    if(key == 'q') {
    	glutLeaveMainLoop();
        free(vertices);
        free(colors);
        if(fp != NULL) fclose(fp);
        printf("\nEXIT SUCCESSFUL\n");
    }

    if(key == 'd') for(int i = 0; i < num_vertices; i++) print_vector_ptr(&colors[i]);

    if(key == 'r') {final_rot = identity; final_scal = identity; ctm_base = identity;}

    //===================== SCROLLING SIZE =====================
    mat4x4 sc = identity;

    //Optional keyboard input
    if(key == '+') s = (affine) {s.x + .02, s.y + .02, s.z + .02};
    else if(key == '-') s = (affine) {s.x - .02, s.y - .02, s.z - .02};

    //Resize management
    scal(s, &sc);
    copy_matrix(&sc, &final_scal);
}

void idle() {
    mat_mult((mat4x4[3]){final_rot, final_scal, ctm_base}, 3, &ctm);
    glutPostRedisplay();
}

int main(int argc, char **argv)
{
    sphere(vertices = (vector4*) malloc(sizeof(vector4) * num_vertices), num_vertices, .5, 16);
    
    //Assign color and print statistics
    //random_colors(colors = (vector4*) malloc(sizeof(vector4) * num_vertices), num_vertices);
    surface_normals(vertices, num_vertices, colors = (vector4*) malloc(sizeof(vector4) * num_vertices));
    printf("COUNT: %d\n",  num_vertices);
    
    //Get center of mass
    com(vertices, num_vertices, &center);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(512, 512);
    glutInitWindowPosition(100,100);
    glutCreateWindow("Project 1");

    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutIdleFunc(idle);

    glewInit();
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutReshapeFunc(reshape);
    glutMainLoop();

    return 0;
}
