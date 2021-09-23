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
#include "../matrix library/shapes.h"

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

int num_vertices = 300;

vector4* vertices;
vector4* colors;

//===================== LAB03 =====================

void init(void)
{   
    //Start Shader Program
    GLuint program = initShader("vshader.glsl", "fshader.glsl");
    glUseProgram(program);

    //The Attributes We Pass-In Will Be Associated With This Array
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    //Set Aside GPU Memory, Name It GL_ARRAY_BUFFER, Pass-In Vertex Attibutes
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vector4) * 2 * num_vertices, NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vector4) * num_vertices, vertices);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vector4) * num_vertices, sizeof(vector4) * num_vertices, colors);

    //Get vPosition To Point To Where The Beginning Of The Position Attribute Vertex Array Is
    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    //Do The Same With The Beginning Of The Color Attribute Vertex Array
    GLuint vColor = glGetAttribLocation(program, "vColor");
    glEnableVertexAttribArray(vColor);
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *) (sizeof(vector4) * num_vertices));

    //Enablle Hidden Surface Removal, Background Color, And Depth-Range
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glDepthRange(1,0);
}

void display(void)
{
    //Clear The Buffer Each New Canvas
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Front Of Polygons Will Have Color-Fill, Back Only Lines
    //Draw Triangles (3 Vertices Per Triangle), Stop After num_vertices Vertices
    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_LINE);
    glDrawArrays(GL_TRIANGLES, 0, num_vertices);

    //Swap The Drawing Buffer And The Displayed Buffer
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
    /*//Assign origin
    vector4 origin = {0,0,0,1.0};
    //Assign points
    circle(vertices, num_vertices, .75, origin, 'z');*/
    if(argc != 1) {
        num_vertices = atoi(argv[1]);
        if(num_vertices % 3 != 0 || num_vertices < 9) {printf("Not enought vertices\n"); return -1;}
    }

    vertices = (vector4*) malloc(sizeof(vector4) * num_vertices);
    colors = (vector4*) malloc(sizeof(vector4) * num_vertices);

    if(colors == NULL || vertices == NULL) return -1;

    //Assign tip and height
    vector4 tip = {0,.5,0,1};
    GLfloat height = 1;

    //Create cone location vertex array
    if(cone(vertices, num_vertices, .5, height, tip, 'y') != 0) return -1;

    //Assign random colors
    if(random_colors(colors, num_vertices) != 0) return -1;

    //Initializes OpenGL Utility Library
    //Pass fake args so I can use the command line >:)
    char* fake_argc[] = {"my program", NULL};
    int fake_argv = 0;
    glutInit(&fake_argv, fake_argc);

    //Color Type, Double Buffer (Draw & Image), Enable Hidden Surface Removal
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(512, 512);
    glutInitWindowPosition(100,100);
    glutCreateWindow("Template");

    //Initialize OpenGL Main Library Entry Points
    glewInit();

    //User-Made, Defines Vertex Attributes, Uploads Them To GPU Memory, Set-Up Shader Programs
    init();

    //Calls The Passed-In Function Every Time Something In Display Is Changed
    glutDisplayFunc(display);

    //User-Events Triggered When Keyboard Buttons Are Pressed
    glutKeyboardFunc(keyboard);
    glutReshapeFunc(reshape);

    //Starts The Event-Based Loop
    glutMainLoop();

    printf("\nEXIT\n");
    free(vertices);
    free(colors);

    return 0;
}
