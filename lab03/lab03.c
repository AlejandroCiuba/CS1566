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

int num_vertices = 300;

vector4* vertices;
vector4* colors;

//===================== STUDENT-IMPLEMENTED FUNCTIONS =====================
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

    print_vector(colors[0]);

    return 0;
}

//Assumes triangle-based implementation
ERROR_NUM circle(vector4* vertices, int count, GLfloat radius, vector4 origin, char align) {

    if(vertices == NULL || count == 0 || count % 3 != 0) return MATLIB_POINTER_ERROR;

    //Calculate how many triangles there will be based on the count
    int num_of_triangles = count / 3;

    //Get how many degrees each triangle "bends"
    GLfloat deg_per_triangle = (GLfloat) 360 / num_of_triangles;

    print_GLfloat(deg_per_triangle);
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
                vertices[i * 3].y = (GLfloat) (radius * sin((deg_per_triangle * (i + 1)) * M_PI / 180) + origin.y);
                vertices[i * 3].z = (GLfloat) (radius * cos((deg_per_triangle * (i + 1)) * M_PI / 180) + origin.z);
                vertices[i * 3].w = 1;

                vertices[i * 3 + 1].x = origin.x;
                vertices[i * 3 + 1].y = origin.y;
                vertices[i * 3 + 1].z = origin.z;
                vertices[i * 3 + 1].w = 1.0;

                vertices[i * 3 + 2].x = origin.x;
                vertices[i * 3 + 2].y = (GLfloat) (radius * sin((deg_per_triangle * (i + 1)) * M_PI / 180) + origin.y);
                vertices[i * 3 + 2].z = (GLfloat) (radius * cos((deg_per_triangle * i) * M_PI / 180) + origin.z);
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
        vertices[i * 3 + base_vertices + 1] = tip;
        vertices[i * 3 + base_vertices + 2] = vertices[i * 3 + 2];
    }

    return 0;
}

//===================== TEMPLATE =====================

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
    vector4 tip = {0,1,0,1};
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
