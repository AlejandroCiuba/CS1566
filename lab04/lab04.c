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

#include "../matrix library/initShader.h"
#include "../matrix library/matrix_def.h"
#include "../matrix library/matrix_utility.h"
#include "../matrix library/matrix_ops.h"
#include "../matrix library/affine.h"
#include "../matrix library/shapes.h"

#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

GLuint ctm_location;
mat4x4 ctm = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};

vector4* vertices;
vector4* colors;

int num_vertices = 300;

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

void wipeout() {free(vertices); free(colors);}

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

void keyboard(unsigned char key, int mousex, int mousey)
{
    if(key == 'q') {

    	glutLeaveMainLoop();
        wipeout();
        printf("\nEXIT SUCCESSFUL\n");
    }

    //glutPostRedisplay();
}

void reshape(int width, int height)
{
    glViewport(0, 0, 512, 512);
}

//idle global variables
affine size = {1,1,1};
bool grow = true;

GLfloat rot = 0;

void idle() {

    if(grow) {

        size.x += .02;
        size.y += .02;
        size.z += .02;
        if(size.x >= 1)
            grow = 0;
    }
    else {

        size.x -= .02;
        size.y -= .02;
        size.z -= .02;
        if(size.x <= .25)
            grow = 1;
    }

    rot += .5;

    //Create matrices for growing and shrinking
    mat4x4 growth;
    zero_matrix(&growth);
    scal(size, &growth);

    mat4x4 roy;
    zero_matrix(&roy);
    rotate(rot, 'y', &roy);

    mat4x4 roz;
    zero_matrix(&roz);
    rotate(rot, 'z', &roz);

    mat4x4 i;
    zero_matrix(&i);

    //Multiply
    matxmat(&growth, &roy, &i);
    matxmat(&i, &roz, &ctm);
    
    //Create matrices for scaling and rotation
    glutPostRedisplay();
}

int main(int argc, char **argv)
{
    if(argc >= 2) 
        num_vertices = atof(argv[1]);

    //Initialize and establish cone
    vertices = (vector4*) malloc(sizeof(vector4) * num_vertices);
    colors = (vector4*) malloc(sizeof(vector4) * num_vertices);

    //Establish points
    if(argc == 3)
        if(atoi(argv[2]) == 0) {if(cone(vertices, num_vertices, .5, 1, (vector4) {0,1,0,1}, 'y') != 0) return -1;}
        else if(atoi(argv[2]) == 1) {if(circle(vertices, num_vertices, .5, (vector4) {0,0,0,1}, 'z') != 0) return -1;}
        else if(atoi(argv[2]) == 2) {if(flat_taurus(vertices, num_vertices, .1, .5, (vector4) {0,0,0,1}) != 0) return -1;}
        else {
            printf("\nSHAPE NOT RECOGNIZED\n");
            wipeout();
            return -1;
        }
    else
        if(cone(vertices, num_vertices, .5, 1, (vector4) {0,1,0,1}, 'y') != 0) return -1;

    if(random_colors(colors, num_vertices) != 0) return -1;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(512, 512);
    glutInitWindowPosition(100,100);
    glutCreateWindow("lab04");
    glutIdleFunc(idle); //Does this constantly
    glewInit();
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutReshapeFunc(reshape);
    glutMainLoop();

    return 0;
}
