/*
 * template.c
 *
 * An OpenGL source code template.
 */


#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>
#include <stdio.h>

#include <time.h>
#include <math.h>

#include "../matrix library/initShader.h"
#include "../matrix library/matrix_def.h"
#include "../matrix library/matrix_utility.h"
#include "../matrix library/matrix_ops.h"

#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

/*vector4 vertices[6] =
{{-.5,.5,0,1},{-1,0,0,1},{0,0,0,1},
{.5,.5,0,1},{0,0,0,1},{1,0,0,1}};

vector4 colors[6] =
{{1.0, 0.0, 0.0, 1.0},
{1.0, 0.0, 0.0, 1.0},
{1.0, 0.0, 0.0, 1.0},
{1.0, 0.0, 0.0, 1.0},
{1.0, 0.0, 0.0, 1.0},
{1.0, 0.0, 0.0, 1.0}};*/

vector4 vertices[30];
vector4 colors[30];

int num_vertices = 30;

//===================== STUDENT-IMPLEMENTED FUNCTIONS =====================
void random_colors(vector4* colors, const int num_vertices) {
    
    if(colors == NULL || num_vertices <= 0) return;

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
}

//Assumes triangle-based implementation
void circle(vector4* vertices, int count, GLfloat radius) {

    if(vertices == NULL || count == 0 || count % 3 != 0) return;

    //Calculate how many triangles there will be based on the count
    int num_of_triangles = count / 3;

    //Get how many degrees each triangle "bends"
    GLfloat deg_per_triangle = (GLfloat) 360 / num_of_triangles;

    print_GLfloat(deg_per_triangle);

    for(int i = 0; i < num_of_triangles; i++) {

        vertices[i * 3].x = (GLfloat) (radius * cos((deg_per_triangle * (i + 1)) * M_PI / 180));
        vertices[i * 3].y = (GLfloat) (radius *sin((deg_per_triangle * (i + 1)) * M_PI / 180));
        vertices[i * 3].z = 0;
        vertices[i * 3].w = 1.0;

        vertices[i * 3 + 1].x = 0;
        vertices[i * 3 + 1].y = 0;
        vertices[i * 3 + 1].z = 0;
        vertices[i * 3 + 1].w = 1.0;

        vertices[i * 3 + 2].x = (GLfloat) (radius * cos((deg_per_triangle * i) * M_PI / 180));
        vertices[i * 3 + 2].y = (GLfloat) (radius * sin((deg_per_triangle * i) * M_PI / 180));
        vertices[i * 3 + 2].z = 0;
        vertices[i * 3 + 2].w = 1.0;
    }
}

//===================== TEMPLATE =====================

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
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(colors), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(colors), colors);

    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    GLuint vColor = glGetAttribLocation(program, "vColor");
    glEnableVertexAttribArray(vColor);
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *) sizeof(vertices));

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glDepthRange(1,0);
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_LINE);
    glDrawArrays(GL_TRIANGLES, 0, num_vertices);

    glutSwapBuffers();
}

void keyboard(unsigned char key, int mousex, int mousey)
{
    if(key == 'q')
    	glutLeaveMainLoop();

    //glutPostRedisplay();
}

void reshape(int width, int height)
{
    glViewport(0, 0, 512, 512);
}

int main(int argc, char **argv)
{   
    //Assign points
    circle(vertices, num_vertices, .5);

    //Assign random colors
    random_colors(colors, num_vertices);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(512, 512);
    glutInitWindowPosition(100,100);
    glutCreateWindow("Template");
    glewInit();
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutReshapeFunc(reshape);
    glutMainLoop();

    return 0;
}
