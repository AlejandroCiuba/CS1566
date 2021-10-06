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
#include "../matrix library/shapes.h"
#include "../matrix library/affine.h"
#include "../matrix library/file_reader.h"

#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

mat4x4 ctm = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
GLuint ctm_location; 

vector4* vertices;
vector4* colors;

int num_vertices = 360;

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

void keyboard(unsigned char key, int mousex, int mousey)
{
    if(key == 'q') {
    	glutLeaveMainLoop();
        free(vertices);
        free(colors);
        if(fp != NULL) fclose(fp);
        printf("\nEXIT SUCCESSFUL\n");
    }

    if(key == 'd') for(int i = 0; i < num_vertices; i++) print_vector_ptr(&vertices[i]);

    if(key == 'f') if(view_file(fp) != 0) printf("\nNO FILE TO VIEW!!!\n");

    //glutPostRedisplay();
}

void reshape(int width, int height)
{
    glViewport(0, 0, 512, 512);
}

//Handles menu
void menu(char* dec) {

    printf("\n===================== PROJECT 1 =====================\n");
    printf("\nWould you like to view a file (1) or a computer-generated object(2)? ");
    int ui = 0;
    if(scanf("%d", &ui) == EOF) return;

    while(ui != 2 && ui != 1){printf("\nINVALID OPTION!!!"); if(scanf("%d", &ui) == EOF) return;}

    if(ui == 1) {
        printf("\nType the name of the file, located in \"files\": ");
        if(scanf("%s", dec) == EOF) return;
    }
    else strcpy(dec, "2\0");
}

//Handles zoom in and zoom out
//Global Variables associated with scale
affine s = {1,1,1};

void mouse(int button, int state, int x, int y) {

    mat4x4 final; identity(&final);

    //===================== SCROLLING SIZE =====================
    mat4x4 sc; identity(&sc);

    if(button == 3) s = (affine) {s.x + .02, s.y + .02, s.z + .02};
    else if(button == 4) s = (affine) {s.x - .02, s.y - .02, s.z - .02};

    //Resize management
    scal(s, &sc);
    copy_matrix(&sc, &final);

    //===================== ROTATION =====================
    vector4 screen_points = {x, y, 0, 1};
    vector4 world_points; zero_vector(&world_points);

    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        screen_to_world(&screen_points, &world_points, 512, 512);
        print_vector(world_points);
    }

    //Put all changes onto ctm
    copy_matrix(&final, &ctm);

    //Redraw
    glutPostRedisplay();
}

int main(int argc, char **argv)
{
    //Get user decision
    char dec[18];
    menu(dec);

    if(atoi(dec) != 2) { 

        printf("\nFILE NAME: %s\n", dec);

        //Load file
        fp = fopen(strcat((char[24]) {"files/"}, dec), "r");
        if(load_count(fp, &num_vertices) != 0) return -1;
        if(load_va(fp, vertices = (vector4*) malloc(sizeof(vector4) * num_vertices), num_vertices) != 0) return -1;

        //Get the center of mass
        vector4 cm = {0,0,0,0};
        com(vertices, num_vertices, &cm);
        printf("\nCENTER OF MASS:");
        print_vector(cm);

        //Move to origin
        //Scale to fit OpenGL Canonical View
        mat4x4 base_or, shrink, move, final;
        vector4 base[num_vertices];
        for(int i = 0; i < num_vertices; i++) copy_vector(vertices + i, base + i);
        
        rotate(-90, 'x', &base_or);
        scaling(.01, .01, .01, &shrink);
        translate(-1 * cm.x,-1 * cm.y, -1 * cm.z, &move);
        mat_mult((mat4x4[3]) {base_or, shrink, move}, 3, &final);
        matxvar(&final, base, num_vertices, vertices);
    }
    else {
        //torus(vertices = (vector4*) malloc(sizeof(vector4) * num_vertices), num_vertices, 10, .25, .25);
        sphere(vertices = (vector4*) malloc(sizeof(vector4) * num_vertices), num_vertices, 10, .25);
        mat4x4 sc, ro, final;
        vector4 base[num_vertices];
        for(int i = 0; i < num_vertices; i++) copy_vector(vertices + i, base + i);

        scal((affine){.5,.5,.5}, &sc);
        rotate(45, 'x', &ro);
        matxmat(&ro, &sc, &final);
        matxvar(&final, base, num_vertices, vertices);
    }

    //Assign color and print statistics
    random_colors(colors = (vector4*) malloc(sizeof(vector4) * num_vertices), num_vertices);
    printf("COUNT: %d\n",  num_vertices);
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(512, 512);
    glutInitWindowPosition(100,100);
    glutCreateWindow("Project 1");

    glutMouseFunc(mouse);

    glewInit();
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutReshapeFunc(reshape);
    glutMainLoop();

    return 0;
}
